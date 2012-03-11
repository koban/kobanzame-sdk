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
	printf("Hello World\n");
	
	/* main_tsk exit */
	ext_tsk();
}
