/**
 *	@file	kzdev.h
 *	@brief	KOBANZAME SDK Device Driver
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
 *
 */

#ifndef _kz_device_h_
#define _kz_device_h_

/*--- include -----------------------------------*/
#include "kernel.h"
#include "cdefBF533.h"


/*--- sdk private functions ---------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/*-- kzdev_spi.c --*/
typedef enum { KZDEV_SPI_8BIT, KZDEV_SPI_16BIT } KZDEV_SPI_BITLEN_t;
int			kzdev_spi_regist( KZDEV_SPI_BITLEN_t nBitLen, UW dwBps, int nCS, BOOL bCPOL, BOOL bCPHA );
UW	 		kzdev_spi_chgbps( int nID, UW dwBps );
void		kzdev_spi_open(void);
void		kzdev_spi_close(void);
int			kzdev_spi_readwrite(int nID, const void* pTx, int nTx, void* pRx, int nRx );
int			kzdev_spi_read( int nID, void* pRx, int nRx );
int			kzdev_spi_write(int nID, const void* pTx, int nTx );
int			kzdev_spi_dma_read( int nID, void* pRx, int nRx );
int			kzdev_spi_dma_write(int nID, const void* pTx, int nTx );

/*-- kzdev_mems.c --*/
void		kzdev_mems_att_ini(void);
BOOL		kzdev_mems_start(void);
void		kzdev_mems_stop(void);
BOOL		kzdev_mems_isReady(void);
void		kzdev_mems_getVal( int *x, int *y, int *z );

/*-- kzdev_mmc.c --*/
void 		kzdev_mmc_att_ini(void);
void		disk_timerproc(void);

/*-- kzdev_audio.c --*/
BOOL		kzdev_audio_start(void *fnCbk, long lSampleRate, int nBlocks, int nChannels );
void		kzdev_audio_stop(void);

/*-- kzdev_codec.c --*/
void		kzdev_codec_att_ini(void);
void		kzdev_codec_start_DSPMODE(long lSampleRate);
void		kzdev_codec_start_I2CMODE(long lSampleRate);
void		kzdev_codec_stop(void);

/*-- kzdev_cache.asm --*/
void		kzdev_cache_ini(void);
void		kzdev_cache_prefetch( void *addr , int n );
void		kzdev_cache_flush( void *addr, int n );
void		kzdev_cache_flushInv( void *addr, int n );


#ifdef __cplusplus
}
#endif


/*--- GPIO Macro -----------------------------------*/
#define	KZ_GPIO_SET_DIR_OUT(no)		do {	*pFIO_DIR |= (1<<no );	*pFIO_INEN &= ~(1<<no);  }while(0)
#define	KZ_GPIO_SET_DIR_IN(no)		do {	*pFIO_DIR &= ~(1<<no );	*pFIO_INEN |= (1<<no);	 }while(0)
#define	KZ_GPIO_SET_HI(no)			do {	asm volatile("ssync;"); *pFIO_FLAG_S = (1<<no);	}while(0)
#define	KZ_GPIO_SET_LO(no)			do {	asm volatile("ssync;"); *pFIO_FLAG_C = (1<<no);	}while(0)
#define	KZ_GPIO_SET_TGL(no)			do {	asm volatile("ssync;"); *pFIO_FLAG_T = (1<<no);	}while(0)
#define	KZ_GPIO_GET(no)				*pFIO_FLAG_D & ( 1 << no )



#endif	/* !_kz_device_h_ */
