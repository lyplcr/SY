/*
*********************************************************************************************************
* @file    	UDP_Server.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-28 10:14:06
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	UDP服务器源文件
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
#include "SeqQueue.h"
#include "lwip/netif.h"
#include "UDP_Server.h"


/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/
#define UDP_SERVER_PORT    					9999   /* define the UDP local connection port */


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
static struct udp_pcb *g_UDP_ServerHandle;
extern uint8_t ETH_RxBuff[];
extern SEQUEUE_TypeDef g_ETH_RxQueue;

/*
*********************************************************************************************************
*                              				Private function prototypes
*********************************************************************************************************
*/
static void UDP_ServerReceive_CallBack(void *arg, struct udp_pcb *pcb, struct pbuf *p,
    const ip_addr_t *addr, u16_t port);

/*
*********************************************************************************************************
*                              				Private functions
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : UDP_ServerInit
* Description	: UDP服务器初始化
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void UDP_ServerInit( void )
{
	g_UDP_ServerHandle = udp_new();
	if (g_UDP_ServerHandle)
	{
		/* Bind the upcb to the UDP_PORT port */
		/* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
		err_t err = udp_bind(g_UDP_ServerHandle, IP_ADDR_ANY, UDP_SERVER_PORT);
		if(err == ERR_OK)
		{
			/* Set a receive callback for the upcb */
			udp_recv(g_UDP_ServerHandle, UDP_ServerReceive_CallBack, NULL);
		}
	}
}

/*
*********************************************************************************************************
* Function Name : UDP_ServerReceive_CallBack
* Description	: UDP服务器接收数据回调函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void UDP_ServerReceive_CallBack(void *arg, struct udp_pcb *pcb, struct pbuf *p,\
				const ip_addr_t *addr, u16_t port)
{
	/**
	 * 	实际测试，一台客户端连接服务器后，其他IP、其他端口向本服务器发送数据均失败！
	 *	从而实现绑定首次连接的客户端。
	 */
	if ( (*(uint32_t *)&pcb->remote_ip == 0) && (pcb->remote_port == 0) )
	{
		ECHO(DEBUG_BSP_ETH,"<UDP> 客户端主动连接服务器！");

		udp_connect(pcb, addr, port);
	}
	
	if (p)
	{	
		uint8_t packageBuff[ETH_MAX_POOL_SIZE];
		uint32_t rxLen = ETH_GetPackageBody(p, packageBuff);
		
		for (uint32_t i=0; i<rxLen; ++i)
		{
			if (PushSeqQueue(&g_ETH_RxQueue, &packageBuff[i], PushSeqQueueU8_CallBack)\
				== STATUS_DATA_STRUCT_FALSE)
			{
				ECHO(DEBUG_BSP_ETH,"<UDP> 缓存溢出！");
				
				break;
			}
		}
		
		/* Free the p buffer */
		pbuf_free(p);
	}
}

/*
*********************************************************************************************************
* Function Name : UDP_ServerReadByte
* Description	: 读取UDP数据
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool UDP_ServerReadByte( uint8_t *data )
{
	if (PopSeqQueue(&g_ETH_RxQueue, data, PopSeqQueueU8_CallBack) == STATUS_DATA_STRUCT_TRUE)
	{
		return true;
	}
	
	return false;
}

/*
*********************************************************************************************************
* Function Name : UDP_ServerWriteBytes
* Description	: 使用UDP发送数据
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool UDP_ServerWriteBytes( uint8_t *IP_Addr, uint16_t Port, \
		const uint8_t *data, uint32_t lenth )
{
	err_t ret = ERR_OK;
	
	if (lenth)
	{
		/*
			PBUF_RAM：当应用程序发送动态地产生的数据时,PBUF_RAM类型的Pbufs也被使用。 
			在这种情况下,pbuf系统不光为应用数据分配存储空间,也为将要发送的数据的报头准备空间。
			pbuf系统不能预先知道为将要发送的数据准备什么样的报头,并且假定最坏的情况。
			报头大小在编译时可动态配置。
		*/
		struct pbuf *p = pbuf_alloc(PBUF_RAW, lenth, PBUF_RAM);
		if (p == NULL)
		{
			ECHO(DEBUG_BSP_ETH,"<UDP> 内存分配错误！");
			
			return false;
		}
	
		/* copy data to pbuf */
		pbuf_take(p, data, lenth);
		ip_addr_t DestIPaddr;
		IP4_ADDR( &DestIPaddr, IP_Addr[0], IP_Addr[1], IP_Addr[2], IP_Addr[3] );
		ret = udp_sendto(g_UDP_ServerHandle, p, &DestIPaddr, Port);
		/* Free the p buffer */
		pbuf_free(p);
	}
	
	if (ret == ERR_OK)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
*********************************************************************************************************
* Function Name : UDP_ServerEchoWriteBytes
* Description	: 使用UDP发送数据
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool UDP_ServerEchoWriteBytes( const uint8_t *data, uint32_t lenth )
{
	err_t ret = ERR_OK;
	
	if (lenth)
	{
		struct pbuf *p = pbuf_alloc(PBUF_RAW, lenth, PBUF_RAM);
		if (p == NULL)
		{
			ECHO(DEBUG_BSP_ETH,"<UDP> 内存分配错误！");
			
			return false;
		}
	
		/* copy data to pbuf */
		pbuf_take(p, data, lenth);
		ret = udp_send(g_UDP_ServerHandle, p);
		/* Free the p buffer */
		pbuf_free(p);
	}
	
	if (ret == ERR_OK)
	{
		return true;
	}
	else
	{
		return false;
	}
}


/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
