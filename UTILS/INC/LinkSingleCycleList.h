/*
*********************************************************************************************************
* @file    	LinkSingleCycleList.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-9-9 13:49:34
* @IDE	 	V4.70.0.0
* @Chip    	STM32F407VE
* @brief   	单向循环链表源文件
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
#ifndef __LINK_SINGLE_CYCLE_LIST_H
#define __LINK_SINGLE_CYCLE_LIST_H

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
typedef struct tagLINK_SINGLE_CYCLE_LIST_TypeDef
{
	void *data;
	struct tagLINK_SINGLE_CYCLE_LIST_TypeDef *next;
}LINK_SINGLE_CYCLE_LIST_TypeDef;

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
LINK_SINGLE_CYCLE_LIST_TypeDef *CreateLinkSingleCycleList( void );
DATA_STRUCT_STATUS_ENUM LinkSingleCycleListIsEmpty( LINK_SINGLE_CYCLE_LIST_TypeDef *pHead );
uint32_t GetLinkSingleCycleListLenth( LINK_SINGLE_CYCLE_LIST_TypeDef *pHead );
void ClearLinkSingleCycleList( LINK_SINGLE_CYCLE_LIST_TypeDef *pHead );
void DestoryLinkSingleCycleList( LINK_SINGLE_CYCLE_LIST_TypeDef **pHead );
DATA_STRUCT_STATUS_ENUM InsertLinkSingleCycleList( LINK_SINGLE_CYCLE_LIST_TypeDef *pHead, void *dataIn, uint32_t dataSize,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) );
DATA_STRUCT_STATUS_ENUM DeleteLinkSingleCycleList( LINK_SINGLE_CYCLE_LIST_TypeDef *pHead,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) );
DATA_STRUCT_STATUS_ENUM PushLinkSingleCycleList( LINK_SINGLE_CYCLE_LIST_TypeDef *pHead, void *dataIn, uint32_t dataSize,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) );
DATA_STRUCT_STATUS_ENUM PopLinkSingleCycleList( LINK_SINGLE_CYCLE_LIST_TypeDef *pHead, void *dataOut, uint32_t dataSize,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) );
DATA_STRUCT_STATUS_ENUM ReverseLinkSingleCycleList( LINK_SINGLE_CYCLE_LIST_TypeDef *pHead );
DATA_STRUCT_STATUS_ENUM TraverseLinkSingleCycleList( LINK_SINGLE_CYCLE_LIST_TypeDef *pHead,\
		void (*show_CallBack)( void *nodeData ) );

#endif
/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
