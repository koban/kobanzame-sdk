/**
 *	@file	kzdev_codec.c
 *	@brief	KOBANZAME SDK Audio Codec Driver
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

#include "kobanzame.h"

#define DAC_CONTROL_1 		0x0000
#define DAC_CONTROL_2  		0x1000
#define DAC_VOLUME_0  		0x2000
#define DAC_VOLUME_1  		0x3000
#define DAC_VOLUME_2  		0x4000
#define DAC_VOLUME_3  		0x5000
#define DAC_VOLUME_4  		0x6000
#define DAC_VOLUME_5  		0x7000
#define ADC_0_PEAK_LEVEL  	0x8000
#define ADC_1_PEAK_LEVEL  	0x9000
#define ADC_2_PEAK_LEVEL  	0xA000
#define ADC_3_PEAK_LEVEL  	0xB000
#define ADC_CONTROL_1  		0xC000
#define ADC_CONTROL_2  		0xD000
#define ADC_CONTROL_3  		0xE000

#define TIMOD_DMA_TX		0x0003

static int					mnSpiId;

void kzdev_codec_att_ini(void)
{
	mnSpiId = kzdev_spi_regist( KZDEV_SPI_16BIT, 100000,4, FALSE, FALSE );	
}


void kzdev_codec_start_DSPMODE(long lSampleRate)
{
	int i;
	const UH CodecReg[] = {
		DAC_CONTROL_1	| 0x000,
		DAC_CONTROL_2	| 0x000,
		DAC_VOLUME_0	| 0x3ff,
		DAC_VOLUME_1	| 0x3ff,
		DAC_VOLUME_2	| 0x3ff,
		DAC_VOLUME_3	| 0x3ff,
		DAC_VOLUME_4	| 0x3ff,
		DAC_VOLUME_5	| 0x3ff,
		ADC_CONTROL_1	| 0x000,
		ADC_CONTROL_2	| 0x180,
		ADC_CONTROL_3	| 0x000
	};

	kzdev_spi_open();
	for(i=0;i<numof(CodecReg);++i)
	{
		kzdev_spi_write( mnSpiId, &CodecReg[i]	, 1 );
		dly_tsk(1);
	}
	//kzdev_spi_write( mnSpiId, CodecReg, numof(CodecReg) );
	kzdev_spi_close();
	
	/* wait for pll sutable */
	dly_tsk(500);
			
}

void kzdev_codec_start_I2CMODE(long lSampleRate)
{
	const UH CodecReg[] = {
		DAC_CONTROL_1	| 0x000,
		DAC_CONTROL_2	| 0x000,
		DAC_VOLUME_0	| 0x3ff,
		DAC_VOLUME_1	| 0x3ff,
		DAC_VOLUME_2	| 0x3ff,
		DAC_VOLUME_3	| 0x3ff,
		DAC_VOLUME_4	| 0x000,
		DAC_VOLUME_5	| 0x000,
		ADC_CONTROL_1	| 0x000,
		ADC_CONTROL_2	| 0x000,
		ADC_CONTROL_3	| 0x000
	};

	kzdev_spi_open();
	kzdev_spi_write( mnSpiId, CodecReg, numof(CodecReg) );
	kzdev_spi_close();
	
	/* wait for pll sutable */
	dly_tsk(500);
}

void kzdev_codec_stop(void)
{
	static const UH CodecReg[] = {
		DAC_CONTROL_1	| 0x004,
		ADC_CONTROL_1	| 0x080,
	};

	kzdev_spi_open();
	kzdev_spi_write( mnSpiId, CodecReg, numof(CodecReg) );
	kzdev_spi_close();		
	
}
