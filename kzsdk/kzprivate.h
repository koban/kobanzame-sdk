/**
 *	@file	kzprivate.h
 *	@brief	SDK Private Global Function define
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
 *	@note	ONLY kobanzame.h can include this code
 *			Assume that kobanzame defines are already available.
 */
 
#ifndef _kz_private_h_
#define _kz_private_h_




/*--- kernel global functions  -------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/*-- kzlib.c --*/
void		KzAttIni(VP_INT arg);

/*-- kzdev.c --*/
void		KzDevCycHander(void);
void		KzDevPollingTask(VP_INT arg);

/*-- kzdev_audio.c --*/
void		KzAudioDspTask(VP_INT arg);
void		KzAudioISRSport0(void);

#ifdef __cplusplus
}
#endif


/*--- private global functions  -------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/*-- fs_cmd.c --*/
extern const char*	KzGetCurPath(void);
extern void KzSetPath( char *dest, const char *arg);
extern void KzSetStdoutFp(FILE *fp);
extern void KzRelStdoutFp(void);

/*-- serial_io.c --*/
extern B KzGetchar(void);
extern void KzPutchar(B c);

/*-- kzdev_mmc.c --*/
extern int KzMMCGetCardType(void);

#ifdef __cplusplus
}
#endif

#endif	/* !_kz_private_h_ */
