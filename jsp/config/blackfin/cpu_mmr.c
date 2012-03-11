/*
 * Blackfin Core MMR定義ファイル。
 *
 * このファイルはGDBでのデバッグの支援用に開発したものである。GCC環境での開発に用いても構わないが、
 * VisualDSP++への移行は少々複雑になる。また、開発に使うための検証は行っていない。
 *
 * アドレスの正しさの検証は、以下の比較によって行っている。
 *
$ readelf -s  -W jsp | grep mmr | grep -v DUMMY | grep -v blackfin | sort -k 2 | awk '/_mmr/{print $8, $2}' -  | sed 's/_mmr//p' | uniq > regelf.txt
$ cat ../jsp/config/blackfin/def_lpblackfin.h ../jsp/config/blackfin/defbf532.h | grep '0xFF[CE]0'  | grep "#define" | sed 's/_MA /_MAP /' - | awk '{print $2,$3}' - | sort -k 2 | sed 's/0x//' - > regdef.txt
$ diff -i -w regelf.txt regdef.txt 24a25,27
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
 * 得られた結果では、def_lpblackfin.hにある3つのレジスタがcore_mmr.cでは無視されていることを指摘している。
 * これらのレジスタはエイリアスであり、同じアドレスに別レジスタがあるので、この結果で正しい。
 *
 */
volatile	void	*	volatile	mmrSRAM_BASE_ADDRESS	__attribute__ ((aligned(4), section("DMEMCTL")));
volatile	unsigned	long		mmrDMEM_CONTROL	__attribute__ ((aligned(4), section("DMEMCTL")));
volatile	unsigned	long		mmrDCPLB_STATUS	__attribute__ ((aligned(4), section("DMEMCTL")));
volatile	void	*	volatile	mmrDCPLB_FAULT_ADDR	__attribute__ ((aligned(4), section("DMEMCTL")));
volatile	void	*	volatile	mmrDCPLB_ADDR0	__attribute__ ((aligned(4), section("DCPLBA")));
volatile	void	*	volatile	mmrDCPLB_ADDR1	__attribute__ ((aligned(4), section("DCPLBA")));
volatile	void	*	volatile	mmrDCPLB_ADDR2	__attribute__ ((aligned(4), section("DCPLBA")));
volatile	void	*	volatile	mmrDCPLB_ADDR3	__attribute__ ((aligned(4), section("DCPLBA")));
volatile	void	*	volatile	mmrDCPLB_ADDR4	__attribute__ ((aligned(4), section("DCPLBA")));
volatile	void	*	volatile	mmrDCPLB_ADDR5	__attribute__ ((aligned(4), section("DCPLBA")));
volatile	void	*	volatile	mmrDCPLB_ADDR6	__attribute__ ((aligned(4), section("DCPLBA")));
volatile	void	*	volatile	mmrDCPLB_ADDR7	__attribute__ ((aligned(4), section("DCPLBA")));
volatile	void	*	volatile	mmrDCPLB_ADDR8	__attribute__ ((aligned(4), section("DCPLBA")));
volatile	void	*	volatile	mmrDCPLB_ADDR9	__attribute__ ((aligned(4), section("DCPLBA")));
volatile	void	*	volatile	mmrDCPLB_ADDR10	__attribute__ ((aligned(4), section("DCPLBA")));
volatile	void	*	volatile	mmrDCPLB_ADDR11	__attribute__ ((aligned(4), section("DCPLBA")));
volatile	void	*	volatile	mmrDCPLB_ADDR12	__attribute__ ((aligned(4), section("DCPLBA")));
volatile	void	*	volatile	mmrDCPLB_ADDR13	__attribute__ ((aligned(4), section("DCPLBA")));
volatile	void	*	volatile	mmrDCPLB_ADDR14	__attribute__ ((aligned(4), section("DCPLBA")));
volatile	void	*	volatile	mmrDCPLB_ADDR15	__attribute__ ((aligned(4), section("DCPLBA")));
volatile	unsigned	long		mmrDCPLB_DATA0	__attribute__ ((aligned(4), section("DCPLBD")));
volatile	unsigned	long		mmrDCPLB_DATA1	__attribute__ ((aligned(4), section("DCPLBD")));
volatile	unsigned	long		mmrDCPLB_DATA2	__attribute__ ((aligned(4), section("DCPLBD")));
volatile	unsigned	long		mmrDCPLB_DATA3	__attribute__ ((aligned(4), section("DCPLBD")));
volatile	unsigned	long		mmrDCPLB_DATA4	__attribute__ ((aligned(4), section("DCPLBD")));
volatile	unsigned	long		mmrDCPLB_DATA5	__attribute__ ((aligned(4), section("DCPLBD")));
volatile	unsigned	long		mmrDCPLB_DATA6	__attribute__ ((aligned(4), section("DCPLBD")));
volatile	unsigned	long		mmrDCPLB_DATA7	__attribute__ ((aligned(4), section("DCPLBD")));
volatile	unsigned	long		mmrDCPLB_DATA8	__attribute__ ((aligned(4), section("DCPLBD")));
volatile	unsigned	long		mmrDCPLB_DATA9	__attribute__ ((aligned(4), section("DCPLBD")));
volatile	unsigned	long		mmrDCPLB_DATA10	__attribute__ ((aligned(4), section("DCPLBD")));
volatile	unsigned	long		mmrDCPLB_DATA11	__attribute__ ((aligned(4), section("DCPLBD")));
volatile	unsigned	long		mmrDCPLB_DATA12	__attribute__ ((aligned(4), section("DCPLBD")));
volatile	unsigned	long		mmrDCPLB_DATA13	__attribute__ ((aligned(4), section("DCPLBD")));
volatile	unsigned	long		mmrDCPLB_DATA14	__attribute__ ((aligned(4), section("DCPLBD")));
volatile	unsigned	long		mmrDCPLB_DATA15	__attribute__ ((aligned(4), section("DCPLBD")));
volatile	unsigned	long		mmrDTEST_COMMAND	__attribute__ ((aligned(4), section("DTESTC")));
volatile	unsigned	long		mmrDTEST_DATA0	__attribute__ ((aligned(4), section("DTESTD")));
volatile	unsigned	long		mmrDTEST_DATA1	__attribute__ ((aligned(4), section("DTESTD")));


volatile	unsigned	long		mmrIMEM_DUMMY	__attribute__ ((aligned(4), section("IMEMCTL")));
volatile	unsigned	long		mmrIMEM_CONTROL	__attribute__ ((aligned(4), section("IMEMCTL")));
volatile	unsigned	long		mmrICPLB_STATUS	__attribute__ ((aligned(4), section("IMEMCTL")));
volatile	void	*	volatile	mmrICPLB_FAULT_ADDR	__attribute__ ((aligned(4), section("IMEMCTL")));
volatile	void	*	volatile	mmrICPLB_ADDR0	__attribute__ ((aligned(4), section("ICPLBA")));
volatile	void	*	volatile	mmrICPLB_ADDR1	__attribute__ ((aligned(4), section("ICPLBA")));
volatile	void	*	volatile	mmrICPLB_ADDR2	__attribute__ ((aligned(4), section("ICPLBA")));
volatile	void	*	volatile	mmrICPLB_ADDR3	__attribute__ ((aligned(4), section("ICPLBA")));
volatile	void	*	volatile	mmrICPLB_ADDR4	__attribute__ ((aligned(4), section("ICPLBA")));
volatile	void	*	volatile	mmrICPLB_ADDR5	__attribute__ ((aligned(4), section("ICPLBA")));
volatile	void	*	volatile	mmrICPLB_ADDR6	__attribute__ ((aligned(4), section("ICPLBA")));
volatile	void	*	volatile	mmrICPLB_ADDR7	__attribute__ ((aligned(4), section("ICPLBA")));
volatile	void	*	volatile	mmrICPLB_ADDR8	__attribute__ ((aligned(4), section("ICPLBA")));
volatile	void	*	volatile	mmrICPLB_ADDR9	__attribute__ ((aligned(4), section("ICPLBA")));
volatile	void	*	volatile	mmrICPLB_ADDR10	__attribute__ ((aligned(4), section("ICPLBA")));
volatile	void	*	volatile	mmrICPLB_ADDR11	__attribute__ ((aligned(4), section("ICPLBA")));
volatile	void	*	volatile	mmrICPLB_ADDR12	__attribute__ ((aligned(4), section("ICPLBA")));
volatile	void	*	volatile	mmrICPLB_ADDR13	__attribute__ ((aligned(4), section("ICPLBA")));
volatile	void	*	volatile	mmrICPLB_ADDR14	__attribute__ ((aligned(4), section("ICPLBA")));
volatile	void	*	volatile	mmrICPLB_ADDR15	__attribute__ ((aligned(4), section("ICPLBA")));
volatile	unsigned	long		mmrICPLB_DATA0	__attribute__ ((aligned(4), section("ICPLBD")));
volatile	unsigned	long		mmrICPLB_DATA1	__attribute__ ((aligned(4), section("ICPLBD")));
volatile	unsigned	long		mmrICPLB_DATA2	__attribute__ ((aligned(4), section("ICPLBD")));
volatile	unsigned	long		mmrICPLB_DATA3	__attribute__ ((aligned(4), section("ICPLBD")));
volatile	unsigned	long		mmrICPLB_DATA4	__attribute__ ((aligned(4), section("ICPLBD")));
volatile	unsigned	long		mmrICPLB_DATA5	__attribute__ ((aligned(4), section("ICPLBD")));
volatile	unsigned	long		mmrICPLB_DATA6	__attribute__ ((aligned(4), section("ICPLBD")));
volatile	unsigned	long		mmrICPLB_DATA7	__attribute__ ((aligned(4), section("ICPLBD")));
volatile	unsigned	long		mmrICPLB_DATA8	__attribute__ ((aligned(4), section("ICPLBD")));
volatile	unsigned	long		mmrICPLB_DATA9	__attribute__ ((aligned(4), section("ICPLBD")));
volatile	unsigned	long		mmrICPLB_DATA10	__attribute__ ((aligned(4), section("ICPLBD")));
volatile	unsigned	long		mmrICPLB_DATA11	__attribute__ ((aligned(4), section("ICPLBD")));
volatile	unsigned	long		mmrICPLB_DATA12	__attribute__ ((aligned(4), section("ICPLBD")));
volatile	unsigned	long		mmrICPLB_DATA13	__attribute__ ((aligned(4), section("ICPLBD")));
volatile	unsigned	long		mmrICPLB_DATA14	__attribute__ ((aligned(4), section("ICPLBD")));
volatile	unsigned	long		mmrICPLB_DATA15	__attribute__ ((aligned(4), section("ICPLBD")));
volatile	unsigned	long		mmrITEST_COMMAND	__attribute__ ((aligned(4), section("ITESTC")));
volatile	unsigned	long		mmrITEST_DATA0	__attribute__ ((aligned(4), section("ITESTD")));
volatile	unsigned	long		mmrITEST_DATA1	__attribute__ ((aligned(4), section("ITESTD")));


volatile	void	*	volatile	mmrEVT0	__attribute__ ((aligned(4), section("EVT")));
volatile	void	*	volatile	mmrEVT1	__attribute__ ((aligned(4), section("EVT")));
volatile	void	*	volatile	mmrEVT2	__attribute__ ((aligned(4), section("EVT")));
volatile	void	*	volatile	mmrEVT3	__attribute__ ((aligned(4), section("EVT")));
volatile	void	*	volatile	mmrEVT4	__attribute__ ((aligned(4), section("EVT")));
volatile	void	*	volatile	mmrEVT5	__attribute__ ((aligned(4), section("EVT")));
volatile	void	*	volatile	mmrEVT6	__attribute__ ((aligned(4), section("EVT")));
volatile	void	*	volatile	mmrEVT7	__attribute__ ((aligned(4), section("EVT")));
volatile	void	*	volatile	mmrEVT8	__attribute__ ((aligned(4), section("EVT")));
volatile	void	*	volatile	mmrEVT9	__attribute__ ((aligned(4), section("EVT")));
volatile	void	*	volatile	mmrEVT10	__attribute__ ((aligned(4), section("EVT")));
volatile	void	*	volatile	mmrEVT11	__attribute__ ((aligned(4), section("EVT")));
volatile	void	*	volatile	mmrEVT12	__attribute__ ((aligned(4), section("EVT")));
volatile	void	*	volatile	mmrEVT13	__attribute__ ((aligned(4), section("EVT")));
volatile	void	*	volatile	mmrEVT14	__attribute__ ((aligned(4), section("EVT")));
volatile	void	*	volatile	mmrEVT15	__attribute__ ((aligned(4), section("EVT")));
volatile	unsigned	long		mmrIDUMMY	__attribute__ ((aligned(4), section("CEC")));
volatile	unsigned	long		mmrIMASK	__attribute__ ((aligned(4), section("CEC")));
volatile	unsigned	long		mmrIPEND	__attribute__ ((aligned(4), section("CEC")));
volatile	unsigned	long		mmrILAT	__attribute__ ((aligned(4), section("CEC")));
volatile	unsigned	long		mmrIPRIO	__attribute__ ((aligned(4), section("CEC")));


volatile	unsigned	long		mmrTCNTL	__attribute__ ((aligned(4), section("CTMR")));
volatile	unsigned	long		mmrTPERIOD	__attribute__ ((aligned(4), section("CTMR")));
volatile	unsigned	long		mmrTSCALE	__attribute__ ((aligned(4), section("CTMR")));
volatile	unsigned	long		mmrTCOUNT	__attribute__ ((aligned(4), section("CTMR")));


volatile	unsigned	long		mmrDSPID	__attribute__ ((aligned(4), section("DBG")));
volatile	unsigned	short		mmrDBGDUMMY	__attribute__ ((aligned(4), section("DBG")));
volatile	unsigned	long		mmrDBGSTAT	__attribute__ ((aligned(4), section("DBG")));


volatile	unsigned	long		mmrTBUFCTL	__attribute__ ((aligned(4), section("TBCTL")));
volatile	unsigned	long		mmrTBUFSTAT	__attribute__ ((aligned(4), section("TBCTL")));
volatile	void	*	volatile	mmrTBUF	__attribute__ ((aligned(4), section("TBUF")));


volatile	unsigned	long		mmrWPIACTL	__attribute__ ((aligned(4), section("WPICTL")));
volatile	void	*	volatile	mmrWPIA0	__attribute__ ((aligned(4), section("WPIADDR")));
volatile	void	*	volatile	mmrWPIA1	__attribute__ ((aligned(4), section("WPIADDR")));
volatile	void	*	volatile	mmrWPIA2	__attribute__ ((aligned(4), section("WPIADDR")));
volatile	void	*	volatile	mmrWPIA3	__attribute__ ((aligned(4), section("WPIADDR")));
volatile	void	*	volatile	mmrWPIA4	__attribute__ ((aligned(4), section("WPIADDR")));
volatile	void	*	volatile	mmrWPIA5	__attribute__ ((aligned(4), section("WPIADDR")));
volatile	unsigned	long		mmrWPIACNT0	__attribute__ ((aligned(4), section("WPICNT")));
volatile	unsigned	long		mmrWPIACNT1	__attribute__ ((aligned(4), section("WPICNT")));
volatile	unsigned	long		mmrWPIACNT2	__attribute__ ((aligned(4), section("WPICNT")));
volatile	unsigned	long		mmrWPIACNT3	__attribute__ ((aligned(4), section("WPICNT")));
volatile	unsigned	long		mmrWPIACNT4	__attribute__ ((aligned(4), section("WPICNT")));
volatile	unsigned	long		mmrWPIACNT5	__attribute__ ((aligned(4), section("WPICNT")));
volatile	unsigned	long		mmrWPDACTL	__attribute__ ((aligned(4), section("WPDCTL")));
volatile	void	*	volatile	mmrWPDA0	__attribute__ ((aligned(4), section("WPDADDR")));
volatile	void	*	volatile	mmrWPDA1	__attribute__ ((aligned(4), section("WPDADDR")));
volatile	unsigned	long		mmrWPDACNT0	__attribute__ ((aligned(4), section("WPDCNT")));
volatile	unsigned	long		mmrWPDACNT1	__attribute__ ((aligned(4), section("WPDCNT")));
volatile	unsigned	long		mmrWPSTAT	__attribute__ ((aligned(4), section("WPSTAT")));


volatile	unsigned	long		mmrPFCTL	__attribute__ ((aligned(4), section("PFCTL")));
volatile	unsigned	long		mmrPFCNTR0	__attribute__ ((aligned(4), section("PFCNT")));
volatile	unsigned	long		mmrPFCNTR1	__attribute__ ((aligned(4), section("PFCNT")));

