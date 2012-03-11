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
 *	ターゲットシステムに依存する定義（ADSP-BF534,6,7用）
 *
 *  このインクルードファイルは，kernel.h と sil.h でインクルードされる．
 *  他のファイルから直接インクルードすることはない．このファイルをイン
 *  クルードする前に，t_stddef.h と itron.h がインクルードされるので，
 *  それらに依存してもよい．
 */

#ifndef _CHIP_DEFS_H_
#define _CHIP_DEFS_H_

/*
 * システム依存部用のMMR定義
 */
#define __PLL_CTL			0xFFC00000
#define __PLL_DIV			0xFFC00004

#define __SWRST			0xFFC00100
#define __SYSCR			0xFFC00104
#define __SIC_IMASK		0xFFC0010C
#define __SIC_IAR0			0xFFC00110
#define __SIC_IAR1			0xFFC00114
#define __SIC_IAR2			0xFFC00118
#define __SIC_IAR3			0xFFC0011C
#define __SIC_ISR			0xFFC00120
#define __SIC_IWR			0xFFC00124

#define __TIMER7_CONFIG     0xFFC00670
#define __TIMER7_COUNTER    0xFFC00674
#define __TIMER7_PERIOD     0xFFC00678
#define __TIMER7_WIDTH      0xFFC0067C

#define __TIMER_ENABLE		0xFFC00680
#define __TIMER_DISABLE	0xFFC00684
#define __TIMER_STATUS		0xFFC00688

#define __EBIU_SDBCTL		0xFFC00A14

#define __PORTF_FER		0xFFC03200
#define __PORTG_FER		0xFFC03204
#define __PORTH_FER		0xFFC03208
#define __PORT_MUX			0xFFC0320C


#define __pPLL_CTL 		((volatile unsigned short *)__PLL_CTL)
#define __pPLL_DIV 		((volatile unsigned short *)__PLL_DIV)

#define __pSWRST 			((volatile unsigned short *)__SWRST)
#define __pSYSCR 			((volatile unsigned short *)__SYSCR)
#define __pSIC_IMASK 		((volatile unsigned long *)__SIC_IMASK)
#define __pSIC_IAR0 		((volatile unsigned long *)__SIC_IAR0)
#define __pSIC_IAR1 		((volatile unsigned long *)__SIC_IAR1)
#define __pSIC_IAR2 		((volatile unsigned long *)__SIC_IAR2)
#define __pSIC_IAR3 		((volatile unsigned long *)__SIC_IAR2)
#define __pSIC_ISR 		((volatile unsigned long *)__SIC_ISR)
#define __pSIC_IWR 		((volatile unsigned long *)__SIC_IWR)

#define __pTIMER7_CONFIG 	((volatile unsigned short *)__TIMER7_CONFIG)
#define __pTIMER7_COUNTER ((volatile unsigned long *)__TIMER7_COUNTER)
#define __pTIMER7_PERIOD 	((volatile unsigned long *)__TIMER7_PERIOD)
#define __pTIMER7_WIDTH 	((volatile unsigned long *)__TIMER7_WIDTH)

#define __pTIMER_ENABLE 	((volatile unsigned short *)__TIMER_ENABLE)
#define __pTIMER_DISABLE 	((volatile unsigned short *)__TIMER_DISABLE)
#define __pTIMER_STATUS 	((volatile unsigned long  *)__TIMER_STATUS)

#define __pEBIU_SDBCTL 	((volatile unsigned short *)__EBIU_SDBCTL)

#define __pPORTF_FER		((volatile unsigned short *)__PORTF_FER)
#define __pPORTG_FER		((volatile unsigned short *)__PORTG_FER)
#define __pPORTH_FER		((volatile unsigned short *)__PORTH_FER)
#define __pPORT_MUX		((volatile unsigned short *)__PORT_MUX)


/*
 *  タイムティックの定義
 */
#define	TIC_NUME	1u	/* タイムティックの周期の分子 */
#define	TIC_DENO	1u	/* タイムティックの周期の分母 */

/*
 *  破壊性読み出しレジスタの判別式
 *  与えられたアドレス iop が破壊性読み出しレジスタの
 *  場合には真を返す。
 */
#ifndef DESTRUCTIVE_READ
#define DESTRUCTIVE_READ( iop ) (iop<(VP)0xF0000000)
#endif

#ifndef _MACRO_ONLY

/*
 *  システムの停止処理
 */

Inline void
kernel_abort()
{
}

#endif /* _MACRO_ONLY */
#endif /* _SYS_DEFS_H_ */
