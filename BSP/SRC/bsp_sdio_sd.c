/*
*********************************************************************************************************
* @file    	bsp_sdio_sd.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-20 19:41:05
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	SD卡源文件
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
#if (USE_SD_DETECT == 0x01U)
	#define SD_DETECT_PIN                    	GPIO_PIN_13
	#define SD_DETECT_GPIO_PORT              	GPIOH
	#define __SD_DETECT_GPIO_CLK_ENABLE()    	__GPIOH_CLK_ENABLE()
	#define SD_DETECT_IRQn                   	EXTI15_10_IRQn
#endif

#define SD_DATATIMEOUT           			((uint32_t)100000000)

/* DMA definitions for SD DMA transfer */
#define __DMAx_TxRx_CLK_ENABLE           	__DMA2_CLK_ENABLE
#define SD_DMAx_Tx_CHANNEL                	DMA_CHANNEL_4
#define SD_DMAx_Rx_CHANNEL                	DMA_CHANNEL_4
#define SD_DMAx_Tx_STREAM                 	DMA2_Stream6  
#define SD_DMAx_Rx_STREAM                 	DMA2_Stream3  
#define SD_DMAx_Tx_IRQn                   	DMA2_Stream6_IRQn
#define SD_DMAx_Rx_IRQn                   	DMA2_Stream3_IRQn
#define SD_DMAx_Tx_IRQHandler             	DMA2_Stream6_IRQHandler
#define SD_DMAx_Rx_IRQHandler             	DMA2_Stream3_IRQHandler
#if (USE_SD_DETECT == 0x01U)
	#define SD_DetectIRQHandler()             	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13)
#endif



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
static SD_HandleTypeDef uSdHandle;
static HAL_SD_CardInfoTypedef uSdCardInfo;

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
* Function Name : BSP_SD_Init
* Description	: 初始化SD卡
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint8_t BSP_SD_Init( void )
{ 
	uint8_t SD_state = MSD_OK;

	/* uSD device interface configuration */
	uSdHandle.Instance = SDIO;

	uSdHandle.Init.ClockEdge           = SDIO_CLOCK_EDGE_RISING;
	uSdHandle.Init.ClockBypass         = SDIO_CLOCK_BYPASS_DISABLE;
	uSdHandle.Init.ClockPowerSave      = SDIO_CLOCK_POWER_SAVE_DISABLE;
	uSdHandle.Init.BusWide             = SDIO_BUS_WIDE_1B;
	uSdHandle.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
	uSdHandle.Init.ClockDiv            = SDIO_TRANSFER_CLK_DIV;

#if (USE_SD_DETECT == 0x01U)
	/* Check if the SD card is plugged in the slot */
	if(BSP_SD_IsDetected() != SD_PRESENT)
	{
		return MSD_ERROR;
	}
#endif

	/* HAL SD initialization */
	if(HAL_SD_Init(&uSdHandle, &uSdCardInfo) != SD_OK)
	{
		SD_state = MSD_ERROR;
	}

	/* Configure SD Bus width */
	if(SD_state == MSD_OK)
	{
		/* Enable wide operation */
		if(HAL_SD_WideBusOperation_Config(&uSdHandle, SDIO_BUS_WIDE_4B) != SD_OK)
		{
		  SD_state = MSD_ERROR;
		}
		else
		{
		  SD_state = MSD_OK;
		}
	}
	
	ECHO(DEBUG_BSP_INIT, "SD卡初始化 .......... OK");

	return SD_state;
}

/*
*********************************************************************************************************
* Function Name : HAL_SD_MspInit
* Description	: SD卡MSP初始化
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
	static DMA_HandleTypeDef dmaRxHandle;
	static DMA_HandleTypeDef dmaTxHandle;
	GPIO_InitTypeDef GPIO_Init_Structure;

	if (hsd->Instance == SDIO)
	{
		/* Enable SDIO clock */
		__SDIO_CLK_ENABLE();

		/* Enable DMA2 clocks */
		__DMAx_TxRx_CLK_ENABLE();

		/* Enable GPIOs clock */
		__GPIOC_CLK_ENABLE();
		__GPIOD_CLK_ENABLE();

	#if (USE_SD_DETECT == 0x01U)
		__SD_DETECT_GPIO_CLK_ENABLE();
	#endif
		
		/* Common GPIO configuration */
		GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
		GPIO_Init_Structure.Pull      = GPIO_PULLUP;
		GPIO_Init_Structure.Speed     = GPIO_SPEED_HIGH;
		GPIO_Init_Structure.Alternate = GPIO_AF12_SDIO;

		/* GPIOC configuration */
		GPIO_Init_Structure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;

		HAL_GPIO_Init(GPIOC, &GPIO_Init_Structure);

		/* GPIOD configuration */
		GPIO_Init_Structure.Pin = GPIO_PIN_2;
		HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

	#if (USE_SD_DETECT == 0x01U)
		/* SD Card detect pin configuration */
		GPIO_Init_Structure.Mode      = GPIO_MODE_INPUT;
		GPIO_Init_Structure.Pull      = GPIO_PULLUP;
		GPIO_Init_Structure.Speed     = GPIO_SPEED_HIGH;
		GPIO_Init_Structure.Pin       = SD_DETECT_PIN;
		HAL_GPIO_Init(SD_DETECT_GPIO_PORT, &GPIO_Init_Structure);
	#endif
	
		dmaRxHandle.Instance = SD_DMAx_Rx_STREAM;
		
		/* Configure DMA Rx parameters */
		dmaRxHandle.Init.Channel             = SD_DMAx_Rx_CHANNEL;
		dmaRxHandle.Init.Direction           = DMA_PERIPH_TO_MEMORY;
		dmaRxHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
		dmaRxHandle.Init.MemInc              = DMA_MINC_ENABLE;
		dmaRxHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
		dmaRxHandle.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
		dmaRxHandle.Init.Mode                = DMA_PFCTRL;
		dmaRxHandle.Init.Priority            = DMA_PRIORITY_LOW;
		dmaRxHandle.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
		dmaRxHandle.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
		dmaRxHandle.Init.MemBurst            = DMA_MBURST_INC4;
		dmaRxHandle.Init.PeriphBurst         = DMA_PBURST_INC4;

		/* Associate the DMA handle */
		__HAL_LINKDMA(hsd, hdmarx, dmaRxHandle);

		/* Deinitialize the stream for new transfer */
		HAL_DMA_DeInit(&dmaRxHandle);

		/* Configure the DMA stream */
		HAL_DMA_Init(&dmaRxHandle);
		
		dmaTxHandle.Instance = SD_DMAx_Tx_STREAM;

		/* Configure DMA Tx parameters */
		dmaTxHandle.Init.Channel             = SD_DMAx_Tx_CHANNEL;
		dmaTxHandle.Init.Direction           = DMA_MEMORY_TO_PERIPH;
		dmaTxHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
		dmaTxHandle.Init.MemInc              = DMA_MINC_ENABLE;
		dmaTxHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
		dmaTxHandle.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
		dmaTxHandle.Init.Mode                = DMA_PFCTRL;
		dmaTxHandle.Init.Priority            = DMA_PRIORITY_LOW;
		dmaTxHandle.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
		dmaTxHandle.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
		dmaTxHandle.Init.MemBurst            = DMA_MBURST_INC4;
		dmaTxHandle.Init.PeriphBurst         = DMA_PBURST_INC4;

		/* Associate the DMA handle */
		__HAL_LINKDMA(hsd, hdmatx, dmaTxHandle);

		/* Deinitialize the stream for new transfer */
		HAL_DMA_DeInit(&dmaTxHandle);

		/* Configure the DMA stream */
		HAL_DMA_Init(&dmaTxHandle); 
		
		/* NVIC configuration for SDIO interrupts */
		HAL_NVIC_SetPriority(SDIO_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(SDIO_IRQn);

		/* NVIC configuration for DMA transfer complete interrupt */
		HAL_NVIC_SetPriority(SD_DMAx_Rx_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(SD_DMAx_Rx_IRQn);

		/* NVIC configuration for DMA transfer complete interrupt */
		HAL_NVIC_SetPriority(SD_DMAx_Tx_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(SD_DMAx_Tx_IRQn);
	}
}

/**
  * @brief  This function handles DMA2 Stream 3 interrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Stream3_IRQHandler(void)
{
#if (uCOS_EN)	
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
	OSIntEnter(); 
	CPU_CRITICAL_EXIT();
#endif
	
	BSP_SD_DMA_Rx_IRQHandler();
	
#if (uCOS_EN)
	OSIntExit();
#endif		
}

/**
  * @brief  This function handles DMA2 Stream 6 interrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Stream6_IRQHandler(void)
{
#if (uCOS_EN)	
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
	OSIntEnter(); 
	CPU_CRITICAL_EXIT();
#endif
	
	BSP_SD_DMA_Tx_IRQHandler(); 
	
#if (uCOS_EN)
	OSIntExit();
#endif		
}

/**
  * @brief  This function handles SDIO interrupt request.
  * @param  None
  * @retval None
  */
void SDIO_IRQHandler(void)
{
#if (uCOS_EN)	
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
	OSIntEnter(); 
	CPU_CRITICAL_EXIT();
#endif	
		
	BSP_SD_IRQHandler();
	
#if (uCOS_EN)
	OSIntExit();
#endif	
}

#if (USE_SD_DETECT == 0x01U)
/**
  * @brief  Configures Interrupt mode for SD detection pin.
  * @retval Returns 0
  */
uint8_t BSP_SD_ITConfig(void)
{ 
  GPIO_InitTypeDef GPIO_Init_Structure;
  
  /* Configure Interrupt mode for SD detection pin */ 
  GPIO_Init_Structure.Mode      = GPIO_MODE_IT_RISING_FALLING;
  GPIO_Init_Structure.Pull      = GPIO_PULLUP;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_HIGH;
  GPIO_Init_Structure.Pin       = SD_DETECT_PIN;
  HAL_GPIO_Init(SD_DETECT_GPIO_PORT, &GPIO_Init_Structure);
    
  /* NVIC configuration for SDIO interrupts */
  HAL_NVIC_SetPriority(SD_DETECT_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(SD_DETECT_IRQn);
  
  return 0;
}

/**
 * @brief  Detects if SD card is correctly plugged in the memory slot or not.
 * @retval Returns if SD is detected or not
 */
uint8_t BSP_SD_IsDetected(void)
{
  __IO uint8_t status = SD_PRESENT;

  /* Check SD card detect pin */
  if(HAL_GPIO_ReadPin(SD_DETECT_GPIO_PORT, SD_DETECT_PIN) != GPIO_PIN_RESET) 
  {
    status = SD_NOT_PRESENT;
  }
  
  return status;
}

/** @brief  SD detect IT treatment
  */
void BSP_SD_DetectIT(void)
{
  /* SD detect IT callback */
  BSP_SD_DetectCallback();
  
}


/** @brief  SD detect IT detection callback
  */
__weak void BSP_SD_DetectCallback(void)
{
  /* NOTE: This function Should not be modified, when the callback is needed,
  the BSP_SD_DetectCallback could be implemented in the user file
  */ 
  
}
#endif

/**
  * @brief  Reads block(s) from a specified address in an SD card, in polling mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  ReadAddr: Address from where data is to be read  
  * @param  BlockSize: SD card data block size, that should be 512
  * @param  NumOfBlocks: Number of SD blocks to read 
  * @retval SD status
  */
uint8_t BSP_SD_ReadBlocks(uint32_t *pData, uint64_t ReadAddr, uint32_t BlockSize, uint32_t NumOfBlocks)
{
  if(HAL_SD_ReadBlocks(&uSdHandle, pData, ReadAddr, BlockSize, NumOfBlocks) != SD_OK)
  {
    return MSD_ERROR;
  }
  else
  {
    return MSD_OK;
  }
}

/**
  * @brief  Writes block(s) to a specified address in an SD card, in polling mode. 
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  WriteAddr: Address from where data is to be written  
  * @param  BlockSize: SD card data block size, that should be 512
  * @param  NumOfBlocks: Number of SD blocks to write
  * @retval SD status
  */
uint8_t BSP_SD_WriteBlocks(uint32_t *pData, uint64_t WriteAddr, uint32_t BlockSize, uint32_t NumOfBlocks)
{
  if(HAL_SD_WriteBlocks(&uSdHandle, pData, WriteAddr, BlockSize, NumOfBlocks) != SD_OK)
  {
    return MSD_ERROR;
  }
  else
  {
    return MSD_OK;
  }
}

/**
  * @brief  Reads block(s) from a specified address in an SD card, in DMA mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  ReadAddr: Address from where data is to be read  
  * @param  BlockSize: SD card data block size, that should be 512
  * @param  NumOfBlocks: Number of SD blocks to read 
  * @retval SD status
  */
uint8_t BSP_SD_ReadBlocks_DMA(uint32_t *pData, uint64_t ReadAddr, uint32_t BlockSize, uint32_t NumOfBlocks)
{
  uint8_t SD_state = MSD_OK;
  
  /* Read block(s) in DMA transfer mode */
  if(HAL_SD_ReadBlocks_DMA(&uSdHandle, pData, ReadAddr, BlockSize, NumOfBlocks) != SD_OK)
  {
    SD_state = MSD_ERROR;
  }
  
  /* Wait until transfer is complete */
  if(SD_state == MSD_OK)
  {
    if(HAL_SD_CheckReadOperation(&uSdHandle, (uint32_t)SD_DATATIMEOUT) != SD_OK)
    {
      SD_state = MSD_ERROR;
    }
    else
    {
      SD_state = MSD_OK;
    }
  }
  
  return SD_state; 
}

/**
  * @brief  Writes block(s) to a specified address in an SD card, in DMA mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  WriteAddr: Address from where data is to be written  
  * @param  BlockSize: SD card data block size, that should be 512
  * @param  NumOfBlocks: Number of SD blocks to write 
  * @retval SD status
  */
uint8_t BSP_SD_WriteBlocks_DMA(uint32_t *pData, uint64_t WriteAddr, uint32_t BlockSize, uint32_t NumOfBlocks)
{
  uint8_t SD_state = MSD_OK;
  
  /* Write block(s) in DMA transfer mode */
  if(HAL_SD_WriteBlocks_DMA(&uSdHandle, pData, WriteAddr, BlockSize, NumOfBlocks) != SD_OK)
  {
    SD_state = MSD_ERROR;
  }
  
  /* Wait until transfer is complete */
  if(SD_state == MSD_OK)
  {
    if(HAL_SD_CheckWriteOperation(&uSdHandle, (uint32_t)SD_DATATIMEOUT) != SD_OK)
    {
      SD_state = MSD_ERROR;
    }
    else
    {
      SD_state = MSD_OK;
    }
  }
  
  return SD_state;  
}

/**
  * @brief  Erases the specified memory area of the given SD card. 
  * @param  StartAddr: Start byte address
  * @param  EndAddr: End byte address
  * @retval SD status
  */
uint8_t BSP_SD_Erase(uint64_t StartAddr, uint64_t EndAddr)
{
  if(HAL_SD_Erase(&uSdHandle, StartAddr, EndAddr) != SD_OK)
  {
    return MSD_ERROR;
  }
  else
  {
    return MSD_OK;
  }
}

/**
  * @brief  Handles SD card interrupt request.
  */
void BSP_SD_IRQHandler(void)
{
  HAL_SD_IRQHandler(&uSdHandle);
}

/**
  * @brief  Handles SD DMA Tx transfer interrupt request.
  */
void BSP_SD_DMA_Tx_IRQHandler(void)
{
  HAL_DMA_IRQHandler(uSdHandle.hdmatx); 
}

/**
  * @brief  Handles SD DMA Rx transfer interrupt request.
  */
void BSP_SD_DMA_Rx_IRQHandler(void)
{
  HAL_DMA_IRQHandler(uSdHandle.hdmarx);
}

/**
  * @brief  Gets the current SD card data status.
  * @retval Data transfer state.
  *          This value can be one of the following values:
  *            @arg  SD_TRANSFER_OK: No data transfer is acting
  *            @arg  SD_TRANSFER_BUSY: Data transfer is acting
  *            @arg  SD_TRANSFER_ERROR: Data transfer error 
  */
HAL_SD_TransferStateTypedef BSP_SD_GetStatus(void)
{
  return(HAL_SD_GetStatus(&uSdHandle));
}

/**
  * @brief  Get SD information about specific SD card.
  * @param  CardInfo: Pointer to HAL_SD_CardInfoTypedef structure
  */
void BSP_SD_GetCardInfo(HAL_SD_CardInfoTypedef *CardInfo)
{
  /* Get SD card Information */
  HAL_SD_Get_CardInfo(&uSdHandle, CardInfo);
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
