/*
*********************************************************************************************************
* @file    	key.h
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-14 16:10:26
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	按键头文件
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
#ifndef __KEY_H
#define __KEY_H

/*
*********************************************************************************************************
*                              				Exported Includes
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Exported define
*********************************************************************************************************
*/
#define BIT_KEY_NONE					 	0x00000000
#define BIT_KEY_ADD							0x00000001
#define BIT_KEY_7						 	0x00000002
#define BIT_KEY_4						 	0x00000004	
#define BIT_KEY_1						 	0x00000008
#define BIT_KEY_2						 	0x00000010
#define BIT_KEY_5						 	0x00000020
#define BIT_KEY_8						 	0x00000040
#define BIT_KEY_0							0x00000080
#define BIT_KEY_POINT					 	0x00000100
#define BIT_KEY_9						 	0x00000200
#define BIT_KEY_6						 	0x00000400
#define BIT_KEY_3						 	0x00000800
#define BIT_KEY_BACK					 	0x00001000
#define BIT_KEY_LEFT					 	0x00002000
#define BIT_KEY_SHIFT					 	0x00004000
#define BIT_KEY_PRINT					 	0x00008000
#define BIT_KEY_RUN						 	0x00010000
#define BIT_KEY_DOWN						0x00020000
#define BIT_KEY_ENTER					 	0x00040000
#define BIT_KEY_UP						 	0x00080000
#define BIT_KEY_ESC						 	0x00100000
#define BIT_KEY_RIGHT					 	0x00200000
#define BIT_KEY_TARE				 	 	0x00400000
#define BIT_KEY_STOP					 	0x00800000
#define BIT_KEY_BACKUP1					 	0x01000000
#define BIT_KEY_OIL_RUN					 	0x02000000
#define BIT_KEY_OIL_STOP				 	0x04000000
#define BIT_KEY_BACKUP2					 	0x08000000
#define BIT_KEY_F4						 	0x10000000
#define BIT_KEY_F3						 	0x20000000
#define BIT_KEY_F2						 	0x40000000	
#define BIT_KEY_F1						 	0x80000000

#define KEY_VALUE_START						KEY_VALUE_0
#define KEY_VALUE_END						KEY_VALUE_BACKUP2

/*
*********************************************************************************************************
*                              				Exported types
*********************************************************************************************************
*/
typedef enum
{
	KEY_VALUE_NONE								= 0,
	KEY_VALUE_0									= 1,
	KEY_VALUE_1									= 2,
	KEY_VALUE_2									= 3,
	KEY_VALUE_3									= 4,
	KEY_VALUE_4									= 5,
	KEY_VALUE_5									= 6,
	KEY_VALUE_6									= 7,
	KEY_VALUE_7									= 8,
	KEY_VALUE_8									= 9,
	KEY_VALUE_9									= 10,	
	KEY_VALUE_UP								= 11,
	KEY_VALUE_DOWN								= 12,
	KEY_VALUE_LEFT								= 13,
	KEY_VALUE_RIGHT								= 14,
	KEY_VALUE_ENTER								= 15,
	KEY_VALUE_ESC								= 16,
	KEY_VALUE_F1								= 17,
	KEY_VALUE_F2								= 18,
	KEY_VALUE_F3								= 19,
	KEY_VALUE_F4								= 20,
	KEY_VALUE_RUN								= 21,
	KEY_VALUE_STOP								= 22,
	KEY_VALUE_POINT								= 23,
	KEY_VALUE_BACK								= 24,
	KEY_VALUE_SHIFT								= 25,
	KEY_VALUE_TARE								= 26,
	KEY_VALUE_ADD								= 27,
	KEY_VALUE_PRINT								= 28,
	KEY_VALUE_OIL_RUN							= 29,
	KEY_VALUE_OIL_STOP							= 30,
	KEY_VALUE_BACKUP1							= 31,
	KEY_VALUE_BACKUP2							= 32,	
}KEY_VALUE_VALUE_ENUM;

typedef enum
{
	STATUS_KEY_NONE,			/* 未按下 	*/
	STATUS_KEY_PRESS = 0,		/* 按下 	*/
	STATUS_KEY_LONG,			/* 长按 	*/
	STATUS_KEY_HIGH_SPEED,		/* 高速 	*/
	STATUS_KEY_LEAVE,			/* 松手 	*/
}KEY_STATUS_TypeDef;

typedef struct tagKEY_TypeDef KEY_TypeDef;


/*
*********************************************************************************************************
*                              				Exported constants
*********************************************************************************************************
*/
static const uint32_t DEFAULT_KEY_LONG_PRESS_TIME_MS			= 500;
static const uint32_t DEFAULT_KEY_HIGH_SPEED_TIME_MS			= 50;
static const uint32_t DEFAULT_KEY_REPEAT_NUMS					= 3;

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
extern KEY_TypeDef * const g_keyPtr;

/*
*********************************************************************************************************
*                              				Exported functions
*********************************************************************************************************
*/
void Key_Init( void );
bool ConfigKeyTimes( KEY_TypeDef *keyPtr, uint32_t keyValue, \
		uint32_t longPressTime, uint32_t repeatNums, uint32_t highSpeedTime );
bool ConfigKeyFilterTimes( KEY_TypeDef *keyPtr, uint32_t keyValue, uint32_t filterTime );
void KeyScanCycleTask( KEY_TypeDef *keyPtr );
void ClearKeyQueue( KEY_TypeDef *keyPtr );
uint32_t ReadKeyValue( KEY_TypeDef *keyPtr );
KEY_STATUS_TypeDef ReadKeyStatus( KEY_TypeDef *keyPtr );
bool ReleaseKeyMsg( KEY_TypeDef *keyPtr );

#endif
/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
