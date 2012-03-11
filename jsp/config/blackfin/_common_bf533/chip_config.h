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


#ifndef _CHIP_CONFIG_H_
#define _CHIP_CONFIG_H_


/*
 *	ターゲットシステム依存モジュール（ADSP-BF531/2/3共用）
 *
 *  このインクルードファイルは，t_config.h のみからインクルードされる．
 *  他のファイルから直接インクルードしてはならない．
*/

/*
 *  ターゲットシステムのハードウェア資源の定義
 *
 *  DEVICE_INTERRUPT_COUNTはSIC_IMASKのビット数
 *  を与える。
 *  ADSP-BF533のビット数は24なので、値は24である。
 */

#define DEVICE_INTERRUPT_COUNT 24



/*
 *  PLLクロック周波数
 */
#define PLLCLOCK (CLKIN*MSELVAL)
#define SYSCLOCK  (PLLCLOCK / SSELVAL)
#define CORECLOCK (PLLCLOCK / CSELVAL)



/*
 *  TICの割込みハンドラのベクタ番号
 */
#ifdef USE_TIC_CORE
#define INHNO_TIMER	INHNO_CORE_TIMER
#else
#define INHNO_TIMER	INHNO_GP_TIMER2
#endif


/*
 *  微少時間待ちのための定義
 *  sil_dly_nseの呼び出しオーバーヘッドとループボディーサイクルを
 *  nSで表す。関数はcpu_support.asmに定義してある。
 *  SIL_DLY_TIM1は24サイクル。32nS( 750MHz ), 40nS( 600MHz )
 *  SIL_DLY_TIM2は12サイクル。16nS( 750MHz ), 20nS( 600MHz )
 */


#define	SIL_DLY_TIM1	(24*1000000000/CORECLOCK)
#define	SIL_DLY_TIM2	(12*1000000000/CORECLOCK)

/*************************************************************************
 * uart.c用構成マクロ
 */

/*  シリアルの割込みハンドラのベクタ番号 */
#define INHNO_SIO0_TX   INHNO_UART_TX
#define INHNO_SIO0_RX   INHNO_UART_RX

/*
 *  UARTレジスタの配置境界。ADSP-BF533の内蔵UARTは4バイト周期でレジスタが
 *  並んでいるので、ここには4を指定する。
 */
#define UART_BOUNDARY 4

/*  UARTレジスタにsil_xxb_iop()を使ってアクセスするときにはUART_IOP_ACCESSを 宣言する。*/
#define UART_IOP_ACCESS

/*  シリアルポートのアドレス。 */
#define UART0_ADDRESS 0xFFC00400	// 内蔵UART THRのアドレス。

/*  UARTのボーレートジェネレータに設定すべき値． */
#define UART0_DIVISOR	SYSCLOCK/16/UART0_BAUD_RATE

/*  内蔵UARTを使う場合は、UARTx_BLACKFIN_UCENを宣言してUCENを初期化しなければならない。*/
#define UART0_BLACKFIN_UCEN


/*
 * uart.c用構成マクロ終わり
 *************************************************************************/



/*
 *  割り込みベクトル番号
 *
 *  この定義はコンフィグレーションファイルでDEF_INHの引数として使う
 *  SIC_ISRにおけるビット番号である。
 *
 */
#define INHNO_PLL			0
#define INHNO_DMA_ERROR		1
#define INHNO_PPI_ERROR		2
#define INHNO_SPORT0_ERROR	3
#define INHNO_SPORT2_ERROR	4
#define INHNO_SPI_ERROR		5
#define INHNO_UART_ERROR	6
#define INHNO_RTC			7
#define INHNO_PPI			8
#define INHNO_SPORT0_RX		9
#define INHNO_SPORT0_TX		10
#define INHNO_SPORT1_RX		11
#define INHNO_SPORT1_TX		12
#define INHNO_SPI			13
#define INHNO_UART_RX		14
#define INHNO_UART_TX		15
#define INHNO_GP_TIMER0		16
#define INHNO_GP_TIMER1		17
#define INHNO_GP_TIMER2		18
#define INHNO_PFA			19
#define INHNO_PFB			20
#define INHNO_MEMORY_DMA0	21
#define INHNO_MEMORY_DMA1	22
#define INHNO_WDG			23

// SIC_ISRにない特殊な割り込み
#define INHNO_HW_ERROR 		24
#define INHNO_CORE_TIMER 		25
#define INHNO_RAISE 		26


/*
 *  SIC_ISRの割り込みのベクタ番号
 *
 *  この定義はena_int, dis_intの引数として使う。
 */

#define INTNO_PLL			0
#define INTNO_DMA_ERROR		1
#define INTNO_PPI_ERROR		2
#define INTNO_SPORT0_ERROR	3
#define INTNO_SPORT2_ERROR	4
#define INTNO_SPI_ERROR		5
#define INTNO_UART_ERROR	6
#define INTNO_RTC			7
#define INTNO_PPI			8
#define INTNO_SPORT0_RX		9
#define INTNO_SPORT0_TX		10
#define INTNO_SPORT1_RX		11
#define INTNO_SPORT1_TX		12
#define INTNO_SPI			13
#define INTNO_UART_RX		14
#define INTNO_UART_TX		15
#define INTNO_GP_TIMER0		16
#define INTNO_GP_TIMER1		17
#define INTNO_GP_TIMER2		18
#define INTNO_PFA			19
#define INTNO_PFB			20
#define INTNO_MEMORY_DMA0	21
#define INTNO_MEMORY_DMA1	22
#define INTNO_WDG			23



#ifndef _MACRO_ONLY

/*
* SIC_IARxを解析して、イベント順位ごとに割り当てられている割り込み
* のビットマップを作る。SIC_IARxのフィールド値は優先順位-7なので
* その分補正する。
*/
void make_priority_mask( void );

/*
 *  ターゲットシステム依存の初期化
 */
extern void	sys_initialize(void);

/*
 *  ターゲットシステムの終了
 *
 *  システムを終了する時に使う．ROMモニタ／GDB STUB呼出しで実現する．
 */
extern void	sys_exit(void);

/*
 *  ターゲットシステムの文字出力
 *
 *  システムの低レベルの文字出力ルーチン．ROMモニタ／GDB STUB呼出しで実
 *  現する．
 */
extern void	sys_putc(char c);

/*
* SIC_IARxを解析して、イベント順位ごとに割り当てられている割り込み
* のビットマップを作る。SIC_IARxのフィールド値は優先順位-7なので
* その分補正する。
*/
void make_priority_mask( void );

/*
 * 割り込みの許可、禁止
 *
 */
typedef unsigned int INTNO;
extern ER ena_int( INTNO intno );
extern ER dis_int( INTNO intno );

/*
 * 割り込みマスクの操作
 *
 *
 */
typedef unsigned int IMS;
extern ER chg_ims( IMS ims );
extern ER get_ims( IMS * p_ims );

/*
 * 割り込みを起こしたデバイスを調べる
*/
extern void device_dispatcher(  unsigned int priority, unsigned int imask );

#endif /* _MACRO_ONLY */
#endif /* _CHIP_CONFIG_H_ */
