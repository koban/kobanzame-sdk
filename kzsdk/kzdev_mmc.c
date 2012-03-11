/*-----------------------------------------------------------------------*/
/* MMCv3/SDv1/SDv2 (in SPI mode) control module  (C)ChaN, 2009           */
/*-----------------------------------------------------------------------*/
/* Only rcvr_spi(), xmit_spi(), disk_timerproc() and some macros are     */
/* platform dependent.                                                   */
/*-----------------------------------------------------------------------*/

#include "diskio.h"
#include "kernel.h"
#include "kzdev.h"

#define SPI_CS				(6)
#define USING_READ_DMA		(0)
#define	USING_WRITE_DMA		(0)
#define SPI_LOW_SPEED		(400000)		/* 400kHz at init */
#define SPI_HIGH_SPEED		(25000000)		/* 25MHz at running */

/* MMC/SD command (in SPI) */
#define CMD0	(0x40+0)	/* GO_IDLE_STATE */
#define CMD1	(0x40+1)	/* SEND_OP_COND */
#define	ACMD41	(0xC0+41)	/* SEND_OP_COND (SDC) */
#define CMD8	(0x40+8)	/* SEND_IF_COND */
#define CMD9	(0x40+9)	/* SEND_CSD */
#define CMD10	(0x40+10)	/* SEND_CID */
#define CMD12	(0x40+12)	/* STOP_TRANSMISSION */
#define ACMD13	(0xC0+13)	/* SD_STATUS (SDC) */
#define CMD16	(0x40+16)	/* SET_BLOCKLEN */
#define CMD17	(0x40+17)	/* READ_SINGLE_BLOCK */
#define CMD18	(0x40+18)	/* READ_MULTIPLE_BLOCK */
#define CMD23	(0x40+23)	/* SET_BLOCK_COUNT */
#define	ACMD23	(0xC0+23)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24	(0x40+24)	/* WRITE_BLOCK */
#define CMD25	(0x40+25)	/* WRITE_MULTIPLE_BLOCK */
#define CMD41	(0x40+41)	/* SEND_OP_COND (ACMD) */
#define CMD55	(0x40+55)	/* APP_CMD */
#define CMD58	(0x40+58)	/* READ_OCR */

#define		SETUP_SPI_CS()		KZ_GPIO_SET_DIR_OUT( SPI_CS )
#define 	DESELECT()			KZ_GPIO_SET_HI( SPI_CS )
#define		SELECT()			KZ_GPIO_SET_LO( SPI_CS )


/*--------------------------------------------------------------------------

   Module Private Functions

---------------------------------------------------------------------------*/

static volatile
DSTATUS Stat = STA_NOINIT;	/* Disk status */

static volatile
BYTE Timer1, Timer2;	/* 100Hz decrement timer */

static
BYTE CardType;			/* b0:MMC, b1:SDv1, b2:SDv2, b3:Block addressing */

static
int mSpiDesc;

/*-----------------------------------------------------------------------*/
/* SPI Speed Set (Platform dependent)                                    */
/*-----------------------------------------------------------------------*/
static void set_spi_spped(BOOL bHigh )
{
	if( bHigh )
		kzdev_spi_chgbps( mSpiDesc, SPI_HIGH_SPEED );
	else
		kzdev_spi_chgbps( mSpiDesc, SPI_LOW_SPEED );
}

/*-----------------------------------------------------------------------*/
/* SPI Lock/Unlock (Platform dependent)                                  */
/*-----------------------------------------------------------------------*/
static void spi_lock(void)
{
	kzdev_spi_open();
}
static void spi_unlock(void)
{
	kzdev_spi_close();
}

/*-----------------------------------------------------------------------*/
/* Transmit a byte to MMC via SPI  (Platform dependent)                  */
/*-----------------------------------------------------------------------*/

static
void xmit_spi (BYTE dat)
{
	kzdev_spi_write( mSpiDesc, &dat, 1 );
}

static
void xmit_spi_blk( const BYTE *dat, int n)
{
#if USING_WRITE_DMA == 1
	kzdev_spi_dma_write( mSpiDesc, dat, n );
#else
	kzdev_spi_write( mSpiDesc, dat, n );
#endif
}

/*-----------------------------------------------------------------------*/
/* Receive a byte from MMC via SPI (Platform dependent)                  */
/*-----------------------------------------------------------------------*/

static
BYTE rcvr_spi (void)
{
	BYTE r = 0, tx = 0xFF;

	kzdev_spi_readwrite( mSpiDesc, &tx, 1, &r,  1 );
	
	return r;
}

static
void rcvr_spi_blk( BYTE *dat, int n)
{
#if USING_READ_DMA == 1
	kzdev_spi_dma_read( mSpiDesc, dat, n );
#else
	kzdev_spi_read( mSpiDesc, dat, n );
#endif
}



/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                   */
/*-----------------------------------------------------------------------*/

static
BYTE wait_ready (void)
{
	BYTE res;


	Timer2 = 50;			/* Wait for ready in timeout of 500ms */
	rcvr_spi();
	do
		res = rcvr_spi();
	while ((res != 0xFF) && Timer2);
	return res;
}


/*-----------------------------------------------------------------------*/
/* Deselect the card and release SPI bus                                 */
/*-----------------------------------------------------------------------*/

static
void release_spi (void)
{
	DESELECT();
	rcvr_spi();
}



/*-----------------------------------------------------------------------*/
/* Power Control  (Platform dependent)                                   */
/*-----------------------------------------------------------------------*/
/* When the target system does not support socket power control, there   */
/* is nothing to do in these functions and chk_power always returns 1.   */

static
void power_on (void)
{
}


static
void power_off (void)
{
	SELECT();				/* Wait for card ready */
	wait_ready();
	DESELECT();
	rcvr_spi();

	Stat |= STA_NOINIT;		/* Set STA_NOINIT */
}



/*-----------------------------------------------------------------------*/
/* Receive a data packet from MMC                                        */
/*-----------------------------------------------------------------------*/

static
BOOL rcvr_datablock (
	BYTE *buff,			/* Data buffer to store received data */
	UINT btr			/* Byte count (must be even number) */
)
{
	BYTE token;


	Timer1 = 10;
	do {							/* Wait for data packet in timeout of 100ms */
		token = rcvr_spi();
	} while ((token == 0xFF) && Timer1);
	if(token != 0xFE) return FALSE;	/* If not valid data token, retutn with error */

#if 0
	do {							/* Receive the data block into buffer */
		*buff++ = rcvr_spi();
		*buff++ = rcvr_spi();
	} while (btr -= 2);
#else
	rcvr_spi_blk( buff, 512 );
#endif
	rcvr_spi();						/* Discard CRC */
	rcvr_spi();

	return TRUE;					/* Return with success */
}



/*-----------------------------------------------------------------------*/
/* Send a data packet to MMC                                             */
/*-----------------------------------------------------------------------*/

#if _READONLY == 0
static
BOOL xmit_datablock (
	const BYTE *buff,	/* 512 byte data block to be transmitted */
	BYTE token			/* Data/Stop token */
)
{
	BYTE resp, wc;


	if (wait_ready() != 0xFF) return FALSE;

	xmit_spi(token);					/* Xmit data token */
	if (token != 0xFD) {	/* Is data token */
#if 0
		wc = 0;
		do {							/* Xmit the 512 byte data block to MMC */
			xmit_spi(*buff++);
			xmit_spi(*buff++);
		} while (--wc);
#else
		xmit_spi_blk(buff,512);
#endif
		xmit_spi(0xFF);					/* CRC (Dummy) */
		xmit_spi(0xFF);
		resp = rcvr_spi();				/* Reveive data response */
		if ((resp & 0x1F) != 0x05)		/* If not accepted, return with error */
			return FALSE;
	}

	return TRUE;
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Send a command packet to MMC                                          */
/*-----------------------------------------------------------------------*/

static
BYTE send_cmd (
	BYTE cmd,		/* Command byte */
	DWORD arg		/* Argument */
)
{
	BYTE n, res;


	if (cmd & 0x80) {	/* ACMD<n> is the command sequense of CMD55-CMD<n> */
		cmd &= 0x7F;
		res = send_cmd(CMD55, 0);
		if (res > 1) return res;
	}

	/* Select the card and wait for ready */
	DESELECT();
	rcvr_spi();
	SELECT();
	if (wait_ready() != 0xFF) return 0xFF;

	/* Send command packet */
	xmit_spi(cmd);						/* Start + Command index */
	xmit_spi((BYTE)(arg >> 24));		/* Argument[31..24] */
	xmit_spi((BYTE)(arg >> 16));		/* Argument[23..16] */
	xmit_spi((BYTE)(arg >> 8));			/* Argument[15..8] */
	xmit_spi((BYTE)arg);				/* Argument[7..0] */
	n = 0x01;							/* Dummy CRC + Stop */
	if (cmd == CMD0) n = 0x95;			/* Valid CRC for CMD0(0) */
	if (cmd == CMD8) n = 0x87;			/* Valid CRC for CMD8(0x1AA) */
	xmit_spi(n);

	/* Receive command response */
	if (cmd == CMD12) rcvr_spi();		/* Skip a stuff byte when stop reading */
	n = 10;								/* Wait for a valid response in timeout of 10 attempts */
	do
		res = rcvr_spi();
	while ((res & 0x80) && --n);

	return res;			/* Return with the response value */
}



/*--------------------------------------------------------------------------

   Public Functions

---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE drv		/* Physical drive nmuber (0) */
)
{
	BYTE n, ty, cmd, ocr[4];


	if (drv) return STA_NOINIT;			/* Supports only single drive */
	if (Stat & STA_NODISK) return Stat;	/* No card in the socket */

	spi_lock();
	power_on();							/* Force socket power on */
	set_spi_spped(FALSE);				/* SPI Low Speed */
	for (n = 10; n; n--) rcvr_spi();	/* 80 dummy clocks */

	ty = 0;
	if (send_cmd(CMD0, 0) == 1) {			/* Enter Idle state */
		Timer1 = 100;						/* Initialization timeout of 1000 msec */
		if (send_cmd(CMD8, 0x1AA) == 1) {	/* SDHC */
			for (n = 0; n < 4; n++) ocr[n] = rcvr_spi();		/* Get trailing return value of R7 resp */
			if (ocr[2] == 0x01 && ocr[3] == 0xAA) {				/* The card can work at vdd range of 2.7-3.6V */
				while (Timer1 && send_cmd(ACMD41, 1UL << 30));	/* Wait for leaving idle state (ACMD41 with HCS bit) */
				if (Timer1 && send_cmd(CMD58, 0) == 0) {		/* Check CCS bit in the OCR */
					for (n = 0; n < 4; n++) ocr[n] = rcvr_spi();
					ty = (ocr[0] & 0x40) ? 12 : 4;
				}
			}
		} else {							/* SDSC or MMC */
			if (send_cmd(ACMD41, 0) <= 1) 	{
				ty = 2; cmd = ACMD41;	/* SDSC */
			} else {
				ty = 1; cmd = CMD1;		/* MMC */
			}
			while (Timer1 && send_cmd(cmd, 0));			/* Wait for leaving idle state */
			if (!Timer1 || send_cmd(CMD16, 512) != 0)	/* Set R/W block length to 512 */
				ty = 0;
		}
	}
	CardType = ty;
	release_spi();

	if (ty) {			/* Initialization succeded */
		Stat &= ~STA_NOINIT;		/* Clear STA_NOINIT */
		set_spi_spped(TRUE);		/* SPI High Speed */
	} else {			/* Initialization failed */
		power_off();
	}
	spi_unlock();
	return Stat;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE drv			/* Drive number (0) */
)
{
	return (drv) ? STA_NODISK : Stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE drv,			/* Physical drive nmuber (0) */
	BYTE *buff,			/* Pointer to the data buffer to store read data */
	DWORD sector,		/* Start sector number (LBA) */
	BYTE count			/* Sector count (1..255) */
)
{
	if (drv || !count) return RES_PARERR;
	if (Stat & STA_NOINIT) return RES_NOTRDY;

	if (!(CardType & 8)) sector *= 512;	/* Convert to byte address if needed */

	spi_lock();
	if (count == 1) {	/* Single block read */
		if ((send_cmd(CMD17, sector) == 0)	/* READ_SINGLE_BLOCK */
			&& rcvr_datablock(buff, 512))
			count = 0;
	}
	else {				/* Multiple block read */
		if (send_cmd(CMD18, sector) == 0) {	/* READ_MULTIPLE_BLOCK */
			do {
				if (!rcvr_datablock(buff, 512)) break;
				buff += 512;
			} while (--count);
			send_cmd(CMD12, 0);				/* STOP_TRANSMISSION */
		}
	}
	release_spi();
	spi_unlock();

	return count ? RES_ERROR : RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0) */
	const BYTE *buff,	/* Pointer to the data to be written */
	DWORD sector,		/* Start sector number (LBA) */
	BYTE count			/* Sector count (1..255) */
)
{
	if (drv || !count) return RES_PARERR;
	if (Stat & STA_NOINIT) return RES_NOTRDY;
	if (Stat & STA_PROTECT) return RES_WRPRT;

	if (!(CardType & 8)) sector *= 512;	/* Convert to byte address if needed */

	spi_lock();
	if (count == 1) {	/* Single block write */
		if ((send_cmd(CMD24, sector) == 0)	/* WRITE_BLOCK */
			&& xmit_datablock(buff, 0xFE))
			count = 0;
	}
	else {				/* Multiple block write */
		if (CardType & 6) send_cmd(ACMD23, count);
		if (send_cmd(CMD25, sector) == 0) {	/* WRITE_MULTIPLE_BLOCK */
			do {
				if (!xmit_datablock(buff, 0xFC)) break;
				buff += 512;
			} while (--count);
			if (!xmit_datablock(0, 0xFD))	/* STOP_TRAN token */
				count = 1;
		}
	}
	release_spi();
	spi_unlock();
	
	return count ? RES_ERROR : RES_OK;
}
#endif /* _READONLY == 0 */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	BYTE n, csd[16], *ptr = buff;
	WORD csize;


	if (drv) return RES_PARERR;
	if (Stat & STA_NOINIT) return RES_NOTRDY;

	spi_lock();
	res = RES_ERROR;
	switch (ctrl) {
		case CTRL_SYNC :		/* Make sure that no pending write process */
			SELECT();
			if (wait_ready() == 0xFF)
				res = RES_OK;
			break;

		case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
			if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) {
				if ((csd[0] >> 6) == 1) {	/* SDC ver 2.00 */
					csize = csd[9] + ((WORD)csd[8] << 8) + 1;
					*(DWORD*)buff = (DWORD)csize << 10;
				} else {					/* SDC ver 1.XX or MMC */
					n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
					csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
					*(DWORD*)buff = (DWORD)csize << (n - 9);
				}
				res = RES_OK;
			}
			break;

		case GET_SECTOR_SIZE :	/* Get R/W sector size (WORD) */
			*(WORD*)buff = 512;
			res = RES_OK;
			break;

		case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
			if (CardType & 4) {			/* SDC ver 2.00 */
				if (send_cmd(ACMD13, 0) == 0) {		/* Read SD status */
					rcvr_spi();
					if (rcvr_datablock(csd, 16)) {				/* Read partial block */
						for (n = 64 - 16; n; n--) rcvr_spi();	/* Purge trailing data */
						*(DWORD*)buff = 16UL << (csd[10] >> 4);
						res = RES_OK;
					}
				}
			} else {					/* SDC ver 1.XX or MMC */
				if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) {	/* Read CSD */
					if (CardType & 2) {			/* SDC ver 1.XX */
						*(DWORD*)buff = (((csd[10] & 63) << 1) + ((WORD)(csd[11] & 128) >> 7) + 1) << ((csd[13] >> 6) - 1);
					} else {					/* MMC */
						*(DWORD*)buff = ((WORD)((csd[10] & 124) >> 2) + 1) * (((csd[11] & 3) << 3) + ((csd[11] & 224) >> 5) + 1);
					}
					res = RES_OK;
				}
			}
			break;

		case MMC_GET_TYPE :		/* Get card type flags (1 byte) */
			*ptr = CardType;
			res = RES_OK;
			break;

		case MMC_GET_CSD :		/* Receive CSD as a data block (16 bytes) */
			if (send_cmd(CMD9, 0) == 0		/* READ_CSD */
				&& rcvr_datablock(ptr, 16))
				res = RES_OK;
			break;

		case MMC_GET_CID :		/* Receive CID as a data block (16 bytes) */
			if (send_cmd(CMD10, 0) == 0		/* READ_CID */
				&& rcvr_datablock(ptr, 16))
				res = RES_OK;
			break;

		case MMC_GET_OCR :		/* Receive OCR as an R3 resp (4 bytes) */
			if (send_cmd(CMD58, 0) == 0) {	/* READ_OCR */
				for (n = 4; n; n--) *ptr++ = rcvr_spi();
				res = RES_OK;
			}
			break;

		case MMC_GET_SDSTAT :	/* Receive SD statsu as a data block (64 bytes) */
			if (send_cmd(ACMD13, 0) == 0) {	/* SD_STATUS */
				rcvr_spi();
				if (rcvr_datablock(ptr, 64))
					res = RES_OK;
			}
			break;

		default:
			res = RES_PARERR;
	}

	release_spi();
	spi_unlock();

	return res;
}



/*-----------------------------------------------------------------------*/
/* Device timer interrupt procedure  (Platform dependent)                */
/*-----------------------------------------------------------------------*/
/* This function must be called in period of 10ms                        */

void disk_timerproc (void)
{
	static BYTE pv;
	BYTE n, s;


	n = Timer1;						/* 100Hz decrement timer */
	if (n) Timer1 = --n;
	n = Timer2;
	if (n) Timer2 = --n;


#if 0
	n = pv;
	pv = SOCKPORT & (SOCKWP | SOCKINS);	/* Sapmle socket switch */

	if (n == pv) {					/* Have contacts stabled? */
		s = Stat;

		if (pv & SOCKWP)			/* WP is H (write protected) */
			s |= STA_PROTECT;
		else						/* WP is L (write enabled) */
			s &= ~STA_PROTECT;

		if (pv & SOCKINS)			/* INS = H (Socket empty) */
			s |= (STA_NODISK | STA_NOINIT);
		else						/* INS = L (Card inserted) */
			s &= ~STA_NODISK;

		Stat = s;
	}
#endif
}

void kzdev_mmc_att_ini(void)
{
	/*	SPI Setting
		BitLength:  8bit
		BitRatio :	400kHz
		ChipSel  :	-1 (None)
		bCPOL	 :	None
		bCPHA	 :	None
	
	*/
	mSpiDesc = kzdev_spi_regist( KZDEV_SPI_8BIT, 400000, -1, TRUE, TRUE );
	
	SETUP_SPI_CS();
	DESELECT();
}

/**
 * @fn		int KzMMCGetCardType(void)
 * @brief	Get SDCARD Type
 * @return	0:MMC 1:SDv1 2:SDv2 3:SDHC
 * @note	KOBANZAME SDK private function
 */
int KzMMCGetCardType(void)
{
	if( CardType & 0x8 ) return 3;
	else if( CardType & 0x4 ) return 2;
	else if( CardType & 0x2 ) return 1;
	else return 0;	
}




