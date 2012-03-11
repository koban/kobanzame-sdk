/**
 *	@file	kzdev_cache.asm
 *	@brief	KOBANZAME SDK Cache Init
 *
 *	KOBANZAME SDK
 *		Software Developers Kit for Blackfin DSP Evaluation Board(KOBANZAME).
 *		
 *	Copyright (C) 2010, KOBANZAME SDK Project, all right reserved
 *
 *	LICENSE:
 *	The software is a free and you can use and redistribute it for
 *	personal, non-profit or commercial products. Redistributions of
 *	source code must retain the above copyright notice. There is no 
 *	warranty in this software, if you suffer any damages by using 
 *	the software.
 */
 
 #include "defBF533.h"
 #include "kzdev_cache.h"
 
 .section L1_code;
.align 4;

.global _kzdev_cache_ini;
_kzdev_cache_ini:

	m0=0(z); m1=0(z); m2=0(z);
	l0=0(z); l1=0(z); l2=0(z);
	b0=0(z); b1=0(z); b2=0(z);
	
	
#ifdef INSTR_CACHE_EN
	// Check if already data was set
	p0.l = LO(IMEM_CONTROL); 
	p0.h = HI(IMEM_CONTROL);
	r0 = [ p0 ];
	cc = bittst( r0, 1 );		// bit 1 is ENICPLB
	if cc jump	__data_cache_set;

//  Initialize INSTRUCTION CPLBs
	i0.l = lo(ICPLB_ADDR0);
	i0.h = hi(ICPLB_ADDR0);

	i1.l = lo(ICPLB_DATA0);
	i1.h = hi(ICPLB_DATA0);
	
	i2.l = icplbs_table;
	i2.h = icplbs_table;

	r1 = -1;  	// end point comparison 
	r3 = 15;	// max counter 

// read entries from table 
	read_iaddr:
		r0 = [i2++];
		cc = r0 == r1;
		if cc jump idone;
		[i0++] = r0;
	read_idata:
		r2 = [i2++];
		[i1++] = r2;
		r3 = r3 + r1;
		cc = r3 == r1;
		if !cc jump read_iaddr;
	idone:

//  ENABLE INSTRUCTION CACHE 

	p0.l = LO(IMEM_CONTROL); 
	p0.h = HI(IMEM_CONTROL);

	r0 = (IMC | ENICPLB);
	[p0] = r0;
	ssync;
#endif //(INSTR_CACHE_EN)
	
__data_cache_set:
#ifdef DATA_CACHE_A_EN
	// Check if already data was set
	p0.l = LO(DMEM_CONTROL); 
	p0.h = HI(DMEM_CONTROL);
	r0 = [ p0 ];
	cc = bittst( r0, 1 );		// bit 1 is ENICPLB
	if cc jump __cache_set_end;

//  INITIALIZE DATA CPLBs
	i0.l = lo(DCPLB_ADDR0);
	i0.h = hi(DCPLB_ADDR0);

	i1.l = lo(DCPLB_DATA0);
	i1.h = hi(DCPLB_DATA0);

	i2.l = dcplbs_table;
	i2.h = dcplbs_table;

	r1 = -1;  	// end point comparison 
	r3 = 15;  	// max counter 

	// read entries from table 
	read_daddr:
		r0 = [i2++];
		cc = r0 == r1;
		if cc jump ddone;
		[i0++] = r0;
	read_ddata:
		r2 = [i2++];
		[i1++] = r2;
		r3 = r3 + r1;
		cc = r3 == r1;
		if !cc jump read_daddr;
	ddone:

//  ENABLE DATA CPLBS
	p0.l = lo(DMEM_CONTROL);
	p0.h = hi(DMEM_CONTROL);

	// DCACHE_CONFIG is a #define in cache_init.h
	r0 = (DCACHE_CONFIG | ENDCPLB);
	[p0] = r0;
	ssync;
				
#endif //(DATA_CACHE_EN)

__cache_set_end:
	rts;

_kzdev_cache_ini.END:


.section L1_data_a;
.align 4;

#if defined(DATA_CACHE_A_EN) || defined(DATA_CACHE_B_EN)

#define SDRAM_DNON_CHBL  	(PAGE_SIZE_1MB | CPLB_SUPV_WR | CPLB_LOCK  | CPLB_VALID)
#define L1_DMEMORY			(PAGE_SIZE_1MB | CPLB_SUPV_WR | CPLB_LOCK  | CPLB_VALID)	
#define ASYNC_DMEMORY		(PAGE_SIZE_1MB | CPLB_SUPV_WR | CPLB_LOCK  | CPLB_VALID)
#define DCACHE_SAFE			(PAGE_SIZE_1MB | CPLB_SUPV_WR | CPLB_LOCK  | CPLB_VALID)

#ifdef DCACHE_WB
	#define SDRAM_DGENERIC	(PAGE_SIZE_4MB | CPLB_L1_CHBL | CPLB_DIRTY | CPLB_SUPV_WR | CPLB_VALID | Bit9)
#else
	#define SDRAM_DGENERIC	(PAGE_SIZE_4MB | CPLB_L1_CHBL | CPLB_WT | CPLB_SUPV_WR | CPLB_VALID | Bit9)
#endif // DCACHE_WB


	dcplbs_table:
	.byte4=
		0xFF800000, (L1_DMEMORY), 		//  DATA BANK A SRAM
		0xFF900000, (L1_DMEMORY),		//  DATA BANK B SRAM
		0x00000000, (SDRAM_DGENERIC),	//	SDRAM_Page0
		0x00400000, (SDRAM_DGENERIC),	//	SDRAM_Page1
		0x00800000, (SDRAM_DGENERIC),	//	SDRAM_Page2
		0x00C00000, (SDRAM_DGENERIC),	//	SDRAM_Page3
		0x01000000, (SDRAM_DGENERIC),	//	SDRAM_Page4
		0x01400000, (SDRAM_DGENERIC),	//	SDRAM_Page5
		0x01800000, (SDRAM_DGENERIC),	//	SDRAM_Page6
		0x01C00000, (SDRAM_DGENERIC),	//	SDRAM_Page7
		0x20000000,	(ASYNC_DMEMORY),	//	ASYNC 0
		0x20100000,	(ASYNC_DMEMORY),	//	ASYNC 0
		0x20200000,	(ASYNC_DMEMORY),	//	ASYNC 0
		0x20300000,	(ASYNC_DMEMORY),	//	ASYNC 0		
		0xffffffff;	// end of section - termination	
#endif //(DATA_CACHE_EN)

#ifdef INSTR_CACHE_EN

	/************************************************************
	* 	Bit 9 of the ICPLB_DATAx is shown as Reserved bit		*
	*	however, when ICACHE is used this bit needs to be		*
	*	set to the same state as bit 12 (CPLB_L1_CHBL) 			*
	*	of this register as a workaround to anomaly: 05000258	*
	* 	This bit is defined in cache_init.h						*				
	************************************************************/
	#define SDRAM_IGENERIC		(PAGE_SIZE_4MB | CPLB_L1_CHBL | Bit9 | CPLB_USER_RD | CPLB_VALID)
	#define SDRAM_INON_CHBL  	(PAGE_SIZE_1MB | CPLB_USER_RD | CPLB_VALID)
	#define L1_IMEMORY			(PAGE_SIZE_1MB | CPLB_LOCK  | CPLB_VALID)
	#define ASYNC_IMEMORY		(PAGE_SIZE_1MB | CPLB_USER_RD | CPLB_VALID)
	#define ICACHE_SAFE			(PAGE_SIZE_4KB | CPLB_USER_RD | CPLB_VALID)
	
	icplbs_table:
	.byte4=
		0xFFA00000, (L1_IMEMORY), 		//  L1 SRAM
		0x00000000, (SDRAM_IGENERIC),	//	SDRAM_Page0
		0x00400000, (SDRAM_IGENERIC),	//	SDRAM_Page1
		0x00800000, (SDRAM_IGENERIC),	//	SDRAM_Page2
		0x00C00000, (SDRAM_IGENERIC),	//	SDRAM_Page3
		0x01000000, (SDRAM_IGENERIC),	//	SDRAM_Page4
		0x01400000, (SDRAM_IGENERIC),	//	SDRAM_Page5
		0x01800000, (SDRAM_IGENERIC),	//	SDRAM_Page6
		0x01C00000, (SDRAM_IGENERIC),	//	SDRAM_Page7
		0x20000000,	(ASYNC_IMEMORY),	//	ASYNC 0
		0x20100000,	(ASYNC_IMEMORY),	//	ASYNC 0
		0x20200000,	(ASYNC_IMEMORY),	//	ASYNC 0
		0x20300000,	(ASYNC_IMEMORY),	//	ASYNC 0
		0xffffffff;	// end of section - termination
#endif //(INSTR_CACHE_EN)

page_size_table:
.byte4=
	0x00000400,	/* 1K */
	0x00001000,	/* 4K */
	0x00100000,	/* 1M */
	0x00400000; /* 4M */
	

//void kzdev_cache_prefetch( void *addr , int n );
.section L1_code;
.global _kzdev_cache_prefetch;

_kzdev_cache_prefetch:
	r2 = 0x1F;
	r2 = ~r2;
	r0 = r0 & r2;
	p0 = r0;
	r1 = r1 >> 5; // blackfin cache line is 32bytes
	r1 += 2;
	p1 = r1;
	
	lsetup( _Prefetch_Drv_Cache_s, _Prefetch_Drv_Cache_e ) lc0 = p1;

_Prefetch_Drv_Cache_s:
_Prefetch_Drv_Cache_e:
		prefetch[ p0++ ];

	rts;
_kzdev_cache_prefetch.END:


//void kzdev_cache_flush( void *addr, int n );

.section L1_code;
.global _kzdev_cache_flush;

_kzdev_cache_flush:
	r2 = 0x1F;
	r2 = ~r2;
	r0 = r0 & r2;
	p0 = r0;
	r1 = r1 >> 5; // blackfin cache line is 32bytes
	r1 += 2;
	p1 = r1;
	
	lsetup( _Flush_Drv_Cache_e, _Flush_Drv_Cache_s ) lc0 = p1;

_Flush_Drv_Cache_s:
_Flush_Drv_Cache_e:
		flush[ p0++ ];

	rts;
_kzdev_cache_flush.END:


//void FlushInv_Drv_Cache( void *addr, int n );
.section L1_code;
.global _kzdev_cache_flushInv;

_kzdev_cache_flushInv:
	r2 = 0x1F;
	r2 = ~r2;
	r0 = r0 & r2;
	p0 = r0;
	r1 = r1 >> 5; // blackfin cache line is 32bytes
	r1 += 2;
	p1 = r1;
	
	lsetup( _FlushInv_Drv_Cache_e, _FlushInv_Drv_Cache_s ) lc0 = p1;

_FlushInv_Drv_Cache_e:
_FlushInv_Drv_Cache_s:
		flushinv[ p0++ ];

	rts;
_kzdev_cache_flushInv.END:



