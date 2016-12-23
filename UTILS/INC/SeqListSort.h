/*
*********************************************************************************************************
* @file    	SeqListSort.h
* @author  	SY
* @version 	V1.0.0
* @date    	2016-9-18 11:09:31
* @IDE	 	V4.70.0.0
* @Chip    	STM32F407VE
* @brief   	有序线性表排序头文件
*********************************************************************************************************
* @attention
*		
*
* 
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Define to prevent recursive inclusion
*********************************************************************************************************
*/
#ifndef __SEQ_LIST_SORT_H
#define __SEQ_LIST_SORT_H

/*
*********************************************************************************************************
*                              				Exported Includes
*********************************************************************************************************
*/
#include "SeqList.h"


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
}SEQ_LIST_SORT_NODE_TypeDef;

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
* Function Name : SeqListSortCompAscendingU32_CallBack
* Description	: 顺序线性表按照升序进行比较
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE bool SeqListSortCompAscendingU32_CallBack( void *base, uint32_t index, void *dataIn )
{
	SEQ_LIST_SORT_NODE_TypeDef *listNodePtr = (SEQ_LIST_SORT_NODE_TypeDef *)base + index;
	uint32_t listKey = *(uint32_t *)listNodePtr->key;
	SEQ_LIST_SORT_NODE_TypeDef *targetNodePtr = (SEQ_LIST_SORT_NODE_TypeDef *)dataIn;
	uint32_t targetKey = *(uint32_t *)targetNodePtr->key;
	
	if (targetKey < listKey)
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
* Function Name : SeqListSortCompDescendingU32_CallBack
* Description	: 顺序线性表按照降序进行比较
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE bool SeqListSortCompDescendingU32_CallBack( void *base, uint32_t index, void *dataIn )
{
	SEQ_LIST_SORT_NODE_TypeDef *listNodePtr = (SEQ_LIST_SORT_NODE_TypeDef *)base + index;
	uint32_t listKey = *(uint32_t *)listNodePtr->key;
	SEQ_LIST_SORT_NODE_TypeDef *targetNodePtr = (SEQ_LIST_SORT_NODE_TypeDef *)dataIn;
	uint32_t targetKey = *(uint32_t *)targetNodePtr->key;
	
	if (targetKey > listKey)
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
* Function Name : PushSeqListSortU8_CallBack
* Description	: 压入顺序线性表数据
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE void PushSeqListSorting_CallBack( void *base, uint32_t index, void *dataIn )
{
	SEQ_LIST_SORT_NODE_TypeDef *listNodePtr = (SEQ_LIST_SORT_NODE_TypeDef *)base + index;
	
	*listNodePtr = *(SEQ_LIST_SORT_NODE_TypeDef *)dataIn;
}

/*
*********************************************************************************************************
* Function Name : PopSeqListSorting_CallBack
* Description	: 弹出顺序线性表数据
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE void PopSeqListSorting_CallBack( void *base, uint32_t index, void *dataOut )
{
	SEQ_LIST_SORT_NODE_TypeDef *listNodePtr = (SEQ_LIST_SORT_NODE_TypeDef *)base + index;
	
	*(SEQ_LIST_SORT_NODE_TypeDef *)dataOut = *listNodePtr;
}

/*
*********************************************************************************************************
* Function Name : CopySeqListSorting_CallBack
* Description	: 拷贝顺序线性表数据
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE void CopySeqListSorting_CallBack( void *base, uint32_t targetIndex, uint32_t sourceIndex )
{
	SEQ_LIST_SORT_NODE_TypeDef *targetNodePtr = (SEQ_LIST_SORT_NODE_TypeDef *)base + targetIndex;
	SEQ_LIST_SORT_NODE_TypeDef *sourceNodePtr = (SEQ_LIST_SORT_NODE_TypeDef *)base + sourceIndex;
	
	*targetNodePtr = *sourceNodePtr;
}

/*
*********************************************************************************************************
* Function Name : SwapSeqListSorting_CallBack
* Description	: 交换顺序线性表数据
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE void SwapSeqListSorting_CallBack( void *base, uint32_t targetIndex, uint32_t sourceIndex )
{
	SEQ_LIST_SORT_NODE_TypeDef *targetNodePtr = (SEQ_LIST_SORT_NODE_TypeDef *)base + targetIndex;
	SEQ_LIST_SORT_NODE_TypeDef *sourceNodePtr = (SEQ_LIST_SORT_NODE_TypeDef *)base + sourceIndex;
	SEQ_LIST_SORT_NODE_TypeDef tempNode;
	
	tempNode = *targetNodePtr;
	*targetNodePtr = *sourceNodePtr;
	*sourceNodePtr = tempNode;
}

/*
*********************************************************************************************************
* Function Name : SeqListSortingIsEqualKeyU32_CallBack
* Description	: 顺序线性表键值是否相等
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE int8_t SeqListSortingIsEqualKeyU32_CallBack( void *base, uint32_t nodeIndex,\
		void *dataIn )
{
	SEQ_LIST_SORT_NODE_TypeDef *nodePtr = ((SEQ_LIST_SORT_NODE_TypeDef *)base + nodeIndex);
	uint32_t nodeKey = *(uint32_t *)nodePtr->key;
	uint32_t findKey = *(uint32_t *)((SEQ_LIST_SORT_NODE_TypeDef *)dataIn)->key;
	
	if (findKey > nodeKey)
	{
		return 1;
	}
	else if (findKey < nodeKey)
	{
		return -1;
	}
	else
	{	
		return 0;
	}
}

/*
*********************************************************************************************************
* Function Name : SeqListSortingReverseIsEqualKeyU32_CallBack
* Description	: 顺序线性表键值是否相等（取反）
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE int8_t SeqListSortingReverseIsEqualKeyU32_CallBack( void *base, uint32_t nodeIndex,\
		void *dataIn )
{
	SEQ_LIST_SORT_NODE_TypeDef *nodePtr = ((SEQ_LIST_SORT_NODE_TypeDef *)base + nodeIndex);
	uint32_t nodeKey = *(uint32_t *)nodePtr->key;
	uint32_t findKey = *(uint32_t *)((SEQ_LIST_SORT_NODE_TypeDef *)dataIn)->key;
	
	if (findKey > nodeKey)
	{
		return -1;
	}
	else if (findKey < nodeKey)
	{
		return 1;
	}
	else
	{	
		return 0;
	}
}

/*
*********************************************************************************************************
* Function Name : SeqListSortingIndexIsEqualKeyU32_CallBack
* Description	: 顺序线性表键值是否相等
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE int8_t SeqListSortingIndexIsEqualKeyU32_CallBack( void *base, uint32_t targetIndex,\
		uint32_t sourceIndex )
{
	SEQ_LIST_SORT_NODE_TypeDef *targetNodePtr = ((SEQ_LIST_SORT_NODE_TypeDef *)base + targetIndex);
	uint32_t targetNodeKey = *(uint32_t *)targetNodePtr->key;
	SEQ_LIST_SORT_NODE_TypeDef *sourceNodePtr = ((SEQ_LIST_SORT_NODE_TypeDef *)base + sourceIndex);
	uint32_t sourceNodeKey = *(uint32_t *)sourceNodePtr->key;
	
	if (sourceNodeKey > targetNodeKey)
	{
		return 1;
	}
	else if (sourceNodeKey < targetNodeKey)
	{
		return -1;
	}
	else
	{	
		return 0;
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
DATA_STRUCT_STATUS_ENUM InsertSeqListSequenceSorting( SEQ_LIST_TypeDef *listPtr, void *dataIn,\
		bool (*comp_CallBack)( void *base, uint32_t index, void *dataIn ),\
		void (*push_CallBack)( void *base, uint32_t pos, void *dataIn ),\
		void (*copy_CallBack)( void *base, uint32_t targetNode, uint32_t sourceNode ) );
DATA_STRUCT_STATUS_ENUM InsertSeqListBinSorting( SEQ_LIST_TypeDef *listPtr, void *dataIn,\
		int8_t (*equalKey_CallBack)( void *base, uint32_t nodeIndex, void *dataIn ),\
		void (*push_CallBack)( void *base, uint32_t index, void *dataIn ),\
		void (*copy_CallBack)( void *base, uint32_t targetIndex, uint32_t sourceIndex ) );
DATA_STRUCT_STATUS_ENUM SeqListBubSorting( SEQ_LIST_TypeDef *listPtr,\
		int8_t (*equalKey_CallBack)( void *base, uint32_t targetIndex, uint32_t sourceIndex ),\
		void (*swap_CallBack)( void *base, uint32_t targetIndex, uint32_t sourceIndex ) );
DATA_STRUCT_STATUS_ENUM SeqListQuickSorting( SEQ_LIST_TypeDef *listPtr, uint32_t low, uint32_t high,\
		int8_t (*equalKey_CallBack)( void *base, uint32_t nodeIndex, void *dataIn ),\
		void (*copy_CallBack)( void *base, uint32_t targetIndex, uint32_t sourceIndex ),\
		void (*push_CallBack)( void *base, uint32_t index, void *data ),\
		void (*pop_CallBack)( void *base, uint32_t index, void *data ));
void TraverseSeqListSorting( SEQ_LIST_TypeDef *listPtr, void *dataOut,\
		void (*pop_CallBack)( void *base, uint32_t index, void *data ),\
		void (*show_CallBack)( void *dataOut ) );

#endif
/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
