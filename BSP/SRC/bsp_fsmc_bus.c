/*
*********************************************************************************************************
* @file    	bsp_fsmc_bus.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-19 13:20:28
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	FSMC总线驱动源文件
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
*                              				Private typedef
*********************************************************************************************************
*/

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
static SRAM_HandleTypeDef SRAM_Handle;
SRAM_HandleTypeDef *FSMC_LCD_Handle = &SRAM_Handle;


/*
*********************************************************************************************************
*                              				Private function prototypes
*********************************************************************************************************
*/
static void __bsp_FSMC_LCD_Init( SRAM_HandleTypeDef *hlcd );


/*
*********************************************************************************************************
*                              				Private functions
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : bsp_InitFSMC_Bus
* Description	: 初始化FSMC总线
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_InitFSMC_Bus( void )
{
	__bsp_FSMC_LCD_Init(&SRAM_Handle);
	
	ECHO(DEBUG_BSP_INIT, "FSMC总线初始化 .......... OK");
}

/*
*********************************************************************************************************
* Function Name : __bsp_FSMC_LCD_Init
* Description	: FSMC_LCD总线初始化
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void __bsp_FSMC_LCD_Init( SRAM_HandleTypeDef *hlcd )
{
	FSMC_NORSRAM_TimingTypeDef SRAM_Timing;
	
	hlcd->Instance  = FSMC_NORSRAM_DEVICE;
	hlcd->Extended  = FSMC_NORSRAM_EXTENDED_DEVICE;

	SRAM_Timing.AddressSetupTime       = 4;
	SRAM_Timing.AddressHoldTime        = 0;
	SRAM_Timing.DataSetupTime          = 6;
	SRAM_Timing.BusTurnAroundDuration  = 1;
	SRAM_Timing.CLKDivision            = 0;
	SRAM_Timing.DataLatency            = 0;
	SRAM_Timing.AccessMode             = FSMC_ACCESS_MODE_A;

	hlcd->Init.NSBank             = FSMC_NORSRAM_BANK1;
	hlcd->Init.DataAddressMux     = FSMC_DATA_ADDRESS_MUX_DISABLE;
	hlcd->Init.MemoryType         = FSMC_MEMORY_TYPE_SRAM;
	hlcd->Init.MemoryDataWidth    = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
	hlcd->Init.BurstAccessMode    = FSMC_BURST_ACCESS_MODE_DISABLE;
	hlcd->Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
	hlcd->Init.WrapMode           = FSMC_WRAP_MODE_DISABLE;
	hlcd->Init.WaitSignalActive   = FSMC_WAIT_TIMING_BEFORE_WS;
	hlcd->Init.WriteOperation     = FSMC_WRITE_OPERATION_ENABLE;
	hlcd->Init.WaitSignal         = FSMC_WAIT_SIGNAL_DISABLE;
	hlcd->Init.ExtendedMode       = FSMC_EXTENDED_MODE_DISABLE;
	hlcd->Init.AsynchronousWait   = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
	hlcd->Init.WriteBurst         = FSMC_WRITE_BURST_DISABLE;
	hlcd->Init.PageSize 		  = FSMC_PAGE_SIZE_NONE;

	/* Initialize the SRAM controller */
	if(HAL_SRAM_Init(hlcd, &SRAM_Timing, NULL) != HAL_OK)
	{
		/* Initialization Error */
		bsp_ErrorHandler(); 
	}
}

/*
*********************************************************************************************************
* Function Name : HAL_FSMC_LCD_MspInit
* Description	: FSMC_LCD MSP初始化
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void HAL_FSMC_LCD_MspInit(SRAM_HandleTypeDef *hsram)
{
	GPIO_InitTypeDef GPIO_Init_Structure;
	
	/** 
	 *	FSMC GPIO Configuration  
	 *	数据总线
	 =*	PD14  ------> FSMC_D0
	 =*	PD15  ------> FSMC_D1
	 =*	PD0   ------> FSMC_D2
	 =*	PD1   ------> FSMC_D3
	 *	PE7   ------> FSMC_D4
	 *	PE8   ------> FSMC_D5
	 * 	PE9   ------> FSMC_D6
	 *	PE10  ------> FSMC_D7
	 *	PE11  ------> FSMC_D8
	 *	PE12  ------> FSMC_D9
	 *	PE13  ------> FSMC_D10
	 *	PE14  ------> FSMC_D11
	 *	PE15  ------> FSMC_D12
	 =*	PD8   ------> FSMC_D13
	 =*	PD9   ------> FSMC_D14
	 =*	PD10  ------> FSMC_D15
	 *	控制总线
	 =*	PD11  ------> FSMC_A16(RS)
	 =*	PD4   ------> FSMC_NOE(读控制信号，OE = Output Enable ， N 表示低有效)
	 =*	PD5   ------> FSMC_NWE(写控制信号，WE = Output Enable ， N 表示低有效)
	 *	PD7   ------> FSMC_NE1(CS)
	 *  PD6	  ------> LCD_BUSY(忙信号)
	 *	TFT-LCD控制芯片
	 *	PB0	  ------> LCD_RST(复位信号)
	 */
	/* Enable FSMC clock */
	__HAL_RCC_FSMC_CLK_ENABLE();

	/* Enable GPIOs clock */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();

	/* Common GPIO configuration */
	GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
	GPIO_Init_Structure.Pull      = GPIO_NOPULL;
	GPIO_Init_Structure.Speed     = GPIO_SPEED_HIGH;
	GPIO_Init_Structure.Alternate = GPIO_AF12_FSMC;

	/* GPIOD configuration */
	GPIO_Init_Structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8     |\
								GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_14 | GPIO_PIN_15;

	HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

	/* GPIOE configuration */  
	GPIO_Init_Structure.Pin   = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | \
								GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);

	LCD_GPIO_CS_ENABLE();
	LCD_GPIO_BUSY_ENABLE();
	LCD_GPIO_RST_ENABLE();
	
	GPIO_Init_Structure.Pin		  = LCD_CS_GPIO_PIN;
	GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
	GPIO_Init_Structure.Pull      = GPIO_NOPULL;
	GPIO_Init_Structure.Speed     = GPIO_SPEED_HIGH;
	GPIO_Init_Structure.Alternate = GPIO_AF12_FSMC;
	HAL_GPIO_Init(LCD_CS_GPIO_PORT, &GPIO_Init_Structure);
	
	GPIO_Init_Structure.Pin		  = LCD_RST_GPIO_PIN;
	GPIO_Init_Structure.Mode      = GPIO_MODE_OUTPUT_PP;
	GPIO_Init_Structure.Pull      = GPIO_NOPULL;
	GPIO_Init_Structure.Speed     = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(LCD_RST_GPIO_PORT, &GPIO_Init_Structure);
	
	GPIO_Init_Structure.Pin		  = LCD_BUSY_GPIO_PIN;
	GPIO_Init_Structure.Mode      = GPIO_MODE_INPUT;
	GPIO_Init_Structure.Pull      = GPIO_NOPULL;
	GPIO_Init_Structure.Speed     = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(LCD_BUSY_GPIO_PORT, &GPIO_Init_Structure);
	
	/* 必须硬件复位，否则屏幕可能存在偏移 */
	HAL_GPIO_WritePin(LCD_RST_GPIO_PORT, LCD_RST_GPIO_PIN, GPIO_PIN_RESET);
	bsp_DelayMS(20);
	HAL_GPIO_WritePin(LCD_RST_GPIO_PORT, LCD_RST_GPIO_PIN, GPIO_PIN_SET);
	bsp_DelayMS(20);
}

/*
*********************************************************************************************************
* Function Name : HAL_SRAM_MspInit
* Description	: SRAM MSP初始化
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
{
	HAL_FSMC_LCD_MspInit(hsram);
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
