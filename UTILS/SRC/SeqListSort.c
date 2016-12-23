/*
*********************************************************************************************************
* @file    	SeqListSort.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-9-18 11:09:31
* @IDE	 	V4.70.0.0
* @Chip    	STM32F407VE
* @brief   	顺序线性表排序源文件
*********************************************************************************************************
* @attention
*	插入排序：在线性表基础上实现插入排序算法，前提是线性表必须有序。
*		1、直接插入排序：
*			在顺序线性表中，将待插入元素的键值，与表中的键值一一进行比较，找到待插入位置后，插入元素。
*		2、折半查找排序：
*			在顺序线性表中，通过二分法快速找到待插入元素在线性表中的位置，然后插入元素。
*			二分法通过操作线性表的索引值，比对线性表中元素的键值，得到待插入元素的位置。
*	交换排序：在线性表基础上实现交换排序算法，线性表可以是乱序。
*		1、冒泡法排序：
*			在顺序线性表中，每次排序都将最值(大/小)放到最后，重复进行，直至排序完毕。
*		2、快速排序：使用“分治法”思想，
*			（1）选取一个基准值，进行一次排序，将整个线性表分为两部分，左半部分比基准值小，右半部分
*				比基准值大。
*			（2）使用递归调用方式，对左、右两部分线性表，重复执行步骤(1)，直至整个线性表有序。
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
#include "SeqListSort.h"


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
* Function Name : InsertSeqListSequenceSorting
* Description	: 直接插入顺序线性表
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM InsertSeqListSequenceSorting( SEQ_LIST_TypeDef *listPtr, void *dataIn,\
		bool (*comp_CallBack)( void *base, uint32_t index, void *dataIn ),\
		void (*push_CallBack)( void *base, uint32_t index, void *dataIn ),\
		void (*copy_CallBack)( void *base, uint32_t targetIndex, uint32_t sourceIndex ) )
{
	if ((comp_CallBack == NULL) || (push_CallBack == NULL) || (copy_CallBack == NULL))
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	if (SeqListIsFull(listPtr) == STATUS_DATA_STRUCT_TRUE)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	/* 定位待插入位置 */
	uint32_t insertIndex = 0;
	for (insertIndex=0; insertIndex<listPtr->index; ++insertIndex)
	{
		if (comp_CallBack(listPtr->basePtr, insertIndex, dataIn) == true)
		{
			break;
		}
	}
	
	/* 将顺序表中元素依次后移 */
	for (uint32_t i=listPtr->index; i>insertIndex; --i)
	{
		copy_CallBack(listPtr->basePtr, i, i-1);
	}
	
	/* 插入新元素 */
	push_CallBack(listPtr->basePtr, insertIndex, dataIn);
	listPtr->index++;
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : InsertSeqListBinSorting
* Description	: 折半查找方式插入顺序线性表
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM InsertSeqListBinSorting( SEQ_LIST_TypeDef *listPtr, void *dataIn,\
		int8_t (*equalKey_CallBack)( void *base, uint32_t nodeIndex, void *dataIn ),\
		void (*push_CallBack)( void *base, uint32_t index, void *dataIn ),\
		void (*copy_CallBack)( void *base, uint32_t targetIndex, uint32_t sourceIndex ) )
{
	if ((equalKey_CallBack == NULL) || (push_CallBack == NULL) || (copy_CallBack == NULL))
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	if (SeqListIsFull(listPtr) == STATUS_DATA_STRUCT_TRUE)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	/* 定位待插入位置 */	
	uint32_t low = 1;
	uint32_t high = listPtr->index;

	while (low <= high)
	{
		uint32_t middle = (low + high) / 2;	
		int8_t result = equalKey_CallBack(listPtr->basePtr, middle-1, dataIn);
		if (result >= 0)
		{
			low = middle + 1;
		}
		else
		{
			high = middle - 1;
		}
	}
	uint32_t insertIndex = low - 1;
	
	/* 将顺序表中元素依次后移 */
	for (uint32_t i=listPtr->index; i>insertIndex; --i)
	{
		copy_CallBack(listPtr->basePtr, i, i-1);
	}
	
	/* 插入新元素 */
	push_CallBack(listPtr->basePtr, insertIndex, dataIn);
	listPtr->index++;
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : SeqListBubSorting
* Description	: 顺序线性表冒泡排序
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM SeqListBubSorting( SEQ_LIST_TypeDef *listPtr,\
		int8_t (*equalKey_CallBack)( void *base, uint32_t targetIndex, uint32_t sourceIndex ),\
		void (*swap_CallBack)( void *base, uint32_t targetIndex, uint32_t sourceIndex ) )
{
	if ((equalKey_CallBack == NULL) || (swap_CallBack == NULL))
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	if (SeqListIsEmpty(listPtr) == STATUS_DATA_STRUCT_TRUE)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	/* 最多执行 (n-1) 次排序 */
	for (uint32_t i=0; i<listPtr->index-1; ++i)
	{
		for (uint32_t j=0; j<listPtr->index-i-1; ++j)
		{
			int8_t result = equalKey_CallBack(listPtr->basePtr, j+1, j);
			if (result > 0)
			{
				swap_CallBack(listPtr->basePtr, j+1, j);
			}
		}
	}
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : SeqListQuickSortingPartition
* Description	: 顺序线性表快速排序分为两部分
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void SeqListQuickSortingPartition( SEQ_LIST_TypeDef *listPtr, uint32_t low, uint32_t high, uint32_t *middle,\
		int8_t (*equalKey_CallBack)( void *base, uint32_t nodeIndex, void *dataIn ),\
		void (*copy_CallBack)( void *base, uint32_t targetIndex, uint32_t sourceIndex ),\
		void (*push_CallBack)( void *base, uint32_t index, void *data ),\
		void (*pop_CallBack)( void *base, uint32_t index, void *data ))
{
	SEQ_LIST_SORT_NODE_TypeDef referenceNode;
	PopSeqList(listPtr, &referenceNode, low, pop_CallBack);
	
	/**
	 *	原理：从右->左寻找比基准值小的值 Value1，从左->右寻找比基准值大的值 Value2。
	 *	 交换 Value1 与 Value2，直至基准值左边的值全部小于基准值右边的值。
	 */
	while (low < high)
	{
		while ((low < high) && (equalKey_CallBack(listPtr->basePtr, high, &referenceNode) >= 0))
		{
			high--;
		}
		if (low < high)
		{
			copy_CallBack(listPtr->basePtr, low, high);
		}
		
		while ((low < high) && (equalKey_CallBack(listPtr->basePtr, low, &referenceNode) <= 0))
		{
			low++;
		}
		if (low < high)
		{
			copy_CallBack(listPtr->basePtr, high, low);
		}
	}
	
	PushSeqList(listPtr, &referenceNode, low, push_CallBack);	
	*middle = low;
}

/*
*********************************************************************************************************
* Function Name : __SeqListQuickSorting
* Description	: 顺序线性表快速排序核心（low/high从0开始）
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void __SeqListQuickSorting( SEQ_LIST_TypeDef *listPtr, uint32_t low, uint32_t high,\
		int8_t (*equalKey_CallBack)( void *base, uint32_t nodeIndex, void *dataIn ),\
		void (*copy_CallBack)( void *base, uint32_t targetIndex, uint32_t sourceIndex ),\
		void (*push_CallBack)( void *base, uint32_t index, void *data ),\
		void (*pop_CallBack)( void *base, uint32_t index, void *data ))
{
	uint32_t middle = 0;
	
	if (low < high)
	{
		SeqListQuickSortingPartition(listPtr, low, high, &middle,\
								equalKey_CallBack,\
								copy_CallBack,\
								push_CallBack,\
								pop_CallBack);
		if (middle > 0)
		{
			SeqListQuickSorting(listPtr, low, middle-1,\
								equalKey_CallBack,\
								copy_CallBack,\
								push_CallBack,\
								pop_CallBack);
		}
		
		SeqListQuickSorting(listPtr, middle+1, high,\
								equalKey_CallBack,\
								copy_CallBack,\
								push_CallBack,\
								pop_CallBack);
	}
}

/*
*********************************************************************************************************
* Function Name : SeqListCoreQuickSorting
* Description	: 顺序线性表快速排序核心（low/high从0开始）
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM SeqListQuickSorting( SEQ_LIST_TypeDef *listPtr, uint32_t low, uint32_t high,\
		int8_t (*equalKey_CallBack)( void *base, uint32_t nodeIndex, void *dataIn ),\
		void (*copy_CallBack)( void *base, uint32_t targetIndex, uint32_t sourceIndex ),\
		void (*push_CallBack)( void *base, uint32_t index, void *data ),\
		void (*pop_CallBack)( void *base, uint32_t index, void *data ))
{
	if ((equalKey_CallBack == NULL) || (copy_CallBack == NULL) ||\
		(push_CallBack == NULL) || (pop_CallBack == NULL))
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	if (SeqListIsEmpty(listPtr) == STATUS_DATA_STRUCT_TRUE)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	__SeqListQuickSorting(listPtr, low, high,\
								equalKey_CallBack,\
								copy_CallBack,\
								push_CallBack,\
								pop_CallBack);
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : TraverseSeqListSorting
* Description	: 遍历顺序线性表
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void TraverseSeqListSorting( SEQ_LIST_TypeDef *listPtr, void *dataOut,\
		void (*pop_CallBack)( void *base, uint32_t index, void *data ),\
		void (*show_CallBack)( void *dataOut ) )
{
	SEQ_LIST_TypeDef list = *listPtr;
	uint32_t posIndex = 0;
	
	while (PopSeqList(&list, dataOut, posIndex++, pop_CallBack) == STATUS_DATA_STRUCT_TRUE)
	{
		if (show_CallBack)
		{
			show_CallBack(dataOut);
		}
	}
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
