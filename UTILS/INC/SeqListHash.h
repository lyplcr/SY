/*
*********************************************************************************************************
* @file    	SeqListHash.h
* @author  	SY
* @version 	V1.0.0
* @date    	2016-9-13 15:26:08
* @IDE	 	V4.70.0.0
* @Chip    	STM32F407VE
* @brief   	顺序哈希表头文件
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
#ifndef __SEQ_LIST_HASH_H
#define __SEQ_LIST_HASH_H

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
typedef struct
{
	void *key;	
	void *value;	
}SEQ_LIST_HASH_TABLE_NODE_TypeDef;

typedef struct
{
	void *basePtr;
	uint32_t maxLenth;
}SEQ_LIST_HASH_TABLE_TypeDef;

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
* Function Name : DirectAddressGetSeqListHashKeyU32_CallBack
* Description	: 直接地址法获取哈希函数键值
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE bool DirectAddressGetSeqListHashKeyU32_CallBack( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, \
		void *keyPtr, uint32_t *hashKeyPtr )
{
	const int32_t hashCoefA = 1;
	const int32_t hashCoefB = 0;
	uint32_t key = *(uint32_t *)keyPtr;
	
	int32_t hashKey = hashCoefA * key + hashCoefB;
	if ((hashKey < 0) || (hashKey >= hashPtr->maxLenth)) 
	{
		return false;
	}
	
	*hashKeyPtr = hashKey;
	
	return true;
}

/*
*********************************************************************************************************
* Function Name : KeepDivisorGetSeqListHashKeyU32_CallBack
* Description	: 保留除数法获取哈希函数键值
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE bool KeepDivisorGetSeqListHashKeyU32_CallBack( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, \
		void *keyPtr, uint32_t *hashKeyPtr )
{
	uint32_t key = *(uint32_t *)keyPtr;
	const uint32_t M = 5;
	
	int32_t hashKey = key % M;
	if ((hashKey < 0) || (hashKey >= hashPtr->maxLenth)) 
	{
		return false;
	}
	*hashKeyPtr = hashKey;
	
	return true;
}

/*
*********************************************************************************************************
* Function Name : MatchSeqListHashKeyU32_CallBack
* Description	: 匹配哈希函数键值
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE bool MatchSeqListHashKeyU32_CallBack( SEQ_LIST_HASH_TABLE_NODE_TypeDef *base,\
		uint32_t hashKey, void *keyPtr )
{
	SEQ_LIST_HASH_TABLE_NODE_TypeDef *nodePtr = base + hashKey;	
	
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
* Function Name : OpenAddressMakeNextSeqListHashKey_CallBack
* Description	: 开放地址法生成下一条哈希表键值
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE bool OpenAddressMakeNextSeqListHashKey_CallBack( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr,\
		uint32_t *hashKeyPtr )
{
	const int32_t INC = 1;
	uint32_t hashKey = (uint32_t)(*hashKeyPtr + INC);
	
	if (hashKey < hashPtr->maxLenth)
	{
		*hashKeyPtr = hashKey;
		
		return true;
	}
	else
	{
		return false;
	}
}

/*
*********************************************************************************************************
* Function Name : InsertSeqListHashValue_CallBack
* Description	: 插入顺序哈希表值
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE void InsertSeqListHashValue_CallBack( SEQ_LIST_HASH_TABLE_NODE_TypeDef *base,\
		uint32_t hashKey, void *keyPtr, void *valuePtr )
{
	SEQ_LIST_HASH_TABLE_NODE_TypeDef *nodePtr = base + hashKey;

	nodePtr->key = keyPtr;
	nodePtr->value = valuePtr;
}

/*
*********************************************************************************************************
* Function Name : DeleteSeqListHashValue_CallBack
* Description	: 删除顺序哈希表值
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE void DeleteSeqListHashValue_CallBack( SEQ_LIST_HASH_TABLE_NODE_TypeDef *base,\
		uint32_t hashKey )
{
	SEQ_LIST_HASH_TABLE_NODE_TypeDef *nodePtr = base + hashKey;

	nodePtr->key = NULL;
	nodePtr->value = NULL;
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
void InitSeqListHashTable( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, void *pBase, uint32_t lenth );
STATUS_HASH_ENUM SearchSeqListHashTable( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr, uint32_t *hashKeyPtr,\
		bool (*getHashKey_CallBack)( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr, uint32_t *hashKeyPtr ),\
		bool (*matchHashKey_CallBack)( SEQ_LIST_HASH_TABLE_NODE_TypeDef *base, uint32_t hashKey, void *keyPtr ),\
		bool (*nextHashKey_CallBack)( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, uint32_t *hashKeyPtr ) );
STATUS_HASH_ENUM InsertSeqListHashTable( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr, void *valuePtr,\
		bool (*getHashKey_CallBack)( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr, uint32_t *hashKeyPtr ),\
		bool (*matchHashKey_CallBack)( SEQ_LIST_HASH_TABLE_NODE_TypeDef *base, uint32_t hashKey, void *keyPtr ),\
		bool (*nextHashKey_CallBack)( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, uint32_t *hashKeyPtr ),\
		void (*insertHashValue_CallBack)( SEQ_LIST_HASH_TABLE_NODE_TypeDef *base, uint32_t hashKey, \
			void *keyPtr, void *valuePtr ));
STATUS_HASH_ENUM DeleteSeqListHashTable( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr,\
		bool (*getHashKey_CallBack)( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr, uint32_t *hashKeyPtr ),\
		bool (*matchHashKey_CallBack)( SEQ_LIST_HASH_TABLE_NODE_TypeDef *base, uint32_t hashKey, void *keyPtr ),\
		bool (*nextHashKey_CallBack)( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, uint32_t *hashKeyPtr ),\
		void (*deleteHashValue_CallBack)( SEQ_LIST_HASH_TABLE_NODE_TypeDef *base, uint32_t hashKey));
		

#endif
/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
