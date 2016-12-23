/*
*********************************************************************************************************
* @file    	SeqQueue.c
* @author  	SY
* @version 	V1.0.2
* @date    	2016-9-12 09:11:43
* @IDE	 	V4.70.0.0
* @Chip    	STM32F407VE
* @brief   	顺序队列源文件
*********************************************************************************************************
* @attention
*	队列是限制在两端进行插入操作和删除操作的线性表，允许进行存入操作的一端称为“队尾”，
* 	允许进行删除操作的一端称为“队头”。当线性表中没有元素时，称为“空队”。特点：先进先出（FIFO）。
* ---------------------------------------------------------
* 版本：V1.0.1 	修改人：SY		修改日期：2016-9-1 09:59:09	
* 
* 1、修改队列头尾顺序，将“头部插入，尾部删除”修改为“尾部插入，头部删除”!
* -------------------------------------------------------------------------------------------------------
* 版本：V1.0.2 	修改人：SY		修改日期：2016-9-12 09:12:17
* 
* 1、增加顺序队列的遍历。
* -------------------------------------------------------------------------------------------------------	
* 
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private Includes
*********************************************************************************************************
*/
#include "utils.h"
#include "SeqQueue.h"


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
* Function Name : CreateSeqQueue
* Description	: 创建顺序队列(内存空间由外部提供、静态内存)
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void CreateSeqQueue( SEQUEUE_TypeDef *queuePtr, void *pBase, uint32_t lenth )
{
	queuePtr->basePtr = pBase;
	queuePtr->maxLenth = lenth;
	queuePtr->front = 0;
	queuePtr->rear = 0;
}

/*
*********************************************************************************************************
* Function Name : SeqQueueIsFull
* Description	: 顺序队列是否为满
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM SeqQueueIsFull( SEQUEUE_TypeDef *queuePtr )
{
	if (((queuePtr->rear + 1) % queuePtr->maxLenth) == queuePtr->front)
	{
		return STATUS_DATA_STRUCT_TRUE;
	}
	else
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
}

/*
*********************************************************************************************************
* Function Name : SeqQueueIsEmpty
* Description	: 顺序队列是否为空
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM SeqQueueIsEmpty( SEQUEUE_TypeDef *queuePtr )
{
	if (queuePtr->front == queuePtr->rear)
	{
		return STATUS_DATA_STRUCT_TRUE;
	}
	else
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
}

/*
*********************************************************************************************************
* Function Name : ClearSeqQueue
* Description	: 清空顺序队列
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void ClearSeqQueue( SEQUEUE_TypeDef *queuePtr )
{
	queuePtr->front = 0;
	queuePtr->rear = 0;
}

/*
*********************************************************************************************************
* Function Name : GetSeqQueueFront
* Description	: 获取顺序队列头节点
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t GetSeqQueueFront( SEQUEUE_TypeDef *queuePtr )
{
	return queuePtr->front;
}

/*
*********************************************************************************************************
* Function Name : GetSeqQueueRear
* Description	: 获取顺序队列尾节点
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t GetSeqQueueRear( SEQUEUE_TypeDef *queuePtr )
{
	return queuePtr->rear;
}

/*
*********************************************************************************************************
* Function Name : GetSeqQueueLenth
* Description	: 获取顺序队列长度
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t GetSeqQueueLenth( SEQUEUE_TypeDef *queuePtr )
{
	return queuePtr->maxLenth;
}

/*
*********************************************************************************************************
* Function Name : PushSeqQueue
* Description	: 顺序队列入队
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PushSeqQueue( SEQUEUE_TypeDef *queuePtr, void *dataIn,\
		void (*push_CallBack)( void *base, uint32_t offset, void *data ) )
{
	if (SeqQueueIsFull(queuePtr) == STATUS_DATA_STRUCT_TRUE)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	push_CallBack(queuePtr->basePtr, queuePtr->rear, dataIn);
	queuePtr->rear = (queuePtr->rear + 1) % queuePtr->maxLenth;
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : PopSeqQueue
* Description	: 顺序队列出队
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PopSeqQueue( SEQUEUE_TypeDef *queuePtr, void *dataOut,\
		void (*pop_CallBack)( void *base, uint32_t offset, void *data ) )
{
	if (SeqQueueIsEmpty(queuePtr) == STATUS_DATA_STRUCT_TRUE)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	pop_CallBack(queuePtr->basePtr, queuePtr->front, dataOut);
	queuePtr->front = (queuePtr->front + 1) % queuePtr->maxLenth;
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : GetSeqQueueElement
* Description	: 获取顺序队列元素
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM GetSeqQueueElement( SEQUEUE_TypeDef *queuePtr, void *dataOut,\
		void (*pop_CallBack)( void *base, uint32_t offset, void *data ) )
{
	SEQUEUE_TypeDef queue = *queuePtr;
	
	return PopSeqQueue(&queue,dataOut,pop_CallBack);
}

/*
*********************************************************************************************************
* Function Name : GetSeqQueueDisposeDataRate
* Description	: 获取顺序队列未处理的数据率(最大只能到达80%，因为是循环队列，
*				: 为了区分队列满、队列空，有一个数据点没用)
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint8_t GetSeqQueueDisposeDataRate( SEQUEUE_TypeDef *queuePtr )
{
	uint32_t front = queuePtr->front;
	uint32_t rear = queuePtr->rear;
	uint32_t lenth = queuePtr->maxLenth;
	uint32_t diff = 0;
	
	if (rear >= front)
	{
		diff = rear - front;
	}
	else
	{
		diff = lenth + rear - front;
	}
	
	return ((float)diff / lenth * 100);
}

/*
*********************************************************************************************************
* Function Name : TraverseSeqQueue
* Description	: 遍历顺序队列
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void TraverseSeqQueue( SEQUEUE_TypeDef *queuePtr, void *dataOut,\
		void (*pop_CallBack)( void *base, uint32_t offset, void *data ),\
		void (*show_CallBack)( void *dataOut ) )
{
	SEQUEUE_TypeDef queue = *queuePtr;
	
	while ( PopSeqQueue(&queue, dataOut, pop_CallBack) == STATUS_DATA_STRUCT_TRUE)
	{
		if (show_CallBack)
		{
			show_CallBack(dataOut);
		}
	}
}


/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
