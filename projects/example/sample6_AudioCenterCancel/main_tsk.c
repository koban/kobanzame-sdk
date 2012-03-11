/**
 *	@file	main_task.c
 *	@brief	KOBANZAME SDK Sample Task
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

 /*
  *	SAMPLE CODE: Center cancel
  */
 
 /*--- include -----------------------------------*/
#include "kobanzame.h"

/**
 * @fn		static void audio_cb( int *in, int *out, int nBlocks , int nChannels )
 * @brief	Audio Driver Callback
 * @param	in audio input buffer
 * @param	out audio output buffer
 * @param	nBlocks is the number of the audio blocks
 * @param	nChannels is the number of the audio channels in one block.
 */
static void audio_cb( int *in, int *out, int nBlocks, int nChannels )
{
	int i,j;
	/* Center Cancel(Stereo) */
	for(i=0;i<nBlocks;++i)
	{
		
		/* load from input buffer */
		int channel_L0 = *in++;
		int channel_R0 = *in++;
		int channel_L1 = *in++;
		int channel_R1 = *in++;
		
		/* output = L - R */
		int output0 = KzDspSub( channel_L0, channel_R0 );
		int output1 = KzDspSub( channel_L1, channel_R1 );
		
		/* save to output buffer */
		*out++ = output0;
		*out++ = output0;
		*out++ = output1;
		*out++ = output1;
	}
}


/*=========================================================================*/
/**
 * @fn		void main_task(VP_INT exinf)
 * @brief	Main Task
 * @param	extinf from uITRON Configurator
 */
/*=========================================================================*/
void main_task(VP_INT exinf)
{
	/* Audio Driver start as interrupt interval is 8 blocks, 4 channels*/
	 KzAudioStart( audio_cb, 48000, 8, 4 );
	 
	/* main_task exit */
	ext_tsk();
}
