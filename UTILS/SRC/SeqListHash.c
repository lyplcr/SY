/*
*********************************************************************************************************
* @file    	SeqListHash.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-9-13 15:26:08
* @IDE	 	V4.70.0.0
* @Chip    	STM32F407VE
* @brief   	顺序哈希表源文件
*********************************************************************************************************
* @attention
*		哈希表可以快速查找到目标键值对应的元素，如果不存在哈希冲突的情况下，时间复杂度可以达到O(1)。
*	原理：
*	（1）根据一定的编码规则，将目标键值重新编码为哈希表键值，并将数据存储于哈希表键值对应的位置。
*	（2）编码的过程可以理解为加密算法，一般包括：直接地址法，保留除数法。
*	（3）查找目标键值时，仍然通过加密算法将目标键值编码为哈希表键值，查找哈希表，提取存储的数据。
*	（4）通过加密算法得到的哈希表键值，可能会存在重复情况，也即是哈希冲突。
*		一般解决冲突的方法：开放地址法，链地址法。
*	（5）本文通过开放地址法解决冲突，大致描述为：首先通过哈希函数计算哈希表键值，读取该键值对应的原始数据，
*		数据中包含已存储的键值，如果键值不为空即为有冲突，必须寻找另一个地址存储新数据。
*		公式为：Hash(key[New]) = (Hash(key[Old]) + di) % M,(M表示哈希表的长度)。
*		开放地址法一般通过在哈希表键值附近，按以下方法改变键值：
*			1）di = 1, 2, 3...M-1, 称为线性探测再散列
*			2）di = 1^2, -1^2, 2^2, -2^2...k^2,-k^2(k<=M/2)，称为二次探测再散列。
*			3）di取随机序列,称为随机探测
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
#include "SeqListHash.h"


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
* Function Name : InitSeqListHashTable
* Description	: 初始化顺序哈希表
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void InitSeqListHashTable( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, void *pBase, uint32_t lenth )
{
	hashPtr->basePtr = pBase;
	hashPtr->maxLenth = lenth;
	
	memset(pBase, NULL, lenth*sizeof(SEQ_LIST_HASH_TABLE_NODE_TypeDef));
}

/*
*********************************************************************************************************
* Function Name : SearchSeqListHashTable
* Description	: 搜索顺序哈希表
* Input			: hashPtr：哈希表句柄; keyPtr：关键字地址; hashKeyPtr:哈希表键值
*				: getHashKey_CallBack：将目标键值转换为哈希表键值
*				: matchHashKey_CallBack：目标键值与哈希表存储的键值是否匹配
*				: nextHashKey_CallBack：获取下一个可用的哈希表键值
* Output		: None
* Return		: None
*********************************************************************************************************
*/
STATUS_HASH_ENUM SearchSeqListHashTable( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr, uint32_t *hashKeyPtr,\
		bool (*getHashKey_CallBack)( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr, uint32_t *hashKeyPtr ),\
		bool (*matchHashKey_CallBack)( SEQ_LIST_HASH_TABLE_NODE_TypeDef *base, uint32_t hashKey, void *keyPtr ),\
		bool (*nextHashKey_CallBack)( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, uint32_t *hashKeyPtr ) )
{
	if ((getHashKey_CallBack == NULL) || (matchHashKey_CallBack == NULL) || \
		(nextHashKey_CallBack == NULL))
	{
		return STATUS_HASH_UNDEFINED;
	}
	
	if (getHashKey_CallBack(hashPtr, keyPtr, hashKeyPtr) == false)
	{
		return STATUS_HASH_UNDEFINED;
	}
		
	while (1)
	{
		SEQ_LIST_HASH_TABLE_NODE_TypeDef *nodePtr = (SEQ_LIST_HASH_TABLE_NODE_TypeDef *)hashPtr->basePtr + *hashKeyPtr;
		
		/* 表中无记录 */
		if (nodePtr->key == NULL)
		{
			return STATUS_HASH_FALSE;
		}
		else if (matchHashKey_CallBack(hashPtr->basePtr, *hashKeyPtr, keyPtr) == true)
		{
			return STATUS_HASH_TRUE;
		}
		else if (nextHashKey_CallBack(hashPtr, hashKeyPtr) == false)
		{
			return STATUS_HASH_UNDEFINED;
		}
	}
}

/*
*********************************************************************************************************
* Function Name : InsertSeqListHashTable
* Description	: 插入顺序哈希表
* Input			: hashPtr：哈希表句柄; keyPtr：关键字地址; valuePtr：数据的地址
*				: getHashKey_CallBack：将目标键值转换为哈希表键值
*				: matchHashKey_CallBack：目标键值与哈希表存储的键值是否匹配
*				: nextHashKey_CallBack：获取下一个可用的哈希表键值
*				: insertHashValue_CallBack：在哈希表中插入新节点
* Output		: None
* Return		: None
*********************************************************************************************************
*/
STATUS_HASH_ENUM InsertSeqListHashTable( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr, void *valuePtr,\
		bool (*getHashKey_CallBack)( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr, uint32_t *hashKeyPtr ),\
		bool (*matchHashKey_CallBack)( SEQ_LIST_HASH_TABLE_NODE_TypeDef *base, uint32_t hashKey, void *keyPtr ),\
		bool (*nextHashKey_CallBack)( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, uint32_t *hashKeyPtr ),\
		void (*insertHashValue_CallBack)( SEQ_LIST_HASH_TABLE_NODE_TypeDef *base, uint32_t hashKey, \
			void *keyPtr, void *valuePtr ))
{
	if ((getHashKey_CallBack == NULL) || (matchHashKey_CallBack == NULL) ||\
		(nextHashKey_CallBack == NULL) || (insertHashValue_CallBack == NULL))
	{
		return STATUS_HASH_UNDEFINED;
	}
	
	uint32_t hashKey = 0;
	STATUS_HASH_ENUM status = SearchSeqListHashTable(hashPtr, keyPtr, &hashKey,\
								getHashKey_CallBack,\
								matchHashKey_CallBack,\
								nextHashKey_CallBack);
	
	switch( status )
	{
		case STATUS_HASH_FALSE:
			insertHashValue_CallBack(hashPtr->basePtr, hashKey, keyPtr, valuePtr);
			return STATUS_HASH_TRUE;
		
		case STATUS_HASH_TRUE:
			return STATUS_HASH_FALSE;
		
		default:
			return STATUS_HASH_UNDEFINED;
	}
}

/*
*********************************************************************************************************
* Function Name : DeleteSeqListHashTable
* Description	: 删除顺序哈希表
*				: getHashKey_CallBack：将目标键值转换为哈希表键值
*				: matchHashKey_CallBack：目标键值与哈希表存储的键值是否匹配
*				: nextHashKey_CallBack：获取下一个可用的哈希表键值
* Output		: None
* Return		: None
*********************************************************************************************************
*/
STATUS_HASH_ENUM DeleteSeqListHashTable( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr,\
		bool (*getHashKey_CallBack)( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr, uint32_t *hashKeyPtr ),\
		bool (*matchHashKey_CallBack)( SEQ_LIST_HASH_TABLE_NODE_TypeDef *base, uint32_t hashKey, void *keyPtr ),\
		bool (*nextHashKey_CallBack)( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, uint32_t *hashKeyPtr ),\
		void (*deleteHashValue_CallBack)( SEQ_LIST_HASH_TABLE_NODE_TypeDef *base, uint32_t hashKey))
{
	if ((getHashKey_CallBack == NULL) || (matchHashKey_CallBack == NULL) || \
		(nextHashKey_CallBack == NULL) || (deleteHashValue_CallBack == NULL))
	{
		return STATUS_HASH_UNDEFINED;
	}
	
	uint32_t hashKey = 0;
	if (getHashKey_CallBack(hashPtr, keyPtr, &hashKey) == false)
	{
		return STATUS_HASH_UNDEFINED;
	}
		
	while (1)
	{
		if (matchHashKey_CallBack(hashPtr->basePtr, hashKey, keyPtr) == true)
		{
			deleteHashValue_CallBack(hashPtr->basePtr, hashKey);
			
			return STATUS_HASH_TRUE;
		}
		else if (nextHashKey_CallBack(hashPtr, &hashKey) == false)
		{
			return STATUS_HASH_FALSE;
		}
	}
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
