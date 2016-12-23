/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/
#include "kernel.h"
#include "diskio.h"		/* FatFs lower layer API */
#include "ff_gen_drv.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */

extern Disk_drvTypeDef  disk;


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	return disk.drv[pdrv]->disk_status(disk.lun[pdrv]);
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = RES_OK;

	if(disk.is_initialized[pdrv] == 0)
	{ 
		disk.is_initialized[pdrv] = 1;
		stat = disk.drv[pdrv]->disk_initialize(disk.lun[pdrv]);
	}
	
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	return disk.drv[pdrv]->disk_read(disk.lun[pdrv], buff, sector, count); 
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	return disk.drv[pdrv]->disk_write(disk.lun[pdrv], buff, sector, count);
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	return disk.drv[pdrv]->disk_ioctl(disk.lun[pdrv], cmd, buff);
}

/**
  * @brief  Gets Time from RTC 
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime (void)
{
	/**
		bit31:25 	Year origin from the 1980 (0..127)
		bit24:21	Month (1..12)
		bit20:16	Day of the month(1..31)
		bit15:11	Hour (0..23)
		bit10:5		Minute (0..59)
		bit4:0		Second / 2 (0..29)
	*/
	tTime time;
	
	ReadSystemDate(&time);
	ReadSystemTime(&time);
    
	return  ((time.usYear-1980) << 25)    	// Year = 2007
            | (time.ucMon << 21)            // Month = June
            | (time.ucMday << 16)           // Day = 5
            | (time.ucHour << 11)           // Hour = 11
            | (time.ucMin << 5)            	// Min = 38
            | (time.ucSec / 2);             // Sec = 0
            
}

