/**
 *	@file	cmdline.c
 *	@brief	command line
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
#include "serial.h"

/*--- file domain values ------------------------*/
static char				mInputBuffer[ KZCMDLINE_MAX_INPUT_CHAR ];
static int				mnWpBuffer;
static BOOL				mbCmdExitSignal;
static char*			msCommandArgv[ KZCMDLINE_MAX_ARGS ];
static int				mnCommandArgc;
static BOOL				mbPrintCurPath=TRUE;
static BOOL				mbPrintExeRet = FALSE;

/*--- Ascii Map  ------------------------*/

/*
	0:	ignored
	1:	letter
	2:	end
	3:	space
	4:	back
	5:	redirect
*/
static const B mAsciiMap[128] = {
2,0,0,0,0,0,0,0,4,3,2,0,0,2,0,0,		/* 0x00 - 0x0F */
4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		/* 0x10 - 0x1F */
3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,		/* 0x20 - 0x2F */
1,1,1,1,1,1,1,1,1,1,1,1,0,0,5,1,		/* 0x30 - 0x3F */
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,		/* 0x40 - 0x4F */
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,		/* 0x50 - 0x5F */
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,		/* 0x60 - 0x6F */
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,4,		/* 0x70 - 0x7F */
};

/*--- file domain functions ---------------------*/
static int	help_exe( int argc, char *argv[] );
static int	exit_exe( int argc, char *argv[] );
static int	showret_exe( int argc, char *argv[] );
static int	showpath_exe( int argc, char *argv[] );
static void	add_test_exe(void);

static KzCmdRegist_t	mCommandList[ KZCMDLINE_MAX_COMMANDS ] = {
	/* define default command line functions below. */
	{	help_exe,		"help",		"print command list"							},
	{	exit_exe,		"exit",		"exit command line"								},
	{	showret_exe,	"showret",	"show return value : showret 0  ... is off"		},
	{	showpath_exe,	"showpath",	"show currnt path  : showpath 0 ... is off"		},
};
static int				mnCommands = 4;		/** Number of the default commands */


/**
 * @fn		static void ResetBuffer(void)
 * @brief	reset mInputBuffer 
 */
static void ResetBuffer(void)
{
	int i;
	/* Input buffer reset */
	for( i=0; i<numof(mInputBuffer); ++i )	mInputBuffer[i] = 0;

	/* Write pointer reset */
	mnWpBuffer = 0;

	/* Number of Argument reset */
	mnCommandArgc = 0;

	/* Argument list rest */
	for(i=0;i<KZCMDLINE_MAX_ARGS;++i)	msCommandArgv[i] = 0;
}

/**
 * @fn		static int DoExe(void)
 * @brief	Execute command function
 * @return	Command function's return value	
 */
static int DoExe(void)
{
	int i;

	for(i=0;i<mnCommands;++i)
	{
		if( strcmp( mCommandList[i].msCmd, msCommandArgv[0] ) == 0 )
		{
			return  mCommandList[i].mfnExe( mnCommandArgc, msCommandArgv );
		}
	}
	
	printf("%s :Command not found\n",msCommandArgv[0]);
	return 0;
}

/**
 * @fn		static BOOL PharseCmd(int *ret)
 * @brief	Pharsing Command
 * @param	ret	executed function's return value
 * @return	TRUE	execute function done
 * @return	FALSE	half way of the pharsing
 */
static BOOL PharseCmd(int *ret)
{
	int i;
	char *redirect_file_name = 0;
	BOOL bRedirect = FALSE;
	mnCommandArgc = 0;
	*ret = 0;
	for(i=0;i<KZCMDLINE_MAX_INPUT_CHAR;++i)
	{
		B c = mInputBuffer[i];
		B type = mAsciiMap[c];

		switch( type )
		{
		case 1:/*letter*/
			if( i == 0 || mInputBuffer[ i-1] == 0 )
			{
				if( bRedirect )
					redirect_file_name = &mInputBuffer[ i ];
				else
					msCommandArgv[ mnCommandArgc++ ] = &mInputBuffer[ i ];
			}
			break;
		case 2:/*end*/
			{
				if( mnCommandArgc )
				{
					if( redirect_file_name == 0 )	*ret = DoExe();
					else
					{
						/* redirect */
						FILE *fp;
						if( (fp=fopen(redirect_file_name,"w")) != NULL )
						{
							KzSetStdoutFp( fp );
							*ret = DoExe();
							KzRelStdoutFp();
							fclose( fp );
						}
						else	*ret = DoExe();
					}
				}
			}
			return TRUE;
		case 3:/*space*/
			mInputBuffer[ i ] = 0;
			break;
		case 5:/*redirect*/
			bRedirect = TRUE;
			break;
		default:
		case 4:/*back*/
		case 0:/*ignored*/
			break;
		}
	}

	return FALSE;

}

/**
 * @fn		static BOOL SigBuffer( B c, BOOL *ret )
 * @brief	Receive a Charactor Signal
 * @param 	c	revieved charactor
 * @param	ret	executed function's return value
 * @return	TRUE	execute function done
 * @return	FALSE	half way of the pharsing
 */
static BOOL SigBuffer( B c, BOOL *ret )
{
	B type;
	if( c < 0 ) return FALSE;

	type = mAsciiMap[c];
	*ret = 0;

	if( mnWpBuffer >= (KZCMDLINE_MAX_INPUT_CHAR - 1 ) )
	{
		/* lack of buffer , signal end */
		if( type != 2 ) SigBuffer( 0, ret );
	}


	switch( type )
	{
	case 1:/*letter*/
	case 5:/*redirect*/
		mInputBuffer[ mnWpBuffer++] = c;
		KzPutchar( c );
		break;
	case 2:/*end*/
		KzPutchar('\r');
		KzPutchar('\n');
		mInputBuffer[ mnWpBuffer++] = 0;
		return PharseCmd( ret );
	case 3:/*space*/
		KzPutchar( ' ' );
		mInputBuffer[ mnWpBuffer++] = ' ';
		break;
	case 4:/*back*/
		if( mnWpBuffer )
		{
			KzPutchar(0x08);
			KzPutchar(0x20);
			KzPutchar(0x08);
			mInputBuffer[ mnWpBuffer--] = 0;
		}
		break;
	default:
	case 0:/*ignored*/
		break;
	}

	return FALSE;

}
/**
 * @fn		static void print_cmd(void)
 * @brief	print command line prompt
 */
static void print_cmd(void)
{
	KzPutchar('\r');
	if( mbPrintCurPath )
		printf("/%s>",KzGetCurPath() );
	else
	
		KzPutchar('>');
}

/*=========================================================================*/
/**
 * @fn		KZSTATUS_t	KzCmdlineStart(void)
 * @brief	Command Line Start
 * @ingroup	API_CommandLine
 * @return	KZ_OK	Success	(constant)
 * @note	The function does not return until KzCmdlineExit() is called
 */
/*=========================================================================*/
KZSTATUS_t	KzCmdlineStart(void)
{
	int k=0;

	serial_ctl_por( 1, IOCTL_CRLF | IOCTL_FCSND | IOCTL_FCRCV);
	
	mbCmdExitSignal = FALSE;
	ResetBuffer();
	print_cmd();
	for(;;)
	{
		int ret;
		if( SigBuffer( KzGetchar(),&ret ) == TRUE )
		{
			ResetBuffer();
			if( mbPrintExeRet )
				fprintf(stderr, "\n[OS INFO] return value: %d\n", ret );
			print_cmd();
		}

		if( mbCmdExitSignal ) break;

	}

	return KZ_OK;
}

/*=========================================================================*/
/**
 * @fn		KZSTATUS_t	KzCmdlineExit(void)
 * @brief	Command Line Exit
 * @ingroup	API_CommandLine
 * @return	KZ_OK	Success	(constant)
 */
/*=========================================================================*/
KZSTATUS_t	KzCmdlineExit(void)
{
	mbCmdExitSignal = TRUE;
	return KZ_OK;
}
/*=========================================================================*/
/**
 * @fn		KZSTATUS_t	KzCmdlineAdd( const KzCmdRegist_t *exe )
 * @brief	Add a Command  
 * @ingroup	API_CommandLine
 * @param	exe		add command
 * @return	KZ_OK	Success
 * @return	KZ_ERR	Lack of memory space for the command
 * @note	if KZ_ERR is occured, you can revise KZCMDLINE_MAX_COMMANDS 
 * @note	in kzconfig.h to increse the number of the command line.
 */
/*=========================================================================*/
KZSTATUS_t	KzCmdlineAdd( const KzCmdRegist_t *exe )
{
	if( mnCommands >= KZCMDLINE_MAX_COMMANDS ) return KZ_ERR;

	mCommandList[mnCommands].mfnExe = exe->mfnExe;
	mCommandList[mnCommands].msCmd  = exe->msCmd;
	mCommandList[mnCommands].msHelp = exe->msHelp;
	++mnCommands;

	return KZ_OK;
}
/*=========================================================================*/
/**
 * @fn		KZSTATUS_t	KzCmdlineAddMany( const KzCmdRegist_t *exe )
 * @brief	Add Many Commands
 * @ingroup	API_CommandLine
 * @param	exe		add command, must be terminated by ZERO
 * @return	KZ_OK	Success
 * @return	KZ_ERR	Lack of memory space for the command
 * @note	if KZ_ERR is occured, you can revise KZCMDLINE_MAX_COMMANDS 
 * @note	in kzconfig.h to increse the number of the command line.
 */
/*=========================================================================*/
KZSTATUS_t	KzCmdlineAddMany( const KzCmdRegist_t *exes )
{
	for(;;)
	{
		const KzCmdRegist_t *exe = exes++;
		KZSTATUS_t stat;
		if( exe->mfnExe == 0 ) break;
		stat = KzCmdlineAdd( exe );
		if( stat != KZ_OK ) return stat;
	}

	return KZ_OK;
}


/*--- Commands ------------------------*/
/**
 * @fn		static int help_exe( int argc, char *argv[] )
 * @brief	help function in command
 * @param	argc	number of the arguments
 * @param  argv	command string from command line
 * @return	0	(constant)
 */
static int help_exe( int argc, char *argv[] )
{
	int i;

	for(i=0;i<mnCommands;++i)
	{
		char str[9];
		if( strlen( mCommandList[i].msCmd ) < 9 )
		{
			int j;
			for(j=0;j<sizeof(str)-1;++j) str[j] = ' ';
			strcpy( &str[sizeof(str) - strlen(mCommandList[i].msCmd)-1],  mCommandList[i].msCmd );
			printf( "%s ... %s\n", str, mCommandList[i].msHelp);
		}
		else
			printf( "%s ... %s\n", mCommandList[i].msCmd, mCommandList[i].msHelp);
	}

	return 0;
}
/**
 * @fn		static int exit_exe( int argc, char *argv[] )
 * @brief	exit function in command
 * @param	argc	number of the arguments
 * @param  argv	command string from command line
 * @return	0	(constant)
 */
static int exit_exe( int argc, char *argv[] )
{
	KzCmdlineExit();
	return 0;
}
/**
 * @fn		static int showret_exe( int argc, char *argv[] )
 * @brief	show return value function in command
 * @param	argc	number of the arguments
 * @param  argv	command string from command line
 * @return	0	(constant)
 */
static int showret_exe( int argc, char *argv[] )
{
	if (argc == 1 )
		mbPrintExeRet = TRUE;
	else
		mbPrintExeRet = ( argv[1][0] == '0' )? FALSE : TRUE;
	return 0;
}
/**
 * @fn		static int showpath_exe( int argc, char *argv[] )
 * @brief	show path strings function in command
 * @param	argc	number of the arguments
 * @param  argv	command string from command line
 * @return	0	(constant)
 */
static int showpath_exe( int argc, char *argv[] )
{
	if (argc == 1 )
		mbPrintCurPath = TRUE;
	else
		mbPrintCurPath = ( argv[1][0] == '0' )? FALSE : TRUE;
	return 0;
}
/*--- Test Commands ------------------------*/
#if KZUSING_STDIO_TEST == 1
/**
 * @fn		static int arg_exe( int argc, char *argv[] )
 * @brief	show arg strings function in command
 * @param	argc	number of the arguments
 * @param  argv	command string from command line
 * @return	0	(constant)
 * @note	test command function
 */
static int arg_exe( int argc, char *argv[] )
{
	int i;

	for(i=0;i<argc;++i)
	{
		printf( "[%x] :%s\n", i, argv[i] );
	}
	return 0;
}

static  const KzCmdRegist_t	test_exes[] = {
	{	arg_exe,	"arg",		"print arg",	},
	{	0,			0,			0,				}		/* end mark */
};

extern void kzstdio_test_cmd(void);

/**
 * @fn		static void add_test_exe(void)
 * @brief	Add command line for testing
 */
static void add_test_exe(void)
{
	KzCmdlineAddMany( test_exes );
	kzstdio_test_cmd();
}

#endif	/* USING_STD_TEST == 1 */


