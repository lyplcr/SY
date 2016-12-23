/*
*********************************************************************************************************
* @file    	bsp_sdio_sd.h
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-20 19:41:05
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	SD¿¨Í·ÎÄ¼þ
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
#ifndef __BSP_SDIO_SD_H
#define __BSP_SDIO_SD_H

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
#define MSD_OK         						0x00
#define MSD_ERROR     				 		0x01

#define SD_PRESENT               			((uint8_t)0x01)
#define SD_NOT_PRESENT           			((uint8_t)0x00)

#define USE_SD_DETECT						0x00U

/*
*********************************************************************************************************
*                              				Exported types
*********************************************************************************************************
*/
#ifndef SD_CardInfo
  #define SD_CardInfo HAL_SD_CardInfoTypedef
#endif 

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

/*
*********************************************************************************************************
*                              				Exported functions
*********************************************************************************************************
*/
uint8_t BSP_SD_Init( void );
uint8_t BSP_SD_ITConfig(void);
#if (USE_SD_DETECT == 0x01U)
void    BSP_SD_DetectIT(void);
void    BSP_SD_DetectCallback(void);
#endif
uint8_t BSP_SD_ReadBlocks(uint32_t *pData, uint64_t ReadAddr, uint32_t BlockSize, uint32_t NumOfBlocks);
uint8_t BSP_SD_WriteBlocks(uint32_t *pData, uint64_t WriteAddr, uint32_t BlockSize, uint32_t NumOfBlocks);
uint8_t BSP_SD_ReadBlocks_DMA(uint32_t *pData, uint64_t ReadAddr, uint32_t BlockSize, uint32_t NumOfBlocks);
uint8_t BSP_SD_WriteBlocks_DMA(uint32_t *pData, uint64_t WriteAddr, uint32_t BlockSize, uint32_t NumOfBlocks);
uint8_t BSP_SD_Erase(uint64_t StartAddr, uint64_t EndAddr);
void    BSP_SD_IRQHandler(void);
void    BSP_SD_DMA_Tx_IRQHandler(void);
void    BSP_SD_DMA_Rx_IRQHandler(void);
HAL_SD_TransferStateTypedef BSP_SD_GetStatus(void);
void    BSP_SD_GetCardInfo(HAL_SD_CardInfoTypedef *CardInfo);
uint8_t BSP_SD_IsDetected(void);   

#endif
/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
