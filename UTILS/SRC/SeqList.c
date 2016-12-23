/*
*********************************************************************************************************
* @file    	SeqList.c
* @author  	SY
* @version 	V1.0.1
* @date    	2016-9-12 15:26:11
* @IDE	 	V4.70.0.0
* @Chip    	STM32F407VE
* @brief   	顺序线性表源文件
*********************************************************************************************************
* @attention
*		线性表是最基本、最简单、也是最常用的一种数据结构。线性表中数据元素之间的关系是一对一的关系，
*	即除了第一个和最后一个数据元素之外，其它数据元素都是首尾相接的。线性表的逻辑结构简单，便于实现和操作。
*	因此，线性表这种数据结构在实际应用中是广泛采用的一种数据结构。
*   	线性表是一个线性结构，它是一个含有n≥0个结点的有限序列，对于其中的结点，有且仅有一个开始结点
*	没有前驱但有一个后继结点，有且仅有一个终端结点没有后继但有一个前驱结点，其它的结点都有且仅有一个前驱
*	和一个后继结点。
*
* ---------------------------------------------------------
* 版本：V1.0.1 	修改人：SY		修改日期：2016-9-12 15:26:19
* 
* 1、增加顺序线性表的遍历。
* -------------------------------------------------------------------------------------------------------	
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
#include "SeqList.h"


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
* Function Name : CreateSeqList
* Description	: 创建顺序线性表(内存空间由外部提供、静态内存)
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void CreateSeqList( SEQ_LIST_TypeDef *listPtr, void *pBase, uint32_t lenth )
{
	listPtr->basePtr = pBase;
	listPtr->maxLenth = lenth;
	listPtr->index = 0;
}

/*
*********************************************************************************************************
* Function Name : SeqListIsFull
* Description	: 顺序线性表是否为满
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM SeqListIsFull( SEQ_LIST_TypeDef *listPtr )
{
	if (listPtr->index == listPtr->maxLenth)
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
* Function Name : SeqListIsEmpty
* Description	: 顺序线性表是否为空
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM SeqListIsEmpty( SEQ_LIST_TypeDef *listPtr )
{
	if (listPtr->index == 0)
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
* Function Name : GetSeqListLenth
* Description	: 获取顺序线性表长度
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t GetSeqListLenth( SEQ_LIST_TypeDef *listPtr )
{
	return listPtr->index;
}

/*
*********************************************************************************************************
* Function Name : ClearSeqList
* Description	: 清空线性表
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void ClearSeqList( SEQ_LIST_TypeDef *listPtr )
{
	listPtr->index = 0;
}

/*
*********************************************************************************************************
* Function Name : PushSeqList
* Description	: 压入线性表
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PushSeqList( SEQ_LIST_TypeDef *listPtr, void *dataIn, uint32_t pos,\
		void (*push_CallBack)( void *base, uint32_t pos, void *dataIn ) )
{
	if (pos >= listPtr->index)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	push_CallBack(listPtr->basePtr, pos, dataIn);
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : PopSeqList
* Description	: 弹出线性表
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PopSeqList( SEQ_LIST_TypeDef *listPtr, void *dataOut, uint32_t pos,\
		void (*pop_CallBack)( void *base, uint32_t pos, void *dataOut ) )
{
	if (pos >= listPtr->index)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}

	pop_CallBack(listPtr->basePtr, pos, dataOut);

	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : InsertSeqList
* Description	: 插入线性表（在pos的前方插入，pos从0开始）
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM InsertSeqList( SEQ_LIST_TypeDef *listPtr, void *dataIn, uint32_t pos,\
		void (*push_CallBack)( void *base, uint32_t pos, void *dataIn ),\
		void (*copy_CallBack)( void *base, uint32_t targetPos, uint32_t sourcePos ) )
{
	if (SeqListIsFull(listPtr) == STATUS_DATA_STRUCT_TRUE)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	if (pos >= listPtr->index)
	{
		pos = listPtr->index;
	}
	
	for (uint32_t i=listPtr->index; i>pos; --i)
	{
		copy_CallBack(listPtr->basePtr, i, i-1);
	}
	push_CallBack(listPtr->basePtr, pos, dataIn);
	listPtr->index++;
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : DeleteSeqList
* Description	: 删除线性表
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM DeleteSeqList( SEQ_LIST_TypeDef *listPtr, uint32_t pos,\
		void (*copy_CallBack)( void *base, uint32_t targetPos, uint32_t sourcePos ) )
{
	if (pos >= listPtr->index)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}

	listPtr->index--;
	for (uint32_t i=pos; i<listPtr->index; ++i)
	{
		copy_CallBack(listPtr->basePtr, i, i+1);
	}

	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : TraverseSeqList
* Description	: 遍历顺序线性表
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void TraverseSeqList( SEQ_LIST_TypeDef *listPtr, void *dataOut,\
		void (*pop_CallBack)( void *base, uint32_t offset, void *data ),\
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
