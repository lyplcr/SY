/*
*********************************************************************************************************
* @file    	bsp_uart.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-17 13:05:46
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	串口模块源文件
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
#include "SeqQueue.h"

/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/
#if 0
	#define	USART1_TX_GPIO_CLK_ENABLE()			__HAL_RCC_GPIOB_CLK_ENABLE()
	#define	USART1_RX_GPIO_CLK_ENABLE()			__HAL_RCC_GPIOB_CLK_ENABLE()		
	#define USART1_CLK_ENABLE()					__HAL_RCC_USART1_CLK_ENABLE()
	#define USART1_TX_AF						GPIO_AF7_USART1
	#define USART1_RX_AF						GPIO_AF7_USART1
	#define	USART1_TX_GPIO_PORT 				GPIOB
	#define	USART1_TX_GPIO_PIN					GPIO_PIN_6
	#define	USART1_RX_GPIO_PORT 				GPIOB
	#define	USART1_RX_GPIO_PIN					GPIO_PIN_7
#else
	#define	USART1_TX_GPIO_CLK_ENABLE()			__HAL_RCC_GPIOA_CLK_ENABLE()
	#define	USART1_RX_GPIO_CLK_ENABLE()			__HAL_RCC_GPIOA_CLK_ENABLE()
	#define USART1_CLK_ENABLE()					__HAL_RCC_USART1_CLK_ENABLE()
	#define USART1_TX_AF						GPIO_AF7_USART1
	#define USART1_RX_AF						GPIO_AF7_USART1
	#define	USART1_TX_GPIO_PORT 				GPIOA
	#define	USART1_TX_GPIO_PIN					GPIO_PIN_9
	#define	USART1_RX_GPIO_PORT 				GPIOA
	#define	USART1_RX_GPIO_PIN					GPIO_PIN_10
#endif

#if 0
	#define	USART6_TX_GPIO_CLK_ENABLE()			__HAL_RCC_GPIOG_CLK_ENABLE()
	#define	USART6_RX_GPIO_CLK_ENABLE()			__HAL_RCC_GPIOG_CLK_ENABLE()
	#define USART6_CLK_ENABLE()					__HAL_RCC_USART6_CLK_ENABLE()
	#define USART6_TX_AF						GPIO_AF8_USART6
	#define USART6_RX_AF						GPIO_AF8_USART6
	#define	USART6_TX_GPIO_PORT 				GPIOG
	#define	USART6_TX_GPIO_PIN					GPIO_PIN_14
	#define	USART6_RX_GPIO_PORT 				GPIOG
	#define	USART6_RX_GPIO_PIN					GPIO_PIN_9
#else
	#define	USART6_TX_GPIO_CLK_ENABLE()			__HAL_RCC_GPIOC_CLK_ENABLE()
	#define	USART6_RX_GPIO_CLK_ENABLE()			__HAL_RCC_GPIOC_CLK_ENABLE()
	#define USART6_CLK_ENABLE()					__HAL_RCC_USART6_CLK_ENABLE()
	#define USART6_TX_AF						GPIO_AF8_USART6
	#define USART6_RX_AF						GPIO_AF8_USART6
	#define	USART6_TX_GPIO_PORT 				GPIOC
	#define	USART6_TX_GPIO_PIN					GPIO_PIN_6
	#define	USART6_RX_GPIO_PORT 				GPIOC
	#define	USART6_RX_GPIO_PIN					GPIO_PIN_7
#endif

/*
*********************************************************************************************************
*                              				Private typedef
*********************************************************************************************************
*/
/* 串口设备结构体 */
typedef struct
{
	USART_TypeDef *uart;			/* STM32内部串口设备指针 */
	UART_HandleTypeDef *handle;		/* 句柄 */
	SEQUEUE_TypeDef sendQueue;		/* 发送队列 */
	SEQUEUE_TypeDef receiveQueue;	/* 接收队列 */
	uint8_t receiveByte;			/* 接收字节 */
}UART_T;  
 


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
/* 定义每个串口结构体变量 */
#if UART1_FIFO_EN == 1
	static UART_T g_tUart1;
	UART_HandleTypeDef g_Uart1Handle;
	static uint8_t g_TxBuf1[UART1_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf1[UART1_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif

#if UART2_FIFO_EN == 1
	static UART_T g_tUart2;
	UART_HandleTypeDef g_Uart2Handle;
	static uint8_t g_TxBuf2[UART2_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf2[UART2_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif

#if UART3_FIFO_EN == 1
	static UART_T g_tUart3;
	UART_HandleTypeDef g_Uart3Handle;
	static uint8_t g_TxBuf3[UART3_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf3[UART3_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif

#if UART4_FIFO_EN == 1
	static UART_T g_tUart4;
	UART_HandleTypeDef g_Uart4Handle;
	static uint8_t g_TxBuf4[UART4_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf4[UART4_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif

#if UART5_FIFO_EN == 1
	static UART_T g_tUart5;
	UART_HandleTypeDef g_Uart5Handle;
	static uint8_t g_TxBuf5[UART5_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf5[UART5_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif

#if UART6_FIFO_EN == 1
	static UART_T g_tUart6;
	UART_HandleTypeDef g_Uart6Handle;
	static uint8_t g_TxBuf6[UART6_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf6[UART6_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif


/*
*********************************************************************************************************
*                              				Private function prototypes
*********************************************************************************************************
*/
static UART_T *ComToUart(COM_PORT_E _ucPort);


/*
*********************************************************************************************************
*                              				Private functions
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : UartVarInit
* Description	: 初始化串口相关的变量
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void UartVarInit(void)
{
#if UART1_FIFO_EN == 1
	g_tUart1.uart = USART1;						
	g_tUart1.handle = &g_Uart1Handle;
	CreateSeqQueue(&g_tUart1.sendQueue, g_TxBuf1, UART1_TX_BUF_SIZE);
	CreateSeqQueue(&g_tUart1.receiveQueue, g_RxBuf1, UART1_RX_BUF_SIZE);
	HAL_UART_Receive_IT(&g_Uart1Handle, &g_tUart1.receiveByte, 1);
#endif

#if UART2_FIFO_EN == 1
	g_tUart2.uart = USART2;						
	g_tUart2.handle = &g_Uart2Handle;
	CreateSeqQueue(&g_tUart2.sendQueue,g_TxBuf2,UART2_TX_BUF_SIZE);
	CreateSeqQueue(&g_tUart2.receiveQueue,g_RxBuf2,UART2_RX_BUF_SIZE);
	HAL_UART_Receive_IT(&g_Uart2Handle, &g_tUart2.receiveByte, 1);
#endif

#if UART3_FIFO_EN == 1
	g_tUart3.uart = USART3;						
	g_tUart3.handle = &g_Uart3Handle;
	CreateSeqQueue(&g_tUart3.sendQueue,g_TxBuf3,UART3_TX_BUF_SIZE);
	CreateSeqQueue(&g_tUart3.receiveQueue,g_RxBuf3,UART3_RX_BUF_SIZE);
	HAL_UART_Receive_IT(&g_Uart3Handle, &g_tUart3.receiveByte, 1);
#endif

#if UART4_FIFO_EN == 1
	g_tUart4.uart = UART4;			
	g_tUart4.handle = &g_Uart4Handle;
	CreateSeqQueue(&g_tUart4.sendQueue,g_TxBuf4,UART4_TX_BUF_SIZE);
	CreateSeqQueue(&g_tUart4.receiveQueue,g_RxBuf4,UART4_RX_BUF_SIZE);
	HAL_UART_Receive_IT(&g_Uart4Handle, &g_tUart4.receiveByte, 1);
#endif

#if UART5_FIFO_EN == 1
	g_tUart5.uart = UART5;				
	g_tUart5.handle = &g_Uart5Handle;
	CreateSeqQueue(&g_tUart5.sendQueue,g_TxBuf5,UART5_TX_BUF_SIZE);
	CreateSeqQueue(&g_tUart5.receiveQueue,g_RxBuf5,UART5_RX_BUF_SIZE);
	HAL_UART_Receive_IT(&g_Uart5Handle, &g_tUart5.receiveByte, 1);
#endif

#if UART6_FIFO_EN == 1
	g_tUart6.uart = USART6;					
	g_tUart6.handle = &g_Uart6Handle;
	CreateSeqQueue(&g_tUart6.sendQueue,g_TxBuf6,UART6_TX_BUF_SIZE);
	CreateSeqQueue(&g_tUart6.receiveQueue,g_RxBuf6,UART6_RX_BUF_SIZE);
	HAL_UART_Receive_IT(&g_Uart6Handle, &g_tUart6.receiveByte, 1);
#endif
}

/*
*********************************************************************************************************
* Function Name : UartHardInit
* Description	: 配置串口的硬件参数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void UartHardInit(void)
{
#if UART1_FIFO_EN == 1
	g_Uart1Handle.Instance = USART1;
	
	g_Uart1Handle.Init.BaudRate     = UART1_BAUD;
	g_Uart1Handle.Init.WordLength   = UART_WORDLENGTH_8B;
	g_Uart1Handle.Init.StopBits     = UART_STOPBITS_1;
	g_Uart1Handle.Init.Parity       = UART_PARITY_NONE;
	g_Uart1Handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	g_Uart1Handle.Init.Mode         = UART_MODE_TX_RX;
	if(HAL_UART_DeInit(&g_Uart1Handle) != HAL_OK)
	{
		bsp_ErrorHandler();
	}  
	if(HAL_UART_Init(&g_Uart1Handle) != HAL_OK)
	{
		bsp_ErrorHandler();
	}
#endif
	
#if UART6_FIFO_EN == 1
	g_Uart6Handle.Instance = USART6;
	
	g_Uart6Handle.Init.BaudRate     = UART6_BAUD;
	g_Uart6Handle.Init.WordLength   = UART_WORDLENGTH_8B;
	g_Uart6Handle.Init.StopBits     = UART_STOPBITS_1;
	g_Uart6Handle.Init.Parity       = UART_PARITY_NONE;
	g_Uart6Handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	g_Uart6Handle.Init.Mode         = UART_MODE_TX_RX;
	g_Uart6Handle.Init.OverSampling = UART_OVERSAMPLING_16;
	if(HAL_UART_DeInit(&g_Uart6Handle) != HAL_OK)
	{
		bsp_ErrorHandler();
	}  
	if(HAL_UART_Init(&g_Uart6Handle) != HAL_OK)
	{
		bsp_ErrorHandler();
	}
	
#endif	
}

/*
*********************************************************************************************************
* Function Name : bsp_InitCOM
* Description	: 串口初始化
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_InitCOM(void)
{
	UartHardInit();
	UartVarInit();
	ECHO(DEBUG_BSP_INIT, "==================== BSP ====================");
	ECHO(DEBUG_BSP_INIT, "USART初始化 .......... OK");
}

/*
*********************************************************************************************************
* Function Name : ComToUart
* Description	: 将COM端口号转换为UART指针
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static UART_T *ComToUart(COM_PORT_E _ucPort)
{
	if (_ucPort == COM1)
	{
		#if UART1_FIFO_EN == 1
			return &g_tUart1;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM2)
	{
		#if UART2_FIFO_EN == 1
			return &g_tUart2;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM3)
	{
		#if UART3_FIFO_EN == 1
			return &g_tUart3;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM4)
	{
		#if UART4_FIFO_EN == 1
			return &g_tUart4;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM5)
	{
		#if UART5_FIFO_EN == 1
			return &g_tUart5;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM6)
	{
		#if UART6_FIFO_EN == 1
			return &g_tUart6;
		#else
			return 0;
		#endif
	}
	else
	{
		/* 不做任何处理 */
		return 0;
	}
}  
	
/*
*********************************************************************************************************
* Function Name : ComSend
* Description	: 向COM口发送一组数据。数据放到发送缓冲区后立即返回，由中断服务程序在后台完成发送
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool ComSend(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen)
{
	UART_T *pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{		
		return false;
	}
	
	/**
	 *	使用 HAL_UART_Transmit_IT() 中断发送函数，使能<发送寄存器为空>中断。
	 *	进入中断服务函数后，如果发现还有数据未发送，则继续发送。
	 *	发送完成后，使能<发送完成>中断，然后调用回调函数通知用户，
	 *	用户可以选择在回调函数中继续发送数据。
	 */
	for (uint16_t i=0; i<_usLen; ++i)
	{
		if (PushSeqQueue(&pUart->sendQueue, &_ucaBuf[i], PushSeqQueueU8_CallBack) != STATUS_DATA_STRUCT_TRUE)
		{
			return false;
		}
	}
	
	if (pUart->handle->gState == HAL_UART_STATE_READY)
	{
		static uint8_t txData = 0;
		if (PopSeqQueue(&pUart->sendQueue, &txData, PopSeqQueueU8_CallBack) == STATUS_DATA_STRUCT_TRUE)
		{
			if (HAL_UART_Transmit_IT(pUart->handle, &txData, 1) != HAL_OK)
			{
				return false;
			}
		}
	}
	
	return true;
}

/*
*********************************************************************************************************
* Function Name : ComGet
* Description	: 从串口缓冲区读取1字节，非阻塞。无论有无数据均立即返回
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool ComGet(COM_PORT_E _ucPort, uint8_t *_pByte)
{
	UART_T *pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{		
		return false;
	}
	
	if (PopSeqQueue(&pUart->receiveQueue, _pByte, PopSeqQueueU8_CallBack) == STATUS_DATA_STRUCT_TRUE)
	{
		return true;
	}
	
	return false;
}

/*
*********************************************************************************************************
* Function Name : USART1_IRQHandler、USART2_IRQHandler...USART6_IRQHandler
* Description	: USART中断服务程序
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
#if UART1_FIFO_EN == 1
void USART1_IRQHandler(void)
{
#if (uCOS_EN)
	CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();                                          
    CPU_CRITICAL_EXIT();
#endif
	
	HAL_UART_IRQHandler(&g_Uart1Handle);
	
#if (uCOS_EN)
	OSIntExit(); 
#endif	
}
#endif

#if UART2_FIFO_EN == 1
void USART2_IRQHandler(void)
{
#if (uCOS_EN)
	CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();                                          
    CPU_CRITICAL_EXIT();
#endif
	
	HAL_UART_IRQHandler(&g_Uart2Handle);
	
#if (uCOS_EN)
	OSIntExit(); 
#endif		
}
#endif

#if UART3_FIFO_EN == 1
void USART3_IRQHandler(void)
{
#if (uCOS_EN)
	CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();                                          
    CPU_CRITICAL_EXIT();
#endif
	
	HAL_UART_IRQHandler(&g_Uart3Handle);
	
#if (uCOS_EN)
	OSIntExit(); 
#endif		
}
#endif

#if UART4_FIFO_EN == 1
void UART4_IRQHandler(void)
{
#if (uCOS_EN)
	CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();                                          
    CPU_CRITICAL_EXIT();
#endif
	
	HAL_UART_IRQHandler(&g_Uart4Handle);
	
#if (uCOS_EN)
	OSIntExit(); 
#endif	
}
#endif

#if UART5_FIFO_EN == 1
void UART5_IRQHandler(void)
{
#if (uCOS_EN)
	CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();                                          
    CPU_CRITICAL_EXIT();
#endif
	
	HAL_UART_IRQHandler(&g_Uart5Handle);
	
#if (uCOS_EN)
	OSIntExit(); 
#endif	
}
#endif

#if UART6_FIFO_EN == 1
void USART6_IRQHandler(void)
{
#if (uCOS_EN)
	CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();                                          
    CPU_CRITICAL_EXIT();
#endif
	
	HAL_UART_IRQHandler(&g_Uart6Handle);
	
#if (uCOS_EN)
	OSIntExit(); 
#endif	
}
#endif

/*
*********************************************************************************************************
*                              				MSP
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : HAL_USART_MspInit
* Description	: 串口MSP初始化
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void HAL_UART_MspInit(UART_HandleTypeDef *husart)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	if (husart->Instance == USART1)
	{
		/* 初始化时钟 */
		USART1_TX_GPIO_CLK_ENABLE();
		USART1_RX_GPIO_CLK_ENABLE();
		USART1_CLK_ENABLE();
		
		/* 初始化GPIO-TX */
		GPIO_InitStructure.Pin = USART1_TX_GPIO_PIN;
		GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;	
		GPIO_InitStructure.Alternate = USART1_TX_AF;
		HAL_GPIO_Init(USART1_TX_GPIO_PORT, &GPIO_InitStructure);
		
		/* 初始化GPIO-RX */
		GPIO_InitStructure.Pin = USART1_RX_GPIO_PIN;
		GPIO_InitStructure.Alternate = USART1_RX_AF;
		HAL_GPIO_Init(USART1_RX_GPIO_PORT, &GPIO_InitStructure);
		
		/* 设置优先级 */
		HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	}
	
	if (husart->Instance == USART6)
	{
		/* 初始化时钟 */
		USART6_TX_GPIO_CLK_ENABLE();
		USART6_RX_GPIO_CLK_ENABLE();
		USART6_CLK_ENABLE();
		
		/* 初始化GPIO-TX */
		GPIO_InitStructure.Pin = USART6_TX_GPIO_PIN;
		GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;	
		GPIO_InitStructure.Alternate = USART6_TX_AF;
		HAL_GPIO_Init(USART6_TX_GPIO_PORT, &GPIO_InitStructure);
		
		/* 初始化GPIO-RX */
		GPIO_InitStructure.Pin = USART6_RX_GPIO_PIN;
		GPIO_InitStructure.Alternate = USART6_RX_AF;
		HAL_GPIO_Init(USART6_RX_GPIO_PORT, &GPIO_InitStructure);
		
		/* 设置优先级 */
		HAL_NVIC_SetPriority(USART6_IRQn, 3, 0);
		HAL_NVIC_EnableIRQ(USART6_IRQn);
	}
}

/*
*********************************************************************************************************
* Function Name : HAL_UART_TxCpltCallback
* Description	: 串口发送完成回调函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if (UartHandle->Instance == USART1)
	{
		if (UartHandle->gState == HAL_UART_STATE_READY)
		{
			static uint8_t txData = 0;
			if (PopSeqQueue(&g_tUart1.sendQueue, &txData, PopSeqQueueU8_CallBack) == STATUS_DATA_STRUCT_TRUE)
			{
				HAL_UART_Transmit_IT(UartHandle, &txData, 1);
			}
		}
	}
	
	if (UartHandle->Instance == USART6)
	{
		if (UartHandle->gState == HAL_UART_STATE_READY)
		{
			static uint8_t txData = 0;
			if (PopSeqQueue(&g_tUart6.sendQueue, &txData, PopSeqQueueU8_CallBack) == STATUS_DATA_STRUCT_TRUE)
			{
				HAL_UART_Transmit_IT(UartHandle, &txData, 1);
			}
		}
	}
}

/*
*********************************************************************************************************
* Function Name : HAL_UART_RxCpltCallback
* Description	: 串口接收完成回调函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if (UartHandle->Instance == USART1)
	{
		if (PushSeqQueue(&g_tUart1.receiveQueue, &g_tUart1.receiveByte, PushSeqQueueU8_CallBack) == STATUS_DATA_STRUCT_TRUE)
		{
			HAL_UART_Receive_IT(&g_Uart1Handle, &g_tUart1.receiveByte, 1);
		}
	}
	
	if (UartHandle->Instance == USART6)
	{
		if (PushSeqQueue(&g_tUart6.receiveQueue, &g_tUart6.receiveByte, PushSeqQueueU8_CallBack) == STATUS_DATA_STRUCT_TRUE)
		{
			HAL_UART_Receive_IT(&g_Uart6Handle, &g_tUart6.receiveByte, 1);
		}
	}
}

/*
*********************************************************************************************************
* Function Name : bsp_fputc
* Description	: fputc处理
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__weak void bsp_fputc( uint8_t ch )
{
	;
}

/*
*********************************************************************************************************
* Function Name : fputc
* Description	: 重定义putc函数，这样可以使用printf函数从串口1打印输出
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
	#if 1	/* 将需要printf的字符通过串口中断FIFO发送出去，printf函数会立即返回 */
		ComSend(DEBUG_COM,(uint8_t *)&ch,1);
		bsp_fputc(ch);
	
		return ch;
	#else	/* 采用阻塞方式发送每个字符,等待数据发送完毕 */
		/* 写一个字节到USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		/* 等待发送结束 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		{
			;
		}
		return ch;
	#endif
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
