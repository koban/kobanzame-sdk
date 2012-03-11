///////////////////////////////////////////////////////////////
//
// BF533EzFlash.c
//
// Analog Devices, Inc. - 2001
//
// Rev - 1.00.2
//
// Change Log
//
//		1.00.2
//			- removed inline assembly code because it was not
//			  compliler safe
//
//		1.00.1
//			- made changes so that the driver will work with
//			  the revised GUI
//
//		1.00.0
//			- initial release
//
// VisualDSP++ "Flash Programmer" flash driver for use with the
// ADSP-BF533 EZ-KIT Lite containing the STMicroelectronics PSD4256G
// flash device.
//
///////////////////////////////////////////////////////////////


// error enum
#include <stdlib.h>
#include "Errors.h"
#include <cdefbf532.h>

// #defines
#define TRUE			0x1
#define FALSE			0x0
#define BUFFER_SIZE		0x600
#define	NUM_SECTORS 	32				// number of sectors in the flash device

#define SECTOR_SIZE		0x10000

// structure for flash sector information
typedef struct _SECTORLOCATION
{
	long lStartOff;
	long lEndOff;
}
SECTORLOCATION;


// Flash Programmer commands
typedef enum
{
	NO_COMMAND,		// 0
	GET_CODES,		// 1
	RESET,			// 2
	WRITE,			// 3
	FILL,			// 4
	ERASE_ALL,		// 5
	ERASE_SECT,		// 6
	READ,			// 7
	GET_SECTNUM,	// 8
	GET_SECSTARTEND	// 9
} enProgCmds;

// M24P40 Instruction Codes.
typedef enum
{
	WREN = 0x06,
	WRDI = 0x04,
	RDID = 0x9f,
	RDSR = 0x05,
	WRSR = 0x01,
	READD = 0x03,
	FAST_READ = 0x0b,
	PP = 0x02,
	SE = 0xd8,
	BE = 0xc7
} InstructionCode;


// function prototypes
ERROR_CODE SetupForFlash();
ERROR_CODE GetCodes();
ERROR_CODE PollToggleBit(unsigned long ulOffset);
ERROR_CODE ResetFlash();
ERROR_CODE EraseFlash();
ERROR_CODE EraseBlock( int nBlock );
ERROR_CODE UnlockFlash(unsigned long ulOffset);
ERROR_CODE WriteData(unsigned long ulStart, long lCount, long lStride, int *pnData);
ERROR_CODE FillData(unsigned long ulStart, long lCount, long lStride, int *pnData);
ERROR_CODE ReadData(unsigned long ulStart, long lCount, long lStride, int *pnData);
ERROR_CODE ReadFlash(unsigned long ulOffset, int *pnValue);
ERROR_CODE WriteFlash(unsigned long ulOffset, int nValue);

void StartSPI(void);
void EndSPI(void);
void WriteFlashData(int value);
void WriteFlashAdr(long offset);
unsigned char ReadFlashData(void);

ERROR_CODE ReadFlash8(InstructionCode code, int *value);
ERROR_CODE WriteFlash0(InstructionCode code);
ERROR_CODE WriteFlash8(InstructionCode code, int value);
ERROR_CODE StartWriteFlash(InstructionCode code);
ERROR_CODE EndWriteFlash(InstructionCode code);
ERROR_CODE GetSectorNumber(unsigned long ulOffset, int *pnSector);
ERROR_CODE GetSectorStartEnd(long *lStartOff, long *lEndOff, int nSector);

// global data for use with the VisualDSP++ plug-in
char 			*AFP_Title = "BF533CB";
char 			*AFP_Description = "STMicroelectronics M25P16-VMN6TP";
enProgCmds 		AFP_Command = NO_COMMAND;
int 			AFP_ManCode = 0;				// 0x20 = STMicroelectronics
int 			AFP_DevCode = 0;				// 0x2015 = M25P16
unsigned long 	AFP_Offset = 0x0;
int 			*AFP_Buffer;
long 			AFP_Size = BUFFER_SIZE;
long 			AFP_Count = -1;
long 			AFP_Stride = -1;
int 			AFP_NumSectors = NUM_SECTORS;
long 			AFP_SectorSize1 = SECTOR_SIZE;
int 			AFP_SectorSize2 = SECTOR_SIZE;
int 			AFP_Sector = -1;
int 			AFP_Error 			= 0;			// contains last error encountered
bool 			AFP_Verify 			= FALSE;		// verify writes or not
long 			AFP_StartOff 		= 0x0;			// sector start offset
long 			AFP_EndOff 			= 0x0;			// sector end offset
int				AFP_FlashWidth		= 0x08;			// width of the flash device
int 			*AFP_SectorInfo;
SECTORLOCATION SectorInfo[NUM_SECTORS];



// flag telling us that the flash had to be reset
char reset = 0x0;

// exit flag
bool bExit = FALSE;

// *********************
// Map of flash sectors
// *********************

//	Type			START ADDR    	END ADDR		SECTOR NUM
//	-------			----------		--------		----------
//					0x00000			0x0FFFF			00
//					0x10000			0x1FFFF			01
//					0x20000			0x2FFFF			02
//					0x30000			0x3FFFF			03
//					0x40000			0x4FFFF			04
//					0x50000			0x5FFFF			05
//					0x60000			0x6FFFF			06
//					0x70000			0x7FFFF			07
//

main()
{
	int i = 0;

	// by making AFP_Buffer as big as possible the plug-in can send and
	// receive more data at a time making the data transfer quicker
	//
	// by allocating it on the heap the compiler does not create an
	// initialized array therefore making the driver image smaller
	// and faster to load
	//
	// we have modified the linker description file (LDF) so that the heap
	// is large enough to store BUFFER_SIZE elements at this point
	AFP_Buffer = (int *)malloc(BUFFER_SIZE);

	// AFP_Buffer will be NULL if we could not allocate storage for the
	// buffer
	if(AFP_Buffer == NULL)
	{
		// tell GUI that our buffer was not initialized
		AFP_Error = BUFFER_IS_NULL;
	}

	//initiate sector information structures
	for(i = 0; i < AFP_NumSectors; i++)
	{
		GetSectorStartEnd(&SectorInfo[i].lStartOff, &SectorInfo[i].lEndOff, i);
	}

	AFP_SectorInfo = (int*)&SectorInfo[0];

	// setup the flash so the DSP can access it
	SetupForFlash();

	//setup code so that flash programmer can just read memory instead of call GetCodes().
	GetCodes();
	
//	ReadData(0, 16, 1, AFP_Buffer);
	
	// command processing loop
	while(!bExit)
	{
		// the plug-in will set a breakpoint at "AFP_BreakReady" so it knows
		// when we are ready for a new command because the DSP will halt
		//
		// the jump is used so that the label will be part of the debug
		// information in the driver image otherwise it may be left out
		// since the label is not referenced anywhere
		asm("AFP_BreakReady:");
		if(FALSE)
			asm("jump AFP_BreakReady;");

		// switch on the command
		switch(AFP_Command)
		{
			// get manufacturer and device codes
			case GET_CODES:
				AFP_Error = GetCodes();
				break;

			// reset
			case RESET:
				AFP_Error = ResetFlash();
				break;

			// write
			case WRITE:
				AFP_Error = WriteData(AFP_Offset, AFP_Count, AFP_Stride, AFP_Buffer);
				break;

			// fill
			case FILL:
				AFP_Error = FillData(AFP_Offset, AFP_Count, AFP_Stride, AFP_Buffer);
				break;

			// erase all
			case ERASE_ALL:
				AFP_Error = EraseFlash();
				break;

			// erase sector
			case ERASE_SECT:
				AFP_Error = EraseBlock(AFP_Sector);
				break;

			// read
			case READ:
				AFP_Error = ReadData(AFP_Offset, AFP_Count, AFP_Stride, AFP_Buffer);
				break;

			// get sector number based on address
			case GET_SECTNUM:
				AFP_Error = GetSectorNumber(AFP_Offset, &AFP_Sector);
				break;

			// get sector number start and end offset
			case GET_SECSTARTEND:
				AFP_Error = GetSectorStartEnd(&AFP_StartOff, &AFP_EndOff, AFP_Sector);
				break;

			// no command or unknown command do nothing
			case NO_COMMAND:
			default:
				// set our error
				AFP_Error = UNKNOWN_COMMAND;
				break;
		}

		// clear the command
		AFP_Command = NO_COMMAND;
	}

	// free the buffer if we were able to allocate one
	if(AFP_Buffer)
		free(AFP_Buffer);


	// all done
	return 0;
}


//////////////////////////////////////////////////////////////
// ERROR_CODE SetupForFlash()
//
// Perform necessary setup for the processor to talk to the
// flash such as external memory interface registers, etc.
//
//////////////////////////////////////////////////////////////

ERROR_CODE SetupForFlash()
{
	*pFIO_FLAG_D = 0xffff;	// 1111-1111-1111-1111
	*pFIO_DIR = 0xffff;		// 1111-1111-1111-1111
	*pFIO_INEN = 0x0000;
	
	return NO_ERR;
}


//////////////////////////////////////////////////////////////
// ERROR_CODE WriteData()
//
// Write a buffer to flash.
//
// Inputs:	unsigned long ulStart - offset in flash to start the writes at
//			long lCount - number of elements to write, in this case bytes
//			long lStride - number of locations to skip between writes
//			int *pnData - pointer to data buffer
//
//////////////////////////////////////////////////////////////

ERROR_CODE WriteData(unsigned long ulStart, long lCount, long lStride, int *pnData)
{
	long i = 0;						// loop counter
	long lc = 0;
	int j = 0;						// inner loop counter
	unsigned long ulOffset = ulStart;// current offset to write
	int iShift = 0;					// shift value by iShift bits
	int iNumWords = 2;				// number of words in a long
	int nLeftover = lCount % 4;		// how much if any do we have leftover to write
	ERROR_CODE ErrorCode = NO_ERR;	// tells whether there was an error trying to write
	int nCompare = 0;				// value that we use to verify flash
	bool bVerifyError = FALSE;		// lets us know if there was a verify error
	int nSector = 0;

	// if the user wants to verify then do it
	if(AFP_Verify == TRUE)
	{
		// write the buffer up to BUFFER_SIZE items
		for(i = 0; (i < lCount / 4) && (i < BUFFER_SIZE); i++)
		{
			for(iShift = 0, j = 0; ((j < iNumWords) && (ErrorCode == NO_ERR)); j++, ulOffset += (lStride * 2))
			{
				// check to see that the address is within a valid sector
				ErrorCode = GetSectorNumber(ulOffset, &nSector);
				if(NO_ERR == ErrorCode)
				{
					// unlock the flash, do the write, increase shift, and wait for completion
					UnlockFlash(ulOffset);
					WriteFlash(ulOffset, (pnData[i] >> iShift));
					ErrorCode = PollToggleBit(ulOffset);
					ReadFlash(ulOffset, &nCompare);
					if(nCompare != ((pnData[i]  >> iShift) & 0x0000ffff))
					{
						bVerifyError = TRUE;
						break;
					}
					iShift += 16;
				}
				else
				{	// handle error
					bVerifyError = TRUE;
					return ErrorCode;
				}
			}
		}

		// because of the way our ldr file is built, we will always have
		// 2 bytes leftover if there is leftover, because the flash is 16 bit
		// that will mean only one write to do.
		if((nLeftover > 0) && (ErrorCode == NO_ERR) && bVerifyError == FALSE)
		{
			// check to see that the address is within a valid sector
			ErrorCode = GetSectorNumber(ulOffset, &nSector);
			if(NO_ERR == ErrorCode)
			{
				// unlock the flash, do the write, increase shift, and wait for completion
				UnlockFlash(ulOffset);
				WriteFlash(ulOffset, pnData[i]);
				ErrorCode = PollToggleBit(ulOffset);
				ReadFlash(ulOffset, &nCompare);
				if(nCompare != (pnData[i] & 0x0000FFFF))
				{
					bVerifyError = TRUE;
				}
			}
			else
			{
				return ErrorCode;
			}
		}

		// return appropriate error code if there was a verification error
		if( bVerifyError == TRUE )
			return VERIFY_WRITE;
	}
	// the user does not want to verify
	else
	{
		// write the buffer up to BUFFER_SIZE items
		for(lc = 0; lc < (BUFFER_SIZE / 4); lc++, ulOffset += 4)
		{
			// check to see that the address is within a valid sector
			ErrorCode = GetSectorNumber(ulOffset, &nSector);
			if(NO_ERR == ErrorCode)
			{
				// unlock the flash, do the write and wait for completion
				UnlockFlash(ulOffset);
				WriteFlash(ulOffset, pnData[lc] & 0x0000ffff);
				ErrorCode = PollToggleBit(ulOffset);

				UnlockFlash(ulOffset);
				WriteFlash(ulOffset + 2, pnData[lc] >> 16);
				ErrorCode = PollToggleBit(ulOffset);
			}
			else
			{
				return ErrorCode;
			}
		}
	}

	// return the appropriate error code
	return ErrorCode;
}


//////////////////////////////////////////////////////////////
// ERROR_CODE FillData()
//
// Fill flash with a value.
//
// Inputs:	unsigned long ulStart - offset in flash to start the writes at
//			long lCount - number of elements to write, in this case bytes
//			long lStride - number of locations to skip between writes
//			int *pnData - pointer to data buffer
//
//////////////////////////////////////////////////////////////

ERROR_CODE FillData(unsigned long ulStart, long lCount, long lStride, int *pnData)
{
	long i = 0;							// loop counter
	unsigned long ulOffset = ulStart;	// current offset to write
	ERROR_CODE ErrorCode = NO_ERR;		// tells whether we had an error while filling
	int nCompare = 0;					// value that we use to verify flash
	bool bVerifyError = FALSE;			// lets us know if there was a verify error
	unsigned long ulNewOffset = 0;		// used if we have an odd address
	int nSector = 0;

	// if we have an odd offset we need to write a byte
	// to the first location and the last
	if(ulOffset % 2 != 0)
	{
		// read the offset - 1 and OR in our value
		ulNewOffset = ulOffset - 1;
		ReadFlash(ulNewOffset, &nCompare);
		nCompare &= 0xFF00;
		nCompare |= (pnData[0] & 0x00FF);

		// unlock the flash, do the write, and wait for completion
		UnlockFlash(ulNewOffset);
		WriteFlash(ulNewOffset, nCompare);
		ErrorCode = PollToggleBit(ulNewOffset);

		// move to the last offset
		ulNewOffset = (ulOffset - 1) + (lCount * (lStride * 2));

		// read the value and OR in our value
		ReadFlash(ulNewOffset, &nCompare);
		nCompare &= 0x00FF;
		nCompare |= (pnData[0] & 0xFF00);

		// unlock the flash, do the write, and wait for completion
		UnlockFlash(ulNewOffset);
		WriteFlash(ulNewOffset, nCompare);
		ErrorCode = PollToggleBit(ulNewOffset);

		// increment the offset and count
		ulOffset = ((ulOffset - 1) + (lStride * 2));
		lCount--;
	}

	// verify writes if the user wants to
	if(AFP_Verify == TRUE)
	{
		// fill the value
		for(i = 0; ((i < lCount) && (ErrorCode == NO_ERR)); i++, ulOffset += (lStride * 2))
		{
			// check to see that the address is within a valid sector
			ErrorCode = GetSectorNumber(ulOffset, &nSector);
			if(NO_ERR == ErrorCode)
			{
				// unlock the flash, do the write, and wait for completion
				UnlockFlash(ulOffset);
				WriteFlash(ulOffset, pnData[0]);
				ErrorCode = PollToggleBit(ulOffset);
				ReadFlash( ulOffset, &nCompare );
				if( nCompare != ( pnData[0] & 0x0000FFFF ) )
				{
					bVerifyError = TRUE;
					break;
				}
			}
			else
			{
				return ErrorCode;
			}

		}

		// return appropriate error code if there was a verification error
		if( bVerifyError == TRUE )
			return VERIFY_WRITE;
	}
	// user did not want to verify writes
	else
	{
		// fill the value
		for (i = 0; ( ( i < lCount ) && ( ErrorCode == NO_ERR ) ); i++, ulOffset += (lStride * 2) )
		{
			// you cannot write to this region in flash
			if( ( ulOffset >= 0x20270000 ) && ( ulOffset < 0x20280000 ) )
				return INVALID_SECTOR;

			// check to see that the address is within a valid sector
			ErrorCode = GetSectorNumber( ulOffset, &nSector );
			if( NO_ERR == ErrorCode )
			{
				// unlock the flash, do the write, and wait for completion
				UnlockFlash(ulOffset);
				WriteFlash( ulOffset, pnData[0] );
				ErrorCode = PollToggleBit(ulOffset);
			}
			else
			{
				return ErrorCode;
			}
		}
	}

	// return the appropriate error code
	return ErrorCode;
}


//////////////////////////////////////////////////////////////
// ERROR_CODE ReadData()
//
// Read a buffer from flash.
//
// Inputs:	unsigned long ulStart - offset in flash to start the reads at
//			int nCount - number of elements to read, in this case bytes
//			int nStride - number of locations to skip between reads
//			int *pnData - pointer to data buffer to fill
//
//////////////////////////////////////////////////////////////

ERROR_CODE ReadData( unsigned long ulStart, long lCount, long lStride, int *pnData )
{
	long i = 0;						// loop counter
	int j = 0;						// inner loop counter
	unsigned long ulOffset = ulStart;			// current offset to write
	int iShift = 0;					// shift value by iShift bits
	int iNumWords = 2;				// number of words in a long
	int nLeftover = lCount % 4;		// how much if any do we have leftover to write
	int nHi,nLow;
	int nSector = 0;
	ERROR_CODE ErrorCode = NO_ERR;		// tells whether we had an error while filling

	// write the buffer up to BUFFER_SIZE items
	for(i = 0; (i < lCount/4) && (i < BUFFER_SIZE); i++)
	{
		for(iShift = 0, j = 0; j < iNumWords ; j+=2)
		{
			// check to see that the address is within a valid sector
			ErrorCode = GetSectorNumber(ulOffset, &nSector);
			if(NO_ERR == ErrorCode)
			{
				// Read flash
				ReadFlash(ulOffset, &nLow);
				ulOffset += (lStride * 2);
				ReadFlash(ulOffset, &nHi);
				ulOffset += (lStride * 2);
				pnData[i] = (nHi << 16) | nLow;
			}
			else
			{
				return ErrorCode;
			}
		}
	}

	// because of the way our ldr file is built, we will always have
	// 2 bytes leftover if there is leftover, because the flash is 16 bit
	// that will mean that there is only one write left to do.
	if(nLeftover > 0)
	{
		// check to see that the address is within a valid sector
		ErrorCode = GetSectorNumber(ulOffset, &nSector);
		if(NO_ERR == ErrorCode)
		{
			ReadFlash(ulOffset, &pnData[i]);
		}
		else
		{
			return ErrorCode;
		}
	}

	// return the appropriate error code
	return ErrorCode;

/*
	long lc;
	int offset;
	int rd;
	int value;
	ERROR_CODE ErrorCode = NO_ERR;		// tells whether we had an error while filling
	
	for(lc = 0; (lc < lCount / 4) && (lc < BUFFER_SIZE); lc++)
		pnData[lc] = 0;

	StartSPI();
	WriteFlashData(READD);
	WriteFlashAdr(ulStart);
	for(lc = 0; lc < lCount; lc++)
	{
		offset = lc / 4;
		rd = ReadFlashData();
		rd &= 0xff:
		switch(lc % 4)
		{
			case 0:
				pnData[offset] = rd << 24;
				break;
			
			case 1:
				pnData[offset] |= (rd << 16);
				break;

			case 2:
				pnData[offset] |= (rd << 8);
				break;

			case 3:
				pnData[offset] |= rd;
				break;
			}
		}
	}
	EndSPI();

	// return the appropriate error code
	return ErrorCode;
*/
}

//////////////////////////////////////////////////////////////
// ERROR_CODE WriteFlash()
//
// Write a value to an offset in flash.
//
// Inputs:	unsigned long ulOffset - offset to write to
//			int nValue - value to write
//
//////////////////////////////////////////////////////////////

ERROR_CODE WriteFlash(unsigned long ulOffset, int nValue)
{
	StartSPI();
	WriteFlashData(PP);
	WriteFlashAdr(ulOffset);
	WriteFlashData(nValue & 0xff);
	WriteFlashData((nValue >> 8) & 0xff);
	EndSPI();

	// ok
	return NO_ERR;
}

//////////////////////////////////////////////////////////////
// ERROR_CODE ReadFlash()
//
// Read a value from an offset in flash.
//
// Inputs:	unsigned long ulOffset - offset to read from
//			int pnValue - pointer to store value read from flash
//
//////////////////////////////////////////////////////////////

ERROR_CODE ReadFlash( unsigned long ulOffset, int *pnValue )
{
	unsigned char hi, low;
	
	StartSPI();
	WriteFlashData(READD);
	WriteFlashAdr(ulOffset);
	low = ReadFlashData();
	hi = ReadFlashData();
	EndSPI();
	
	*pnValue = (hi << 8) | low;

	// ok
	return NO_ERR;
}

void StartSPI(void)
{
	// Enable PF2
	*pSPI_FLG = FLS2;
	// Set baud rate SCK = HCLK/(2*SPIBAUD) SCK = 5MHz
	*pSPI_BAUD = 12;
	// configure spi port
	// SPI 8-bit data, MSB first, SPI Master
	*pSPI_CTL = SPE | CPHA | CPOL | MSTR | GM | EMISO | 0x01;
	asm("ssync;\n");
}

void EndSPI(void)
{
	// disable spi
	*pSPI_FLG = 0xff00;
	*pSPI_CTL = 0x0000;
	asm("ssync;\n");
}

void WaitForCompleteSPI(void)
{
	asm("ssync;\n");

	// Wait for send complete.
	while(!(*pSPI_STAT & SPIF))
		;
}

void WriteFlashData(int value)
{
	// Wait for send complete.
	while(*pSPI_STAT & TXS)
		;

	*pSPI_TDBR = (unsigned short)value;
	WaitForCompleteSPI();
}

void WriteFlashAdr(long offset)
{
	offset &= 0x00ffffff;

	WriteFlashData((offset >> 16) & 0x00ff);
	WriteFlashData((offset >> 8) & 0x00ff);
	WriteFlashData(offset & 0x00ff);
}

unsigned char ReadFlashData(void)
{
	// Wait for send complete.
	while(*pSPI_STAT & TXS)
		;

	*pSPI_TDBR = 0x00;
	WaitForCompleteSPI();

	return (unsigned char)(*pSPI_RDBR & 0x00ff);
}

ERROR_CODE WriteFlash0(InstructionCode code)
{
	StartSPI();
	WriteFlashData(code);
	EndSPI();
	
	// ok
	return NO_ERR;
}

//////////////////////////////////////////////////////////////
// ERROR_CODE WriteFlash8()
//
// Write a value with instruction code in flash.
//
// Inputs:	InstructionCode code - instruction code
//			int - value to write
//
//////////////////////////////////////////////////////////////

ERROR_CODE WriteFlash8(InstructionCode code, int value)
{
	StartSPI();
	WriteFlashData(code);
	WriteFlashData(value);
	EndSPI();
	
	// ok
	return NO_ERR;
}

//////////////////////////////////////////////////////////////
// ERROR_CODE ReadFlash8()
//
// Read a value from an offset in flash.
//
// Inputs:	InstructionCode code - instruction code
//			int *value - pointer to store value read from flash
//
//////////////////////////////////////////////////////////////

ERROR_CODE ReadFlash8(InstructionCode code, int *value)
{
	StartSPI();
	WriteFlashData(code);
	*value = ReadFlashData();
	EndSPI();
	
	// ok
	return NO_ERR;
}

//////////////////////////////////////////////////////////////
// ERROR_CODE PollToggleBit()
//
// Polls the toggle bit in the flash to see when the operation
// is complete.
//
// Inputs:	unsigned long ulOffset - offset in flash
//
//////////////////////////////////////////////////////////////

ERROR_CODE PollToggleBit(unsigned long ulOffset)
{
	ERROR_CODE errorCode = POLL_TIMEOUT;	// flag to indicate error
	int value;

	while(1)
	{
		ReadFlash8(RDSR, &value);
		if(!(value & 0x01))
		{
			errorCode = NO_ERR;
			break;
		}
	}
	
	// we can return
	return errorCode;
}


//////////////////////////////////////////////////////////////
// ERROR_CODE ResetFlash()
//
// Sends a "reset" command to the flash.
//
//////////////////////////////////////////////////////////////

ERROR_CODE ResetFlash()
{
	WriteFlash8(WRSR, 0x1c);		// 0-00-111-00
	WriteFlash0(WRDI);

	return NO_ERR;
}


//////////////////////////////////////////////////////////////
// ERROR_CODE EraseFlash()
//
// Sends an "erase all" command to the flash.
//
//////////////////////////////////////////////////////////////

ERROR_CODE EraseFlash()
{
	ERROR_CODE errorCode = NO_ERR;	// tells us if there was an error erasing flash

	UnlockFlash(0);
	
	// erase contents in Main Flash Array
	WriteFlash0(BE);

	// poll until the command has completed
	errorCode = PollToggleBit(0x0000);

	// erase should be complete
	return errorCode;
}


//////////////////////////////////////////////////////////////
// ERROR_CODE EraseBlock()
//
// Sends an "erase block" command to the flash.
//
//////////////////////////////////////////////////////////////

ERROR_CODE EraseBlock(int nBlock)
{
	ERROR_CODE errorCode = NO_ERR;	// tells us if there was an error erasing flash
	long start, end;
	
	GetSectorStartEnd(&start, &end, nBlock);
	StartSPI();
	WriteFlashData(SE);
	WriteFlashAdr(start);
	EndSPI();

	// poll until the command has completed
	errorCode = PollToggleBit(start);

	// block erase should be complete
	return errorCode;
}


//////////////////////////////////////////////////////////////
// ERROR_CODE UnlockFlash()
//
// Sends an "unlock" command to the flash to allow the flash
// to be programmed.
//
//////////////////////////////////////////////////////////////

ERROR_CODE UnlockFlash(unsigned long ulOffset)
{
	WriteFlash8(WRSR, 0x80);		// 1-00-000-00
	WriteFlash0(WREN);
	int a;
	ReadFlash8(RDSR, &a);

	// ok
	return NO_ERR;
}

//////////////////////////////////////////////////////////////
// ERROR_CODE GetCodes()
//
// Sends an "auto select" command to the flash which will allow
// us to get the manufacturer and device codes.
//
//////////////////////////////////////////////////////////////

ERROR_CODE GetCodes()
{
	// send the auto select command to the flash
	StartSPI();
	WriteFlashData(RDID);
	AFP_ManCode = ReadFlashData();
	AFP_DevCode = ReadFlashData();
	AFP_DevCode <<= 8;
	AFP_DevCode |= ReadFlashData();
	EndSPI();

	// we need to issue another command to get the part out
	// of auto select mode so issue a reset which just puts
	// the device back in read mode
	ResetFlash();

	// ok
	return NO_ERR;
}


//////////////////////////////////////////////////////////////
// ERROR_CODE GetSectorNumber()
//
// Gets a sector number based on the offset.
//
//////////////////////////////////////////////////////////////

ERROR_CODE GetSectorNumber(unsigned long ulOffset, int *pnSector)
{
	int sector = ulOffset / SECTOR_SIZE;
	
	if(sector > NUM_SECTORS)
		// invalid sector
		return INVALID_SECTOR;

	*pnSector = sector;

	// ok
	return NO_ERR;
}

//////////////////////////////////////////////////////////////
// ERROR_CODE GetSectorStartEnd()
//
// Gets a sector number based on the offset.
//
// Inputs:	long *lStartOff - pointer to the start offset
//			long *lEndOff - pointer to the end offset
//			int nSector - sector number
//
//////////////////////////////////////////////////////////////

ERROR_CODE GetSectorStartEnd( long *lStartOff, long *lEndOff, int nSector )
{
	long lSectorSize = 0;

	if(nSector < NUM_SECTORS)
	{
		*lStartOff = nSector * SECTOR_SIZE;
		*lEndOff = ((nSector + 1) * SECTOR_SIZE) - 1;
	}
	// no such sector
	else
		return INVALID_SECTOR;

	// ok
	return NO_ERR;
}

