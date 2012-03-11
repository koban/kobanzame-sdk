/**
 *	@file	kzlib.c
 *	@brief	KOBANZAME SDK Library Activation
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
 *	@note	You shuld add this code in your project to use KOBANZAME SDK.
 */
/*--- include -----------------------------------*/
#include "kobanzame.h"
#include "kernel_id.h"

/*--- global extern functions  ------------------*/
extern void KzAttIniSpi(ID sem, ID sig);
extern void	KzAttIniAudio(ID idTskDevAudio);
extern void KzAttIniJspSerial( ID rx, ID tx );
extern void	KzAttIniFatfs(ID semid);
extern void KzAttIniStdio(void);
extern void KzAttIniDeviceDrivers(ID idCycDevPol, ID idTskDevPole);

/*=========================================================================*/
/**
 * @fn		void KzAttIni(VP_INT exinf)
 * @brief	SDK Initialize Function
 * @param	exinf Set by uITRON Configuration. Must be constant 0 in this case
 */
/*=========================================================================*/
void KzAttIni(VP_INT exinf)
{
	/* SPI Initialize */
	KzAttIniSpi( SEM_SPI, SIG_SPI );
	
	/* Audio Initiazlize */
	KzAttIniAudio( TSK_DEV_AUDIO );
	
	/* JSP Serial driver Initiazlize */
	KzAttIniJspSerial( SERIAL_RCV_SEM1, SERIAL_SND_SEM1 );
	
	/* fatfs Initialize */
	KzAttIniFatfs( SEM_FS );	
	
	/* Device Drivers Initialize */
	KzAttIniDeviceDrivers( CYC_DEV_POL, TSK_DEV_POL );
	
	/* Stdio Initialize */
	KzAttIniStdio();
}
