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


#define _MACRO_ONLY

/*
 *	カーネル用のスタートアップモジュール（BLACKfin用）
 */

#include "jsp_kernel.h"

#include <defBF533.h>

			// 割込み、例外からの戻りをリクエストする例外引数
#define RETURNREQ 0xf

#define CORETIMER_CTRL 0xFFE03000

			// 32bit レジスタに即値をロードするマクロ
#ifdef __GNUC__
#define LOADLABEL( reg, value32 ) reg##.h = value32; reg##.l = value32;
#define LOADVALUE( reg, value32 ) reg##.h = ( value32 >> 16 ) &0xFFFF; reg##.l = value32 & 0xFFFF;
#elif defined(__ECC__)
#define LOADVALUE( reg, value32 ) reg##.h = hi(value32); reg##.l = lo(value32);
#define LOADLABEL LOADVALUE
#else
#error "Compiler is not supported"
#endif


#include "saverestore.h"


#ifdef __GNUC__
.section .init;
.extern _boot_for_gdb;
.extern __bss_start, __bss_end;
#elif defined(__ECC__)
.section program;
#else
#error "Compiler is not supported"
#endif

.global start;
.extern _exc_vector;
.extern ldf_stack_end;
.extern _kernel_start;
.extern _reqflg;
.extern expEntry;
.extern nmiEntry;
.extern ivHWEntry;
.extern ivTMREntry;
.extern ivg7Entry;
.extern ivg8Entry;
.extern ivg9Entry;
.extern ivg10Entry;
.extern ivg11Entry;
.extern ivg12Entry;
.extern ivg13Entry;
.extern ivg14Entry;

#ifdef USE_RUNTIME_INIT
.extern _mi_initialize;
#endif

start:
	R0 = 0;
	LC0 = R0;
	LC1 = R0;
	L0 = R0;
	L1 = R0;
	L2 = R0;
	L3 = R0;
    LOADLABEL( p0, ldf_stack_end )
    sp = p0;
    
    LOADVALUE( p0, CORETIMER_CTRL )		// koban 2010.2.2 
    [ p0 ] = r0;						// koban 2010.2.2
    
#ifdef __GNUC__                 // bss領域の初期化
    LOADLABEL( r0, __bss_start) // bss開始番地
    LOADLABEL( r1, __bss_end )
    r2 = r1 - r0;            // bssのサイズ
    p0 = r0;                // 開始アドレス
    p2 = r2;
    r0 = 0;
    loop bssfill lc0 = p2;
loop_begin bssfill
        b[p0++] = r0;
loop_end bssfill;
			// call boot_for_debug
    LOADLABEL( p0, _boot_for_gdb )
    cc = p0 == 0;
    if cc jump skip_boot_for_gdb;
    r0 = 0;			// boot_for_gdbの debug 引数。0のとき、リセットしない。gdbを使うときには、1にする。
    sp += -12;
    call (p0);     // gdbでロードしたときにリセットを確実にするためのコード
    sp += 12;
skip_boot_for_gdb:

#elif defined(__ECC__)
#else
#error "Compiler is not supported"
#endif

#ifdef USE_RUNTIME_INIT			// board_config.h を参照
	sp += -12;
	call _mi_initialize;		// テーブル初期化
	sp += 12;
	cc = r0;					// 返り値をチェック
invalid :
	if cc jump invalid; 		// 非0ならエラー
#endif
					// TOPPERS/JSP for BLACKfinは emu,rst,nmiを管理しない
	LOADVALUE( p0,__EVT0+2*4)	// NMIベクトル
	p1 = 4;


	LOADLABEL( r0, nmiEntry )		// NMI
	[p0++p1] = r0;

	LOADLABEL( r0, expEntry)		// EXP
	[p0++p1] = r0;

	p0 = p0+p1;					// 予約領域をスキップ

	LOADLABEL( r0, ivHWEntry)		// ハードウェアエラー
	[p0++p1] = r0;

	LOADLABEL( r0, ivTMREntry)		// タイマー
	[p0++p1] = r0;

	LOADLABEL( r0, ivg7Entry)		// IVG7
	[p0++p1] = r0;

	LOADLABEL( r0, ivg8Entry)		// IVG8
	r0.H = ivg8Entry;
	[p0++p1] = r0;

	LOADLABEL( r0, ivg9Entry)		// IVG9
	[p0++p1] = r0;

	LOADLABEL( r0, ivg10Entry)		// IVG10
	[p0++p1] = r0;

	LOADLABEL( r0, ivg11Entry)		// IVG11
	[p0++p1] = r0;

	LOADLABEL( r0, ivg12Entry)		// IVG12
	[p0++p1] = r0;

	LOADLABEL( r0, ivg13Entry)		// IVG13
	[p0++p1] = r0;

	LOADLABEL( r0, ivg14Entry)		// IVG14
	[p0++p1] = r0;

	LOADLABEL( r0, task_level)		// IVG15はタスクに遷移するためのエントリー
	[p0++p1] = r0;

	LOADLABEL( p2, user)				// ダミーの戻り番地を設定
	reti = p2;
	csync;
	raise	15;					// IVG15を生起する。割り込みは遅延発生する。

	r0 = 0xC01F(z);				// IVG15を受付可能に(CPUロック状態)
	sti r0;
	csync;
	rti;						// ユーザーモードへ遷移する

task_level:                     // ここからタスクの実行順位
    r0 = syscfg;
    r1 = 0x6;                   // セルフ・ネスティング割込み及びCYCLESカウンタ
    r0 = r0 | r1;
    syscfg = r0;                // セルフ・ネストを有効に

    sp += -12;
#ifdef __GNUC__					/* GCC */
#elif defined(__ECC__)			/* VisualDSP++ */
#ifdef INIT_C_PLUS_PLUS
    r0 = 0;
    r1 = 0;
    call.x ___ctorloop;
#endif
#else
#error "Compiler is not supported"
#endif

    [--sp] = reti;				// 戻り番地を捨てて割り込み可能にする
    jump.x _kernel_start;			// kernel_stgart()からは戻ってこない
user:
	jump	user;
start.end:







#ifdef __GNUC__
#elif defined(__ECC__)

#ifdef INIT_C_PLUS_PLUS
.section .gdt;
        .align 4;
___eh_gdt:
.global ___eh_gdt;
        .byte4=0;
.type ___eh_gdt,STT_OBJECT;
.section .frt;
        .align 4;
___eh_frt:
.global ___eh_frt;
        .byte4=0;
.type ___eh_frt,STT_OBJECT;
___ctor_table:
	.align 4;	
	.byte4=0;
___ctor_table.end:
____ctor_table:
.section ctor;
	.align 4;
.global ___ctor_table;
.type ___ctor_table,STT_OBJECT;
.extern ___ctorloop;
#endif  /* INIT_C_PLUS_PLUS */

#else
#error "Compiler is not supported"
#endif

