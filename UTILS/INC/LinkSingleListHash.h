/*
*********************************************************************************************************
* @file    	LinkSingleListHash.h
* @author  	SY
* @version 	V1.0.0
* @date    	2016-9-14 10:31:57
* @IDE	 	V4.70.0.0
* @Chip    	STM32F407VE
* @brief   	链式哈希表头文件
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
#ifndef __LINK_SINGLE_LIST_HASH_H
#define __LINK_SINGLE_LIST_HASH_H

/*
*********************************************************************************************************
*                              				Exported Includes
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                              				Exported define
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Exported types
*********************************************************************************************************
*/
typedef struct tagLINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef
{
	void *key;	
	void *value;	
	struct tagLINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef *next;
}LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef;

typedef struct
{
	LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef **basePtr;
	uint32_t maxLenth;
}LINK_SINGLE_LIST_HASH_TABLE_TypeDef;

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
* Function Name : DirectAddressGetLinkSingleListHashKeyU32_CallBack
* Description	: 直接地址法获取哈希函数键值
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE uint32_t DirectAddressGetLinkSingleListHashKeyU32_CallBack( LINK_SINGLE_LIST_HASH_TABLE_TypeDef *hashPtr, \
		void *keyPtr )
{
	const int32_t hashCoefA = 1;
	const int32_t hashCoefB = 0;
	uint32_t key = *(uint32_t *)keyPtr;
	
	return ((hashCoefA * key + hashCoefB) % hashPtr->maxLenth);
}

/*
*********************************************************************************************************
* Function Name : KeepDivisorGetLinkSingleListHashKeyU32_CallBack
* Description	: 保留除数法获取哈希函数键值
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE uint32_t KeepDivisorGetLinkSingleListHashKeyU32_CallBack( LINK_SINGLE_LIST_HASH_TABLE_TypeDef *hashPtr, \
		void *keyPtr )
{
	uint32_t key = *(uint32_t *)keyPtr;
	const uint32_t M = hashPtr->maxLenth;	
	
	return ((key % M) % hashPtr->maxLenth);
}

/*
*********************************************************************************************************
* Function Name : MatchLinkSingleListHashKeyU32_CallBack
* Description	: 匹配哈希函数键值
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE bool MatchLinkSingleListHashKeyU32_CallBack( LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef *nodePtr,\
		void *keyPtr )
{
	uint32_t nodeKey = *(uint32_t *)nodePtr->key;
	uint32_t key = *(uint32_t *)keyPtr;
	
	if (nodePtr->key == NULL)
	{
		return false;
	}
	if (nodeKey == key)
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
*                              				Exported variables
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Exported functions
*********************************************************************************************************
*/
void InitLinkSingleListHashTable( LINK_SINGLE_LIST_HASH_TABLE_TypeDef *hashPtr,\
		LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef **pBase, uint32_t lenth );
STATUS_HASH_ENUM SearchLinkSingleListHashTable( LINK_SINGLE_LIST_HASH_TABLE_TypeDef *hashPtr,\
		LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef **nodePtr, void *keyPtr,\
		uint32_t (*getHashKey_CallBack)( LINK_SINGLE_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr ),\
		bool (*matchHashKey_CallBack)( LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef *base, void *keyPtr ) );
STATUS_HASH_ENUM InsertLinkSingleListHashTable( LINK_SINGLE_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr, void *valuePtr,\
		uint32_t (*getHashKey_CallBack)( LINK_SINGLE_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr ),\
		bool (*matchHashKey_CallBack)( LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef *base, void *keyPtr ) );
STATUS_HASH_ENUM DeleteLinkSingleListHashTableNode( LINK_SINGLE_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr,\
		uint32_t (*getHashKey_CallBack)( LINK_SINGLE_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr ),\
		bool (*matchHashKey_CallBack)( LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef *base, void *keyPtr ) );

#endif
/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
