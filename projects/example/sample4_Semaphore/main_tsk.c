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
#include "kernel_id.h"


/*
 *	See also Semaphore.cfg
 */

 
 /*
 * @fn		void high_task(VP_INT exinf)
 * @brief	Print '*' every 300 ms interrupting low_task below.
 */
void high_task(VP_INT exinf)
{
	for(;;)
	{
		/* protect start */
		wai_sem( SEM_PRINT );
		
		/* protected code below */
		putchar('*');
		
		/* protect end */
		sig_sem( SEM_PRINT );
		
		dly_tsk(0);
	}	
}

/*
 * @fn		void low_task(VP_INT extinf)
 * @brief	Print "Low Priotiry Task Running" continusouly
 */
void low_task(VP_INT extinf)
{
	for(;;)
	{
		/* protect start */
		wai_sem( SEM_PRINT );
		
		/* protected code below */
		printf("Low Priority Task Running\n");
		
		/* protect end */
		sig_sem( SEM_PRINT );
	}
}

/*=========================================================================*/
/**
 * @fn		void main_task(VP_INT exinf)
 * @brief	Main Task
 * @param	extinf from uITRON Configurator
 */
/*=========================================================================*/
void main_task(VP_INT exinf)
{
	/* start High priority task */
	act_tsk( TASK_HI );
	
	/* start Low priority task */
	act_tsk( TASK_LO );
	
	/* exit main_tsk */
	ext_tsk();
}
