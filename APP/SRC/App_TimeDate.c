/*
*********************************************************************************************************
* @file    	App_TimeDate.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-12-26 16:20:16
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	时间日期设置源文件
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
#define GUI_ID_DIALOG0     					(GUI_ID_USER + 0x00)

#define GUI_DIALOG_WIDTH					320
#define GUI_DIALOG_HEIGHT					150
#define GUI_DIALOG_START_X					(30)
#define GUI_DIALOG_START_Y					(30)



/*
*********************************************************************************************************
*                              				Private typedef
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private constants
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
	{ FRAMEWIN_CreateIndirect, "Framewin", GUI_ID_DIALOG0, 
		GUI_DIALOG_START_X, GUI_DIALOG_START_Y, GUI_DIALOG_WIDTH, GUI_DIALOG_HEIGHT, 
		0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Text", GUI_ID_TEXT0, 
		10, 20, 36, 24, 
		TEXT_CF_LEFT, 0x0, 0 },
	{ SPINBOX_CreateIndirect, "Spinbox", GUI_ID_SPINBOX0,
		50, 20, 48, 24, 
		TEXT_CF_HCENTER | TEXT_CF_VCENTER, 0x0, 0 },
	{ TEXT_CreateIndirect, "Text", GUI_ID_TEXT1, 
		120, 20, 36, 24, 
		TEXT_CF_LEFT, 0x0, 0 },
	{ SPINBOX_CreateIndirect, "Spinbox", GUI_ID_SPINBOX1, 
		160, 20, 48, 24,  
		TEXT_CF_HCENTER | TEXT_CF_VCENTER, 0x0, 0 },
	{ TEXT_CreateIndirect, "Text", GUI_ID_TEXT2, 
		230, 20, 36, 24, 
		TEXT_CF_LEFT, 0x0, 0 },
	{ SPINBOX_CreateIndirect, "Spinbox", GUI_ID_SPINBOX2,
		270, 20, 48, 24, 
		TEXT_CF_HCENTER | TEXT_CF_VCENTER, 0x0, 0 },
};

static const char * _aLang[][SUPPORT_LANGUAGE_NUMS] = {
	{
		"时间日期设置",
		"Time Data Set",
	},	//1
	{
		"年",
		"Year",
	},	//2
	{
		"月",
		"Month",
	},	//3
	{
		"日",
		"Day",
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
* Function Name : WindowsConstructor
* Description	: 窗口构造函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void WindowsConstructor(WM_MESSAGE *pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;
	WM_SelectWindow(hWin);
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] 构造 <时间日期> 窗口");
}

/*
*********************************************************************************************************
* Function Name : WindowsDestructor
* Description	: 窗口析构函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void WindowsDestructor( WM_MESSAGE *pMsg )
{
	WM_DeleteWindow(pMsg->hWin);	
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] 析构 <时间日期> 窗口");
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
	(void)hWin;
	static bool deleteFlag = false;
	
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:	
			deleteFlag = false;
			WindowsConstructor(pMsg);
			break;
		case WM_PAINT:
			_PaintFrame();
			break;
		case WM_KEY:
		{
			break;
		}		
		case WM_NOTIFY_PARENT:
		{
			int NCode = pMsg->Data.v;
			
			/* 该命令会收到多次，只需要执行一次 */	
			if (NCode == WM_NOTIFICATION_CHILD_DELETED)
			{
				if (deleteFlag == false)
				{
					deleteFlag = true;
					
					WindowsDestructor(pMsg);
					App_MenuTaskCreate();
				}
			}
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
* Function Name : DialogConstructor
* Description	: 对话框构造函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void DialogConstructor(WM_MESSAGE *pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;
	
	FRAMEWIN_SetFont(hWin, FRAME_FONT);
	FRAMEWIN_SetTitleHeight(hWin, DIALOG_TITLE_HEIGHT);
	FRAMEWIN_SetTextAlign(hWin, GUI_TA_HCENTER | GUI_TA_VCENTER);
	FRAMEWIN_SetText(hWin, _GetLang(1));
	
	WM_HWIN hChild = WM_GetDialogItem(hWin, GUI_ID_TEXT0);			
	TEXT_SetFont(hChild, FRAME_TEXT_FONT);
	TEXT_SetText(hChild, _GetLang(2));
	
	hChild = WM_GetDialogItem(hWin, GUI_ID_TEXT1);			
	TEXT_SetFont(hChild, FRAME_TEXT_FONT);
	TEXT_SetText(hChild, _GetLang(3));

	hChild = WM_GetDialogItem(hWin, GUI_ID_TEXT2);			
	TEXT_SetFont(hChild, FRAME_TEXT_FONT);	
	TEXT_SetText(hChild, _GetLang(4));
	
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
			DialogConstructor(pMsg);
			break;
		case WM_CREATE:
		{
			break;
		}
		case WM_PAINT:
			break;
		case WM_KEY:
		{
			int key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
			
			switch (key)
			{
				case GUI_KEY_UP:
				{
					
					break;				
				}
				case GUI_KEY_DOWN:	
				{	
					
					break;				
				}
				case GUI_KEY_ENTER:
				{									
					GUI_EndDialog(hWin, 0);					
					break;
				}
				case GUI_KEY_ESCAPE:
				{
					GUI_EndDialog(hWin, 1);	
					break;
				}
				default:				
					break;
			}
			break;
		}
		case WM_NOTIFY_PARENT:
		{
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
* Function Name : App_TimeDateTaskCreate
* Description	: 创建时间日期应用程序任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void App_TimeDateTaskCreate(void)
{
	WM_HWIN hWin = _CreateFrame(_cbDesktop);	
	GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbDialog, hWin, 0, 0);
}


/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
