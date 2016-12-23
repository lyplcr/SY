/*
*********************************************************************************************************
* @file    	TCP_Client.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-28 10:14:06
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	TCP客户端源文件
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
#include "TCP_Client.h"


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
/* ECHO protocol states */
enum echoclient_states
{
  ES_NOT_CONNECTED = 0,
  ES_CONNECTED,
  ES_RECEIVED,
  ES_CLOSING,
};

/* structure to be passed as argument to the tcp callbacks */
struct echoclient
{
  enum echoclient_states state; /* connection status */
  struct tcp_pcb *pcb;          /* pointer on the current tcp_pcb */
  struct pbuf *p_tx;            /* pointer on pbuf to be transmitted */
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
static struct tcp_pcb *g_TCP_ClientHandle;
static struct echoclient *g_EsHandle = NULL;
extern uint8_t ETH_RxBuff[];
extern SEQUEUE_TypeDef g_ETH_RxQueue;



/*
*********************************************************************************************************
*                              				Private function prototypes
*********************************************************************************************************
*/
static err_t tcp_echoclient_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static void tcp_echoclient_connection_close(struct tcp_pcb *tpcb, struct echoclient * es);
static err_t tcp_echoclient_poll(void *arg, struct tcp_pcb *tpcb);
static err_t tcp_echoclient_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static void tcp_echoclient_send(struct tcp_pcb *tpcb, struct echoclient * es);
static err_t tcp_echoclient_connected(void *arg, struct tcp_pcb *tpcb, err_t err);
static void TCP_ClientReceive_CallBack(struct tcp_pcb *pcb, struct pbuf *p);

/*
*********************************************************************************************************
*                              				Private functions
*********************************************************************************************************
*/
/**
* @brief  Connects to the TCP echo server
* @param  None
* @retval None
*/
err_t tcp_echoclient_connect( uint8_t *IP_Addr, uint16_t Port )
{
	ip_addr_t DestIPaddr;
	err_t ret = ERR_VAL;

	/* create new tcp pcb */
	g_TCP_ClientHandle = tcp_new();

	if (g_TCP_ClientHandle != NULL)
	{
		IP4_ADDR( &DestIPaddr, IP_Addr[0], IP_Addr[1], IP_Addr[2], IP_Addr[3] );

		/* connect to destination address/port */
		ret = tcp_connect(g_TCP_ClientHandle, &DestIPaddr, Port, tcp_echoclient_connected);
		
		ECHO(DEBUG_BSP_ETH,"<TCP> 客户端主动连接服务器...");
	}
	
	return ret;
}

/**
  * @brief Function called when TCP connection established
  * @param tpcb: pointer on the connection contol block
  * @param err: when connection correctly established err should be ERR_OK 
  * @retval err_t: returned error 
  */
static err_t tcp_echoclient_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{
	if (err == ERR_OK)   
	{
		/* allocate structure es to maintain tcp connection informations */
		g_EsHandle = (struct echoclient *)mem_malloc(sizeof(struct echoclient));

		if (g_EsHandle != NULL)
		{
			g_EsHandle->state = ES_CONNECTED;
			g_EsHandle->pcb = tpcb; 
			g_EsHandle->p_tx = NULL;

			/* pass newly allocated es structure as argument to tpcb */
			tcp_arg(tpcb, g_EsHandle);

			/* initialize LwIP tcp_recv callback function */ 
			tcp_recv(tpcb, tcp_echoclient_recv);

			/* initialize LwIP tcp_sent callback function */
			tcp_sent(tpcb, tcp_echoclient_sent);

			/* initialize LwIP tcp_poll callback function */
			tcp_poll(tpcb, tcp_echoclient_poll, 1);

			ECHO(DEBUG_BSP_ETH,"<TCP> 连接服务器成功...");

			return ERR_OK;
		}
		else
		{
			/* close connection */
			tcp_echoclient_connection_close(tpcb, g_EsHandle);

			/* return memory allocation error */
			return ERR_MEM;  
		}
	}
	else
	{
		/* close connection */
		tcp_echoclient_connection_close(tpcb, g_EsHandle);
	}
	
	return err;
}
    
/**
  * @brief tcp_receiv callback
  * @param arg: argument to be passed to receive callback 
  * @param tpcb: tcp connection control block 
  * @param err: receive error code 
  * @retval err_t: retuned error  
  */
static err_t tcp_echoclient_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{ 
	struct echoclient *es;
	err_t ret_err;

	LWIP_ASSERT("arg != NULL",arg != NULL);

	es = (struct echoclient *)arg;

	/* if we receive an empty tcp frame from server => close connection */
	if (p == NULL)
	{
		/* remote host closed connection */
		es->state = ES_CLOSING;
		if(es->p_tx == NULL)
		{
			/* we're done sending, close connection */
			tcp_echoclient_connection_close(tpcb, es);
		}
		else
		{    
			/* send remaining data*/
			tcp_echoclient_send(tpcb, es);
		}
		ret_err = ERR_OK;
	}   
	/* else : a non empty frame was received from echo server but for some reason err != ERR_OK */
	else if(err != ERR_OK)
	{
		/* free received pbuf*/
		if (p != NULL)
		{
			pbuf_free(p);
		}
		ret_err = err;
	}
	else if(es->state == ES_CONNECTED)
	{
		/* Acknowledge data reception */
		tcp_recved(tpcb, p->tot_len);  
		TCP_ClientReceive_CallBack(tpcb,p);
		ECHO(DEBUG_BSP_ETH,"<TCP> 收到服务器数据！");
		
		ret_err = ERR_OK;
	}
	/* data received when connection already closed */
	else
	{
		/* Acknowledge data reception */
		tcp_recved(tpcb, p->tot_len);

		/* free pbuf and do nothing */
		pbuf_free(p);
		ret_err = ERR_OK;
	}
	
	return ret_err;
}

/**
  * @brief function used to send data
  * @param  tpcb: tcp control block
  * @param  es: pointer on structure of type echoclient containing info on data 
  *             to be sent
  * @retval None 
  */
static void tcp_echoclient_send(struct tcp_pcb *tpcb, struct echoclient * es)
{
	struct pbuf *ptr;
	err_t wr_err = ERR_OK;
	
	ECHO(DEBUG_BSP_ETH,"<TCP> Client Send...");

	while ((wr_err == ERR_OK) &&
			(es->p_tx != NULL) && 
			(es->p_tx->len <= tcp_sndbuf(tpcb)))
	{
		/* get pointer on pbuf from es structure */
		ptr = es->p_tx;

		/* enqueue data for transmission */
		wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);

		if (wr_err == ERR_OK)
		{ 
			/* continue with next pbuf in chain (if any) */
			es->p_tx = ptr->next;

			if(es->p_tx != NULL)
			{
				/* increment reference count for es->p */
				pbuf_ref(es->p_tx);
			}

			/* free pbuf: will free pbufs up to es->p (because es->p has a reference count > 0) */
			pbuf_free(ptr);
		}
		else if(wr_err == ERR_MEM)
		{
			/* we are low on memory, try later, defer to poll */
			es->p_tx = ptr;
		}
		else
		{
			/* other problem ?? */
		}
	}
}

/**
  * @brief  This function implements the tcp_poll callback function
  * @param  arg: pointer on argument passed to callback
  * @param  tpcb: tcp connection control block
  * @retval err_t: error code
  */
static err_t tcp_echoclient_poll(void *arg, struct tcp_pcb *tpcb)
{
	err_t ret_err;
	struct echoclient *es;

	es = (struct echoclient*)arg;
	if (es != NULL)
	{
		if (es->p_tx != NULL)
		{
			/* there is a remaining pbuf (chain) , try to send data */
			tcp_echoclient_send(tpcb, es);
			ECHO(DEBUG_BSP_ETH,"<TCP> Client Poll...");
		}
		else
		{
			/* no remaining pbuf (chain)  */
			if(es->state == ES_CLOSING)
			{
				/* close tcp connection */
				tcp_echoclient_connection_close(tpcb, es);
			}
		}
		ret_err = ERR_OK;
	}
	else
	{
		/* nothing to be done */
		tcp_abort(tpcb);
		ret_err = ERR_ABRT;
	}
	return ret_err;
}

/**
  * @brief  This function implements the tcp_sent LwIP callback (called when ACK
  *         is received from remote host for sent data) 
  * @param  arg: pointer on argument passed to callback
  * @param  tcp_pcb: tcp connection control block
  * @param  len: length of data sent 
  * @retval err_t: returned error code
  */
static err_t tcp_echoclient_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
	struct echoclient *es;
	
	ECHO(DEBUG_BSP_ETH,"<TCP> Client Sent...");

	LWIP_UNUSED_ARG(len);

	es = (struct echoclient *)arg;

	if(es->p_tx != NULL)
	{
		/* still got pbufs to send */
		tcp_echoclient_send(tpcb, es);
	}

	return ERR_OK;
}

/**
  * @brief This function is used to close the tcp connection with server
  * @param tpcb: tcp connection control block
  * @param es: pointer on echoclient structure
  * @retval None
  */
static void tcp_echoclient_connection_close(struct tcp_pcb *tpcb, struct echoclient * es )
{
	/* remove callbacks */
	tcp_recv(tpcb, NULL);
	tcp_sent(tpcb, NULL);
	tcp_poll(tpcb, NULL,0);

	if (es != NULL)
	{
		mem_free(es);
	}

	/* close tcp connection */
	tcp_close(tpcb);
	
	ECHO(DEBUG_BSP_ETH,"<TCP> Client Close!");
}

/*
*********************************************************************************************************
* Function Name : TCP_ClientConnect
* Description	: 建立TCP连接
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool TCP_ClientConnect( uint8_t *IP_Addr, uint16_t Port )
{
	if (tcp_echoclient_connect(IP_Addr, Port) == ERR_OK)
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
* Function Name : TCP_ClientClose
* Description	: 关闭TCP连接
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool TCP_ClientClose( void )
{
	struct tcp_pcb *tpcb = g_TCP_ClientHandle;
	struct echoclient *es = (struct echoclient *)tpcb->callback_arg;
	
	if (tpcb == NULL) 
	{
		return false;
	}
	
	ECHO(DEBUG_BSP_ETH,"<TCP> 客户端主动关闭！");
	tcp_echoclient_connection_close(tpcb,es);
	
	return true;
}

/*
*********************************************************************************************************
* Function Name : TCP_ClientReceive_CallBack
* Description	: TCP客户端接收数据回调函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void TCP_ClientReceive_CallBack(struct tcp_pcb *pcb, struct pbuf *p)
{	
	if (p)
	{	
		uint8_t packageBuff[ETH_MAX_POOL_SIZE];
		uint32_t rxLen = ETH_GetPackageBody(p, packageBuff);
		
		for (uint32_t i=0; i<rxLen; ++i)
		{
			if (PushSeqQueue(&g_ETH_RxQueue, &packageBuff[i], PushSeqQueueU8_CallBack)\
				== STATUS_DATA_STRUCT_FALSE)
			{
				ECHO(DEBUG_BSP_ETH,"<TCP> 缓存溢出！");
				
				break;
			}
		}
		
		/* Free the p buffer */
		pbuf_free(p);
	}
}

/*
*********************************************************************************************************
* Function Name : TCP_ClientReadByte
* Description	: 读取TCP数据
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool TCP_ClientReadByte( uint8_t *data )
{
	if (PopSeqQueue(&g_ETH_RxQueue, data, PopSeqQueueU8_CallBack) == STATUS_DATA_STRUCT_TRUE)
	{
		return true;
	}
	
	return false;
}

/*
*********************************************************************************************************
* Function Name : TCP_ClientWriteBytes
* Description	: 使用TCP发送数据
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool TCP_ClientWriteBytes( const uint8_t *data, uint32_t lenth )
{
	if (lenth)
	{
		/*
			PBUF_RAM：当应用程序发送动态地产生的数据时,PBUF_RAM类型的Pbufs也被使用。 
			在这种情况下,pbuf系统不光为应用数据分配存储空间,也为将要发送的数据的报头准备空间。
			pbuf系统不能预先知道为将要发送的数据准备什么样的报头,并且假定最坏的情况。
			报头大小在编译时可动态配置。
		*/
		g_EsHandle->p_tx = pbuf_alloc(PBUF_RAW, lenth, PBUF_RAM);
		if (g_EsHandle->p_tx == NULL)
		{
			ECHO(DEBUG_BSP_ETH,"<TCP> 内存分配错误！");
			
			return false;
		}
		
		 /* copy data to pbuf */
        pbuf_take(g_EsHandle->p_tx, (char*)data, lenth);
		
		/* send data */
		tcp_echoclient_send(g_TCP_ClientHandle,g_EsHandle);
	}
	
	return true;
}



/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
