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
 *	プロセッサに依存するアプリケーション用資源の定義（BLACKfin用）
 *
 *  このインクルードファイルは，kernel.h と sil.h でインクルードされる．
 *  他のファイルから直接インクルードすることはない．このファイルをイン
 *  クルードする前に，t_stddef.h と itron.h がインクルードされるので，
 *  それらに依存してもよい．
 */

#ifndef _CPU_DEFS_H_
#define _CPU_DEFS_H_

#define BLACKFIN			/* プロセッサ略称 */

/*
 * Core MMR
 */

#define __IMASK	0xFFE02104
#define __IPEND	0xFFE02108
#define __EVT0     0xFFE02000

#define __TCNTL    0xFFE03000
#define __TPERIOD  0xFFE03004


#define __pIMASK ((volatile unsigned long *)__IMASK)
#define __pIPEND ((volatile unsigned long *)__IPEND)
#define __pEVT0 ((volatile void * volatile) __EVT0)

#define __pTCNTL ((volatile unsigned long *)__TCNTL)
#define __pTPERIOD ((volatile unsigned long *)__TPERIOD)
/*
 *  システム・インターフェース・レイヤー
 */

/*
 *  プロセッサのエンディアン
 */
#define	SIL_ENDIAN	SIL_ENDIAN_LITTLE	/* リトルエンディアン */


/*
* 割り込みロック制御
*/

#define SIL_PRE_LOC		UINT _intmask_
#define SIL_LOC_INT()	asm volatile ( "cli %0;": "=d"(_intmask_) )
#define SIL_UNL_INT()	asm volatile ( "sti %0;": :"d"(_intmask_) )





#ifndef _MACRO_ONLY

/*
 *  割込み／CPU例外ハンドラ番号の型
 */
typedef	UINT	INHNO;		/* 割込みハンドラ番号 */
typedef	UINT	EXCNO;		/* CPU例外ハンドラ番号 */



/*
 *  微少時間待ち
 *  システム依存部で SIL_DLY_TIM1とSIL_DLY_TIM2を定義しておく。
 *  SIL_DLY_TIM1はsil_dly_nseの呼び出しオーバーヘッド(nS)
 *  SIL_DLY_TIM2はsil_dly_nseのループの長さ(nS)
 */
void sil_dly_nse(UINT dlytim);



/*
 *  性能評価用システム時刻の参照
 *  cycleカウンタから64ビットの値を取り出す。
 */
typedef	long long	SYSUTIM;	/* 性能評価用システム時刻 */

Inline  ER	vxget_tim(SYSUTIM *p_sysutim)
{
	unsigned int lo, hi;

	Asm( "%0=CYCLES; %1=CYCLES2;" :"=d"(lo), "=d"(hi) );
	((unsigned int *)p_sysutim)[0] = lo;
	((unsigned int *)p_sysutim)[1] = hi;
	return(0);
}

/*
* システム・メモリ・アクセス
*/

#define sil_wrb_iop(iop, data)	sil_wrh_mem( iop, (UB)data )
#define sil_wrh_iop	sil_wrh_mem
#define sil_wrw_iop	sil_wrw_mem
#define sil_wrh_lep sil_wrh_lem
#define sil_wrw_lep sil_wrw_lem
#define sil_wrh_bep sil_wrh_bem
#define sil_wrw_bep sil_wrw_bem

VB sil_reb_iop(VP iop);
VH sil_reh_iop(VP iop);
VW sil_rew_iop(VP iop);
VH sil_reh_lep(VP iop);
VW sil_rew_lep(VP iop);
VH sil_reh_bep(VP iop);
VW sil_rew_bep(VP iop);


#endif /* _MACRO_ONLY */




#endif /* _CPU_DEFS_H_ */
