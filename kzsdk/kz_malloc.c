/**
 *	@file	kz_malloc.c
 *	@brief	re-entrant malloc support
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
 *	@note	This file is for supports re-entrant heap opration
 */

 
 
#define HEAP_CHECK			(0)

#include <stdlib.h>
#include "jsp_kernel.h"


#if HEAP_CHECK == 1
unsigned long	max_heap = 0;
#endif

/*=========================================================================*/
/**
 * @fn			void *malloc(size_t size)
 * @brief		Overload malloc for re-entrant heap operation
 */
/*=========================================================================*/
void *malloc(size_t size)
{
	void * result;

	if ( iniflg && !sense_lock() )
	{
		dis_dsp();
		result = heap_malloc( 0, size );
		ena_dsp();
	}
	else
	{
		result = heap_malloc( 0, size );
	}
	
#if HEAP_CHECK == 1
	{
		unsigned long r = (unsigned long)result + size;
		if( r > max_heap) max_heap = r;
	}
#endif
	
	return( result );
}

/*=========================================================================*/
/**
 * @fn			void free(void *ptr)
 * @brief		Overload free for re-entrant heap operation
 */
/*=========================================================================*/
void free(void *ptr)
{
	void * result;
	
	if ( iniflg && !sense_lock() )
	{
		dis_dsp();
		heap_free( 0, ptr );
		ena_dsp();
	}
	else
	{
		heap_free( 0, ptr );
	}
	
}
