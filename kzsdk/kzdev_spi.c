/**
 *	@file	kzdev_spi.c
 *	@brief	KOBANZAME SDK SPI Device Driver
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

/*--- include -----------------------------------*/
#include "kobanzame.h"
#include "jsp_kernel.h"

/*--- define ------------------------------------*/
#define MAX_SPI_DEVICES			(8)
#define	SPI_DEVICE_SEM			(mSemSpi)
#define	SPI_COMPLETE_SIG		(mSpiCompleteSig)
#define	SPI_TIMEOUT_PER_BYTE	(10)

typedef struct {
	int					mnCS;			/*!< chip select number */
	UW					mdwBPS;			/*!< bit per second */
	KZDEV_SPI_BITLEN_t	mnDeviceBit;	/*!< 8bit or 16bit */
	BOOL				mbCPOL;			/*!< SPI CPOL set or not */
	BOOL				mbCPHA;			/*!< SPI CPHA set or not */
} SpiDeviceConfigurator_t;

/*--- file domain values ------------------------*/
static SpiDeviceConfigurator_t	mSpiDevConfig[MAX_SPI_DEVICES];
const static SpiDeviceConfigurator_t	*mpCurConfig = 0;
static UB*	mpbyTxBuf;
static UB*	mpbyRxBuf;
static int	mnTxCount;
static int	mnRxCount;
static int	mnRegistered = 0;
static ID	mSpiCompleteSig;
static ID	mSemSpi;


static BOOL rcv_snd(void);

/**
 * @fn		KzAttIniSpi(ID sem, ID sig )
 * @brief	Initialize called from uITRON ATT_INI
 * @param	sem a semphore ID for re-entrant
 * @param	sig a complete signal
 * @note	KOBANZAME SDK kernel private function
 */
void KzAttIniSpi(ID sem, ID sig)
{
	mSemSpi = sem;
	mSpiCompleteSig = sig;
}
/**
 * @fn		void KzISRSpi(void)
 * @brief	SPI Ingterrupt Survice Routine
 * @note	KOBANZAME SDK kernel private function
 */
L1CODE
void KzISRSpi(void)
{
	if( !rcv_snd() )
		isig_sem( SPI_COMPLETE_SIG );

}
/**
 * @fn		static BOOL snd(void)
 * @brief	Send
 * @return	TRUE  ... Send Ok
 * @return	FALSE ... No send data
 */
L1CODE
static BOOL snd(void)
{
	if( mpCurConfig->mnDeviceBit == KZDEV_SPI_8BIT )
	{
		/* Send 8bit */
		if( mnTxCount > 0 )
		{
			UH reg;
			reg = (UH) *(mpbyTxBuf) & 0xFF;
			++mpbyTxBuf;
			--mnTxCount;
			
			*pSPI_TDBR = reg;
			ssync();
			return TRUE;
		}
		
		/* No data, send 0xFF */
		*pSPI_TDBR = 0xFF;
	}
	else
	{
		/* Send 16bit */
		if( mnTxCount > 0 )
		{
			UH reg;
			reg= *((UH*)mpbyTxBuf);
			mpbyTxBuf+=2;
			--mnTxCount;
			
			*pSPI_TDBR = reg;
			ssync();
			return TRUE;
		}
		
		/* No data, send 0xFFFF */
		*pSPI_TDBR = 0xFFFF;
	}
	return FALSE;
	
}

/**
 *	@fn			static BOOL get_rx( )
 *	@brief		get from rx buffer
 */
L1CODE
static BOOL get_rx(void)
{
	if( mpCurConfig->mnDeviceBit == KZDEV_SPI_8BIT )
	{
		/* Get 8bit */
		if ( mnRxCount > 0) 
		{
			UH reg = *pSPI_RDBR;
			ssync();
			
			*mpbyRxBuf = (UB) reg & 0xFF;
			++mpbyRxBuf;
			--mnRxCount;
			return TRUE;
		}
	}
	else
	{
		/* Get 16bit */
		if ( mnRxCount > 0) 
		{	
			UH reg = *pSPI_RDBR;
			ssync();
			
			*((UH *) mpbyRxBuf) = reg;
			mpbyRxBuf+=2;
			--mnRxCount;
			return TRUE;
		}
	}
	return FALSE;	
	
}
/**
 *	@fn			static BOOL rcv_snd(void)
 *	@brief		Receive a data and send a data
 */
L1CODE
static BOOL rcv_snd(void)
{
	if ( mnTxCount <= 0 && mnRxCount <= 1 )
	{
		/* transfer end */
		/* CS disable */
		if( mpCurConfig->mnCS >= 0 )
			*pSPI_FLG &= ~( 1 << ( mpCurConfig->mnCS  ) );

		/* SPI disable */
		*pSPI_CTL = 0;
		ssync();
		
		/* Get rest of one rx message, if there is. */
		get_rx();

		/* Complete! */
		mpCurConfig = 0;
		
		return FALSE;
	} 
	else
	{
		/* transfer continue */
		/* Send tx */
		snd();

		if (!get_rx() )
		{
			/* get dummy data for trigger */
			volatile UH dummy = *pSPI_RDBR;
		}
		return TRUE;
	}
}
/**
 * @fn			static void spi_start(const SpiDeviceConfigurator_t* config, const void* pTx, int nTx,  void* pRx, int nRx )
 * @brief		Start SPI
 * @param		config a SPI Configuration
 * @param		pTx Tx Buffer pointer
 * @param		nTx number of the send units
 * @param		pRx Rx Buffer pointer
 * @param		nRx number of the receive units
 */
static void spi_start(const SpiDeviceConfigurator_t* config, const void* pTx, int nTx,  void* pRx, int nRx )
{
	volatile UH dummy;
	UH	spi_ctl;
	
	mpCurConfig = config;
	mpbyTxBuf = (UB*) pTx;
	mpbyRxBuf = (UB*) pRx;
	mnTxCount = nTx;
	mnRxCount = nRx;
	
	*pSPI_BAUD = SYSCLOCK / 2 / config->mdwBPS;

	/* double bufferの空読み */
	dummy = *pSPI_RDBR;
	dummy = *pSPI_RDBR;
	
	spi_ctl = ( config->mnDeviceBit == KZDEV_SPI_8BIT )? SPE | MSTR | GM | ( 00 & TIMOD)
			: SPE | MSTR |  0x0100/*16bit*/| GM | ( 00 & TIMOD);
	
	spi_ctl |= ( config->mbCPOL )? CPOL : 0;
	spi_ctl |= ( config->mbCPHA )? CPHA : 0;
			
	*pSPI_CTL = spi_ctl;
				
	snd();
					
	if( config->mnCS >= 0 )
		*pSPI_FLG |=  1 <<( config->mnCS );
		
	ssync();
	
	dummy = *pSPI_RDBR;
}
/**
 * @fn			static int exec_by_interrupt(const SpiDeviceConfigurator_t* config, const void* pTx, int nTx,  void* pRx, int nRx )
 * @brief		SPI execute using interrupt
 * @param		config a SPI Configuration
 * @param		pTx Tx Buffer pointer
 * @param		nTx number of the send units
 * @param		pRx Rx Buffer pointer
 * @param		nRx number of the receive units
 */
static int exec_by_interrupt(const SpiDeviceConfigurator_t* config, const void* pTx, int nTx,  void* pRx, int nRx )
{
	int ret = (nTx > nRx)? nTx : nRx;
	spi_start(config,pTx,nTx,pRx,nRx);	
	
	ena_int(INHNO_SPI);

	if( twai_sem( SPI_COMPLETE_SIG , SPI_TIMEOUT_PER_BYTE * ret) != E_OK )
	{
		sig_sem( SPI_COMPLETE_SIG );
		ret = 0;
	}
			
	return ret;
}
/**
 * @fn			static int exec_by_interrupt(const SpiDeviceConfigurator_t* config, const void* pTx, int nTx,  void* pRx, int nRx )
 * @brief		SPI execute using polling
 * @param		config a SPI Configuration
 * @param		pTx Tx Buffer pointer
 * @param		nTx number of the send units
 * @param		pRx Rx Buffer pointer
 * @param		nRx number of the receive units
 */
static int exec_by_polling(const SpiDeviceConfigurator_t* config, const void* pTx, int nTx,  void* pRx, int nRx )
{
	int ret = (nTx > nRx)? nTx : nRx;
	spi_start(config,pTx,nTx,pRx,nRx);
	
	for(;;)
	{
		while( !(*pSPI_STAT & 01)) ;
		if ( !rcv_snd() )	break;
	}
	
	return ret;
}
/**
 * @fn		int		kzdev_spi_regist( KZDEV_SPI_BITLEN_t nBitLen, UW dwBps, int nCS, BOOL bCPOL, BOOL bCPHA )
 * @brief	SPI Registration
 * @param	nBitLen 16bit or 8bit
 * @param	dwBps bit per second (BPS can be changed by calling kzdev_spi_chgbps even after this function is done )
 * @param	nCS	a chip select number , -1 is indicate no chip select work.
 * @param 	bCPOL SPI CPOL is set or not
 * @param	bCPHA SPI CPHA is set or not
 * @return	SPI ID
 * @note	KOBANZAME SDK private function
 */
int		kzdev_spi_regist( KZDEV_SPI_BITLEN_t nBitLen, UW dwBps, int nCS, BOOL bCPOL, BOOL bCPHA )
{
	int ret = -1;
	if( mnRegistered < MAX_SPI_DEVICES )
	{
		SpiDeviceConfigurator_t *config = &mSpiDevConfig[mnRegistered];
		
		ret = mnRegistered++;
		
		config->mnCS = nCS;
		config->mdwBPS = dwBps;
		config->mnDeviceBit = nBitLen;
		config->mbCPOL = bCPOL;
		config->mbCPHA = bCPHA;
		
		if( nCS >= 0 )
			*pSPI_FLG &= ~( 1 <<( config->mnCS + 8) );
	}
	
	return ret;
}
/**
 * @fn		UW 	kzdev_spi_chgbps( int nID, UW dwBps )
 * @brief	Change bit ratio
 * @param	nID a SPI ID
 * @param	dwBps bit per second
 * @return	Old value
 * @note	KOBANZAME SDK private function
 */
UW 	kzdev_spi_chgbps( int nID, UW dwBps )
{
	UW ret=0;
	SpiDeviceConfigurator_t *config = &mSpiDevConfig[nID];

	if( nID < mnRegistered )
	{	
		ret = config->mdwBPS;	
		config->mdwBPS = dwBps;
	}
	return ret;
}
/**
 * @fn		void	kzdev_spi_open(void)
 * @brief	Open SPI ( SPI Semaphore Lock )
 * @note	KOBANZAME SDK private function
 */
void	kzdev_spi_open(void)
{
	if( iniflg ) wai_sem(SPI_DEVICE_SEM);
}
/**
 * @fn		void	kzdev_spi_close(void)
 * @brief	Close SPI ( Release SPI Semaphore )
 * @note	KOBANZAME SDK private function
 */
void	kzdev_spi_close(void)
{
	if( iniflg ) sig_sem(SPI_DEVICE_SEM);
}
/**
 * @fn		int	kzdev_spi_readwrite(int nID, const void* pTx, int nTx, void* pRx, int nRx )
 * @brief	SPI Read and write
 * @param	nID a SPI ID
 * @param	pTx Tx Buffer pointer
 * @param	nTx number of the send units
 * @param	pRx Rx Buffer pointer
 * @param	nRx number of the receive units
 * @note	KOBANZAME SDK private function
 */
int		kzdev_spi_readwrite(int nID, const void* pTx, int nTx, void* pRx, int nRx )
{
	int ret = 0;
	SpiDeviceConfigurator_t* config = &mSpiDevConfig[nID];
	
	if( nID >= mnRegistered ) return 0;

	ret = (iniflg)? exec_by_interrupt(config, pTx,nTx,pRx,nRx)
		:			exec_by_polling  (config, pTx,nTx,pRx,nRx);
	
	return ret;
}
/**
 * @fn		int	kzdev_spi_read( int nID, void* pRx, int nRx )
 * @brief	SPI Read
 * @param	nID a SPI ID
 * @param	pRx Rx Buffer pointer
 * @param	nRx number of the receive units
 * @note	KOBANZAME SDK private function
 */
int	kzdev_spi_read( int nID, void* pRx, int nRx )
{
	return kzdev_spi_readwrite(nID,0,0,pRx,nRx);
}
/**
 * @fn		int	kzdev_spi_write(int nID, const void* pTx, int nTx)
 * @brief	SPI write
 * @param	nID a SPI ID
 * @param	pTx Tx Buffer pointer
 * @param	nTx number of the send units
 * @note	KOBANZAME SDK private function
 */
int		kzdev_spi_write(int nID, const void* pTx, int nTx )
{
	return kzdev_spi_readwrite(nID,pTx,nTx,0,0);
}
/**
 * @fn		int	kzdev_spi_dma_read(int nID, void* pRx, int nRx )
 * @brief	SPI DMA Read 
 * @param	nID a SPI ID
 * @param	pRx Rx Buffer pointer
 * @param	nRx number of the receive units
 * @note	KOBANZAME SDK private function
 */
int		kzdev_spi_dma_read( int nID, void* pRx, int nRx )
{
	return 0;
}
/**
 * @fn		int	kzdev_spi_dma_write(int nID, const void* pTx, int nTx )
 * @brief	SPI DMA write
 * @param	nID a SPI ID
 * @param	pTx Tx Buffer pointer
 * @param	nTx number of the send units
 * @note	KOBANZAME SDK private function
 */
int		kzdev_spi_dma_write(int nID, const void* pTx, int nTx )
{
	return 0;
}

