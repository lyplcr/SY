/*
*********************************************************************************************************
* @file    	bsp_network.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-24 16:43:34
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	网络源文件
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
#include "ethernetif.h"
#include "lwip/netif.h"
#include "UDP_Server.h"
#include "SeqQueue.h"

/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/
#define IP_ADDR0   							192
#define IP_ADDR1   							168
#define IP_ADDR2   							0
#define IP_ADDR3   							234
   
/*NETMASK*/
#define NETMASK_ADDR0   					255
#define NETMASK_ADDR1   					255
#define NETMASK_ADDR2   					255
#define NETMASK_ADDR3   					0

/*Gateway Address*/
#define GW_ADDR0   							192
#define GW_ADDR1   							168
#define GW_ADDR2   							0
#define GW_ADDR3  							1


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
struct netif g_Netif_LAN8720;
SWITCH_TASK_TypeDef g_ETH_Switch;
SEQUEUE_TypeDef g_ETH_RxQueue;
static uint8_t ETH_RxBuff[ETH_RX_BUFF_SIZE];

/*
*********************************************************************************************************
*                              				Private function prototypes
*********************************************************************************************************
*/
static void Netif_Config(void);
static void ETH_ConnectionSwitchTaskInit( SWITCH_TASK_TypeDef *switchPtr );

/*
*********************************************************************************************************
*                              				Private functions
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : bsp_InitNetwork
* Description	: 初始化以太网
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_InitNetwork( void )
{
	/* Initialize the LwIP stack */
	lwip_init();
	
	/* Configure the Network interface */
	Netif_Config(); 
	
	/* 以太网连接开关任务 */
	ETH_ConnectionSwitchTaskInit(&g_ETH_Switch);
	
	/* 接收队列初始化 */
	CreateSeqQueue(&g_ETH_RxQueue, ETH_RxBuff, ETH_RX_BUFF_SIZE);
	
	/* UDP服务器 */
	UDP_ServerInit();
	
	ECHO(DEBUG_BSP_INIT, "以太网初始化 .......... OK");
}

/*
*********************************************************************************************************
* Function Name : Netif_Config
* Description	: 网卡配置
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void Netif_Config(void)
{
	ip_addr_t ipaddr;
	ip_addr_t netmask;
	ip_addr_t gw;

	Netif_ConfigIP(&ipaddr);
	Netif_ConfigNETMASK(&netmask);
	Netif_ConfigGW_ADDR(&gw);

	/* add the network interface */    
	netif_add(&g_Netif_LAN8720, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

	/*  Registers the default network interface */
	netif_set_default(&g_Netif_LAN8720);

	/**
	 *	暂时不需要处理，后面会在 SwitchTask(&g_ETH_Switch);中自动处理。
	 */
#if 0
	if (netif_is_link_up(&g_Netif_LAN8720))
	{
		/* When the netif is fully configured this function must be called */
		netif_set_up(&g_Netif_LAN8720);
		ECHO(DEBUG_BSP_INIT, "初始化时网线已连接！");
	}
	else
	{
		/* When the netif link is down this function must be called */
		netif_set_down(&g_Netif_LAN8720);
		ECHO(DEBUG_BSP_INIT, "初始化网线未连接！");
	}
#endif

	/* Set the link callback function, this function is called on change of link status*/
	netif_set_link_callback(&g_Netif_LAN8720, ethernetif_update_config);
}

/*
*********************************************************************************************************
* Function Name : ETH_GetPackageBody
* Description	: 从网卡获取数据包
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t ETH_GetPackageBody(struct pbuf *p, __IO uint8_t *pBuff)
{
	__IO uint8_t *pdata = NULL;
	struct pbuf *pFrame = p;
	uint32_t cur_len = 0;
	uint32_t tot_len = pFrame->tot_len;
	
	if (tot_len <= ETH_MAX_POOL_SIZE)
	{
		for (pdata=pBuff; cur_len!=tot_len; pFrame=pFrame->next)
		{
			memcpy((void *)pdata,pFrame->payload,pFrame->len);
			pdata += pFrame->len;
			cur_len += pFrame->len;
		}
	}
	
	return cur_len;
}

/*
*********************************************************************************************************
* Function Name : Netif_ConfigIP
* Description	: 配置网卡IP
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void Netif_ConfigIP(ip_addr_t *ipaddr)
{
	IP4_ADDR(ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
}

/*
*********************************************************************************************************
* Function Name : Netif_ConfigNETMASK
* Description	: 配置网卡子网掩码
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void Netif_ConfigNETMASK(ip_addr_t *netmask)
{
	IP4_ADDR(netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
}

/*
*********************************************************************************************************
* Function Name : Netif_ConfigGW_ADDR
* Description	: 配置网卡网关地址
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void Netif_ConfigGW_ADDR(ip_addr_t *gw)
{
	IP4_ADDR(gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3); 
}

/*
*********************************************************************************************************
* Function Name : Netif_ConfigMAC_ADDR
* Description	: 配置网卡物理地址
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void Netif_ConfigMAC_ADDR(uint8_t *addr)
{
	addr[0] = MAC_ADDR0;
	addr[1] = MAC_ADDR1;
	addr[2] = MAC_ADDR2;
	addr[3] = MAC_ADDR3;
	addr[4] = MAC_ADDR4;
	addr[5] = MAC_ADDR5;
}

#if 0
/*
*********************************************************************************************************
* Function Name : ETH_IRQHandler
* Description	: 以太网中断
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void ETH_IRQHandler(void)
{
  HAL_ETH_IRQHandler(&EthHandle);
}
#endif

/*
*********************************************************************************************************
* Function Name : HAL_ETH_RxCpltCallback
* Description	: 以太网接收传输完成回调函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth)
{
	if (heth->Instance == ETH)
	{
		ethernetif_input(&g_Netif_LAN8720);
	}
}	

/*
*********************************************************************************************************
* Function Name : BSP_ETH_IsDetected
* Description	: 检测以太网连接状态
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool BSP_ETH_IsDetected( ETH_HandleTypeDef *heth )
{
	uint32_t phyReg;
	
	if (HAL_ETH_ReadPHYRegister(heth, PHY_BSR, &phyReg) == HAL_OK)
	{
		if ((phyReg & PHY_LINKED_STATUS) == PHY_LINKED_STATUS)
		{
			return true;
		}
	}
	
	return false;
}

/*
*********************************************************************************************************
* Function Name : __IsETH_Connect
* Description	: 以太网是否连接
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE bool __IsETH_Connect( void *devicePrt )
{
	return BSP_ETH_IsDetected(devicePrt);
}

/*
*********************************************************************************************************
* Function Name : __ETH_ConnectNotify
* Description	: 以太网连接通知
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE void __ETH_ConnectNotify( void *devicePrt )
{
	ethernetif_set_link(&g_Netif_LAN8720);
}

/*
*********************************************************************************************************
* Function Name : __ETH_DisConnectNotify
* Description	: 以太网断开连接通知
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE void __ETH_DisConnectNotify( void *devicePrt )
{
	ethernetif_set_link(&g_Netif_LAN8720);
}

/*
*********************************************************************************************************
* Function Name : ETH_ConnectionSwitchTaskInit
* Description	: 以太网连接开关任务初始化
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void ETH_ConnectionSwitchTaskInit( SWITCH_TASK_TypeDef *switchPtr )
{	
	SwitchTaskInit(switchPtr, &EthHandle);
	RegisterSwitchTaskIsSwitchOn_CallBack(switchPtr, __IsETH_Connect);
	RegisterSwitchTaskOnDevice_CallBack(switchPtr, __ETH_ConnectNotify);
	RegisterSwitchTaskOffDevice_CallBack(switchPtr, __ETH_DisConnectNotify);
}

/*
*********************************************************************************************************
* Function Name : ethernetif_notify_conn_changed
* Description	: 网络切换通知
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void ethernetif_notify_conn_changed(struct netif *netif)
{
	ip_addr_t ipaddr;
	ip_addr_t netmask;
	ip_addr_t gw;

	if(netif_is_link_up(netif))
	{
		Netif_ConfigIP(&ipaddr);
		Netif_ConfigNETMASK(&netmask);
		Netif_ConfigGW_ADDR(&gw);  

		netif_set_addr(netif, &ipaddr , &netmask, &gw);

		/* When the netif is fully configured this function must be called.*/
		netif_set_up(netif);      

		ECHO(DEBUG_BSP_INIT, "网线已连接！");
	}
	else
	{    
		/*  When the netif link is down this function must be called.*/
		netif_set_down(netif);
		  
		ECHO(DEBUG_BSP_INIT, "网线已断开！"); 
	}
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
