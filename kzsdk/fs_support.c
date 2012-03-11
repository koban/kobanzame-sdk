/**
 *	@file	fs_support.c
 *	@brief	filesystem support functions
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
 *	@note	filesystem re-entrant support functions
 */
 
#include "kobanzame.h"

static ID	mFsSem;

void	KzAttIniFatfs(ID semid)
{
	mFsSem = semid;
}


DWORD get_fattime (void)
{
	return 0;	
}


BOOL ff_cre_syncobj(BYTE a, _SYNC_t* s)
{
#if TARGET_TOOL == TOOL_VS2008
	return TRUE;
#else
	*s = mFsSem;
	return TRUE;
#endif
}

BOOL ff_del_syncobj(_SYNC_t s)
{
	return TRUE;
}

BOOL ff_req_grant(_SYNC_t s)
{
#if TARGET_TOOL == TOOL_VS2008
	return TRUE;
#else
	BOOL ret = (twai_sem( s, _FS_TIMEOUT ) == E_OK );
	return ret;
#endif
}
void ff_rel_grant(_SYNC_t s)
{
#if TARGET_TOOL == TOOL_VS2008
#else
	sig_sem( s );
#endif
}


