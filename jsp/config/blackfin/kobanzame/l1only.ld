
OUTPUT_FORMAT("elf32-bfin","elf32-bfin","elf32-bfin")
OUTPUT_ARCH(bfin)

PROVIDE (ldf_stack_end = 0xffb00FFC ) ;
PROVIDE (hardware_init_hook = 0) ;
PROVIDE (software_init_hook = 0) ;
PROVIDE (software_term_hook = 0) ;
PROVIDE (__kernel_boot_for_gdb = 0);
ENTRY (start);

MEMORY
  {
    MEM_SDRAM (XW)    : ORIGIN = 0x00000000, LENGTH = 128M
    MEM_ASYNC_0 (XW)  : ORIGIN = 0x20000000, LENGTH = 1M
    MEM_ASYNC_1 (XW)  : ORIGIN = 0x20100000, LENGTH = 1M
    MEM_ASYNC_2 (XW)  : ORIGIN = 0x20200000, LENGTH = 1M
    MEM_ASYNC_3 (XW)  : ORIGIN = 0x20300000, LENGTH = 1M

    MEM_DATA_A (W!X)  : ORIGIN = 0xff800000, LENGTH = 32K
    MEM_DATA_B (W!X)  : ORIGIN = 0xff900000, LENGTH = 32K
    MEM_INST (XR)     : ORIGIN = 0xffa00000, LENGTH = 64K
    MEM_SCRATCH (W!X) : ORIGIN = 0xffb00000, LENGTH = 4K
    
    MEM_SYSMMR (W!X)  : ORIGIN = 0xffc00000, LENGTH = 2M
    MEM_COREMMR (W!X) : ORIGIN = 0xffe00000, LENGTH = 2M
  }

SECTIONS
{
    .init :
    {
        KEEP (*(.init))
    } > MEM_INST

    .text :
    {
        *(.text .stub .text.* .gnu.linkonce.t.*)
    } > MEM_INST

    .fini :
    {
        KEEP (*(.fini))
    } > MEM_INST

    .rodata :
    {
        *(.rodata .rodata.* .gnu.linkonce.r.*)
    } > MEM_DATA_A

    .data :
    {
        *(.data)
    } > MEM_DATA_A

    .eh_frame : { KEEP (*(.eh_frame)) }
    .gcc_except_table : { *(.gcc_except_table) }

    .ctors :
    {
        KEEP (*crtbegin.o(.ctors))
        KEEP (*(EXCLUDE_FILE (*crtend.o ) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*(.ctors))
    } > MEM_DATA_A

    .dtors :
    {
        KEEP (*crtbegin.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o ) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*(.dtors))
    } > MEM_DATA_A

    .bss :
    {
        __bss_start = . ;
        *(.bss)
        *(COMMON)
        __bss_end = . ;
    } > MEM_DATA_B

    /* PROVIDE (end = .) ; */
	_end = .;
	
	.l1data_a :
	{
		*(.l1data_a)
	} > MEM_DATA_A
	
	.l1data_b :
	{
		*(l1data_b)
	} > MEM_DATA_B
	
	.l1data_s :
	{
		*(.l1data_s)
	} > MEM_SCRATCH
	
	.l1code :
	{
		*(.l1code)
	} > MEM_INST

	.l3data :
	{
		*(.l3data)
	} > MEM_SDRAM
	
	.l3code :
	{
		*(.l3code)
	} > MEM_SDRAM
    
/* ADSP-BF533依存のSYSTEM MMR */	
   	.bss.sysmmr (NOLOAD) : {                    chip_mmr.o(PWRMGMT)}> MEM_SYSMMR
   	.bss.sysmmr (NOLOAD) : { . = ALIGN(0x0100); chip_mmr.o(SYSRST)} > MEM_SYSMMR
   	.bss.sysmmr (NOLOAD) : { . = ALIGN(0x0100); chip_mmr.o(WDOG)}   > MEM_SYSMMR
   	.bss.sysmmr (NOLOAD) : { . = ALIGN(0x0100); chip_mmr.o(RTC)}    > MEM_SYSMMR
   	.bss.sysmmr (NOLOAD) : { . = ALIGN(0x0100); chip_mmr.o(UART)}   > MEM_SYSMMR
   	.bss.sysmmr (NOLOAD) : { . = ALIGN(0x0100); chip_mmr.o(SPI)}    > MEM_SYSMMR
   	.bss.sysmmr (NOLOAD) : { . = ALIGN(0x0100); chip_mmr.o(TIMER)}  > MEM_SYSMMR
   	.bss.sysmmr (NOLOAD) : { . = ALIGN(0x0040); chip_mmr.o(TMRCTL)} > MEM_SYSMMR
   	.bss.sysmmr (NOLOAD) : { . = ALIGN(0x0100); chip_mmr.o(FLAG)}   > MEM_SYSMMR
   	.bss.sysmmr (NOLOAD) : { . = ALIGN(0x0100); chip_mmr.o(SPORT0)} > MEM_SYSMMR
   	.bss.sysmmr (NOLOAD) : { . = ALIGN(0x0100); chip_mmr.o(SPORT1)} > MEM_SYSMMR
   	.bss.sysmmr (NOLOAD) : { . = ALIGN(0x0100); chip_mmr.o(EBIU)} > MEM_SYSMMR
   	.bss.sysmmr (NOLOAD) : { . = ALIGN(0x0100); chip_mmr.o(DMATC)} > MEM_SYSMMR
   	.bss.sysmmr (NOLOAD) : { . = ALIGN(0x0100); chip_mmr.o(DMA)} > MEM_SYSMMR
   	.bss.sysmmr (NOLOAD) : { . = ALIGN(0x1000); chip_mmr.o(PPI)} > MEM_SYSMMR

/* Blackfin 共有のCORE MMR */	
   	.bss.coremmr (NOLOAD) : {                    cpu_mmr.o(DMEMCTL)}> MEM_COREMMR
   	.bss.coremmr (NOLOAD) : { . = ALIGN(0x0100); cpu_mmr.o(DCPLBA)}> MEM_COREMMR
   	.bss.coremmr (NOLOAD) : { . = ALIGN(0x0100); cpu_mmr.o(DCPLBD)}> MEM_COREMMR
   	.bss.coremmr (NOLOAD) : { . = ALIGN(0x0100); cpu_mmr.o(DTESTC)}> MEM_COREMMR
   	.bss.coremmr (NOLOAD) : { . = ALIGN(0x0100); cpu_mmr.o(DTESTD)}> MEM_COREMMR
   	.bss.coremmr (NOLOAD) : { . = ALIGN(0x1000); cpu_mmr.o(IMEMCTL)}> MEM_COREMMR
   	.bss.coremmr (NOLOAD) : { . = ALIGN(0x0100); cpu_mmr.o(ICPLBA)}> MEM_COREMMR
   	.bss.coremmr (NOLOAD) : { . = ALIGN(0x0100); cpu_mmr.o(ICPLBD)}> MEM_COREMMR
   	.bss.coremmr (NOLOAD) : { . = ALIGN(0x0100); cpu_mmr.o(ITESTC)}> MEM_COREMMR
   	.bss.coremmr (NOLOAD) : { . = ALIGN(0x0100); cpu_mmr.o(ITESTD)}> MEM_COREMMR
   	.bss.coremmr (NOLOAD) : { . = ALIGN(0x1000); cpu_mmr.o(EVT)}> MEM_COREMMR
   	.bss.coremmr (NOLOAD) : { . = ALIGN(0x0100); cpu_mmr.o(CEC)}> MEM_COREMMR
   	.bss.coremmr (NOLOAD) : { . = ALIGN(0x1000); cpu_mmr.o(CTMR)}> MEM_COREMMR
   	.bss.coremmr (NOLOAD) : { . = ALIGN(0x1000)+0x1000; cpu_mmr.o(DBG)}> MEM_COREMMR	/* 1000飛ばす */
   	.bss.coremmr (NOLOAD) : { . = ALIGN(0x1000); cpu_mmr.o(TBCTL)}> MEM_COREMMR
   	.bss.coremmr (NOLOAD) : { . = ALIGN(0x0100); cpu_mmr.o(TBUF)}> MEM_COREMMR
   	.bss.coremmr (NOLOAD) : { . = ALIGN(0x1000); cpu_mmr.o(WPICTL)}> MEM_COREMMR
   	.bss.coremmr (NOLOAD) : { . = ALIGN(0x0040); cpu_mmr.o(WPIADDR)}> MEM_COREMMR
   	.bss.coremmr (NOLOAD) : { . = ALIGN(0x0040); cpu_mmr.o(WPICNT)}> MEM_COREMMR
   	.bss.coremmr (NOLOAD) : { . = ALIGN(0x0100); cpu_mmr.o(WPDCTL)}> MEM_COREMMR
   	.bss.coremmr (NOLOAD) : { . = ALIGN(0x0040); cpu_mmr.o(WPDADDR)}> MEM_COREMMR
   	.bss.coremmr (NOLOAD) : { . = ALIGN(0x0040); cpu_mmr.o(WPDCNT)}> MEM_COREMMR
   	.bss.coremmr (NOLOAD) : { . = ALIGN(0x0100); cpu_mmr.o(WPSTAT)}> MEM_COREMMR
   	.bss.coremmr (NOLOAD) : { . = ALIGN(0x1000); cpu_mmr.o(PFCTL)}> MEM_COREMMR
   	.bss.coremmr (NOLOAD) : { . = ALIGN(0x0100); cpu_mmr.o(PFCNT)}> MEM_COREMMR

    
    .comment        0 : { *(.comment) }
    .debug          0 : { *(.debug) }
    .line           0 : { *(.line) }
    .debug_srcinfo  0 : { *(.debug_srcinfo) }
    .debug_sfnames  0 : { *(.debug_sfnames) }
    .debug_aranges  0 : { *(.debug_aranges) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
}
