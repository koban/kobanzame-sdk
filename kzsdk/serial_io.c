/**
 *	@file	serial_io.c
 *	@brief	KOBANZAME SDK Serial I/O function
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
 *	@note Serial I/O Definitions
 */

/*--- include -----------------------------------*/
#include "target_def.h"
#include "kernel.h"
#include <stdio.h>

#if TARGET_TOOL == TOOL_GCC || TARGET_TOOL == TOOL_VDSP
#include "serial.h"
#endif

B KzGetchar(void)
{
	char ret;
#if TARGET_TOOL == TOOL_GCC || TARGET_TOOL == TOOL_VDSP
	serial_rea_dat(1,&ret,1);
#elif TARGET_TOOL == TOOL_VS2008
#undef getchar
	ret = getchar();
#else
# error		You should select tool in target.h
#endif

	return ret;
}

void KzPutchar(B c)
{

#if TARGET_TOOL == TOOL_GCC || TARGET_TOOL == TOOL_VDSP
	serial_wri_dat(1,(char*)&c,1);
#elif TARGET_TOOL == TOOL_VS2008
#undef putchar
	putchar( c );
#endif
}
