/**
 *	@file	kzdev_audio.c
 *	@brief	KOBANZAME SDK Audio Driver
 *
 *	KOBANZAME SDK
 *		Software Developers Kit for Blackfin DSP Evaluation Board(KOBANZAME).
 *		
 *	Copyright (C) 2010, KOBANZAME SDK Project, all right reserved
 *
 *	LICENSE:
 *	The software is a free and you can use and redistribute it for
 *	personal, non-profit or commercial products. Redistributions of
 *	source code must retain the above copyright notice. There is no 
 *	warranty in this software, if you suffer any damages by using 
 *	the software.
 *
 *	@author	Analog Devices Inc.
 *	@author 	Suikan
 *	@note		This code is copied and modified from VisualDSP++5.0 Audio Codec Talkthrough
 *				and AudioFrameWorkBF533 by Suikan.
 */

	 
#include "kobanzame.h"
#include "jsp_kernel.h"

#if TARGET_TOOL == TOOL_VDSP || TARGET_TOOL == TOOL_GCC
#include <fract.h>
#endif

static ID		mTskDevAudio;

/** 割り込み１回あたりに転送する、サンプル数。
*
* ユーザーが変えてよい。
*/
#define		SAMPLES_PER_INTR		(8)

/** サンプルあたりのスロット数。
*
* AD1836Aのマルチチャンネル・モードでは8。ユーザーが変えてはならない。
*/
#define		SLOT_PER_SAMPLE			(8)

/** 過去何回分の割り込みのデータを用意するか。
*
* トリプルバッファなので3
*/
#define 	INTR_PER_BUFFER			(3)
#define		FLOW_Autobuffer			(0x1000)

#define		INTERNAL_ADC_L0			(0)
#define		INTERNAL_ADC_L1			(1)
#define		INTERNAL_ADC_R0			(4)
#define		INTERNAL_ADC_R1			(5)
#define		INTERNAL_DAC_L0			(0)
#define		INTERNAL_DAC_L1			(1)
#define		INTERNAL_DAC_L2			(2)
#define		INTERNAL_DAC_R0			(4)
#define		INTERNAL_DAC_R1			(5)
#define		INTERNAL_DAC_R2			(6)

#define		NUM_BUF_AUDIO_STATE		(2)
#define		QSHIFT32				(31 - KZQ_VAL)

static BOOL				mbStarted = FALSE;
static KzAudioCbk_t		mfnCbk = 0;
static int				mnBlocks = 0;
static int				miTxBuffer1[INTR_PER_BUFFER][SAMPLES_PER_INTR][SLOT_PER_SAMPLE];	// Need place for each sample
static int				miRxBuffer1[INTR_PER_BUFFER][SAMPLES_PER_INTR][SLOT_PER_SAMPLE];
static int				mnAudioBufState1;
static int				miRxBuffer2[NUM_BUF_AUDIO_STATE*KZAUDIO_MAX_AUDIO_NBLOCKS*KZ_AUDIOCLUSTAR];
static int				miTxBuffer2[NUM_BUF_AUDIO_STATE*KZAUDIO_MAX_AUDIO_NBLOCKS*KZ_AUDIOCLUSTAR];
static int				mnAudioBufState2;
static int				mnAudioBufInnerMaxCount2;
static int				mnAudioBufInnerCount2;
static int				mnAudioChannels;

const long				mSupportedFreq[] = { 48000 };

void		KzAttIniAudio(ID idTskDevAudio)
{
	mTskDevAudio = idTskDevAudio;
}

void		KzAudioISRSport0(void)
{
	int i;
	int *rx,*tx;
	
	*pDMA1_IRQ_STATUS = 0x0001;
		
	if (  2 == *pDMA2_CURR_Y_COUNT )
		mnAudioBufState1 = 0;  // トリプルバッファの最初でfield変数を同期する
	else
	    mnAudioBufState1 ++;   // それ以外では決めうちで更新

	rx = &miRxBuffer2[mnAudioBufState2*mnBlocks*mnAudioChannels + mnAudioBufInnerCount2*mnAudioChannels ];
	tx = &miTxBuffer2[mnAudioBufState2*mnBlocks*mnAudioChannels + mnAudioBufInnerCount2*mnAudioChannels ];
	
	for( i=0 ; i<SAMPLES_PER_INTR; ++i )
	{
		/* RX */
		*rx++ = miRxBuffer1[mnAudioBufState1][i][INTERNAL_ADC_L0];
		if( mnAudioChannels >=2 )
			*rx++ = miRxBuffer1[mnAudioBufState1][i][INTERNAL_ADC_R0];
		if( mnAudioChannels >=3 )
			*rx++ = miRxBuffer1[mnAudioBufState1][i][INTERNAL_ADC_L1];
		if( mnAudioChannels >=4 )
			*rx++ = miRxBuffer1[mnAudioBufState1][i][INTERNAL_ADC_R1];
		
		/* TX */
		miTxBuffer1[mnAudioBufState1][i][INTERNAL_DAC_L0] = *tx++;
		if( mnAudioChannels >=2 )
			miTxBuffer1[mnAudioBufState1][i][INTERNAL_DAC_R0] = *tx++;
		if( mnAudioChannels >=3 )
			miTxBuffer1[mnAudioBufState1][i][INTERNAL_DAC_L1] = *tx++;
		if( mnAudioChannels >=4 )
			miTxBuffer1[mnAudioBufState1][i][INTERNAL_DAC_R1] = *tx++;
	}	
		
	mnAudioBufInnerCount2+=SAMPLES_PER_INTR;
	if( mnAudioBufInnerCount2 >= mnBlocks )
	{
		mnAudioBufInnerCount2 = 0;
		mnAudioBufState2 = (mnAudioBufState2>=NUM_BUF_AUDIO_STATE-1)? 0 : mnAudioBufState2 + 1;
		iwup_tsk( mTskDevAudio );
	}
}

void		KzAudioDspTask(VP_INT arg)
{	
	int i;
	int preState2;
	int *tx,*rx;
	int *tmp;
	for(;;)
	{
		slp_tsk();

		preState2 = (mnAudioBufState2==0)? NUM_BUF_AUDIO_STATE-1 : mnAudioBufState2 -1;
		
		/* prepare rx buffer */
		rx = &miRxBuffer2[ preState2 * mnBlocks * mnAudioChannels ];
		tmp = rx; for( i=0;i<mnBlocks*mnAudioChannels;++i) *tmp++ >>= QSHIFT32;
		
		/* prepare tx buffer */
		tx = &miTxBuffer2[ preState2 * mnBlocks * mnAudioChannels ];
		tmp = tx; for( i=0;i<mnBlocks*mnAudioChannels;++i) *tmp++ = 0L;
		
		/* call callback */
		if( mfnCbk ) mfnCbk( rx, tx, mnBlocks ,mnAudioChannels);
		
		/* post tx buffer */
		tmp = tx;
		for( i=0;i<mnBlocks*mnAudioChannels;++i,++tmp) 
		{
#if TARGET_TOOL == TOOL_VDSP || TARGET_TOOL == TOOL_GCC
			*tmp = shl_fr1x32( *tmp , QSHIFT32 );
#else
#error "Please define TARGET_TOOL"
#endif	
		}
	}
}

static BOOL setup_audio(long lSampleRate, int nBlocks, int nChannel )
{
	int i;
	int *p;
	/* check value */
	for(i=0;i<numof(mSupportedFreq);++i )
		if( mSupportedFreq[i] == lSampleRate ) break;
	if( i >= numof(mSupportedFreq) ) return FALSE;	/* invalid frequency */
	
	if( nBlocks < SAMPLES_PER_INTR || nBlocks > KZAUDIO_MAX_AUDIO_NBLOCKS ) return FALSE;	/* overflow nBlocks */
	
	if( nBlocks % SAMPLES_PER_INTR ) return FALSE;		/* SAMPLES_PER_INTRの倍数でない */
	
	if( nChannel < 0 || nChannel > KZ_AUDIOCLUSTAR ) return FALSE;	/* overflow channels */
	
	/* setup value */
	mnAudioBufState1 = 2;
	mnAudioBufState2 = 0;
	mnAudioBufInnerMaxCount2 = nBlocks / SAMPLES_PER_INTR;
	mnAudioBufInnerCount2 = 0;
	mnBlocks = nBlocks;
	p=&miRxBuffer1[0][0][0];for(i=0;i<numof( miRxBuffer1 );++i) *p++ = 0;
	p=&miTxBuffer1[0][0][0];for(i=0;i<numof( miTxBuffer1 );++i) *p++ = 0;
	p=miRxBuffer2;for(i=0;i<numof( miRxBuffer2 );++i) *p++ = 0;
	p=miTxBuffer2;for(i=0;i<numof( miTxBuffer2 );++i) *p++ = 0;
	mnAudioChannels = nChannel;
	
	/* Init AD1836 */
	kzdev_codec_start_DSPMODE( lSampleRate );
	
	/* Init SPORT */
	#define SLEN_32	0x001F
	#define SLEN_24	0x0017
	#define FLOW_1	0x1000
	{
    	// Sport0 receive configuration
    	// External CLK, External Frame sync, MSB first
    	// 32-bit data
    	*pSPORT0_RCR1 = RFSR;
    	*pSPORT0_RCR2 = SLEN_32;
	
    	// Sport0 transmit configuration
    	// External CLK, External Frame sync, MSB first
    	// 24-bit data
    	*pSPORT0_TCR1 = TFSR;
    	*pSPORT0_TCR2 = SLEN_32;
	
    	// Enable MCM 8 transmit & receive channels
    	*pSPORT0_MTCS0 = 0x000000FF;
    	*pSPORT0_MRCS0 = 0x000000FF;
	
    	// Set MCM configuration register and enable MCM mode
    	*pSPORT0_MCMC1 = 0x0000;
    	*pSPORT0_MCMC2 = 0x101c;
	}
    	
	/* Init DMA */
	{
    	int i, field;
	
    	// Set up DMA1 to receive
    	// Map DMA1 to Sport0 RX
    	*pDMA1_PERIPHERAL_MAP = 0x1000;
	
    	// Configure DMA1
    	// 32-bit transfers, Interrupt on completion, Autobuffer mode
    	*pDMA1_CONFIG = WNR | WDSIZE_32 | DI_EN | DI_SEL | FLOW_Autobuffer | DMA2D ;
    	// Start address of data buffer
    	*pDMA1_START_ADDR = (void *)miRxBuffer1;
    	// DMA inner loop count
    	*pDMA1_X_COUNT = SLOT_PER_SAMPLE * SAMPLES_PER_INTR;
    	// Inner loop address increment
    	*pDMA1_X_MODIFY	= sizeof(int);
    	// DMA outer loop count, triple buffer
    	*pDMA1_Y_COUNT = INTR_PER_BUFFER;
    	// Outer loop address increment
    	*pDMA1_Y_MODIFY	= sizeof(int);	
	
    	// Set up DMA2 to transmit
    	// Map DMA2 to Sport0 TX
    	*pDMA2_PERIPHERAL_MAP = 0x2000;
	
    	// Configure DMA2
    	// 32-bit transfers, Autobuffer mode
    	*pDMA2_CONFIG = WDSIZE_32 | FLOW_Autobuffer | DMA2D;
    	// Start address of data buffer
    	*pDMA2_START_ADDR = (void *)miTxBuffer1;
    	// DMA inner loop count
    	*pDMA2_X_COUNT = SLOT_PER_SAMPLE * SAMPLES_PER_INTR;
    	// Inner loop address increment
    	*pDMA2_X_MODIFY	= sizeof(int);
    	// DMA outer loop count, triple buffere
    	*pDMA2_Y_COUNT = INTR_PER_BUFFER;
    	// Outer loop address increment
    	*pDMA2_Y_MODIFY	= sizeof(int);
	 
	}
	
	
	return TRUE;
}
static void start_audio(void)
{
	/* Start SPORT 0 */
	ena_int( INHNO_SPORT0_RX );
	
	// enable DMAs
	*pDMA2_CONFIG	= (*pDMA2_CONFIG | DMAEN);
	*pDMA1_CONFIG	= (*pDMA1_CONFIG | DMAEN);

	// enable Sport0 TX and RX
	*pSPORT0_TCR1 	= (*pSPORT0_TCR1 | TSPEN);
	*pSPORT0_RCR1 	= (*pSPORT0_RCR1 | RSPEN);

}

static void stop_audio(void)
{
	// disable Sport0 TX and RX
	*pSPORT0_TCR1 	= 0;
	*pSPORT0_RCR1 	= 0;
	// disable DMAs
	*pDMA2_CONFIG	= 0;
	*pDMA1_CONFIG	= 0;
	
	/* wait a while */
	dly_tsk(10);
	
	/* status clear */
	*pDMA1_IRQ_STATUS = 0x0001;
		
}

BOOL		kzdev_audio_start(void *fnCbk, long lSampleRate, int nBlocks, int nChannel )
{
	if( mbStarted ) return FALSE;
	mbStarted = TRUE;
	
	if( setup_audio( lSampleRate, nBlocks, nChannel ) )
	{
			
		mfnCbk = (KzAudioCbk_t) fnCbk;
	
		start_audio();
		
		return TRUE;
	}
	else
	{
		mbStarted = FALSE;
		mfnCbk = 0;
		mnBlocks = 0;
		return FALSE;
	}
}

void		kzdev_audio_stop(void)
{
	if( mbStarted )
	{		
		stop_audio();		
		mfnCbk = 0;
		mnBlocks = 0;
		mbStarted = FALSE;
	}
}



