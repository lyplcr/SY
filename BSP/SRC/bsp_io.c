/*
*********************************************************************************************************
* @file    	bsp_io.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-13 19:53:06
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	IO驱动源文件
*********************************************************************************************************
* @attention
*
* 
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private Includes
*********************************************************************************************************
*/
#include "bsp.h"


/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				74HC165
*********************************************************************************************************
*/
#define	HC165_GPIO_CLK_ENABLE()				__HAL_RCC_GPIOE_CLK_ENABLE()

#define	HC165_CLK_GPIO_PIN					GPIO_PIN_0
#define	HC165_CLK_GPIO_PORT					GPIOE

#define	HC165_LOCK_GPIO_PIN					GPIO_PIN_1
#define	HC165_LOCK_GPIO_PORT				GPIOE

#define	HC165_DAT_GPIO_PIN					GPIO_PIN_2
#define	HC165_DAT_GPIO_PORT					GPIOE
	
#define	HC165_GPIO_CLK_SET()				HAL_GPIO_WritePin(HC165_CLK_GPIO_PORT,HC165_CLK_GPIO_PIN,GPIO_PIN_SET)
#define	HC165_GPIO_LOCK_SET() 				HAL_GPIO_WritePin(HC165_LOCK_GPIO_PORT,HC165_LOCK_GPIO_PIN,GPIO_PIN_SET)
	
#define	HC165_GPIO_CLK_CLEAR()				HAL_GPIO_WritePin(HC165_CLK_GPIO_PORT,HC165_CLK_GPIO_PIN,GPIO_PIN_RESET)	
#define	HC165_GPIO_LOCK_CLEAR() 			HAL_GPIO_WritePin(HC165_LOCK_GPIO_PORT,HC165_LOCK_GPIO_PIN,GPIO_PIN_RESET)

#define	HC165_GPIO_DAT_READ()   			HAL_GPIO_ReadPin(HC165_DAT_GPIO_PORT,HC165_DAT_GPIO_PIN)

/*
*********************************************************************************************************
*                              				74HC595
*********************************************************************************************************
*/
#define USE_HC595_EN_GPIO					0x00U	//使用芯片使能配置引脚

#define	HC595_GPIO_CLK_ENABLE()				do {\
													__HAL_RCC_GPIOB_CLK_ENABLE();\
													__HAL_RCC_GPIOE_CLK_ENABLE();\
											} while (0)

#define	HC595_RCK_GPIO_PORT					GPIOE
#define	HC595_RCK_GPIO_PIN					GPIO_PIN_4

#define	HC595_SCK_GPIO_PORT					GPIOE
#define	HC595_SCK_GPIO_PIN					GPIO_PIN_0

#define	HC595_SDA_GPIO_PORT					GPIOE
#define	HC595_SDA_GPIO_PIN					GPIO_PIN_3

#define	HC595_SE_GPIO_PORT					GPIOB
#define	HC595_SE_GPIO_PIN					GPIO_PIN_9
	
#define	HC595_GPIO_RCK_SET()				HAL_GPIO_WritePin(HC595_RCK_GPIO_PORT,HC595_RCK_GPIO_PIN,GPIO_PIN_SET)
#define	HC595_GPIO_SCK_SET() 				HAL_GPIO_WritePin(HC595_SCK_GPIO_PORT,HC595_SCK_GPIO_PIN,GPIO_PIN_SET)
#define	HC595_GPIO_SDA_SET() 				HAL_GPIO_WritePin(HC595_SDA_GPIO_PORT,HC595_SDA_GPIO_PIN,GPIO_PIN_SET)	
#define	HC595_GPIO_SE_SET() 				HAL_GPIO_WritePin(HC595_SE_GPIO_PORT,HC595_SE_GPIO_PIN,GPIO_PIN_SET)
 
#define	HC595_GPIO_RCK_CLEAR()				HAL_GPIO_WritePin(HC595_RCK_GPIO_PORT,HC595_RCK_GPIO_PIN,GPIO_PIN_RESET)
#define	HC595_GPIO_SCK_CLEAR() 				HAL_GPIO_WritePin(HC595_SCK_GPIO_PORT,HC595_SCK_GPIO_PIN,GPIO_PIN_RESET)
#define	HC595_GPIO_SDA_CLEAR() 				HAL_GPIO_WritePin(HC595_SDA_GPIO_PORT,HC595_SDA_GPIO_PIN,GPIO_PIN_RESET)	
#define	HC595_GPIO_SE_CLEAR() 				HAL_GPIO_WritePin(HC595_SE_GPIO_PORT,HC595_SE_GPIO_PIN,GPIO_PIN_RESET)

/*
*********************************************************************************************************
*                              				蜂鸣器
*********************************************************************************************************
*/
#define	BEEP_GPIO_CLK_ENABLE()				__HAL_RCC_GPIOB_CLK_ENABLE()
#define	BEEP_GPIO_PORT  					GPIOB
#define	BEEP_GPIO_PIN						GPIO_PIN_8

/*
*********************************************************************************************************
*                              				小灯
*********************************************************************************************************
*/
#define	LED_GPIO_CLK_ENABLE()				__HAL_RCC_GPIOB_CLK_ENABLE()
#define	LED_GPIO_PORT 						GPIOB
#define	LED_GPIO_PIN						GPIO_PIN_5

#define INPUT_REV							(0)

/*
*********************************************************************************************************
*                              				Private typedef
*********************************************************************************************************
*/
typedef enum
{
	BIT_PUMP_START 	= 0x00000040,
	BIT_PUMP_STOP 	= 0x00000080,
}LAMP_74HC595_ENUM;

/*
*********************************************************************************************************
*                              				Private constants
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private macro
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private variables
*********************************************************************************************************
*/
static uint32_t g_InportData;
static uint32_t g_InportShadow;
static uint32_t g_OutportData;
static uint32_t g_OutportShadow;
static uint32_t g_LampData;
static uint32_t g_LampShadow;

/*
*********************************************************************************************************
*                              				Private function prototypes
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private functions
*********************************************************************************************************
*/  

/*
*********************************************************************************************************
*                              				输入端口
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : Init_74HC165
* Description	: 初始化芯片74HC165
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void Init_74HC165( void )
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	HC165_GPIO_CLK_ENABLE();
	
	GPIO_InitStructure.Pin = HC165_CLK_GPIO_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;	
	HAL_GPIO_Init(HC165_CLK_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = HC165_LOCK_GPIO_PIN;
	HAL_GPIO_Init(HC165_LOCK_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = HC165_DAT_GPIO_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(HC165_DAT_GPIO_PORT, &GPIO_InitStructure);
	
	HC165_GPIO_CLK_CLEAR();
	HC165_GPIO_LOCK_SET();
}

/*
*********************************************************************************************************
* Function Name : __bsp_74HC165_Read32
* Description	: 从74HC165读取32位数据
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static uint32_t __bsp_74HC165_Read32( void )
{
	uint8_t i;
	uint32_t value = 0;
	
	HC165_GPIO_LOCK_CLEAR();
	bsp_DelayUS(1);
	HC165_GPIO_LOCK_SET(); 
	
	for (i=0; i<32; i++)
	{		  			
		value <<= 1;
		if( HC165_GPIO_DAT_READ() )
		{
			value |= 0x01;
		} 
		
		HC165_GPIO_CLK_SET();
		bsp_DelayUS(1);
		HC165_GPIO_CLK_CLEAR();	
	}
	
	return value;
}

/*
*********************************************************************************************************
* Function Name : bsp_74HC165_Read
* Description	: 从74HC165读取数据
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t bsp_74HC165_Read( void )
{
	return ( ~__bsp_74HC165_Read32() );
}

/*
*********************************************************************************************************
* Function Name : InportInit
* Description	: 初始化输入端口
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void bsp_InitInport(void)
{
	Init_74HC165();	
	
	g_InportShadow = 0x00000000 ^ INPUT_REV;
	g_InportData   = 0x00000000 ^ INPUT_REV;
	
	bsp_InportCycleTask();	
}

/*
*********************************************************************************************************
* Function Name : bsp_InportRead
* Description	: 读取输入端口
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t bsp_InportRead( void )
{	
	return (g_InportData^INPUT_REV);
}

/*
*********************************************************************************************************
* Function Name : bsp_InportCycleTask
* Description	: 端口输入循环任务(内部已实现防抖)
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_InportCycleTask(void)
{
	uint32_t tempu32 = 0;
	
	/* 	
		如果检测到输入信号发生变化，本周期将更新影子寄存器，下个周期再次比对
		输入信号是否发生变化，如果不变了，将影子寄存器的值存储到输入寄存器。
		效果相当于延迟一个周期，起到防抖效果。
	*/	
	if (tempu32 != g_InportShadow)
	{
		g_InportShadow = tempu32;
	}	
	else
	{
		g_InportData = g_InportShadow;
	}
	
	ECHO(DEBUG_BSP_IO,"in   : %08x H",g_InportData^INPUT_REV);
}

/*
*********************************************************************************************************
*                              				输出端口
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : bsp_Init_74HC595
* Description	: 初始化芯片74HC595
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void bsp_Init_74HC595( void )
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	HC595_GPIO_CLK_ENABLE(); 
	
	GPIO_InitStructure.Pin = HC595_RCK_GPIO_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;	
	HAL_GPIO_Init(HC595_RCK_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = HC595_SCK_GPIO_PIN;	
	HAL_GPIO_Init(HC595_SCK_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = HC595_SDA_GPIO_PIN;	
	HAL_GPIO_Init(HC595_SDA_GPIO_PORT, &GPIO_InitStructure);
	
	#if (USE_HC595_EN_GPIO == 0x01U)
		GPIO_InitStructure.Pin = HC595_SE_GPIO_PIN;
		HAL_GPIO_Init(HC595_SE_GPIO_PORT, &GPIO_InitStructure);
	#endif
	
 	HC595_GPIO_RCK_CLEAR();
 	HC595_GPIO_SCK_CLEAR();
 	HC595_GPIO_SDA_CLEAR();
	
	#if (USE_HC595_EN_GPIO == 0x01U)
		HC595_GPIO_SE_CLEAR();
	#endif
}

/*
*********************************************************************************************************
* Function Name : __bsp_74HC595_Write32
* Description	: 向74HC595写入32位数据
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void __bsp_74HC595_Write32( uint32_t value )
{
	uint8_t i;
	
	for (i=0; i<32; i++)
	{		  	
		if (value & 0x80000000)
		{
			HC595_GPIO_SDA_SET();
		}
		else
		{
			HC595_GPIO_SDA_CLEAR();
		}
		
		HC595_GPIO_SCK_SET();
		bsp_DelayUS(1);
		HC595_GPIO_SCK_CLEAR();
		
		value<<=1;		
	}
	
	HC595_GPIO_RCK_SET();
	bsp_DelayUS(1);
	HC595_GPIO_RCK_CLEAR();
}

/*
*********************************************************************************************************
* Function Name : bsp_74HC595_Write
* Description	: 向74HC595写入数据
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_74HC595_Write( uint32_t value )
{
	__bsp_74HC595_Write32(value);
}

/*
*********************************************************************************************************
* Function Name : bsp_InitBeep
* Description	: 蜂鸣器初始化
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void bsp_InitBeep( void )
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	BEEP_GPIO_CLK_ENABLE();
	
	GPIO_InitStructure.Pin = BEEP_GPIO_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;	
	HAL_GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);
	
	HAL_GPIO_WritePin(BEEP_GPIO_PORT,BEEP_GPIO_PIN,GPIO_PIN_RESET);
}

/*
*********************************************************************************************************
* Function Name : bsp_SetOutportBit
* Description	: 设置输出位
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_SetOutportBit( uint32_t bit )
{ 
	SET_BIT(g_OutportShadow, bit);
} 

/*
*********************************************************************************************************
* Function Name : bsp_ClearOutportBit
* Description	: 清除输出位
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_ClearOutportBit( uint32_t bit )
{ 
	CLEAR_BIT(g_OutportShadow, bit);
} 
  
/*
*********************************************************************************************************
* Function Name : bsp_ToggleOutportBit
* Description	: 翻转输出位
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_ToggleOutportBit( uint32_t bit )
{ 
	XOR_BIT(g_OutportShadow, bit); 
} 
    
/*
*********************************************************************************************************
* Function Name : bsp_SetOutport
* Description	: 设置输出值
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_SetOutport(uint32_t out)
{ 
	WRITE_REG(g_OutportShadow, out);
	g_OutportData = ~g_OutportShadow;
}
 
/*
*********************************************************************************************************
* Function Name : bsp_GetOutport
* Description	: 获取输出值
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t bsp_GetOutport( void )
{
	/* 防止设置值后，并未执行循环，导致读取的值错误 */
	bsp_OutportCycleTask();
	
	return g_OutportData;
}

/*
*********************************************************************************************************
* Function Name : outport_task
* Description	: 端口循环输出任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_OutportCycleTask( void )
{
	uint32_t flag = g_OutportData ^ g_OutportShadow; 
	
	if ( flag )		//如果有输出需要改变，刷新输出
	{	
		if (flag & DO_BEEP)
		{
			if (g_OutportShadow & DO_BEEP)
			{
				HAL_GPIO_WritePin(BEEP_GPIO_PORT,BEEP_GPIO_PIN,GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(BEEP_GPIO_PORT,BEEP_GPIO_PIN,GPIO_PIN_RESET);
			}
		}
		
		g_OutportData = g_OutportShadow;
	}

	ECHO(DEBUG_BSP_IO,"out  : %08x H",g_OutportData);
}

/*
*********************************************************************************************************
* Function Name : bsp_InitOutport
* Description	: 初始化输出
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void bsp_InitOutport( void )
{
	bsp_Init_74HC595();
	bsp_InitBeep();
	
	g_OutportShadow = 0x00000000;
	g_OutportData = ~g_OutportShadow;
	
	bsp_OutportCycleTask();
}

/*
*********************************************************************************************************
*                              				小灯
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : bsp_InitLamp
* Description	: 小灯初始化
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void bsp_InitLamp( void )
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	LED_GPIO_CLK_ENABLE(); 
	
	GPIO_InitStructure.Pin = LED_GPIO_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;	
	HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);
	
	HAL_GPIO_WritePin(LED_GPIO_PORT,LED_GPIO_PIN,GPIO_PIN_SET);
	
	g_LampShadow = 0x00000000;
	g_LampData = ~g_LampShadow;
	
	bsp_LampCycleTask();
}

/*
*********************************************************************************************************
* Function Name : bsp_SetLampBit
* Description	: 设置小灯输出位
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_SetLampBit( uint32_t bit )
{ 
	SET_BIT(g_LampShadow, bit);
} 

/*
*********************************************************************************************************
* Function Name : bsp_ClearLampBit
* Description	: 清除小灯输出位
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_ClearLampBit( uint32_t bit )
{ 
	CLEAR_BIT(g_LampShadow, bit);
} 
  
/*
*********************************************************************************************************
* Function Name : bsp_ToggleLampBit
* Description	: 翻转小灯输出位
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_ToggleLampBit( uint32_t bit )
{ 
	XOR_BIT(g_LampShadow, bit); 
} 
    
/*
*********************************************************************************************************
* Function Name : bsp_SetLamp
* Description	: 设置小灯输出值
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_SetLamp(uint32_t out)
{ 
	WRITE_REG(g_LampShadow, out);
	g_LampData = ~g_LampShadow;
}
 
/*
*********************************************************************************************************
* Function Name : bsp_GetLamp
* Description	: 获取小灯输出值
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t bsp_GetLamp( void )
{
	/* 防止设置值后，并未执行循环，导致读取的值错误 */
	bsp_LampCycleTask();
	
	return g_LampData;
}

/*
*********************************************************************************************************
* Function Name : bsp_LampCycleTask
* Description	: 小灯循环输出任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_LampCycleTask( void )
{
	uint32_t tempu32 = g_LampData ^ g_LampShadow; 
	
	if( tempu32 )		//如果有输出需要改变，刷新输出
	{	
		if (tempu32 & LAMP_BOARD_COM)
		{
			if (g_LampShadow & LAMP_BOARD_COM)
			{
				HAL_GPIO_WritePin(LED_GPIO_PORT,LED_GPIO_PIN,GPIO_PIN_RESET);
			}
			else
			{
				HAL_GPIO_WritePin(LED_GPIO_PORT,LED_GPIO_PIN,GPIO_PIN_SET);
			}
		}
		
		/* 芯片-74HC595 */
		{
			uint32_t value = 0;
			
			if (tempu32 & LAMP_PUMP_START)
			{
				if (g_LampShadow & LAMP_PUMP_START)
				{
					SET_BIT(value, BIT_PUMP_START);
				}
			}
			
			if (tempu32 & LAMP_PUMP_STOP)
			{
				if (g_LampShadow & LAMP_PUMP_STOP)
				{
					SET_BIT(value, BIT_PUMP_STOP);
				}
			}
			
			bsp_74HC595_Write(value);
		}

		g_LampData = g_LampShadow;
	}

	ECHO(DEBUG_BSP_IO,"out  : %08x H",g_LampData);
}

/*
*********************************************************************************************************
* Function Name : bsp_InitIO
* Description	: 初始化IO
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_InitIO( void )
{
	/* 输入IO初始化 */ 
	bsp_InitInport();
	
	/* 输出IO初始化 */
	bsp_InitOutport();
	
	/* 小灯初始化 */
	bsp_InitLamp();
	
	ECHO(DEBUG_BSP_INIT, "IO初始化 .......... OK");
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
