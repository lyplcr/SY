/*
*********************************************************************************************************
* @file    	utils.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-11 15:10:28
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	工具包头文件
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
#ifndef __UTILS_H
#define __UTILS_H

/*
*********************************************************************************************************
*                              				Exported Includes
*********************************************************************************************************
*/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#include "stm32f4xx_hal.h"
#include "ustdlib.h"
#include "ustring.h"
#include "isqrt.h" 
#include "fonts.h"
#include "list.h"

#include "config.h"
#include "MY_DEBUG.H"
#include "MY_ASSERT.H"

/*
*********************************************************************************************************
*                              				Exported define
*********************************************************************************************************
*/
#define MAX_STRING_NUM						(32)
#define EPSINON								(0.00001f)				//浮点数不存在绝对的0，只能取近似值
#define PIC_BUFF_SIZE						(4*1024)				//用于图片等大容量共享缓存
/*
*********************************************************************************************************
*                              				Exported types
*********************************************************************************************************
*/
/* 数据结构类型定义 */
typedef enum
{
	STATUS_DATA_STRUCT_FALSE = 0,
	STATUS_DATA_STRUCT_TRUE,
	STATUS_DATA_STRUCT_UNDEFINED,
}DATA_STRUCT_STATUS_ENUM;

/* 哈希表结构类型定义 */
typedef enum
{
	STATUS_HASH_FALSE = 0,
	STATUS_HASH_TRUE = 1,
	STATUS_HASH_UNDEFINED,
}STATUS_HASH_ENUM;

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
__STATIC_INLINE void ExchangeData_UINT16( void *data1, void *data2 )
{
	uint16_t *data1Ptr = (uint16_t *)data1;
	uint16_t *data2Ptr = (uint16_t *)data2;
	uint16_t temp = *data1Ptr;
	
	*data1Ptr = *data2Ptr;
	*data2Ptr = temp;
}

__STATIC_INLINE void *new(uint32_t size) {	
	void *object = calloc(1, size);
	ECHO_ASSERT(object != NULL);
	
	return object;
}

__STATIC_INLINE void delete(void *object) {
	ECHO_ASSERT(object != NULL);
	
	free(object);
}				


/*
*********************************************************************************************************
*                              				Exported variables
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Exported functions
*********************************************************************************************************
*/
uint16_t hton16(uint16_t n);
uint32_t hton32(uint32_t n);
void float2char( uint8_t sumBit, uint8_t pointBit, float source, char * const str );
void num2char( uint8_t sumBit, int32_t source, char * const str );
uint8_t RemoveStrChar( char *str, const char c );
bool FloatIsEqual(float fp1, float fp2);
uint8_t GetMonthDays( uint16_t year, uint8_t mouth );
TestStatus BuffCmpWithChar( const uint8_t *pSorce, uint8_t target, uint32_t lenth );
void ExchangeData( void *data1, void *data2, void (*exchangeData_CallBack)( void *data1, void *data2 ) );


#endif
/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
