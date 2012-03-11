/**
 *	@file	def_mma7455l.h
 *	@brief	Freescale MMA7455L MEMS Define
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
 *	@note	MMA7455L(freescale MEMS) register define
 */

#ifndef _def_mma7455l_h_
#define _def_mma7455l_h_

//--- REGISTER DEFINE -----------------------------------------------
#define		REG_MMA7455L_XOUTL				(0x00)
#define		REG_MMA7455L_XOUTH				(0x01)
#define		REG_MMA7455L_YOUTL				(0x02)
#define		REG_MMA7455L_YOUTH				(0x03)
#define		REG_MMA7455L_ZOUTL				(0x04)
#define		REG_MMA7455L_ZOUTH				(0x05)
#define		REG_MMA7455L_XOUT8				(0x06)
#define		REG_MMA7455L_YOUT8				(0x07)
#define		REG_MMA7455L_ZOUT8				(0x08)
#define		REG_MMA7455L_STATUS				(0x09)
#define		REG_MMA7455L_DETSRC				(0x0A)
#define		REG_MMA7455L_TOUT				(0x0B)
#define		REG_MMA7455L_RESERVED0			(0x0C)
#define		REG_MMA7455L_I2CAD				(0x0D)
#define		REG_MMA7455L_USRINF				(0x0E)
#define		REG_MMA7455L_WHOAMI				(0x0F)
#define		REG_MMA7455L_XOFFL				(0x10)
#define		REG_MMA7455L_XOFFH				(0x11)
#define		REG_MMA7455L_YOFFL				(0x12)
#define		REG_MMA7455L_YOFFH				(0x13)
#define		REG_MMA7455L_ZOFFL				(0x14)
#define		REG_MMA7455L_ZOFFH				(0x15)
#define		REG_MMA7455L_MCTL				(0x16)
#define		REG_MMA7455L_INTRST				(0x17)
#define		REG_MMA7455L_CTL1				(0x18)
#define		REG_MMA7455L_CTL2				(0x19)
#define		REG_MMA7455L_LDTH				(0x1A)
#define		REG_MMA7455L_PDTH				(0x1B)
#define		REG_MMA7455L_PW					(0x1C)
#define		REG_MMA7455L_LT					(0x1D)
#define		REG_MMA7455L_TW					(0x1E)
#define		REG_MMA7455L_RESERVED1			(0x1F)

//	REG_MMA7455L_STATUS		(0x09) BIT DEF
#define		BITDEF_MMA7455L_DRDY			(1<<0)
#define		BITDEF_MMA7455L_DOVR			(1<<1)
#define		BITDEF_MMA7455L_PERR			(1<<2)

// REG_MMA7455L_DETSRC		(0x0A) BIT DEF
#define		BITDEF_MMA7455L_INT1			(1<<0)
#define		BITDEF_MMA7455L_INT2			(1<<1)
#define		BITDEF_MMA7455L_PDZ				(1<<2)
#define		BITDEF_MMA7455L_PDY				(1<<3)
#define		BITDEF_MMA7455L_PDX				(1<<4)
#define		BITDEF_MMA7455L_LDZ				(1<<5)
#define		BITDEF_MMA7455L_LDY				(1<<6)
#define		BITDEF_MMA7455L_LDX				(1<<7)

// REG_MMA7455L_I2CAD		(0x0D) BIT DEF
#define		BITDEF_MMA7455L_I2CDIS		(1<<7)

// REG_MMA7455L_MCTL		(0x16) BIT DEF
#define		BITMASK_MMA7455L_MODE			(0x3<<0)
#define		BITATTR_MMA7455L_STANBY_MODE	(0<<0)
#define		BITATTR_MMA7455L_MEASURE_MODE	(1<<0)
#define		BITATTR_MMA7455L_LEVEL_MODE		(2<<0)
#define		BITATTR_MMA7455L_PULSE_MODE		(3<<0)

#define		BITMASK_MMA7455L_GLVL			(0x3<<2)
#define		BITATTR_MMA7455L_8G				(0<<2)
#define		BITATTR_MMA7455L_4G				(1<<2)
#define		BITATTR_MMA7455L_2G				(2<<2)

#define		BITDEF_MMA7455L_STON			(1<<4)
#define		BITDEF_MMA7455L_SPI3W			(1<<5)
#define		BITDEF_MMA7455L_DRPD			(1<<6)

// REG_MMA7455L_INTRST		(0x17) BIT DEF
#define		BITDEF_MMA7455L_CLR_INT1		(1<<0)
#define		BITDEF_MMA7455L_CLR_INT2		(1<<1)

// REG_MMA7455L_CTL1		(0x18) BIT DEF
#define		BITDEF_MMA7455L_INTPIN			(1<<0)
#define		BITDEF_MMA7455L_INTREG0			(1<<1)
#define		BITDEF_MMA7455L_INTREG1			(1<<2)
#define		BITDEF_MMA7455L_XDA				(1<<3)
#define		BITDEF_MMA7455L_YDA				(1<<4)
#define		BITDEF_MMA7455L_ZDA				(1<<5)
#define		BITDEF_MMA7455L_THOPT			(1<<6)
#define		BITDEF_MMA7455L_DFBW			(1<<7)

// REG_MMA7455L_CTL2		(0x19) BIT DEF
#define		BITDEF_MMA7455L_LDPL			(1<<0)
#define		BITDEF_MMA7455L_PDPL			(1<<1)
#define		BITDEF_MMA7455L_DRVO			(1<<2)


#endif	// !_def_mma7455l_h_
