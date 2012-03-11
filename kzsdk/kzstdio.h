/**
 *	@file	kzstdio.h
 *	@brief	KOBANZAME SDK stdio function
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

#ifndef _kzstdio_h_
#define _kzstdio_h_

/*--- include -----------------------------------*/
#include <stdarg.h>
#include "ff.h"

/*--- renamer  ----------------------------------*/
#define		FILE		FIL

#undef		fopen
#define		fopen		Kz_fopen
#undef		fclose
#define		fclose		Kz_fclose
#undef		fseek
#define		fseek		Kz_fseek
#undef		ftell
#define		ftell		Kz_ftell
#undef		fread
#define		fread		Kz_fread
#undef		fwrite
#define		fwrite		Kz_fwrite
#undef		fgets
#define		fgets		Kz_fgets
#undef		fputs
#define		fputs		Kz_fputs
#undef		fgetc
#define		fgetc		Kz_fgetc
#undef		fputc
#define		fputc		Kz_fputc
#undef		fscanf
#define		fscanf		Kz_fscanf
#undef		fprintf
#define		fprintf		Kz_fprintf
#undef		printf
#define		printf		Kz_printf
#undef		scanf
#define		scanf		Kz_scanf
#undef		getchar
#define		getchar		Kz_getchar
#undef		putchar
#define		putchar		Kz_putchar
#undef		gets
#define		gets		Kz_gets
#undef		puts
#define		puts		Kz_puts
#undef		getc
#define		getc		Kz_getc
#undef		putc
#define		putc		Kz_putc
#undef		vprintf
#define		vprintf		Kz_vprintf
#undef		vfprintf
#define		vfprintf	Kz_vfprintf

/*--- kobanzame stdio  --------------------------*/
#undef	stdin
#undef	stdout
#undef	stderr
#define		stdin			((FILE*)-1)
#define		stdout			((FILE*)-1)
#define		stderr			((FILE*)-2)


/*--- global functions  -------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

extern FILE*	Kz_fopen( const char *filename, const char *mode );
extern int		Kz_fclose( FILE *fp);
extern int		Kz_fseek(FILE *fp, long offset, int origin); 
extern size_t	Kz_fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
extern size_t	Kz_fwrite(const void *ptr, size_t size, size_t nmemb,FILE *stream);
extern char*	Kz_fgets(char *s, int n, FILE *fp );
extern int		Kz_fputs(const char *s, FILE *fp);
extern int		Kz_fgetc( FILE *fp);
extern int		Kz_fputc(int c, FILE *fp);
extern int		Kz_fscanf(FILE *fp,const char *format,...);
extern int		Kz_fprintf(FILE *fp,const char *format,...);
extern int		Kz_printf(const char *format,...);
extern int		Kz_scanf(const char *format,...);
extern char*	Kz_gets(char *s);
extern int		Kz_puts(const char *s);
extern int		Kz_vprintf(const char *format, va_list arg);
extern int		Kz_vfprintf(FILE *fp,const char *format, va_list arg );

extern int		Kz_getchar(void);
extern int		Kz_putchar(int c);
extern int		Kz_getc( FILE *fp);
extern int		Kz_putc(int c, FILE *fp);

#ifdef __cplusplus
}
#endif


#endif	/* !_kzstdio_h_ */
