/*
*********************************************************************************************************
* @file    	SeqListSearch.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-9-12 15:56:05
* @IDE	 	V4.70.0.0
* @Chip    	STM32F407VE
* @brief   	顺序线性表查找算法源文件
*********************************************************************************************************
* @attention
*		1、被查找对象一般由键值和数据两部分组成。键值必须具有唯一性，可以标识对应数据。
*		2、顺序查找：键值可以是数字、字符串等类型，通过逐一比较目标键值与表键值，如果相等，则查找成功。
*			否则，查找失败。
*		3、折半查找：键值可以是数字、字符串等类型，但必须有序，通过调整回调函数，支持顺序和逆序。
*			通过操作数组索引值，比较目标键值与数组索引对应的表键值，如果目标键值等于表键值，则查找成功。
*			否则，比较目标键值与表键值的大小关系，移动索引值，重新比较。
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
#include "SeqListSearch.h"


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
* Function Name : InitSeqListSearching
* Description	: 初始化顺序表查找
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void InitSeqListSearching( SEQ_LIST_SEARCH_TypeDef *listPtr, void *pBase, uint32_t lenth )
{
	listPtr->basePtr = pBase;
	listPtr->maxLenth = lenth;
}

/*
*********************************************************************************************************
* Function Name : InsertSeqListSearchingNode
* Description	: 插入顺序表查找节点
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool InsertSeqListSearchingNode( SEQ_LIST_SEARCH_TypeDef *listPtr,\
		uint32_t nodeIndex, void *key, void *value,\
		void (*setNode_CallBack)( void *base, uint32_t nodeIndex, void *key, void *value ) )
{
	if (nodeIndex >= listPtr->maxLenth)
	{
		return false;
	}
	
	if (setNode_CallBack)
	{
		setNode_CallBack(listPtr->basePtr, nodeIndex, key, value);
	}
	
	return true;
}

/*
*********************************************************************************************************
* Function Name : SeqListSequenceSearching
* Description	: 顺序线性表顺序查找
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void *SeqListSequenceSearching( SEQ_LIST_SEARCH_TypeDef *listPtr, void *findKey,\
		void *(*matchKey_CallBack)( void *base, uint32_t nodeIndex, void *findKey ) )
{
	uint32_t nodeIndex = 0;

	while (nodeIndex < listPtr->maxLenth)
	{
		if (matchKey_CallBack)
		{
			void *value = matchKey_CallBack(listPtr->basePtr, nodeIndex, findKey);
			if (value != NULL)
			{
				return value;
			}
		}
		nodeIndex++;
	}
	
	return NULL;
}

/*
*********************************************************************************************************
* Function Name : SeqListBinSearching
* Description	: 顺序线性表折半查找
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool SeqListBinSearching( SEQ_LIST_SEARCH_TypeDef *listPtr, void *findKey, uint32_t *keyIndex,\
		int8_t (*equalKey_CallBack)( void *base, uint32_t nodeIndex, void *findKey ) )
{
	if (equalKey_CallBack == NULL)
	{
		return NULL;
	}
	uint32_t low = 1;
	uint32_t high = listPtr->maxLenth;

	while (low <= high)
	{
		uint32_t middle = (low + high) / 2;	
		int8_t result = equalKey_CallBack(listPtr->basePtr, middle-1, findKey);
		if (result > 0)
		{
			low = middle + 1;
		}
		else if (result < 0)
		{
			high = middle - 1;
		}
		else
		{
			*keyIndex = middle - 1;
			
			return true;
		}
	}
	
	return false;
}



/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
