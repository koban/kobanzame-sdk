/**
 *	@file	kzdsp.h
 *	@brief	DSP Libray
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

/*
 *	To use this DSP Libray, please include "kobanzame.h"
 *	Don't include this file directly.
 */

#ifndef _kz_dsp_h_
#define _kz_dsp_h_

/*--- include -----------------------------------*/
#if TARGET_TOOL == TOOL_GCC || TARGET_TOOL == TOOL_VDSP
#include <fract.h>
#elif TARGET_TOOL == TOOL_VS2008

#else
#error Target no found
#endif


/*--- fixed define  -----------------------------*/
#define		KZQ_VAL					(26)	/*!< KOBANZAME SDK 32bit Q value */

/*=========================================================================*/
/**
 * @fn			int KzDspAdd( int a, int b)
 * @brief		a + b with saturation
 * @ingroup	API_AudioDSP
 * @param		a parameter1
 * @param		b parameter2
 * @return		a + b with saturation
 */
/*=========================================================================*/
Inline int KzDspAdd( int a, int b)
{
#if TARGET_TOOL == TOOL_GCC || TARGET_TOOL == TOOL_VDSP
	return add_fr1x32(a,b);
#else /* TARGET_TOOL == TOOL_VS2008 */
#endif
}
/*=========================================================================*/
/**
 * @fn			int KzDspSub( int a, int b)
 * @brief		a - b with saturation
 * @ingroup	API_AudioDSP
 * @param		a parameter1
 * @param		b parameter2
 * @return		a - b with saturation
 */
/*=========================================================================*/
Inline int KzDspSub( int a, int b)
{
#if TARGET_TOOL == TOOL_GCC || TARGET_TOOL == TOOL_VDSP
	return sub_fr1x32(a,b);
#else /* TARGET_TOOL == TOOL_VS2008 */
#endif
}
/*=========================================================================*/
/**
 * @fn			int KzDspMpy( int a, int b)
 * @brief		a * b with saturation
 * @ingroup	API_AudioDSP
 * @param		a parameter1
 * @param		b parameter2
 * @return		a * b with saturation
 * @see			http://blackfin.s36.coreserver.jp/index.php?id=89
 */
/*=========================================================================*/
Inline int KzDspMpy( int a, int b)
{
#if TARGET_TOOL == TOOL_GCC || TARGET_TOOL == TOOL_VDSP
	{
		int y;
		asm volatile("									\
			a1 = %2.H * %1.L(M), a0 = %2.H * %1.H;		\
			a1 += %1.H * %2.L(M);						\
			a0 = a0 <<  (5);							\
			a1 = a1 >>> (10);							\
			%0 = (a0+=a1);"								\
			: "=d"(y) : "d"(a),"d"(b) : "A0","A1" );
		return y;
	}
#else /* TARGET_TOOL == TOOL_VS2008 */
#endif
}
/*=========================================================================*/
/**
 * @fn			int KzDspMac( int *a, int *b, int n)
 * @brief		Multiply and Accumrate
 * @ingroup	API_AudioDSP
 * @param		a parameter1
 * @param		b parameter2
 * @param		n loops
 * @return		ans += *a++ * *b++; loop n
 */
/*=========================================================================*/
Inline int KzDspMac( int *a, int *b, int n )
{
#if TARGET_TOOL == TOOL_GCC || TARGET_TOOL == TOOL_VDSP
	{
		int y;
		asm volatile("												\
			i0 = %1;												\
			i1 = %2;												\
			p0 = %3;												\
			a1 = a0 = 0 || r0 = [i0++] || r1 = [i1++];				\
			lsetup(4,8) lc0 = p0;									\
			a1 += r0.H * r1.L(M), a0 += r0.H * r1.H;				\
			a1 += r1.H * r0.L(M) || r0 = [i0++] || r1 = [i1++];		\
			a0 = a0 <<  (5);										\
			a1 = a1 >>> (10);										\
			%0 = (a0+=a1);"											\
			: "=d"(y)  
#if TARGET_TOOL == TOOL_GCC
			: "d"(a),"d"(b),"d"(n) 
#else
			: "i"(a),"i"(b),"p"(n) 
#endif
			: "R0","R1","I0","I1","P0","A0","A1","LC0" );
		return y;
	}
#else /* TARGET_TOOL == TOOL_VS2008 */
#endif
}
/*=========================================================================*/
/**
 * @fn			int KzDspSat(int a)
 * @brief		Saturation
 * @ingroup	API_AudioDSP
 * @param		a parameter1
 * @return		saturation limit is [-1.0,1.0]
 * @note		if (a>1.0) ans =1.0;  if ( a<-1.0) ans=-1.0; else ans = a;
 */
/*=========================================================================*/
Inline int KzDspSat(int a)
{
#if TARGET_TOOL == TOOL_GCC || TARGET_TOOL == TOOL_VDSP
	return shr_fr1x32( shl_fr1x32( a, 31-KZQ_VAL), 31-KZQ_VAL );
#else /* TARGET_TOOL == TOOL_VS2008 */
#endif
}
/*=========================================================================*/
/**
 * @fn			int KzDspAbs(int a)
 * @brief		Absolute
 * @ingroup	API_AudioDSP
 * @param		a parameter1
 * @return		if ( a<0.0) ans=-a; else ans = a;
 */
/*=========================================================================*/
Inline int KzDspAbs(int a)
{
#if TARGET_TOOL == TOOL_GCC || TARGET_TOOL == TOOL_VDSP
	return abs_fr1x32( a );
#else /* TARGET_TOOL == TOOL_VS2008 */
#endif
}
/*=========================================================================*/
/**
 * @fn			int KzDspShl(int a, int s)
 * @brief		Shift left with saturation
 * @ingroup	API_AudioDSP
 * @param		a parameter1
 * @param		s shift value ( minus value is acceptable )
 * @return		a << s
 */
/*=========================================================================*/
Inline int KzDspShl(int a, int s)
{
#if TARGET_TOOL == TOOL_GCC || TARGET_TOOL == TOOL_VDSP
	return shl_fr1x32( a , s);
#else /* TARGET_TOOL == TOOL_VS2008 */
#endif
}
/*=========================================================================*/
/**
 * @fn			int KzDspShr(int a, int s)
 * @brief		Shift right with saturation
 * @ingroup	API_AudioDSP
 * @param		a parameter1
 * @param		s shift value ( minus value is acceptable )
 * @return		a >> s
 */
/*=========================================================================*/
Inline int KzDspShr(int a, int s)
{
#if TARGET_TOOL == TOOL_GCC || TARGET_TOOL == TOOL_VDSP
	return shr_fr1x32( a , s);
#else /* TARGET_TOOL == TOOL_VS2008 */
#endif
}
/*=========================================================================*/
/**
 * @fn			int KzDspNeg(int a)
 * @brief		Nagate
 * @ingroup	API_AudioDSP
 * @param		a parameter1
 * @return		-a
 */
/*=========================================================================*/
Inline int KzDspNeg(int a)
{
#if TARGET_TOOL == TOOL_GCC || TARGET_TOOL == TOOL_VDSP
	return negate_fr1x32( a );
#else /* TARGET_TOOL == TOOL_VS2008 */
#endif
}
/*=========================================================================*/
/**
 * @fn			float KzDspTof(int a)
 * @brief		To float
 * @ingroup	API_AudioDSP
 * @param		a parameter1
 * @return		(float)a
 */
/*=========================================================================*/
Inline float KzDspTof(int a)
{
	return (float)a / (1<<KZQ_VAL);
}
/*=========================================================================*/
/**
 * @fn			int KzDspToi(float a)
 * @brief		To int
 * @ingroup	API_AudioDSP
 * @param		a parameter1
 * @return		(int)a
 */
/*=========================================================================*/
Inline int KzDspToi(float a)
{
	return  (int)(a * (1<<KZQ_VAL));
}

#endif	/* !_kz_dsp_h_ */
