/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *
 *  TOPPERS/JSP for Blackfin
 *
 *  Copyright (C) 2004,2006,2006 by Takemasa Nakamura
 *  Copyright (C) 2004 by Ujinosuke
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
 *
 */

/*
 *	タイマドライバ（ADSP-BF537用）
 *
 */

#ifndef _HW_TIMER_H_
#define _HW_TIMER_H_

#include <s_services.h>
#include "sil.h"

#ifndef _MACRO_ONLY



//#include <ccblkfn.h>




/**************************************************************
 *  タイマの起動処理
 *
 *  タイマを初期化し，周期的なタイマ割込み要求を発生させる．
 *
 **************************************************************/
Inline void
hw_timer_initialize()
{
#ifdef USE_TIC_CORE
	// Core timer
	// TCNTL bit assignement
	// TMPWR                  0x00000001
	// TMREN                  0x00000002
	// TAUTORLD               0x00000004
	// TINT                   0x00000008
	*__pTCNTL = 0x00000001;					// TMPWR : power up timer;
	*__pTPERIOD = CORECLOCK/1000;			// CORE CLOCK is defined in sys_config.h
	*__pTCNTL = 0x00000007;					// TMPWR | TMREN | TAUTORLD;
#else
	// GPT7(General-purpose timer7)
		// PWM_OUT			0x0001
		// WDTH_CAP			0x0002
		// EXT_CLK			0x0003
		// PULSE_HI			0x0004
		// PERIOD_CNT		0x0008
		// IRQ_ENA			0x0010
		// TIN_SEL			0x0020
		// OUT_DIS			0x0040
		// CLK_SEL			0x0080
		// TOGGLE_HI		0x0100
		// EMU_RUN			0x0200
		// ERR_TYP			0xC000
	*__pTIMER7_CONFIG = 0x0059;// PERIOD_CNT | OUT_DIS |IRQ_ENA | PWM_OUT;	// PWM_OUT, Output Pad disable
	*__pTIMER7_PERIOD = SYSCLOCK/1000;	// SYS CLOCK is defined in sys_config.h
	*__pTIMER7_WIDTH = 1;					// 0 < width < period
		// TIMEN0			0x0001
		// TIMEN1			0x0002
		// TIMEN2			0x0004
		// TIMEN3			0x0008
		// TIMEN4			0x0010
		// TIMEN5			0x0020
		// TIMEN6			0x0040
		// TIMEN7			0x0080
	*__pTIMER_ENABLE =  0x0080; //TIMEN7;			// timer7 start
	ena_int( INHNO_TIMER );				// enable Timer Interrupt

#endif
	asm("ssync;");

}

/**************************************************************
 *  タイマ割込み要求のクリア
 **************************************************************/
Inline void
hw_timer_int_clear()
{
#ifdef USE_TIC_CORE
	// Core timer
	// TCNTL bit assignement
	// TMPWR                  0x00000001
	// TMREN                  0x00000002
	// TAUTORLD               0x00000004
	// TINT                   0x00000008
	*__pTCNTL = 0x0000000F;	// TMPWR | TMREN | TAUTORLD | TINT;
#else
	// GPT7(General-purpose timer7)
		// TIMIL7			0x00080000
	*__pTIMER_STATUS =  0x00080000; // TIMIL7;
#endif
	asm("ssync;");
}

/**************************************************************
 *  タイマの停止処理
 *
 *  タイマの動作を停止させる．
 **************************************************************/
Inline void
hw_timer_terminate()
{
	/*
	 *  タイマの動作を停止する．
	 */
#ifdef USE_TIC_CORE
	// Core timer
	// TCNTL bit assignement
	// TMPWR                  0x00000001
	// TMREN                  0x00000002
	// TAUTORLD               0x00000004
	// TINT                   0x00000008
	*__pTCNTL = 0x0000000D;	//TMPWR | TAUTORLD | TINT;		// 停止
	*__pTCNTL = 0;							// パワーダウンモード
#else
	// GPT2(General-purpose timer7)
		// TIMDIS7			0x0080
	*__pTIMER_DISABLE = 0x0080; // TIMDIS7;			// timer7 disable
#endif
	asm("ssync;");
}


#endif /* _MACRO_ONLY */
#endif /* _HW_TIMER_H_ */
