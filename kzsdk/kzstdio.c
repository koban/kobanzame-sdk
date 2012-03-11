/**
 *	@file	kzstdio.c
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
 */

/*--- include -----------------------------------*/
#include "kobanzame.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

/*--- define  -----------------------------------*/
/** file object type */
typedef struct {
	FILE		mFile;		/*!< file object */
	BOOL		mbUsed;		/*!< file object is used or not*/
} file_t;

#define	KZUSING_STDIO_TEST					(1)

/*--- file domain values ------------------------*/
static char		mScanfBuf[64];
static file_t	mFileObj[KZFILE_MAX_OPEN];		
static FILE*	mfpStdout = (FILE*) -1;

/**
 * @fn			void KzAttIniStdio(void)
 * @brief		Initialize called in uITRON ATT_INI
 */
void KzAttIniStdio(void)
{
	int i;
	for(i=0;i<KZFILE_MAX_OPEN;++i)
		mFileObj[i].mbUsed = FALSE;
}
/**
 * @fn			void KzSetStdoutFp(FILE* fp)
 * @brief		Set stdout to fp
 */
void KzSetStdoutFp(FILE* fp)
{
	mfpStdout = fp;
}
/**
 * @fn			void KzRelStdoutFp(void)
 * @brief		release STDOUT
 */
void KzRelStdoutFp(void)
{
	mfpStdout = (FILE*)-1;
}
/*=========================================================================*/
/**
 * @fn			int		Kz_fgetc( FILE *fp)
 * @brief		Standard C Compatible function: fgetc
 * @ingroup	API_stdio
 * @see			General Standard C documents
 * @note		Renamed to standard symbol name in kzstdio.h
 */
/*=========================================================================*/
int		Kz_fgetc( FILE *fp)
{
	if( fp == (FILE*)-1)
	{
		return (int)KzGetchar();
	}
	else
	{
		char c=0;
		Kz_fread(&c,1,1,fp);
		return (int)c;
	}
}
/*=========================================================================*/
/**
 * @fn			int		Kz_fputc(int c, FILE *fp)
 * @brief		Standard C Compatible function: fputc
 * @ingroup	API_stdio
 * @see			General Standard C documents
 * @note		Renamed to standard symbol name in kzstdio.h
 */
/*=========================================================================*/
int		Kz_fputc(int c, FILE *fp)
{
	BOOL bFile = (fp==(FILE*)-1 && mfpStdout==(FILE*)-1 )? FALSE	/* stdout */
		: ( fp==(FILE*)-2 )? FALSE	/* stderr */
		: TRUE;
	int ret;

	if( bFile )
	{
		if( fp==(FILE*)-1 )
			Kz_fwrite(&c,1,1,mfpStdout);
		else
			Kz_fwrite(&c,1,1,fp);
		ret = (int)c;
	}
	else
	{
		KzPutchar((B)c);
		ret = (int)c;
	}
	return c;
}
/**
 * @fn			static file_t *get_file_obj(void)
 * @brief		release a file object
 * @return		file object
 */
static file_t *get_file_obj(void)
{
	int i;
	file_t *ret=0;
	dis_dsp();
	for(i=0;i<KZFILE_MAX_OPEN;++i)
	{
		if( mFileObj[i].mbUsed == FALSE )
		{
			mFileObj[i].mbUsed = TRUE;
			ret = &mFileObj[i];
			break;
		}
	}
	ena_dsp();
	return ret;
}
/**
 * @fn			static void rel_file_obj(FILE *fp)
 * @brief		release file object
 * @note		fp a file object to release
 */
static void rel_file_obj(FILE *fp)
{
	int i;
	dis_dsp();
	for(i=0;i<KZFILE_MAX_OPEN;++i)
	{
		if( &mFileObj[i].mFile == fp)
		{
			mFileObj[i].mbUsed = FALSE;
			break;
		}
	}
	ena_dsp();
}


/*
 *  ポインタを格納できる整数型のデフォルトの定義
 */
#ifndef _intptr_
#if TARGET_TOOL == TOOL_GCC || TARGET_TOOL == TOOL_VDSP
#define _intptr_	long		// blackfin
#else
#define _intptr_	long long		// win32
#endif

#endif /* _intptr_ */

/*
 *  数値を文字列に変換
 */
#define CONVERT_BUFLEN	((sizeof(_intptr_) * CHAR_BIT + 2) / 3)
					/* _intptr_型の数値の最大文字数 */
/**
 * @fn			static int convert(unsigned _intptr_ val, unsigned int radix, const char *radchar, int width, int minus, int padzero, FILE *fp)
 * @note		This function is copied and modified from TOPPERS JSP
 */
static int
convert(unsigned _intptr_ val, unsigned int radix, const char *radchar,
		int width, int minus, int padzero, FILE *fp)
{
	char	buf[CONVERT_BUFLEN];
	int	i, j;
	int ret = 0;

	i = 0;
	do {
		buf[i++] = radchar[val % radix];
		val /= radix;
	} while (val != 0);

	width -= minus;
	if (minus > 0 && padzero > 0) {
		++ret;
		Kz_fputc('-', fp);
	}
	for (j = i; j < width; j++) {
		++ret;
		Kz_fputc((char)(padzero > 0 ? '0' : ' '), fp);
	}
	if (minus > 0 && padzero <= 0) {
		++ret;
		Kz_fputc('-', fp);
	}
	while (i > 0) {
		++ret;
		Kz_fputc(buf[--i], fp);
	}

	return ret;
}

/*
 *  ログ情報のフォーマット出力
 */
static const char raddec[] = "0123456789";
static const char radhex[] = "0123456789abcdef";
static const char radHEX[] = "0123456789ABCDEF";

/**
 * @fn			static int _printf_(const char *format, va_list args, FILE *fp)
 * @note		This function is copied and modified from TOPPERS JSP
 */
static int _printf_(const char *format, va_list args, FILE *fp)
{
	int		c;
	int		width;
	int		padzero;
	_intptr_	val;
	const char	*str;
	int		ret = 0;

	while ((c = *format++) != '\0') {
		if (c != '%') {
			Kz_fputc((char)c, fp);
			++ret;
			continue;
		}

		width = padzero = 0;
		if ((c = *format++) == '0') {
			padzero = 1;
			c = *format++;
		}
		while ('0' <= c && c <= '9') {
			width = width * 10 + c - '0';
			c = *format++;
		}
		if (c == 'l') {
			c = *format++;
		}
		switch (c) {
		case 'd':
			val = va_arg(args,int);
			if (val >= 0) {
				ret += convert((unsigned _intptr_) val, 10, raddec,
						width, 0, padzero, fp);
			}
			else {
				ret += convert((unsigned _intptr_)(-val), 10, raddec,
						width, 1, padzero, fp);
			}
			break;
		case 'u':
			val = va_arg(args,unsigned int);
			ret += convert((unsigned _intptr_) val, 10, raddec,
						width, 0, padzero, fp);
			break;
		case 'x':
		case 'p':
			val = va_arg(args,unsigned int);
			ret += convert((unsigned _intptr_) val, 16, radhex,
						width, 0, padzero, fp);
			break;
		case 'X':
			val = va_arg(args,unsigned int);
			ret += convert((unsigned _intptr_) val, 16, radHEX,
						width, 0, padzero, fp);
			break;
		case 'c':
			Kz_fputc((char)va_arg(args,int), fp);
			break;
		case 's':
			str = va_arg(args, const char *);
			while ((c = *str++) != '\0') {
				++ret;
				Kz_fputc((char)c, fp);
			}
			break;
		case '%':
			++ret;
			Kz_fputc('%', fp);
			break;
		case '\0':
			format--;
			break;
		default:
			break;
		}
	}
//	(*putc)('\n');
	return ret;
}
/**
 * @fn			static int _atox_( const char *str )
 * @brief		convert ascii to hex
 */
static int _atox_( const char *str )
{
	char buf[16],c,ans;
	int i,j=0;
	int ret = 0;
	for( i=0;i<sizeof(buf);++i) buf[i] = *str++;
	for( i=0;i<sizeof(buf);++i)
	{
		c = buf[i];	
		if( c >= 'A' && c <= 'F' ) c += 'a' - 'A';
		if( c == 0 ) break;
		ans = ( c >= '0' && c <= '9' )? c - '0'
			: ( c >= 'a' && c <= 'f' )? c - 'a' + 10
			: 0;
		ret = (ret << 4) + ans;
	}
	return ret;
}
/**
 * @fn			static void _vsscanf_(char *str, const char *format, va_list arg )
 * @brief		Standard C Compatible function: vsscanf
 */
static void _vsscanf_(char *str, const char *format, va_list arg )
{
	char c;
	char *input = str;

	while( c = *input ) { if( c == ' ' ) *input = 0; ++input; }

	input = str;
	while ( c = *format++ ) 
	{
		if ( c != '%' ) continue;
		c = *format++;
		switch( c )
		{
		case 'd':
			{
				int *out = va_arg( arg, int *);
				*out = atoi( input );
				while ( *input++  ) ;
			}
			break;
		case 'u':
			{
				unsigned int *out = va_arg( arg, unsigned int* );
				*out = atoi( input );
				while ( *input++ ) ;
			}
			break;
		case 's':
			{
				char *out = va_arg( arg, char *);
				while( c = *input++ ) *out++= c;
				*out = c;
			}
			break;
		case 'x': case 'X':
			{
				int *out = va_arg( arg, int *);
				*out = _atox_( input );
				while( *input++ );
			}
			break;
		case 'f':
			{
				double *out = va_arg( arg, double *);
				*out = atof( input );
				while( *input++ );
			}
		case 'c':
			{
				char *out = va_arg( arg, char *);
				*out = *input;
				while( *input++ );
			}
			break;
		}
	}
}
/*=========================================================================*/
/**
 * @fn			FILE*	Kz_fopen( const char *filename, const char *mode )
 * @brief		Standard C Compatible function: fopen
 * @ingroup	API_stdio
 * @see			General Standard C documents
 * @note		Renamed to standard symbol name in kzstdio.h
 */
/*=========================================================================*/
FILE*	Kz_fopen( const char *filename, const char *mode )
{
	int i,idx;
	const struct {
		const char	*ansi;
		BYTE		fatfs;
		BOOL		bApend;
	} conv_mode[] = {
		{	"r",		FA_OPEN_EXISTING | FA_READ				,	FALSE	},
		{	"w",		FA_CREATE_ALWAYS | FA_WRITE				,	FALSE	},
		{	"a",		FA_CREATE_ALWAYS | FA_WRITE				,	TRUE	},
		{	"r+",		FA_OPEN_EXISTING | FA_READ | FA_WRITE	,	FALSE	},
		{	"w+",		FA_CREATE_ALWAYS | FA_READ | FA_WRITE	,	FALSE	},
		{	"a+",		FA_CREATE_ALWAYS | FA_READ | FA_WRITE	,	TRUE	},
	};
	file_t *file_obj = get_file_obj();
	FILE *fp = &file_obj->mFile;
	char	path[KZFILE_MAX_PATH_LEN];

	if( fp == 0 ) return NULL;

	for(i=0;i,numof(conv_mode);++i)
	{
		if( strcmp( mode, conv_mode[i].ansi ) == 0 )
		{
			idx = i;
			break;
		}
	}
	if( i==numof(conv_mode) ) return NULL;

	KzSetPath( path, filename );
	if( f_open(fp,path,conv_mode[idx].fatfs ) != FR_OK )
	{
		/* file open fail*/
		rel_file_obj( fp );
		return NULL;
	}
	
	if( conv_mode[idx].bApend )
	{
		/* in case of append, set filepointer tail */
		f_lseek( fp, fp->fsize );
	}

	return fp;
}
/*=========================================================================*/
/**
 * @fn			int		Kz_fclose( FILE *fp)
 * @brief		Standard C Compatible function: fclose
 * @ingroup	API_stdio
 * @see			General Standard C documents
 * @note		Renamed to standard symbol name in kzstdio.h
 */
/*=========================================================================*/
int		Kz_fclose( FILE *fp)
{
	if( f_close( fp ) != FR_OK )	return EOF;
	rel_file_obj( fp );
	return 0;
	
}
/*=========================================================================*/
/**
 * @fn			int		Kz_fseek(FILE *fp, long offset, int whence )
 * @brief		Standard C Compatible function: fseek
 * @ingroup	API_stdio
 * @see			General Standard C documents
 * @note		Renamed to standard symbol name in kzstdio.h
 */
/*=========================================================================*/
int		Kz_fseek(FILE *fp, long offset, int whence )
{
	DWORD ofs = ( whence == SEEK_SET )?	offset
		:( whence == SEEK_END )? fp->fsize + offset
		:fp->fptr + offset;/*(whence == SEEK_CUR ) */
	return (f_lseek( fp, ofs ) == FR_OK )? 0 : -1;
}
/*=========================================================================*/
/**
 * @fn			long		Kz_ftell(FILE *fp)
 * @brief		Standard C Compatible function: ftell
 * @ingroup	API_stdio
 * @see			General Standard C documents
 * @note		Renamed to standard symbol name in kzstdio.h
 */
/*=========================================================================*/
long		Kz_ftell(FILE *fp)
{
	FIL *fp_ = (FIL*)fp;
	return (long)fp_->fptr;
}
/*=========================================================================*/
/**
 * @fn			size_t	Kz_fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
 * @brief		Standard C Compatible function: fread
 * @ingroup	API_stdio
 * @see			General Standard C documents
 * @note		Renamed to standard symbol name in kzstdio.h
 */
/*=========================================================================*/
size_t	Kz_fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	UINT total = size * nmemb;
	UINT done;
	if( f_read(stream, ptr, total, &done ) != FR_OK )
	{
		/* ERROR発生 feof/ferrorは未対応 */
		return 0;
	}
	return done/size;
}
/*=========================================================================*/
/**
 * @fn			size_t	Kz_fwrite(const void *ptr, size_t size, size_t nmemb,FILE *stream)
 * @brief		Standard C Compatible function: fwrite
 * @ingroup	API_stdio
 * @see			General Standard C documents
 * @note		Renamed to standard symbol name in kzstdio.h
 */
/*=========================================================================*/
size_t	Kz_fwrite(const void *ptr, size_t size, size_t nmemb,FILE *stream)
{
	UINT total = size * nmemb;
	UINT done;
	if( f_write(stream, ptr, total, &done ) != FR_OK )
	{
		/* ERROR発生 feof/ferrorは未対応 */
		return 0;
	}
	return done/size;
}
/*=========================================================================*/
/**
 * @fn			char*	Kz_fgets(char *s, int n, FILE *fp )
 * @brief		Standard C Compatible function: fgets
 * @ingroup	API_stdio
 * @see			General Standard C documents
 * @note		Renamed to standard symbol name in kzstdio.h
 */
/*=========================================================================*/
char*	Kz_fgets(char *s, int n, FILE *fp )
{
	int i;
	BOOL bEOF = FALSE;
	BOOL bFoundTer = FALSE;
	for(i=0;i<n-1;++i)
	{
		char c = Kz_fgetc(fp);
		/* stdinの場合は指標のためにプリントする */
		if( fp == (FILE*)-1) 
		{
			if( c == '\r' ) Kz_fputc( '\n', fp );
			Kz_fputc( c, fp );
		}

		s[i] = c;
		if( c == '\r' || c == '\n'  )
		{
			bFoundTer = TRUE;
			break;
		}
		if( c == 0 ) 
		{
			bEOF = TRUE;
			break;
		}
	}
	s[i] = 0;

	// 残りTerminateされるまで空読みする 
	while( bFoundTer == FALSE )
	{
		char c = Kz_fgetc(fp);
		/* stdinの場合は指標のためにプリントする */
		if( fp == (FILE*)-1) Kz_fputc( c, fp );
		if( c == '\r' || c == '\n' || c == 0 ) bFoundTer = TRUE;
	}

	return (bEOF)? NULL : s;
}
/*=========================================================================*/
/**
 * @fn			int		Kz_fputs(const char *s, FILE *fp)
 * @brief		Standard C Compatible function: fputs
 * @ingroup	API_stdio
 * @see			General Standard C documents
 * @note		Renamed to standard symbol name in kzstdio.h
 */
/*=========================================================================*/
int		Kz_fputs(const char *s, FILE *fp)
{
	int ret = 0;
	int c;
	while( c = *s++ )
	{
		Kz_fputc( c, fp );
		ret++;
	}
	return ret;
}
/*=========================================================================*/
/**
 * @fn			int		Kz_fscanf(FILE *fp,const char *format,...)
 * @brief		Standard C Compatible function: fscanf
 * @ingroup	API_stdio
 * @see			General Standard C documents
 * @note		Renamed to standard symbol name in kzstdio.h
 */
/*=========================================================================*/
int		Kz_fscanf(FILE *fp,const char *format,...)
{
	va_list args;
	va_start(args,format);
	Kz_fgets( mScanfBuf, sizeof(mScanfBuf), fp );
	_vsscanf_( mScanfBuf, format, args );
	return 0;
}
/*=========================================================================*/
/**
 * @fn			int		Kz_fprintf(FILE *fp,const char *format,...)
 * @brief		Standard C Compatible function: fprintf
 * @ingroup	API_stdio
 * @see			General Standard C documents
 * @note		Renamed to standard symbol name in kzstdio.h
 */
/*=========================================================================*/
int		Kz_fprintf(FILE *fp,const char *format,...)
{
	va_list args;
	va_start(args,format );
	return Kz_vfprintf( fp, format, args );
}
/*=========================================================================*/
/**
 * @fn			int		Kz_printf(const char *format,...)
 * @brief		Standard C Compatible function: printf
 * @ingroup	API_stdio
 * @see			General Standard C documents
 * @note		Renamed to standard symbol name in kzstdio.h
 */
/*=========================================================================*/
int		Kz_printf(const char *format,...)
{
	va_list args;
	va_start(args, format);
	return Kz_vfprintf( stdout, format, args  );
}
/*=========================================================================*/
/**
 * @fn			int		Kz_scanf(const char *format,...)
 * @brief		Standard C Compatible function: scanf
 * @ingroup	API_stdio
 * @see			General Standard C documents
 * @note		Renamed to standard symbol name in kzstdio.h
 */
/*=========================================================================*/
int		Kz_scanf(const char *format,...)
{
	va_list args;
	va_start(args, format );
	Kz_fgets( mScanfBuf, sizeof(mScanfBuf), stdin );
	_vsscanf_( mScanfBuf, format, args );
	return 0;
}
/*=========================================================================*/
/**
 * @fn			char*	Kz_gets(char *s)
 * @brief		Standard C Compatible function: gets
 * @ingroup	API_stdio
 * @see			General Standard C documents
 * @note		Renamed to standard symbol name in kzstdio.h
 */
/*=========================================================================*/
char*	Kz_gets(char *s)
{
	char *in = s;
	char c;
	for(;;)
	{
		c = Kz_fgetc(stdin);
		if( c == '\r' || c == '\n' || c == 0 )
		{
			*in = 0;
			break;
		}
		*in++ = c;
	}
	return s;
}
/*=========================================================================*/
/**
 * @fn			int		Kz_puts(const char *s)
 * @brief		Standard C Compatible function: puts
 * @ingroup	API_stdio
 * @see			General Standard C documents
 * @note		Renamed to standard symbol name in kzstdio.h
 */
/*=========================================================================*/
int		Kz_puts(const char *s)
{
	int ret = 0;
	int c;
	while( c = *s++ )
	{
		Kz_putc((B)c, stdout );	
		++ret;
	}
	return ret;
}
/*=========================================================================*/
/**
 * @fn			int		Kz_vprintf(const char *format, va_list arg)
 * @brief		Standard C Compatible function: vprintf
 * @ingroup	API_stdio
 * @see			General Standard C documents
 * @note		Renamed to standard symbol name in kzstdio.h
 */
/*=========================================================================*/
int		Kz_vprintf(const char *format, va_list arg)
{
	return Kz_vfprintf(stdout, format, arg );
}
/*=========================================================================*/
/**
 * @fn			int		Kz_vfprintf(FILE *fp,const char *format, va_list arg )
 * @brief		Standard C Compatible function: vfprintf
 * @ingroup	API_stdio
 * @see			General Standard C documents
 * @note		Renamed to standard symbol name in kzstdio.h
 */
/*=========================================================================*/
int		Kz_vfprintf(FILE *fp,const char *format, va_list arg )
{
	return _printf_( format, arg, fp );
}

/*=========================================================================*/
/**
 * @fn			int		Kz_getchar(void)
 * @brief		Standard C Compatible function: getchar
 * @ingroup	API_stdio
 * @see			General Standard C documents
 * @note		Renamed to standard symbol name in kzstdio.h
 */
/*=========================================================================*/
int		Kz_getchar(void) { return Kz_getc(stdin); }

/*=========================================================================*/
/**
 * @fn			int		Kz_putchar(int c)
 * @brief		Standard C Compatible function: putchar
 * @ingroup	API_stdio
 * @see			General Standard C documents
 * @note		Renamed to standard symbol name in kzstdio.h
 */
/*=========================================================================*/
int		Kz_putchar(int c) { return Kz_fputc(c,stdout); }


/*=========================================================================*/
/**
 * @fn			iint		Kz_getc( FILE *fp)
 * @brief		Standard C Compatible function: getc
 * @ingroup	API_stdio
 * @see			General Standard C documents
 * @note		Renamed to standard symbol name in kzstdio.h
 */
/*=========================================================================*/
int		Kz_getc( FILE *fp) { return Kz_fgetc(fp); }

/*=========================================================================*/
/**
 * @fn			int		Kz_putc(int c, FILE *fp)
 * @brief		Standard C Compatible function: putc
 * @ingroup	API_stdio
 * @see			General Standard C documents
 * @note		Renamed to standard symbol name in kzstdio.h
 */
/*=========================================================================*/
int		Kz_putc(int c, FILE *fp) { return Kz_fputc(c,fp); }



/*--- Test Routinue ----------------------------------*/
#if KZUSING_STDIO_TEST == 1

static void printret_int( int ret )			{ 	printf(" --- return value is %d\n", ret );	}
static void printret_char( int ret )		{ 	printf(" --- return value is '%c'\n", ret );	}
static void printret_str( const char *ret ) { 	printf(" --- return value is %s\n", ret );	}
static void printret_FILE( FILE *ret )		{ 	printf(" --- return value is 0x%08X\n", ret );	}
static void print_title(const char *title)	{	printf("\n+++++ testing %s +++++\n",title); }

static int puts_stdout_exe( int argc, char *argv[] )
{
	printf("start puts stdout exe\n");

	print_title		("fputs(\"abc\",stdout)");
	printret_int	( fputs( "abc", stdout ) );

	print_title		("fputc('0',stdout)");
	printret_char	( fputc('0',stdout ) );

	print_title		("fprintf(stdout,\"This %s\",\"is a pen.\")");
	printret_int	( fprintf(stdout,"This %s" , "is a pen.") );

	print_title		("printf(\"Yes, %s\",\"it is.\")");
	printret_int	( printf( "Yes, %s" , "it is.") );

	print_title		("puts(\"Kobanzame\")" );
	printret_int	( puts( "Kobanzame") );

	print_title		("putchar('a')"  );
	printret_char	( putchar('a') );

	print_title		("putc('B',stdout)");
	printret_char	( putc('B',stdout) );

	return 0;
}
static int gets_stdin_exe( int argc, char *argv[] )
{
	char str0[16],str1[16];
	int a,b;

	printf("start gets kzstdin exe\n");

	print_title		("scanf(\"%d %d\",a,b)");
	scanf("%d %d", &a, &b );
	printf("%d, %d\n", a, b );

	print_title		("scanf(\"%s %s\",a,b)");
	scanf("%s %s", str0, str1 );
	printf("%s, %s", str0, str1 );

	print_title		("scanf(\"%x %x\",a,b)");
	scanf("%x %x", &a, &b );
	printf("%d, %d", a, b );

	print_title		("fgets(str0,16,stdin)");
	printret_str	( fgets(str0,16,stdin) );

	print_title		("getc(stdin)");
	printret_char	( getc(stdin) );

	return 0;
}

static  const KzCmdRegist_t	test_exes[] = {
	{	puts_stdout_exe,	"puts",		"kzstdio test puts functions"	},
	{	gets_stdin_exe,		"gets",		"kzstdio test gets functions"	},
	{	0,					0,			0,								}		/* end mark */
};
/*=========================================================================*/
/**
 * @fn			KZSTATUS_t KzAddCmdStdioTest(void)
 * @brief		Add stdio test commands
 * @return		KZ_OK	Success
 * @return		Lack of memory space for the command. See KzCmdlineAddMany
 * @note		After this function call. User can use following commands.
 *				<UL><Li>puts</Li><Li>gets</Li></UL>
 */
/*=========================================================================*/
KZSTATUS_t KzAddCmdStdioTest(void)
{
	return KzCmdlineAddMany( test_exes );
}

#endif	/* KZUSING_STDIO_TEST == 1 */
