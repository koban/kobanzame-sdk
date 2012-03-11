/**
 *	@file	kzdev_mems.c
 *	@brief	KOBANZAME SDK MEMS Device Driver
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
#include "def_mma7455l.h"

#define MESURE_MODE		(BITATTR_MMA7455L_2G | BITATTR_MMA7455L_MEASURE_MODE)
#define STANBY_MODE	(0)
#define PORT_MEMS_READY	(0)

/*--- file domain values ------------------------*/
static int 		mSpiDesc;
static BOOL		mbActivated = FALSE;

/**
 * @fn		void	kzdev_mems_att_ini(void)
 * @brief	Initialize MEMS called from uITRON ATT_INI
 * @note	KOBANZAME SDK private function
 */
void	kzdev_mems_att_ini(void)
{
	mSpiDesc = kzdev_spi_regist( KZDEV_SPI_16BIT, 500000,5, FALSE, TRUE );	
	KZ_GPIO_SET_DIR_IN( PORT_MEMS_READY );
}
/**
 * @fn		static void spi_write( UB addr, UB dat )
 * @brief	MEMS SPI Write
 * @param	addr MEMS Address
 * @param	dat write data
 */
static void spi_write( UB addr, UB dat )
{
	UH tx = 0x8000 | ( (addr&0x3F)<<9 ) | (dat&0xFF);
	kzdev_spi_open();
	kzdev_spi_write( mSpiDesc, &tx, 1 );
	kzdev_spi_close();
		
}
/**
 * @fn		static UB spi_read(UB addr )
 * @brief	MEMS SPI Read
 * @param	addr MEMS Address
 * @return	read data
 */
static UB spi_read(UB addr )
{
	UH tx = 0x0000 | ( (addr&0x3F)<<9 );
	UH rx;
	kzdev_spi_open();
	kzdev_spi_readwrite( mSpiDesc, &tx, 1, &rx, 1 );
	kzdev_spi_close();
	return rx & 0xFF;
	
}
/**
 * @fn		BOOL	kzdev_mems_start(void)
 * @brief	Start MEMS
 * @return	TRUE  ... Success /
 * @return	FALSE ... Fail to start (MEMS is broken?)
 * @note	KOBANZAME SDK private function
 */
BOOL	kzdev_mems_start(void)
{
	/* dummy read for start */
	spi_read( REG_MMA7455L_I2CAD );
	
	/*  Disable I2C */
	spi_write( REG_MMA7455L_I2CAD, 0x80 );
	
	/* set musurement mode */
	spi_write( REG_MMA7455L_MCTL, MESURE_MODE );
		
	/* confirm device */
	mbActivated = (spi_read( REG_MMA7455L_MCTL ) == MESURE_MODE )? TRUE : FALSE;
	
	return mbActivated;
	
}
/**
 * @fn		void	kzdev_mems_stop(void)
 * @brief	Stop MEMS
 * @note	KOBANZAME SDK private function
 */
void	kzdev_mems_stop(void)
{
	mbActivated = FALSE;
	/* set standby mode */
	spi_write( REG_MMA7455L_MCTL, STANBY_MODE );
	
}
/**
 * @fn		BOOL	kzdev_mems_isReady(void)
 * @brief	Check MEMS data are ready
 * @return	TRUE ... Data ready
 * @note	KOBANZAME SDK private function
 */
BOOL	kzdev_mems_isReady(void)
{
	if( mbActivated )
		return (KZ_GPIO_GET( PORT_MEMS_READY ))? TRUE : FALSE;
	else
		return FALSE;
}
/**
 * @fn		void	kzdev_mems_getVal( int *x, int *y, int *z )
 * @brief	Get MEMS Value
 * @param	x MEMS X value
 * @param	y MEMS Y value
 * @param	z MEMS Z value
 * @note	KOBANZAME SDK private function
 */
void	kzdev_mems_getVal( int *x, int *y, int *z )
{
	*x = (char)spi_read( REG_MMA7455L_XOUT8 );
	*y = (char)spi_read( REG_MMA7455L_YOUT8 );
	*z = (char)spi_read( REG_MMA7455L_ZOUT8 );		
}

