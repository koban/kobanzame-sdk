/**
 *	@file	kzdev.c
 *	@brief	KOBANZAME SDK Device Drivers
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

/*--- define  -----------------------------------*/
#define USING_LED_AS_DEBUG_PORT				(0)

/*--- file domain values ------------------------*/
static	ID		mCycDevPol;
static	ID		mTskDevPol;

/*-- Switch Control Block --*/
#define KZDEV_SW_NUM		(2)
#define KZDEV_SW_SCAN_TIME	(20)
static const int sw_def[] = { 10, 11 };
static struct {
	KzSwCbk_t			mfnCbk;
	struct {
		int				mnSwBit;
		BOOL			mbIniVal;
		BOOL			mbCurVal;
	} mIndi[KZDEV_SW_NUM];
	int					mnScanCount;
	BOOL				mbForceCbk;
} mSwCB;

/*-- LED Control Block --*/
#define KZDEV_LED_NUM		(2)
#define KZDEV_LED_SCAN_TIME	(5)
static const int led_def[] = { 8, 9 };
typedef enum {LED_OFF, LED_ON,LED_Blink } LEDStat_t;	/* LED status */
static struct {
	struct {
		int				mnLedBit;
		LEDStat_t		mnStat;
		BOOL			mbBlinkTimeOn;
		int				mnBlinkTimeOFF;
		int				mnBlinkTimeON;
		int				mnBlinkCount;
	} mIndi[KZDEV_LED_NUM];
	int					mnScanCount;
} mLedCB;

/*-- MEMS Control Block --*/
static struct {
	KzMemsCbk_t			mfnCbk;
} mMemsCB;

/*-- Audio Control Block --*/
static struct {
	KzAudioCbk_t		mfnCbk;
	long				mlFreq;
	int					mnBlock;
} mAudioCB;
	
/*-- MMC Control Block --*/
#define KZDEV_MMC_SCAN_TIME		(10)
static struct {
	int					mnScanCount;
} mMmcCB;

/*--- initializer -------------------------------*/
/**
 * @fn			void KzAttIniDeviceDrivers(ID idCycDevPol, ID idTskDevPol)
 * @brief		Driver Initialize
 * @param		idCycDevPol ID for Device driver cyclic hander defined in kernel_id.h
 * @param		idTskDevPol ID for Device driver task defined in kernel_id.h
 * @note		SDK Private function
 */
void KzAttIniDeviceDrivers(ID idCycDevPol, ID idTskDevPol)
{
	int i;
	
	mCycDevPol = idCycDevPol;
	mTskDevPol = idTskDevPol;
	
#if KZCACAHE_ENABLE == 1
	/*-- cache init --*/
	kzdev_cache_ini();
#endif
	
	/*-- mmc init --*/
	kzdev_mmc_att_ini();
	
	/*-- mems init --*/
	kzdev_mems_att_ini();
	
	/*-- codec init --*/
	kzdev_codec_att_ini();
	
	/*-- Switch Block Initialize --*/
	
	for(i=0;i<KZDEV_SW_NUM;++i)
	{
		mSwCB.mIndi[i].mnSwBit = sw_def[i];
		mSwCB.mIndi[i].mbIniVal = FALSE;
		mSwCB.mIndi[i].mbCurVal = FALSE;
		KZ_GPIO_SET_DIR_IN( mSwCB.mIndi[i].mnSwBit );		
		
		/* store Initial status */
		mSwCB.mIndi[i].mbIniVal = ( KZ_GPIO_GET( mSwCB.mIndi[i].mnSwBit ) )? FALSE : TRUE;
		mSwCB.mIndi[i].mbCurVal = mSwCB.mIndi[i].mbIniVal;
	}
	mSwCB.mfnCbk = 0;
	mSwCB.mnScanCount = 0;
	mSwCB.mbForceCbk = FALSE;
	
	/*-- LED Block Initialize --*/
	for(i=0;i<KZDEV_LED_NUM;++i)
	{
		mLedCB.mIndi[i].mnLedBit = led_def[i];
		mLedCB.mIndi[i].mnStat = LED_OFF;
		mLedCB.mIndi[i].mbBlinkTimeOn = FALSE;
		mLedCB.mIndi[i].mnBlinkTimeOFF = 0;
		mLedCB.mIndi[i].mnBlinkTimeON = 0;
		mLedCB.mIndi[i].mnBlinkCount = 0;
		KZ_GPIO_SET_HI( mLedCB.mIndi[i].mnLedBit );	
		KZ_GPIO_SET_DIR_OUT( mLedCB.mIndi[i].mnLedBit );	
	}
	mLedCB.mnScanCount = 0;
	
	/*-- MMC Block Initialize --*/
	mMmcCB.mnScanCount = 0;
}

/*--- Cyclic Driver -----------------------------*/
/**
 * @fn			static void device_pol(void)
 * @brief		Device Driver polling every 1ms interval
 */
static void device_pol(void)
{
	int i;
	
	/*-- Switch Block Polling --*/	
	if( ++mSwCB.mnScanCount >= KZDEV_SW_SCAN_TIME )
	{
		mSwCB.mnScanCount = 0;
		for(i=0;i<KZDEV_SW_NUM;++i)
		{
			BOOL sw = (KZ_GPIO_GET( mSwCB.mIndi[i].mnSwBit ))? FALSE : TRUE;
			if( sw != mSwCB.mIndi[i].mbCurVal || mSwCB.mbForceCbk  )
			{
				mSwCB.mIndi[i].mbCurVal = sw;
				if( mSwCB.mfnCbk )	mSwCB.mfnCbk( (KzSW_t)i, sw );
			}
		}
		mSwCB.mbForceCbk = FALSE;
	}
	
	/*-- LED Block Polling --*/
#if USING_LED_AS_DEBUG_PORT == 0
	if( ++mLedCB.mnScanCount >= KZDEV_LED_SCAN_TIME )
	{
		mLedCB.mnScanCount = 0;
		for(i=0;i<KZDEV_LED_NUM;++i)
		{
			BOOL bOn=FALSE;
			if( mLedCB.mIndi[i].mnStat == LED_ON ) 	bOn = TRUE;
			else if (mLedCB.mIndi[i].mnStat == LED_Blink )
			/* in case of LED_Blink */
			{
				mLedCB.mIndi[i].mnBlinkCount += KZDEV_LED_SCAN_TIME;
				if( mLedCB.mIndi[i].mbBlinkTimeOn )
				{
					bOn = TRUE;
					if( mLedCB.mIndi[i].mnBlinkCount >= mLedCB.mIndi[i].mnBlinkTimeON )
					{
						mLedCB.mIndi[i].mbBlinkTimeOn = FALSE;
						mLedCB.mIndi[i].mnBlinkCount = 0;
					}
				}
				else
				{	
					bOn = FALSE;
					if( mLedCB.mIndi[i].mnBlinkCount >= mLedCB.mIndi[i].mnBlinkTimeOFF )
					{
						mLedCB.mIndi[i].mbBlinkTimeOn = TRUE;
						mLedCB.mIndi[i].mnBlinkCount = 0;
					}
				}
			}
			if( bOn )	KZ_GPIO_SET_LO( mLedCB.mIndi[i].mnLedBit );	
			else		KZ_GPIO_SET_HI( mLedCB.mIndi[i].mnLedBit );	
		}
	}
#endif
	
	/*-- MEMS polling --*/
	if( kzdev_mems_isReady() )
	{
		int x,y,z;
		kzdev_mems_getVal( &x, &y, &z );
		if( mMemsCB.mfnCbk ) mMemsCB.mfnCbk(x,y,z);
	}
	/*-- MMC polling --*/
	if( ++mMmcCB.mnScanCount >= KZDEV_MMC_SCAN_TIME )
	{
		mMmcCB.mnScanCount = 0;
		disk_timerproc();
	}
}
/**
 * @fn			void KzDevPollingTask(VP_INT arg)
 * @brief		Device Driver Task
 * @note		SDK Private function
 */
void KzDevPollingTask(VP_INT arg)
{
	/*-- Start Polling --*/
	sta_cyc( mCycDevPol );	
	
	for(;;)
	{
		slp_tsk();
		device_pol();
	}
}
/**
 * @fn			void KzDevCycHander(void)
 * @brief		Cyclic handler for Device Driver for making time tick.
 * @note		SDK Private function
 */
void KzDevCycHander(void)
{
	iwup_tsk( mTskDevPol);
}

/*--- Switch Driver -----------------------------*/
/*=========================================================================*/
/**
 * @fn			KZSTATUS_t	KzSwStart(KzSwCbk_t cbk)
 * @brief		Swith start
 * @ingroup	API_DeviceDriver
 * @param		cbk User callback function. This function is called in DeviceDriver(middle priority) every switches Status changes.
 * @return		KZ_OK	Success 
 * @return		KZ_ERR	Fail to start switch
 */
/*=========================================================================*/
KZSTATUS_t	KzSwStart(KzSwCbk_t cbk)
{
	KZSTATUS_t ret = KZ_ERR;
	if( mSwCB.mfnCbk == 0 )
	{
		mSwCB.mfnCbk = cbk;
		mSwCB.mbForceCbk = TRUE;
		ret = KZ_OK;
	}
	return ret;
}
/*=========================================================================*/
/**
 * @fn			KZSTATUS_t	KzSwStop(void)
 * @brief		Swith stop
 * @ingroup	API_DeviceDriver
 * @return		KZ_OK	Success 
 * @return		KZ_ERR	Fail to stop switch
 */
/*=========================================================================*/
KZSTATUS_t	KzSwStop(void)
{
	KZSTATUS_t ret = KZ_ERR;
	if( mSwCB.mfnCbk  )
	{
		mSwCB.mfnCbk = 0;
		ret = KZ_OK;
	}
	return ret;

}
/*--- LED Driver --------------------------------*/
/*=========================================================================*/
/**
 * @fn			KZSTATUS_t	KzLedOn(KzLED_t nLedNo)
 * @brief		Set LED ON
 * @ingroup	API_DeviceDriver
 * @param		nLedNo KOBANZAME hardware LED number
 * @return		KZ_OK	Success 
 * @return		KZ_ERR	Undefined led number
 */
/*=========================================================================*/
KZSTATUS_t	KzLedOn(KzLED_t nLedNo)
{
	if( nLedNo >= KZDEV_LED_NUM) return KZ_ERR;
	mLedCB.mIndi[ nLedNo ].mnStat = LED_ON;
	return KZ_OK;
}
/*=========================================================================*/
/**
 * @fn			KZSTATUS_t	KzLedOff(KzLED_t nLedNo)
 * @brief		Set LED OFF
 * @ingroup	API_DeviceDriver
 * @param		nLedNo KOBANZAME hardware LED number
 * @return		KZ_OK	Success 
 * @return		KZ_ERR	Undefined led number
 */
/*=========================================================================*/
KZSTATUS_t	KzLedOff(KzLED_t nLedNo)
{
	if( nLedNo >= KZDEV_LED_NUM) return KZ_ERR;
	mLedCB.mIndi[ nLedNo ].mnStat = LED_OFF;
	return KZ_OK;
}
/*=========================================================================*/
/**
 * @fn			KZSTATUS_t	KzLedBlink(KzLED_t nLedNo,int nOnTime, int nOffTime)
 * @brief		Set LED blink
 * @ingroup	API_DeviceDriver
 * @param		nLedNo KOBANZAME hardware LED number
 * @param		nOnTime Interval of LED Lit time (ms)
 * @param		nOffTime Interval of LED Off time (ms)
 * @return		KZ_OK	Success 
 * @return		KZ_ERR	Undefined led number
 */
/*=========================================================================*/
KZSTATUS_t	KzLedBlink(KzLED_t nLedNo,int nOnTime, int nOffTime)
{
	if( nLedNo >= KZDEV_LED_NUM) return KZ_ERR;
	mLedCB.mIndi[ nLedNo ].mbBlinkTimeOn = TRUE;
	mLedCB.mIndi[ nLedNo ].mnBlinkTimeOFF = nOffTime;
	mLedCB.mIndi[ nLedNo ].mnBlinkTimeON = nOnTime;
	mLedCB.mIndi[ nLedNo ].mnBlinkCount = 0;
	mLedCB.mIndi[ nLedNo ].mnStat = LED_Blink;
	return KZ_OK;
}
/*--- MEMS Driver -------------------------------*/
/*=========================================================================*/
/**
 * @fn			KZSTATUS_t	KzMemsStart(KzMemsCbk_t cbk)
 * @brief		MEMS start
 * @ingroup	API_DeviceDriver
 * @param		cbk User callback function. This function is called in DeviceDriver(middle priority) every MEMS Status changes.
 * @return		KZ_OK	Success 
 * @return		KZ_ERR	Fail to start MEMS
 */
/*=========================================================================*/
KZSTATUS_t	KzMemsStart(KzMemsCbk_t cbk)
{
	KZSTATUS_t ret = KZ_ERR;
	if( mMemsCB.mfnCbk == 0 )
	{
		mMemsCB.mfnCbk = cbk;
		ret= (kzdev_mems_start())? KZ_OK : KZ_ERR;
		if (ret==KZ_ERR) mMemsCB.mfnCbk = 0;
	}
	return ret;
}
/*=========================================================================*/
/**
 * @fn			KZSTATUS_t	KzMemsStop(void)
 * @brief		MEMS stop
 * @ingroup	API_DeviceDriver
 * @return		KZ_OK	Success 
 * @return		KZ_ERR	Fail to stop
 */
/*=========================================================================*/
KZSTATUS_t	KzMemsStop(void)
{
	KZSTATUS_t ret = KZ_ERR;
	if( mMemsCB.mfnCbk )
	{
		mMemsCB.mfnCbk = 0;
		kzdev_mems_stop();
		ret = KZ_OK;
	}
	return ret;
}
/*--- Audio Driver ------------------------------*/
/*=========================================================================*/
/**
 * @fn			KZSTATUS_t	KzAudioStart(KzAudioCbk_t cbk,long lSampleRate, int nBlocks, int nChannels )
 * @brief		Audio start
 * @ingroup	API_DeviceDriver
 * @param		cbk User callback function. This function is called in Audio Task(highest priority) every nBlocks.
 * @param		lSampleRate Audio sample rate. now you can set only 48000.
 * @param		nBlocks Audio blocks that callback function has. This must be a multiple of 8(SAMPLES_PER_INTR defined in kzdev_audio.c).
 * @param		nChannels numober of the Audio Channels (1-4)
 * @return		KZ_OK	Success
 * @return		KZ_ERR	Fail to start Audio
 */
/*=========================================================================*/
KZSTATUS_t	KzAudioStart(KzAudioCbk_t cbk,long lSampleRate, int nBlocks, int nChannels )
{
	KZSTATUS_t ret;
	ret = ( kzdev_audio_start( cbk, lSampleRate, nBlocks, nChannels ) )? KZ_OK : KZ_ERR;
	return ret;
}
/*=========================================================================*/
/**
 * @fn			KZSTATUS_t	KzAudioStop(void)
 * @brief		Audio stop
 * @ingroup	API_DeviceDriver
 * @return		KZ_OK	Success (constant)
 */
/*=========================================================================*/
KZSTATUS_t	KzAudioStop(void)
{
	kzdev_audio_stop();
	return KZ_OK;
}

/*--- Driver's Commands ------------------------------*/
/**
 * @fn			static void sw_cbk( KzSW_t nSw, BOOL bPushed )
 * @brief		switch callback function
 * @note		When switch status is changed, this function is called by Device Driver
 *				This function is called under DeviceDriver's Task level
 */
static void sw_cbk( KzSW_t nSw, BOOL bPushed )
{	
	static const struct {
		KzLED_t		led;	KzSW_t		sw;
	} conv_led_sw[] = {
		{	eKzLED0,		eKzSW0	},
		{	eKzLED1,		eKzSW1	},
	};
	KzLED_t led=eKzLED0;
	int i;
	for(i=0;i<numof(conv_led_sw);++i)
		if( conv_led_sw[i].sw == nSw ) { led=conv_led_sw[i].led; break; }
		
	if( bPushed )	KzLedOn( led );
	else			KzLedOff( led );
	
}
/**
 * @fn			static int swled_exe( int argc, char *argv[] )
 * @brief		COMMAND: LED lit where switch is pushed
 * @note		COMMAND: swled
 */
static int swled_exe( int argc, char *argv[] )
{
	
	printf("Start program\n");
	printf("Push KOBANZAME switch. Can you see LED is lit where you switch pushing?\n");
	printf(" -- Hit any key to stop the program. -- \n");
	
	KzSwStart( sw_cbk );
	
	/* hit any key */
	getc(stdin);
	
	KzSwStop();
	
	printf("End program\n");
	return 0;	
}
/**
 * @fn			static int wink_exe( int argc, char *argv[] )
 * @brief		COMMAND: LED wink
 * @note		COMMAND: wink
 */
static int wink_exe( int argc, char *argv[] )
{	
	printf("Start program\n");
	printf("Can you see two LEDs are winking?\n");
	printf(" -- Hit any key to stop the program. -- \n");

	KzLedBlink( eKzLED0, 500, 500 );
	dly_tsk(500);
	KzLedBlink( eKzLED1, 500, 500 );
	
	/* hit any key */
	getc(stdin);
	
	KzLedOff(eKzLED0);
	KzLedOff(eKzLED1);
	
	printf("End program\n");
	
	return 0;
}

/**
 * @fn			static void mems_cbk(int x, int y, int z )
 * @brief		MEMS callback function
 * @param		x MEMS X position
 * @param		y MEMS Y position
 * @param		z MEMS Z position
 * @note		When MEMS status is changed, this function is called by Device Driver
 *				This function is called under DeviceDriver's Task level
 */
static int mems_scan_count = 0;
static void mems_cbk(int x, int y, int z )
{
#define MEMS_SCAN_COUNT		100		/* set for diaplay interval */
	if( ++mems_scan_count >= MEMS_SCAN_COUNT )
	{
		mems_scan_count = 0;
		printf("MEMS OUT: x[%4d] y[%4d] z[%4d]\n",x,y,z );
	}
}
/**
 * @fn			static int mems_exe( int argc, char *argv[] )
 * @brief		COMMAND: Print mems values
 * @note		COMMAND: mems
 */
static int mems_exe( int argc, char *argv[] )
{
	printf("Start program\n");
	
	if( KzMemsStart( mems_cbk) == KZ_ERR )
	{
		printf("MEMS ERROR\n");
		return 0;	
	}
	
	/* hit any key */
	printf(" -- Hit any key to stop the program. -- \n");
	getc(stdin);
	
	KzMemsStop();
	
	printf("End program\n");
	
	return 0;
}
/**
 * @fn			static void audio_cb( int *in, int *out, int nBlocks )
 * @brief		Qudio callback function
 * @param		in Audio Input buffer from Audio CODEC
 * @param		out Audio Output buffer to Audio CODEC
 * @param		nBlocks number of the Audio blocks
 * @param		nChannels number of the Audio Channels per one block
 * @note		This function is called by Audio Task, is every 8 samples defined in echo_exe.
 *				This function is called under Audio Task level (highest priority task)
 */
static void audio_cb( int *in, int *out, int nBlocks, int nChannels )
{
	int i;
	/* echo back */
	for(i=0;i<nBlocks*nChannels;++i)
	{
		*out++ = *in++;	
		
	}
}
/**
 * @fn			static int echo_exe( int argc, char *argv[] )
 * @brief		COMMAND: Audio Echo back
 * @note		COMMAND: echo
 */
static int echo_exe( int argc, char *argv[] )
{
	printf("Start Echo Back\n");

	if( KzAudioStart( audio_cb, 48000, 8, 4 ) != KZ_OK )
	{
		printf("AUDIO ERROR\n");
		return 0;		
	}
	printf("Audio Start: Input0 -> Output0, Input1 -> Output 1\n");
	
	/* hit any key */
	printf(" -- Hit any key to stop the program. -- \n");
	getc(stdin);

	KzAudioStop();
		
	printf("End program\n");
	return 0;
}


/*--- command list ------------------------------*/
static  const KzCmdRegist_t	exes[] = {
	{	swled_exe,			"swled",	"Driver sample : Lit the led where switch pushing"		},
	{	wink_exe,			"wink",		"Driver sample : Wink LEDs"								},
	{	mems_exe,			"mems",		"Driver sample : Printing mems starus"					},
	{	echo_exe,			"echo",		"Driver sample : Audio Echoback "						},
	{	0,					0,			0														}		/* end mark */
};

/*=========================================================================*/
/**
 * @fn			KZSTATUS_t KzAddCmdDeviceDriver(void)
 * @brief		Add DeviceDriver commands
 * @ingroup	API_CommandLine
 * @return		KZ_OK	Success
 * @return		Lack of memory space for the command. See KzCmdlineAddMany
 * @note		After this function call. User can use following commands.
 *				<UL><Li>swled</Li><Li>wink</Li><Li>mems</Li><Li>echo</Li></UL>
 */
/*=========================================================================*/
KZSTATUS_t KzAddCmdDeviceDriver(void)
{
	return KzCmdlineAddMany( exes );
	
}
