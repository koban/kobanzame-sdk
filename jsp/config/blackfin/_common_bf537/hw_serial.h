/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *
 *  上記著作権者は，以下の (1)〜(4) の条件か，Free Software Foundation
 *  によって公表されている GNU General Public License の Version 2 に記
 *  述されている条件を満たす場合に限り，本ソフトウェア（本ソフトウェア
 *  を改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，その適用可能性も
 *  含めて，いかなる保証も行わない．また，本ソフトウェアの利用により直
 *  接的または間接的に生じたいかなる損害に関しても，その責任を負わない．
 *
 */

/*
 *  シリアルI/Oデバイス（SIO）ドライバ（Blackfin UART用）
 *
 *   このファイルは、TOPPERS/JSP 1.4.2の config/sh3/ms7727cp01/hw_serial.hを
 *   リネームし、内部の識別子を変更したものである。
 *
 *   このファイルは下位のデバイス・アクセス・ルーチンを束ねて上位の層に渡す
 *   ためのものである。Blackfinの外部にあるシリアルをTOPPERS/JSPで管理しない
 *   のなら、変更する必要はない。
 */

#ifndef _HW_SERIAL_H_
#define _HW_SERIAL_H_

#include <s_services.h>
#include <sil.h>

#include "uart.h"


/*
 *  SIOドライバの初期化ルーチン
 */
#define sio_initialize  uart_initialize

#ifndef _MACRO_ONLY
/*
 *  シリアルI/Oポートのオープン
 */
Inline SIOPCB *
sio_opn_por(ID siopid, VP_INT exinf)
{
    SIOPCB  *siopcb;
    UW regBase;

    /*
     *  デバイス依存のオープン処理．
     */
    siopcb = uart_opn_por(siopid, exinf);

	/*
	* Enable Corrensponding Interrupt at IMASK
	*/
    regBase = siopcb->siopinib->reg_base;
    if ( regBase == UART0_ADDRESS ) {        // UART0の場合
	    ena_int( INTNO_UART0_TX );
	    ena_int( INTNO_UART0_RX );
        *__pPORT_MUX  &= ~0x0008;   // 機能をUART0に割り振る
        *__pPORTF_FER |=  0x0003;   // PF0,PF1をUART0に割り振る
    }
    else if ( regBase == UART1_ADDRESS ){    // uart1の場合
        ena_int( INTNO_UART1_TX );
        ena_int( INTNO_UART1_RX );
        *__pPORT_MUX  &= ~0x0010;   // 機能をUART0に割り振る
        *__pPORTF_FER |=  0x000C;   // PF2,PF3をUART0に割り振る
    }

    return(siopcb);
}

/*
 *  シリアルI/Oポートのクローズ
 */
Inline void
sio_cls_por(SIOPCB *siopcb)
{
	/*
	 * ここではSIC_IMASKの対応ビットをクリアしない。SIC_IMASKのクリアは
	 * 危険である。UARTからの割り込みは uart_cls_por内部で禁止するので
	 * 充分である。
	 */

    /*
     *  デバイス依存のクローズ処理．
     */
    uart_cls_por(siopcb);
}


/*
 *  SIOの割込みハンドラ
 */
#define sio0_rx_handler uart0_rx_isr
#define sio0_tx_handler uart0_tx_isr
#define sio1_rx_handler uart1_rx_isr
#define sio1_tx_handler uart1_tx_isr

/*
 *  シリアルI/Oポートへの文字送信
 */
#define sio_snd_chr uart_snd_chr

/*
 *  シリアルI/Oポートからの文字受信
 */
#define sio_rcv_chr uart_rcv_chr

/*
 *  シリアルI/Oポートからのコールバックの許可
 */
#define sio_ena_cbr uart_ena_cbr

/*
 *  シリアルI/Oポートからのコールバックの禁止
 */
#define sio_dis_cbr uart_dis_cbr

/*
 *  シリアルI/Oポートからの送信可能コールバック
 */
#define sio_ierdy_snd   uart_ierdy_snd

/*
 *  シリアルI/Oポートからの受信通知コールバック
 */
#define sio_ierdy_rcv   uart_ierdy_rcv

#endif /* _MACRO_ONLY */
#endif /* _HW_SERIAL_H_ */
