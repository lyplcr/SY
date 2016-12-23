/*
*********************************************************************************************************
* @file    	bsp_network.h
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-24 16:43:34
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	����ͷ�ļ�
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
#ifndef __BSP_NETWORK_H
#define __BSP_NETWORK_H

/*
*********************************************************************************************************
*                              				Exported Includes
*********************************************************************************************************
*/
#include "lwip/opt.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip/lwip_timers.h"
#include "netif/etharp.h"
#include "utils.h"

/*
*********************************************************************************************************
*                              				Exported define
*********************************************************************************************************
*/
#define ETH_RX_BUFF_SIZE					(1024*2)
#define ETH_MAX_POOL_SIZE					(TCP_MSS)

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
extern struct netif g_Netif_LAN8720;

/*
*********************************************************************************************************
*                              				Exported functions
*********************************************************************************************************
*/
void bsp_InitNetwork( void );
void Netif_ConfigIP(ip_addr_t *ipaddr);
void Netif_ConfigNETMASK(ip_addr_t *netmask);
void Netif_ConfigGW_ADDR(ip_addr_t *gw);
void Netif_ConfigMAC_ADDR(uint8_t *addr);
bool BSP_ETH_IsDetected( ETH_HandleTypeDef *heth );
uint32_t ETH_GetPackageBody(struct pbuf *p, __IO uint8_t *pBuff);

#endif
/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/