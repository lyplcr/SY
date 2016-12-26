/*
*********************************************************************************************************
* @file    	App_Menu.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-12-1 12:36:43
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	应用层菜单源文件
*********************************************************************************************************
* @attention
*
* 
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private Includes
*********************************************************************************************************
*/
#include "app.h"
#include "App_Menu.h"
#include "App_SystemParameter.h"
#include "App_Language.h"
#include "App_TimeDate.h"

/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/
#define START_WIDGET_X						10
#define START_WIDGET_Y						40
#define WIDGET_ROWS_DISTANCE				20
#define WIDGET_COLUMNS_DISTANCE				(CONST_FRAME_WIDTH - \
												BUTTON_LENTH*2 - START_WIDGET_X*2)

#define BUTTON_LENTH						250	
#define BUTTON_WIDTH						60

/*
*********************************************************************************************************
*                              				Private typedef
*********************************************************************************************************
*/
typedef struct {
	struct list_head handleHead;
}APP_MENU_TypeDef;


/*
*********************************************************************************************************
*                              				Private constants
*********************************************************************************************************
*/
static const char * _aLang[][SUPPORT_LANGUAGE_NUMS] = {
	{
		"系统参数设置",
		"System Parameter Set",
	},	//1
	{
		"时间日期设置",
		"Time Data Set",
	},	//2
	{
		"软件激活设置",
		"Software Active Set",							
	},	//3
	{
		"语言选择",
		"Language Select",						
	},	//4											
};


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
static APP_MENU_TypeDef *this;
static int g_widgetId = ERROR_WIDGET_ID;

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
* Function Name : _GetLang
* Description	: 获取文本
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static const char *_GetLang(uint32_t Index) 
{
	const char *p = LANG_EXCEPTION;
	uint8_t languageType = GetLanguageType();
	
	Index--;
	if ((Index < GUI_COUNTOF(_aLang)) && (languageType < GUI_COUNTOF(_aLang[0]))) 
	{
		p = _aLang[Index][languageType];
	}
	
	return p;
}

/*
*********************************************************************************************************
* Function Name : GetFocusListHandle
* Description	: 获取已经聚焦的链表句柄
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static LIST_HANDLE_TypeDef *GetFocusListHandle(void)
{
	LIST_HANDLE_TypeDef *handle;
	
	list_for_each_entry(handle, &this->handleHead, LIST_HANDLE_TypeDef, list)
	{
		if (WM_HasFocus(handle->cursorHandle) == true)
		{
			return handle;
		}
	}
	
	return NULL;
}

/*
*********************************************************************************************************
* Function Name : Constructor
* Description	: 构造函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void Constructor(WM_MESSAGE* pMsg) 
{
	uint16_t x = START_WIDGET_X;
	uint16_t y = START_WIDGET_Y;
	WM_HWIN hWin = pMsg->hWin;
	
	this = new(sizeof(APP_MENU_TypeDef));

	INIT_LIST_HEAD(&this->handleHead);
	
	{
		LIST_HANDLE_TypeDef *listHandle = new(sizeof(LIST_HANDLE_TypeDef));
		
		listHandle->handleLevel1 = _CreateButton(hWin, 
												FRAME_BUTTON_EFFECT,
												_GetLang(1), 
												GUI_ID_BUTTON0, 
												x, 
												y, 
												BUTTON_LENTH,  
												BUTTON_WIDTH);
		listHandle->cursorHandle = listHandle->handleLevel1;
		list_add_tail(&listHandle->list, &this->handleHead);
	}
	y += WIDGET_ROWS_DISTANCE + BUTTON_WIDTH;
	{
		LIST_HANDLE_TypeDef *listHandle = new(sizeof(LIST_HANDLE_TypeDef));
		
		listHandle->handleLevel1 = _CreateButton(hWin,
												FRAME_BUTTON_EFFECT,
												_GetLang(2), 
												GUI_ID_BUTTON1, 
												x, 
												y, 
												BUTTON_LENTH,  
												BUTTON_WIDTH);
		listHandle->cursorHandle = listHandle->handleLevel1;
		list_add_tail(&listHandle->list, &this->handleHead);
	}
	y += WIDGET_ROWS_DISTANCE + BUTTON_WIDTH;
	{
		LIST_HANDLE_TypeDef *listHandle = new(sizeof(LIST_HANDLE_TypeDef));
		
		listHandle->handleLevel1 = _CreateButton(hWin,
												FRAME_BUTTON_EFFECT,
												_GetLang(3), 
												GUI_ID_BUTTON2, 
												x, 
												y, 
												BUTTON_LENTH,  
												BUTTON_WIDTH);
		listHandle->cursorHandle = listHandle->handleLevel1;
		list_add_tail(&listHandle->list, &this->handleHead);
	}
	y += WIDGET_ROWS_DISTANCE + BUTTON_WIDTH;
	{
		LIST_HANDLE_TypeDef *listHandle = new(sizeof(LIST_HANDLE_TypeDef));
		
		listHandle->handleLevel1 = _CreateButton(hWin,
												FRAME_BUTTON_EFFECT,
												_GetLang(4), 
												GUI_ID_BUTTON3, 
												x, 
												y, 
												BUTTON_LENTH,  
												BUTTON_WIDTH);
		listHandle->cursorHandle = listHandle->handleLevel1;
		list_add_tail(&listHandle->list, &this->handleHead);
	}

	WM_SendMessageNoPara(hWin, WM_SET_FOCUS);
	WM_SelectWindow(hWin);
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] 构造 <菜单> 窗口");
}

/*
*********************************************************************************************************
* Function Name : Destructor
* Description	: 析构函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void Destructor(WM_MESSAGE* pMsg) 
{
	{
		LIST_HANDLE_TypeDef *handle = GetFocusListHandle();
		if (handle)
		{
			g_widgetId = WM_GetId(handle->cursorHandle);			
		}
	}
	
	WM_DeleteWindow(pMsg->hWin);
	
	LIST_HANDLE_TypeDef *handle;
	list_for_each_entry(handle, &this->handleHead, LIST_HANDLE_TypeDef, list)
	{
		delete(handle);
	}
	
	delete(this);
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] 析构 <菜单> 窗口");
}

/*
*********************************************************************************************************
* Function Name : _cbCallback
* Description	: 窗口回调函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void _cbCallback(WM_MESSAGE* pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;
	(void)hWin;
	
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:	
			Constructor(pMsg);
			break;
		case WM_PAINT:		
			_PaintFrame();			
			break;
		case WM_KEY:
		{
			int key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;			
			switch (key)
			{
				case GUI_KEY_UP:
				{
					LIST_HANDLE_TypeDef *handle = GetFocusListHandle();
					if (handle)
					{
						if (handle == list_first_entry(&this->handleHead, LIST_HANDLE_TypeDef, list))
						{
							handle = list_last_entry(&this->handleHead, LIST_HANDLE_TypeDef, list);
						}	
						else					
						{
							handle = list_prev_entry(handle, LIST_HANDLE_TypeDef, list);						
						}
						if (handle)
						{
							WM_SetFocus(handle->cursorHandle);
						}
					}
					break;				
				}
				case GUI_KEY_DOWN:	
				{
					LIST_HANDLE_TypeDef *handle = GetFocusListHandle();
					if (handle)
					{
						if (handle == list_last_entry(&this->handleHead, LIST_HANDLE_TypeDef, list))
						{
							handle = list_first_entry(&this->handleHead, LIST_HANDLE_TypeDef, list);
						}	
						else					
						{
							handle = list_next_entry(handle, LIST_HANDLE_TypeDef, list);						
						}
						if (handle)
						{
							WM_SetFocus(handle->cursorHandle);
						}
					}					
					break;				
				}
				default:				
				{					
					break;
				}
			}
			break;
		}
		case WM_NOTIFY_PARENT:
		{
			int Id    = WM_GetId(pMsg->hWinSrc);      
			int NCode = pMsg->Data.v;  
			
			if (NCode == WM_NOTIFICATION_RELEASED)
			{
				switch (Id)
				{
					case GUI_ID_BUTTON0:
						Destructor(pMsg);					
						App_SystemParameterTaskCreate();
						break;
					case GUI_ID_BUTTON1:
						Destructor(pMsg);
						App_TimeDateTaskCreate();
						break;
					case GUI_ID_BUTTON2:
						Destructor(pMsg);
						break;	
					case GUI_ID_BUTTON3:
						Destructor(pMsg);
						App_LanguageTaskCreate();
						break;
				}
			}
			break;
		}
		case WM_SET_FOCUS:
		{		
			if (g_widgetId == ERROR_WIDGET_ID)
			{
				LIST_HANDLE_TypeDef *handle = NULL;
				handle = list_first_entry(&this->handleHead, LIST_HANDLE_TypeDef, list);
				WM_SetFocus(handle->cursorHandle);
			}
			else
			{
				LIST_HANDLE_TypeDef *handle;
				list_for_each_entry(handle, &this->handleHead, LIST_HANDLE_TypeDef, list)
				{
					if (WM_GetId(handle->cursorHandle) == g_widgetId)
					{
						WM_SetFocus(handle->cursorHandle);
						break;
					}
				}
			}
			break;
		}
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/*
*********************************************************************************************************
* Function Name : App_MenuTaskCreate
* Description	: 创建菜单任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void App_MenuTaskCreate(void)
{
	ECHO(DEBUG_APP_WINDOWS, "[emWin] 剩余内存 %ld Byte", GUI_ALLOC_GetNumFreeBytes());
	
	_CreateFrame(_cbCallback);
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
