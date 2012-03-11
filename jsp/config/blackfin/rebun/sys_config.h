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


#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

#include <chip_config.h>

/*
 * ADSP-BF532に依存せず、ボードへの実装によって決まるパラメータを
 * ここで定義する。
 */


/*
 * どのようなクロック入力にも対応できるよう、以下でCSEL, SSEL, MSELの値
 * とクロック入力周波数(Hz)を指定する。PLLのプログラムはsys_config.cの
 * sys_initialize()関数で行われる。
 *
 * CSELVALは1,2,4,8の中から選ぶ。CLKINの単位はHz
 */

#define CSELVAL 1
#define SSELVAL 4
#define MSELVAL 14
#define CLKIN 27000000


/*
 * ハードウェア・エラー割り込みの許可
 * QUICK_HW_ERRORをdefineすると、ハードウェア・エラー・イベント
 * が即座に受理される。defineしない場合には、割り込み待ち状態で
 * ハードウェア・エラー・割り込みが発生した場合、次の割り込みまで
 * 待たされることになる。
 *
 */
//#define QUICK_HW_ERROR


/*
 * 実行時初期化関数の使用
 *
 * マクロ USE_RUNTIME_INIT を定義すると、start.asmのなかで _mi_initialize
 * を呼び出す。この関数はノーブートモードで起動するときにのみ必要になるので
 * 通常は USE_RUNTIME_INITを使う必要はない。
 * 詳細はADIのEE239に詳しい。http://tinyurl.com/2hpbk (tinyURL)
 *
*/
//#define USE_RUNTIME_INIT

/*
 * カーネル管理外割り込みの宣言
 *
 * マクロUNMANAGED_INTはカーネル管理外割り込みを宣言する。この
 * マクロは16ビットのビットマップになっており、以下にあるパターン
 * のうちひとつを使う。なお、どのようなパターンを選んでもNMIは管理外
 * 割込みとして扱われる。
 * UNMANAGED_INTを宣言しない場合、NMIだけが管理外割込みとして扱われる。
 *
*/
//#define UNMANAGED_INT 0x0020			// IVHWがカーネル管理外.
//#define UNMANAGED_INT 0x0060			// IVTMRとIVHWがカーネル管理外.
//#define UNMANAGED_INT 0x00E0			// IVG7, IVTMRとIVHWがカーネル管理外.
//#define UNMANAGED_INT 0x01E0			// IVG8 ... IVG7, IVTMRとIVHWがカーネル管理外.
//#define UNMANAGED_INT 0x03E0			// IVG9 ... IVG7, IVTMRとIVHWがカーネル管理外.
//#define UNMANAGED_INT 0x07E0			// IVG10 ... IVG7, IVTMRとIVHWがカーネル管理外.
//#define UNMANAGED_INT 0x0FE0			// IVG11 ... IVG7, IVTMRとIVHWがカーネル管理外.
//#define UNMANAGED_INT 0x1FE0			// IVG12 ... IVG7, IVTMRとIVHWがカーネル管理外.
//#define UNMANAGED_INT 0x3FE0			// IVG13 ... IVG7, IVTMRとIVHWがカーネル管理外.

/*
 * C++関連資源を用意する
 *
 * マクロINIT_C_PLUS_PLUSを定義すると、C++言語用のテーブルを
 * 初期化コードを実行する。また、C++言語用のテーブルを宣言する
 *
*/
//#define INIT_C_PLUS_PLUS

/*
 *  起動メッセージのターゲットシステム名
 */
#define	TARGET_NAME	"Rebun"


/*
 *  シリアルポート数の定義
 *  TNUM_SIOP_XXXは、UARTの種別ごとに宣言する。
 *  TNUM_SIOP_UARTはBF532内蔵UARTのうち、PDICが使用するもの。
 *  BF532はUARTをひとつしかもっていないので、1か0を宣言する。
 *  TNUM_PORTは、TNUM_SIOP_XXXのうち、GDICが使用するものの総和。
 *  uart.cで使う。
 */
#define TNUM_PORT	1		/* GDICがサポートするシリアルポートの数 */
#define TNUM_SIOP_UART	1	/* PIDCがサポートするシリアルポートの数 */

/*
 *  シリアルポート番号に関する定義
 *  ログタスクがシリアルポートを使わない場合は無視していい。
 */
#define	LOGTASK_PORTID	1	/* システムログに用いるシリアルポート番号 */

/*  UART0のボーレート． */
#define UART0_BAUD_RATE 57600		/* 9600bps*/

/*
 * PLLの初期化の強制
 * PLLは、PLL_CTLレジスタがデフォルト値で、かつ、SDRAMコントローラが
 * ディセーブルのときに限り初期化を行うようになっている。これは、ブート
 * ローダーなどの初期化ソフトがSDRAMを初期化したときにそれを保護するため
 * である。一方でVisualDSP++のデバッガはSDRAMを自分で初期化するため、
 * この機能とうまく折り合いがつかない場合がある。
 * 状況に関わらず常に初期化したい時には FORCE_PLL_INITIALIZE マクロを
 * 宣言する。
 */
// #deinfe FORCE_PLL_INITIALIZE


#ifndef __SILICON_REVISION__
#define __SILICON_REVISION__ 0x05
#endif

#endif /* _SYS_CONFIG_H_ */
