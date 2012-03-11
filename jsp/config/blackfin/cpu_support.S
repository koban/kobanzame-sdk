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
 *	プロセッサ依存モジュール アセンブリ言語部（BLACKfin用）
 *  Copyright (C) 2004,2006,2006 by Takemasa Nakamura
 */

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



#define	_MACRO_ONLY
#include "jsp_kernel.h"
#include "offset.h"

/*
 *  タスクディスパッチャ
 *
 *  dispatch は，割込み禁止状態で呼び出さなければならな
 *  い．exit_and_dispatch も，割込み禁止状態で呼び出す。
 *  p0 : &runtsk
 *  p1 : &schedtsk
 *  p2 : runtsk
 *  p3 : schedtsk
 *  p4 : &reqflg
 */
#ifdef __GNUC__
.section .text;
#elif defined(__ECC__)
.section program;
#else
#error "Compiler is not supported"
#endif

.global _dispatch;
.global _exit_and_dispatch;
.extern ldf_stack_end;
.extern _reqflg;
.extern _call_texrtn;
.extern _runtsk, _schedtsk;
#ifdef __GNUC__
.type _dispatch, STT_FUNC;
#elif defined(__ECC__)
#else
#error "Compiler is not supported"
#endif
_dispatch:
	[--sp] = (r7:4, p5:3);
	[--sp] = rets;
	[--sp] = fp;
	LOADLABEL( p0,  _runtsk )
	LOADLABEL( p1,  _schedtsk)
	p2 = [p0];					// p2 を runtsk に
	[p2+TCB_sp] = sp;			// タスクスタックを保存
	LOADLABEL( r1, dispatch_r )	// 実行再開番地を保存
	[p2+TCB_pc] = r1;
			// スケジュールされたタスクを取り出す
dispatcher:
	r0 = [p1];
	[p0] = r0;					// runtsk = schedtsk
	cc = r0;
	if !cc jump dispatcher_1;	// runtskが無ければ割り込み待ちに。
	p3 = r0;					// p3はschedtsk
	sp = [p3+TCB_sp];			// タスクスタック復帰
	p5 = [p3+TCB_pc];			// 実行再開番地復帰
	jump (p5);					// 実行再開番地へ飛ぶ

			// ready タスクがない場合の割込み待ち
dispatcher_1:
	LOADLABEL( r0, ldf_stack_end)	// イベントスタックポインタを設定
	sp = r0;
	LOADLABEL( p4, _reqflg )
wait_for_intr:
	csync;
	raise 14;					// 割り込み待ち状態に移行
	csync;
	/*
	 *  割り込み待ちはIVG14の割り込み状態で行う。ここでIVG14に遷移するのは
	 *  どのスタックを使うかという問題の解決と，割込みハンドラ内で
	 *  のタスクディスパッチの防止という2つの意味がある．
	 *
	 *  割込みを待つ間は，runtsk を NULL（=0）に設定しなければなら
	 *  ない．このように設定しないと，割込みハンドラから iget_tid
	 *  を呼び出した際の動作が仕様に合致しなくなる．
	 *
	 *  割り込み待ち状態はidleになるため、実際には割り込みではなくwakeup
	 *  イベントが捕捉され、それに伴う割り込みを処理することになる。
	 *  したがって割り込み待ちをおこなうためには、対応するSIC_IWRのビットを
    *  1にしておかなければならない。
	 *  これはアプリケーションプログラマの責任で行う。
	 */
	r0 = [p4];					// reqflag取得
	cc = r0;
	if !cc jump wait_for_intr;	// reqflgが0なら割り込み待ち
	r0 = 0;
	[p4] = r0;					// reqflgをクリア
	jump dispatcher;


			// タスクの再開番地。タスク例外の処理をおこない、
			// dispatch()から戻る
			// このアドレスへは必ず dispatcherから飛んでくる。
			// したがって、p3は必ずschedtskの内容になっている。
dispatch_r:
	fp = [sp++];
	r2 = [p3+TCB_texptn];		// schedtsk->texptn
	cc = r2 == 0;				// texptrnが0ならcc => 1
//	r0 = [p3+TCB_enatex];
//	LOADVALUE( r1, TCB_enatex_mask )
	r0 = b[p3+TCB_enatex](z);
	r1 = TCB_enatex_mask;
	r0 = r0 & r1;				// enatexが0なら、az => 1
	cc |= az;					// cc = ( !enatex || ! texptn ) = !( enatex && texptn )
	if cc jump dispatch_r_1(bp);	// if ( !enatex || !texptn ) 即リターン
	sp += -12;					// ダミー引数領域確保
	call _call_texrtn;
	sp += 12;					// ダミー引数領域開放
dispatch_r_1:
	rets = [sp++];
	(r7:4, p5:3) = [sp++];
	rts;
_dispatch.end:

#ifdef __GNUC__
.size	_dispatch, .-_dispatch
#elif defined(__ECC__)
#else
#error "Compiler is not supported"
#endif


/*
 *	TOPPERS/JSP開始処理:
 *	 start.asm は kernel_startをCPUロック状態、タスク優先順位で呼ぶ。
 *	 kernel_startはそのままexit_and_dispatchを呼ぶ。
 *	 _exit_and_dispatchは、それまでのスタック状態などを単に廃棄し、
 *	 スケジュールされたタスクを開始する。
*/
_exit_and_dispatch:
		// dispatcher以降で使うポインタを設定
	LOADLABEL( p0,  _runtsk )
	LOADLABEL( p1,  _schedtsk)
	jump dispatcher;
_exit_and_dispatch.end:

/*
 *  タスク起動時処理
 *  _activate_rへはかならずdispatcherから飛んでくる。そのため、CPUロックを
 * はずさなければならない。
 */

	.global _activate_r;
_activate_r:
	r1 = 0xffff(z);
    cli r0;         // 現在のマスクを取り出す。
                        // 管理外割り込みを使わないときにも、割り込み禁止のためcliが必要
#ifdef UNMANAGED_INT
	r2 = UNMANAGED_INT(z);
	r0 = r0 & r2;				// 管理外割り込みのみ取り出す
	r2 = ~r2;					// 逆マスク
	r1 = r1 & r2;				//
	r1 = r1 | r0;				// 管理外割り込みを考慮したマスク
#endif
	sti r1;				// CPU アンロック
	p0 = [sp++];		// タスクエントリーを取り出す
	r0 = [sp++];		// ext_tskの番地を取り出す
	rets = r0;			// ext_tskをダミーの戻り番地にする
	r0 = [sp++];		// ext_infを取り出す : r0 は引数1
	sp += -12;
	jump (p0);			// タスクの開始
_activate_r.end:


/*
*	割込み処理のエントリ
*
*	interrupt_handlerへはivgXXEntryからジャンプしてくる。p0レジスタには
*	割り込みレベル番号が格納されている。
*	割り込みとのネスト関係を調べ、ネストしていないならイベントスタックをセットする。
*		p2 : reqflgのアドレス
*		r6 : タスクスタック
*		r7 : ASTATの退避情報
*
*/

#include "saverestore.h"

.global interrupt_dispatcher;
.global task_context;
.extern _device_dispatcher, _enadsp;
.extern _event_fp;

#ifdef __GNUC__
.type interrupt_dispatcher, STT_FUNC;
#elif defined(__ECC__)
#else
#error "Compiler is not supported"
#endif
interrupt_dispatcher:
				// save all task context register
	save_regs;					// ユーザーレジスタの退避
	r0 = 0;
	l0 = r0;
	l1 = r0;
	l2 = r0;
	l3 = r0;
	lc0 = r0;					// 性能低下抑制のためにLCxをクリア
	lc1 = r0;
	LOADVALUE( p1, __IPEND )			// IPENDレジスタのアドレス
	r0 = [p1];					// IPENDの値を取得
	r1 = 0x7fef;				//
	r0 = r0 & r1;				// IVG15とGID以外を調べる
	r0.L = ones r0;				// 処理中のイベントの数を数える
	r0 += -1;					// イベントの数-1。ネストしていなければ0になる。
	cc = az;					// ネストしてなければ真
	if !cc jump call_interrupt;	// もしネストしていればハンドラを呼ぶ
								// ネストしていない割り込みなら、スタックを切り替える
	r6 = sp;					// タスクスタックポインタを退避
	LOADLABEL( r0, ldf_stack_end)	// イベントスタックポインタを設定
	sp = r0;

call_interrupt:
	cli r1;						// IMASKの古い値を第二パラメータに
	[--sp] = reti;				// ここで割り込み可能になる
	r7 = astat;					// ccにはネスト情報が入っている。それを退避
	r0 = p0;					// 割り込み順位を第一パラメータに
	LOADLABEL( p0, _device_dispatcher)
	sp += -12;					// ダミー引数確保 (A)
	call	(p0);				// C言語で書かれたデバイスディスパッチャを呼ぶ
	sp += 12;					// ダミー引数開放 (A)
	astat = r7;					// ccを復帰
	reti = [sp++];				// ここで再び割り込み禁止
	if !cc jump get_back;		// もしネストしているならば、このまま帰る

								// ここでは割り込みはネストしていない
	sp = r6;					// スタックをタスクスタックに設定

	LOADLABEL( p2, _reqflg)			// reqflgのアドレス
	r0 = [p2];					// reqflgの値を取得
	cc = r0;					// ディスパッチかタスク例外の要求は出ているか
	if !cc jump get_back(bp);	// 出ていないならば割り込まれたタスクに戻る

								// コンテキスト切り替え作業開始
	r0 = 0;						//
	[p2] = r0;					// reqflgをクリアする.
	r0 = reti;					// タスクの戻り番地を取り出す
	r1 = 1;
	r0 = r1 | r0; 				// セルフ・ネストであると偽装する
	[--sp] = r0;				// 戻り番地をスタックにつむ
	LOADLABEL( p0, task_context)

	cli r0;						// 現在のCPUロック状態を取得
	[--sp] = r0;				// ロック状態を退避

	r1 = 0xC01F(z);				// タスクコンテキストははCPUロック状態で実行する
#ifdef UNMANAGED_INT
    r2 = UNMANAGED_INT(z);
    r0 = r0 & r2;				// 管理外割り込みのマスク状態を抽出
    r1 = r0 | r1;				// タスクコンテキストはCPUロック状態で実行する
#endif
	sti r1;						// CPUロック (B)

	reti = p0;					// ラベル"task_context"を割り込みからの戻り番地にする
	rti;						// 割り込みから戻る。コンテキストはタスクになる
	csync;
task_context:					// ここはタスクコンテキストで、CPUロック状態である
	LOADLABEL( p0, _enadsp)
	LOADLABEL( p1, _runtsk)


	r2 = [p0];					// load enadsp
	cc = r2;					// ディスパッチ可能か？
	if !cc jump go_tex;			// もしディスパッチ禁止なら例外チェックに

	LOADLABEL( p0, _schedtsk)		// ディスパッチ可能の場合
	r1 = [p1];					// runtsk
	r0 = [p0];					// schedtsk
	cc = r0 == r1;				// schedtsk == runtsk　か？
	if cc jump go_tex(bp);		// もし schedtsk == runtsk　ならば、タスク例外に行く
								// そうでなければディスパッチする
	sp += -12;					// ダミー引数領域確保 (C)
	call _dispatch;				// レッツゴー
	sp += 12;					// ダミー引数領域開放 (C)
	jump return_as_task;		// タスクに戻る

go_tex:
	p1 = [p1];					// runtsk
	r0 = [p1 + TCB_texptn];		// runtsk->texptrnを取得 ( UINT )
	cc = r0 == 0;				// texptnが0なら1
//	r0 = [p1 + TCB_enatex];		// runtsk->enatexを取得
//	LOADVALUE( r1, TCB_enatex_mask)
	r0 = b[p1 + TCB_enatex](z);		// runtsk->enatexを取得
	r1 = TCB_enatex_mask;
	r1 = r0 & r1;				// タスク例外が許可されていなければ1
	cc |= az;					// 例外が許可されていないか、texptnが0ならば即帰る
	if cc jump return_as_task(bp);	// 許可されていなければタスクに戻る
	sp += -12;					// ダミー引数領域確保 (D)
	call _call_texrtn;			// 条件がそろったのでタスク例外処理を行う。
	sp += 12;					// ダミー引数領域開放 (D)

return_as_task:					// タスクコンテキスト状態での戻り
	r0 = [sp++];				// 退避していたロック状態
	reti = [sp++];				// 戻り番地を復帰。以後rtiまで割込み禁止
    cli r1;               // 現在のロック状態を取り出す
                                // 管理外割り込みを使わない場合もこのcliは保護のため必要
#ifdef UNMANAGED_INT
	r2 = UNMANAGED_INT(z);
	r1 = r1 & r2;				// 実行中のマスクから管理外割り込みの状態を抽出
	r2 = ~ r2;					// UNMANAGED_INTの逆マスク
	r0 = r0 & r2;				// 退避中のロック状態から管理外割り込みマスクをクリア
	r0 = r0 | r1;				// 退避していたロック状態をアップデート
#endif
	sti r0;						// CPUロックを解除(B)(F)


get_back:						// 非タスクコンテキスト状態での戻り
	restore_regs;				// 退避したレジスタを全て戻す
	p0=[sp++];
	rti;						// タスクに戻る


#ifdef __GNUC__
.size	interrupt_dispatcher, .-interrupt_dispatcher
#elif defined(__ECC__)
interrupt_dispatcher.end:
#else
#error "Compiler is not supported"
#endif


.global expEntry
.global expEntry;
.global nmiEntry;
.global ivHWEntry;
.global ivTMREntry;
.global ivg7Entry;
.global ivg8Entry;
.global ivg9Entry;
.global ivg10Entry;
.global ivg11Entry;
.global ivg12Entry;
.global ivg13Entry;
.global ivg14Entry;
#ifdef __GNUC__
.section .text;
.type expEntry, STT_FUNC;
.type nmiEntry, STT_FUNC;
.type ivTMREntry, STT_FUNC;
.type ivHWEntry, STT_FUNC;
.type ivg7Entry, STT_FUNC;
.type ivg8Entry, STT_FUNC;
.type ivg9Entry, STT_FUNC;
.type ivg10Entry, STT_FUNC;
.type ivg11Entry, STT_FUNC;
.type ivg12Entry, STT_FUNC;
.type ivg13Entry, STT_FUNC;
.type ivg14Entry, STT_FUNC;
#elif defined(__ECC__)
.section program;
#else
#error "Compiler is not supported"
#endif


/*
*	例外処理のエントリ
*
*	割り込みとのネスト関係を調べ、ネストしていないならイベントスタックをセットする。
*		p0 : ユーザー定義の例外ハンドラのアドレス
*		p1 : IPENDのアドレス
*		p2 : reqflgのアドレス
*		r6 : タスクスタック
*		r7 : ASTATの退避情報
*
*/
expEntry:
	[--sp] = p0;
	save_regs;					// ユーザーレジスタの退避

	LOADLABEL( p0, _exc_vector)		// ユーザー定義例外ハンドラの格納アドレス
	LOADVALUE( p1, __IPEND )			// IPENDレジスタのアドレス

	l0 = r0;
	l1 = r0;
	l2 = r0;
	l3 = r0;
	lc0 = r0;					// 性能低下抑制のためにLCxをクリア
	lc1 = r0;

	p0 = [p0];					// p0 = exc_vector
	r0 = [p1];					// IPENDの値を取得
	r1 = 0x7fef;				//
	r0 = r0 & r1;				// IVG15とGID以外を調べる. r0.Hは必ず0
	r0.L = ones r0;				// 処理中のイベントの数を数える
	r0 += -1;					// イベントの数-1。ネストしていなければ0になる。
	cc = r0 == 0;				// ネストしてなければ真
	if !cc jump call_exception; // もしネストしていればハンドラを呼ぶ
								// ネストしていないイベントなら、スタックを切り替える
	r6 = sp;					// タスクスタックポインタを退避
	LOADLABEL( r0, ldf_stack_end)	// イベントスタックポインタを設定
	sp = r0;

call_exception:
	r7 = astat;					// ccにはネスト情報が入っている。それを退避
	r0 = p1;					// 第一引数はIPENDのアドレス
	sp += -12;					// ダミー引数領域確保 (E)
	call	(p0);				// C言語で書かれた例外ハンドラを呼ぶ
	sp += 12;					// ダミー引数領域開放 (E)
	astat = r7;					// ccを復帰
	if !cc jump get_back_x;		// もしネストしているならば、このまま帰る

								// ここではイベントはネストしていない
	sp = r6;					// スタックをタスクスタックに設定

	LOADLABEL( p2, _reqflg )
	r0 = [p2];					// reqflgの値を取得
	cc = r0;					// ディスパッチかタスク例外の要求は出ているか
	if !cc jump get_back_x;		// 出ていないならば割り込まれたタスクに戻る

								// コンテキスト切り替え作業開始
	r0 = 0;						//
	[p2] = r0;					// reqflgをクリアする.
	r0 = retx;					// タスクの戻り番地を取り出す
	r1 = 1;
	r0 = r1 | r0; 				// セルフ・ネストであると偽装する
	[--sp] = r0;				// 戻り番地をスタックにつむ
	LOADLABEL( p0, task_context)

	cli r0;						// 現在のCPUロック状態を取得
	[--sp] = r0;				// ロック状態を退避
	r1 = 0xC01F(z);				// タスクコンテキストははCPUロック状態で実行する

#ifdef UNMANAGED_INT
    r2 = UNMANAGED_INT(z);
    r0 = r0 & r2;				// 管理外割り込みのマスク状態を抽出
    r1 = r0 | r1;				// タスクコンテキストはCPUロック状態で実行する
#endif
	sti r1;						// CPUロック (F)

	retx = p0;					// ラベル"taskiv_context"を例外からの戻り番地にする
	rtx;						// 例外から戻る。コンテキストはタスクになる

get_back_x:
	restore_regs;				// 退避したレジスタを全て戻す
	p0=[sp++];
	rtx;


#ifdef __GNUC__
.size	expEntry, .-expEntry
#elif defined(__ECC__)
expEntry.end:
#else
#error "Compiler is not supported"
#endif



// イベントハンドラのエントリーコード群。
// EVTのエントリには対応する以下のコードのラベルが格納される。
// それぞれのコードはイベント中にをp0に格納した後、
// interrupt_dispatcherにジャンプする
// evtvectorはユーザーハンドラへのポインタの配列




ivHWEntry:
	[--sp] = p0;
	p0 = 5;
	jump.x interrupt_dispatcher;
#ifdef __GNUC__
.size	ivHWEntry, .-ivHWEntry
#elif defined(__ECC__)
ivHWEntry.end:
#else
#error "Compiler is not supported"
#endif


ivTMREntry:
	[--sp] = p0;
	p0 = 6;
	jump.x interrupt_dispatcher;
#ifdef __GNUC__
.size	ivTMREntry, .-ivTMREntry
#elif defined(__ECC__)
ivTMREntry.end:
#else
#error "Compiler is not supported"
#endif


ivg7Entry:
	[--sp] = p0;
	p0 = 7;
	jump.x interrupt_dispatcher;
#ifdef __GNUC__
.size	ivg7Entry, .-ivg7Entry
#elif defined(__ECC__)
ivg7Entry.end:
#else
#error "Compiler is not supported"
#endif

ivg8Entry:
	[--sp] = p0;
	p0 = 8;
	jump.x interrupt_dispatcher;
#ifdef __GNUC__
.size	ivg8Entry, .-ivg8Entry
#elif defined(__ECC__)
ivg8Entry.end:
#else
#error "Compiler is not supported"
#endif

ivg9Entry:
	[--sp] = p0;
	p0 = 9;
	jump.x interrupt_dispatcher;
#ifdef __GNUC__
.size	ivg9Entry, .-ivg8Entry
#elif defined(__ECC__)
ivg9Entry.end:
#else
#error "Compiler is not supported"
#endif

ivg10Entry:
	[--sp] = p0;
	p0 = 10;
	jump.x interrupt_dispatcher;
#ifdef __GNUC__
.size	ivg10Entry, .-ivg10Entry
#elif defined(__ECC__)
ivg10Entry.end:
#else
#error "Compiler is not supported"
#endif

ivg11Entry:
	[--sp] = p0;
	p0 = 11;
	jump.x interrupt_dispatcher;
#ifdef __GNUC__
.size	ivg11Entry, .-ivg11Entry
#elif defined(__ECC__)
ivg11Entry.end:
#else
#error "Compiler is not supported"
#endif

ivg12Entry:
	[--sp] = p0;
	p0 = 12;
	jump.x interrupt_dispatcher;
#ifdef __GNUC__
.size	ivg12Entry, .-ivg12Entry
#elif defined(__ECC__)
ivg12Entry.end:
#else
#error "Compiler is not supported"
#endif

ivg13Entry:
	[--sp] = p0;
	p0 = 13;
	jump.x interrupt_dispatcher;
#ifdef __GNUC__
.size	ivg13Entry, .-ivg13Entry
#elif defined(__ECC__)
ivg13Entry.end:
#else
#error "Compiler is not supported"
#endif

ivg14Entry:					// dispatch()の割り込み待ち部
								// USE_HW_ERRORは互換性の為だけに残してある。
#if !defined(USE_TIC_CORE) && !defined(USE_HW_ERROR) && !defined(QUICK_HW_ERROR)
	csync;						// コアタイマーでティックを刻むときにはidle命令を使えない。
	idle;						// イベント待ち
	csync;
#endif
	[--sp] = reti;				// 割込みネスト許可
	r1 = 0xffff(z);				// CPU アンロック用パターン
#ifdef UNMANAGED_INT
    cli r0;               // 現在のマスクを取り出す
	r2 = UNMANAGED_INT(z);
	r0 = r0 & r2;				// 管理外割り込みのみ取り出す
	r2 = ~r2;					// 逆マスク
	r1 = r1 & r2;				//
	r1 = r1 | r0;				// 管理外割り込みを考慮したマスク
#endif
	sti r1;						// CPU アンロック
	r1 = 0xC01F(z);				// CPU ロック用パターン
    cli r0;               // 現在のマスクを取り出す
                                // 管理外割り込みを使わない場合も、保護のため必要
#ifdef UNMANAGED_INT
	r2 = UNMANAGED_INT(z);
	r0 = r0 & r2;				// 管理外割り込みのみ取り出す
	r1 = r1 | r0;				// 管理外割り込みを考慮したマスク
#endif
	sti r1;
	reti=[sp++];				// 割込みネスト禁止
	rti;
#ifdef __GNUC__
.size	ivg14Entry, .-ivg14Entry
#elif defined(__ECC__)
ivg14Entry.end:
#else
#error "Compiler is not supported"
#endif


nmiEntry:
	jump	nmiEntry;
	rtn;
#ifdef __GNUC__
.size	nmiEntry, .-nmiEntry
#elif defined(__ECC__)
nmiEntry.end:
#else
#error "Compiler is not supported"
#endif




/*
 *  微少時間待ち
 *  内側のループは12サイクルで回る。
 */
	.global _sil_dly_nse;
_sil_dly_nse:
	r1 = SIL_DLY_TIM1;
	r2 = SIL_DLY_TIM2;
	r0 = r0 - r1;			// dilay - SIL_DLY_TIM1
	cc = an;			// 結果が0より大きければ
	if !cc jump _sil_dly_nse_1;	// ループ
	rts;
_sil_dly_nse_1:
	r0 = r0 - r2;
	nop;
	cc = an;			// 結果が0より大きければ
	if !cc jump _sil_dly_nse_1;	// ループ
	rts;
_sil_dly_nse.end:

