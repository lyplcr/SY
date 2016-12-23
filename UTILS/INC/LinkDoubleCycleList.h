/*
*********************************************************************************************************
* @file    	LinkDoubleCycleList.h
* @author  	SY
* @version 	V1.0.0
* @date    	2016-9-9 15:09:24
* @IDE	 	V4.70.0.0
* @Chip    	STM32F407VE
* @brief   	双向循环链表头文件
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
#ifndef __LINK_DOUBLE_CYCLE_LIST_H
#define __LINK_DOUBLE_CYCLE_LIST_H

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
typedef struct tagLINK_DOUBLE_CYCLE_LIST_TypeDef
{
	void *data;
	struct tagLINK_DOUBLE_CYCLE_LIST_TypeDef *prev;
	struct tagLINK_DOUBLE_CYCLE_LIST_TypeDef *next;
}LINK_DOUBLE_CYCLE_LIST_TypeDef;

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
*                              				Exported variables
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Exported functions
*********************************************************************************************************
*/
LINK_DOUBLE_CYCLE_LIST_TypeDef *CreateLinkDoubleCycleList( void );
DATA_STRUCT_STATUS_ENUM LinkDoubleCycleListIsEmpty( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead );
uint32_t GetLinkDoubleCycleListLenth( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead );
void ClearLinkDoubleCycleList( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead );
void DestoryLinkDoubleCycleList( LINK_DOUBLE_CYCLE_LIST_TypeDef **pHead );
DATA_STRUCT_STATUS_ENUM InsertLinkDoubleCycleList( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead, void *dataIn, uint32_t dataSize,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) );
DATA_STRUCT_STATUS_ENUM DeleteLinkDoubleCycleList( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) );
DATA_STRUCT_STATUS_ENUM PushLinkDoubleCycleList( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead, void *dataIn, uint32_t dataSize,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) );
DATA_STRUCT_STATUS_ENUM PopLinkDoubleCycleList( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead, void *dataOut, uint32_t dataSize,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) );
DATA_STRUCT_STATUS_ENUM TraverseNextLinkDoubleCycleList( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead,\
		void (*show_CallBack)( void *nodeData ) );
DATA_STRUCT_STATUS_ENUM TraversePrevLinkDoubleCycleList( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead,\
		void (*show_CallBack)( void *nodeData ) );
DATA_STRUCT_STATUS_ENUM SpliceLinkDoubleCycleList( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead,\
		LINK_DOUBLE_CYCLE_LIST_TypeDef *pHeadNew );

#endif
/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
