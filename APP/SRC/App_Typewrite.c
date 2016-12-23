/*
*********************************************************************************************************
* @file    	App_Typewrite.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-12-2 13:46:06
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	输入法源文件
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
#include "App_Typewrite.h"
#include "SeqList.h"

/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/
#define NUMPAD_BKCOLOR           			GUI_LIGHTGRAY
#define NUMPAD_EFFECT             			(&WIDGET_Effect_3D2L)
#define NUMPAD_BORDER             			9

#define NUMPAD_BUTTON_BKCOLOR0    			0xB8B8B8              /* Unpressed */
#define NUMPAD_BUTTON_BKCOLOR1    			0xE0E0E0              /* Pressed   */
#define NUMPAD_BUTTON_BKCOLOR2    			0xC0C0C0              /* Disabled  */
#define NUMPAD_BUTTON_COLOR0      			0x700000              /* Unpressed */
#define NUMPAD_BUTTON_COLOR1      			0x700000              /* Pressed   */
#define NUMPAD_BUTTON_RED0        			0x0060FF              /* Unpressed */
#define NUMPAD_BUTTON_RED1        			0x0030E8              /* Pressed   */
#define NUMPAD_BUTTON_YELLOW0     			0x00F0FF              /* Unpressed */
#define NUMPAD_BUTTON_YELLOW1     			0x00B0D0              /* Pressed   */
#define NUMPAD_BUTTON_GREEN0      			0x00B040              /* Unpressed */
#define NUMPAD_BUTTON_GREEN1     	 		0x00D000              /* Pressed   */

#define NUMPAD_BUTTON_FONT        			(&GUI_FontSongTi_24X24)
#define NUMPAD_BUTTON_EFFECT      			(&WIDGET_Effect_3D1L)
#define NUMPAD_BUTTON_WIDTH       			40
#define NUMPAD_BUTTON_HEIGHT      			32
#define NUMPAD_BUTTON_DIST_X      			7
#define NUMPAD_BUTTON_DIST_Y      			7

#define NUMPAD_ID_0         				(GUI_ID_USER +  0)
#define NUMPAD_ID_1         				(GUI_ID_USER +  1)
#define NUMPAD_ID_2         				(GUI_ID_USER +  2)
#define NUMPAD_ID_3         				(GUI_ID_USER +  3)
#define NUMPAD_ID_4         				(GUI_ID_USER +  4)
#define NUMPAD_ID_5         				(GUI_ID_USER +  5)
#define NUMPAD_ID_6         				(GUI_ID_USER +  6)
#define NUMPAD_ID_7         				(GUI_ID_USER +  7)
#define NUMPAD_ID_8         				(GUI_ID_USER +  8)
#define NUMPAD_ID_9         				(GUI_ID_USER +  9)
#define NUMPAD_ID_MINUS         			(GUI_ID_USER + 10)
#define NUMPAD_ID_POINT         			(GUI_ID_USER + 11)
#define NUMPAD_ID_DELETE         			(GUI_ID_USER + 12)
#define NUMPAD_ID_CANCEL         			(GUI_ID_USER + 13)
#define NUMPAD_ID_ENTER         			(GUI_ID_USER + 14)
#define NUMPAD_ID_CLEAR         			(GUI_ID_USER + 15)

#define NUMPAD_ROW_NUMS						4
#define NUMPAD_COLUMNS_NUMS					4

/*
*********************************************************************************************************
*                              				Private typedef
*********************************************************************************************************
*/
typedef struct {
	uint8_t putinNums;
	struct list_head handleHead;
	SEQ_LIST_TypeDef padList;
	char padBuff[MAX_STRING_NUM+1];
}APP_TYPEWRITE_TypeDef;

/*
*********************************************************************************************************
*                              				Private constants
*********************************************************************************************************
*/
static const char * _aLang[][SUPPORT_LANGUAGE_NUMS] = {
	{
		"删除",
		"Delete",
	},													//1
	{
		"清空",
		"Clear",
	},													//2
	{
		"确认",
		"OK",
	},													//3
	{
		"取消",
		"Cancel",										
	},													//4	
		
														
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
/* 输入法输入位数 */
static uint8_t g_maxPutinNums = MAX_STRING_NUM;
static APP_TYPEWRITE_TypeDef *this;

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
* Function Name : SetPadMaxPutinNums
* Description	: 设置键盘最大输入个数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void SetPadMaxPutinNums( uint8_t nums )
{
	if (nums > MAX_STRING_NUM)
	{
		nums = MAX_STRING_NUM;
	}
	g_maxPutinNums = nums;
}

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
* Function Name : _CreateNumPadButton
* Description	: 创建数字小键盘按钮
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static WM_HWIN _CreateNumPadButton(WM_HWIN hParent, 
									const char* pText, 
									int Id,
									int x, 
									int y, 
									int w, 
									int h,
									GUI_COLOR Color, 
									GUI_COLOR PressedColor, 
									int Enable, 
									unsigned TextId)
{
	WM_HWIN hButton;
	hButton = BUTTON_CreateEx(x, y, w, h, hParent, WM_CF_SHOW, 0, Id);
	if (Enable) 
	{
		if (TextId) 
		{
			pText = _GetLang(TextId);
		}
		BUTTON_SetText(hButton, pText);
	} 
	else 
	{
		WM_DisableWindow(hButton);
	}
	BUTTON_SetFont      (hButton,   NUMPAD_BUTTON_FONT);
	BUTTON_SetBkColor   (hButton, 	BUTTON_CI_UNPRESSED, 	NUMPAD_BUTTON_BKCOLOR0);
	BUTTON_SetBkColor   (hButton, 	BUTTON_CI_PRESSED, 		NUMPAD_BUTTON_BKCOLOR1);
	BUTTON_SetBkColor   (hButton, 	BUTTON_CI_DISABLED, 	NUMPAD_BUTTON_BKCOLOR2);
	BUTTON_SetTextColor (hButton, 	BUTTON_CI_UNPRESSED, 	Color);
	BUTTON_SetTextColor (hButton, 	BUTTON_CI_PRESSED, 		PressedColor);
	BUTTON_SetTextColor (hButton, 	BUTTON_CI_DISABLED, 	Color);
	BUTTON_SetFocussable(hButton, 	true);
	WIDGET_SetEffect    (hButton, 	NUMPAD_BUTTON_EFFECT);
	
	return hButton;
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
	WM_HWIN hWin = pMsg->hWin;
	
	this->putinNums = 0;
	CreateSeqList(&this->padList, this->padBuff, MAX_STRING_NUM);
	
	WM_SelectWindow(hWin);
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] 构造 <输入法> 窗口");
}

/*
*********************************************************************************************************
* Function Name : _DeleteSystemParameterWindows
* Description	: 删除系统参数窗口
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void _DeleteTypeWriteWindows(WM_MESSAGE* pMsg)
{
	WM_DeleteWindow(pMsg->hWin);
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
	_DeleteTypeWriteWindows(pMsg);
	
	LIST_HANDLE_TypeDef *handle;
	list_for_each_entry(handle, &this->handleHead, LIST_HANDLE_TypeDef, list)
	{
		delete(handle);
	}
	
	delete(this);
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] 析构 <输入法> 窗口");
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
* Function Name : _cbNumPad
* Description	: 数字小键盘回调函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void _cbNumPad(WM_MESSAGE* pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;
	
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:
			Constructor(pMsg);			
			break;
		case WM_PAINT: 
		{
			GUI_RECT r;
			WM_GetClientRect(&r);
			GUI_SetBkColor(NUMPAD_BKCOLOR);
			GUI_Clear();
			_DrawUpRectEx(NUMPAD_EFFECT, &r);
			break;
		} 		
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
						int Id = WM_GetId(handle->cursorHandle);
						switch (Id)
						{
							case NUMPAD_ID_1:
							case NUMPAD_ID_2:
							case NUMPAD_ID_3:
							case NUMPAD_ID_DELETE:
								return;
							default:								
								break;
						}
						for (uint8_t i=0; i<NUMPAD_COLUMNS_NUMS; ++i)
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
						int Id = WM_GetId(handle->cursorHandle);
						switch (Id)
						{
							case NUMPAD_ID_MINUS:
							case NUMPAD_ID_0:
							case NUMPAD_ID_POINT:
							case NUMPAD_ID_CANCEL:
								return;
							default:								
								break;
						}
						for (uint8_t i=0; i<NUMPAD_COLUMNS_NUMS; ++i)
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
				case GUI_KEY_LEFT:
				{
					LIST_HANDLE_TypeDef *handle = GetFocusListHandle();
					if (handle)
					{
						int Id = WM_GetId(handle->cursorHandle);
						switch (Id)
						{
							case NUMPAD_ID_1:
							case NUMPAD_ID_4:
							case NUMPAD_ID_7:
							case NUMPAD_ID_MINUS:
								return;
							default:								
								break;
						}
						handle = list_prev_entry(handle, LIST_HANDLE_TypeDef, list);					
						if (handle)
						{
							WM_SetFocus(handle->cursorHandle);
						}
					}
					break;
				}			
				case GUI_KEY_RIGHT:	
				{
					LIST_HANDLE_TypeDef *handle = GetFocusListHandle();
					if (handle)
					{
						int Id = WM_GetId(handle->cursorHandle);
						switch (Id)
						{
							case NUMPAD_ID_DELETE:
							case NUMPAD_ID_CANCEL:
							case NUMPAD_ID_ENTER:
							case NUMPAD_ID_CLEAR:
								return;
							default:								
								break;
						}
						handle = list_next_entry(handle, LIST_HANDLE_TypeDef, list);					
						if (handle)
						{
							WM_SetFocus(handle->cursorHandle);
						}
					}
					break;
				}
				case GUI_KEY_ESCAPE:	
				{
					WM_SendMessageNoPara(hWin, MSG_USER_PAD_CANCEL);
					break;
				}
			}
			break;
		}
		case MSG_USER_PAD_OK:
		{
			WM_SendMessageNoPara(WM_GetParent(hWin), MSG_USER_PAD_OK);						
			Destructor(pMsg);
			break;
		}
		case MSG_USER_PAD_CANCEL:
		{
			WM_SendMessageNoPara(WM_GetParent(hWin), MSG_USER_PAD_CANCEL);						
			Destructor(pMsg);
			break;
		}
		case WM_SET_FOCUS:
		{			
			LIST_HANDLE_TypeDef *handle = NULL;
			handle = list_first_entry(&this->handleHead, LIST_HANDLE_TypeDef, list);
			WM_SetFocus(handle->cursorHandle);
			break;
		}
		case WM_NOTIFY_PARENT:
		{   
			int NCode = pMsg->Data.v; 		
			if (NCode == WM_NOTIFICATION_RELEASED)
			{
				bool isAvailPutin = false;
				bool isNotifyFather = false;
				char c = NULL;
				WM_MESSAGE msg = {0};
				
				int Id = WM_GetId(pMsg->hWinSrc);
				switch (Id) 
				{
					case NUMPAD_ID_0:
					{
						isAvailPutin = true;
						c = '0';
						break;
					}
					case NUMPAD_ID_1:
					{
						isAvailPutin = true;
						c = '1';
						break;
					}
					case NUMPAD_ID_2:
					{
						isAvailPutin = true;
						c = '2';
						break;
					}
					case NUMPAD_ID_3:
					{
						isAvailPutin = true;
						c = '3';
						break;
					}
					case NUMPAD_ID_4:
					{
						isAvailPutin = true;
						c = '4';
						break;
					}
					case NUMPAD_ID_5:
					{
						isAvailPutin = true;
						c = '5';
						break;
					}
					case NUMPAD_ID_6:
					{
						isAvailPutin = true;
						c = '6';
						break;
					}
					case NUMPAD_ID_7:
					{
						isAvailPutin = true;
						c = '7';
						break;
					}
					case NUMPAD_ID_8:
					{
						isAvailPutin = true;
						c = '8';
						break;
					}
					case NUMPAD_ID_9:
					{
						isAvailPutin = true;
						c = '9';
						break;
					}
					case NUMPAD_ID_MINUS:
					{
						isAvailPutin = true;
						c = '-';
						break;
					}
					case NUMPAD_ID_POINT:
					{
						isAvailPutin = true;
						c = '.';
						break;
					}
					case NUMPAD_ID_DELETE:   
					{
						if (this->putinNums) 
						{
							this->putinNums--;							
							if (this->putinNums)
							{
								char buff = NULL;
								PushSeqList(&this->padList,
											&buff,
											this->putinNums,
											PushSeqListU8_CallBack);
								DeleteSeqList(&this->padList,
										  this->putinNums,
										  CopySeqListU8_CallBack);
							}
							else
							{
								memset(this->padList.basePtr, NULL, this->padList.maxLenth);
							}
						}
						msg.MsgId = MSG_USER_PAD_DELETE;
						isNotifyFather = true;
						break;
					}
					case NUMPAD_ID_CANCEL:   
					{		
						WM_SendMessageNoPara(hWin, MSG_USER_PAD_CANCEL);
						break;
					}
					case NUMPAD_ID_ENTER:   
					{
						WM_SendMessageNoPara(hWin, MSG_USER_PAD_OK);			
						break;
					}
					case NUMPAD_ID_CLEAR:		
					{
						this->putinNums = 0;
						ClearSeqList(&this->padList);
						memset(this->padList.basePtr, NULL, this->padList.maxLenth);
						msg.MsgId = MSG_USER_PAD_CLEAR;
						isNotifyFather = true;
						break;
					}
				}
				
				if (isAvailPutin)
				{
					if (this->putinNums < g_maxPutinNums) 
					{
						this->putinNums++;
						InsertSeqList(&this->padList, 
									  &c, 
									  GetSeqListLenth(&this->padList),
									  PushSeqListU8_CallBack,
									  CopySeqListU8_CallBack);
					}
					msg.MsgId = MSG_USER_PAD_CHANGED;
					isNotifyFather = true;
				}
				
				if (isNotifyFather)
				{
					msg.MsgId = MSG_USER_PAD_CHANGED;
					msg.Data.p = this->padList.basePtr;
					WM_SendToParent(hWin, &msg);	
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
* Function Name : _CreateNumPad
* Description	: 创建数字小键盘
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
WM_HWIN _CreateNumPad(WM_HWIN hParent, int x, int y) 
{	
	this = new(sizeof(APP_TYPEWRITE_TypeDef));
	INIT_LIST_HEAD(&this->handleHead);	
	
	int w, h, bx, by, bw, bh, dx, dy;
	/* Position and size */
	bx = NUMPAD_BORDER;
	by = NUMPAD_BORDER;
	bw = NUMPAD_BUTTON_WIDTH;
	bh = NUMPAD_BUTTON_HEIGHT;
	dx = NUMPAD_BUTTON_DIST_X;
	dy = NUMPAD_BUTTON_DIST_Y;
	w  = (bx * 2) + (bw * 11/2) + (dx * 5);
	h  = (by * 2) + (bh * 4)    + (dy * 3);
	x -= (w >> 1);
	y -= (h >> 1);
	
	/* Create NumPad window */
	WM_HWIN _hNumPad = WM_CreateWindowAsChild(x, 
												y, 
												w, 
												h, 
												hParent, 
												WM_CF_SHOW, 
												_cbNumPad, 
												0);
	
	/* Create buttons for NumPad */
	{
		LIST_HANDLE_TypeDef *listHandle = new(sizeof(LIST_HANDLE_TypeDef));
		listHandle->handleLevel1 = _CreateNumPadButton(_hNumPad, 
														"1", 
														NUMPAD_ID_1, 
														bx + 0*(bw+dx), 
														by + 0*(bh+dy), 
														bw, 
														bh, 
														NUMPAD_BUTTON_COLOR0, 
														NUMPAD_BUTTON_COLOR1, 
														1, 
														0);
		listHandle->cursorHandle = listHandle->handleLevel1;
		list_add_tail(&listHandle->list, &this->handleHead);
	}
	{
		LIST_HANDLE_TypeDef *listHandle = new(sizeof(LIST_HANDLE_TypeDef));
		listHandle->handleLevel1 = _CreateNumPadButton(_hNumPad, 
														"2", 
														NUMPAD_ID_2,
														bx + 1*(bw+dx), 
														by + 0*(bh+dy), 
														bw,
														bh, 
														NUMPAD_BUTTON_COLOR0,
														NUMPAD_BUTTON_COLOR1, 
														1, 
														0);
		listHandle->cursorHandle = listHandle->handleLevel1;
		list_add_tail(&listHandle->list, &this->handleHead);
	}	
	{
		LIST_HANDLE_TypeDef *listHandle = new(sizeof(LIST_HANDLE_TypeDef));
		listHandle->handleLevel1 = _CreateNumPadButton(_hNumPad, 
														"3", 
														NUMPAD_ID_3,  
														bx + 2*(bw+dx), 
														by + 0*(bh+dy), 
														bw, 
														bh, 
														NUMPAD_BUTTON_COLOR0, 
														NUMPAD_BUTTON_COLOR1,  
														1, 
														0);
		listHandle->cursorHandle = listHandle->handleLevel1;
		list_add_tail(&listHandle->list, &this->handleHead);
	}
	{
		LIST_HANDLE_TypeDef *listHandle = new(sizeof(LIST_HANDLE_TypeDef));
		listHandle->handleLevel1 = _CreateNumPadButton(_hNumPad, 
														"", 
														NUMPAD_ID_DELETE, 
														bx + 3*bw + 5*dx, 
														by + 0*(bh+dy), 
														bw*5/2, 
														bh, 
														NUMPAD_BUTTON_RED0, 
														NUMPAD_BUTTON_RED1,   
														1, 
														1);
		listHandle->cursorHandle = listHandle->handleLevel1;
		list_add_tail(&listHandle->list, &this->handleHead);
	}
	{
		LIST_HANDLE_TypeDef *listHandle = new(sizeof(LIST_HANDLE_TypeDef));
		listHandle->handleLevel1 = _CreateNumPadButton(_hNumPad, 
														"4", 
														NUMPAD_ID_4,
														bx + 0*(bw+dx), 
														by + 1*(bh+dy), 
														bw, 
														bh, 
														NUMPAD_BUTTON_COLOR0, 
														NUMPAD_BUTTON_COLOR1,
														1, 
														0);
		listHandle->cursorHandle = listHandle->handleLevel1;
		list_add_tail(&listHandle->list, &this->handleHead);
	}
	{
		LIST_HANDLE_TypeDef *listHandle = new(sizeof(LIST_HANDLE_TypeDef));
		listHandle->handleLevel1 = _CreateNumPadButton(_hNumPad, 
														"5", 
														NUMPAD_ID_5,
														bx + 1*(bw+dx), 
														by + 1*(bh+dy), 
														bw, 
														bh, 
														NUMPAD_BUTTON_COLOR0, 
														NUMPAD_BUTTON_COLOR1, 
														1, 
														0);
		listHandle->cursorHandle = listHandle->handleLevel1;
		list_add_tail(&listHandle->list, &this->handleHead);
	}
	{
		LIST_HANDLE_TypeDef *listHandle = new(sizeof(LIST_HANDLE_TypeDef));
		listHandle->handleLevel1 = _CreateNumPadButton(_hNumPad, 
														"6", 
														NUMPAD_ID_6, 
														bx + 2*(bw+dx), 
														by + 1*(bh+dy), 
														bw,
														bh, 
														NUMPAD_BUTTON_COLOR0,
														NUMPAD_BUTTON_COLOR1,
														1, 
														0);	
		listHandle->cursorHandle = listHandle->handleLevel1;
		list_add_tail(&listHandle->list, &this->handleHead);
	}
	{
		LIST_HANDLE_TypeDef *listHandle = new(sizeof(LIST_HANDLE_TypeDef));
		listHandle->handleLevel1 = _CreateNumPadButton(_hNumPad, 
														"",  
														NUMPAD_ID_CLEAR, 
														bx + 3*bw + 5*dx, 
														by + 1*(bh+dy), 
														bw*5/2, 
														bh, 
														NUMPAD_BUTTON_GREEN0,
														NUMPAD_BUTTON_GREEN1,  
														1, 
														2);
		listHandle->cursorHandle = listHandle->handleLevel1;
		list_add_tail(&listHandle->list, &this->handleHead);
	}
	{
		LIST_HANDLE_TypeDef *listHandle = new(sizeof(LIST_HANDLE_TypeDef));
		listHandle->handleLevel1 = _CreateNumPadButton(_hNumPad, 
														"7", 
														NUMPAD_ID_7, 
														bx + 0*(bw+dx), 
														by + 2*(bh+dy), 
														bw, 
														bh, 
														NUMPAD_BUTTON_COLOR0, 
														NUMPAD_BUTTON_COLOR1,  
														1, 
														0);
		listHandle->cursorHandle = listHandle->handleLevel1;
		list_add_tail(&listHandle->list, &this->handleHead);
	}
	{
		LIST_HANDLE_TypeDef *listHandle = new(sizeof(LIST_HANDLE_TypeDef));
		listHandle->handleLevel1 = _CreateNumPadButton(_hNumPad, 
														"8", 
														NUMPAD_ID_8,  
														bx + 1*(bw+dx), 
														by + 2*(bh+dy), 
														bw, 
														bh, 
														NUMPAD_BUTTON_COLOR0, 
														NUMPAD_BUTTON_COLOR1,  
														1, 
														0);
		listHandle->cursorHandle = listHandle->handleLevel1;
		list_add_tail(&listHandle->list, &this->handleHead);
	}
	{
		LIST_HANDLE_TypeDef *listHandle = new(sizeof(LIST_HANDLE_TypeDef));
		listHandle->handleLevel1 = _CreateNumPadButton(_hNumPad, 
														"9", 
														NUMPAD_ID_9, 
														bx + 2*(bw+dx),
														by + 2*(bh+dy), 
														bw,
														bh, 
														NUMPAD_BUTTON_COLOR0, 
														NUMPAD_BUTTON_COLOR1,  
														1, 
														0);
		listHandle->cursorHandle = listHandle->handleLevel1;
		list_add_tail(&listHandle->list, &this->handleHead);
	}
	{
		LIST_HANDLE_TypeDef *listHandle = new(sizeof(LIST_HANDLE_TypeDef));
		listHandle->handleLevel1 = _CreateNumPadButton(_hNumPad, 
														"",
														NUMPAD_ID_ENTER, 
														bx + 3*bw + 5*dx, 
														by + 2*(bh+dy), 
														bw*5/2, 
														bh, 
														NUMPAD_BUTTON_COLOR0,
														NUMPAD_BUTTON_COLOR1,  
														1, 
														3);
		listHandle->cursorHandle = listHandle->handleLevel1;
		list_add_tail(&listHandle->list, &this->handleHead);
	}
	{
		LIST_HANDLE_TypeDef *listHandle = new(sizeof(LIST_HANDLE_TypeDef));
		listHandle->handleLevel1 = _CreateNumPadButton(_hNumPad, 
														"-", 
														NUMPAD_ID_MINUS, 
														bx + 0*(bw+dx), 
														by + 3*(bh+dy), 
														bw,
														bh, 
														NUMPAD_BUTTON_COLOR0, 
														NUMPAD_BUTTON_COLOR1,  
														1,
														0) ;
		listHandle->cursorHandle = listHandle->handleLevel1;
		list_add_tail(&listHandle->list, &this->handleHead);
	}
	{
		LIST_HANDLE_TypeDef *listHandle = new(sizeof(LIST_HANDLE_TypeDef));
		listHandle->handleLevel1 = _CreateNumPadButton(_hNumPad, 
														"0",
														NUMPAD_ID_0, 
														bx + 1*(bw+dx), 
														by + 3*(bh+dy), 
														bw, 
														bh, 
														NUMPAD_BUTTON_COLOR0,
														NUMPAD_BUTTON_COLOR1, 
														1,
														0);
		listHandle->cursorHandle = listHandle->handleLevel1;
		list_add_tail(&listHandle->list, &this->handleHead);
	}
	{
		LIST_HANDLE_TypeDef *listHandle = new(sizeof(LIST_HANDLE_TypeDef));
		listHandle->handleLevel1 = _CreateNumPadButton(_hNumPad, 
														".", 
														NUMPAD_ID_POINT, 
														bx + 2*(bw+dx), 
														by + 3*(bh+dy), 
														bw, 
														bh, 
														NUMPAD_BUTTON_COLOR0,
														NUMPAD_BUTTON_COLOR1, 
														1, 
														0);
		listHandle->cursorHandle = listHandle->handleLevel1;
		list_add_tail(&listHandle->list, &this->handleHead);
	}
	{
		LIST_HANDLE_TypeDef *listHandle = new(sizeof(LIST_HANDLE_TypeDef));
		listHandle->handleLevel1 = _CreateNumPadButton(_hNumPad, 
														"",  
														NUMPAD_ID_CANCEL, 
														bx + 3*bw + 5*dx,
														by + 3*(bh+dy), 
														bw*5/2,
														bh, 
														NUMPAD_BUTTON_COLOR0,
														NUMPAD_BUTTON_COLOR1,  
														1, 
														4);
		listHandle->cursorHandle = listHandle->handleLevel1;
		list_add_tail(&listHandle->list, &this->handleHead);
	}
	
	WM_SendMessageNoPara(_hNumPad, WM_SET_FOCUS);
	
	return _hNumPad;
}




/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
