/*
*********************************************************************************************************
* @file    	emwin.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-19 09:33:30
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	单元测试源文件
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
#include "initial.h"
#include "emWinUtils.h"


/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/
#define TEST_TEXT							0x00U
#define TEST_PIC							0x00U
#define TEST_FONTS							0x00U
#define TEST_CURSOR							0x00U		//游标
#define TEST_ANTI_ALIASING					0x00U		//抗锯齿
#define TEST_MULTITASK						0x00U		//多任务
#define TEST_KEY_PID						0x00U		//PID设备按键
#define TEST_KEY							0x00U		//设备按键
#define TEST_BUTTON							0x00U		//按钮控件
#define TEST_DOWN_LIST						0x00U		//下拉列表
#define TEST_CHECK_BOX						0x00U		//复选框
#define TEST_EDIT							0x00U		//编辑框
#define TEST_LIST_BOX						0x00U		//列表框
#define TEST_MENU							0x00U		//菜单控件
#define TEST_MULTI_TEXT						0x00U		//多行文本
#define TEST_MULTI_PAGE						0x00U		//多页控件
#define TEST_PROGRESS_BAR					0x00U		//进度条
#define TEST_SINGLE_BUTTON					0x00U		//单选按钮
#define TEST_SCROLLBAR						0x00U		//滚动条
#define TEST_SLIDERBAR						0x00U		//滑块
#define TEST_TEXT_CONTROL					0x00U		//文本控件
#define TEST_SPINBOX						0x00U		//SPINBOX
#define TEST_LIST_VIEW						0x00U		//表格
#define TEST_MESSAGE_BOX					0x00U		//消息弹窗
#define TEST_GRAPHYT						0x00U		//图表

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
* Function Name : DisplayText
* Description	: 显示文本
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void DisplayText( void )
{
	GUI_RECT rect = {0,0,50,50};
	
	#if 0
		GUI_DispString("emWin");
	#endif
	#if 0
		GUI_DispStringAt("emWin",100,100);
	#endif
	#if 0
		GUI_SetFont(GUI_FONT_16_ASCII);
		GUI_DispStringInRectMax("STM32-emWin", &rect, GUI_TA_LEFT, 10);
	#endif
	
	#if 1
		GUI_SetTextMode(GUI_TM_TRANS);
		
		GUI_SetColor(GUI_GRAY);
		GUI_FillRectEx(&rect);
		
		GUI_SetColor(GUI_BLUE);
		GUI_SetFont(GUI_FONT_16_ASCII);
		GUI_DispStringInRectMax("STM32-emWin", &rect, GUI_TA_LEFT, 50);
	#endif
	
	#if 1
		GUI_WRAPMODE warp[] = {
			GUI_WRAPMODE_NONE,
			GUI_WRAPMODE_CHAR,
			GUI_WRAPMODE_WORD,
		};
	
		GUI_SetTextMode(GUI_TM_TRANS);	
		
		for (uint8_t i=0; i<3; ++i)
		{
			GUI_SetColor(GUI_GRAY);
			GUI_FillRectEx(&rect);
			
			GUI_SetColor(GUI_BLUE);
			GUI_SetFont(GUI_FONT_16_ASCII);

			GUI_DispStringInRectWrap("STM32-emWin", &rect, GUI_TA_LEFT, warp[i]);
			rect.x0 += 100;
			rect.x1 += 100;
		}
	#endif	
		
	#if 1
		GUI_SetFont(GUI_FONT_D80);
		GUI_SetColor(GUI_LIGHTBLUE);
		GUI_DispString("-123.980");
	#endif
}

/*
*********************************************************************************************************
* Function Name : DisplayPIC
* Description	: 显示图片
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void DisplayPIC( void )
{
//	emWinShowBMPEx(100,50,"0:/bmp/1.bmp");
//	if (emWinSaveBMPEx("0:/bmp/10.bmp") == true)
//	{
//		GUI_DispString("OK");
//	}
//	else
//	{
//		GUI_DispString("ERROR");
//	}
	
//	emWinShowJPEGEx(100,50,"0:/jpg/1.jpg");
	
//	ShowPNGEx(100,50,"0:/png/music.png");
}

#if (TEST_FONTS == 0x01U)
/*
*********************************************************************************************************
* Function Name : DisplayFonts
* Description	: 显示字体(必须将本文件转换为UTF-8格式，方可正常显示。)
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void MainTask( void )
{
	GUI_Delay(100);
	
	GUI_UC_SetEncodeUTF8();
	WM_SetCreateFlags(WM_CF_MEMDEV);
	GUI_SetFont(&g_XBF_Font);
	GUI_SetColor(GUI_GREEN);
	OpenXBF("0:/SONG_16X16.xbf");
	GUI_DispString("杭州鑫高科技有限公司");
}

extern GUI_CONST_STORAGE GUI_FONT GUI_FontYaHei_128X128;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontYaHeiB_128X128;

extern GUI_CONST_STORAGE GUI_FONT GUI_FontSongTi_128X128;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSongTiB_128X128;
/*
*********************************************************************************************************
*	函 数 名: MainTask
*	功能说明: GUI主任务
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
//void MainTask(void) 
//{      
//    GUI_UC_SetEncodeUTF8();
//	WM_SetCreateFlags(WM_CF_MEMDEV);
//	GUI_SetColor(GUI_GREEN);
//	
//	while(1)
//	{
//		uint32_t int_num = rand() % 10000;
//		uint32_t dot_num = rand() % 1000;
//		float num = ((int_num * 1000) + dot_num)/1000.0f;
//		char buff[20];
//		sprintf(buff, "%f", num);		
//		
////		GUI_SetFont(&GUI_FontSongTi_128X128);
////		GUI_DispStringAt(buff,0,0);
////		GUI_Delay(1);
//		
//		GUI_SetFont(&GUI_FontSongTiB_128X128);
//		GUI_DispStringAt(buff,0,200);
//		GUI_Delay(1);
//	}
//}
#endif

static const GUI_CURSOR* _apCursor[] = {
  &GUI_CursorArrowS,  &GUI_CursorArrowM,  &GUI_CursorArrowL,
  &GUI_CursorArrowLI, &GUI_CursorArrowMI, &GUI_CursorArrowSI,
  &GUI_CursorCrossS,  &GUI_CursorCrossM,  &GUI_CursorCrossL,
  &GUI_CursorCrossLI, &GUI_CursorCrossMI, &GUI_CursorCrossSI
};

/*
*********************************************************************************************************
* Function Name : DisplayCursor
* Description	: 显示光标
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void DisplayCursor( void )
{
	while (1)
	{
		GUI_CURSOR_SetPosition(100,100);
		for (uint8_t i=0; i<sizeof(_apCursor)/sizeof(_apCursor[0]); ++i)
		{
			GUI_CURSOR_Select(_apCursor[i]);
			GUI_CURSOR_Show();
			GUI_Delay(500);
		}
		
		GUI_CURSOR_Select(_apCursor[3]);
		for (uint16_t i=0; i<480; ++i)
		{
			for (uint16_t j=0; j<800; ++j)
			{
				GUI_CURSOR_SetPosition(j,i);
				GUI_CURSOR_Show();
				GUI_Delay(5);
			}
		}
		
		GUI_CURSOR_SetPosition(100,100);
		GUI_CURSOR_SelectAnim(&GUI_CursorAnimHourglassM);
		GUI_Delay(5000);
	}
}

/*
*********************************************************************************************************
* Function Name : DisplayAntiAliasing
* Description	: 显示抗锯齿
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void DisplayAntiAliasing( void )
{
	GUI_AA_FillCircle(200,100,50);
	GUI_AA_FillEllipse(500,100,50,20);
	GUI_AA_FillRoundedRect(300,200,500,300,20);
}

/*
*********************************************************************************************************
*	函 数 名: _cbBackgroundWin
*	功能说明: 背景窗口的回调函数		  			  
*	形    参：pMsg  消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbBackgroundWin(WM_MESSAGE* pMsg) 
{
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
			GUI_SetBkColor(GUI_LIGHTBLUE);
			GUI_Clear();
			GUI_SetFont(&GUI_Font24_ASCII);
			GUI_DispStringHCenterAt("i am bakcground!\n", 100, 100);
		
		default:
			WM_DefaultProc(pMsg);
	}
}

/*
*********************************************************************************************************
*	函 数 名: _cbCallbackT0
*	功能说明: 任务0的回调函数	  			  
*	形    参: pMsg  消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbCallbackT0(WM_MESSAGE * pMsg) 
{
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
			GUI_SetBkColor(GUI_RED);
			GUI_SetColor(GUI_BLACK);
			GUI_SetFont(&GUI_FontComic24B_ASCII);
			GUI_Clear();
			GUI_DispStringAt("Task0 Moving", 0, 0);
			break;
		
		default:
			WM_DefaultProc(pMsg);
	}
}

/*
*********************************************************************************************************
*	函 数 名: Task_0
*	功能说明: GUI任务0			  			  
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Task_0(void) 
{
	/* 创建框架窗口 */
	FRAMEWIN_Handle hFrameWin = FRAMEWIN_Create("Task 0",  NULL, WM_CF_SHOW | WM_CF_STAYONTOP,  0, 0, 200, 100);
	
	/* 创建子窗口 */
	WM_HWIN         hChildWin = WM_CreateWindowAsChild(0, 0, 0, 0, WM_GetClientWindow(hFrameWin), 
											       WM_CF_SHOW | WM_CF_MEMDEV,  _cbCallbackT0, 0);
	
	FRAMEWIN_SetActive(hFrameWin, 0);
	
	WM_SelectWindow(hChildWin);
	
	/* 设置字体 */
	GUI_SetFont(&GUI_FontComic24B_ASCII);
	
	while(1) 
	{
		int i;
		int nx = 80;
		int ny = 80;
	
		/* 移动窗口 */
		for (i = 0; i < nx; i++) 
		{
			WM_MoveWindow(hFrameWin, 3, 0);
			GUI_Delay(50);
		}
		
		/* 移动窗口 */
		for (i = 0; i < ny; i++) 
		{
			WM_MoveWindow(hFrameWin, 0, 3);
			GUI_Delay(50);
		}
		for (i = 0; i < nx; i++) 
		{
			WM_MoveWindow(hFrameWin, -3, 0);
			GUI_Delay(50);
		}
		for (i = 0; i < ny; i++) 
		{
			WM_MoveWindow(hFrameWin, 0, -3);
			GUI_Delay(50);
		}
	}	
}

/*
*********************************************************************************************************
* Function Name : TestMultiTask
* Description	: 测试多任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void TestMultiTask( void )
{
	WM_SetCreateFlags(WM_CF_MEMDEV);              
	WM_SetCallback(WM_HBKWIN, _cbBackgroundWin);
	
	/* 由于未使用操作系统，暂时测试1个窗口 */
	while(1)
	{
		Task_0();		
	}
}

/*
*********************************************************************************************************
* Function Name : CheckKeyPID
* Description	: 检测按键
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void CheckKeyPID( void )
{
	uint32_t key = ReadKeyValue(g_keyPtr);
	if (key != KEY_NONE)
	{	
		GUI_PID_STATE State;
		const uint16_t STEP_LENTH = 3;
		
		GUI_PID_GetState(&State);
		
		switch (ReadKeyStatus(g_keyPtr))
		{
			case STATUS_KEY_PRESS:
			case STATUS_KEY_LONG:
			case STATUS_KEY_HIGH_SPEED:	
			{		
				switch (key)
				{
					case KEY_UP:
						State.y -= STEP_LENTH;
						break;
					case KEY_DOWN:
						State.y += STEP_LENTH;
						break;
					case KEY_LEFT:
						State.x -= STEP_LENTH;
						break;
					case KEY_RIGHT:
						State.x += STEP_LENTH;
						break;
					default:						
						break;
				}			
				
				if (State.x < 0)
				{
					State.x = 0;
				}
				if (State.y < 0)
				{
					State.y = 0;
				}
				
				uint16_t maxX = LCD_GetXSize() - 10;
				if (State.x > maxX)
				{
					State.x = maxX;
				}
				uint16_t maxY = LCD_GetYSize() - 20;
				if (State.y > maxY)
				{
					State.y = maxY;
				}
				State.Pressed = 1;
				GUI_PID_StoreState(&State);				
			}
			case STATUS_KEY_LEAVE:
			{
				printf("按键松手\r\n");
				break;
			}
			default:
			{		
				printf("按键状态错误\r\n");
				break;
			}
		}			
		ReleaseKeyMsg(g_keyPtr);
	}
}

/*
*********************************************************************************************************
* Function Name : TestKeyPID
* Description	: 测试按键模拟PID设备
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void TestKeyPID( void )
{
	GUI_CURSOR_Show();
	
	while (1)
	{		
		GUI_Delay(10);
	}
}

/*
*********************************************************************************************************
* Function Name : CheckKey
* Description	: 检测按键
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void CheckKey( void )
{
	uint32_t key = ReadKeyValue(g_keyPtr);
	if (key != KEY_NONE)
	{		
		switch (ReadKeyStatus(g_keyPtr))
		{
			case STATUS_KEY_PRESS:
			case STATUS_KEY_LONG:
			case STATUS_KEY_HIGH_SPEED:	
			{		
				switch ( key )		
				{				
					case KEY_ENTER:
						GUI_SendKeyMsg(GUI_KEY_ENTER, 1);
						break;
					case KEY_ESC:
						GUI_SendKeyMsg(GUI_KEY_ESCAPE, 1);
						break;
					case KEY_BACK:
						GUI_SendKeyMsg(GUI_KEY_BACKSPACE, 1);
						break;
					case KEY_LEFT:
						GUI_SendKeyMsg(GUI_KEY_LEFT, 1);
						break;
					case KEY_RIGHT:
						GUI_SendKeyMsg(GUI_KEY_RIGHT, 1);
						break;
					case KEY_UP:
						GUI_SendKeyMsg(GUI_KEY_UP, 1);
						break;
					case KEY_DOWN:
						GUI_SendKeyMsg(GUI_KEY_DOWN, 1);
						break;		
					case KEY_1:
						GUI_SendKeyMsg(GUI_KEY_BACKTAB, 1);
						break;
					case KEY_2:
						GUI_SendKeyMsg(GUI_KEY_HOME, 1);
						break;
					case KEY_3:
						GUI_SendKeyMsg(GUI_KEY_END, 1);
						break;
					case KEY_4:
						GUI_SendKeyMsg(GUI_KEY_SHIFT, 1);
						break;
					case KEY_5:
						GUI_SendKeyMsg(GUI_KEY_CONTROL, 1);
						break;
					case KEY_6:
						GUI_SendKeyMsg(GUI_KEY_INSERT, 1);
						break;
					case KEY_7:
						GUI_SendKeyMsg(GUI_KEY_DELETE, 1);
						break;
					case KEY_8:
						GUI_SendKeyMsg(GUI_KEY_SPACE, 1);
						break;
					case KEY_9:
						GUI_SendKeyMsg(GUI_KEY_PGUP, 1);
						break;
					case KEY_0:
						GUI_SendKeyMsg(GUI_KEY_PGDOWN, 1);
						break;
					case KEY_F1:
						GUI_SendKeyMsg(GUI_KEY_F1, 1);
						break;
					case KEY_F2:
						GUI_SendKeyMsg(GUI_KEY_F2, 1);
						break;
					case KEY_F4:
						GUI_SendKeyMsg(GUI_KEY_TAB, 1);
						break;
				}
			}
			case STATUS_KEY_LEAVE:
			{
				printf("按键松手\r\n");
				break;
			}
			default:
			{		
				printf("按键状态错误\r\n");
				break;
			}
		}			
		ReleaseKeyMsg(g_keyPtr);
	}
}

#if (TEST_KEY == 0x01U)

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
	{ FRAMEWIN_CreateIndirect, "Edit ", 0,         90,  90, 140, 130, FRAMEWIN_CF_MOVEABLE},
	{ EDIT_CreateIndirect,     "name:",     GUI_ID_EDIT0,    10,  10, 100,  20, 0, 15},
	{ EDIT_CreateIndirect,     "sex:",     GUI_ID_EDIT1,    10,  40, 100,  20, 0, 15},
	{ BUTTON_CreateIndirect,   "Ok",     GUI_ID_OK,       10,  80,  50,  20 },
	{ BUTTON_CreateIndirect,   "Cancel", GUI_ID_CANCEL,   70,  80,  50,  20 },
};

static char * _apExplain[] = 
{
	"This sample shows how to use edit widgets with a",
	"user defined callback function and how to set a",
	"user defined AddKey function. It selects the",
	"contents of the edit field on receiving the focus",
	"and overwrites the contents if a key other than",
	"a cursor key is pressed.",
};

#define GUI_KEY_ADD  (GUI_ID_USER + 1)


/*
*********************************************************************************************************
*	函 数 名: _cbDialog
*	功能说明: 对话框的回调函数
*	形    参: pMsg 消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbDialog(WM_MESSAGE * pMsg) 
{
	int     i;
	int     NCode;
	int     Id;
	WM_HWIN hDlg;
	WM_HWIN hItem;

	hDlg = pMsg->hWin;
	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			FRAMEWIN_SetFont(pMsg->hWin, &GUI_Font13_ASCII);
			FRAMEWIN_SetTextAlign(pMsg->hWin, GUI_TA_HCENTER);
			for (i = 0; i < 2; i++) 
			{
				hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT0 + i);  // Get the handle of the edit widget
				EDIT_AddKey(hItem, GUI_KEY_ADD);
				EDIT_SetText(hItem, "Hello world!");             // Fill widget with text
				EDIT_SetSel(hItem, 0, -1);                       // Select the whole contents of the edit field
			}
			break;
			
		 case WM_KEY:
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
            { 
				case GUI_KEY_ESCAPE:
                    GUI_EndDialog(hDlg, 1);
                    break;
				
                case GUI_KEY_ENTER:
                    GUI_EndDialog(hDlg, 0);
                    break;
            }
            break;
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);      // Id of widget
			NCode = pMsg->Data.v;                 // Notification code
			switch (NCode) 
			{
				case WM_NOTIFICATION_RELEASED:      // React only if released
					if (Id == GUI_ID_OK) 
					{         // OK Button
						GUI_EndDialog(hDlg, 0);
					}
					if (Id == GUI_ID_CANCEL) 
					{        // Cancel Button
						GUI_EndDialog(hDlg, 1);
					}				
					break;
			}
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}

/*
*********************************************************************************************************
*	函 数 名: _cbDesktop
*	功能说明: 桌面窗口的回调函数
*	形    参: pMsg 消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbDesktop(WM_MESSAGE * pMsg) 
{
	unsigned i;

	switch (pMsg->MsgId) 
	{
		/* 重绘 */
		case WM_PAINT:
			GUI_SetBkColor(GUI_RED);
			GUI_Clear();
			GUI_SetFont(&GUI_Font24_ASCII);
			GUI_DispStringHCenterAt("table", 160, 5);
			GUI_DispNextLine();
			GUI_SetFont(GUI_DEFAULT_FONT);
			GUI_DispNextLine();
			for (i = 0; i < GUI_COUNTOF(_apExplain); i++) 
			{
				GUI_DispStringHCenterAt(_apExplain[i], 160, GUI_GetDispPosY());
				GUI_DispNextLine();
			}
			break;
	}
}

/*
*********************************************************************************************************
* Function Name : TestKey
* Description	: 测试按键设备
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void TestKey( void )
{
	WM_SetCallback(WM_HBKWIN, _cbDesktop);
	
	while (1)
	{		
		/* 创建阻塞式对话框 */
		GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbDialog, 0, 0, 0);
		
		GUI_Delay(10);
	}
}

#endif

#if (TEST_BUTTON == 0x01U)
/*
*********************************************************************************************************
*	                                    变量    
*********************************************************************************************************
*/
GUI_COLOR _acColor[3] = {GUI_BLUE,GUI_RED,GUI_YELLOW};                                              
static int BackColor;                                                                   

/*
*********************************************************************************************************
*	                          GUI_WIDGET_CREATE_INFO类型数组 
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { FRAMEWIN_CreateIndirect,  "armfly",            0,                       0,  0,  480,272,FRAMEWIN_CF_MOVEABLE,0},
    { BUTTON_CreateIndirect,    "BUTTON0",           GUI_ID_BUTTON0,          52, 93, 110,30, 0,0},
    { BUTTON_CreateIndirect,    "BUTTON1",           GUI_ID_BUTTON1,          289,90, 117,33, 0,0}
};

/*
*********************************************************************************************************
*	函 数 名: PaintDialog
*	功能说明: 对话框回调函数重绘消息的执行函数
*	形    参: pMsg  消息
*	返 回 值: 无
*********************************************************************************************************
*/
void PaintDialog(WM_MESSAGE * pMsg)                                                               
{
//    WM_HWIN hWin = pMsg->hWin;
	GUI_SetBkColor(_acColor[BackColor]);                                                       
	GUI_Clear();
}

/*
*********************************************************************************************************
*	函 数 名: InitDialog
*	功能说明: 对话框回调函数初始化消息的执行函数
*	形    参: pMsg  消息
*	返 回 值: 无
*********************************************************************************************************
*/
void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    
	//
    //FRAMEWIN
    //
    FRAMEWIN_SetFont(hWin,&GUI_Font24B_ASCII);
    FRAMEWIN_SetTextAlign(hWin,GUI_TA_VCENTER|GUI_TA_CENTER);
    FRAMEWIN_AddCloseButton(hWin, FRAMEWIN_BUTTON_RIGHT, 0);
    FRAMEWIN_AddMaxButton(hWin, FRAMEWIN_BUTTON_RIGHT, 1);
    FRAMEWIN_AddMinButton(hWin, FRAMEWIN_BUTTON_RIGHT, 2);
    FRAMEWIN_SetTitleHeight(hWin,30);
    
	//
    //GUI_ID_BUTTON0
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON0),&GUI_Font24B_ASCII);                      
    
	//
    //GUI_ID_BUTTON1
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON1),&GUI_Font24_1);

	/* 默认颜色取*/
	BackColor = 0;  
}

/*
*********************************************************************************************************
*	函 数 名: _cbCallback
*	功能说明: 对话框回调函数
*	形    参: pMsg  消息
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbCallback(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            PaintDialog(pMsg);
            break;
        case WM_INIT_DIALOG:
            InitDialog(pMsg);
            break;
        case WM_KEY:
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
            {
                case GUI_KEY_ESCAPE:
                    GUI_EndDialog(hWin, 1);
                    break;
                case GUI_KEY_ENTER:
                    GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (Id) 
            {
                case GUI_ID_OK:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
                case GUI_ID_CANCEL:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
                case GUI_ID_BUTTON0:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:                                            
                            BackColor++;
							if (BackColor == 3)
							{
								BackColor = 0;
							}
							WM_InvalidateWindow(hWin);
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            
                            break;
                        case WM_NOTIFICATION_MOVED_OUT:
                            
                            break;
                    }
                    break;
                case GUI_ID_BUTTON1:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:                                              
                            BackColor--;
							if (BackColor < 0)
							{
								BackColor = 2;
							}
							WM_InvalidateWindow(hWin);
                            break;
							
                        case WM_NOTIFICATION_RELEASED:
                            break;
						
                        case WM_NOTIFICATION_MOVED_OUT:
                            break;
                    }
                    break;
            }
            break;
        default:
            WM_DefaultProc(pMsg);
    }
}

/*
*********************************************************************************************************
* Function Name : TestButton
* Description	: 测试按钮
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void TestButton( void )
{
	WM_SetDesktopColor(GUI_WHITE);      
    WM_SetCreateFlags(WM_CF_MEMDEV);   
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	
	while (1)
	{
		GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
		
		GUI_Delay(10);
	}
}
#endif

#if (TEST_DOWN_LIST == 0x01U)
/*
*********************************************************************************************************
*	                          GUI_WIDGET_CREATE_INFO类型数组 
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { FRAMEWIN_CreateIndirect,  "emWin",            0,                       0,  0,  480,272,FRAMEWIN_CF_MOVEABLE,0},
    { DROPDOWN_CreateIndirect,   NULL,               GUI_ID_DROPDOWN0,        129,103,163,80, 0,0}
};

/*
*********************************************************************************************************
*	函 数 名: PaintDialog
*	功能说明: 对话框回调函数重绘消息的执行函数
*	形    参: pMsg  消息
*	返 回 值: 无
*********************************************************************************************************
*/
void PaintDialog(WM_MESSAGE * pMsg)
{
//    WM_HWIN hWin = pMsg->hWin;

}

/*
*********************************************************************************************************
*	函 数 名: InitDialog
*	功能说明: 对话框回调函数初始化消息的执行函数
*	形    参: pMsg  消息
*	返 回 值: 无
*********************************************************************************************************
*/
void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    
	//
    //FRAMEWIN
    //
    FRAMEWIN_SetFont(hWin,&GUI_Font32B_ASCII);
    FRAMEWIN_AddCloseButton(hWin, FRAMEWIN_BUTTON_RIGHT, 0);
    FRAMEWIN_AddMaxButton(hWin, FRAMEWIN_BUTTON_RIGHT, 1);
    FRAMEWIN_AddMinButton(hWin, FRAMEWIN_BUTTON_RIGHT, 2);
    FRAMEWIN_SetTitleHeight(hWin,35);
    //
    //GUI_ID_DROPDOWN0
    //
    DROPDOWN_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN0),0,0xffffff);
    DROPDOWN_SetFont(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN0),&GUI_Font24B_ASCII);
    DROPDOWN_SetAutoScroll(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN0),1);
    DROPDOWN_AddString(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN0),"Item0");
    DROPDOWN_AddString(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN0),"Item1");
    DROPDOWN_AddString(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN0),"Item2");
    DROPDOWN_AddString(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN0),"Item3");
    DROPDOWN_AddString(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN0),"Item4");
}

/*
*********************************************************************************************************
*	函 数 名: _cbCallback
*	功能说明: 对话框回调函数
*	形    参: pMsg  消息
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbCallback(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            PaintDialog(pMsg);
            break;
        case WM_INIT_DIALOG:
            InitDialog(pMsg);
            break;
        case WM_KEY:
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
            {
                case GUI_KEY_ESCAPE:
                    GUI_EndDialog(hWin, 1);
                    break;
                case GUI_KEY_ENTER:
                    GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (Id) 
            {
                case GUI_ID_OK:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
                case GUI_ID_CANCEL:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
				case GUI_ID_DROPDOWN0:
					if(NCode==WM_NOTIFICATION_CLICKED)
					{
						NCode = NCode;
					}
					break;
            }
            break;
        default:
            WM_DefaultProc(pMsg);
    }
}

/*
*********************************************************************************************************
* Function Name : TestDownList
* Description	: 测试下拉列表
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void TestDownList( void )
{
	WM_SetDesktopColor(GUI_WHITE);      
    WM_SetCreateFlags(WM_CF_MEMDEV);   
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	while (1)
	{
		GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);   
		GUI_Delay(10);
	}
}
#endif

#if (TEST_CHECK_BOX == 0x01U)
/*
*********************************************************************************************************
*	                          GUI_WIDGET_CREATE_INFO类型数组 
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { FRAMEWIN_CreateIndirect,  "Check box",          0,                       0,  0,  480,272,FRAMEWIN_CF_MOVEABLE,0},
    { CHECKBOX_CreateIndirect,   NULL,        	GUI_ID_CHECK0,        116,57, 150, 15, 0,0}
};

/*
*********************************************************************************************************
*	函 数 名: PaintDialog
*	功能说明: 对话框回调函数重绘消息的执行函数
*	形    参: pMsg  消息
*	返 回 值: 无
*********************************************************************************************************
*/
void PaintDialog(WM_MESSAGE * pMsg)
{
//    WM_HWIN hWin = pMsg->hWin;

}

/*
*********************************************************************************************************
*	函 数 名: InitDialog
*	功能说明: 对话框回调函数初始化消息的执行函数
*	形    参: pMsg  消息
*	返 回 值: 无
*********************************************************************************************************
*/
void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    
	//
    //FRAMEWIN
    //
    FRAMEWIN_SetFont(hWin,&GUI_Font32B_ASCII);
    FRAMEWIN_AddCloseButton(hWin, FRAMEWIN_BUTTON_RIGHT, 0);
    FRAMEWIN_AddMaxButton(hWin, FRAMEWIN_BUTTON_RIGHT, 1);
    FRAMEWIN_AddMinButton(hWin, FRAMEWIN_BUTTON_RIGHT, 2);
    FRAMEWIN_SetTitleHeight(hWin,35);

    CHECKBOX_SetText(WM_GetDialogItem(hWin,GUI_ID_CHECK0),"CHECKBOX");
}

/*
*********************************************************************************************************
*	函 数 名: _cbCallback
*	功能说明: 对话框回调函数
*	形    参: pMsg  消息
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbCallback(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            PaintDialog(pMsg);
            break;
        case WM_INIT_DIALOG:
            InitDialog(pMsg);
            break;
        case WM_KEY:
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
            {
                case GUI_KEY_ESCAPE:
                    GUI_EndDialog(hWin, 1);
                    break;
                case GUI_KEY_ENTER:
                    GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (Id) 
            {
                case GUI_ID_OK:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
                case GUI_ID_CANCEL:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
        default:
            WM_DefaultProc(pMsg);
    }
}

/*
*********************************************************************************************************
* Function Name : TestCheckBox
* Description	: 测试复选框
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void TestCheckBox( void )
{
	WM_SetDesktopColor(GUI_WHITE);      
    WM_SetCreateFlags(WM_CF_MEMDEV);   
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	while (1)
	{
		GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);   
		GUI_Delay(10);
	}
}
#endif

#if (TEST_EDIT == 0x01U)
/*
*********************************************************************************************************
*	                          GUI_WIDGET_CREATE_INFO类型数组 
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { FRAMEWIN_CreateIndirect,  "armfly",            0,                       0,  0,  480,272,FRAMEWIN_CF_MOVEABLE,0},
    { EDIT_CreateIndirect,      "armfly",            GUI_ID_EDIT0,            239,36, 100,21, 0,0},
    { TEXT_CreateIndirect,      "TextMode",          GUI_ID_TEXT0,            81, 36, 106,24, 0,0},
    { TEXT_CreateIndirect,      "FloatMode",         GUI_ID_TEXT4,            81, 199,118,24, 0,0},
    { TEXT_CreateIndirect,      "DecMode",           GUI_ID_TEXT3,            81, 158,94, 24, 0,0},
    { TEXT_CreateIndirect,      "BinMode",           GUI_ID_TEXT2,            81, 117,94, 24, 0,0},
    { TEXT_CreateIndirect,      "HexMode",           GUI_ID_TEXT1,            81, 73, 94, 24, 0,0},
    { EDIT_CreateIndirect,      "FFFF",              GUI_ID_EDIT1,            239,73, 100,21, 0,0},
    { EDIT_CreateIndirect,      "11110000",          GUI_ID_EDIT2,            239,120,100,21, 0,0},
    { EDIT_CreateIndirect,      "128",               GUI_ID_EDIT3,            239,163,100,21, 0,0},
    { EDIT_CreateIndirect,      "12.123456",         GUI_ID_EDIT4,            239,199,100,21, 0,0}
};

/*
*********************************************************************************************************
*	函 数 名: PaintDialog
*	功能说明: 对话框回调函数重绘消息的执行函数
*	形    参: pMsg  消息
*	返 回 值: 无
*********************************************************************************************************
*/
void PaintDialog(WM_MESSAGE * pMsg)
{
//    WM_HWIN hWin = pMsg->hWin;

}

/*
*********************************************************************************************************
*	函 数 名: InitDialog
*	功能说明: 对话框回调函数初始化消息的执行函数
*	形    参: pMsg  消息
*	返 回 值: 无
*********************************************************************************************************
*/
void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    //
    //FRAMEWIN
    //
    FRAMEWIN_SetFont(hWin,&GUI_Font32B_ASCII);
    FRAMEWIN_AddCloseButton(hWin, FRAMEWIN_BUTTON_RIGHT, 0);
    FRAMEWIN_AddMaxButton(hWin, FRAMEWIN_BUTTON_RIGHT, 1);
    FRAMEWIN_AddMinButton(hWin, FRAMEWIN_BUTTON_RIGHT, 2);
    FRAMEWIN_SetTitleHeight(hWin,35);
	
	 //
    //GUI_ID_TEXT0
    //
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT0),&GUI_Font24B_ASCII);
    //
    //GUI_ID_TEXT4
    //
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT4),&GUI_Font24B_ASCII);
    //
    //GUI_ID_TEXT3
    //
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT3),&GUI_Font24B_ASCII);
    //
    //GUI_ID_TEXT2
    //
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT2),&GUI_Font24B_ASCII);
    //
    //GUI_ID_TEXT1
    //
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT1),&GUI_Font24B_ASCII);  
	
    //
    //GUI_ID_EDIT0
    //
    EDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_EDIT0),&GUI_Font16B_ASCII);
    EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT0),"armfly");                  
    //
    //GUI_ID_EDIT1
    //
    EDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_EDIT1),&GUI_Font16B_ASCII);
    EDIT_SetHexMode(WM_GetDialogItem(hWin,GUI_ID_EDIT1),65535,0,255);                               
    //
    //GUI_ID_EDIT2
    //
    EDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_EDIT2),&GUI_Font16B_ASCII);
    EDIT_SetBinMode(WM_GetDialogItem(hWin,GUI_ID_EDIT2),240,0,255);                                
    //
    //GUI_ID_EDIT3
    //
    EDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_EDIT3),&GUI_Font16B_ASCII);
    EDIT_SetDecMode(WM_GetDialogItem(hWin,GUI_ID_EDIT3),128,0,255,0,0);                               
    //
    //GUI_ID_EDIT4
    //
    EDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_EDIT4),&GUI_Font16B_ASCII);
    EDIT_SetFloatMode(WM_GetDialogItem(hWin,GUI_ID_EDIT4),12.12346,0,255,6,0);                       
}

/*
*********************************************************************************************************
*	函 数 名: _cbCallback
*	功能说明: 对话框回调函数
*	形    参: pMsg  消息
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbCallback(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            PaintDialog(pMsg);
            break;
		
        case WM_INIT_DIALOG:
            InitDialog(pMsg);
            break;
		
        case WM_KEY:
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
            {
                case GUI_KEY_ESCAPE:
                    GUI_EndDialog(hWin, 1);
                    break;
                case GUI_KEY_ENTER:
                    GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
			
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (Id) 
            {
                case GUI_ID_OK:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
                case GUI_ID_CANCEL:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;

            }
            break;
			
        default:
            WM_DefaultProc(pMsg);
    }
}

/*
*********************************************************************************************************
* Function Name : TestEdit
* Description	: 测试编辑框
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void TestEdit( void )
{
	WM_SetDesktopColor(GUI_WHITE);      
    WM_SetCreateFlags(WM_CF_MEMDEV);   
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	while (1)
	{
		GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);   
		GUI_Delay(10);
	}
}
#endif

#if (TEST_LIST_BOX == 0x01U)
/*
*********************************************************************************************************
*	                          GUI_WIDGET_CREATE_INFO类型数组 
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { FRAMEWIN_CreateIndirect,  "armfly",            0,                       0,  0,  480,272,FRAMEWIN_CF_MOVEABLE,0},
    { LISTBOX_CreateIndirect,    NULL,               GUI_ID_LISTBOX0,         165,52, 121,110,0,0}
};

/*
*********************************************************************************************************
*	函 数 名: PaintDialog
*	功能说明: 对话框回调函数重绘消息的执行函数
*	形    参: pMsg  消息
*	返 回 值: 无
*********************************************************************************************************
*/
void PaintDialog(WM_MESSAGE * pMsg)                                                               
{
//    WM_HWIN hWin = pMsg->hWin;

}

/*
*********************************************************************************************************
*	函 数 名: InitDialog
*	功能说明: 对话框回调函数初始化消息的执行函数
*	形    参: pMsg  消息
*	返 回 值: 无
*********************************************************************************************************
*/
void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
	
    //
    //FRAMEWIN
    //
    FRAMEWIN_SetFont(hWin,&GUI_Font32B_ASCII);
    FRAMEWIN_AddCloseButton(hWin, FRAMEWIN_BUTTON_RIGHT, 0);
    FRAMEWIN_AddMaxButton(hWin, FRAMEWIN_BUTTON_RIGHT, 1);
    FRAMEWIN_AddMinButton(hWin, FRAMEWIN_BUTTON_RIGHT, 2);
    FRAMEWIN_SetTitleHeight(hWin,35);
    //
    //GUI_ID_LISTBOX0
    //
    LISTBOX_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_LISTBOX0),0,0xffffff);
    LISTBOX_SetFont(WM_GetDialogItem(hWin,GUI_ID_LISTBOX0),&GUI_Font24B_ASCII);
    LISTBOX_SetAutoScrollH(WM_GetDialogItem(hWin,GUI_ID_LISTBOX0),1);
    LISTBOX_SetAutoScrollV(WM_GetDialogItem(hWin,GUI_ID_LISTBOX0),1);
    LISTBOX_AddString(WM_GetDialogItem(hWin,GUI_ID_LISTBOX0),"STM32-V5");
    LISTBOX_AddString(WM_GetDialogItem(hWin,GUI_ID_LISTBOX0),"STM32-X3");
    LISTBOX_AddString(WM_GetDialogItem(hWin,GUI_ID_LISTBOX0),"STM32-V3");
    LISTBOX_AddString(WM_GetDialogItem(hWin,GUI_ID_LISTBOX0),"STM32-V2");
    LISTBOX_AddString(WM_GetDialogItem(hWin,GUI_ID_LISTBOX0),"STM32-X2");
}

/*
*********************************************************************************************************
*	函 数 名: _cbCallback
*	功能说明: 对话框回调函数
*	形    参: pMsg  消息
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbCallback(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            PaintDialog(pMsg);
            break;
        case WM_INIT_DIALOG:
            InitDialog(pMsg);
            break;
        case WM_KEY:
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
            {
                case GUI_KEY_ESCAPE:
                    GUI_EndDialog(hWin, 1);
                    break;
                case GUI_KEY_ENTER:
                    GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (Id) 
            {
                case GUI_ID_OK:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
                case GUI_ID_CANCEL:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;

            }
            break;
        default:
            WM_DefaultProc(pMsg);
    }
}

/*
*********************************************************************************************************
* Function Name : TestListBox
* Description	: 列表框
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void TestListBox(void) 
{ 
    WM_SetDesktopColor(GUI_WHITE);      
    WM_SetCreateFlags(WM_CF_MEMDEV);   
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	
	while (1)
	{
		GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);           
	}
}

#endif

#if (TEST_MENU == 0x01U)
/*
*********************************************************************************************************
*	                                    宏定义     
*********************************************************************************************************
*/
#define ID_FRAMEWIN_0  (GUI_ID_USER + 0x00)
#define ID_MENU_0  (GUI_ID_USER + 0x01)


/*
*********************************************************************************************************
*	                                GUI_WIDGET_CREATE_INFO 类型数组    
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 480, 272, 0, 0x64, 0 },
  { MENU_CreateIndirect, "Menu", ID_MENU_0, 133, 51, 187, 38, 0, 0x0, 0 },
};

/*
*********************************************************************************************************
*	函 数 名: _cbDialog
*	功能说明: 回调函数
*	形    参: pMsg 消息
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbDialog(WM_MESSAGE * pMsg) 
{
	MENU_ITEM_DATA   ItemData;
	WM_HWIN          hItem;
	int              NCode;
	int              Id;

	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			//
			// 初始化 'Framewin'
			//
			hItem = pMsg->hWin;
			FRAMEWIN_SetFont(hItem, GUI_FONT_32B_ASCII);
			FRAMEWIN_SetText(hItem, "armfly");

			//
			// 初始化 'Menu'
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_MENU_0);
			ItemData.Flags    = 0;
			ItemData.hSubmenu = 0;
			ItemData.Id       = 0;
			ItemData.pText    = "STM32-V5";
			MENU_AddItem(hItem, &ItemData);
			ItemData.Flags    = 0;
			ItemData.hSubmenu = 0;
			ItemData.Id       = 1;
			ItemData.pText    = "STM32-X3";
			MENU_AddItem(hItem, &ItemData);
			ItemData.Flags    = 0;
			ItemData.hSubmenu = 0;
			ItemData.Id       = 0;
			ItemData.pText    = "STM32-V3";
			MENU_AddItem(hItem, &ItemData);
			break;
			
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) 
			{
				case ID_MENU_0: 
				switch(NCode) 
				{
					case WM_NOTIFICATION_CLICKED:
						break;
					
					case WM_NOTIFICATION_RELEASED:
						break;
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
			}
			break;
			
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: CreateFramewin
*	功能说明: 创建对话框
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
WM_HWIN CreateFramewin(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

/*
*********************************************************************************************************
*	函 数 名: TestMenu
*	功能说明: 测试菜单
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void TestMenu(void) 
{ 
    WM_SetDesktopColor(GUI_WHITE);     
    WM_SetCreateFlags(WM_CF_MEMDEV);  
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
    CreateFramewin();
	while (1)
	{
		GUI_Delay(100);
	}
}
#endif

#if (TEST_MULTI_TEXT == 0x01U)
/*
*********************************************************************************************************
*	                          GUI_WIDGET_CREATE_INFO类型数组 
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { FRAMEWIN_CreateIndirect,  "armfly",            0,                       0,  0,  480,272,FRAMEWIN_CF_MOVEABLE,0},
    { MULTIEDIT_CreateIndirect, "MULTIEDIT",         GUI_ID_MULTIEDIT0,       161,43, 125,137,0,0}
};

/*
*********************************************************************************************************
*	函 数 名: PaintDialog
*	功能说明: 对话框回调函数重绘消息的执行函数
*	形    参: pMsg  消息
*	返 回 值: 无
*********************************************************************************************************
*/
void PaintDialog(WM_MESSAGE * pMsg)                                                               
{
//    WM_HWIN hWin = pMsg->hWin;

}

/*
*********************************************************************************************************
*	函 数 名: InitDialog
*	功能说明: 对话框回调函数初始化消息的执行函数
*	形    参: pMsg  消息
*	返 回 值: 无
*********************************************************************************************************
*/
void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    //
    //FRAMEWIN
    //
    FRAMEWIN_SetFont(hWin,&GUI_Font32B_ASCII);
    FRAMEWIN_SetTextAlign(hWin,GUI_TA_VCENTER|GUI_TA_CENTER);
    FRAMEWIN_AddCloseButton(hWin, FRAMEWIN_BUTTON_RIGHT, 0);
    FRAMEWIN_AddMaxButton(hWin, FRAMEWIN_BUTTON_RIGHT, 1);
    FRAMEWIN_AddMinButton(hWin, FRAMEWIN_BUTTON_RIGHT, 2);
    FRAMEWIN_SetTitleHeight(hWin,30);
    //
    //GUI_ID_MULTIEDIT0
    //
    MULTIEDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_MULTIEDIT0),&GUI_Font24B_ASCII);
    MULTIEDIT_SetAutoScrollH(WM_GetDialogItem(hWin,GUI_ID_MULTIEDIT0),1);
    MULTIEDIT_SetAutoScrollV(WM_GetDialogItem(hWin,GUI_ID_MULTIEDIT0),1);
    MULTIEDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_MULTIEDIT0),"STM32-V5\nSTM32-X3\nSTM32-V3\nSTM32-V2\nSTM32-X2");
}

/*
*********************************************************************************************************
*	函 数 名: _cbCallback
*	功能说明: 对话框回调函数
*	形    参: pMsg  消息
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbCallback(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            PaintDialog(pMsg);
            break;
        case WM_INIT_DIALOG:
            InitDialog(pMsg);
            break;
        case WM_KEY:
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
            {
                case GUI_KEY_ESCAPE:
                    GUI_EndDialog(hWin, 1);
                    break;
                case GUI_KEY_ENTER:
                    GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (Id) 
            {
                case GUI_ID_OK:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
                case GUI_ID_CANCEL:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;

            }
            break;
        default:
            WM_DefaultProc(pMsg);
    }
}

/*
*********************************************************************************************************
*	函 数 名: TestMultiText
*	功能说明: 多行文本
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void TestMultiText(void) 
{ 
    WM_SetDesktopColor(GUI_WHITE);      
    WM_SetCreateFlags(WM_CF_MEMDEV);   
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
    GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);           
}	
#endif

#if (TEST_MULTI_PAGE == 0x01U)
/*
*********************************************************************************************************
*	                                    宏定义     
*********************************************************************************************************
*/
#define ID_FRAMEWIN_0  (GUI_ID_USER + 0x00)
#define ID_MULTIPAGE_0  (GUI_ID_USER + 0x01)
#define ID_BUTTON_0  (GUI_ID_USER + 0x02)

/*
*********************************************************************************************************
*	                                GUI_WIDGET_CREATE_INFO 类型数组    
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 480, 272, FRAMEWIN_CF_ACTIVE|FRAMEWIN_CF_MOVEABLE, 0x64, 0 },
  { MULTIPAGE_CreateIndirect, "Multipage", ID_MULTIPAGE_0, 60, 8, 339, 215, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 154, 115, 80, 20, 0, 0x0, 0 },
};

/*
*********************************************************************************************************
*	函 数 名: _cbDialog
*	功能说明: 回调函数
*	形    参：pMsg 消息
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbDialog(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
	int     NCode;
	int     Id;

	switch (pMsg->MsgId) 
	{
		/* 初始化消息 */
		case WM_INIT_DIALOG:
			
			//
			// Initialization of 'Framewin'
			//
			hItem = pMsg->hWin;
			FRAMEWIN_SetFont(hItem, GUI_FONT_32B_ASCII);
			FRAMEWIN_SetText(hItem, "armfly");
			FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			//
			// Initialization of 'Multipage'
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIPAGE_0);
			MULTIPAGE_AddEmptyPage(hItem, 0, "STM32-V5");
			MULTIPAGE_AddEmptyPage(hItem, 0, "STM32-X3");
			MULTIPAGE_AddEmptyPage(hItem, 0, "STM32-V3");
			MULTIPAGE_AddEmptyPage(hItem, 0, "STM32-V2");
			MULTIPAGE_AddEmptyPage(hItem, 0, "STM32-X2");
			break;
		
		 case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) 
			{
				case ID_MULTIPAGE_0:
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						
						case WM_NOTIFICATION_RELEASED:
							break;
						
						case WM_NOTIFICATION_VALUE_CHANGED:
							break;
					}
					break;
					
				case ID_BUTTON_0:
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						
						case WM_NOTIFICATION_RELEASED:
							break;
						
						case WM_NOTIFICATION_VALUE_CHANGED:
							break;
					}
					break;
			}
			break;
			
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: CreateFramewin
*	功能说明: 创建对话框
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
WM_HWIN CreateFramewin(void) 
{
	WM_HWIN hWin;
	hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	
	return hWin;
}

/*
*********************************************************************************************************
*	函 数 名: TestMultiPage
*	功能说明: 多页控件（按键不能响应）
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void TestMultiPage(void) 
{ 
    WM_SetDesktopColor(GUI_WHITE);     
    WM_SetCreateFlags(WM_CF_MEMDEV);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	SPINBOX_SetDefaultSkin(SPINBOX_SKIN_FLEX);
	CreateFramewin();
	
	while (1)
	{
		GUI_Delay(100);
	}
}
#endif

#if (TEST_PROGRESS_BAR == 0x01U)

static void _DemoProgBar(void) {
  int i;
  PROGBAR_Handle ahProgBar[2];

  GUI_SetBkColor(GUI_BLACK);
  GUI_Clear();
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(&GUI_Font24_ASCII);
  GUI_DispStringHCenterAt("WIDGET_Progbar - Sample", 160, 5);
  GUI_SetFont(&GUI_Font8x16);
  GUI_DispStringAt("Progress bar", 100,80);
  //
  // Create the progbars
  //
  ahProgBar[0] = PROGBAR_Create(100,100,100,20, WM_CF_SHOW);
  ahProgBar[1] = PROGBAR_Create( 80,150,140,10, WM_CF_SHOW);
  //
  // Use memory device (optional, for better looks)
  //
  WM_EnableMemdev(ahProgBar[0]);
  WM_EnableMemdev(ahProgBar[1]);
  PROGBAR_SetMinMax(ahProgBar[1], 0, 500);
  PROGBAR_SetFont(ahProgBar[0], &GUI_Font8x16);
  GUI_Delay(500);
  while(1) {
    PROGBAR_SetFont(ahProgBar[0], &GUI_Font8x16);
    if (LCD_GetDevCap(LCD_DEVCAP_BITSPERPIXEL) <= 4) {
      PROGBAR_SetBarColor(ahProgBar[0], 0, GUI_DARKGRAY);
      PROGBAR_SetBarColor(ahProgBar[0], 1, GUI_LIGHTGRAY);
    } else {
      PROGBAR_SetBarColor(ahProgBar[0], 0, GUI_GREEN);
      PROGBAR_SetBarColor(ahProgBar[0], 1, GUI_RED);
    }
    PROGBAR_SetTextAlign(ahProgBar[0], GUI_TA_HCENTER);
    PROGBAR_SetText(ahProgBar[0], NULL);
    for (i=0; i<=100; i++) {
      PROGBAR_SetValue(ahProgBar[0], i);
      PROGBAR_SetValue(ahProgBar[1], i);
      GUI_Delay(5);
    }
    PROGBAR_SetText(ahProgBar[0], "Tank empty");
    for (; i>=0; i--) {
      PROGBAR_SetValue(ahProgBar[0], i);
      PROGBAR_SetValue(ahProgBar[1], 200-i);
      GUI_Delay(5);
    }
    PROGBAR_SetText(ahProgBar[0], "Any text...");
    PROGBAR_SetTextAlign(ahProgBar[0], GUI_TA_LEFT);
    for (; i<=100; i++) {
      PROGBAR_SetValue(ahProgBar[0], i);
      PROGBAR_SetValue(ahProgBar[1], 200+i);
      GUI_Delay(5);
    }
    PROGBAR_SetTextAlign(ahProgBar[0], GUI_TA_RIGHT);
    for (; i>=0; i--) {
      PROGBAR_SetValue(ahProgBar[0], i);
      PROGBAR_SetValue(ahProgBar[1], 400-i);
      GUI_Delay(5);
    }
    PROGBAR_SetFont(ahProgBar[0], &GUI_FontComic18B_1);
    PROGBAR_SetText(ahProgBar[0], "Any font...");
    for (; i<=100; i++) {
      PROGBAR_SetValue(ahProgBar[0], i);
      PROGBAR_SetValue(ahProgBar[1], 400+i);
      GUI_Delay(5);
    }
    GUI_Delay(500);
  }
}

/*
*********************************************************************************************************
*	函 数 名: TestProgressBar
*	功能说明: 进度条
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void TestProgressBar(void) 
{ 
    WM_SetDesktopColor(GUI_WHITE);      
    WM_SetCreateFlags(WM_CF_MEMDEV);   
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	
    while (1) {
    _DemoProgBar();
  }
}

#endif

#if (TEST_SINGLE_BUTTON == 0x01U)
/*
*********************************************************************************************************
*	                                    宏定义     
*********************************************************************************************************
*/
#define ID_FRAMEWIN_0  (GUI_ID_USER + 0x00)
#define ID_RADIO_0  (GUI_ID_USER + 0x04)

/*
*********************************************************************************************************
*	                                GUI_WIDGET_CREATE_INFO 类型数组    
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 480, 272, 0, 0x64, 0 },
  { RADIO_CreateIndirect, "Radio", ID_RADIO_0, 79, 32, 232, 137, 0, 0x2305, 0 },
};

/*
*********************************************************************************************************
*	函 数 名: _cbDialog
*	功能说明: 回调函数
*	形    参：pMsg 消息
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbDialog(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
	int     NCode;
	int     Id;

	switch (pMsg->MsgId) 
	{
		/* 初始化消息 */
		case WM_INIT_DIALOG:
			
			//
			// 初始化 'Framewin'
			//
			hItem = pMsg->hWin;
			FRAMEWIN_SetText(hItem, "armfly");
			FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			FRAMEWIN_SetFont(hItem, GUI_FONT_32B_ASCII);
			//
			// 初始化 'Radio'
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_0);
			RADIO_SetTextColor(hItem, 0x00000000);
			RADIO_SetFont(hItem, GUI_FONT_32B_ASCII);
			RADIO_SetText(hItem, "STM32-V5", 0);
			RADIO_SetText(hItem, "STM32-X3", 1);
			RADIO_SetText(hItem, "STM32-V3", 2);
			RADIO_SetText(hItem, "STM32-V2", 3);
			break;
		
		 case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) 
			{
				case ID_RADIO_0:
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						
						case WM_NOTIFICATION_RELEASED:
							break;
						
						case WM_NOTIFICATION_VALUE_CHANGED:
							break;
					}
				
			}
			break;
			
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: CreateFramewin
*	功能说明: 创建对话框
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
WM_HWIN CreateFramewin(void) 
{
	WM_HWIN hWin;
	hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	
	return hWin;
}

/*
*********************************************************************************************************
*	函 数 名: TestSingleButton
*	功能说明: GUI主函数
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void TestSingleButton(void) 
{ 
    WM_SetDesktopColor(GUI_WHITE);     
    WM_SetCreateFlags(WM_CF_MEMDEV);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	SPINBOX_SetDefaultSkin(SPINBOX_SKIN_FLEX);
	CreateFramewin();
	
	while (1)
	{
		GUI_Delay(100);
	}
}	
#endif

#if (TEST_SCROLLBAR == 0x01U)
/*
*********************************************************************************************************
*	                                    宏定义     
*********************************************************************************************************
*/
#define ID_FRAMEWIN_0  (GUI_ID_USER + 0x00)
#define ID_SCROLLBAR_0  (GUI_ID_USER + 0x01)
#define ID_SCROLLBAR_1  (GUI_ID_USER + 0x02)

/*
*********************************************************************************************************
*	                                GUI_WIDGET_CREATE_INFO 类型数组    
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 480, 272, 0, 0x64, 0 },
  { SCROLLBAR_CreateIndirect, "Scrollbar", ID_SCROLLBAR_0, 37, 15, 269, 30, 0, 0x0, 0 },
  { SCROLLBAR_CreateIndirect, "Scrollbar", ID_SCROLLBAR_1, 367, 13, 29, 195, 8, 0x0, 0 },
};

/*
*********************************************************************************************************
*	函 数 名: _cbDialog
*	功能说明: 回调函数
*	形    参：pMsg 消息
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbDialog(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
	int     NCode;
	int     Id;

	switch (pMsg->MsgId) 
	{
		/* 初始化消息 */
		case WM_INIT_DIALOG:
			hItem = pMsg->hWin;
			FRAMEWIN_SetText(hItem, "armfly");
			FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			FRAMEWIN_SetFont(hItem, GUI_FONT_32B_ASCII);
			SCROLLBAR_CreateAttached(hItem, 0);
			SCROLLBAR_CreateAttached(hItem, SCROLLBAR_CF_VERTICAL);
			break;
		
		 case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) 
			{
				case ID_SCROLLBAR_0:
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						
						case WM_NOTIFICATION_RELEASED:
							break;
						
						case WM_NOTIFICATION_VALUE_CHANGED:
							break;
					}
				
				case ID_SCROLLBAR_1:
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						
						case WM_NOTIFICATION_RELEASED:
							break;
						
						case WM_NOTIFICATION_VALUE_CHANGED:
							break;
					}
			}
			break;
			
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: CreateFramewin
*	功能说明: 创建对话框
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
WM_HWIN CreateFramewin(void) 
{
	WM_HWIN hWin;
	hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	
	return hWin;
}

/*
*********************************************************************************************************
*	函 数 名: MainTask
*	功能说明: GUI主函数
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void MainTask(void) 
{ 
    WM_SetDesktopColor(GUI_WHITE);     
    WM_SetCreateFlags(WM_CF_MEMDEV);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	SPINBOX_SetDefaultSkin(SPINBOX_SKIN_FLEX);
	CreateFramewin();
	
	while (1)
	{
		GUI_Delay(100);
	}
}		
#endif

#if (TEST_SLIDERBAR == 0x01U)
/*
*********************************************************************************************************
*	                                    宏定义     
*********************************************************************************************************
*/
#define ID_FRAMEWIN_0  (GUI_ID_USER + 0x03)
#define ID_SLIDER_0  (GUI_ID_USER + 0x04)
#define ID_SLIDER_1  (GUI_ID_USER + 0x07)

/*
*********************************************************************************************************
*	                                GUI_WIDGET_CREATE_INFO 类型数组    
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 480, 272, 0, 0x64, 0 },
  { SLIDER_CreateIndirect, "Slider", ID_SLIDER_0, 38, 19, 210, 39, 0, 0x0, 0 },
  { SLIDER_CreateIndirect, "Slider", ID_SLIDER_1, 309, 16, 40, 150, 8, 0x0, 0 },
};

/*
*********************************************************************************************************
*	函 数 名: _cbDialog
*	功能说明: 回调函数
*	形    参：pMsg 消息
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbDialog(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
	int     NCode;
	int     Id;

	switch (pMsg->MsgId) 
	{
		/* 初始化消息 */
		case WM_INIT_DIALOG:
			
			hItem = pMsg->hWin;
			FRAMEWIN_SetText(hItem, "armfly");
			FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			FRAMEWIN_SetFont(hItem, GUI_FONT_32B_ASCII);
			break;
		
		 case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) 
			{
				case ID_SLIDER_0:
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						
						case WM_NOTIFICATION_RELEASED:
							break;
						
						case WM_NOTIFICATION_VALUE_CHANGED:
							break;
					}
				
				case ID_SLIDER_1:
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						
						case WM_NOTIFICATION_RELEASED:
							break;
						
						case WM_NOTIFICATION_VALUE_CHANGED:
							break;
					}
			}
			break;
			
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: CreateFramewin
*	功能说明: 创建对话框
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
WM_HWIN CreateFramewin(void) 
{
	WM_HWIN hWin;
	hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	
	return hWin;
}

/*
*********************************************************************************************************
*	函 数 名: MainTask
*	功能说明: GUI主函数
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void MainTask(void) 
{ 
    WM_SetDesktopColor(GUI_WHITE);     
    WM_SetCreateFlags(WM_CF_MEMDEV);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	SPINBOX_SetDefaultSkin(SPINBOX_SKIN_FLEX);
	CreateFramewin();
	
	while (1)
	{
		GUI_Delay(100);
	}
}
#endif	

#if (TEST_TEXT_CONTROL == 0x01U)
/*
*********************************************************************************************************
*	                                    宏定义     
*********************************************************************************************************
*/
#define ID_FRAMEWIN_0  (GUI_ID_USER + 0x00)
#define ID_TEXT_0  (GUI_ID_USER + 0x01)
#define ID_TEXT_1  (GUI_ID_USER + 0x02)
#define ID_TEXT_2  (GUI_ID_USER + 0x03)

/*
*********************************************************************************************************
*	                                GUI_WIDGET_CREATE_INFO 类型数组    
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 480, 272, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 149, 24, 99, 19, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_1, 149, 55, 92, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_2, 149, 92, 94, 20, 0, 0x64, 0 },
};

/*
*********************************************************************************************************
*	函 数 名: _cbDialog
*	功能说明: 回调函数
*	形    参：pMsg 消息
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbDialog(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;

	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			//
			// 初始化 'Framewin'
			//
			hItem = pMsg->hWin;
			FRAMEWIN_SetText(hItem, "Framewin");
			FRAMEWIN_SetFont(hItem, GUI_FONT_32B_ASCII);
			FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			
			//
			// 初始化 'Text'
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
			TEXT_SetFont(hItem, GUI_FONT_20B_ASCII);
			TEXT_SetText(hItem, "STM32-V5");
			
			//
			// 初始化 'Text'
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
			TEXT_SetFont(hItem, GUI_FONT_20B_ASCII);
			TEXT_SetText(hItem, "STM32-X3");
			
			//
			// 初始化 'Text'
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
			TEXT_SetFont(hItem, GUI_FONT_20B_ASCII);
			TEXT_SetText(hItem, "STM32-V3");
			break;
		
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: CreateFramewin
*	功能说明: 创建对话框
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
WM_HWIN CreateFramewin(void);
WM_HWIN CreateFramewin(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

/*
*********************************************************************************************************
*	函 数 名: MainTask
*	功能说明: GUI主函数
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void MainTask(void) 
{ 
    GUI_Init();
    WM_SetDesktopColor(GUI_WHITE);     
    WM_SetCreateFlags(WM_CF_MEMDEV);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	CreateFramewin();
	while (1)
	{
		GUI_Delay(100);
	}
  
}
#endif	

#if (TEST_LIST_VIEW == 0x01U)
/*
*********************************************************************************************************
*	                                    宏定义     
*********************************************************************************************************
*/
#define ID_FRAMEWIN_0  (GUI_ID_USER + 0x00)
#define ID_LISTVIEW_0  (GUI_ID_USER + 0x02)


/*
*********************************************************************************************************
*	                                GUI_WIDGET_CREATE_INFO 类型数组    
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 480, 272, 0, 0x64, 0 },
  { LISTVIEW_CreateIndirect, "Listview", ID_LISTVIEW_0, 81, 18, 264, 84, 0, 0x0, 0 },
};

/*
*********************************************************************************************************
*	函 数 名: _cbDialog
*	功能说明: 回调函数
*	形    参：pMsg 消息
*	返 回 值: 无
********************************************************************************************************
*/
static void _cbDialog(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
	int     NCode;
	int     Id;

	switch (pMsg->MsgId) 
	{
		/* 初始化消息 */
		case WM_INIT_DIALOG:
			hItem = pMsg->hWin;
			FRAMEWIN_SetFont(hItem, GUI_FONT_32B_ASCII);
			FRAMEWIN_SetText(hItem, "armfly");
			FRAMEWIN_SetTextColor(hItem, 0x00000000);
			FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);

			//
			// 初始化 'Listview'
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
			LISTVIEW_AddColumn(hItem, 30, "Col 0", GUI_TA_HCENTER | GUI_TA_VCENTER);
			LISTVIEW_AddColumn(hItem, 30, "Col 1", GUI_TA_HCENTER | GUI_TA_VCENTER);
			LISTVIEW_AddColumn(hItem, 30, "Col 2", GUI_TA_HCENTER | GUI_TA_VCENTER);
			LISTVIEW_AddRow(hItem, NULL);
			LISTVIEW_SetGridVis(hItem, 1);
			LISTVIEW_SetFont(hItem, GUI_FONT_13B_ASCII);
			LISTVIEW_SetItemText(hItem, 0, 0, "AAA");
			LISTVIEW_SetItemText(hItem, 1, 0, "BBB");
			LISTVIEW_SetItemText(hItem, 2, 0, "CCC");
			LISTVIEW_AddRow(hItem, NULL);
			LISTVIEW_SetItemText(hItem, 0, 1, "DDD");
			LISTVIEW_SetItemText(hItem, 1, 1, "EEE");
			LISTVIEW_SetItemText(hItem, 2, 1, "FFF");
			break;
		
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) 
			{
				case ID_LISTVIEW_0: 
				switch(NCode) 
				{
					case WM_NOTIFICATION_CLICKED:
						break;
					
					case WM_NOTIFICATION_RELEASED:
						break;
					
					case WM_NOTIFICATION_SEL_CHANGED:
						break;
				}
				break;
			}
			break;

		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: CreateFramewin
*	功能说明: 创建对话框
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
WM_HWIN CreateFramewin(void) 
{
	WM_HWIN hWin;
	hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	
	return hWin;
}

/*
*********************************************************************************************************
*	函 数 名: MainTask
*	功能说明: GUI主函数
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void MainTask(void) 
{ 
    WM_SetDesktopColor(GUI_WHITE);     
    WM_SetCreateFlags(WM_CF_MEMDEV);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	SPINBOX_SetDefaultSkin(SPINBOX_SKIN_FLEX);
	CreateFramewin();
	
	while (1)
	{
		GUI_Delay(100);
	}
}
#endif

#if (TEST_MESSAGE_BOX == 0x01U)
static void _cbBkWindow(WM_MESSAGE* pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    GUI_SetBkColor(GUI_RED);
    GUI_Clear();
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(&GUI_Font24_ASCII);
    GUI_DispStringHCenterAt("DIALOG_MessageBox - Sample", 160, 5);
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       MainTask
*/
void MainTask(void) {
  GUI_Init();

  WM_SetCreateFlags(WM_CF_MEMDEV);

  WM_SetCallback(WM_HBKWIN, &_cbBkWindow);
  //
  // Create message box and wait until it is closed
  //
  while (1) {
    GUI_MessageBox("This text is shown\nin a message box",
                   "Caption/Title", GUI_MESSAGEBOX_CF_MOVEABLE);
    GUI_Delay(750);                    // Wait for a short moment ...
    GUI_MessageBox("New message !",
                   "Caption/Title", GUI_MESSAGEBOX_CF_MOVEABLE);
    GUI_Delay(750);
  }
}
#endif

#if (TEST_GRAPHYT == 0x01U)
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define MAX_VALUE 180

//
// Recommended memory to run the sample with adequate performance
//
#define RECOMMENDED_MEMORY (1024L * 30)

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static GRAPH_DATA_Handle  _ahData[3]; // Array of handles for the GRAPH_DATA objects
static GRAPH_SCALE_Handle _hScaleV;   // Handle of vertical scale
static GRAPH_SCALE_Handle _hScaleH;   // Handle of horizontal scale

static I16 _aValue[3];
static int _Stop = 0;

static GUI_COLOR _aColor[] = {GUI_RED, GUI_GREEN, GUI_LIGHTBLUE}; // Array of colors for the GRAPH_DATA objects

//
// Dialog ressource
//
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Graph widget demo",  0                ,   0,   0, 320, 240, FRAMEWIN_CF_MOVEABLE },
  { GRAPH_CreateIndirect,     0,                   GUI_ID_GRAPH0    ,   5,   5, 265, 170 },
  { TEXT_CreateIndirect,      "Spacing X:",        0                ,  10, 180,  50,  20 },
  { TEXT_CreateIndirect,      "Spacing Y:",        0                ,  10, 200,  50,  20 },
  { SLIDER_CreateIndirect,    0,                   GUI_ID_SLIDER0   ,  60, 180,  60,  16 },
  { SLIDER_CreateIndirect,    0,                   GUI_ID_SLIDER1   ,  60, 200,  60,  16 },
  { CHECKBOX_CreateIndirect,  0,                   GUI_ID_CHECK0    , 130, 180,  50,   0 },
  { CHECKBOX_CreateIndirect,  0,                   GUI_ID_CHECK1    , 130, 200,  50,   0 },
  { TEXT_CreateIndirect,      "Border",            0                , 275,   5,  35,  15 },
  { CHECKBOX_CreateIndirect,  0,                   GUI_ID_CHECK2    , 275,  20,  35,   0 },
  { CHECKBOX_CreateIndirect,  0,                   GUI_ID_CHECK3    , 275,  40,  35,   0 },
  { CHECKBOX_CreateIndirect,  0,                   GUI_ID_CHECK4    , 275,  60,  35,   0 },
  { CHECKBOX_CreateIndirect,  0,                   GUI_ID_CHECK5    , 275,  80,  35,   0 },
  { TEXT_CreateIndirect,      "Effect",            0                , 275, 100,  35,  15 },
  { RADIO_CreateIndirect,     0,                   GUI_ID_RADIO0    , 270, 115,  35,   0, 0, 3 },
  { CHECKBOX_CreateIndirect,  0,                   GUI_ID_CHECK6    , 180, 180,  50,   0 },
  { CHECKBOX_CreateIndirect,  0,                   GUI_ID_CHECK7    , 180, 200,  50,   0 },
  { BUTTON_CreateIndirect,    "Full Screen",       GUI_ID_BUTTON0   , 240, 180,  65,  18 },
  { CHECKBOX_CreateIndirect,  0,                   GUI_ID_CHECK8    , 240, 200,  70,   0 },
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _AddValues
*
* Function description
*   This routine calculates new random values in dependence of the previous added values
*   and adds them to the GRAPH_DATA objects
*/
static void _AddValues(void) {
  unsigned i;

  for (i = 0; i < GUI_COUNTOF(_aColor); i++) {
    int Add = ((unsigned)rand()) % (2 + i * i);
    int Vz  = (((unsigned)(rand()) % 2) << 1) - 1;
    _aValue[i] += Add * Vz;
    if (_aValue[i] > MAX_VALUE) {
      _aValue[i] = MAX_VALUE;
    } else if (_aValue[i] < 0) {
      _aValue[i] = 0;
    }
    GRAPH_DATA_YT_AddValue(_ahData[i], _aValue[i]);
  }
}

/*********************************************************************
*
*       _UserDraw
*
* Function description
*   This routine is called by the GRAPH object before anything is drawn
*   and after the last drawing operation.
*/
static void _UserDraw(WM_HWIN hWin, int Stage) {
  if (Stage == GRAPH_DRAW_LAST) {
    char acText[] = "Temperature";
    GUI_RECT Rect;
    GUI_RECT RectInvalid;
    int FontSizeY;

    GUI_SetFont(&GUI_Font13_ASCII);
    FontSizeY = GUI_GetFontSizeY();
    WM_GetInsideRect(&Rect);
    WM_GetInvalidRect(hWin, &RectInvalid);
    Rect.x1 = Rect.x0 + FontSizeY;
    GUI_SetColor(GUI_YELLOW);
    GUI_DispStringInRectEx(acText, &Rect, GUI_TA_HCENTER, strlen(acText), GUI_ROTATE_CCW);
  }
}

/*********************************************************************
*
*       _ForEach
*
* Function description
*   This routine hides/shows all windows except the button, graph and scroll bar widgets
*/
static void _ForEach(WM_HWIN hWin, void * pData) {
  int Id;
  int FullScreenMode;

  FullScreenMode = *(int *)pData;
  Id = WM_GetId(hWin);
  if ((Id == GUI_ID_GRAPH0) || (Id == GUI_ID_BUTTON0) || (Id == GUI_ID_VSCROLL) || (Id == GUI_ID_HSCROLL)) {
    return;
  }
  if (FullScreenMode) {
    WM_HideWindow(hWin);
  } else {
    WM_ShowWindow(hWin);
  }
}

/*********************************************************************
*
*       _ToggleFullScreenMode
*
* Function description
*   This routine switches between full screen mode and normal mode by hiding or showing the
*   widgets of the dialog, enlarging/shrinking the graph widget and modifying some other
*   attributes of the dialog widgets.
*/
static void _ToggleFullScreenMode(WM_HWIN hDlg) {
  static int FullScreenMode;
  static GUI_RECT Rect;
  static unsigned ScalePos;
  WM_HWIN hGraph;
  WM_HWIN hButton;
  WM_HWIN hClient;
  GUI_RECT RectInside;
  int xPos, yPos;

  hGraph  = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
  hButton = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
  FullScreenMode ^= 1;
  if (FullScreenMode) {       
    //
    // Enter the full screen mode
    //
    hClient = WM_GetClientWindow(hDlg);
    BUTTON_SetText(hButton, "Back");
    WM_MoveWindow(hButton, 0, 11);
    FRAMEWIN_SetTitleVis(hDlg, 0);
    WM_GetInsideRectEx(hClient, &RectInside);
    WM_GetWindowRectEx(hGraph, &Rect);
    WM_ForEachDesc(hClient, _ForEach, &FullScreenMode); // Hide all descendants
    xPos = WM_GetWindowOrgX(hClient);
    yPos = WM_GetWindowOrgY(hClient);
    WM_SetWindowPos(hGraph, xPos, yPos, RectInside.x1, RectInside.y1);
    ScalePos = GRAPH_SCALE_SetPos(_hScaleH, RectInside.y1 - 20);
  } else {
    //
    // Return to normal mode
    //
    BUTTON_SetText(hButton, "Full Screen");
    WM_MoveWindow(hButton, 0, -11);
    WM_ForEachDesc(WM_GetClientWindow(hDlg), _ForEach, &FullScreenMode); // Show all descendants
    WM_SetWindowPos(hGraph, Rect.x0, Rect.y0, Rect.x1 - Rect.x0 + 1, Rect.y1 - Rect.y0 + 1);
    FRAMEWIN_SetTitleVis(hDlg, 1);
    GRAPH_SCALE_SetPos(_hScaleH, ScalePos);
  }
}

/*********************************************************************
*
*       _cbCallback
*
* Function description
*   Callback function of the dialog
*/
static void _cbCallback(WM_MESSAGE * pMsg) {
  unsigned i;
  int      NCode;
  int      Id;
  int      Value;
  WM_HWIN  hDlg;
  WM_HWIN  hItem;

  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
    //
    // Add graphs
    //
    for (i = 0; i < GUI_COUNTOF(_aColor); i++) {
      _aValue[i] = rand() % 180;
      _ahData[i] = GRAPH_DATA_YT_Create(_aColor[i], 500, 0, 0);
      GRAPH_AttachData(hItem, _ahData[i]);
    }
    //
    // Set graph attributes
    //
    GRAPH_SetGridDistY(hItem, 25);
    GRAPH_SetGridVis(hItem, 1);
    GRAPH_SetGridFixedX(hItem, 1);
    GRAPH_SetUserDraw(hItem, _UserDraw);
    //
    // Create and add vertical scale
    //
    _hScaleV = GRAPH_SCALE_Create( 35, GUI_TA_RIGHT, GRAPH_SCALE_CF_VERTICAL, 25);
    GRAPH_SCALE_SetTextColor(_hScaleV, GUI_YELLOW);
    GRAPH_AttachScale(hItem, _hScaleV);
    //
    // Create and add horizontal scale
    //
    _hScaleH = GRAPH_SCALE_Create(155, GUI_TA_HCENTER, GRAPH_SCALE_CF_HORIZONTAL, 50);
    GRAPH_SCALE_SetTextColor(_hScaleH, GUI_DARKGREEN);
    GRAPH_AttachScale(hItem, _hScaleH);
    //
    // Init check boxes
    //
    hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK2);
    CHECKBOX_SetText(hItem, "L");
    hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK3);
    CHECKBOX_SetText(hItem, "T");
    hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK4);
    CHECKBOX_SetText(hItem, "R");
    hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK5);
    CHECKBOX_SetText(hItem, "B");
    hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK0);
    CHECKBOX_SetText(hItem, "Stop");
    hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK1);
    CHECKBOX_SetText(hItem, "Grid");
    CHECKBOX_SetState(hItem, 1);
    hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK6);
    CHECKBOX_SetText(hItem, "HScroll");
    CHECKBOX_SetState(hItem, 1);
    hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK7);
    CHECKBOX_SetText(hItem, "VScroll");
    hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK8);
    CHECKBOX_SetText(hItem, "MirrorX");
    //
    // Init slider widgets
    //
    hItem = WM_GetDialogItem(hDlg, GUI_ID_SLIDER0);
    SLIDER_SetRange(hItem, 0, 10);
    SLIDER_SetValue(hItem, 5);
    SLIDER_SetNumTicks(hItem, 6);
    hItem = WM_GetDialogItem(hDlg, GUI_ID_SLIDER1);
    SLIDER_SetRange(hItem, 0, 20);
    SLIDER_SetValue(hItem, 5);
    SLIDER_SetNumTicks(hItem, 6);
    //
    // Init radio widget
    //
    hItem = WM_GetDialogItem(hDlg, GUI_ID_RADIO0);
    RADIO_SetText(hItem, "3D", 0);
    RADIO_SetText(hItem, "flat", 1);
    RADIO_SetText(hItem, "-", 2);
    //
    // Init button widget
    //
    hItem = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
    WM_SetStayOnTop(hItem, 1);
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);      // Id of widget
    NCode = pMsg->Data.v;                 // Notification code
    switch (NCode) {
    case WM_NOTIFICATION_CLICKED:
      switch (Id) {
      case GUI_ID_BUTTON0:
        _ToggleFullScreenMode(hDlg);
        break;
      }
      break;
    case WM_NOTIFICATION_VALUE_CHANGED:
      switch (Id) {
      case GUI_ID_CHECK0:
        //
        // Toggle stop mode
        //
        _Stop ^= 1;
        break;
      case GUI_ID_CHECK1:
        //
        // Toggle grid
        //
        hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
        GRAPH_SetGridVis(hItem, CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK1)));
        break;
      case GUI_ID_CHECK2:
      case GUI_ID_CHECK3:
      case GUI_ID_CHECK4:
      case GUI_ID_CHECK5:
        //
        // Toggle border
        //
        hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
        GRAPH_SetBorder(hItem, 
                        CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK2)) * 40, 
                        CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK3)) * 5, 
                        CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK4)) * 5, 
                        CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK5)) * 5);
        break;
      case GUI_ID_SLIDER0:
        //
        // Set horizontal grid spacing
        //
        hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
        Value = SLIDER_GetValue(pMsg->hWinSrc) * 10;
        GRAPH_SetGridDistX(hItem, Value);
        GRAPH_SCALE_SetTickDist(_hScaleH, Value);
        break;
      case GUI_ID_SLIDER1:
        //
        // Set vertical grid spacing
        //
        hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
        Value = SLIDER_GetValue(pMsg->hWinSrc) * 5;
        GRAPH_SetGridDistY(hItem, Value);
        GRAPH_SCALE_SetTickDist(_hScaleV, Value);
        break;
      case GUI_ID_RADIO0:
        //
        // Set the widget effect
        //
        hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
        switch (RADIO_GetValue(pMsg->hWinSrc)) {
        case 0:
          WIDGET_SetEffect(hItem, &WIDGET_Effect_3D);
          break;
        case 1:
          WIDGET_SetEffect(hItem, &WIDGET_Effect_Simple);
          break;
        case 2:
          WIDGET_SetEffect(hItem, &WIDGET_Effect_None);
          break;
        }
        break;
      case GUI_ID_CHECK6:
        //
        // Toggle horizontal scroll bar
        //
        hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
        if (CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK6))) {
          GRAPH_SetVSizeX(hItem, 500);
        } else {
          GRAPH_SetVSizeX(hItem, 0);
        }
        break;
      case GUI_ID_CHECK7:
        //
        // Toggle vertical scroll bar
        //
        hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
        if (CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK7))) {
          GRAPH_SetVSizeY(hItem, 300);
        } else {
          GRAPH_SetVSizeY(hItem, 0);
        }
        break;
      case GUI_ID_CHECK8:
        //
        // Toggle alignment
        //
        WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
        for (i = 0; i < GUI_COUNTOF(_aColor); i++) {
          if (CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK8))) {
            GRAPH_DATA_YT_SetAlign(_ahData[i], GRAPH_ALIGN_LEFT);
            GRAPH_DATA_YT_MirrorX (_ahData[i], 1);
          } else {
            GRAPH_DATA_YT_SetAlign(_ahData[i], GRAPH_ALIGN_RIGHT);
            GRAPH_DATA_YT_MirrorX (_ahData[i], 0);
          }
        }
        break;
      }
      break;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       MainTask
*/
void MainTask(void) {
  WM_HWIN hDlg;
  WM_HWIN hGraph;

  hGraph = 0;
  GUI_Init();
  //
  // Check if recommended memory for the sample is available
  //
  if (GUI_ALLOC_GetNumFreeBytes() < RECOMMENDED_MEMORY) {
    GUI_ErrorOut("Not enough memory available."); 
    return;
  }
  GUI_CURSOR_Show();
  WM_SetDesktopColor(GUI_BLACK);
  #if GUI_SUPPORT_MEMDEV
    WM_SetCreateFlags(WM_CF_MEMDEV);
  #endif
  hDlg = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbCallback, 0, 0, 0);
  while (1) {
    #ifdef WIN32
      GUI_Delay(10);
    #endif
    if (!_Stop) {
      if (!hGraph) {
        hGraph = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
      }
      _AddValues();
    }
    GUI_Exec();
  }
}
#endif

/*
*********************************************************************************************************
* Function Name : main
* Description	: 程序入口
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
int main( void )
{
	System_Init();

	#if (TEST_TEXT == 0x01U)
		DisplayText();
	#endif
	
	#if (TEST_PIC == 0x01U)
		DisplayPIC();
	#endif
	
	#if (TEST_FONTS == 0x01U)
		MainTask();
	#endif
	
	#if (TEST_CURSOR == 0x01U)
		DisplayCursor();
	#endif
	
	#if (TEST_ANTI_ALIASING == 0x01U)
		DisplayAntiAliasing();
	#endif
	
	#if (TEST_MULTITASK == 0x01U)
		TestMultiTask();
	#endif
	
	#if (TEST_KEY_PID == 0x01U)
		TestKeyPID();
	#endif
	
	#if (TEST_KEY == 0x01U)
		TestKey();
	#endif
	
	#if (TEST_BUTTON == 0x01U)
		TestButton();
	#endif
	
	#if (TEST_DOWN_LIST == 0x01U)
		TestDownList();
	#endif
	
	#if (TEST_CHECK_BOX == 0x01U)
		TestCheckBox();
	#endif
	
	#if (TEST_EDIT == 0x01U)
		TestEdit();
	#endif
	
	#if (TEST_LIST_BOX == 0x01U)
		TestListBox();
	#endif
	
	#if (TEST_MENU == 0x01U)
		TestMenu();
	#endif
	
	#if (TEST_MULTI_TEXT == 0x01U)
		TestMultiText();
	#endif
	
	#if (TEST_MULTI_PAGE == 0x01U)
		TestMultiPage();
	#endif
	
	#if (TEST_PROGRESS_BAR == 0x01U)
		TestProgressBar();
	#endif
	
	#if (TEST_SINGLE_BUTTON == 0x01U)
		TestSingleButton();
	#endif
	
	#if (TEST_SCROLLBAR == 0x01U)
		MainTask();
	#endif
	
	#if (TEST_SLIDERBAR == 0x01U)
		MainTask();
	#endif	
	
	#if (TEST_TEXT_CONTROL == 0x01U)
		MainTask();
	#endif	
	
	#if (TEST_SPINBOX == 0x01U)
		MainTask();
	#endif	
	
	#if (TEST_LIST_VIEW == 0x01U)
		MainTask();
	#endif
	
	#if (TEST_MESSAGE_BOX == 0x01U)
		MainTask();
	#endif
	
	#if (TEST_GRAPHYT == 0x01U)
		MainTask();
	#endif
	
	
	while (1)
	{
		GUI_Delay(10);
	}
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
