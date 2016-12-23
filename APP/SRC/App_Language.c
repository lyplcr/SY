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

enum {
	LANGUAGE_CH_INDEX = 0,
	LANGUAGE_EN_INDEX,
};

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
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] 构造 <语言选择> 窗口");
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
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] 析构 <语言选择> 窗口");
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
	FRAMEWIN_SetTitleHeight(hWin, 30);
	FRAMEWIN_SetTextAlign(hWin, GUI_TA_HCENTER | GUI_TA_VCENTER);
	FRAMEWIN_SetText(hWin, _GetLang(1));
	
	WM_HWIN hChild = WM_GetDialogItem(hWin, GUI_ID_TEXT0);			
	TEXT_SetFont(hChild, FRAME_TEXT_FONT);
	
	hChild = WM_GetDialogItem(hWin, GUI_ID_TEXT1);			
	TEXT_SetFont(hChild, FRAME_TEXT_FONT);	
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
					RADIO_Dec(hWin);
					break;				
				}
				case GUI_KEY_DOWN:	
				{	
					RADIO_Inc(hWin);
					break;				
				}
				case GUI_KEY_ENTER:
				{
					int value = RADIO_GetValue(hWin);
					switch (value)
					{
						case LANGUAGE_CH_INDEX:
							SetLanguageType(LANG_CHINESE);
							break;
						case LANGUAGE_EN_INDEX:
							SetLanguageType(LANG_ENGLISH);							
							break;
						default:
							return;
					}					
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

