/*==============================================================
 *
 * kzdev_cache.h
 *
 *		Cache driver for Kobanzame
 *
 *
 *==============================================================*/
 
#ifndef _kzdev_cache_h_
#define _kzdev_cache_h_

/* valid defines for instruction cache */
#define INSTR_CACHE_EN

/* valid defines for data cache */
#define DATA_CACHE_A_EN
#define DATA_CACHE_B_EN

/* choose 1 of the following 2 when data cache enabled */
#if		defined(DATA_CACHE_A_EN) && defined(DATA_CACHE_B_EN)
#define DCACHE_CONFIG	ACACHE_BCACHE
#elif	defined(DATA_CACHE_A_EN) && !defined(DATA_CACHE_B_EN)
#define	DCACHE_CONFIG	ACACHE_BSRAM
#elif	!defined(DATA_CACHE_A_EN) && !defined(DATA_CACHE_B_EN)
#define	DCACHE_CONFIG	ASRAM_BSRAM
#else
#error	Nt support A as SRAM,B as CACHE
#endif

/* Default is Write-through.  */
//#define DCACHE_WB

/************************************************************
* 	Bit 9 of the ICPLB_DATAx is shown as Reserved bit		*
*	however, when ICACHE is used this bit needs to be		*
*	set to the same state as bit 12 (CPLB_L1_CHBL) 			*
*	of this register as a workaround to anomaly: 05000258	*		
*************************************************************/ 	
#define Bit9 0x0200 

#endif	// !_kzdev_cache_h_
