/*
*********************************************************************************************************
* @file    	io.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-18 11:12:27
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	IO内核源文件
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
#include "kernel.h"


/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                              				Private typedef
*********************************************************************************************************
*/
struct tagIO_TypeDef {
	uint32_t inputReg;
	uint32_t outputReg;
	uint32_t lampReg;
	
	CYCLE_TASK_TypeDef beep;
};
	

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
struct tagIO_TypeDef g_IO_Device;
struct tagIO_TypeDef *g_ioDevicePtr = &g_IO_Device;

/*
*********************************************************************************************************
*                              				Private function prototypes
*********************************************************************************************************
*/
static void __IO_Task( IO_TypeDef *IO );
static void InitBeep( IO_TypeDef *IO );

/*
*********************************************************************************************************
*                              				Private functions
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : IO_Init
* Description	: IO初始化
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void IO_Init( void )
{
	InitBeep(g_ioDevicePtr);
	
	ECHO(DEBUG_KERNEL_INIT, "IO初始化 .......... OK");
}

/*
*********************************************************************************************************
* Function Name : IO_Task
* Description	: IO任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void IO_Task( void )
{
	__IO_Task(g_ioDevicePtr);
}

/*
*********************************************************************************************************
* Function Name : IO_Task
* Description	: IO任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void __IO_Task( IO_TypeDef *IO )
{
	/* 输出信号 */
	{
		#if 0
		uint32_t interfaceOutput = IO->outputReg;
		
		if ( READ_BIT(interfaceOutput,IO_OUTPUT_MASK_BEEP) )
		{
			bsp_SetOutportBit(DO_BEEP);
		}
		else
		{
			bsp_ClearOutportBit(DO_BEEP);
		}
		#endif
		
		bsp_OutportCycleTask();
	}
	
	/* 小灯信号 */
	{
		uint32_t interfaceLamp = IO->lampReg;
		
		if ( READ_BIT(interfaceLamp,IO_LAMP_MASK_COMM) )
		{
			bsp_SetLampBit(LAMP_BOARD_COM);
		}
		else
		{
			bsp_ClearLampBit(LAMP_BOARD_COM);
		}
		
		if ( READ_BIT(interfaceLamp,IO_LAMP_MASK_PUMP_OPEN) )
		{
			bsp_SetLampBit(LAMP_PUMP_START);
		}
		else
		{
			bsp_ClearLampBit(LAMP_PUMP_START);
		}
		
		if ( READ_BIT(interfaceLamp,IO_LAMP_MASK_PUMP_CLOSE) )
		{
			bsp_SetLampBit(LAMP_PUMP_STOP);
		}
		else
		{
			bsp_ClearLampBit(LAMP_PUMP_STOP);
		}
		
		bsp_LampCycleTask();
	}
}

/*
*********************************************************************************************************
* Function Name : GetIO_BeepHandle
* Description	: 获取蜂鸣器句柄
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void *GetIO_BeepHandle( IO_TypeDef *IO )
{
	return &IO->beep;
}

/*
*********************************************************************************************************
* Function Name : IO_ReadInput
* Description	: IO读输入信号
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t IO_ReadInput( IO_TypeDef *IO )
{
	return IO->inputReg;
}

/*
*********************************************************************************************************
* Function Name : IO_ReadOutput
* Description	: IO读输出信号
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t IO_ReadOutput( IO_TypeDef *IO )
{
	return IO->outputReg;
}

/*
*********************************************************************************************************
* Function Name : IO_ReadLamp
* Description	: IO读小灯信号
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t IO_ReadLamp( IO_TypeDef *IO )
{
	return IO->lampReg;
}

/*
*********************************************************************************************************
* Function Name : IO_WriteOutputMask
* Description	: IO写输出掩码
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void IO_WriteOutputMask( IO_TypeDef *IO, uint32_t clrMask, uint32_t setMask, uint32_t toggleMask )
{
	CLEAR_BIT(IO->outputReg,clrMask);
	SET_BIT(IO->outputReg,setMask);
	XOR_BIT(IO->outputReg,toggleMask);
}

/*
*********************************************************************************************************
* Function Name : IO_WriteLampMask
* Description	: IO写指示灯掩码
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void IO_WriteLampMask( IO_TypeDef *IO, uint32_t clrMask, uint32_t setMask, uint32_t toggleMask )
{
	CLEAR_BIT(IO->lampReg,clrMask);
	SET_BIT(IO->lampReg,setMask);
	XOR_BIT(IO->lampReg,toggleMask);
}

/*
*********************************************************************************************************
* Function Name : InitBeep
* Description	: 初始化蜂鸣器
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void InitBeep( IO_TypeDef *IO )
{
	CYCLE_TASK_TypeDef * const this = &IO->beep;
	
	RepeatExecuteTaskInit(this, 10, IO);
	RegisterRepeatExecuteTaskOnDevice_CallBack(this, __OpenBeep);
	RegisterRepeatExecuteTaskOffDevice_CallBack(this, __CloseBeep);
	
	ECHO(DEBUG_KERNEL_INIT, "蜂鸣器初始化 .......... OK");
}

/*
*********************************************************************************************************
* Function Name : __OpenBeep
* Description	: 打开蜂鸣器
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void __OpenBeep( void *devicePrt )
{
	/* 加快响应速度 */
	#if 0
		IO_WriteOutputMask(devicePrt,0,IO_OUTPUT_MASK_BEEP,0);
	#else
		bsp_SetOutportBit(DO_BEEP);
		bsp_OutportCycleTask();
	#endif
}

/*
*********************************************************************************************************
* Function Name : __CloseBeep
* Description	: 关闭蜂鸣器
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void __CloseBeep( void *devicePrt )
{
	#if 0
		IO_WriteOutputMask(devicePrt,IO_OUTPUT_MASK_BEEP,0,0);
	#else
		bsp_ClearOutportBit(DO_BEEP);
		bsp_OutportCycleTask();
	#endif
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
