/*
*********************************************************************************************************
* @file    	LinkQueue.c
* @author  	SY
* @version 	V1.0.1
* @date    	2016-9-12 13:08:32
* @IDE	 	V4.70.0.0
* @Chip    	STM32F407VE
* @brief   	链式队列源文件
*********************************************************************************************************
* @attention
*	队列是限制在两端进行插入操作和删除操作的线性表，允许进行存入操作的一端称为“队尾”，
* 	允许进行删除操作的一端称为“队头”。当线性表中没有元素时，称为“空队”。特点：先进先出（FIFO）。
* -------------------------------------------------------------------------------------------------------
* 版本：V1.0.1 	修改人：SY		修改日期：2016-9-12 13:08:40
* 
* 1、增加链式队列的遍历。
* -------------------------------------------------------------------------------------------------------	
*	
*
* 
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private Includes
*********************************************************************************************************
*/
#include "utils.h"
#include "LinkQueue.h"


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
* Function Name : CreateLinkQueue
* Description	: 创建链式队列(内存空间由内部提供、动态内存)
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
LINK_QUEUE_TypeDef *CreateLinkQueue( void )
{
	/* 生成头指针 */
	LINK_QUEUE_TypeDef *pHead = (LINK_QUEUE_TypeDef *)calloc(1,sizeof(LINK_QUEUE_TypeDef));
	if (pHead == NULL)
	{
		return NULL;
	}
	
	/* 生成头结点 */
	LINK_QUEUE_NODE_TypeDef *pNode = (LINK_QUEUE_NODE_TypeDef *)calloc(1,sizeof(LINK_QUEUE_NODE_TypeDef));
	if (pNode == NULL)
	{
		return NULL;
	}
	
	pNode->data = NULL;
	pNode->next = NULL;
	pHead->front = pNode;
	pHead->rear = pNode;

	return pHead;
}

/*
*********************************************************************************************************
* Function Name : LinkQueueIsEmpty
* Description	: 链式队列是否为空
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM LinkQueueIsEmpty( LINK_QUEUE_TypeDef *pHead )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	if (pHead->front->next == NULL)
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
* Function Name : ClearLinkQueue
* Description	: 清空链式队列
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void ClearLinkQueue( LINK_QUEUE_TypeDef *pHead )
{
	if (pHead != NULL)
	{
		LINK_QUEUE_NODE_TypeDef *oldNode = NULL;
		
		while (pHead->front->next != NULL)
		{
			oldNode = pHead->front->next;
			pHead->front->next = oldNode->next;
			oldNode->next = NULL;
			
			if (oldNode->data != NULL)
			{
				free(oldNode->data);
				oldNode->data = NULL;
			}
			
			free(oldNode);
			oldNode = NULL;				
		}
		
		pHead->rear = pHead->front;
	}
}

/*
*********************************************************************************************************
* Function Name : DestoryLinkQueue
* Description	: 销毁链式队列
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void DestoryLinkQueue( LINK_QUEUE_TypeDef **pHead )
{
	LINK_QUEUE_TypeDef *this = *pHead;
	
	if (this != NULL)
	{
		ClearLinkQueue(this);
		
		if (this->front != NULL)
		{
			/* 释放头结点 */
			free(this->front);
			this->front = this->rear = NULL;
			
			/* 释放头指针 */
			free(this);
			
			*pHead = NULL;
		}
	}
}

/*
*********************************************************************************************************
* Function Name : PushLinkQueue
* Description	: 链式队列入队
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PushLinkQueue( LINK_QUEUE_TypeDef *pHead, void *dataIn, uint32_t dataSize )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	/* 增加新节点 */
	LINK_QUEUE_NODE_TypeDef *pNode = (LINK_QUEUE_NODE_TypeDef *)calloc(1,sizeof(LINK_QUEUE_NODE_TypeDef));
	if (pNode == NULL)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
	pNode->next = NULL;	
	
	if (pHead->front->next == NULL)
	{
		pHead->rear = pHead->front;
	}
	pHead->rear->next = pNode;
	pHead->rear = pNode;
	
	/* 存储用户数据 */
	pNode->data = (void *)calloc(1,dataSize);
	if (pNode->data == NULL)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}	
	memcpy(pNode->data,dataIn,dataSize);
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : PopLinkQueue
* Description	: 链式队列出队
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PopLinkQueue( LINK_QUEUE_TypeDef *pHead, void *dataOut, uint32_t dataSize )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	if (LinkQueueIsEmpty(pHead) == STATUS_DATA_STRUCT_TRUE)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	/* 出队时，头指针始终指向头结点，出队头结点后面的节点 */
	LINK_QUEUE_NODE_TypeDef *oldNode = pHead->front->next;	
	pHead->front->next = oldNode->next;
	oldNode->next = NULL;
	if (pHead->front->next == NULL)
	{
		pHead->rear = pHead->front;
	}
	
	DATA_STRUCT_STATUS_ENUM isDataOK = STATUS_DATA_STRUCT_FALSE;
	if (oldNode->data != NULL)
	{
		memcpy(dataOut,oldNode->data,dataSize);
		free(oldNode->data);
		oldNode->data = NULL;
		isDataOK = STATUS_DATA_STRUCT_TRUE;
	}
	free(oldNode);
	oldNode = NULL;
	
	return isDataOK;
}

/*
*********************************************************************************************************
* Function Name : GetLinkQueueElement
* Description	: 获取链式队列元素
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM GetLinkQueueElement( LINK_QUEUE_TypeDef *pHead, void *dataOut, uint32_t dataSize )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	if (LinkQueueIsEmpty(pHead) == STATUS_DATA_STRUCT_TRUE)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	/* 出队时，头指针始终指向头结点，出队头结点后面的节点 */
	DATA_STRUCT_STATUS_ENUM isDataOK = STATUS_DATA_STRUCT_FALSE;
	LINK_QUEUE_NODE_TypeDef *oldNode = pHead->front->next;		
	if (oldNode->data != NULL)
	{
		memcpy(dataOut,oldNode->data,dataSize);
		isDataOK = STATUS_DATA_STRUCT_TRUE;
	}
	
	return isDataOK;
}

/*
*********************************************************************************************************
* Function Name : TraverseLinkQueue
* Description	: 遍历链式队列
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void TraverseLinkQueue( LINK_QUEUE_TypeDef *pHead, void *dataOut, uint32_t dataSize,\
		void (*show_CallBack)( void *dataOut ) )
{
	LINK_QUEUE_TypeDef queue = *pHead;
	LINK_QUEUE_NODE_TypeDef *pNode = queue.front->next;
	
	while (pNode != NULL)
	{
		memcpy(dataOut,pNode->data,dataSize);
		
		if (show_CallBack)
		{
			show_CallBack(dataOut);
		}
		
		pNode = pNode->next;
	}
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
