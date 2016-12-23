/*
*********************************************************************************************************
* @file    	LinkSingleList.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-9-8 15:55:25
* @IDE	 	V4.70.0.0
* @Chip    	STM32F407VE
* @brief   	单向链表源文件
*********************************************************************************************************
* @attention
*		单链表是一种链式存取的数据结构，用一组地址任意的存储单元存放线性表中的数据元素。
*   链表中的数据是以节点来表示的，每个节点由两部分构成：一个是数据域，存储数据值。
*	另一个是指针域，存储指向下一个节点的指针。
* 
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private Includes
*********************************************************************************************************
*/
#include "utils.h"
#include "linkSingleList.h"


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
* Function Name : CreateLinkSingleList
* Description	: 创建单向链表(内存空间由内部提供、动态内存)
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
LINK_SINGLE_LIST_TypeDef *CreateLinkSingleList( void )
{
	/* 创建头节点，头指针由外部提供 */
	LINK_SINGLE_LIST_TypeDef *pHead = (LINK_SINGLE_LIST_TypeDef *)calloc(1,sizeof(LINK_SINGLE_LIST_TypeDef));
	if (pHead == NULL)
	{
		return NULL;
	}
	
	pHead->data = NULL;
	pHead->next = NULL;

	return pHead;
}

/*
*********************************************************************************************************
* Function Name : LinkSingleListIsEmpty
* Description	: 单向链表是否为空
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM LinkSingleListIsEmpty( LINK_SINGLE_LIST_TypeDef *pHead )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	if (pHead->next == NULL)
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
* Function Name : GetLinkSingleListLenth
* Description	: 获取单向链表长度
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t GetLinkSingleListLenth( LINK_SINGLE_LIST_TypeDef *pHead )
{
	uint32_t lenth = 0;
	
	if (pHead != NULL)
	{
		LINK_SINGLE_LIST_TypeDef *pNode = pHead;
		
		while (pNode->next != NULL)
		{
			lenth++;
			pNode = pNode->next;
		}
	}
	
	return lenth;
}

/*
*********************************************************************************************************
* Function Name : ClearLinkSingleList
* Description	: 清空单向链表
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void ClearLinkSingleList( LINK_SINGLE_LIST_TypeDef *pHead )
{
	if (pHead != NULL)
	{
		while (pHead->next != NULL)
		{
			LINK_SINGLE_LIST_TypeDef *oldNode = pHead->next;
			pHead->next = oldNode->next;
			oldNode->next = NULL;
			
			if (oldNode->data != NULL)
			{
				free(oldNode->data);
				oldNode->data = NULL;
			}
			
			free(oldNode);
			oldNode = NULL;				
		}
	}
}

/*
*********************************************************************************************************
* Function Name : DestoryLinkSingleList
* Description	: 销毁单向链表
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void DestoryLinkSingleList( LINK_SINGLE_LIST_TypeDef **pHead )
{
	LINK_SINGLE_LIST_TypeDef *this = *pHead;
	
	if (this != NULL)
	{
		ClearLinkSingleList(this);
		
		/* 释放栈顶节点 */
		free(this);
		
		*pHead = NULL;
	}
}

/*
*********************************************************************************************************
* Function Name : InsertLinkSingleList
* Description	: 单向链表插入
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM InsertLinkSingleList( LINK_SINGLE_LIST_TypeDef *pHead, void *dataIn, uint32_t dataSize,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	LINK_SINGLE_LIST_TypeDef *prevNode = pHead;
	{
		LINK_SINGLE_LIST_TypeDef *pNode = pHead->next;	
		while (pNode != NULL)
		{
			if (match_CallBack(pNode->data, matchData) == true)
			{
				break;
			}
			
			prevNode = pNode;
			pNode = pNode->next;
		}
	}
	
	/* 增加新节点 */
	LINK_SINGLE_LIST_TypeDef *pNode = (LINK_SINGLE_LIST_TypeDef *)calloc(1,sizeof(LINK_SINGLE_LIST_TypeDef));
	if (pNode == NULL)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
	pNode->next = prevNode->next;	
	prevNode->next = pNode;
	
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
* Function Name : DeleteLinkSingleList
* Description	: 单向链表删除
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM DeleteLinkSingleList( LINK_SINGLE_LIST_TypeDef *pHead,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	bool isFind = false;
	LINK_SINGLE_LIST_TypeDef *prevNode = pHead;
	{
		LINK_SINGLE_LIST_TypeDef *pNode = pHead->next;	
		while (pNode != NULL)
		{
			if (match_CallBack(pNode->data, matchData) == true)
			{
				isFind = true;
				break;
			}
			
			prevNode = pNode;
			pNode = pNode->next;
		}
	}
	
	if (isFind == false)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	LINK_SINGLE_LIST_TypeDef *oldNode = prevNode->next;	
	prevNode->next = oldNode->next;
	oldNode->next = NULL;

	if (oldNode->data != NULL)
	{
		free(oldNode->data);
		oldNode->data = NULL;
	}
	free(oldNode);
	oldNode = NULL;
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : PushLinkSingleList
* Description	: 压入单向链表
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PushLinkSingleList( LINK_SINGLE_LIST_TypeDef *pHead, void *dataIn, uint32_t dataSize,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	bool isFind = false;	
	LINK_SINGLE_LIST_TypeDef *pNode = pHead->next;	
	while (pNode != NULL)
	{
		if (match_CallBack(pNode->data, matchData) == true)
		{
			isFind = true;
			break;
		}
		
		pNode = pNode->next;
	}

	if (isFind == false)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}	
	
	DATA_STRUCT_STATUS_ENUM isDataOK = STATUS_DATA_STRUCT_FALSE;
	if (pNode->data != NULL)
	{
		memcpy(pNode->data,dataIn,dataSize);
		isDataOK = STATUS_DATA_STRUCT_TRUE;
	}
	
	return isDataOK;
}

/*
*********************************************************************************************************
* Function Name : PopLinkSingleList
* Description	: 弹出单向链表
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PopLinkSingleList( LINK_SINGLE_LIST_TypeDef *pHead, void *dataOut, uint32_t dataSize,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	bool isFind = false;	
	LINK_SINGLE_LIST_TypeDef *pNode = pHead->next;	
	while (pNode != NULL)
	{
		if (match_CallBack(pNode->data, matchData) == true)
		{
			isFind = true;
			break;
		}
		
		pNode = pNode->next;
	}

	if (isFind == false)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}	
	
	DATA_STRUCT_STATUS_ENUM isDataOK = STATUS_DATA_STRUCT_FALSE;
	if (pNode->data != NULL)
	{
		memcpy(dataOut,pNode->data,dataSize);
		isDataOK = STATUS_DATA_STRUCT_TRUE;
	}
	
	return isDataOK;
}

/*
*********************************************************************************************************
* Function Name : ReverseLinkSingleList
* Description	: 逆序单向链表
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM ReverseLinkSingleList( LINK_SINGLE_LIST_TypeDef *pHead )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	if (pHead->next == NULL)
	{
		return STATUS_DATA_STRUCT_TRUE;
	}
	
	LINK_SINGLE_LIST_TypeDef *pNode1 = pHead->next;
	LINK_SINGLE_LIST_TypeDef *pNode2 = pNode1->next;
	
	pNode1->next = NULL;
	pNode1 = pNode2;
	pNode2 = pNode2->next;
	while (pNode1 != NULL)
	{
		pNode1->next = pHead->next;
		pHead->next = pNode1;
		
		pNode1 = pNode2;
		pNode2 = pNode2->next;
	}
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : TraverseLinkSingleList
* Description	: 遍历单向链表
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM TraverseLinkSingleList( LINK_SINGLE_LIST_TypeDef *pHead,\
		void (*show_CallBack)( void *nodeData ) )
{
	LINK_SINGLE_LIST_TypeDef *pNode = pHead;
	
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	while (pNode->next != NULL)
	{
		pNode = pNode->next;
		if (pNode->data == NULL)
		{
			return STATUS_DATA_STRUCT_FALSE;
		}
		show_CallBack(pNode->data);
	}
	
	return STATUS_DATA_STRUCT_TRUE;
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
