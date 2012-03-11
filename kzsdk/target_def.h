/**
 *	@file	target_def.h
 *	@brief	KOBANZAME SDK Target selector
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
 *		$Revision: $
 */

#ifndef _target_def_h_
#define _target_def_h_

/*	target.hを必ずプロジェクトに一つ宣言をしその中で
	TARGET_TOOL	に下のうちどれか一つを定義すること
	例 : Visual DSPで設計する場合 
	target.h

		#define TARGET_TOOL		TOOL_VDSP
	
*/
#define		TOOL_GCC				(0)
#define		TOOL_VDSP				(1)
#define		TOOL_VS2008				(2)



#endif	/* !_target_def_h_ */
