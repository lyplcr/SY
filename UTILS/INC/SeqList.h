/*
*********************************************************************************************************
* @file    	SeqList.h
* @author  	SY
* @version 	V1.0.0
* @date    	2016-9-8 13:10:28
* @IDE	 	V4.70.0.0
* @Chip    	STM32F407VE
* @brief   	顺序线性表头文件
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
#ifndef __SEQ_LIST_H
#define __SEQ_LIST_H

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
	uint32_t index;
}SEQ_LIST_TypeDef;

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
* Function Name : CopySeqListU8_CallBack
* Description	: 拷贝顺序线性表8Bit整型数据
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE void CopySeqListU8_CallBack( void *base, uint32_t targetPos, uint32_t sourcePos )
{
	uint8_t *basePtr = base;
	
	basePtr[targetPos] = basePtr[sourcePos];
}

/*
*********************************************************************************************************
* Function Name : PushSeqListU8_CallBack
* Description	: 压入顺序队列8Bit整型数据
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE void PushSeqListU8_CallBack( void *base, uint32_t pos, void *dataIn )
{
	uint8_t *dataPtr = dataIn;
	uint8_t *basePtr = base;
	
	basePtr[pos] = *dataPtr;
}

/*
*********************************************************************************************************
* Function Name : PopSeqListU8_CallBack
* Description	: 弹出顺序队列8Bit整型数据
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE void PopSeqListU8_CallBack( void *base, uint32_t pos, void *dataOut )
{
	uint8_t *dataPtr = dataOut;
	uint8_t *basePtr = base;
	
	*dataPtr = basePtr[pos];
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
void CreateSeqList( SEQ_LIST_TypeDef *listPtr, void *pBase, uint32_t lenth );
DATA_STRUCT_STATUS_ENUM SeqListIsFull( SEQ_LIST_TypeDef *listPtr );
DATA_STRUCT_STATUS_ENUM SeqListIsEmpty( SEQ_LIST_TypeDef *listPtr );
uint32_t GetSeqListLenth( SEQ_LIST_TypeDef *listPtr );
void ClearSeqList( SEQ_LIST_TypeDef *listPtr );
DATA_STRUCT_STATUS_ENUM PushSeqList( SEQ_LIST_TypeDef *listPtr, void *dataIn, uint32_t pos,\
		void (*push_CallBack)( void *base, uint32_t pos, void *dataIn ) );
DATA_STRUCT_STATUS_ENUM PopSeqList( SEQ_LIST_TypeDef *listPtr, void *dataOut, uint32_t pos,\
		void (*pop_CallBack)( void *base, uint32_t pos, void *dataOut ) );
DATA_STRUCT_STATUS_ENUM InsertSeqList( SEQ_LIST_TypeDef *listPtr, void *dataIn, uint32_t pos,\
		void (*push_CallBack)( void *base, uint32_t pos, void *dataIn ),\
		void (*copy_CallBack)( void *base, uint32_t targetPos, uint32_t sourcePos ) );
DATA_STRUCT_STATUS_ENUM DeleteSeqList( SEQ_LIST_TypeDef *listPtr, uint32_t pos,\
		void (*copy_CallBack)( void *base, uint32_t targetPos, uint32_t sourcePos ) );
void TraverseSeqList( SEQ_LIST_TypeDef *listPtr, void *dataOut,\
		void (*pop_CallBack)( void *base, uint32_t offset, void *data ),\
		void (*show_CallBack)( void *dataOut ) );		

#endif
/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
