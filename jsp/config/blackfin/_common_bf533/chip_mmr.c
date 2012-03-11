/*
 * ADSP-BF533の System MMR 定義。アドレスの指定は .ldファイルとの組み合わせで行う。
 * .ldファイルのサンプルは、 config/blackfin/bf533cbを参照。
 *
 * アドレスが正しく割り当てられているかは、次の方法で検証した.
 * (Makefileの$(OBJCOPY) -R .sysmmr -R .coremmr jsp をコメントアウトすること)
 *
$ readelf -s  -W jsp | grep mmr | grep -v DUMMY | grep -v blackfin | sort -k 2 | awk '/_mmr/{print $8, $2}' -  | sed 's/_mmr//p' | uniq > regelf.txt
$ cat ../jsp/config/blackfin/def_lpblackfin.h ../jsp/config/blackfin/defbf532.h | grep '0xFF[CE]0'  | grep "#define" | sed 's/_MA /_MAP /' - | awk '{print $2,$3}' - | sort -k 2 | sed 's/0x//' - > regdef.txt
$ diff -i -w regelf.txt regdef.txt
24a25,27
> RTC_PREN FFC00314
> UART_DLL FFC00400
> UART_RBR FFC00400
25a29
> UART_DLH FFC00404
123a128
> DMA_TCPER FFC00B0C
124a130
> DMA_TCCNT FFC00B10
288a295
> DCPLB_FAULT_STATUS FFE00008
326a334
> CODE_FAULT_STATUS FFE01008
327a336
> CODE_FAULT_ADDR FFE0100C


 *
 * つまり、いくつかのレジスタの宣言がELF側から抜けていることになる。ここで、
 * それぞれのレジスタは別のレジスタのエイリアスであることに注目する。
 * RTC_PREN -> RTC_FAST
 * UART_DLL -> UART_THR
 * UART_RBR -> UART_THR
 * UART_DLH -> UART_IER
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
volatile unsigned short mmrPLL_LOCKCNT __attribute__ ((aligned(4), section("PWRMGMT")));
volatile unsigned short mmrCHIPID	__attribute__ ((aligned(4), section("PWRMGMT")));

volatile unsigned short mmrSWRST 		__attribute__ ((aligned(4), section("SYSRST")));
volatile unsigned short mmrSYSCR 		__attribute__ ((aligned(4), section("SYSRST")));
volatile unsigned long  mmrSIC_RVECT		__attribute__ ((aligned(4), section("SYSRST")));
volatile unsigned long  mmrSIC_IMASK 		__attribute__ ((aligned(4), section("SYSRST")));
volatile unsigned long  mmrSIC_IAR0 		__attribute__ ((aligned(4), section("SYSRST")));
volatile unsigned long  mmrSIC_IAR1 		__attribute__ ((aligned(4), section("SYSRST")));
volatile unsigned long  mmrSIC_IAR2 		__attribute__ ((aligned(4), section("SYSRST")));
volatile unsigned long  mmrSIC_IARDUMMY	__attribute__ ((aligned(4), section("SYSRST")));
volatile unsigned long  mmrSIC_ISR 		__attribute__ ((aligned(4), section("SYSRST")));
volatile unsigned long  mmrSIC_IWR 		__attribute__ ((aligned(4), section("SYSRST")));

volatile unsigned short mmrWDOG_CTL  __attribute__ ((aligned(4), section("WDOG")));
volatile unsigned long  mmrWDOG_CNT  __attribute__ ((aligned(4), section("WDOG")));
volatile unsigned long  mmrWDOG_STAT __attribute__ ((aligned(4), section("WDOG")));

volatile unsigned long  mmrRTC_STAT  __attribute__ ((aligned(4), section("RTC")));
volatile unsigned short mmrRTC_ICTL  __attribute__ ((aligned(4), section("RTC")));
volatile unsigned short mmrRTC_ISTAT __attribute__ ((aligned(4), section("RTC")));
volatile unsigned short mmrRTC_SWCNT __attribute__ ((aligned(4), section("RTC")));
volatile unsigned long  mmrRTC_ALARM __attribute__ ((aligned(4), section("RTC")));
volatile unsigned short mmrRTC_FAST  __attribute__ ((aligned(4), section("RTC")));

volatile unsigned short mmrUART_THR    __attribute__ ((aligned(4), section("UART")));
volatile unsigned short mmrUART_IER    __attribute__ ((aligned(4), section("UART")));
volatile unsigned short mmrUART_IIR    __attribute__ ((aligned(4), section("UART")));
volatile unsigned short mmrUART_LCR    __attribute__ ((aligned(4), section("UART")));
volatile unsigned short mmrUART_MCR    __attribute__ ((aligned(4), section("UART")));
volatile unsigned short mmrUART_LSR    __attribute__ ((aligned(4), section("UART")));
volatile unsigned short mmrUART_MSR	__attribute__ ((aligned(4), section("UART")));
volatile unsigned short mmrUART_SCR    __attribute__ ((aligned(4), section("UART")));
volatile unsigned short mmrUART_DUMMY0 __attribute__ ((aligned(4), section("UART")));
volatile unsigned short mmrUART_GCTL   __attribute__ ((aligned(4), section("UART")));

volatile unsigned short mmrSPI_CTL    __attribute__ ((aligned(4), section("SPI")));
volatile unsigned short mmrSPI_FLG    __attribute__ ((aligned(4), section("SPI")));
volatile unsigned short mmrSPI_STAT   __attribute__ ((aligned(4), section("SPI")));
volatile unsigned short mmrSPI_TDBR   __attribute__ ((aligned(4), section("SPI")));
volatile unsigned short mmrSPI_RDBR   __attribute__ ((aligned(4), section("SPI")));
volatile unsigned short mmrSPI_BAUD   __attribute__ ((aligned(4), section("SPI")));
volatile unsigned short mmrSPI_SHADOW __attribute__ ((aligned(4), section("SPI")));

volatile	unsigned	short	mmrTIMER0_CONFIG	__attribute__ ((aligned(4), section("TIMER")));
volatile	unsigned	long	mmrTIMER0_COUNTER	__attribute__ ((aligned(4), section("TIMER")));
volatile	unsigned	long	mmrTIMER0_PERIOD	__attribute__ ((aligned(4), section("TIMER")));
volatile	unsigned	long	mmrTIMER0_WIDTH	__attribute__ ((aligned(4), section("TIMER")));

volatile	unsigned	short	mmrTIMER1_CONFIG	__attribute__ ((aligned(4), section("TIMER")));
volatile	unsigned	long	mmrTIMER1_COUNTER	__attribute__ ((aligned(4), section("TIMER")));
volatile	unsigned	long	mmrTIMER1_PERIOD	__attribute__ ((aligned(4), section("TIMER")));
volatile	unsigned	long	mmrTIMER1_WIDTH	__attribute__ ((aligned(4), section("TIMER")));

volatile	unsigned	short	mmrTIMER2_CONFIG	__attribute__ ((aligned(4), section("TIMER")));
volatile	unsigned	long	mmrTIMER2_COUNTER	__attribute__ ((aligned(4), section("TIMER")));
volatile	unsigned	long	mmrTIMER2_PERIOD	__attribute__ ((aligned(4), section("TIMER")));
volatile	unsigned	long	mmrTIMER2_WIDTH	__attribute__ ((aligned(4), section("TIMER")));

volatile	unsigned	short	mmrTIMER_ENABLE	__attribute__ ((aligned(4), section("TMRCTL")));
volatile	unsigned	short	mmrTIMER_DISABLE	__attribute__ ((aligned(4), section("TMRCTL")));
volatile	unsigned	short	mmrTIMER_STATUS	__attribute__ ((aligned(4), section("TMRCTL")));


volatile	unsigned	short	mmrFIO_FLAG_D		__attribute__ ((aligned(4), section("FLAG")));
volatile	unsigned	short	mmrFIO_FLAG_C		__attribute__ ((aligned(4), section("FLAG")));
volatile	unsigned	short	mmrFIO_FLAG_S		__attribute__ ((aligned(4), section("FLAG")));
volatile	unsigned	short	mmrFIO_FLAG_T		__attribute__ ((aligned(4), section("FLAG")));
volatile	unsigned	short	mmrFIO_MASKA_D	__attribute__ ((aligned(4), section("FLAG")));
volatile	unsigned	short	mmrFIO_MASKA_C	__attribute__ ((aligned(4), section("FLAG")));
volatile	unsigned	short	mmrFIO_MASKA_S	__attribute__ ((aligned(4), section("FLAG")));
volatile	unsigned	short	mmrFIO_MASKA_T	__attribute__ ((aligned(4), section("FLAG")));
volatile	unsigned	short	mmrFIO_MASKB_D	__attribute__ ((aligned(4), section("FLAG")));
volatile	unsigned	short	mmrFIO_MASKB_C	__attribute__ ((aligned(4), section("FLAG")));
volatile	unsigned	short	mmrFIO_MASKB_S	__attribute__ ((aligned(4), section("FLAG")));
volatile	unsigned	short	mmrFIO_MASKB_T	__attribute__ ((aligned(4), section("FLAG")));
volatile	unsigned	short	mmrFIO_DIR		__attribute__ ((aligned(4), section("FLAG")));
volatile	unsigned	short	mmrFIO_POLAR		__attribute__ ((aligned(4), section("FLAG")));
volatile	unsigned	short	mmrFIO_EDGE		__attribute__ ((aligned(4), section("FLAG")));
volatile	unsigned	short	mmrFIO_BOTH		__attribute__ ((aligned(4), section("FLAG")));
volatile	unsigned	short	mmrFIO_INEN		__attribute__ ((aligned(4), section("FLAG")));



volatile	unsigned	short	mmrSPORT0_TCR1	__attribute__ ((aligned(4), section("SPORT0")));
volatile	unsigned	short	mmrSPORT0_TCR2	__attribute__ ((aligned(4), section("SPORT0")));
volatile	unsigned	short	mmrSPORT0_TCLKDIV	__attribute__ ((aligned(4), section("SPORT0")));
volatile	unsigned	short	mmrSPORT0_TFSDIV	__attribute__ ((aligned(4), section("SPORT0")));
volatile	long				mmrSPORT0_TX		__attribute__ ((aligned(4), section("SPORT0")));
volatile	long				mmrSPORT0_DUMMY0	__attribute__ ((aligned(4), section("SPORT0")));
volatile	long				mmrSPORT0_RX		__attribute__ ((aligned(4), section("SPORT0")));
volatile	long				mmrSPORT0_DUMMY1	__attribute__ ((aligned(4), section("SPORT0")));
volatile	unsigned	short	mmrSPORT0_RCR1	__attribute__ ((aligned(4), section("SPORT0")));
volatile	unsigned	short	mmrSPORT0_RCR2	__attribute__ ((aligned(4), section("SPORT0")));
volatile	unsigned	short	mmrSPORT0_RCLKDIV	__attribute__ ((aligned(4), section("SPORT0")));
volatile	unsigned	short	mmrSPORT0_RFSDIV	__attribute__ ((aligned(4), section("SPORT0")));
volatile	unsigned	short	mmrSPORT0_STAT	__attribute__ ((aligned(4), section("SPORT0")));
volatile	unsigned	short	mmrSPORT0_CHNL	__attribute__ ((aligned(4), section("SPORT0")));
volatile	unsigned	short	mmrSPORT0_MCMC1	__attribute__ ((aligned(4), section("SPORT0")));
volatile	unsigned	short	mmrSPORT0_MCMC2	__attribute__ ((aligned(4), section("SPORT0")));
volatile	unsigned	long	mmrSPORT0_MTCS0	__attribute__ ((aligned(4), section("SPORT0")));
volatile	unsigned	long	mmrSPORT0_MTCS1	__attribute__ ((aligned(4), section("SPORT0")));
volatile	unsigned	long	mmrSPORT0_MTCS2	__attribute__ ((aligned(4), section("SPORT0")));
volatile	unsigned	long	mmrSPORT0_MTCS3	__attribute__ ((aligned(4), section("SPORT0")));
volatile	unsigned	long	mmrSPORT0_MRCS0	__attribute__ ((aligned(4), section("SPORT0")));
volatile	unsigned	long	mmrSPORT0_MRCS1	__attribute__ ((aligned(4), section("SPORT0")));
volatile	unsigned	long	mmrSPORT0_MRCS2	__attribute__ ((aligned(4), section("SPORT0")));
volatile	unsigned	long	mmrSPORT0_MRCS3	__attribute__ ((aligned(4), section("SPORT0")));


volatile	unsigned	short	mmrSPORT1_TCR1	__attribute__ ((aligned(4), section("SPORT1")));
volatile	unsigned	short	mmrSPORT1_TCR2	__attribute__ ((aligned(4), section("SPORT1")));
volatile	unsigned	short	mmrSPORT1_TCLKDIV	__attribute__ ((aligned(4), section("SPORT1")));
volatile	unsigned	short	mmrSPORT1_TFSDIV	__attribute__ ((aligned(4), section("SPORT1")));
volatile	long				mmrSPORT1_TX		__attribute__ ((aligned(4), section("SPORT1")));
volatile	long				mmrSPORT1_DUMMY0	__attribute__ ((aligned(4), section("SPORT1")));
volatile	long				mmrSPORT1_RX		__attribute__ ((aligned(4), section("SPORT1")));
volatile	long				mmrSPORT1_DUMMY1	__attribute__ ((aligned(4), section("SPORT1")));
volatile	unsigned	short	mmrSPORT1_RCR1	__attribute__ ((aligned(4), section("SPORT1")));
volatile	unsigned	short	mmrSPORT1_RCR2	__attribute__ ((aligned(4), section("SPORT1")));
volatile	unsigned	short	mmrSPORT1_RCLKDIV	__attribute__ ((aligned(4), section("SPORT1")));
volatile	unsigned	short	mmrSPORT1_RFSDIV	__attribute__ ((aligned(4), section("SPORT1")));
volatile	unsigned	short	mmrSPORT1_STAT	__attribute__ ((aligned(4), section("SPORT1")));
volatile	unsigned	short	mmrSPORT1_CHNL	__attribute__ ((aligned(4), section("SPORT1")));
volatile	unsigned	short	mmrSPORT1_MCMC1	__attribute__ ((aligned(4), section("SPORT1")));
volatile	unsigned	short	mmrSPORT1_MCMC2	__attribute__ ((aligned(4), section("SPORT1")));
volatile	unsigned	long	mmrSPORT1_MTCS0	__attribute__ ((aligned(4), section("SPORT1")));
volatile	unsigned	long	mmrSPORT1_MTCS1	__attribute__ ((aligned(4), section("SPORT1")));
volatile	unsigned	long	mmrSPORT1_MTCS2	__attribute__ ((aligned(4), section("SPORT1")));
volatile	unsigned	long	mmrSPORT1_MTCS3	__attribute__ ((aligned(4), section("SPORT1")));
volatile	unsigned	long	mmrSPORT1_MRCS0	__attribute__ ((aligned(4), section("SPORT1")));
volatile	unsigned	long	mmrSPORT1_MRCS1	__attribute__ ((aligned(4), section("SPORT1")));
volatile	unsigned	long	mmrSPORT1_MRCS2	__attribute__ ((aligned(4), section("SPORT1")));
volatile	unsigned	long	mmrSPORT1_MRCS3	__attribute__ ((aligned(4), section("SPORT1")));



volatile	unsigned	short	mmrEBIU_AMGCTL	__attribute__ ((aligned(4), section("EBIU")));
volatile	unsigned	long	mmrEBIU_AMBCTL0	__attribute__ ((aligned(4), section("EBIU")));
volatile	unsigned	long	mmrEBIU_AMBCTL1	__attribute__ ((aligned(4), section("EBIU")));
volatile	unsigned	long	mmrEBIU_DUMMY0	__attribute__ ((aligned(4), section("EBIU")));


volatile	unsigned	long	mmrEBIU_SDGCTL	__attribute__ ((aligned(4), section("EBIU")));
volatile	unsigned	short	mmrEBIU_SDBCTL	__attribute__ ((aligned(4), section("EBIU")));
volatile	unsigned	short	mmrEBIU_SDRRC	__attribute__ ((aligned(4), section("EBIU")));
volatile	unsigned	short	mmrEBIU_SDSTAT	__attribute__ ((aligned(4), section("EBIU")));



volatile	unsigned	short	mmrDMA_TC_DUMMY0	__attribute__ ((aligned(4), section("DMATC")));
volatile	unsigned	short	mmrDMA_TC_DUMMY1	__attribute__ ((aligned(4), section("DMATC")));
volatile	unsigned	short	mmrDMA_TC_DUMMY3	__attribute__ ((aligned(4), section("DMATC")));
volatile	unsigned	short	mmrDMA_TC_PER	__attribute__ ((aligned(4), section("DMATC")));
volatile	unsigned	short	mmrDMA_TC_CNT	__attribute__ ((aligned(4), section("DMATC")));


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
