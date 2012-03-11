/**
 *	@file	kzconfig.h
 *	@brief	KOBANZAME SDK USER configuration file
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

 /** 
 * @def 	KZCACAHE_ENABLE
 * @brief	If set (1) Cache Enable. only supported on VDSP++ 
 * @ingroup	UserCongifuration
 */
#define KZCACAHE_ENABLE						(0)


/** 
 * @def 	KZFILE_MAX_OPEN
 * @brief	Max files that filesystem can open 
 * @ingroup	UserCongifuration
 */
#define KZFILE_MAX_OPEN						(10)

/**
 * @def		KZFILE_MAX_PATH_LEN
 * @brief	Max file path length
 * @ingroup	UserCongifuration
 */
#define KZFILE_MAX_PATH_LEN					(512)

/**
 * @def		KZCMDLINE_MAX_INPUT_CHAR
 * @brief	Max Command Line charactor length
 * @ingroup	UserCongifuration
 */
#define KZCMDLINE_MAX_INPUT_CHAR			(128)

/**
 * @def		KZCMDLINE_MAX_COMMANDS
 * @brief	Max allocate number of the commands.
 * @ingroup	UserCongifuration
 */
#define KZCMDLINE_MAX_COMMANDS				(32)

/**
 * @def		KZCMDLINE_MAX_ARGS
 * @brief	Max arguments that command line execute function can receive.
 * @ingroup	UserCongifuration
 */
#define KZCMDLINE_MAX_ARGS					(16)

/** 
 * @def		KZAUDIO_MAX_AUDIO_NBLOCKS
 * @brief	Max Audio buffer blocks that callback can receive.
 * @ingroup	UserCongifuration
 */
#define		KZAUDIO_MAX_AUDIO_NBLOCKS		(32)

/** 
 * @def		KZUSING_SD_BENCH_TEST
 * @brief	(1) add SD-Card bench mark test command in filesystem command
 * @ingroup	UserCongifuration
 */
#define		KZUSING_SD_BENCH_TEST			(1)


