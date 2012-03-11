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

 /*--- include -----------------------------------*/
#include "kobanzame.h"

/*
	SAMPLE CODE ; IIR Filter

	in ---|>---+------+--|>--+----- out
		  c[0] |      | c[3] |
		       |      o d[0] |
		       |      |      |
		       +--<|--+--|>--+
		       | c[1] | c[4] |
		       |      o d[1] |
		       |      |      |
		       +--<|--+--|>--+
                 c[2]   c[5]
                 

The coeffs are get by following web based program.
http://momiji.i.ishikawa-nct.ac.jp/index_eng.shtml   
              
Amplitude Response : LPF
Approximation : Chebyshev
Degree of Filter : 2
Number of Sections : 1
Sampling Frequency : 48000 [Hz]
Pass Band Edge Frequency : 1000 [Hz]
Stop Band Edge Frequency : 10000 [Hz]
Pass Band Edge Attenuation : 0.5 [dB]
Stop Band Edge Attenuation : 35 [dB]

c[0] = 2.14568284147626243e-01
c[1] = -1.80641476718095118e+00
c[2] = 8.30101301431402816e-01
c[3] = 2.60540955172957403e-02
c[4] = 5.21081910345914806e-02
c[5] = 2.60540955172957403e-02

*/

/* IIR filter coeff define */
const float iir_coeffs[] = {
2.14568284147626243e-01,
-1.80641476718095118e+00,
8.30101301431402816e-01,
2.60540955172957403e-02,
5.21081910345914806e-02,
2.60540955172957403e-02,
};

/* IIR Filter class */
typedef struct {
	int		c[6];
	int		d[2];
} SIIR;

/* Running IIR proccess */
int iir_process( SIIR *obj, int in)
{
	int tmp, out;
	int *pdly = obj->d;
	int *pcoeff = obj->c;
	int dly[3];
	dly[0] = in;
	dly[1] = pdly[0];
	dly[2] = pdly[1];
	tmp = KzDspMac( dly, pcoeff, 3 );
	dly[0] = tmp; pcoeff+=3;
	out = KzDspMac( dly, pcoeff, 3 );	
	pdly[1] = pdly[0];
	pdly[0] = tmp;
	return out;
}
/* Initialzie IIR object */
void iir_init( SIIR *obj )
{
	int i;
	for(i=0;i<numof(obj->c);++i)
	{
		if( i==1 || i==2 )
			obj->c[i] = KzDspToi(-iir_coeffs[i]);
		else
			obj->c[i] = KzDspToi(iir_coeffs[i]);
	}	
	obj->d[0] = 0;
	obj->d[1] = 0;
}

/* IIR Objecct define */
static SIIR iir[4];

/**
 * @fn		static void audio_cb( int *in, int *out, int nBlocks , int nChannels )
 * @brief	Audio Driver Callback
 * @param	in audio input buffer
 * @param	out audio output buffer
 * @param	nBlocks is the number of the audio blocks
 * @param	nChannels is the number of the audio channels in one block.
 */
static void audio_cb( int *in, int *out, int nBlocks ,int nChannels)
{
	int i,j;
	for(j=0;j<nBlocks;++j)
	{
		/* processing IIR filter */
		for(i=0;i<nChannels;++i)
		{
			*out++ = iir_process( &iir[i], *in++ );	
		}
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
	int i;
	
	/* Initialize IIR Objects */
	for( i=0;i<numof(iir);++i)
		iir_init( &iir[i] );
		
	/* Audio Driver start as interrupt interval is 8 blocks, 4 channels*/
	 KzAudioStart( audio_cb, 48000, 8, 4 );
	 
	/* main_task exit */
	ext_tsk();

}
