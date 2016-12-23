/*
*********************************************************************************************************
* @file    	SeqStack.c
* @author  	SY
* @version 	V1.0.1
* @date    	2016-9-12 13:30:32
* @IDE	 	V4.70.0.0
* @Chip    	STM32F407VE
* @brief   	顺序堆栈源文件
*********************************************************************************************************
* @attention
*	栈是限制在一段进行插入操作和删除操作的线性表（俗称堆栈），允许进行操作的一端称为“栈顶”，
*	另一固定端称为“栈底”，当栈中没有元素称为“空栈”。特点：先进后出（FILO）。
* ---------------------------------------------------------
* 版本：V1.0.1 	修改人：SY		修改日期：2016-9-12 13:30:37
* 
* 1、增加顺序堆栈的遍历。
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
#include "SeqStack.h"


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
* Function Name : CreateSeqStack
* Description	: 创建顺序堆栈(内存空间由外部提供、静态内存)
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void CreateSeqStack( SEQ_STACK_TypeDef *stackPtr, void *pBase, uint32_t lenth )
{
	stackPtr->basePtr = pBase;
	stackPtr->maxLenth = lenth;
	stackPtr->top = 0;
}

/*
*********************************************************************************************************
* Function Name : SeqStackIsFull
* Description	: 顺序堆栈是否为满
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM SeqStackIsFull( SEQ_STACK_TypeDef *stackPtr )
{
	if (stackPtr->top == stackPtr->maxLenth)
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
* Function Name : SeqStackIsEmpty
* Description	: 顺序堆栈是否为空
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM SeqStackIsEmpty( SEQ_STACK_TypeDef *stackPtr )
{
	if (stackPtr->top == 0)
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
* Function Name : GetSeqStackLenth
* Description	: 获取顺序堆栈长度
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t GetSeqStackLenth( SEQ_STACK_TypeDef *stackPtr )
{
	return stackPtr->maxLenth;
}

/*
*********************************************************************************************************
* Function Name : ClearSeqStack
* Description	: 清空堆栈
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void ClearSeqStack( SEQ_STACK_TypeDef *stackPtr )
{
	stackPtr->top = 0;
}

/*
*********************************************************************************************************
* Function Name : PushSeqStack
* Description	: 入栈
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PushSeqStack( SEQ_STACK_TypeDef *stackPtr, void *dataIn,\
		void (*push_CallBack)( void *base, uint32_t offset, void *data ) )
{
	if (SeqStackIsFull(stackPtr) == STATUS_DATA_STRUCT_TRUE)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	push_CallBack(stackPtr->basePtr, stackPtr->top, dataIn);
	stackPtr->top++;
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : PopSeqStack
* Description	: 出栈
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PopSeqStack( SEQ_STACK_TypeDef *stackPtr, void *dataOut,\
		void (*pop_CallBack)( void *base, uint32_t offset, void *data ) )
{
	if (SeqStackIsEmpty(stackPtr) == STATUS_DATA_STRUCT_TRUE)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	stackPtr->top--;
	pop_CallBack(stackPtr->basePtr, stackPtr->top, dataOut);

	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : GetSeqStackElement
* Description	: 获取顺序堆栈元素
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM GetSeqStackElement( SEQ_STACK_TypeDef *stackPtr, void *dataOut,\
		void (*pop_CallBack)( void *base, uint32_t offset, void *data ) )
{
	SEQ_STACK_TypeDef stack = *stackPtr;
	
	return PopSeqStack(&stack,dataOut,pop_CallBack);
}

/*
*********************************************************************************************************
* Function Name : TraverseSeqStack
* Description	: 遍历顺序堆栈
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void TraverseSeqStack( SEQ_STACK_TypeDef *stackPtr, void *dataOut,\
		void (*pop_CallBack)( void *base, uint32_t offset, void *data ),\
		void (*show_CallBack)( void *dataOut ) )
{
	SEQ_STACK_TypeDef stack = *stackPtr;
	
	while ( PopSeqStack(&stack, dataOut, pop_CallBack) == STATUS_DATA_STRUCT_TRUE)
	{
		if (show_CallBack)
		{
			show_CallBack(dataOut);
		}
	}
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
