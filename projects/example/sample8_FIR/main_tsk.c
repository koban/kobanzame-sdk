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

#define FIR_TAPS		(81)

/*
	SAMPLE CODE: FIR Filter

	in --- +---|>---+---- out
	  d[0] o  c[0]  |
		   +---|>---+
	  d[1] o  c[1]  |
		   +---|>---+
	  d[2] o  c[2]  |
		   :    :   :
		   +---|>---+
		      c[80]
                 
The coeffs are get by following web based program.
http://momiji.i.ishikawa-nct.ac.jp/index_eng.shtml   

Filter Length : N = 81
Amplitude Response : LPF
- Band 1 -
Lower Edge Normalized Frequency : 0
Upper Edge Normalized Frequency : 0.2
Desired Amplitude : 1
Weights : 1
- Band 2 -
Lower Edge Normalized Frequency : 0.3
Upper Edge Normalized Frequency : 0.5
Desired Amplitude : 0
Weights : 1
		     
*/

/* FIR filter coeff define */
const float fir_coeffs[] = {
-5.331473963270603e-09 ,
-2.042692003733233e-06 ,
2.756161908989151e-08 ,
8.996986181393548e-06 ,
-8.616966386933824e-08 ,
-2.763988939538630e-05 ,
2.143332719180457e-07 ,
6.969419870501895e-05 ,
-4.593635593211731e-07 ,
-1.541998749377117e-04 ,
8.841875352555360e-07 ,
3.099199248300343e-04 ,
-1.563415559119387e-06 ,
-5.778558150565794e-04 ,
2.579440161987601e-06 ,
1.013727892695423e-03 ,
-4.011301202425065e-06 ,
-1.690441833606298e-03 ,
5.924659705016354e-06 ,
2.700897318076931e-03 ,
-8.356741457664986e-06 ,
-4.162133856046728e-03 ,
1.130450836599544e-05 ,
6.223055491295892e-03 ,
-1.471484093460868e-05 ,
-9.080493538638578e-03 ,
1.847787059314381e-05 ,
1.301409304309030e-02 ,
-2.243093109169198e-05 ,
-1.846520045154258e-02 ,
2.636574361956106e-05 ,
2.622863914940302e-02 ,
-3.004583230929537e-05 ,
-3.798206365618477e-02 ,
3.322801107708681e-05 ,
5.809431990895472e-02 ,
-3.568780342038881e-05 ,
-1.026722032888311e-01 ,
3.724298858040932e-05 ,
3.171507274414941e-01 ,
4.999622248522956e-01 ,
3.171507274414941e-01 ,
3.724298858040932e-05 ,
-1.026722032888311e-01 ,
-3.568780342038881e-05 ,
5.809431990895472e-02 ,
3.322801107708681e-05 ,
-3.798206365618477e-02 ,
-3.004583230929537e-05 ,
2.622863914940302e-02 ,
2.636574361956106e-05 ,
-1.846520045154258e-02 ,
-2.243093109169198e-05 ,
1.301409304309030e-02 ,
1.847787059314381e-05 ,
-9.080493538638578e-03 ,
-1.471484093460868e-05 ,
6.223055491295892e-03 ,
1.130450836599544e-05 ,
-4.162133856046728e-03 ,
-8.356741457664986e-06 ,
2.700897318076931e-03 ,
5.924659705016354e-06 ,
-1.690441833606298e-03 ,
-4.011301202425065e-06 ,
1.013727892695423e-03 ,
2.579440161987601e-06 ,
-5.778558150565794e-04 ,
-1.563415559119387e-06 ,
3.099199248300343e-04 ,
8.841875352555360e-07 ,
-1.541998749377117e-04 ,
-4.593635593211731e-07 ,
6.969419870501895e-05 ,
2.143332719180457e-07 ,
-2.763988939538630e-05 ,
-8.616966386933824e-08 ,
8.996986181393548e-06 ,
2.756161908989151e-08 ,
-2.042692003733233e-06 ,
-5.331473963270603e-09 ,
};

/* FIR coeff buffer */
L1DATA_B static int coeff[FIR_TAPS];

/* FIR Filter class */
typedef struct {
	int		d[FIR_TAPS*2];
	int		wp;
} SFIR;

/* Running FIR proccess */
int fir_process( SFIR *obj, int in)
{
	int out;
	int wp = obj->wp;
	obj->d[wp] = in;
	obj->d[wp + FIR_TAPS ] = in;
	out = KzDspMac( &obj->d[ wp ], coeff, FIR_TAPS );
	obj->wp = (wp==0)? FIR_TAPS-1 : wp-1;
	return out;
}
/* Initialzie FIR object */
void fir_init( SFIR *obj )
{
	int i;
	obj->wp = 0;
	for(i=0;i<numof(obj->d);++i)
		obj->d[i] = 0;
	for(i=0;i<numof(coeff);++i)
		coeff[i] = KzDspToi( fir_coeffs[i] );
}
/* FIR Objecct define */
L1DATA_A
static SFIR fir[4];

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
	for(j=0;j<nBlocks;++j)
	{
		for(i=0;i<nChannels;++i)
			*out++ = fir_process( &fir[i], *in++ );	
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
	
	/* Initialize FIR Objects */
	for( i=0;i<numof(fir);++i)
		fir_init( &fir[i] );

	/* Audio Driver start as interrupt interval is 8 blocks, 4 channels*/
	 KzAudioStart( audio_cb, 48000, 8, 4 );
	 
	/* main_task exit */
	ext_tsk();
}
