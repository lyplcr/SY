/*
*********************************************************************************************************
* @file    	SeqQueue.h
* @author  	SY
* @version 	V1.0.0
* @date    	2016-4-15 14:04:44
* @IDE	 	V4.70.0.0
* @Chip    	STM32F407VE
* @brief   	顺序队列头文件
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
#ifndef __SEQQUEUE_H
#define __SEQQUEUE_H

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
	uint32_t front;
	uint32_t rear;
}SEQUEUE_TypeDef;

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
* Function Name : PushSequeueU8_CallBack
* Description	: 顺序队列8Bit整型数据入队
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE void PushSeqQueueU8_CallBack( void *base, uint32_t offset, void *dataIn )
{
	uint8_t *dataPtr = dataIn;
	uint8_t *basePtr = base;
	
	basePtr[offset] = *dataPtr;
}

/*
*********************************************************************************************************
* Function Name : PopSequeueU8_CallBack
* Description	: 顺序队列8Bit整型数据出队
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE void PopSeqQueueU8_CallBack( void *base, uint32_t offset, void *dataOut )
{
	uint8_t *dataPtr = dataOut;
	uint8_t *basePtr = base;
	
	*dataPtr = basePtr[offset];
}

/*
*********************************************************************************************************
* Function Name : PushSeqQueueU32_CallBack
* Description	: 顺序队列32Bit整型数据入队
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE void PushSeqQueueU32_CallBack( void *base, uint32_t offset, void *dataIn )
{
	uint32_t *dataPtr = dataIn;
	uint32_t *basePtr = base;
	
	basePtr[offset] = *dataPtr;
}

/*
*********************************************************************************************************
* Function Name : PopSeqQueueU32_CallBack
* Description	: 顺序队列32Bit整型数据出队
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE void PopSeqQueueU32_CallBack( void *base, uint32_t offset, void *dataOut )
{
	uint32_t *dataPtr = dataOut;
	uint32_t *basePtr = base;
	
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
void CreateSeqQueue( SEQUEUE_TypeDef *queuePtr, void *pBase, uint32_t lenth );
DATA_STRUCT_STATUS_ENUM SeqQueueIsFull( SEQUEUE_TypeDef *queuePtr );
DATA_STRUCT_STATUS_ENUM SeqQueueIsEmpty( SEQUEUE_TypeDef *queuePtr );
void ClearSeqQueue( SEQUEUE_TypeDef *queuePtr );
uint32_t GetSeqQueueFront( SEQUEUE_TypeDef *queuePtr );
uint32_t GetSeqQueueRear( SEQUEUE_TypeDef *queuePtr );
uint32_t GetSeqQueueLenth( SEQUEUE_TypeDef *queuePtr );
DATA_STRUCT_STATUS_ENUM PushSeqQueue( SEQUEUE_TypeDef *queuePtr, void *dataIn,\
		void (*push_CallBack)( void *base, uint32_t offset, void *data ) );
DATA_STRUCT_STATUS_ENUM PopSeqQueue( SEQUEUE_TypeDef *queuePtr, void *dataOut,\
		void (*pop_CallBack)( void *base, uint32_t offset, void *data ) );
DATA_STRUCT_STATUS_ENUM GetSeqQueueElement( SEQUEUE_TypeDef *queuePtr, void *dataOut,\
		void (*pop_CallBack)( void *base, uint32_t offset, void *data ) );
uint8_t GetSeqQueueDisposeDataRate( SEQUEUE_TypeDef *queuePtr );
void TraverseSeqQueue( SEQUEUE_TypeDef *queuePtr, void *dataOut,\
		void (*pop_CallBack)( void *base, uint32_t offset, void *data ),\
		void (*show_CallBack)( void *dataOut ) );

#endif
/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
