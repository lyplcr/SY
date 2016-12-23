/*
*********************************************************************************************************
* @file    	LinkSingleCycleList.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-9-9 13:49:34
* @IDE	 	V4.70.0.0
* @Chip    	STM32F407VE
* @brief   	单向循环链表源文件
*********************************************************************************************************
* @attention
*		单向循环链表是单链表的一种改进，若将单链表的首尾节点相连，便构成单向循环链表结构。
* 
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private Includes
*********************************************************************************************************
*/
#include "utils.h"
#include "LinkSingleCycleList.h"


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
* Function Name : CreateLinkSingleCycleList
* Description	: 创建单向循环链表(内存空间由内部提供、动态内存)
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
LINK_SINGLE_CYCLE_LIST_TypeDef *CreateLinkSingleCycleList( void )
{
	/* 创建头节点，头指针由外部提供 */
	LINK_SINGLE_CYCLE_LIST_TypeDef *pHead = (LINK_SINGLE_CYCLE_LIST_TypeDef *)calloc(1,sizeof(LINK_SINGLE_CYCLE_LIST_TypeDef));
	if (pHead == NULL)
	{
		return NULL;
	}
	
	pHead->data = NULL;
	pHead->next = pHead;

	return pHead;
}

/*
*********************************************************************************************************
* Function Name : LinkSingleCycleListIsEmpty
* Description	: 单向循环链表是否为空
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM LinkSingleCycleListIsEmpty( LINK_SINGLE_CYCLE_LIST_TypeDef *pHead )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	if (pHead->next == pHead)
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
* Function Name : GetLinkSingleCycleListLenth
* Description	: 获取单向循环链表长度
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t GetLinkSingleCycleListLenth( LINK_SINGLE_CYCLE_LIST_TypeDef *pHead )
{
	uint32_t lenth = 0;
	
	if (pHead != NULL)
	{
		LINK_SINGLE_CYCLE_LIST_TypeDef *pNode = pHead;
		
		while (pNode->next != pHead)
		{
			lenth++;
			pNode = pNode->next;
		}
	}
	
	return lenth;
}

/*
*********************************************************************************************************
* Function Name : ClearLinkSingleCycleList
* Description	: 清空单向循环链表
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void ClearLinkSingleCycleList( LINK_SINGLE_CYCLE_LIST_TypeDef *pHead )
{
	if (pHead != NULL)
	{
		while (pHead->next != pHead)
		{
			LINK_SINGLE_CYCLE_LIST_TypeDef *oldNode = pHead->next;
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
* Function Name : DestoryLinkSingleCycleList
* Description	: 销毁单向循环链表
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void DestoryLinkSingleCycleList( LINK_SINGLE_CYCLE_LIST_TypeDef **pHead )
{
	LINK_SINGLE_CYCLE_LIST_TypeDef *this = *pHead;
	
	if (this != NULL)
	{
		ClearLinkSingleCycleList(this);		
		free(this);		
		*pHead = NULL;
	}
}

/*
*********************************************************************************************************
* Function Name : InsertLinkSingleCycleList
* Description	: 单向循环链表插入
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM InsertLinkSingleCycleList( LINK_SINGLE_CYCLE_LIST_TypeDef *pHead, void *dataIn, uint32_t dataSize,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	LINK_SINGLE_CYCLE_LIST_TypeDef *prevNode = pHead;
	{
		LINK_SINGLE_CYCLE_LIST_TypeDef *pNode = pHead->next;	
		while (pNode != pHead)
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
	LINK_SINGLE_CYCLE_LIST_TypeDef *pNode = (LINK_SINGLE_CYCLE_LIST_TypeDef *)calloc(1,sizeof(LINK_SINGLE_CYCLE_LIST_TypeDef));
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
* Function Name : DeleteLinkSingleCycleList
* Description	: 单向循环链表删除
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM DeleteLinkSingleCycleList( LINK_SINGLE_CYCLE_LIST_TypeDef *pHead,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	bool isFind = false;
	LINK_SINGLE_CYCLE_LIST_TypeDef *prevNode = pHead;
	{
		LINK_SINGLE_CYCLE_LIST_TypeDef *pNode = pHead->next;	
		while (pNode != pHead)
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
	
	LINK_SINGLE_CYCLE_LIST_TypeDef *oldNode = prevNode->next;	
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
* Function Name : PushLinkSingleCycleList
* Description	: 压入单向循环链表
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PushLinkSingleCycleList( LINK_SINGLE_CYCLE_LIST_TypeDef *pHead, void *dataIn, uint32_t dataSize,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	bool isFind = false;
	LINK_SINGLE_CYCLE_LIST_TypeDef *pNode = pHead->next;	
	while (pNode != pHead)
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
* Function Name : PopLinkSingleCycleList
* Description	: 弹出单向循环链表
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PopLinkSingleCycleList( LINK_SINGLE_CYCLE_LIST_TypeDef *pHead, void *dataOut, uint32_t dataSize,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	bool isFind = false;
	LINK_SINGLE_CYCLE_LIST_TypeDef *pNode = pHead->next;	
	while (pNode != pHead)
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
* Function Name : ReverseLinkSingleCycleList
* Description	: 逆序单向循环链表
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM ReverseLinkSingleCycleList( LINK_SINGLE_CYCLE_LIST_TypeDef *pHead )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	if (pHead->next == NULL)
	{
		return STATUS_DATA_STRUCT_TRUE;
	}
	
	LINK_SINGLE_CYCLE_LIST_TypeDef *pNode1 = pHead->next;
	LINK_SINGLE_CYCLE_LIST_TypeDef *pNode2 = pNode1->next;
	
	pNode1->next = pHead;
	pNode1 = pNode2;
	pNode2 = pNode2->next;
	while (pNode1 != pHead)
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
* Function Name : TraverseLinkSingleCycleList
* Description	: 遍历单向循环链表
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM TraverseLinkSingleCycleList( LINK_SINGLE_CYCLE_LIST_TypeDef *pHead,\
		void (*show_CallBack)( void *nodeData ) )
{
	LINK_SINGLE_CYCLE_LIST_TypeDef *pNode = pHead;
	
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	while (pNode->next != pHead)
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
