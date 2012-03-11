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
 *  @(#) $Id: sys_config.c,v 1.1 2009/01/31 05:27:37 suikan Exp $
 */

/*
 *	ターゲットシステム依存モジュール（DVE68K/40用）
 */

#include "jsp_kernel.h"
#include <sil.h>
#include <dve68k_dga.h>

/*
 *  プロセッサ識別のための変数（マルチプロセッサ対応）
 */
UINT	board_id;		/* ボードID */
VP	board_addr;		/* ローカルメモリの先頭アドレス */

/*
 *  ターゲットシステム依存の初期化
 */
void
sys_initialize()
{
	/*
	 *  プロセッサ識別のための変数の初期化
	 */
	board_id = ((UINT)(sil_rew_mem((VP) TADR_BOARD_REG0)) & 0x1f);
	board_addr = (VP)(board_id << 24);

	/*
	 *  割込み関連の初期化
	 *
	 *  すべての割込みをマスク・クリアし，割込みベクトルを設定する．
	 */
	dga_write((VP) TADR_DGA_CSR21, 0);
	dga_write((VP) TADR_DGA_CSR23, ~0);
	dga_write((VP) TADR_DGA_CSR19, (TVEC_G0I << 24) | (TVEC_G1I << 16)
					| (TVEC_SWI << 8) | TVEC_SPRI);

	/*
	 *  アボート割込みの設定（NMI）
	 *
	 *  アボート割込みの割込みレベルを設定し，要求をクリアした後，
	 *  マスクを解除する．
	 */
	dga_set_ilv((VP) TADR_DGA_CSR24, TBIT_ABTIL, TIRQ_NMI);
	dga_write((VP) TADR_DGA_CSR23, TBIT_ABT);
	dga_bit_or((VP) TADR_DGA_CSR21, TBIT_ABT);

	/*
	 *  メモリ領域の設定
	 *
	 *  ローカルメモリのVMEバス上での先頭アドレスとサイズ（16MB）
	 *  し，アクセスを受け付けるように設定する．また，VMEバスから
	 *  拡張アドレスアクセスを受け付けるようにに設定する．
	 */
	dga_write((VP) TADR_DGA_CSR4, (UW) board_addr | 0x00ff);
	dga_write((VP) TADR_DGA_CSR5, 0x0000012fu);

	/*
	 *  インタフェースレジスタ（IFR）の設定
	 *
	 *  インタフェースレジスタのベースアドレスを設定する．また，イ
	 *  ンタフェースレジスタ0のサービスリクエストフラグをクリア．
	 *  インタフェースレジスタ3にボードのID番号を設定．
	 */
	dga_write((VP) TADR_DGA_CSR3, (board_id << 4) | 0x3);
	dga_write((VP) TADR_DGA_IFR0, 0x80000000u);
	dga_write((VP) TADR_DGA_IFR3, board_id);

	/*
	 *  ラウンドロビンモードに設定（マルチプロセッサ対応）
	 */
	dga_write((VP) TADR_DGA_CSR1, (dga_read((VP) TADR_DGA_CSR1)
			& 0xffeffcff) | (1u << 20) | ((board_id % 4) << 8));
}

/*
 *  ターゲットシステムの終了
 */
void
sys_exit()
{
	dve68k_exit();
}

/*
 *  ターゲットシステムの文字出力
 */
void
sys_putc(char c)
{
	if (c == '\n') {
		dve68k_putc('\r');
	}
	dve68k_putc(c);
}
