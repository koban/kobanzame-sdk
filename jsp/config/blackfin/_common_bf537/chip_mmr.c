/*
 * ADSP-BF537の System MMR 定義。アドレスの指定は .ldファイルとの組み合わせで行う。
 * .ldファイルのサンプルは、 config/blackfin/ezkit_bf537を参照。
 *
 * アドレスが正しく割り当てられているかは、次の方法で検証した.
 * (Makefileの$(OBJCOPY) -R .sysmmr -R .coremmr jsp をコメントアウトすること)
 *
$ readelf -s  -W jsp | grep mmr | grep -v DUMMY | grep -v blackfin | sort -k 2 | awk '/_mmr/{print $8, $2}' -  | sed 's/_mmr//p' | uniq > regelf.txt
$ cat ../jsp/config/blackfin/def_lpblackfin.h ../jsp/config/blackfin/defbf537.h  ../jsp/config/blackfin/defbf534.h | grep '0xFF[CE]0'  | grep "#define" | sed 's/_MA /_MAP /' - | awk '{print $2,$3}' - | sort -k 2 | sed 's/0x//' - > regdef.txt
$ diff -i -w regelf.txt regdef.txt

24a25,27
> RTC_PREN FFC00314
> UART0_DLL FFC00400
> UART0_RBR FFC00400
25a29
> UART0_DLH FFC00404
143a148
> DMA_TCPER FFC00B0C
144a150
> DMA_TCCNT FFC00B10
408a415,416
> UART1_DLL FFC02000
> UART1_RBR FFC02000
409a418
> UART1_DLH FFC02004
879a889
> DCPLB_FAULT_STATUS FFE00008
917a928
> CODE_FAULT_STATUS FFE01008
918a930
> CODE_FAULT_ADDR FFE0100C
 *
 * つまり、いくつかのレジスタの宣言がELF側から抜けていることになる。ここで、
 * それぞれのレジスタは別のレジスタのエイリアスであることに注目する。
 * RTC_PREN -> RTC_FAST
 * UARTx_DLL -> UARTx_THR
 * UARTx_RBR -> UARTx_THR
 * UARTx_DLH -> UARTx_IER
 * DMA_TCPER -> DMA_TC_PER
 * DMA_TCCNT -> DMA_TC_CNT
 *
 * したがって、上の結果は問題ないと言える。
 *
 */


volatile unsigned short mmrPLL_CTL 	__attribute__ ((aligned(4), section("PWRMGMT")));
volatile unsigned short mmrPLL_DIV 	__attribute__ ((aligned(4), section("PWRMGMT")));
volatile unsigned short mmrVR_CTL 	__attribute__ ((aligned(4), section("PWRMGMT")));
volatile unsigned short mmrPLL_STAT 	__attribute__ ((aligned(4), section("PWRMGMT")));
volatile unsigned short mmrPLL_LOCKCNT 	__attribute__ ((aligned(4), section("PWRMGMT")));



volatile unsigned short  mmrSWRST 			__attribute__ ((aligned(4), section("SYSRST")));
volatile unsigned short  mmrSYSCR 			__attribute__ ((aligned(4), section("SYSRST")));
volatile unsigned long   mmrSIC_RVECT		__attribute__ ((aligned(4), section("SYSRST")));
volatile unsigned long   mmrSIC_IMASK 		__attribute__ ((aligned(4), section("SYSRST")));
volatile unsigned long   mmrSIC_IAR0 		__attribute__ ((aligned(4), section("SYSRST")));
volatile unsigned long   mmrSIC_IAR1 		__attribute__ ((aligned(4), section("SYSRST")));
volatile unsigned long   mmrSIC_IAR2 		__attribute__ ((aligned(4), section("SYSRST")));
volatile unsigned long   mmrSIC_IAR3 		__attribute__ ((aligned(4), section("SYSRST")));
volatile unsigned long   mmrSIC_ISR 		__attribute__ ((aligned(4), section("SYSRST")));
volatile unsigned long   mmrSIC_IWR 		__attribute__ ((aligned(4), section("SYSRST")));



volatile unsigned short  mmrWDOG_CTL 		__attribute__ ((aligned(4), section("WDOG")));
volatile unsigned long   mmrWDOG_CNT 		__attribute__ ((aligned(4), section("WDOG")));
volatile unsigned long   mmrWDOG_STAT 		__attribute__ ((aligned(4), section("WDOG")));



volatile unsigned long   mmrRTC_STAT 		__attribute__ ((aligned(4), section("RTC")));
volatile unsigned short  mmrRTC_ICTL 		__attribute__ ((aligned(4), section("RTC")));
volatile unsigned short  mmrRTC_ISTAT 		__attribute__ ((aligned(4), section("RTC")));
volatile unsigned short  mmrRTC_SWCNT 		__attribute__ ((aligned(4), section("RTC")));
volatile unsigned long   mmrRTC_ALARM 		__attribute__ ((aligned(4), section("RTC")));
volatile unsigned short  mmrRTC_FAST 		__attribute__ ((aligned(4), section("RTC")));



volatile unsigned short  mmrUART0_THR 		__attribute__ ((aligned(4), section("UART0")));
volatile unsigned short  mmrUART0_IER 		__attribute__ ((aligned(4), section("UART0")));
volatile unsigned short  mmrUART0_IIR 		__attribute__ ((aligned(4), section("UART0")));
volatile unsigned short  mmrUART0_LCR 		__attribute__ ((aligned(4), section("UART0")));
volatile unsigned short  mmrUART0_MCR 		__attribute__ ((aligned(4), section("UART0")));
volatile unsigned short  mmrUART0_LSR 		__attribute__ ((aligned(4), section("UART0")));
volatile unsigned short  mmrUART0_MSR		__attribute__ ((aligned(4), section("UART0")));
volatile unsigned short  mmrUART0_SCR 		__attribute__ ((aligned(4), section("UART0")));
volatile unsigned short  mmrUART0_DUMMY0	__attribute__ ((aligned(4), section("UART0")));
volatile unsigned short  mmrUART0_GCTL 		__attribute__ ((aligned(4), section("UART0")));



volatile unsigned short  mmrSPI_CTL 		__attribute__ ((aligned(4), section("SPI")));
volatile unsigned short  mmrSPI_FLG 		__attribute__ ((aligned(4), section("SPI")));
volatile unsigned short  mmrSPI_STAT 		__attribute__ ((aligned(4), section("SPI")));
volatile unsigned short  mmrSPI_TDBR 		__attribute__ ((aligned(4), section("SPI")));
volatile unsigned short  mmrSPI_RDBR 		__attribute__ ((aligned(4), section("SPI")));
volatile unsigned short  mmrSPI_BAUD 		__attribute__ ((aligned(4), section("SPI")));
volatile unsigned short  mmrSPI_SHADOW 		__attribute__ ((aligned(4), section("SPI")));



volatile unsigned short  mmrTIMER0_CONFIG 	__attribute__ ((aligned(4), section("TIMER")));
volatile unsigned long   mmrTIMER0_COUNTER 	__attribute__ ((aligned(4), section("TIMER")));
volatile unsigned long   mmrTIMER0_PERIOD 	__attribute__ ((aligned(4), section("TIMER")));
volatile unsigned long   mmrTIMER0_WIDTH 	__attribute__ ((aligned(4), section("TIMER")));

volatile unsigned short  mmrTIMER1_CONFIG 	__attribute__ ((aligned(4), section("TIMER")));
volatile unsigned long   mmrTIMER1_COUNTER 	__attribute__ ((aligned(4), section("TIMER")));
volatile unsigned long   mmrTIMER1_PERIOD 	__attribute__ ((aligned(4), section("TIMER")));
volatile unsigned long   mmrTIMER1_WIDTH 	__attribute__ ((aligned(4), section("TIMER")));

volatile unsigned short  mmrTIMER2_CONFIG 	__attribute__ ((aligned(4), section("TIMER")));
volatile unsigned long   mmrTIMER2_COUNTER 	__attribute__ ((aligned(4), section("TIMER")));
volatile unsigned long   mmrTIMER2_PERIOD 	__attribute__ ((aligned(4), section("TIMER")));
volatile unsigned long   mmrTIMER2_WIDTH 	__attribute__ ((aligned(4), section("TIMER")));

volatile unsigned short  mmrTIMER3_CONFIG 	__attribute__ ((aligned(4), section("TIMER")));
volatile unsigned long   mmrTIMER3_COUNTER 	__attribute__ ((aligned(4), section("TIMER")));
volatile unsigned long   mmrTIMER3_PERIOD 	__attribute__ ((aligned(4), section("TIMER")));
volatile unsigned long   mmrTIMER3_WIDTH 	__attribute__ ((aligned(4), section("TIMER")));

volatile unsigned short  mmrTIMER4_CONFIG 	__attribute__ ((aligned(4), section("TIMER")));
volatile unsigned long   mmrTIMER4_COUNTER 	__attribute__ ((aligned(4), section("TIMER")));
volatile unsigned long   mmrTIMER4_PERIOD 	__attribute__ ((aligned(4), section("TIMER")));
volatile unsigned long   mmrTIMER4_WIDTH 	__attribute__ ((aligned(4), section("TIMER")));

volatile unsigned short  mmrTIMER5_CONFIG 	__attribute__ ((aligned(4), section("TIMER")));
volatile unsigned long   mmrTIMER5_COUNTER 	__attribute__ ((aligned(4), section("TIMER")));
volatile unsigned long   mmrTIMER5_PERIOD 	__attribute__ ((aligned(4), section("TIMER")));
volatile unsigned long   mmrTIMER5_WIDTH 	__attribute__ ((aligned(4), section("TIMER")));

volatile unsigned short  mmrTIMER6_CONFIG 	__attribute__ ((aligned(4), section("TIMER")));
volatile unsigned long   mmrTIMER6_COUNTER 	__attribute__ ((aligned(4), section("TIMER")));
volatile unsigned long   mmrTIMER6_PERIOD 	__attribute__ ((aligned(4), section("TIMER")));
volatile unsigned long   mmrTIMER6_WIDTH 	__attribute__ ((aligned(4), section("TIMER")));

volatile unsigned short  mmrTIMER7_CONFIG 	__attribute__ ((aligned(4), section("TIMER")));
volatile unsigned long   mmrTIMER7_COUNTER 	__attribute__ ((aligned(4), section("TIMER")));
volatile unsigned long   mmrTIMER7_PERIOD 	__attribute__ ((aligned(4), section("TIMER")));
volatile unsigned long   mmrTIMER7_WIDTH 	__attribute__ ((aligned(4), section("TIMER")));

volatile unsigned short  mmrTIMER_ENABLE 	__attribute__ ((aligned(4), section("TMRCTL")));
volatile unsigned short  mmrTIMER_DISABLE 	__attribute__ ((aligned(4), section("TMRCTL")));
volatile unsigned long   mmrTIMER_STATUS	__attribute__ ((aligned(4), section("TMRCTL")));



volatile unsigned short  mmrPORTFIO	 				__attribute__ ((aligned(4), section("FLAGF")));
volatile unsigned short  mmrPORTFIO_CLEAR		 	__attribute__ ((aligned(4), section("FLAGF")));
volatile unsigned short  mmrPORTFIO_SET	 			__attribute__ ((aligned(4), section("FLAGF")));
volatile unsigned short  mmrPORTFIO_TOGGLE 			__attribute__ ((aligned(4), section("FLAGF")));
volatile unsigned short  mmrPORTFIO_MASKA 			__attribute__ ((aligned(4), section("FLAGF")));
volatile unsigned short  mmrPORTFIO_MASKA_CLEAR		__attribute__ ((aligned(4), section("FLAGF")));
volatile unsigned short  mmrPORTFIO_MASKA_SET 		__attribute__ ((aligned(4), section("FLAGF")));
volatile unsigned short  mmrPORTFIO_MASKA_TOGGLE 	__attribute__ ((aligned(4), section("FLAGF")));
volatile unsigned short  mmrPORTFIO_MASKB	 		__attribute__ ((aligned(4), section("FLAGF")));
volatile unsigned short  mmrPORTFIO_MASKB_CLEAR 	__attribute__ ((aligned(4), section("FLAGF")));
volatile unsigned short  mmrPORTFIO_MASKB_SET 		__attribute__ ((aligned(4), section("FLAGF")));
volatile unsigned short  mmrPORTFIO_MASKB_TOGGLE 	__attribute__ ((aligned(4), section("FLAGF")));
volatile unsigned short  mmrPORTFIO_DIR 			__attribute__ ((aligned(4), section("FLAGF")));
volatile unsigned short  mmrPORTFIO_POLAR 			__attribute__ ((aligned(4), section("FLAGF")));
volatile unsigned short  mmrPORTFIO_EDGE 			__attribute__ ((aligned(4), section("FLAGF")));
volatile unsigned short  mmrPORTFIO_BOTH 			__attribute__ ((aligned(4), section("FLAGF")));
volatile unsigned short  mmrPORTFIO_INEN 			__attribute__ ((aligned(4), section("FLAGF")));



volatile unsigned short  mmrSPORT0_TCR1 			__attribute__ ((aligned(4), section("SPORT0")));
volatile unsigned short  mmrSPORT0_TCR2 		__attribute__ ((aligned(4), section("SPORT0")));
volatile unsigned short  mmrSPORT0_TCLKDIV 	__attribute__ ((aligned(4), section("SPORT0")));
volatile unsigned short  mmrSPORT0_TFSDIV 		__attribute__ ((aligned(4), section("SPORT0")));
volatile unsigned long   mmrSPORT0_TX 		__attribute__ ((aligned(4), section("SPORT0")));
volatile unsigned short  mmrSPORT0_DUMMY0 		__attribute__ ((aligned(4), section("SPORT0")));
volatile unsigned long   mmrSPORT0_RX 		__attribute__ ((aligned(4), section("SPORT0")));
volatile unsigned short  mmrSPORT0_DUMMY1 		__attribute__ ((aligned(4), section("SPORT0")));
volatile unsigned short  mmrSPORT0_RCR1 		__attribute__ ((aligned(4), section("SPORT0")));
volatile unsigned short  mmrSPORT0_RCR2 		__attribute__ ((aligned(4), section("SPORT0")));
volatile unsigned short  mmrSPORT0_RCLKDIV 	__attribute__ ((aligned(4), section("SPORT0")));
volatile unsigned short  mmrSPORT0_RFSDIV 		__attribute__ ((aligned(4), section("SPORT0")));
volatile unsigned short  mmrSPORT0_STAT 		__attribute__ ((aligned(4), section("SPORT0")));
volatile unsigned short  mmrSPORT0_CHNL 		__attribute__ ((aligned(4), section("SPORT0")));
volatile unsigned short  mmrSPORT0_MCMC1 		__attribute__ ((aligned(4), section("SPORT0")));
volatile unsigned short  mmrSPORT0_MCMC2 		__attribute__ ((aligned(4), section("SPORT0")));
volatile unsigned long   mmrSPORT0_MTCS0 		__attribute__ ((aligned(4), section("SPORT0")));
volatile unsigned long   mmrSPORT0_MTCS1 		__attribute__ ((aligned(4), section("SPORT0")));
volatile unsigned long   mmrSPORT0_MTCS2 		__attribute__ ((aligned(4), section("SPORT0")));
volatile unsigned long   mmrSPORT0_MTCS3 		__attribute__ ((aligned(4), section("SPORT0")));
volatile unsigned long   mmrSPORT0_MRCS0 		__attribute__ ((aligned(4), section("SPORT0")));
volatile unsigned long   mmrSPORT0_MRCS1 		__attribute__ ((aligned(4), section("SPORT0")));
volatile unsigned long   mmrSPORT0_MRCS2 		__attribute__ ((aligned(4), section("SPORT0")));
volatile unsigned long   mmrSPORT0_MRCS3 		__attribute__ ((aligned(4), section("SPORT0")));



volatile unsigned short  mmrSPORT1_TCR1 		__attribute__ ((aligned(4), section("SPORT1")));
volatile unsigned short  mmrSPORT1_TCR2 		__attribute__ ((aligned(4), section("SPORT1")));
volatile unsigned short  mmrSPORT1_TCLKDIV 	__attribute__ ((aligned(4), section("SPORT1")));
volatile unsigned short  mmrSPORT1_TFSDIV 		__attribute__ ((aligned(4), section("SPORT1")));
volatile unsigned long   mmrSPORT1_TX 		__attribute__ ((aligned(4), section("SPORT1")));
volatile unsigned long   mmrSPORT1_DUMMY0 		__attribute__ ((aligned(4), section("SPORT1")));
volatile unsigned long   mmrSPORT1_RX 		__attribute__ ((aligned(4), section("SPORT1")));
volatile unsigned long   mmrSPORT1_DUMMY1 		__attribute__ ((aligned(4), section("SPORT1")));
volatile unsigned short  mmrSPORT1_RCR1 		__attribute__ ((aligned(4), section("SPORT1")));
volatile unsigned short  mmrSPORT1_RCR2 		__attribute__ ((aligned(4), section("SPORT1")));
volatile unsigned short  mmrSPORT1_RCLKDIV 	__attribute__ ((aligned(4), section("SPORT1")));
volatile unsigned short  mmrSPORT1_RFSDIV 		__attribute__ ((aligned(4), section("SPORT1")));
volatile unsigned short  mmrSPORT1_STAT 		__attribute__ ((aligned(4), section("SPORT1")));
volatile unsigned short  mmrSPORT1_CHNL 		__attribute__ ((aligned(4), section("SPORT1")));
volatile unsigned short  mmrSPORT1_MCMC1 		__attribute__ ((aligned(4), section("SPORT1")));
volatile unsigned short  mmrSPORT1_MCMC2 		__attribute__ ((aligned(4), section("SPORT1")));
volatile unsigned long   mmrSPORT1_MTCS0 		__attribute__ ((aligned(4), section("SPORT1")));
volatile unsigned long   mmrSPORT1_MTCS1 		__attribute__ ((aligned(4), section("SPORT1")));
volatile unsigned long   mmrSPORT1_MTCS2 		__attribute__ ((aligned(4), section("SPORT1")));
volatile unsigned long   mmrSPORT1_MTCS3 		__attribute__ ((aligned(4), section("SPORT1")));
volatile unsigned long   mmrSPORT1_MRCS0 		__attribute__ ((aligned(4), section("SPORT1")));
volatile unsigned long   mmrSPORT1_MRCS1 		__attribute__ ((aligned(4), section("SPORT1")));
volatile unsigned long   mmrSPORT1_MRCS2 		__attribute__ ((aligned(4), section("SPORT1")));
volatile unsigned long   mmrSPORT1_MRCS3 		__attribute__ ((aligned(4), section("SPORT1")));



volatile unsigned short  mmrEBIU_AMGCTL 		__attribute__ ((aligned(4), section("EBIU")));
volatile unsigned long   mmrEBIU_AMBCTL0 		__attribute__ ((aligned(4), section("EBIU")));
volatile unsigned long   mmrEBIU_AMBCTL1 		__attribute__ ((aligned(4), section("EBIU")));
volatile unsigned short  mmrEBIU_DUMMY 		__attribute__ ((aligned(4), section("EBIU")));
volatile unsigned long   mmrEBIU_SDGCTL 		__attribute__ ((aligned(4), section("EBIU")));
volatile unsigned short  mmrEBIU_SDBCTL 		__attribute__ ((aligned(4), section("EBIU")));
volatile unsigned short  mmrEBIU_SDRRC 		__attribute__ ((aligned(4), section("EBIU")));
volatile unsigned short  mmrEBIU_SDSTAT 		__attribute__ ((aligned(4), section("EBIU")));



volatile	unsigned	short	mmrDMA_TC_DUMMY0	__attribute__ ((aligned(4), section("DMATC")));
volatile	unsigned	short	mmrDMA_TC_DUMMY1	__attribute__ ((aligned(4), section("DMATC")));
volatile	unsigned	short	mmrDMA_TC_DUMMY3	__attribute__ ((aligned(4), section("DMATC")));
volatile unsigned short  mmrDMA_TC_PER		 __attribute__ ((aligned(4), section("DMATC")));
volatile unsigned short  mmrDMA_TC_CNT		__attribute__ ((aligned(4), section("DMATC")));



volatile	void *	volatile	mmrDMA0_NEXT_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA0_START_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA0_CONFIG			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA0_DUMMY0			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA0_X_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrDMA0_X_MODIFY		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA0_Y_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrDMA0_Y_MODIFY		__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA0_CURR_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA0_CURR_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA0_IRQ_STATUS		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA0_PERIPHERAL_MAP	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA0_CURR_X_COUNT	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA0_DUMMY1			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA0_CURR_Y_COUNT	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA0_DUMMY2			__attribute__ ((aligned(4), section("DMA")));

volatile	void *	volatile	mmrDMA1_NEXT_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA1_START_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA1_CONFIG			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA1_DUMMY0			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA1_X_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrDMA1_X_MODIFY		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA1_Y_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrDMA1_Y_MODIFY		__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA1_CURR_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA1_CURR_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA1_IRQ_STATUS		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA1_PERIPHERAL_MAP	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA1_CURR_X_COUNT	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA1_DUMMY1			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA1_CURR_Y_COUNT	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA1_DUMMY2			__attribute__ ((aligned(4), section("DMA")));

volatile	void *	volatile	mmrDMA2_NEXT_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA2_START_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA2_CONFIG			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA2_DUMMY0			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA2_X_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrDMA2_X_MODIFY		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA2_Y_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrDMA2_Y_MODIFY		__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA2_CURR_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA2_CURR_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA2_IRQ_STATUS		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA2_PERIPHERAL_MAP	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA2_CURR_X_COUNT	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA2_DUMMY1			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA2_CURR_Y_COUNT	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA2_DUMMY2			__attribute__ ((aligned(4), section("DMA")));

volatile	void *	volatile	mmrDMA3_NEXT_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA3_START_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA3_CONFIG			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA3_DUMMY0			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA3_X_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrDMA3_X_MODIFY		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA3_Y_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrDMA3_Y_MODIFY		__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA3_CURR_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA3_CURR_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA3_IRQ_STATUS		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA3_PERIPHERAL_MAP	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA3_CURR_X_COUNT	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA3_DUMMY1			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA3_CURR_Y_COUNT	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA3_DUMMY2			__attribute__ ((aligned(4), section("DMA")));

volatile	void *	volatile	mmrDMA4_NEXT_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA4_START_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA4_CONFIG			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA4_DUMMY0			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA4_X_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrDMA4_X_MODIFY		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA4_Y_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrDMA4_Y_MODIFY		__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA4_CURR_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA4_CURR_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA4_IRQ_STATUS		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA4_PERIPHERAL_MAP	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA4_CURR_X_COUNT	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA4_DUMMY1			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA4_CURR_Y_COUNT	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA4_DUMMY2			__attribute__ ((aligned(4), section("DMA")));

volatile	void *	volatile	mmrDMA5_NEXT_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA5_START_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA5_CONFIG			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA5_DUMMY0			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA5_X_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrDMA5_X_MODIFY		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA5_Y_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrDMA5_Y_MODIFY		__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA5_CURR_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA5_CURR_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA5_IRQ_STATUS		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA5_PERIPHERAL_MAP	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA5_CURR_X_COUNT	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA5_DUMMY1			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA5_CURR_Y_COUNT	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA5_DUMMY2			__attribute__ ((aligned(4), section("DMA")));

volatile	void *	volatile	mmrDMA6_NEXT_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA6_START_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA6_CONFIG			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA6_DUMMY0			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA6_X_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrDMA6_X_MODIFY		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA6_Y_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrDMA6_Y_MODIFY		__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA6_CURR_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA6_CURR_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA6_IRQ_STATUS		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA6_PERIPHERAL_MAP	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA6_CURR_X_COUNT	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA6_DUMMY1			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA6_CURR_Y_COUNT	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA6_DUMMY2			__attribute__ ((aligned(4), section("DMA")));

volatile	void *	volatile	mmrDMA7_NEXT_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA7_START_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA7_CONFIG			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA7_DUMMY0			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA7_X_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrDMA7_X_MODIFY		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA7_Y_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrDMA7_Y_MODIFY		__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA7_CURR_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA7_CURR_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA7_IRQ_STATUS		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA7_PERIPHERAL_MAP	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA7_CURR_X_COUNT	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA7_DUMMY1			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA7_CURR_Y_COUNT	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA7_DUMMY2			__attribute__ ((aligned(4), section("DMA")));

volatile	void *	volatile	mmrDMA8_NEXT_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA8_START_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA8_CONFIG			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA8_DUMMY0			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA8_X_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrDMA8_X_MODIFY		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA8_Y_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrDMA8_Y_MODIFY		__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA8_CURR_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA8_CURR_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA8_IRQ_STATUS		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA8_PERIPHERAL_MAP	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA8_CURR_X_COUNT	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA8_DUMMY1			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA8_CURR_Y_COUNT	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA8_DUMMY2			__attribute__ ((aligned(4), section("DMA")));

volatile	void *	volatile	mmrDMA9_NEXT_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA9_START_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA9_CONFIG			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA9_DUMMY0			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA9_X_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrDMA9_X_MODIFY		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA9_Y_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrDMA9_Y_MODIFY		__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA9_CURR_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA9_CURR_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA9_IRQ_STATUS		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA9_PERIPHERAL_MAP	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA9_CURR_X_COUNT	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA9_DUMMY1			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA9_CURR_Y_COUNT	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA9_DUMMY2			__attribute__ ((aligned(4), section("DMA")));

volatile	void *	volatile	mmrDMA10_NEXT_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA10_START_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA10_CONFIG			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA10_DUMMY0			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA10_X_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrDMA10_X_MODIFY		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA10_Y_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrDMA10_Y_MODIFY		__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA10_CURR_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA10_CURR_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA10_IRQ_STATUS		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA10_PERIPHERAL_MAP	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA10_CURR_X_COUNT	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA10_DUMMY1			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA10_CURR_Y_COUNT	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA10_DUMMY2			__attribute__ ((aligned(4), section("DMA")));

volatile	void *	volatile	mmrDMA11_NEXT_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA11_START_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA11_CONFIG			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA11_DUMMY0			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA11_X_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrDMA11_X_MODIFY		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA11_Y_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrDMA11_Y_MODIFY		__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA11_CURR_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrDMA11_CURR_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA11_IRQ_STATUS		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA11_PERIPHERAL_MAP	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA11_CURR_X_COUNT	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA11_DUMMY1			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA11_CURR_Y_COUNT	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrDMA11_DUMMY2			__attribute__ ((aligned(4), section("DMA")));

volatile	void *	volatile	mmrMDMA_D0_NEXT_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrMDMA_D0_START_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_D0_CONFIG			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_D0_DUMMY0			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_D0_X_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrMDMA_D0_X_MODIFY			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_D0_Y_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrMDMA_D0_Y_MODIFY			__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrMDMA_D0_CURR_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrMDMA_D0_CURR_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_D0_IRQ_STATUS		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_D0_PERIPHERAL_MAP	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_D0_CURR_X_COUNT		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_D0_DUMMY1			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_D0_CURR_Y_COUNT		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_D0_DUMMY2			__attribute__ ((aligned(4), section("DMA")));

volatile	void *	volatile	mmrMDMA_S0_NEXT_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrMDMA_S0_START_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_S0_CONFIG			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_S0_DUMMY0			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_S0_X_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrMDMA_S0_X_MODIFY			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_S0_Y_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrMDMA_S0_Y_MODIFY			__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrMDMA_S0_CURR_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrMDMA_S0_CURR_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_S0_IRQ_STATUS		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_S0_PERIPHERAL_MAP	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_S0_CURR_X_COUNT		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_S0_DUMMY1			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_S0_CURR_Y_COUNT		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_S0_DUMMY2			__attribute__ ((aligned(4), section("DMA")));

volatile	void *	volatile	mmrMDMA_D1_NEXT_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrMDMA_D1_START_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_D1_CONFIG			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_D1_DUMMY0			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_D1_X_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrMDMA_D1_X_MODIFY			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_D1_Y_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrMDMA_D1_Y_MODIFY			__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrMDMA_D1_CURR_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrMDMA_D1_CURR_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_D1_IRQ_STATUS		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_D1_PERIPHERAL_MAP	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_D1_CURR_X_COUNT		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_D1_DUMMY1			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_D1_CURR_Y_COUNT		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_D1_DUMMY2			__attribute__ ((aligned(4), section("DMA")));

volatile	void *	volatile	mmrMDMA_S1_NEXT_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrMDMA_S1_START_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_S1_CONFIG			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_S1_DUMMY0			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_S1_X_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrMDMA_S1_X_MODIFY			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_S1_Y_COUNT			__attribute__ ((aligned(4), section("DMA")));
volatile	signed	short		mmrMDMA_S1_Y_MODIFY			__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrMDMA_S1_CURR_DESC_PTR	__attribute__ ((aligned(4), section("DMA")));
volatile	void *	volatile	mmrMDMA_S1_CURR_ADDR		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_S1_IRQ_STATUS		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_S1_PERIPHERAL_MAP	__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_S1_CURR_X_COUNT		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_S1_DUMMY1			__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_S1_CURR_Y_COUNT		__attribute__ ((aligned(4), section("DMA")));
volatile	unsigned	short	mmrMDMA_S1_DUMMY2			__attribute__ ((aligned(4), section("DMA")));


volatile	unsigned	short	mmrPPI_CONTROL	__attribute__ ((aligned(4), section("PPI")));
volatile	unsigned	short	mmrPPI_STATUS	__attribute__ ((aligned(4), section("PPI")));
volatile	unsigned	short	mmrPPI_COUNT	__attribute__ ((aligned(4), section("PPI")));
volatile	unsigned	short	mmrPPI_DELAY	__attribute__ ((aligned(4), section("PPI")));
volatile	unsigned	short	mmrPPI_FRAME	__attribute__ ((aligned(4), section("PPI")));


volatile unsigned short  mmrTWI_CLKDIV		__attribute__ ((aligned(4), section("TWI")));
volatile unsigned short  mmrTWI_CONTROL		__attribute__ ((aligned(4), section("TWI")));
volatile unsigned short  mmrTWI_SLAVE_CTL		__attribute__ ((aligned(4), section("TWI")));
volatile unsigned short  mmrTWI_SLAVE_STAT		__attribute__ ((aligned(4), section("TWI")));
volatile unsigned short  mmrTWI_SLAVE_ADDR		__attribute__ ((aligned(4), section("TWI")));
volatile unsigned short  mmrTWI_MASTER_CTL		__attribute__ ((aligned(4), section("TWI")));
volatile unsigned short  mmrTWI_MASTER_STAT	__attribute__ ((aligned(4), section("TWI")));
volatile unsigned short  mmrTWI_MASTER_ADDR	__attribute__ ((aligned(4), section("TWI")));
volatile unsigned short  mmrTWI_INT_STAT		__attribute__ ((aligned(4), section("TWI")));
volatile unsigned short  mmrTWI_INT_MASK		__attribute__ ((aligned(4), section("TWI")));
volatile unsigned short  mmrTWI_FIFO_CTL		__attribute__ ((aligned(4), section("TWI")));
volatile unsigned short  mmrTWI_FIFO_STAT		__attribute__ ((aligned(4), section("TWI")));
volatile unsigned short  mmrTWI_XMT_DATA8		__attribute__ ((aligned(4), section("TWIDATA")));
volatile unsigned short  mmrTWI_XMT_DATA16		__attribute__ ((aligned(4), section("TWIDATA")));
volatile unsigned short  mmrTWI_RCV_DATA8		__attribute__ ((aligned(4), section("TWIDATA")));
volatile unsigned short  mmrTWI_RCV_DATA16		__attribute__ ((aligned(4), section("TWIDATA")));



volatile unsigned short  mmrPORTGIO	 	__attribute__ ((aligned(4), section("FLAGG")));
volatile unsigned short  mmrPORTGIO_CLEAR	 	__attribute__ ((aligned(4), section("FLAGG")));
volatile unsigned short  mmrPORTGIO_SET	 	__attribute__ ((aligned(4), section("FLAGG")));
volatile unsigned short  mmrPORTGIO_TOGGLE 	__attribute__ ((aligned(4), section("FLAGG")));
volatile unsigned short  mmrPORTGIO_MASKA		__attribute__ ((aligned(4), section("FLAGG")));
volatile unsigned short  mmrPORTGIO_MASKA_CLEAR	__attribute__ ((aligned(4), section("FLAGG")));
volatile unsigned short  mmrPORTGIO_MASKA_SET	__attribute__ ((aligned(4), section("FLAGG")));
volatile unsigned short  mmrPORTGIO_MASKA_TOGGLE	__attribute__ ((aligned(4), section("FLAGG")));
volatile unsigned short  mmrPORTGIO_MASKB		__attribute__ ((aligned(4), section("FLAGG")));
volatile unsigned short  mmrPORTGIO_MASKB_CLEAR	__attribute__ ((aligned(4), section("FLAGG")));
volatile unsigned short  mmrPORTGIO_MASKB_SET	__attribute__ ((aligned(4), section("FLAGG")));
volatile unsigned short  mmrPORTGIO_MASKB_TOGGLE	__attribute__ ((aligned(4), section("FLAGG")));
volatile unsigned short  mmrPORTGIO_DIR 		__attribute__ ((aligned(4), section("FLAGG")));
volatile unsigned short  mmrPORTGIO_POLAR 		__attribute__ ((aligned(4), section("FLAGG")));
volatile unsigned short  mmrPORTGIO_EDGE 		__attribute__ ((aligned(4), section("FLAGG")));
volatile unsigned short  mmrPORTGIO_BOTH 		__attribute__ ((aligned(4), section("FLAGG")));
volatile unsigned short  mmrPORTGIO_INEN 		__attribute__ ((aligned(4), section("FLAGG")));



volatile unsigned short  mmrPORTHIO	 	__attribute__ ((aligned(4), section("FLAGH")));
volatile unsigned short  mmrPORTHIO_CLEAR	 	__attribute__ ((aligned(4), section("FLAGH")));
volatile unsigned short  mmrPORTHIO_SET	 	__attribute__ ((aligned(4), section("FLAGH")));
volatile unsigned short  mmrPORTHIO_TOGGLE 	__attribute__ ((aligned(4), section("FLAGH")));
volatile unsigned short  mmrPORTHIO_MASKA		__attribute__ ((aligned(4), section("FLAGH")));
volatile unsigned short  mmrPORTHIO_MASKA_CLEAR	__attribute__ ((aligned(4), section("FLAGH")));
volatile unsigned short  mmrPORTHIO_MASKA_SET	__attribute__ ((aligned(4), section("FLAGH")));
volatile unsigned short  mmrPORTHIO_MASKA_TOGGLE	__attribute__ ((aligned(4), section("FLAGH")));
volatile unsigned short  mmrPORTHIO_MASKB		__attribute__ ((aligned(4), section("FLAGH")));
volatile unsigned short  mmrPORTHIO_MASKB_CLEAR	__attribute__ ((aligned(4), section("FLAGH")));
volatile unsigned short  mmrPORTHIO_MASKB_SET	__attribute__ ((aligned(4), section("FLAGH")));
volatile unsigned short  mmrPORTHIO_MASKB_TOGGLE	__attribute__ ((aligned(4), section("FLAGH")));
volatile unsigned short  mmrPORTHIO_DIR 		__attribute__ ((aligned(4), section("FLAGH")));
volatile unsigned short  mmrPORTHIO_POLAR 		__attribute__ ((aligned(4), section("FLAGH")));
volatile unsigned short  mmrPORTHIO_EDGE 		__attribute__ ((aligned(4), section("FLAGH")));
volatile unsigned short  mmrPORTHIO_BOTH 		__attribute__ ((aligned(4), section("FLAGH")));
volatile unsigned short  mmrPORTHIO_INEN 		__attribute__ ((aligned(4), section("FLAGH")));



volatile unsigned short  mmrUART1_THR 		__attribute__ ((aligned(4), section("UART1")));
volatile unsigned short  mmrUART1_IER 		__attribute__ ((aligned(4), section("UART1")));
volatile unsigned short  mmrUART1_IIR 		__attribute__ ((aligned(4), section("UART1")));
volatile unsigned short  mmrUART1_LCR 		__attribute__ ((aligned(4), section("UART1")));
volatile unsigned short  mmrUART1_MCR 		__attribute__ ((aligned(4), section("UART1")));
volatile unsigned short  mmrUART1_LSR 		__attribute__ ((aligned(4), section("UART1")));
volatile unsigned short  mmrUART1_MSR		__attribute__ ((aligned(4), section("UART1")));
volatile unsigned short  mmrUART1_SCR 		__attribute__ ((aligned(4), section("UART1")));
volatile unsigned short  mmrUART1_DUMMY0	__attribute__ ((aligned(4), section("UART1")));
volatile unsigned short  mmrUART1_GCTL 		__attribute__ ((aligned(4), section("UART1")));




volatile unsigned short  mmrCAN_MC1		__attribute__ ((aligned(4), section("CANCTL0")));
volatile unsigned short  mmrCAN_MD1		__attribute__ ((aligned(4), section("CANCTL0")));
volatile unsigned short  mmrCAN_TRS1		__attribute__ ((aligned(4), section("CANCTL0")));
volatile unsigned short  mmrCAN_TRR1		__attribute__ ((aligned(4), section("CANCTL0")));
volatile unsigned short  mmrCAN_TA1		__attribute__ ((aligned(4), section("CANCTL0")));
volatile unsigned short  mmrCAN_AA1		__attribute__ ((aligned(4), section("CANCTL0")));
volatile unsigned short  mmrCAN_RMP1		__attribute__ ((aligned(4), section("CANCTL0")));
volatile unsigned short  mmrCAN_RML1		__attribute__ ((aligned(4), section("CANCTL0")));
volatile unsigned short  mmrCAN_MBTIF1		__attribute__ ((aligned(4), section("CANCTL0")));
volatile unsigned short  mmrCAN_MBRIF1		__attribute__ ((aligned(4), section("CANCTL0")));
volatile unsigned short  mmrCAN_MBIM1		__attribute__ ((aligned(4), section("CANCTL0")));
volatile unsigned short  mmrCAN_RFH1		__attribute__ ((aligned(4), section("CANCTL0")));
volatile unsigned short  mmrCAN_OPSS1		__attribute__ ((aligned(4), section("CANCTL0")));


volatile unsigned short  mmrCAN_MC2		__attribute__ ((aligned(4), section("CANCTL1")));
volatile unsigned short  mmrCAN_MD2		__attribute__ ((aligned(4), section("CANCTL1")));
volatile unsigned short  mmrCAN_TRS2		__attribute__ ((aligned(4), section("CANCTL1")));
volatile unsigned short  mmrCAN_TRR2		__attribute__ ((aligned(4), section("CANCTL1")));
volatile unsigned short  mmrCAN_TA2		__attribute__ ((aligned(4), section("CANCTL1")));
volatile unsigned short  mmrCAN_AA2		__attribute__ ((aligned(4), section("CANCTL1")));
volatile unsigned short  mmrCAN_RMP2		__attribute__ ((aligned(4), section("CANCTL1")));
volatile unsigned short  mmrCAN_RML2		__attribute__ ((aligned(4), section("CANCTL1")));
volatile unsigned short  mmrCAN_MBTIF2		__attribute__ ((aligned(4), section("CANCTL1")));
volatile unsigned short  mmrCAN_MBRIF2		__attribute__ ((aligned(4), section("CANCTL1")));
volatile unsigned short  mmrCAN_MBIM2		__attribute__ ((aligned(4), section("CANCTL1")));
volatile unsigned short  mmrCAN_RFH2		__attribute__ ((aligned(4), section("CANCTL1")));
volatile unsigned short  mmrCAN_OPSS2		__attribute__ ((aligned(4), section("CANCTL1")));

volatile unsigned short  mmrCAN_CLOCK		__attribute__ ((aligned(4), section("CANCTL2")));
volatile unsigned short  mmrCAN_TIMING		__attribute__ ((aligned(4), section("CANCTL2")));
volatile unsigned short  mmrCAN_DEBUG		__attribute__ ((aligned(4), section("CANCTL2")));
volatile unsigned short  mmrCAN_STATUS		__attribute__ ((aligned(4), section("CANCTL2")));
volatile unsigned short  mmrCAN_CEC		__attribute__ ((aligned(4), section("CANCTL2")));
volatile unsigned short  mmrCAN_GIS		__attribute__ ((aligned(4), section("CANCTL2")));
volatile unsigned short  mmrCAN_GIM		__attribute__ ((aligned(4), section("CANCTL2")));
volatile unsigned short  mmrCAN_GIF		__attribute__ ((aligned(4), section("CANCTL2")));
volatile unsigned short  mmrCAN_CONTROL		__attribute__ ((aligned(4), section("CANCTL2")));
volatile unsigned short  mmrCAN_INTR		__attribute__ ((aligned(4), section("CANCTL2")));
volatile unsigned short  mmrCAN_DUMMY0	__attribute__ ((aligned(4), section("CANCTL2")));
volatile unsigned short  mmrCAN_MBTD		__attribute__ ((aligned(4), section("CANCTL2")));
volatile unsigned short  mmrCAN_EWR		__attribute__ ((aligned(4), section("CANCTL2")));
volatile unsigned short  mmrCAN_ESR		__attribute__ ((aligned(4), section("CANCTL2")));
volatile unsigned short  mmrCAN_DUMMY1	__attribute__ ((aligned(4), section("CANCTL2")));
volatile unsigned short  mmrCAN_DUMMY2	__attribute__ ((aligned(4), section("CANCTL2")));
volatile unsigned short  mmrCAN_UCREG		__attribute__ ((aligned(4), section("CANCTL2")));
volatile unsigned short  mmrCAN_UCCNT		__attribute__ ((aligned(4), section("CANCTL2")));
volatile unsigned short  mmrCAN_UCRC		__attribute__ ((aligned(4), section("CANCTL2")));
volatile unsigned short  mmrCAN_UCCNF		__attribute__ ((aligned(4), section("CANCTL2")));


volatile unsigned short  mmrCAN_AM00L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM00H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM01L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM01H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM02L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM02H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM03L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM03H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM04L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM04H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM05L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM05H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM06L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM06H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM07L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM07H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM08L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM08H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM09L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM09H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM10L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM10H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM11L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM11H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM12L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM12H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM13L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM13H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM14L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM14H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM15L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM15H		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_AM16L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM16H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM17L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM17H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM18L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM18H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM19L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM19H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM20L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM20H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM21L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM21H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM22L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM22H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM23L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM23H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM24L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM24H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM25L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM25H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM26L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM26H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM27L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM27H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM28L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM28H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM29L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM29H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM30L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM30H		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM31L		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_AM31H		__attribute__ ((aligned(4), section("CANMBX")));


volatile unsigned short  mmrCAN_MB00_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB00_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB00_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB00_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB00_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB00_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB00_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB00_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB01_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB01_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB01_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB01_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB01_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB01_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB01_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB01_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB02_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB02_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB02_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB02_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB02_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB02_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB02_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB02_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB03_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB03_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB03_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB03_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB03_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB03_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB03_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB03_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB04_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB04_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB04_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB04_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB04_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB04_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB04_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB04_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB05_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB05_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB05_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB05_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB05_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB05_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB05_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB05_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB06_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB06_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB06_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB06_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB06_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB06_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB06_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB06_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB07_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB07_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB07_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB07_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB07_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB07_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB07_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB07_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB08_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB08_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB08_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB08_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB08_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB08_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB08_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB08_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB09_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB09_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB09_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB09_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB09_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB09_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB09_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB09_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB10_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB10_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB10_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB10_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB10_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB10_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB10_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB10_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB11_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB11_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB11_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB11_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB11_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB11_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB11_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB11_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB12_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB12_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB12_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB12_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB12_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB12_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB12_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB12_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB13_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB13_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB13_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB13_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB13_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB13_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB13_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB13_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB14_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB14_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB14_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB14_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB14_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB14_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB14_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB14_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB15_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB15_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB15_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB15_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB15_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB15_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB15_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB15_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB16_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB16_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB16_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB16_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB16_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB16_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB16_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB16_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB17_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB17_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB17_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB17_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB17_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB17_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB17_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB17_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB18_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB18_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB18_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB18_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB18_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB18_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB18_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB18_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB19_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB19_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB19_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB19_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB19_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB19_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB19_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB19_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB20_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB20_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB20_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB20_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB20_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB20_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB20_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB20_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB21_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB21_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB21_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB21_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB21_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB21_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB21_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB21_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB22_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB22_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB22_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB22_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB22_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB22_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB22_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB22_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB23_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB23_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB23_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB23_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB23_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB23_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB23_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB23_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB24_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB24_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB24_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB24_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB24_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB24_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB24_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB24_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB25_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB25_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB25_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB25_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB25_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB25_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB25_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB25_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB26_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB26_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB26_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB26_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB26_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB26_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB26_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB26_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB27_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB27_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB27_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB27_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB27_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB27_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB27_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB27_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB28_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB28_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB28_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB28_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB28_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB28_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB28_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB28_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB29_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB29_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB29_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB29_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB29_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB29_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB29_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB29_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB30_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB30_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB30_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB30_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB30_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB30_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB30_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB30_ID1		__attribute__ ((aligned(4), section("CANMBX")));

volatile unsigned short  mmrCAN_MB31_DATA0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB31_DATA1		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB31_DATA2		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB31_DATA3		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB31_LENGTH	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB31_TIMESTAMP	__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB31_ID0		__attribute__ ((aligned(4), section("CANMBX")));
volatile unsigned short  mmrCAN_MB31_ID1		__attribute__ ((aligned(4), section("CANMBX")));


volatile unsigned short  mmrPORTF_FER		__attribute__ ((aligned(4), section("PORTFER")));
volatile unsigned short  mmrPORTG_FER		__attribute__ ((aligned(4), section("PORTFER")));
volatile unsigned short  mmrPORTH_FER		__attribute__ ((aligned(4), section("PORTFER")));
volatile unsigned short  mmrPORT_MUX		__attribute__ ((aligned(4), section("PORTFER")));

volatile unsigned short  mmrHMDMA0_CONTROL		__attribute__ ((aligned(4), section("HDMA0")));
volatile unsigned short  mmrHMDMA0_ECINIT		__attribute__ ((aligned(4), section("HDMA0")));
volatile unsigned short  mmrHMDMA0_BCINIT		__attribute__ ((aligned(4), section("HDMA0")));
volatile unsigned short  mmrHMDMA0_ECURGENT	__attribute__ ((aligned(4), section("HDMA0")));
volatile unsigned short  mmrHMDMA0_ECOVERFLOW	__attribute__ ((aligned(4), section("HDMA0")));
volatile unsigned short  mmrHMDMA0_ECOUNT		__attribute__ ((aligned(4), section("HDMA0")));
volatile unsigned short  mmrHMDMA0_BCOUNT		__attribute__ ((aligned(4), section("HDMA0")));

volatile unsigned short  mmrHMDMA1_CONTROL		__attribute__ ((aligned(4), section("HDMA1")));
volatile unsigned short  mmrHMDMA1_ECINIT		__attribute__ ((aligned(4), section("HDMA1")));
volatile unsigned short  mmrHMDMA1_BCINIT		__attribute__ ((aligned(4), section("HDMA1")));
volatile unsigned short  mmrHMDMA1_ECURGENT	__attribute__ ((aligned(4), section("HDMA1")));
volatile unsigned short  mmrHMDMA1_ECOVERFLOW	__attribute__ ((aligned(4), section("HDMA1")));
volatile unsigned short  mmrHMDMA1_ECOUNT		__attribute__ ((aligned(4), section("HDMA1")));
volatile unsigned short  mmrHMDMA1_BCOUNT		__attribute__ ((aligned(4), section("HDMA1")));


volatile unsigned long 	mmrEMAC_OPMODE		__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_ADDRLO		__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_ADDRHI		__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_HASHLO		__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_HASHHI		__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_STAADD		__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_STADAT		__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_FLC			__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_VLAN1			__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_VLAN2			__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_DUMMY0		__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_WKUP_CTL		__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_WKUP_FFMSK0	__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_WKUP_FFMSK1	__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_WKUP_FFMSK2	__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_WKUP_FFMSK3	__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_WKUP_FFCMD	__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_WKUP_FFOFF	__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_WKUP_FFCRC0	__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_WKUP_FFCRC1	__attribute__ ((aligned(4), section("EMAC")));

volatile unsigned long  mmrEMAC_DUMMY1		__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_DUMMY2		__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_DUMMY3		__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_DUMMY4		__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long mmrEMAC_SYSCTL			__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_SYSTAT		__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_RX_STAT		__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_RX_STKY		__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_RX_IRQE		__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_TX_STAT		__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_TX_STKY		__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_TX_IRQE		__attribute__ ((aligned(4), section("EMAC")));

volatile unsigned long  mmrEMAC_MMC_CTL		__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_MMC_RIRQS		__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_MMC_RIRQE		__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_MMC_TIRQS		__attribute__ ((aligned(4), section("EMAC")));
volatile unsigned long  mmrEMAC_MMC_TIRQE		__attribute__ ((aligned(4), section("EMAC")));


volatile unsigned long  mmrEMAC_RXC_OK		__attribute__ ((aligned(4), section("EMACR")));
volatile unsigned long  mmrEMAC_RXC_FCS		__attribute__ ((aligned(4), section("EMACR")));
volatile unsigned long  mmrEMAC_RXC_ALIGN		__attribute__ ((aligned(4), section("EMACR")));
volatile unsigned long  mmrEMAC_RXC_OCTET		__attribute__ ((aligned(4), section("EMACR")));
volatile unsigned long  mmrEMAC_RXC_DMAOVF	__attribute__ ((aligned(4), section("EMACR")));
volatile unsigned long  mmrEMAC_RXC_UNICST	__attribute__ ((aligned(4), section("EMACR")));
volatile unsigned long  mmrEMAC_RXC_MULTI		__attribute__ ((aligned(4), section("EMACR")));
volatile unsigned long  mmrEMAC_RXC_BROAD		__attribute__ ((aligned(4), section("EMACR")));
volatile unsigned long  mmrEMAC_RXC_LNERRI	__attribute__ ((aligned(4), section("EMACR")));
volatile unsigned long  mmrEMAC_RXC_LNERRO	__attribute__ ((aligned(4), section("EMACR")));
volatile unsigned long  mmrEMAC_RXC_LONG		__attribute__ ((aligned(4), section("EMACR")));
volatile unsigned long  mmrEMAC_RXC_MACCTL	__attribute__ ((aligned(4), section("EMACR")));
volatile unsigned long  mmrEMAC_RXC_OPCODE	__attribute__ ((aligned(4), section("EMACR")));
volatile unsigned long  mmrEMAC_RXC_PAUSE		__attribute__ ((aligned(4), section("EMACR")));
volatile unsigned long  mmrEMAC_RXC_ALLFRM	__attribute__ ((aligned(4), section("EMACR")));
volatile unsigned long  mmrEMAC_RXC_ALLOCT	__attribute__ ((aligned(4), section("EMACR")));
volatile unsigned long  mmrEMAC_RXC_TYPED		__attribute__ ((aligned(4), section("EMACR")));
volatile unsigned long  mmrEMAC_RXC_SHORT		__attribute__ ((aligned(4), section("EMACR")));
volatile unsigned long  mmrEMAC_RXC_EQ64		__attribute__ ((aligned(4), section("EMACR")));
volatile unsigned long 	mmrEMAC_RXC_LT128		__attribute__ ((aligned(4), section("EMACR")));
volatile unsigned long  mmrEMAC_RXC_LT256		__attribute__ ((aligned(4), section("EMACR")));
volatile unsigned long  mmrEMAC_RXC_LT512		__attribute__ ((aligned(4), section("EMACR")));
volatile unsigned long  mmrEMAC_RXC_LT1024	__attribute__ ((aligned(4), section("EMACR")));
volatile unsigned long  mmrEMAC_RXC_GE1024	__attribute__ ((aligned(4), section("EMACR")));

volatile unsigned long  mmrEMAC_TXC_OK		__attribute__ ((aligned(4), section("EMACT")));
volatile unsigned long  mmrEMAC_TXC_1COL		__attribute__ ((aligned(4), section("EMACT")));
volatile unsigned long  mmrEMAC_TXC_GT1COL	__attribute__ ((aligned(4), section("EMACT")));
volatile unsigned long  mmrEMAC_TXC_OCTET		__attribute__ ((aligned(4), section("EMACT")));
volatile unsigned long  mmrEMAC_TXC_DEFER		__attribute__ ((aligned(4), section("EMACT")));
volatile unsigned long  mmrEMAC_TXC_LATECL	__attribute__ ((aligned(4), section("EMACT")));
volatile unsigned long  mmrEMAC_TXC_XS_COL	__attribute__ ((aligned(4), section("EMACT")));
volatile unsigned long  mmrEMAC_TXC_DMAUND	__attribute__ ((aligned(4), section("EMACT")));
volatile unsigned long  mmrEMAC_TXC_CRSERR	__attribute__ ((aligned(4), section("EMACT")));
volatile unsigned long  mmrEMAC_TXC_UNICST	__attribute__ ((aligned(4), section("EMACT")));
volatile unsigned long  mmrEMAC_TXC_MULTI		__attribute__ ((aligned(4), section("EMACT")));
volatile unsigned long  mmrEMAC_TXC_BROAD		__attribute__ ((aligned(4), section("EMACT")));
volatile unsigned long  mmrEMAC_TXC_XS_DFR	__attribute__ ((aligned(4), section("EMACT")));
volatile unsigned long  mmrEMAC_TXC_MACCTL	__attribute__ ((aligned(4), section("EMACT")));
volatile unsigned long  mmrEMAC_TXC_ALLFRM	__attribute__ ((aligned(4), section("EMACT")));
volatile unsigned long  mmrEMAC_TXC_ALLOCT	__attribute__ ((aligned(4), section("EMACT")));
volatile unsigned long  mmrEMAC_TXC_EQ64		__attribute__ ((aligned(4), section("EMACT")));
volatile unsigned long  mmrEMAC_TXC_LT128		__attribute__ ((aligned(4), section("EMACT")));
volatile unsigned long  mmrEMAC_TXC_LT256		__attribute__ ((aligned(4), section("EMACT")));
volatile unsigned long  mmrEMAC_TXC_LT512		__attribute__ ((aligned(4), section("EMACT")));
volatile unsigned long  mmrEMAC_TXC_LT1024	__attribute__ ((aligned(4), section("EMACT")));
volatile unsigned long  mmrEMAC_TXC_GE1024	__attribute__ ((aligned(4), section("EMACT")));
volatile unsigned long  mmrEMAC_TXC_ABORT		__attribute__ ((aligned(4), section("EMACT")));

