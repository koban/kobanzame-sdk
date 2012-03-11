#include "t_services.h"
#include "s_services.h"
#include <cdefBF533.h>
#include "ad1836a.h"



#define pFLASHA_PORTA_OUT ((volatile unsigned char * )0x20270004)
#define pFLASHA_PORTA_DIR ((volatile unsigned char * )0x20270006)


struct DMA_DESCRIPTOR{
	struct DMA_DESCRIPTOR * next;
	void * start;
	unsigned short config;
	unsigned short x_count;
	short x_modify;
};


// CODEC 初期化データ列
static short sCodec1836TxRegs[] =
{									
					DAC_CONTROL_1	| 0x000,
					DAC_CONTROL_2	| 0x000,
					DAC_VOLUME_0	| 0x3ff,
					DAC_VOLUME_1	| 0x3ff,
					DAC_VOLUME_2	| 0x3ff,
					DAC_VOLUME_3	| 0x3ff,
					DAC_VOLUME_4	| 0x3ff,
					DAC_VOLUME_5	| 0x3ff,
					ADC_CONTROL_1	| 0x000,
					ADC_CONTROL_2	| 0x180,
					ADC_CONTROL_3	| 0x000
					
};

// オーディオバッファ
// 1サンプルあたり8データを転送する。ダブルバッファ構成なのでさらにその2倍必要
static struct CODEC_BUFFER TxBuffer[2], RxBuffer[2];

static struct DMA_DESCRIPTOR tDescA, tDescB, rDescA, rDescB;

void run_ad1836_task(VP_INT extinf)
{
	ena_int( INTNO_SPI ); 			// SPI DMA割り込みマスクをはずす 
	ena_int( INTNO_SPORT0_RX );		// SPORT0 DMA割り込みマスクをはずす 

//--------------------------------------------------------
// 			AD1836のリセット 
//
    *pFLASHA_PORTA_DIR |= 0x01;   	// CODEC リセット信号を出力に	
    *pFLASHA_PORTA_OUT &= 0xFE;   	// CODEC リセットをアサート	
	tslp_tsk( 1 );					// リセット=Lパルスを作る 
    *pFLASHA_PORTA_OUT |= 0x01;    	// CODEC リセットをデアサート 
	tslp_tsk( 1 );					// リセットからの回復時間を与える 

//--------------------------------------------------------
//			SPI設定 
// 	
	// PF4はAD1836のSPI SSピン 
	*pSPI_FLG = FLS4;
	// SPIビットレート = SCLK/(2*SPIBAUD) :およそ 2MHz	
	*pSPI_BAUD = 32;
	// SPI DMA 書き込みによるトリガ, 16bit データ, MSB first, マスタ
	*pSPI_CTL = TIMOD | SIZE | MSTR;

	
	
//--------------------------------------------------------
//			 SPI DMA 設定 
//
	// 16bit データ、メモリから読み出し、終了後割り込み
	*pDMA5_CONFIG = WDSIZE_16 | DI_EN;
	// CODEC 初期化文字列のアドレス 
	*pDMA5_START_ADDR = sCodec1836TxRegs;
	// DMA 転送カウント 
	*pDMA5_X_COUNT = sizeof( sCodec1836TxRegs ) / sizeof( short );
	// DMA 増分 
	*pDMA5_X_MODIFY = sizeof( short );
	

//--------------------------------------------------------
//			 SPI 開始
	// DMA 開始 
	*pDMA5_CONFIG |= DMAEN;
	// SPI 開始 
	*pSPI_CTL |= SPE;

	
	// DMA送出終了を待つ 
	wai_sem( spi_sem );
	// DMA終了はSPI送信終了ではないので、ちょっと待つ 
	tslp_tsk( 1 );

//--------------------------------------------------------
//		SPORT0 設定 
//			マルチチャンネルの場合、常に SPORTx_yCR1.LATFS = 0
	// Sport0 受信設定 
	// 外部クロック, 外部同期信号, MSBファースト 
	// 32-bit データ 
	*pSPORT0_RCR1 = RFSR;
	*pSPORT0_RCR2 = 31;		// データ長 32
	
	// Sport0 送信設定
	// 外部クロック, 外部同期信号, MSBファースト 
	// 32ビットデータ 
	*pSPORT0_TCR1 = TFSR;
	*pSPORT0_TCR2 = 31;		// データ長 32 
	
	// フレームあたり8スロットのデータを送受で使う 
	*pSPORT0_MTCS0 = 0x000000FF;
	*pSPORT0_MRCS0 = 0x000000FF;
	
	// マルチチャンネル設定
	*pSPORT0_MCMC1 = 0x0000; // オフセット = 0, ウインドウサイズ = 8
	*pSPORT0_MCMC2 = 0x1000 | MCMEN | MCDRXPE | MCDTXPE; // MFD = 1;

//--------------------------------------------------------
// 			SPORT DMA 設定 
//
	// DMA1設定 (SPORT0 RX)
	rDescA.next = &rDescB;
	rDescA.start = RxBuffer[0].data;
	rDescA.config = 0x7700 | WNR | WDSIZE_32 | DI_EN | DMAEN;	// Large List, 7 short word.
	rDescA.x_count = 8 * SAMPLE_PER_INTR;
	rDescA.x_modify = 4;
	
	rDescB.next = &rDescA;
	rDescB.start = RxBuffer[1].data;
	rDescB.config = 0x7700 | WNR | WDSIZE_32 | DI_EN | DMAEN;	// Large List, 7 short word.
	rDescB.x_count = 8 * SAMPLE_PER_INTR;
	rDescB.x_modify = 4;
	
	
	// DMA2設定 (SPORT0 TX)
	tDescA.next = &tDescB;
	tDescA.start = TxBuffer[0].data;
	tDescA.config = 0x7700 | WDSIZE_32 | DMAEN;	// Large List, 7 short word.
	tDescA.x_count = 8 * SAMPLE_PER_INTR;
	tDescA.x_modify = 4;
	
	tDescB.next = &tDescA;
	tDescB.start = TxBuffer[1].data;
	tDescB.config = 0x7700 | WDSIZE_32 | DMAEN;	// Large List, 7 short word.
	tDescB.x_count = 8 * SAMPLE_PER_INTR;
	tDescB.x_modify = 4;


	*pDMA2_NEXT_DESC_PTR = &tDescA;
	*pDMA1_NEXT_DESC_PTR = &rDescA;
	*pDMA2_CONFIG = 0x7700 | WDSIZE_32;
	*pDMA1_CONFIG = 0x7700 | WNR | WDSIZE_32;
	
	
//--------------------------------------------------------
//			 SPORT0 開始
	// DMA 開始 
	*pDMA2_CONFIG |= DMAEN;
	*pDMA1_CONFIG |= DMAEN;
	asm("ssync;");
	// SPORT2 開始 
	*pSPORT0_TCR1 |= TSPEN;
	*pSPORT0_RCR1 |= RSPEN;

//

	while( 1 ){
		int sample, slot, bufTx, bufRx;
		struct CODEC_BUFFER * BufToBeTransmit; 
	
				// 受信DMA終了割り込みと同期
		wai_sem( sport0_sem);
	
				// プロセッサが使ってよいバッファを割り出す	
		bufTx = ( &tDescA == *pDMA2_NEXT_DESC_PTR) ? 0 : 1;
		bufRx = ( &rDescA == *pDMA1_NEXT_DESC_PTR) ? 0 : 1;
				
		
		processData(&RxBuffer[bufRx], &TxBuffer[bufTx]);
				
	}

}

