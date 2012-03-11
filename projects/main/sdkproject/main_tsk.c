/**
 *	@file	main_task.c
 *	@brief	KOBANZAME SDK Sample Task
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

 /*--- include -----------------------------------*/
#include "kobanzame.h"

/*=========================================================================*/
/**
 * @fn		void main_task(VP_INT exinf)
 * @brief	Main Task
 * @param	extinf from uITRON Configurator
 */
/*=========================================================================*/
void main_task(VP_INT exinf)
{
	/*
	 *	Display SDK Version and Copyright
	 */
	printf("Start KOBANZEM SDK Sample Project\n");
	printf("KOBANZAME SDK Version   : %d.%02d(%04d)\n",KzGetVersionMajor(), KzGetVersionMinor(), KzGetBuildIdx() );
	printf("              Build Date: %s\n", KzGetBuildDate() );
	printf("              Copyright : %s\n", KzGetCopyRight() );
	
	/*
	 *	Filesystem Start, Insert SD-Card before program start
	 */
	if( KzFilesystemStart() == KZ_OK )
	{
		/*
		 *	Filesystem success to start
		 */
		printf("SD-Card Ready, Filesystem start\n");
		
		/* Add Filesystem relative commands. */
		KzAddCmdFilesystem();
		
	}
	else					
		printf("SD-Card not Ready.\n");
	
	/*
	 *	Add Device Driver Command
	 */
	KzAddCmdDeviceDriver();
	
#if 0
	/* Stdio Test for SDK develop */
	KzAddCmdStdioTest();
#endif

	/*
	 *	Command line start
	 *	This function doesn't return until exit command is issued in the command line.
	 */
	KzCmdlineStart();
	
	/*
	 *　End main task
	 */
	printf("\nExit Command Line\n");
	ext_tsk();
}
