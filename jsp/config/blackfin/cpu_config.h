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
 *	プロセッサ依存のカーネル資源（BLACKfin用）
 *
 *  このインクルードファイルは，t_config.h のみからインクルードされる．
 *  他のファイルから直接インクルードしてはならない．
 */

#ifndef _CPU_CONFIG_H_
#define _CPU_CONFIG_H_

/*
 *  サービスコール選択マクロの読み込み
 */
#ifdef __GNUC__
/*
* サービスコール選択は、configureスクリプトがおこなうので、gnu環境では不要
*/
//#include <api.h>
#elif defined(__ECC__)
/*
* サービスコール選択は、IPAが行うので、VisualDSP++環境ではすべて選択しておく
*/
#include <api.h>
#else
#error "Compiler is not supported"
#endif

/*
*  イベントプライオリティ宣言. sys_config.h の device_dispatcher()で使う。
*/
#ifndef ik_timer
#define ik_timer 6
#endif

#ifndef ik_hardware
#define ik_hardware_err 5
#endif

/*
 *  カーネルの内部識別名のリネーム
 */
#include <cpu_rename.h>

/*
*	BLACKfinのビットマップ検索機能を使う
*   BLACKfinはMSBから数える。
*/
#define	CPU_BITMAP_SEARCH
#define	PRIMAP_BIT(pri)		(0x40000000u >> (pri))


/*
 *  TCB 中のフィールドのビット幅の定義
 *
 *  cpu_context.h に入れる方がエレガントだが，参照の依存性の関係で，
 *  cpu_context.h には入れられない．
 */
#define	TBIT_TCB_TSTAT		8	/* tstat フィールドのビット幅 */
#define	TBIT_TCB_PRIORITY	8	/* priority フィールドのビット幅 */


/*
*	BLACKfinのIMASKへの設定パターン
*
*/
#define IMASK_LOCK   0xc01f
#define IMASK_UNLOCK 0xffff

#ifndef _MACRO_ONLY
/*
 *  プロセッサの特殊命令のインライン関数定義
 */
/*
 *  レディキューサーチのためのビットマップサーチ関数
 *
 *  ビットマップの下位16ビットを使用し，最下位ビットを最低優先度に対応
 *  させる．
 */


Inline UINT
bitmap_search(UINT bitmap)
{
	UINT	numOfSign;

	/*
	 *  このコードは，bitmap（UINT型）が32ビットであることを仮定し
	 *  ている．Signbits 命令は32ビット整数を取ってMSBから連続する
	 *  符号ビット - 1 を返す。all 0のときは31を返す。LSBのみが1の
	 *  ときは30を返す。
	 */
#ifdef __GNUC__			// gcc
	// gcc 3.4.1 の時点では、オペランドに下位ハーフレジスタを指定
	// できないため、余計なコードが必要になる。
	Asm( "r0.L=signbits %1; %0 = r0.L;": "=d"(numOfSign) : "d"(bitmap) : "R0" );
#elif defined(__ECC__)	// visualdsp
	Asm( "%0=signbits %1;": "=l"(numOfSign) : "d"(bitmap) );
#else
#error "Compiler is not supported"
#endif
	return( numOfSign );
}



/*
 *  タスクコンテキストブロックの定義
 */
typedef struct task_context_block {
	VP	sp;		/* スタックポインタ */
	FP	pc;		/* プログラムカウンタ */
} CTXB;


/*
 *  システム状態参照
 *	返す値は以下のとおり
 *	TRUE : 非タスクコンテキスト
 *	FALSE: タスクコンテキスト
 *				IPENDが$8000か$8010の時、タスクコンテキストである。それ以外のときは非タスクコンテキスト。
 */

Inline BOOL
sense_context(void)
{
	return( *__pIPEND & 0x7FEF );
}

/*
* 現在のロック状態を調べる。管理外割り込みまでロックするsil_loc_int()に
* よるロック状態は、このsense_lock()では無視する。
*/
Inline BOOL
sense_lock(void)
{
#ifdef UNMANAGED_INT
	return((*__pIMASK & ~UNMANAGED_INT )== 0xC01F );
#else
	return(*__pIMASK == 0xC01F );
#endif
}

#define t_sense_lock	sense_lock
#define i_sense_lock	sense_lock


/*
 *  CPUロックとその解除（タスクコンテキスト用）
 *
 *  task_intmask は，chg_ipm をサポートするための変数．chg_ipm をサポー
 *  トしない場合には，task_intmask が常に 0 になっていると考えればよい．
 *
 */

#ifdef SUPPORT_CHG_IPM
extern UH	task_intmask;	/* タスクコンテキストでの割込みマスク */
#endif /* SUPPORT_CHG_IPM */

#ifdef UNMANAGED_INT
#if UNMANAGED_INT & 0xC01F
// 管理外割り込みの範囲チェック。管理外割り込みはIVHWからIVG13の間でなければならない
#error "Illegal un-managed interrupt : must be IVHW-IVG13"
#endif
#endif


// BLACKfinではIMASKに0xC01Fを書き込んでCPUをロックする。
Inline void
t_lock_cpu()
{
#ifdef UNMANAGED_INT
	unsigned int imask;
	Asm( "cli %0;" :"=d"(imask) );
	Asm( "sti %0;" : :"d"(0xC01f | (imask & UNMANAGED_INT) ) );
#else
    Asm( "cli r0;" : : :"R0" );
	Asm( "sti %0;" : :"d"(0xC01F) );
#endif
}

Inline void
t_unlock_cpu()
{
#ifdef UNMANAGED_INT
	unsigned int imask;
	Asm( "cli %0;" :"=d"(imask) );
	Asm( "sti %0;" : :"d"( (0xffff & ~UNMANAGED_INT) | (imask & UNMANAGED_INT) ) );
#else
	Asm( "sti %0;" : :"d"(0xffff) );
#endif
}

/*
 *  CPUロックとその解除（非タスクコンテキスト用）
 */


Inline void
i_lock_cpu()
{
#ifdef UNMANAGED_INT
	unsigned int imask;
	Asm( "cli %0;" :"=d"(imask) );
	Asm( "sti %0;" : :"d"(0xC01f | (imask & UNMANAGED_INT) ) );
#else
    Asm( "cli r0;" : : :"R0" );
	Asm( "sti %0;" : :"d"(0xC01F) );
#endif
}

Inline void
i_unlock_cpu()
{
#ifdef UNMANAGED_INT
	unsigned int imask;
	Asm( "cli %0;" :"=d"(imask) );
	Asm( "sti %0;" : :"d"( (0xffff & ~UNMANAGED_INT) | (imask & UNMANAGED_INT) ) );
#else
	Asm( "sti %0;" : :"d"(0xffff) );
#endif
}

/*
 *  タスクディスパッチャ
 */

/*
 *  最高優先順位タスクへのディスパッチ（cpu_support.S）
 *
 *  dispatch は，タスクコンテキストから呼び出されたサービスコール処理
 *  内で，CPUロック状態で呼び出さなければならない．
 */
extern void	dispatch(void);

/*
 *  現在のコンテキストを捨ててディスパッチ（cpu_support.S）
 *
 *  exit_and_dispatch は，CPUロック状態で呼び出さなければならない．
 */
extern void	exit_and_dispatch(void);

/*
 *  割込みハンドラ／CPU例外ハンドラの設定
 */


/*
 *  割込みハンドラの設定
 *
 *  ベクトル番号 inhno の割込みハンドラの起動番地を inthdr に設定する．
 * evtvector はstart.asmで定義されている。
 */
extern void (* dev_vector[])(void) ;

extern void (* exc_vector)(VP) ;
Inline void
define_inh(INHNO inhno, FP inthdr)
{
	dev_vector[inhno] = inthdr;
}

/*
 *  CPU例外ハンドラの設定
 *
 *  ベクトル番号 excno のCPU例外ハンドラの起動番地を exchdr に設定する．
 */
Inline void
define_exc(EXCNO excno, FP exchdr)
{
//	exc_vector[excno] = exchdr;		// 例外ハンドラを配列に格納
	exc_vector = (void (*)(VP))exchdr;		// 例外ハンドラを格納
}

/*
 *  割込みハンドラ／CPU例外ハンドラの出入口処理
 */

/*
 *  割込みハンドラの出入口処理の生成マクロ
 *
 *  reqflg をチェックする前に割込みを禁止しないと，reqflg をチェック後
 *  に起動された割込みハンドラ内でディスパッチが要求された場合に，ディ
 *  スパッチされない．
 */

#define	INTHDR_ENTRY(inthdr)	extern void inthdr(void);
#define	INT_ENTRY(inthdr)	inthdr

/*
 *  CPU例外ハンドラの出入口処理の生成マクロ
 *
 *  reqflg をチェックする前に割込みを禁止しないと，reqflg をチェック後
 *  に起動された割込みハンドラ内でディスパッチが要求された場合に，ディ
 *  スパッチされない．
 */

#define	EXCHDR_ENTRY(exchdr)	extern void exchdr( VP );
#define	EXC_ENTRY(exchdr)	exchdr

/*
 *  CPU例外の発生した時のシステム状態の参照
 */

/*
 *  CPU例外の発生した時のコンテキストの参照
 */
Inline BOOL
exc_sense_context(VP p_excinf)
{
	return( *__pIPEND & 0x7FE6 );				// IVG15, GIE, EXP, EMU 以外のビットが立っていれば非タスクコンテキスト
}

/*
 *  CPU例外の発生した時のCPUロック状態の参照。管理外割り込みの状態は無視する。
 */
Inline BOOL
exc_sense_lock(VP p_excinf)
{
#ifdef UNMANAGED_INT
	return((*__pIMASK | UNMANAGED_INT) != 0xFFFF );		// 0xFFFFならアンロック
#else
	return(*__pIMASK != 0xFFFF );				// 0xFFFFならアンロック
#endif
}

/*
 *  プロセッサ依存の初期化
 */
extern void	cpu_initialize(void);

/*
 *  プロセッサ依存の終了時処理
 */
extern void	cpu_terminate(void);

#endif /* _MACRO_ONLY */
#endif /* _CPU_CONFIG_H_ */
