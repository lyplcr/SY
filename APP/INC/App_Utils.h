/*
*********************************************************************************************************
* @file    	App_Utils.h
* @author  	SY
* @version 	V1.0.0
* @date    	2016-12-1 11:28:11
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	应用层工具类头文件
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
#ifndef __APP_UTILS_H
#define __APP_UTILS_H

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
#define SUPPORT_LANGUAGE_NUMS				(2)
#define LANG_EXCEPTION						"LangException"
#define MAX_TEXT_PUTIN_LENTH				(32)

/* 自定义消息 */
#define MSG_USER_PAD_CHANGED     			(WM_USER + 0)
#define MSG_USER_PAD_OK         			(WM_USER + 1)
#define MSG_USER_PAD_CANCEL      			(WM_USER + 2)
#define MSG_USER_PAD_DELETE       			(WM_USER + 3)
#define MSG_USER_PAD_CLEAR       			(WM_USER + 4)

/*
*********************************************************************************************************
*                              				Exported types
*********************************************************************************************************
*/
typedef enum {
	LANG_CHINESE = 0,
	LANG_ENGLISH,
}LANGUAGE_TYPE_TypeDef;

/**
  *	由于存在多个控件表示一个逻辑项的情况，
  *	需要有一种机制，通过链表找到任意一层的句柄。
  * 最多支持3层控件叠加。
  */
typedef struct {
	WM_HWIN cursorHandle;			//当前光标所在的句柄
	WM_HWIN handleLevel1;
	WM_HWIN handleLevel2;
	WM_HWIN handleLevel3;
	struct list_head list;
}LIST_HANDLE_TypeDef;

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
void _DrawDownRect(const WIDGET_EFFECT* pEffect, int x0, int y0, int x1, int y1);
void _DrawUpRectEx(const WIDGET_EFFECT* pEffect, const GUI_RECT* pRect);
void _DrawUpRect(const WIDGET_EFFECT* pEffect, int x0, int y0, int x1, int y1);
WM_HWIN _CreateFrame(WM_CALLBACK* cb);
WM_HWIN _CreateButton(WM_HWIN hParent, 
						const WIDGET_EFFECT* pEffect,
						const char* pText, 
						int Id,						
						int x, 
						int y, 
						int w, 
						int h);
WM_HWIN _CreateEdit(WM_HWIN hParent, 
					const WIDGET_EFFECT* pEffect,
					const char* pText, 
					int Id,
					int ExFlags, 
					int MaxLen,
					int x, 
					int y, 
					int w, 
					int h);
WM_HWIN _CreateText(WM_HWIN hParent, 
					const char* pText, 
					int Id,
					int ExFlags, 
					int x, 
					int y, 
					int w, 
					int h);	
WM_HWIN _CreateRadio(WM_HWIN hParent, 
					const char* pText[],
					int Id,
					int ExFlags, 
					int NumItems,
					int Spacing,
					int x, 
					int y, 
					int w, 
					int h);					

void _PaintFrame(void);
LANGUAGE_TYPE_TypeDef GetLanguageType( void );
void SetLanguageType( LANGUAGE_TYPE_TypeDef type);
void MoveWindows(WM_HWIN sourceWindows, WM_HWIN targetWindows);
WM_HWIN GetLastUsedWindows(void);

#endif
/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
