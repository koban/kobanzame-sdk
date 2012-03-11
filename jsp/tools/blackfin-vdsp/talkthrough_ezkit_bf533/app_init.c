#include "t_services.h"
#include "talkthrough.h"
#include "kernel_id.h"
#include <cdefBF53x.h>

#define pFLASHA_PORTA_OUT ((volatile unsigned char * )0x20270004)
#define pFLASHA_PORTA_DIR ((volatile unsigned char * )0x20270006)
#define pFLASHA_PORTB_OUT ((volatile unsigned char * )0x20270005)
#define pFLASHA_PORTB_DIR ((volatile unsigned char * )0x20270007)
#define pLED pFLASHA_PORTB_OUT

//------------------------------------------------
// AD1836a 用のID設定
//
//------------------------------------------------
//const int codec_rx_dtq = CODEC_RX_DTQ;
//const int codec_tx_dtq = CODEC_TX_DTQ;
const int spi_sem = SPI_SEM;
const int sport0_sem =SPORT0_SEM;
//------------------------------------------------



void app_init(VP_INT vp_int)
{
    *pEBIU_AMBCTL0 = 0x7bb07bb0;  // 非同期ポート0,1の初期化
    *pEBIU_AMBCTL1 = 0x7bb07bb0;  // 非同期ポート2,3の初期化
    *pEBIU_AMGCTL  = 0x000f;      // 全非同期ポートをイネーブル

    *pFLASHA_PORTB_DIR = 0x3f;    // bit0-5を出力に
    *pFLASHA_PORTB_OUT = 0x00;    // LED オフ	
}
