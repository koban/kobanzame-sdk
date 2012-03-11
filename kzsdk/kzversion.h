/**
 *	@file	kzversion.h
 *	@brief	KOBANZAME SDK Version File
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
 
#ifndef _kz_version_h_
#define _kz_version_h_

#define		KZVER_MAJOR			(01)
#define		KZVER_MINOR			(41)
#define		KZVER_BUILD_IDX		(9)
#define		KZVER_COPYRIGHT		"KOBANZAME SDK Project"

#if TARGET_TOOL ==  TOOL_VDSP
extern const char KzBuildDate[];
#define		KZVER_BUILD_DATE	KzBuildDate
#elif TARGET_TOOL == TOOL_GCC
#define		KZVER_BUILD_DATE	"Unknown build date"
#endif

#endif

