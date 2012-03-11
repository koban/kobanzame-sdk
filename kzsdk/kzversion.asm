/**
 *	@file	kzversion.asm
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
 */
 

.section constdata;
.global _KzBuildDate;

.BYTE _KzBuildDate[] = __DATE__,'/',__TIME__,0;
