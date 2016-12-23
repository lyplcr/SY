/*
*********************************************************************************************************
* @file    	LinkSingleList.h
* @author  	SY
* @version 	V1.0.0
* @date    	2016-9-8 15:56:20
* @IDE	 	V4.70.0.0
* @Chip    	STM32F407VE
* @brief   	单向链表头文件
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
#ifndef __LINK_SINGLE_LIST_H
#define __LINK_SINGLE_LIST_H

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
typedef struct tagLINK_SINGLE_LIST_TypeDef
{
	void *data;
	struct tagLINK_SINGLE_LIST_TypeDef *next;
}LINK_SINGLE_LIST_TypeDef;

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
LINK_SINGLE_LIST_TypeDef *CreateLinkSingleList( void );
DATA_STRUCT_STATUS_ENUM LinkSingleListIsEmpty( LINK_SINGLE_LIST_TypeDef *pHead );
uint32_t GetLinkSingleListLenth( LINK_SINGLE_LIST_TypeDef *pHead );
void ClearLinkSingleList( LINK_SINGLE_LIST_TypeDef *pHead );
void DestoryLinkSingleList( LINK_SINGLE_LIST_TypeDef **pHead );
DATA_STRUCT_STATUS_ENUM InsertLinkSingleList( LINK_SINGLE_LIST_TypeDef *pHead, void *dataIn, uint32_t dataSize,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) );
DATA_STRUCT_STATUS_ENUM DeleteLinkSingleList( LINK_SINGLE_LIST_TypeDef *pHead,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) );
DATA_STRUCT_STATUS_ENUM PushLinkSingleList( LINK_SINGLE_LIST_TypeDef *pHead, void *dataIn, uint32_t dataSize,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) );
DATA_STRUCT_STATUS_ENUM PopLinkSingleList( LINK_SINGLE_LIST_TypeDef *pHead, void *dataOut, uint32_t dataSize,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) );
DATA_STRUCT_STATUS_ENUM ReverseLinkSingleList( LINK_SINGLE_LIST_TypeDef *pHead );
DATA_STRUCT_STATUS_ENUM TraverseLinkSingleList( LINK_SINGLE_LIST_TypeDef *pHead,\
		void (*show_CallBack)( void *nodeData ) );

#endif
/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
