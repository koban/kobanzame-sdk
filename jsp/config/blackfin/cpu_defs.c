/*
 *  TOPPERS/JSP for Blackfin
 *
 *  Copyright (C) 2004,2006,2006 by Takemasa Nakamura
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
// DESTRUCTIVE_READはsys_defs.hで定義する。 

// 破壊性読み出しレジスタにアクセスする場合は
// 割り込みロックをかける

#include <s_services.h>

#define __SIL_LOCK_INTERRUPT( x ) \
	if (DESTRUCTIVE_READ(iop)){\
		SIL_PRE_LOC;\
		SIL_LOC_INT();\
		x;\
		SIL_UNL_INT();\
	}\
	else\
		x;

 VB sil_reb_iop(VP iop)
{
	VH temp;
//	PROLOGUE
	
    __SIL_LOCK_INTERRUPT( temp = sil_reh_mem((VP)iop) );

//	EPILOGUE;
    return temp;
}

 VH sil_reh_iop(VP iop)
{
	VH temp;

    __SIL_LOCK_INTERRUPT( temp = sil_reh_mem((VP)iop) );

    return temp;
}

 VW sil_rew_iop(VP iop)
{
	VW temp;

    __SIL_LOCK_INTERRUPT( temp = sil_rew_mem((VP)iop) );

    return temp;
}


 VH sil_reh_lep(VP iop)
{
	VH temp;
	
    __SIL_LOCK_INTERRUPT( temp = sil_reh_lem((VP)iop) );

    return temp;
}


 VW sil_rew_lep(VP iop)
{
	VW temp;
	
    __SIL_LOCK_INTERRUPT( temp = sil_rew_lem((VP)iop) );

    return temp;
}

 VH sil_reh_bep(VP iop)
{
	VH temp;
	
    __SIL_LOCK_INTERRUPT( temp = sil_reh_bem((VP)iop) );

    return temp;
}

 VW sil_rew_bep(VP iop)
{
	VW temp;
	
    __SIL_LOCK_INTERRUPT( temp = sil_rew_bem((VP)iop) );

    return temp;
}

