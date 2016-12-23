/*
*********************************************************************************************************
* @file    	LinkSingleListHash.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-9-14 10:30:43
* @IDE	 	V4.70.0.0
* @Chip    	STM32F407VE
* @brief   	链式哈希表源文件
*********************************************************************************************************
* @attention
*		哈希表可以快速查找到目标键值对应的元素，如果不存在哈希冲突的情况下，时间复杂度可以达到O(1)。
*	原理：
*	（1）根据一定的编码规则，将目标键值重新编码为哈希表键值，并将数据存储于哈希表键值对应的位置。
*	（2）编码的过程可以理解为加密算法，一般包括：直接地址法，保留除数法。
*	（3）查找目标键值时，仍然通过加密算法将目标键值编码为哈希表键值，查找哈希表，提取存储的数据。
*	（4）通过加密算法得到的哈希表键值，可能会存在重复情况，也即是哈希冲突。
*		一般解决冲突的方法：开放地址法，链地址法。
*	（5）本文通过链地址法解决冲突，大致描述为：首先通过哈希函数计算哈希表键值，读取该键值对应的原始数据，
*		数据中包含已存储的键值，如果键值不为空即为有冲突，必须寻找另一个地址存储新数据。链地址法通过
*		在有冲突的节点后面插入新节点，形成一条链表解决冲突。
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
#include "LinkSingleListHash.h"


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
* Function Name : InitLinkSingleListHashTable
* Description	: 初始化链式哈希表
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void InitLinkSingleListHashTable( LINK_SINGLE_LIST_HASH_TABLE_TypeDef *hashPtr,\
		LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef **pBase, uint32_t lenth )
{
	hashPtr->basePtr = pBase;
	hashPtr->maxLenth = lenth;
	
	memset(pBase, NULL, lenth*sizeof(LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef *));
}

/*
*********************************************************************************************************
* Function Name : SearchLinkSingleListHashTable
* Description	: 搜索链式哈希表
* Input			: hashPtr：哈希表句柄; nodePtr：节点; keyPtr：关键字地址; 
*				: getHashKey_CallBack：将目标键值转换为哈希表键值
*				: matchHashKey_CallBack：目标键值与哈希表存储的键值是否匹配
*				: nextHashKey_CallBack：获取下一个可用的哈希表键值
* Output		: nodePtr：如果找到匹配键值，返回匹配节点。否则返回最后一个有效节点。
* Return		: None
*********************************************************************************************************
*/
STATUS_HASH_ENUM SearchLinkSingleListHashTable( LINK_SINGLE_LIST_HASH_TABLE_TypeDef *hashPtr,\
		LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef **nodePtr, void *keyPtr,\
		uint32_t (*getHashKey_CallBack)( LINK_SINGLE_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr ),\
		bool (*matchHashKey_CallBack)( LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef *base, void *keyPtr ) )
{
	if ((getHashKey_CallBack == NULL) || (matchHashKey_CallBack == NULL))
	{
		return STATUS_HASH_UNDEFINED;
	}
	
	uint32_t hashKey = getHashKey_CallBack(hashPtr, keyPtr);	
	*nodePtr = hashPtr->basePtr[hashKey];	
	while (1)
	{
		if ((*nodePtr) == NULL)
		{
			return STATUS_HASH_FALSE;
		}
		else if (matchHashKey_CallBack((*nodePtr), keyPtr) == true)
		{
			return STATUS_HASH_TRUE;
		}
		else if ((*nodePtr)->next == NULL)
		{
			return STATUS_HASH_FALSE;
		}
		else
		{
			*nodePtr = (*nodePtr)->next;
		}
	}
}

/*
*********************************************************************************************************
* Function Name : InsertLinkSingleListHashTable
* Description	: 插入链式哈希表
* Input			: hashPtr：哈希表句柄; nodePtr:节点
*				: getHashKey_CallBack：将目标键值转换为哈希表键值
*				: matchHashKey_CallBack：目标键值与哈希表存储的键值是否匹配
*				: insertHashValue_CallBack：在哈希表中插入新节点
* Output		: None
* Return		: None
*********************************************************************************************************
*/
STATUS_HASH_ENUM InsertLinkSingleListHashTable( LINK_SINGLE_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr, void *valuePtr,\
		uint32_t (*getHashKey_CallBack)( LINK_SINGLE_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr ),\
		bool (*matchHashKey_CallBack)( LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef *base, void *keyPtr ) )
{
	if ((getHashKey_CallBack == NULL) || (matchHashKey_CallBack == NULL))
	{
		return STATUS_HASH_UNDEFINED;
	}
	
	LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef tempNode;
	LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef *lastNodePtr = &tempNode;
	STATUS_HASH_ENUM status = SearchLinkSingleListHashTable(hashPtr, &lastNodePtr, keyPtr,\
								getHashKey_CallBack, matchHashKey_CallBack);
	
	if (status == STATUS_HASH_FALSE)
	{
		LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef *newNodePtr = \
			(LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef *)calloc(1,sizeof(LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef));
		if (newNodePtr == NULL)
		{
			return STATUS_HASH_FALSE;
		}
		
		newNodePtr->key = keyPtr;
		newNodePtr->value = valuePtr;
		newNodePtr->next = NULL;
		
		/* 头结点为空，将新结点插入头结点 */	
		if (lastNodePtr == NULL)
		{		
			uint32_t hashKey = getHashKey_CallBack(hashPtr, keyPtr);	
			hashPtr->basePtr[hashKey] = newNodePtr;
		}
		else
		{		
			lastNodePtr->next = newNodePtr;
		}
		
		return STATUS_HASH_TRUE;
	}
	else if (status == STATUS_HASH_TRUE)
	{
		return STATUS_HASH_FALSE;
	}
	else
	{
		return STATUS_HASH_UNDEFINED;
	}
}

/*
*********************************************************************************************************
* Function Name : DeleteLinkSingleListHashTableNode
* Description	: 删除链式哈希表节点
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
STATUS_HASH_ENUM DeleteLinkSingleListHashTableNode( LINK_SINGLE_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr,\
		uint32_t (*getHashKey_CallBack)( LINK_SINGLE_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr ),\
		bool (*matchHashKey_CallBack)( LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef *base, void *keyPtr ) )
{
	if ((getHashKey_CallBack == NULL) || (matchHashKey_CallBack == NULL) )
	{
		return STATUS_HASH_UNDEFINED;
	}
	
	uint32_t hashKey = getHashKey_CallBack(hashPtr, keyPtr);
	LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef *nodePtr = hashPtr->basePtr[hashKey];
	LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef *prevNodePtr = nodePtr;
	while (1)
	{
		if (nodePtr == NULL)
		{
			return STATUS_HASH_FALSE;
		}
		else if (matchHashKey_CallBack(nodePtr, keyPtr) == true)
		{
			/* 头结点 */
			if (nodePtr == hashPtr->basePtr[hashKey])
			{
				hashPtr->basePtr[hashKey] = nodePtr->next;
			}
			else
			{
				prevNodePtr->next = nodePtr->next;
			}
			free(nodePtr);
			nodePtr = NULL;
			
			return STATUS_HASH_TRUE;
		}
		else
		{
			prevNodePtr = nodePtr;
			nodePtr = nodePtr->next;
		}
	}
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
