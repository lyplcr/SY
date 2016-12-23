/*
*********************************************************************************************************
* @file    	bsp_spi_bus.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-18 19:21:58
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	SPI总线源文件
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
#define SPI_TIMEOUT_MAX						3000

/* SPI-FLASH */
#define SPI2_CLK_ENABLE()                	__HAL_RCC_SPI2_CLK_ENABLE()
#define SPI2_CS_GPIO_CLK_ENABLE()        	__HAL_RCC_GPIOC_CLK_ENABLE()
#define SPI2_SCK_GPIO_CLK_ENABLE()       	__HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI2_MISO_GPIO_CLK_ENABLE()      	__HAL_RCC_GPIOC_CLK_ENABLE() 
#define SPI2_MOSI_GPIO_CLK_ENABLE()     	 __HAL_RCC_GPIOC_CLK_ENABLE() 

#define SPI2_CS_PIN                      	GPIO_PIN_0
#define SPI2_CS_GPIO_PORT                	GPIOC
#define SPI2_SCK_PIN                     	GPIO_PIN_10
#define SPI2_SCK_GPIO_PORT               	GPIOB
#define SPI2_SCK_AF                      	GPIO_AF5_SPI2
#define SPI2_MISO_PIN                    	GPIO_PIN_2
#define SPI2_MISO_GPIO_PORT              	GPIOC
#define SPI2_MISO_AF                     	GPIO_AF5_SPI2
#define SPI2_MOSI_PIN                    	GPIO_PIN_3
#define SPI2_MOSI_GPIO_PORT              	GPIOC
#define SPI2_MOSI_AF                     	GPIO_AF5_SPI2


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
static SPI_HandleTypeDef SPI2_Handle;
SPI_HandleTypeDef *SPI_FLASH_Handle = &SPI2_Handle;


/*
*********************************************************************************************************
*                              				Private function prototypes
*********************************************************************************************************
*/
static void __bsp_SPI2_Init( SPI_HandleTypeDef *handle );


/*
*********************************************************************************************************
*                              				Private functions
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : bsp_InitSPI_Bus
* Description	: 初始化SPI总线
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_InitSPI_Bus( void )
{
	__bsp_SPI2_Init(&SPI2_Handle);
	
	ECHO(DEBUG_BSP_INIT, "SPI总线初始化 .......... OK");
}

/*
*********************************************************************************************************
* Function Name : __bsp_SPI2_Init
* Description	: 初始化SPI2
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void __bsp_SPI2_Init( SPI_HandleTypeDef *handle )
{
	/*
		STM32F4XX 时钟计算.
			HCLK = 168M
			PCLK1 = HCLK / 4 = 42M
			PCLK2 = HCLK / 2 = 84M

			SPI2、SPI3 在 PCLK1, 时钟42M
			SPI1       在 PCLK2, 时钟84M

			STM32F4 支持的最大SPI时钟为 37.5 Mbits/S, 因此需要分频。
	*/
	/*##-1- Configure the SPI peripheral #######################################*/
	/* Set the SPI parameters */
	handle->Instance               = SPI2;
	handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	handle->Init.Direction         = SPI_DIRECTION_2LINES;
	handle->Init.CLKPhase          = SPI_PHASE_2EDGE;
	handle->Init.CLKPolarity       = SPI_POLARITY_LOW;
	handle->Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	handle->Init.CRCPolynomial     = 7;
	handle->Init.DataSize          = SPI_DATASIZE_8BIT;
	handle->Init.FirstBit          = SPI_FIRSTBIT_MSB;
	handle->Init.NSS               = SPI_NSS_SOFT;
	handle->Init.TIMode            = SPI_TIMODE_DISABLE;
	handle->Init.Mode = SPI_MODE_MASTER;

	if(HAL_SPI_Init(handle) != HAL_OK)
	{
		/* Initialization Error */
		bsp_ErrorHandler();
	}
}

/*
*********************************************************************************************************
* Function Name : bsp_SPI_ReadWrite
* Description	: 通过SPI读写数据
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_SPI_ReadWrite( SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size )
{
	/* Send Master READ command to slave */
	if(HAL_SPI_TransmitReceive(hspi, pTxData, pRxData, Size, SPI_TIMEOUT_MAX) != HAL_OK)
	{
		bsp_ErrorHandler();
	}
}

/*
*********************************************************************************************************
* Function Name : SPI_CS
* Description	: 设置SPI片选
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void SPI_CS( SPI_HandleTypeDef *hspi, FlagStatus status )
{
	if (hspi->Instance == SPI2)
	{
		if (status == SET)
		{
			HAL_GPIO_WritePin(SPI2_CS_GPIO_PORT,SPI2_CS_PIN,GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(SPI2_CS_GPIO_PORT,SPI2_CS_PIN,GPIO_PIN_RESET);
		}
	}
}

/*
*********************************************************************************************************
* Function Name : HAL_SPI_MspInit
* Description	: SPI MSP初始化
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	if (hspi->Instance == SPI2)
	{
		/*##-1- Enable GPIO Clocks #################################################*/
		/* Enable GPIO TX/RX clock */
		SPI2_CS_GPIO_CLK_ENABLE();
		SPI2_SCK_GPIO_CLK_ENABLE();
		SPI2_MISO_GPIO_CLK_ENABLE();
		SPI2_MOSI_GPIO_CLK_ENABLE();

		/*##-2- Configure peripheral GPIO ##########################################*/  
		/* SPI SCK GPIO pin configuration  */
		GPIO_InitStruct.Pin       = SPI2_SCK_PIN;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
		GPIO_InitStruct.Alternate = SPI2_SCK_AF;
		HAL_GPIO_Init(SPI2_SCK_GPIO_PORT, &GPIO_InitStruct);

		/* SPI MISO GPIO pin configuration  */
		GPIO_InitStruct.Pull      = GPIO_PULLUP;
		GPIO_InitStruct.Pin       = SPI2_MISO_PIN;
		HAL_GPIO_Init(SPI2_MISO_GPIO_PORT, &GPIO_InitStruct);

		/* SPI MOSI GPIO pin configuration  */
		GPIO_InitStruct.Pin       = SPI2_MOSI_PIN;
		HAL_GPIO_Init(SPI2_MOSI_GPIO_PORT, &GPIO_InitStruct);
		
		GPIO_InitStruct.Pin       = SPI2_CS_PIN;
		GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull      = GPIO_NOPULL;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
		HAL_GPIO_Init(SPI2_CS_GPIO_PORT, &GPIO_InitStruct);

		/*##-3- Enable SPI peripheral Clock ########################################*/
		SPI2_CLK_ENABLE(); 
	}
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
