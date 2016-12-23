#if 1

/*
*********************************************************************************************************
* @file    	App_Language.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-12-1 15:04:54
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	语言选择源文件
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

/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/
#define GUI_DIALOG_WIDTH					270
#define GUI_DIALOG_HEIGHT					150
#define GUI_DIALOG_START_X					(30)
#define GUI_DIALOG_START_Y					(30)


#define GUI_ID_DIALOG0						0

/*
*********************************************************************************************************
*                              				Private typedef
*********************************************************************************************************
*/
typedef struct {
	struct list_head handleHead;
}APP_LANGUAGE_TypeDef;

/*
*********************************************************************************************************
*                              				Private constants
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
	{ FRAMEWIN_CreateIndirect, 
	"语言选择",
	GUI_ID_DIALOG0,
  	GUI_DIALOG_START_X,
	GUI_DIALOG_START_Y,
	GUI_DIALOG_WIDTH, 
	GUI_DIALOG_HEIGHT, 
	FRAMEWIN_CF_MOVEABLE },
	
	{ RADIO_CreateIndirect,
	"Radio", 
	GUI_ID_RADIO0,
   	10,  
	20,  
	0, 
	0, 
	0,
	0x2402 },
	
	{ TEXT_CreateIndirect,
	"简体中文", 
	GUI_ID_TEXT0,
   	30,  
	15,  
	150, 
	24, 
	TEXT_CF_LEFT },
	
	{ TEXT_CreateIndirect,
	"English", 
    GUI_ID_TEXT1,  
 	30, 
	(15+35),  
	150,  
	24,
	TEXT_CF_LEFT },
	
	{ BUTTON_CreateIndirect,  
	"OK",  
	GUI_ID_OK, 
	180, 
	15, 
	60, 
	20 },
	
	{ BUTTON_CreateIndirect,  
	"Cancel",
	GUI_ID_CANCEL,
   	180, 
	45, 
	60, 
	20 },
};

static const char * _aLang[][SUPPORT_LANGUAGE_NUMS] = {
	{
		"语言选择",
		"Language Select",
	},	//1
	{
		"简体中文",
		"简体中文",
	},	//2
	{
		"English",
		"English",
	},	//3
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
static APP_LANGUAGE_TypeDef *this;


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
* Function Name : _cbDesktop
* Description	: 桌面窗口回调函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void _cbDesktop(WM_MESSAGE *pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;
	
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:				
			break;
		case WM_PAINT:
			_PaintFrame();
			break;
		case WM_KEY:
		{
			int key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
			
			switch (key)
			{
				case GUI_KEY_ESCAPE:
					App_MenuTaskCreate();
					break;
				default:				
					break;
			}
			break;
		}
		case WM_NOTIFY_PARENT:
		{
			int Id = WM_GetId(pMsg->hWinSrc);      
			int NCode = pMsg->Data.v;
			
			break;
		}
		case WM_SET_FOCUS:
		{
			break;
		}
		default:
			WM_DefaultProc(pMsg);
			break;
	}
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
static void Constructor(WM_MESSAGE *pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;
	
	FRAMEWIN_SetFont(hWin, FRAME_FONT);
	FRAMEWIN_SetTitleHeight(hWin, 30);
	FRAMEWIN_SetTextAlign(hWin, GUI_TA_HCENTER | GUI_TA_VCENTER);
	
	WM_HWIN hChild = WM_GetDialogItem(hWin, GUI_ID_TEXT0);			
	TEXT_SetFont(hChild, FRAME_TEXT_FONT);
	
	hChild = WM_GetDialogItem(hWin, GUI_ID_TEXT1);			
	TEXT_SetFont(hChild, FRAME_TEXT_FONT);	
	
	this = new(sizeof(APP_LANGUAGE_TypeDef));

	INIT_LIST_HEAD(&this->handleHead);
	
	{
		LIST_HANDLE_TypeDef *listHandle = new(sizeof(LIST_HANDLE_TypeDef));
		
		listHandle->handleLevel1 = WM_GetDialogItem(hWin, GUI_ID_BUTTON0);	
		listHandle->cursorHandle = listHandle->handleLevel1;
		list_add_tail(&listHandle->list, &this->handleHead);
	}

	{
		LIST_HANDLE_TypeDef *listHandle = new(sizeof(LIST_HANDLE_TypeDef));
		
		listHandle->handleLevel1 = WM_GetDialogItem(hWin, GUI_ID_BUTTON1);	
		listHandle->cursorHandle = listHandle->handleLevel1;
		list_add_tail(&listHandle->list, &this->handleHead);
	}

	WM_SelectWindow(hWin);
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] 构造 <语言选择> 窗口");
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
static void Destructor( WM_MESSAGE *pMsg )
{
	WM_DeleteWindow(pMsg->hWin);	
	
	LIST_HANDLE_TypeDef *handle;
	list_for_each_entry(handle, &this->handleHead, LIST_HANDLE_TypeDef, list)
	{
		delete(handle);
	}
	
	delete(this);
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] 析构 <语言选择> 窗口");
}

/*
*********************************************************************************************************
* Function Name : _cbDialog
* Description	: 对话框窗口回调函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void _cbDialog(WM_MESSAGE *pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;
	
	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:	
			Constructor(pMsg);
			break;
		case WM_PAINT:
			break;
		case WM_KEY:
		{
			int key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
			
			switch (key)
			{
				case GUI_KEY_UP:
				{
					RADIO_Dec(hWin);
					break;				
				}
				case GUI_KEY_DOWN:	
				{		
					RADIO_Inc(hWin);
					break;				
				}
				case GUI_KEY_ESCAPE:
					Destructor(pMsg);					
					break;
				default:				
					break;
			}
			break;
		}
		case WM_NOTIFY_PARENT:
		{
			int Id = WM_GetId(pMsg->hWinSrc);      
			int NCode = pMsg->Data.v;
			
			if (NCode == WM_NOTIFICATION_RELEASED)
			{
				switch(Id)
				{				
					case GUI_ID_BUTTON0:
					{
						SetLanguageType(LANG_CHINESE);
						Destructor(pMsg);
						App_MenuTaskCreate();
						break;
					}
					
					case GUI_ID_BUTTON1:
					{	
						SetLanguageType(LANG_ENGLISH);
						Destructor(pMsg);
						App_MenuTaskCreate();
						break;
					}				
				}
			}
			break;
		}
		case WM_SET_FOCUS:
		{
			LIST_HANDLE_TypeDef *handle = NULL;
			handle = list_first_entry(&this->handleHead, LIST_HANDLE_TypeDef, list);
			WM_SetFocus(handle->cursorHandle);
			break;
		}
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/*
*********************************************************************************************************
* Function Name : App_LanguageTaskCreate
* Description	: 创建语言应用程序任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void App_LanguageTaskCreate(void)
{
	WM_HWIN hWin = _CreateFrame(_cbDesktop);	
	GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbDialog, hWin, 0, 0);
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/

#else

/*
*********************************************************************************************************
* @file    	App_Language.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-12-1 15:04:54
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	语言选择源文件
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

/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/
#define START_WIDGET_X						((CONST_FRAME_WIDTH - BUTTON_LENTH) >> 1)
#define START_WIDGET_Y						100
#define WIDGET_ROWS_DISTANCE				20
#define WIDGET_COLUMNS_DISTANCE				60

#define BUTTON_LENTH						250	
#define BUTTON_WIDTH						60

/*
*********************************************************************************************************
*                              				Private typedef
*********************************************************************************************************
*/
typedef struct {
	struct list_head handleHead;
}APP_LANGUAGE_TypeDef;

/*
*********************************************************************************************************
*                              				Private constants
*********************************************************************************************************
*/
static const char * _aLang[][SUPPORT_LANGUAGE_NUMS] = {
	{
		"简体中文",
		"简体中文",
	},	//1
	{
		"English",
		"English",
	},	//2
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
static APP_LANGUAGE_TypeDef *this;


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
static void Constructor(WM_MESSAGE *pMsg) 
{
	uint16_t x = START_WIDGET_X;
	uint16_t y = START_WIDGET_Y;
	WM_HWIN hWin = pMsg->hWin;
	
	this = new(sizeof(APP_LANGUAGE_TypeDef));

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

	WM_SendMessageNoPara(hWin, WM_SET_FOCUS);
	WM_SelectWindow(hWin);
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] 构造 <语言选择> 窗口");
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
static void Destructor( WM_MESSAGE *pMsg )
{
	WM_DeleteWindow(pMsg->hWin);	
	
	LIST_HANDLE_TypeDef *handle;
	list_for_each_entry(handle, &this->handleHead, LIST_HANDLE_TypeDef, list)
	{
		delete(handle);
	}
	
	delete(this);
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] 析构 <语言选择> 窗口");
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
static void _cbCallback(WM_MESSAGE *pMsg) 
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
			int Id = WM_GetId(pMsg->hWinSrc);      
			int NCode = pMsg->Data.v;
			
			if (NCode == WM_NOTIFICATION_RELEASED)
			{
				switch(Id)
				{				
					case GUI_ID_BUTTON0:
					{
						SetLanguageType(LANG_CHINESE);
						Destructor(pMsg);
						App_MenuTaskCreate();
						break;
					}
					
					case GUI_ID_BUTTON1:
					{	
						SetLanguageType(LANG_ENGLISH);
						Destructor(pMsg);
						App_MenuTaskCreate();
						break;
					}				
				}
			}
			break;
		}
		case WM_SET_FOCUS:
		{
			LIST_HANDLE_TypeDef *handle = NULL;
			handle = list_first_entry(&this->handleHead, LIST_HANDLE_TypeDef, list);
			WM_SetFocus(handle->cursorHandle);
			break;
		}
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/*
*********************************************************************************************************
* Function Name : App_LanguageTaskCreate
* Description	: 创建语言应用程序任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void App_LanguageTaskCreate(void)
{
	_CreateFrame(_cbCallback);
}


/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/

#endif


