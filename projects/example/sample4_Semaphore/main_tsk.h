/**
 *	@file	main_tsk.h
 *	@brief	KOBANZAME SDK Main Task
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
 
 
#ifndef _main_tsk_h_
#define _main_tsk_h_

#ifndef _MACRO_ONLY
/*--- include -----------------------------------*/
#include "kobanzame.h"

/*--- global functions --------------------------*/
extern void main_task( VP_INT extinf );
extern void high_task( VP_INT extinf );
extern void low_task( VP_INT extinf );

#endif	/* !_MACRO_ONLY */

/*--- define ------------------------------------*/

/**	@define MAIN_TSK_PRI
 *		メインタスクの優先順位　1 〜 16　までの値(1が優先度大) 
 */
#define MAIN_TSK_PRI		8

/**	@define MAIN_TSK_STKSIZE
 *		メインタスクのスタックサイズ 
 */
#define MAIN_TSK_STKSIZE	4096


#endif
