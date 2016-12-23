/*
*********************************************************************************************************
* @file    	bsp_i2c_bus.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-18 15:21:26
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	I2C总线源文件
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
#define I2C_TIMEOUT_MAX						3000

/* I2C-PCF8563 */
#define I2C1_ADDRESS        				0xA2	
#define I2C1_CLK_ENABLE()                	__HAL_RCC_I2C1_CLK_ENABLE()
#define I2C1_SDA_GPIO_CLK_ENABLE()       	__HAL_RCC_GPIOB_CLK_ENABLE()
#define I2C1_SCL_GPIO_CLK_ENABLE()       	__HAL_RCC_GPIOB_CLK_ENABLE() 		
#define I2C1_SCL_PIN                    	GPIO_PIN_6
#define I2C1_SCL_GPIO_PORT              	GPIOB
#define I2C1_SCL_AF                     	GPIO_AF4_I2C1
#define I2C1_SDA_PIN                    	GPIO_PIN_7
#define I2C1_SDA_GPIO_PORT             	 	GPIOB
#define I2C1_SDA_AF                     	GPIO_AF4_I2C1


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
static I2C_HandleTypeDef I2C1_Handle;
I2C_HandleTypeDef *I2C_PCF8563_Handle = &I2C1_Handle;

/*
*********************************************************************************************************
*                              				Private function prototypes
*********************************************************************************************************
*/
static void __bsp_I2C1_Init( I2C_HandleTypeDef *handle );



/*
*********************************************************************************************************
*                              				Private functions
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : bsp_InitI2C_Bus
* Description	: 初始化I2C总线
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_InitI2C_Bus( void )
{
	__bsp_I2C1_Init(&I2C1_Handle);
	
	ECHO(DEBUG_BSP_INIT, "I2C总线初始化 .......... OK");
}

/*
*********************************************************************************************************
* Function Name : __bsp_I2C1_Init
* Description	: 初始化I2C1
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void __bsp_I2C1_Init( I2C_HandleTypeDef *handle )
{
	handle->Instance             = I2C1;
  
	handle->Init.AddressingMode  = I2C_ADDRESSINGMODE_10BIT;
	handle->Init.ClockSpeed      = 400000;
	handle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	handle->Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
	handle->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	handle->Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
	handle->Init.OwnAddress1     = I2C1_ADDRESS;
	handle->Init.OwnAddress2     = 0xFE;

	if (HAL_I2C_Init(handle) != HAL_OK)
	{
		/* Initialization Error */
		bsp_ErrorHandler();    
	}
}


/*
*********************************************************************************************************
* Function Name : bsp_I2C_Write
* Description	: 向I2C设备写寄存器
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_I2C_WriteReg( I2C_HandleTypeDef *handle, uint16_t regAddress, uint8_t *pData, uint16_t size )
{
	while(HAL_I2C_Mem_Write(handle, handle->Init.OwnAddress1, regAddress, I2C_MEMADD_SIZE_8BIT,\
			pData, size, I2C_TIMEOUT_MAX)!= HAL_OK)
	{
		/* Error_Handler() function is called when Timeout error occurs.
		   When Acknowledge failure occurs (Slave don't acknowledge it's address)
		   Master restarts communication */
		if (HAL_I2C_GetError(handle) != HAL_I2C_ERROR_AF)
		{
			bsp_ErrorHandler();
		}
	}
}

/*
*********************************************************************************************************
* Function Name : bsp_I2C_ReadReg
* Description	: 从I2C设备读寄存器
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_I2C_ReadReg( I2C_HandleTypeDef *handle, uint16_t regAddress, uint8_t *pData, uint16_t size )
{
	while(HAL_I2C_Mem_Read(handle, handle->Init.OwnAddress1, regAddress, I2C_MEMADD_SIZE_8BIT,\
			pData, size, I2C_TIMEOUT_MAX)!= HAL_OK)
	{
		/* Error_Handler() function is called when Timeout error occurs.
		   When Acknowledge failure occurs (Slave don't acknowledge it's address)
		   Master restarts communication */
		if (HAL_I2C_GetError(handle) != HAL_I2C_ERROR_AF)
		{
			bsp_ErrorHandler();
		}   
	}
}

/*
*********************************************************************************************************
*                              				MSP
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : HAL_I2C_MspInit
* Description	: I2C MSP初始化
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{  
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	if (hi2c->Instance == I2C1)
	{
		/*##-1- Enable GPIO Clocks #################################################*/
		/* Enable GPIO TX/RX clock */
		I2C1_SCL_GPIO_CLK_ENABLE();
		I2C1_SDA_GPIO_CLK_ENABLE();

		/*##-2- Configure peripheral GPIO ##########################################*/  
		/* I2C TX GPIO pin configuration  */
		GPIO_InitStruct.Pin       = I2C1_SCL_PIN;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull      = GPIO_PULLUP;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
		GPIO_InitStruct.Alternate = I2C1_SCL_AF;
		HAL_GPIO_Init(I2C1_SCL_GPIO_PORT, &GPIO_InitStruct);

		/* I2C RX GPIO pin configuration  */
		GPIO_InitStruct.Pin = I2C1_SDA_PIN;
		GPIO_InitStruct.Alternate = I2C1_SDA_AF;
		HAL_GPIO_Init(I2C1_SDA_GPIO_PORT, &GPIO_InitStruct);

		/*##-3- Enable I2C peripheral Clock ########################################*/
		/* Enable I2C1 clock */
		I2C1_CLK_ENABLE(); 
	}
}


/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
