/**
 *	@file	fs_cmd.c
 *	@brief	filesystem relative command line
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
#include <string.h>

/*--- USER define  ------------------------------*/
#define MORE_MAX_LIST_LINES				(32)		/*!< @brief USER DEFINE: Max lines at more command */

/*--- file domain values ------------------------*/
static FATFS	mFatFs;								/*!< @brief filesystem object */
static char		mCurPath[KZFILE_MAX_PATH_LEN]={0};	/*!< @brief Current file path */
static char		mTmpPath1[KZFILE_MAX_PATH_LEN];		/*!< @brief Temporary path 1 */
static char		mTmpPath2[KZFILE_MAX_PATH_LEN];		/*!< @brief Temporaty path 2 */

#if _USE_LFN
static char		mLfname[512];
#endif
/*--- file domain functions ---------------------*/
/*--- global functions  -------------------------*/
const char *KzGetCurPath(void) { return mCurPath; }

/**
 * @fn			static DWORD GetFileLen( const char *filename )
 * @brief		Get file length
 * @param		filename filename full path
 * @return		file length byte order
 */
static DWORD GetFileLen( const char *filename )
{
	FILINFO FileInfo;
	if( f_stat( filename, &FileInfo ) == FR_OK )
	{
		return FileInfo.fsize;
	}
	return 0;
	
}
/**
 * @fn			static BOOL IsRootDir(const char *path)
 * @brief		Check the path is root directory
 * @param		path input path
 * @return		TRUE  ... Root directory /
 *				FALSE ... None root directory
 */
static BOOL IsRootDir(const char *path)
{
	return (path[0] == 0 );
}
/**
 * @fn			static BOOL DelDir(char *path)
 * @brief		Delete one directory from the path
 * @param		path input path
 * @return		TRUE  ... Success to delete /
 *				FALSE ... No deleted directory
 */
static BOOL DelDir(char *path)
{
	int idx = strlen(path) - 1;
	char c;
	if( IsRootDir(path) ) return FALSE;

	while( ( c=path[idx] ) != '/' && idx ) --idx;
	path[idx] = 0;
	return TRUE;
}
/**
 * @fn			static void AddPath(char *path, const char *dir)
 * @brief		Add directory to the path
 * @param		path destination path buffer
 * @param		dir  the add directory
 * @note		buffer length safe
 */
static void AddPath(char *path, const char *dir)
{
	int total_size = strlen( path ) + strlen(dir) + 1;
	if( total_size < KZFILE_MAX_PATH_LEN-1 )
	{
		strcat( path, "/" );
		strcat( path, dir );
	}
}
/**
 * @fn			static void AddCharPath(char *path, char c)
 * @brief		Add charactor to the path
 * @param		path destination path buffer
 * @param		c    the add charactor 
 * @note		strcat with buffer length safe
 */
static void AddCharPath(char *path, char c)
{
	int total_size = strlen( path ) + 1;
	if( total_size < KZFILE_MAX_PATH_LEN-1 )
	{
		char str[2] = { c, 0 };
		strcat( path, str );
	}
}
/**
 * @fn			static void CopyPath(char *dest, const char *src)
 * @brief		Copy path strings
 * @param		dest destination path buffer
 * @param		src  source path buffer
 * @note		strcpy with buffer length safe
 */
static void CopyPath(char *dest, const char *src)
{
	if( strlen(src) < KZFILE_MAX_PATH_LEN -1 )
	{
		strcpy( dest, src );
	}
}
/**
 * @fn			static void SetPath( char *dest, const char *arg)
 * @brief		Makeing a full path form the arg
 * @param		dest destination path buffer
 * @param		arg  arguments for making a full path
 */
static void SetPath( char *dest, const char *arg)
{
	char c;

	if( arg[0] == '/' )
	{
		/* full path */
		++arg;
		dest[0] = 0;
	}
	
	while( c = *arg++ )
	{
		if( c == '.' )
		{
			c = *arg++;
			if( c == '.' ) DelDir(dest);
			
			/* search next '/'	*/
			while( ( c = *arg++ ) != '/' ) if(!c) return;
		}
		else if( c != '/ ' )
		{
			/* add path */
			if( !IsRootDir(dest) ) AddCharPath( dest, '/' );
			AddCharPath( dest, c );
			while( ( c = *arg++ ) != '/' )
			{	
				AddCharPath( dest, c );
				if(!c) return;
			}
		}
	}
}
/**
 * @fn			static BOOL TestPath(const char *path)
 * @brief		Testing the path is exist or not
 * @param		path input path
 * @return		TRUE ...  Found /
 *				FALSE ... Not found
 */
static BOOL TestPath(const char *path)
{
	DIR dir;
	return f_opendir( &dir, path) == FR_OK;
}
/**
 * @fn			static char* ExtractFileName(char *path)
 * @brief		Extract filename strings from the path
 * @param		path input path
 * @return		file name strings pointer
 */
static char* ExtractFileName(char *path)
{
	size_t i;
	size_t len = strlen(path);
	char *ret = path + len - 1;
	while( *ret != '/' ) { if( ret== path) return ret; --ret; }
	return ret+1;
}
/**
 * @fn			static int ls_exe( int argc, char *argv[] )
 * @brief		COMMAND: list up files
 * @note		COMMAND: ls
 */
static int ls_exe( int argc, char *argv[] )
{
	FRESULT res;
	DIR dir;
	FILINFO Finfo;
	long p1;
	UINT s1, s2;
	FATFS *fs;

	p1 = s1 = s2 = 0;
	res = f_opendir(&dir, mCurPath );
	if( res ) goto error;
#if _USE_LFN
	Finfo.mLfname = mLfname;
	Finfo.lfsize = sizeof(mLfname);
#endif
	for(;;)
	{
		res = f_readdir(&dir, &Finfo);
		if ((res != FR_OK) || !Finfo.fname[0]) break;
		if (Finfo.fattrib & AM_DIR) {
			s2++;
		} else {
			s1++; p1 += Finfo.fsize;
		}
		printf("%c%c%c%c%c %u/%02u/%02u %02u:%02u %9lu  %s",
				(Finfo.fattrib & AM_DIR) ? 'D' : '-',
				(Finfo.fattrib & AM_RDO) ? 'R' : '-',
				(Finfo.fattrib & AM_HID) ? 'H' : '-',
				(Finfo.fattrib & AM_SYS) ? 'S' : '-',
				(Finfo.fattrib & AM_ARC) ? 'A' : '-',
				(Finfo.fdate >> 9) + 1980, (Finfo.fdate >> 5) & 15, Finfo.fdate & 31,
				(Finfo.ftime >> 11), (Finfo.ftime >> 5) & 63, Finfo.fsize, Finfo.fname);
#if _USE_LFN
		for (p2 = strlen(Finfo.fname); p2 < 14; p2++)
			putchar(' ');
		printf("%s\n", mLfname);
#else
		putchar('\n');
#endif
	}
	printf("%4u File(s),%10lu bytes total\n%4u Dir(s)", s1, p1, s2);
	if (( res = f_getfree("", (DWORD*)&p1, &fs)) == FR_OK)
		printf(", %10lu bytes free\n", p1 * fs->csize * 512);
	else
		printf("\n NG: %d\n",res );
	
	return 0;
error:
	printf("Drive Error\n");
	return 0;
}
/**
 * @fn			static int pwd_exe( int argc, char *argv[] )
 * @brief		COMMAND: show current directory
 * @note		COMMAND: pwd
 */
static int pwd_exe( int argc, char *argv[] )
{
	if( IsRootDir(mCurPath) )
		printf("/\n");
	else
		printf("/%s/\n",mCurPath);
	return 0;
}
/**
 * @fn			static int cd_exe( int argc, char *argv[] )
 * @brief		COMMAND: change current directory
 * @note		COMMAND: cd [directory]
 */
static int cd_exe( int argc, char *argv[] )
{
	if( argc <= 1 ) return 0;
	
	CopyPath( mTmpPath1, mCurPath );
	SetPath( mTmpPath1, argv[1] );
	if( TestPath( mTmpPath1 ) )
		/* OK */
		CopyPath( mCurPath, mTmpPath1);
	else
		/* NG */
		printf("Can not found the path\n");

	return 0;
}
/**
 * @fn			static int mkdir_exe( int argc, char *argv[] )
 * @brief		COMMAND: make a directory
 * @note		COMMAND: mkdir [directory]
 */
static int mkdir_exe( int argc, char *argv[] )
{
	if( argc <= 1 ) goto usage;
	CopyPath( mTmpPath1, mCurPath );
	SetPath( mTmpPath1, argv[1] );
	if( f_mkdir( mTmpPath1 ) != FR_OK )
		/* NG */
		printf("Can not make the directory\n");
	return 0;
usage:
	printf("usage: mkdir [directory]\n");
	printf("make directory\n");
	return 0;
}
/**
 * @fn			static int rmdir_exe( int argc, char *argv[] )
 * @brief		COMMAND: remove directory
 * @note		COMMAND: rmdir [directory]
 */
static int rmdir_exe( int argc, char *argv[] )
{
	FRESULT res;
	if( argc <= 1 ) goto usage;
	CopyPath( mTmpPath1, mCurPath );
	SetPath( mTmpPath1, argv[1] );
	res = f_unlink( mTmpPath1 );
	if( res == FR_DENIED )
		printf("Can not remove the directory.\nThe directory is not empty.\n");
	else if( res != FR_OK )
		printf("Can not remove the directory.\n");
	return 0;
usage:
	printf("usage: rmdir [directory]\n");
	printf("remove directory\n");
	printf("the directory must be empty\n");
	return 0;
}
/**
 * @fn			static int cp_exe( int argc, char *argv[] )
 * @brief		COMMAND: copy file
 * @note		COMMAND: cp [src] [dest]
 */
static int cp_exe( int argc, char *argv[] )
{
	int i;
	FRESULT res;
	FIL src, dest;
	char buffer[64];
	UINT br,bw;
	char *in_file;
	if( argc <= 2 ) goto usage;
	/* make a src file path */
	CopyPath( mTmpPath1, mCurPath );
	SetPath( mTmpPath1, argv[1] );
	res = f_open(&src, mTmpPath1, FA_READ );
	if( res != FR_OK )
	{
		printf("Can not open source file\n");
		goto error1;
	}
	/* make a dest file path */
	in_file=ExtractFileName(argv[1]);
	i = strlen( argv[2] );
	if( argv[2][i-1] == '.' ) { argv[2][i-1] = 0; strcat( argv[2], in_file ); }
	CopyPath( mTmpPath1, mCurPath );
	SetPath( mTmpPath1, argv[2] );

	res = f_open(&dest, mTmpPath1, FA_WRITE|FA_CREATE_NEW );
	if( res != FR_OK )
	{
		printf("Can not open destination file\n");
		goto error2;
	}
	/* copy */
    for (;;) {
        res = f_read(&src, buffer, sizeof(buffer), &br);
        if (res || br == 0) break;   // error or eof
        res = f_write(&dest, buffer, br, &bw);
        if (res || bw < br) break;   // error or disk full
    }
	f_close( &dest );
error2:
	f_close( &src );
error1:
	return 0;
usage:
	printf("usage: cp [src] [dest]\n");
	printf("copy to dest file.\n");
	return 0;
}

/**
 * @fn			static int rm_exe( int argc, char *argv[] )
 * @brief		COMMAND: remove file
 * @note		COMMAND: rm [filename]
 */
static int rm_exe( int argc, char *argv[] )
{
	FRESULT res;
	if( argc <= 1 ) goto usage;
	CopyPath( mTmpPath1, mCurPath );
	SetPath( mTmpPath1, argv[1] );
	res = f_unlink( mTmpPath1 );
	if( res != FR_OK )
		printf("Can not remove the file.\n");
	return 0;
usage:
	printf("usage: rm [file]\n");
	printf("remove file.\n");
	return 0;
}
/**
 * @fn			static int mv_exe( int argc, char *argv[] )
 * @brief		COMMAND: move file
 * @note		COMMAND: move [src file] [dest file]
 */
static int mv_exe( int argc, char *argv[] )
{
	int i;
	char *in_file;
	if( argc <= 2 ) goto usage;
	/* make a src file	*/
	CopyPath( mTmpPath1, mCurPath );
	SetPath( mTmpPath1, argv[1] );
	
	/* make a dest file */
	in_file=ExtractFileName(argv[1]);
	i = strlen( argv[2] );
	if( argv[2][i-1] == '.' ) { argv[2][i-1] = 0; strcat( argv[2], in_file ); }
	CopyPath( mTmpPath2, mCurPath );
	SetPath( mTmpPath2, argv[2] );

	if( f_rename(mTmpPath1,mTmpPath2) != FR_OK )
		printf("Can not move file.\n");
	return 0;
usage:
	printf("usage: mv [src] [dest]\n");
	printf("move to dest file.\n");
	return 0;
}
/**
 * @fn			static int dump_exe( int argc, char *argv[] )
 * @brief		COMMAND: file dump
 * @note		COMMAND: dump [filename]
 */
static int dump_exe( int argc, char *argv[] )
{
	FILE *fp;
	char buf[16];
	size_t readsize;
	size_t i;
	DWORD ofs = 0;
	if( argc <= 1 ) goto usage;
	if( ( fp = fopen( argv[1], "r") ) != NULL )
	{
		for(;;)
		{
			readsize = fread(buf,sizeof(char),sizeof(buf),fp);
			if( readsize )
			{
				printf("%08lX ", ofs);
				for(i=0;i<readsize;++i)
					printf(" %02X", buf[i] );
				printf("  ");
				for(i=0;i<readsize;++i)
					putchar(((buf[i] < 0x20)||(buf[i] >= 0x7F)) ? '.' : buf[i]);
				putchar('\n');
				if( readsize != sizeof(buf) ) break;
				ofs+=sizeof(buf);
			}
			else
				break;
		}
		fclose( fp );
	}
	else
		printf("File not found : %s\n",argv[1]);

	return 0;
usage:
	printf("usage: dump [filename]\n");
	printf("file dump\n");
	return 0;
}
/**
 * @fn			static int more_exe( int argc, char *argv[] )
 * @brief		COMMAND: list up test file
 * @note		COMMAND: more [filename]
 */
static int more_exe( int argc, char *argv[] )
{
	FILE *fp;
	int i;
	char buf[256];
	if( argc <= 1 ) goto usage;
	if( ( fp = fopen( argv[1], "r") ) != NULL )
	{
		for(;;)
		{
			i = 0;
			for(i=0;i<MORE_MAX_LIST_LINES;++i)
			{
				if( fgets(buf,sizeof(buf),fp) == NULL ) return 0;
				puts( buf );
				puts("\r\n");
			}
			/* hit any key */
			if( getc(stdin) == 'q' ) break;
		}
		fclose( fp );
	}
	else
		printf("File not found : %s\n",argv[1]);
		
	
	return 0;
usage:
	printf("usage: 	more [filename]\n");
	printf("show text file\n");
	return 0;
}

#if KZUSING_SD_BENCH_TEST == 1
//static L3DATA long big_mem[1024*1024];
static long big_mem[1024*2];
/**
 * @fn			static int sdcard_test_exe( int argc, char *argv[] )
 * @brief		COMMAND: SDCARD Test
 * @note		COMMAND: sdcard [SDCARD NAME] ... 
 * @todo		support L3 Memory
 */
static int sdcard_test_exe(int argc, char *argv[] )
{
#define TEST_FILE_SIZE	(1024*1024*10)		/* 10MBYTE */
#define TEST_FILE_NAME	"test_tmp"
#define SD_TEST_VERSION	1
#define TEST_PTN_4BYTE	0x55565758L
	FILE *fp;
	UW i;
	UW len = TEST_FILE_SIZE;
	SYSTIM tim=0;
	FATFS *fs;
	FRESULT res;
	DWORD p1;
	char sdcard_name[64] = {0};
	const char *fattype_str[] = { "Unkonwn", "FAT12", "FAT16", "FAT32" };
	const char *sdtype_str[] =  { "MMC", "SD v1", "SD v2", "SDHC" };
	/* Prepare buffer */
	for(i=0;i<numof(big_mem);++i)	big_mem[i] = TEST_PTN_4BYTE;
	for(i=1;i<argc;++i)
	{
		strcat( sdcard_name, argv[i] );
		strcat( sdcard_name, " " ); 
	}
	printf("++++ Test Infomation ++++\n" );
	printf("Test Version: %d\n", SD_TEST_VERSION );
	
	printf("\n++++ SD CARD Infomation ++++\n" );
	printf("SD-Card Name = %s\n", sdcard_name );
	res = f_getfree("", &p1, &fs );
	printf("FAT type = %s\n", fattype_str[fs->fs_type] );
	printf("Card type = %s\n", sdtype_str[KzMMCGetCardType()] );
	printf("Total size = %d KB\n", (fs->max_clust - 2) * fs->csize /2 );

	/* Write Test */
	printf("\n++++ Write Test ++++\n" );
	if( ( fp = fopen( TEST_FILE_NAME, "w") ) != NULL )
	{
		printf("Test Write Start : file length :%d\n", len );
		set_tim(&tim);		/* timeを0にする */
		for(i=0;i<len;i+=sizeof( big_mem ) )
		{
			if( ( len - i ) < sizeof(big_mem ) )
				fwrite(big_mem,sizeof(char),len-i,fp);
			else
				fwrite(big_mem,sizeof(char),sizeof(big_mem),fp);
		}
		get_tim(&tim);
		fclose(fp);
		printf("total %dms / %d(kbyte/sec)\n", tim, len / tim );
	}
	else
	{
		printf("Write ERROR!!!!\n");
		goto error;
	}
	
	/* Read Test */
	tim=0;
	printf("\n++++ Read Test ++++\n" );
	if( ( fp = fopen( TEST_FILE_NAME, "r") ) != NULL )
	{
		printf("Test Read Start : file length :%d\n", len );
		set_tim(&tim);		/* timeを0にする */
		for(i=0;i<len;i+=sizeof( big_mem ) )
			fread(big_mem,sizeof(char),sizeof(big_mem),fp);
		get_tim(&tim);
		fclose(fp);
		printf("total %dms / %d(kbyte/sec)\n", tim, len / tim );
	}
	else
	{
		printf("Read ERROR!!!!\n");
		goto error;
	}
		
	/* remove file */
	CopyPath( mTmpPath1, mCurPath );
	SetPath( mTmpPath1, TEST_FILE_NAME );
	res = f_unlink( mTmpPath1 );
	
	printf("\n++++ Test END ++++\n" );
	
	return 0;
error:
	printf("SD Card ERROR. Test Stop\n");
	return 0;	
}

#endif

/*=========================================================================*/
/**
 * @fn			void KzSetPath(char *dest, const char *arg)
 * @brief		SDK Private : Set file path
 * @param		dest destination buffer
 * @param		arg  path argument
 */
/*=========================================================================*/
void KzSetPath(char *dest, const char *arg)
{
	CopyPath( dest, mCurPath );
	SetPath( dest, arg );
}

/*--- command list ------------------------------*/
static  const KzCmdRegist_t	exes[] = {
	{	ls_exe,				"ls",		"show directory list"			},
	{	pwd_exe,			"pwd",		"show current directory"		},
	{	cd_exe,				"cd",		"change directory"				},
	{	mkdir_exe,			"mkdir",	"make directory"				},
	{	rmdir_exe,			"rmdir",	"remove directory"				},
	{	cp_exe,				"cp",		"copy file"						},
	{	rm_exe,				"rm",		"remove file"					},
	{	mv_exe,				"mv",		"move file"						},
	{	dump_exe,			"dump",		"file dump"						},
	{	more_exe,			"more",		"show text file"				},
#if KZUSING_SD_BENCH_TEST == 1
	{	sdcard_test_exe,	"sdcard",	"SD Card test"					},
#endif
	{	0,					0,			0,								}		/* end mark */
};

/*=========================================================================*/
/**
 * @fn			KZSTATUS_t KzFilesystemStart(void)
 * @brief		Filesystem start
 * @ingroup	API_CommandLine
 * @return		KZ_OK	Success
 * @return		KZ_ERR	Filesystem cannot start. ( mainly SD-Card is not inserted )
 * @warning	Before this function call. User must insert SD-Card, otherwise
 *				filesystem won't start and this function returns KZ_ERR.
 */
/*=========================================================================*/
KZSTATUS_t KzFilesystemStart(void)
{
	
	if( f_mount( 0, &mFatFs ) == FR_OK && TestPath("") )
	{
		return KZ_OK;
	}
	return KZ_ERR;
}
/*=========================================================================*/
/**
 * @fn			KZSTATUS_t KzAddCmdFilesystem(void)
 * @brief		Add Filesystem commands
 * @ingroup	API_CommandLine
 * @return		KZ_OK	Success
 * @return		Lack of memory space for the command. See KzCmdlineAddMany
 * @note		After this function call. User can use following commands.
 *				<UL><Li>ls</Li><Li>pwd</Li><Li>cd</Li><Li>mkdir</Li><Li>rmdir</Li>
 *				<Li>cp</Li><Li>rm</Li><Li>mv</Li><Li>dump</Li><Li>more</Li></UL>
 */
/*=========================================================================*/
KZSTATUS_t KzAddCmdFilesystem(void)
{	
	return KzCmdlineAddMany( exes );	
}
