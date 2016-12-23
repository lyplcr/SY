/*
*********************************************************************************************************
* @file    	SeqStack.h
* @author  	SY
* @version 	V1.0.0
* @date    	2016-9-2 09:39:06
* @IDE	 	V4.70.0.0
* @Chip    	STM32F407VE
* @brief   	顺序堆栈头文件
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
#ifndef __SEQSTACK_H
#define __SEQSTACK_H

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
	void *basePtr;
	uint32_t maxLenth;
	uint32_t top;
}SEQ_STACK_TypeDef;

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
* Function Name : PushSeqStackU8_CallBack
* Description	: 8Bit整型数据入栈
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE void PushSeqStackU8_CallBack( void *base, uint32_t offset, void *dataIn )
{
	uint8_t *dataPtr = dataIn;
	uint8_t *basePtr = base;
	
	basePtr[offset] = *dataPtr;
}

/*
*********************************************************************************************************
* Function Name : PopSeqStackU8_CallBack
* Description	: 8Bit整型数据出栈
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE void PopSeqStackU8_CallBack( void *base, uint32_t offset, void *dataOut )
{
	uint8_t *dataPtr = dataOut;
	uint8_t *basePtr = base;
	
	*dataPtr = basePtr[offset];
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
void CreateSeqStack( SEQ_STACK_TypeDef *stackPtr, void *pBase, uint32_t lenth );
DATA_STRUCT_STATUS_ENUM SeqStackIsFull( SEQ_STACK_TypeDef *stackPtr );
DATA_STRUCT_STATUS_ENUM SeqStackIsEmpty( SEQ_STACK_TypeDef *stackPtr );
uint32_t GetSeqStackLenth( SEQ_STACK_TypeDef *stackPtr );
void ClearSeqStack( SEQ_STACK_TypeDef *stackPtr );
DATA_STRUCT_STATUS_ENUM PushSeqStack( SEQ_STACK_TypeDef *stackPtr, void *dataIn,\
		void (*push_CallBack)( void *base, uint32_t offset, void *data ) );
DATA_STRUCT_STATUS_ENUM PopSeqStack( SEQ_STACK_TypeDef *stackPtr, void *dataOut,\
		void (*pop_CallBack)( void *base, uint32_t offset, void *data ) );
DATA_STRUCT_STATUS_ENUM GetSeqStackElement( SEQ_STACK_TypeDef *stackPtr, void *dataOut,\
		void (*pop_CallBack)( void *base, uint32_t offset, void *data ) );
void TraverseSeqStack( SEQ_STACK_TypeDef *stackPtr, void *dataOut,\
		void (*pop_CallBack)( void *base, uint32_t offset, void *data ),\
		void (*show_CallBack)( void *dataOut ) );

#endif
/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
