/*
*********************************************************************************************************
* @file    	App_SystemParameter.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-12-2 15:45:07
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	系统参数页面源文件
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
#include "App_SystemParameter.h"
#include "App_Language.h"
#include "App_Menu.h"
#include "App_TypeWrite.h"

/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/
#define START_WIDGET_X						10
#define START_WIDGET_Y						10
#define WIDGET_ROWS_DISTANCE				24
#define WIDGET_COLUMNS_DISTANCE				(0)

#define TEXT_LENTH							250	
#define TEXT_WIDTH							32
#define EDIT_LENTH							250	
#define EDIT_WIDTH							32
#define EDIT_BOTTOM_BUTTON_LENTH_DISTANCE	3
#define EDIT_BOTTOM_BUTTON_WIDTH_DISTANCE	3
#define EDIT_BOTTOM_BUTTON_LENTH			(EDIT_LENTH+2*EDIT_BOTTOM_BUTTON_LENTH_DISTANCE)
#define EDIT_BOTTOM_BUTTON_WIDTH			(EDIT_WIDTH+2*EDIT_BOTTOM_BUTTON_WIDTH_DISTANCE)


/*
*********************************************************************************************************
*                              				Private typedef
*********************************************************************************************************
*/
typedef struct {
	LIST_HANDLE_TypeDef curWidgetHandle;		
	char stringBuff[MAX_STRING_NUM+1];
	struct list_head handleHead;
}APP_SYSTEM_PARAMETER_TypeDef;


/*
*********************************************************************************************************
*                              				Private constants
*********************************************************************************************************
*/
static const char * _aLang[][SUPPORT_LANGUAGE_NUMS] = {
	{
		"系统最大力：",
		"MaxForceOfSystem:",
	},													//1
	{
		"最大控制量：",
		"MaxControlDegree:",
	},													//2
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
static APP_SYSTEM_PARAMETER_TypeDef *this;

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
	
	this = new(sizeof(APP_SYSTEM_PARAMETER_TypeDef));

	INIT_LIST_HEAD(&this->handleHead);
	
	/*
		编辑框聚焦后，直接进入输入状态。
		因此不能单独作为控件，表示当前焦点位置。
		需要在文本控件下面需要放置按钮。
	 */
	{
		LIST_HANDLE_TypeDef *listHandle = new(sizeof(LIST_HANDLE_TypeDef));
		
		listHandle->handleLevel1 = _CreateButton(hWin,
												FRAME_BUTTON_EFFECT,
												"", 
												GUI_ID_BUTTON0, 
												x+TEXT_LENTH, 
												y, 
												EDIT_BOTTOM_BUTTON_LENTH,  
												EDIT_BOTTOM_BUTTON_WIDTH);
		listHandle->handleLevel2 = _CreateEdit(hWin, 
												FRAME_EDIT_EFFECT,
												"", 
												GUI_ID_EDIT0, 
												TEXT_CF_LEFT | TEXT_CF_VCENTER,
												MAX_TEXT_PUTIN_LENTH,
												x+TEXT_LENTH+EDIT_BOTTOM_BUTTON_LENTH_DISTANCE, 
												y+EDIT_BOTTOM_BUTTON_WIDTH_DISTANCE, 
												EDIT_LENTH,  
												EDIT_WIDTH);
		listHandle->handleLevel3 = _CreateText(hWin,
												_GetLang(1),
												GUI_ID_TEXT0,
												TEXT_CF_RIGHT | TEXT_CF_VCENTER,
												x, 
												y+EDIT_BOTTOM_BUTTON_WIDTH_DISTANCE, 
												TEXT_LENTH,  
												TEXT_WIDTH);
		listHandle->cursorHandle = listHandle->handleLevel1;
		list_add_tail(&listHandle->list, &this->handleHead);
	}
	y += WIDGET_ROWS_DISTANCE + EDIT_BOTTOM_BUTTON_WIDTH;
	{
		LIST_HANDLE_TypeDef *listHandle = new(sizeof(LIST_HANDLE_TypeDef));
		
		listHandle->handleLevel1 = _CreateButton(hWin, 
												FRAME_BUTTON_EFFECT,
												"", 
												GUI_ID_BUTTON1, 
												x+TEXT_LENTH, 
												y, 
												EDIT_BOTTOM_BUTTON_LENTH,  
												EDIT_BOTTOM_BUTTON_WIDTH);
		listHandle->handleLevel2 = _CreateEdit(hWin,
												FRAME_EDIT_EFFECT,
												"", 
												GUI_ID_EDIT1, 
												TEXT_CF_LEFT | TEXT_CF_VCENTER,
												MAX_TEXT_PUTIN_LENTH,
												x+TEXT_LENTH+EDIT_BOTTOM_BUTTON_LENTH_DISTANCE, 
												y+EDIT_BOTTOM_BUTTON_WIDTH_DISTANCE, 
												EDIT_LENTH,  
												EDIT_WIDTH);	
		listHandle->handleLevel3 = _CreateText(hWin,
												_GetLang(2),
												GUI_ID_TEXT1,
												TEXT_CF_RIGHT | TEXT_CF_VCENTER,
												x, 
												y+EDIT_BOTTOM_BUTTON_WIDTH_DISTANCE, 
												TEXT_LENTH,  
												TEXT_WIDTH);
		listHandle->cursorHandle = listHandle->handleLevel1;
		list_add_tail(&listHandle->list, &this->handleHead);
	}
	

	WM_SendMessageNoPara(hWin, WM_SET_FOCUS);
	WM_SelectWindow(hWin);
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] 构造 <系统参数设置> 窗口");
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
	WM_DeleteWindow(pMsg->hWin);
	
	LIST_HANDLE_TypeDef *handle;
	list_for_each_entry(handle, &this->handleHead, LIST_HANDLE_TypeDef, list)
	{
		delete(handle);
	}
	
	delete(this);
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] 析构 <系统参数设置> 窗口");
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
				case GUI_KEY_ESCAPE:
					Destructor(pMsg);
					App_MenuTaskCreate();
					break;
				default:
					
					break;
			}
			break;
		}
		case WM_NOTIFY_PARENT:
		{
			int Id    = WM_GetId(pMsg->hWinSrc);      
			int NCode = pMsg->Data.v;  
			
			if (NCode == WM_NOTIFICATION_RELEASED)
			{
				/* 记录当前光标所在控件信息 */
				{
					this->curWidgetHandle = *GetFocusListHandle();
					EDIT_GetText(this->curWidgetHandle.handleLevel2, this->stringBuff, MAX_STRING_NUM);
				}
				
				switch (Id)
				{
					case GUI_ID_BUTTON0:
						_CreateNumPad(hWin, CONST_FRAME_WIDTH >> 1, 150);	
						break;
					case GUI_ID_BUTTON1:
						_CreateNumPad(hWin, CONST_FRAME_WIDTH >> 1, 200);
						break;
				}
			}			
			break;
		}
		case MSG_USER_PAD_CHANGED:
		case MSG_USER_PAD_DELETE:
		case MSG_USER_PAD_CLEAR:
		{		
			EDIT_SetText(this->curWidgetHandle.handleLevel2, pMsg->Data.p);
			break;
		}		
		case MSG_USER_PAD_CANCEL:	
		{		
			EDIT_SetText(this->curWidgetHandle.handleLevel2, this->stringBuff);
			WM_SetFocus(this->curWidgetHandle.handleLevel1);
			break;
		}
		case MSG_USER_PAD_OK:	
		{
			WM_SetFocus(this->curWidgetHandle.handleLevel1);
			break;
		}
		case WM_SET_FOCUS:
		{
			LIST_HANDLE_TypeDef *handle = NULL;
			handle = list_first_entry(&this->handleHead, LIST_HANDLE_TypeDef, list);
			WM_SetFocus(handle->cursorHandle);
			break;
		}
		case WM_GET_RADIOGROUP:		
		{
			LIST_HANDLE_TypeDef *handle;
			
			list_for_each_entry(handle, &this->handleHead, LIST_HANDLE_TypeDef, list)
			{
				int id = WM_GetId(handle->handleLevel3);
				switch (id)
				{
					case GUI_ID_TEXT0:
						TEXT_SetText(handle->handleLevel3,_GetLang(1));
						break;
					case GUI_ID_TEXT1:
						TEXT_SetText(handle->handleLevel3,_GetLang(2));
						break;
					case GUI_ID_TEXT2:
						TEXT_SetText(handle->handleLevel3,_GetLang(3));
						break;					
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
* Function Name : App_SystemParameterTaskCreate
* Description	: 创建系统参数任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void App_SystemParameterTaskCreate( void )
{
	_CreateFrame(_cbCallback);
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
