/*
*********************************************************************************************************
* @file    	emWinUtils.h
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-31 15:48:55
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	emWin相关支持头文件
*********************************************************************************************************
* @attention
*
* 
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Define to prevent recursive inclusion
*********************************************************************************************************
*/
#ifndef __EMWIN_UTILS_H
#define __EMWIN_UTILS_H

/*
*********************************************************************************************************
*                              				Exported Includes
*********************************************************************************************************
*/
#include "bsp.h"
#include "GUI.h"
#include "DIALOG.h"
#include "ff_gen_drv.h"


/*
*********************************************************************************************************
*                              				Exported define
*********************************************************************************************************
*/
/* FLASH 字库 */		
#define FONT_BASE_ADDR						( KByte(100) )						//FLASH字库基地址
#define FONT_PASSWORD_ADDR					( FONT_BASE_ADDR - KByte(4) )		//FLASH字库密码地址
#define	FLASH_FONT_PASSWORD					(0x2F8E4A7D)						//FLASH字库密码

/* 字体 */
#define FONT_CH_SONGTI_16X16_ADDR			(FONT_BASE_ADDR + 0)
#define FONT_CH_SONGTI_16X16_PATH			("hz/song16.zk")

#define FONT_CH_SONGTI_24X24_ADDR			(FONT_BASE_ADDR + KByte(260))
#define FONT_CH_SONGTI_24X24_PATH			("hz/song24.zk")

#define USE_XBF_FONT						(0x00U)


/*
*********************************************************************************************************
*                              				Exported types
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Exported constants
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Exported macro
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Exported variables
*********************************************************************************************************
*/
#if (USE_XBF_FONT)
extern GUI_FONT 		GUI_FontSongTi_16X16;
extern GUI_FONT 		GUI_FontSongTi_24X24;
extern GUI_FONT 		GUI_FontSongTi_32X32;
#endif

extern GUI_CONST_STORAGE  GUI_FONT GUI_FontSongTi_16X16;
extern GUI_CONST_STORAGE  GUI_FONT GUI_FontSongTi_24X24;
extern GUI_CONST_STORAGE  GUI_FONT GUI_FontSongTi_32X32;

/*
*********************************************************************************************************
*                              				Exported functions
*********************************************************************************************************
*/
void emWinShowBMPEx(uint16_t x, uint16_t y, const char * sFilename);
bool emWinSaveBMPEx( const char * filePath );
void emWinShowJPEGEx(uint16_t x, uint16_t y, const char * sFilename);
void ShowGIFEx(uint16_t x, uint16_t y, const char * sFilename);
bool ShowPNGEx(uint16_t usPOSX, uint16_t usPOSY, const char * sFilename);

#if (USE_XBF_FONT)
bool Open_XBF(void);
#endif

void UpdateFlashFontTask( void (*NotifyProgress_CallBack)( uint8_t fontSize, uint8_t progress ) );

#endif
/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
