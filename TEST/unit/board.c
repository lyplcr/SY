/*
*********************************************************************************************************
* @file    	board.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-19 09:33:30
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	电路板驱动测试源文件
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
#include "usbh_usr.h"
#include "ethernetif.h"
#include "UDP_Server.h"
#include "TCP_Client.h"

/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/
#define TEST_KEY							0x00U
#define TEST_LCD							0x00U
#define TEST_SD								0x00U
#define TEST_USB							0x00U
#define TEST_ETH							0x01U

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
static uint8_t buff[1024];

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
* Function Name : Test_Key
* Description	: 测试按键
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void Test_Key( void )
{
	uint32_t key = ReadKeyValue(g_keyPtr);
	if (key != KEY_NONE)
	{	
		switch (ReadKeyStatus(g_keyPtr))
		{
			case STATUS_KEY_PRESS:			
			{		
				printf("按键按下\r\n");
				break;
			}
			case STATUS_KEY_LONG:
			{		
				printf("按键长按\r\n");
				break;
			}
			case STATUS_KEY_HIGH_SPEED:
			{		
				printf("按键高速\r\n");
				break;
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
* Function Name : Test_LCD
* Description	: 测试LCD 1、按下任意键，屏幕轮询显示10种颜色。
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void Test_LCD( void )
{
	static uint8_t index = 0;
	uint16_t table[10] = {CL_WHITE,CL_RED,CL_BLACK,CL_GREEN,CL_BLUE,CL_YELLOW,CL_GREY,CL_GREY1,CL_MAGENTA,CL_CYAN};
	
	uint32_t key = ReadKeyValue(g_keyPtr);
	if (key != KEY_NONE)
	{	
		switch (ReadKeyStatus(g_keyPtr))
		{
			case STATUS_KEY_PRESS:			
			{		
				LCD_ClrScr(table[index++]);
				if (index >= 10)
				{
					index = 0;
				}
				break;
			}
			default:
			{		
				break;
			}
		}			
		ReleaseKeyMsg(g_keyPtr);
	}
	
	LCD_DrawLine(100,100,200,200,CL_RED);
	
	FONT_T font = {
	.FontCode = FC_ST_16,
	.FrontColor = CL_GREEN,
	.BackColor = CL_BLACK,
	.Space = 0,
	};
	
	LCD_Fill_Rect(100,100,300,300,CL_WHITE);
	LCD_DispStrEx(200,200,"STM32 Logo...",&font,100,ALIGN_CENTER);
	
	while (1);
}

/*
*********************************************************************************************************
* Function Name : Test_SD
* Description	: 测试SD卡
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void Test_SD( void )
{
	FIL fileHandle;
	
	uint32_t key = ReadKeyValue(g_keyPtr);
	if (key != KEY_NONE)
	{	
		switch (ReadKeyStatus(g_keyPtr))
		{
			case STATUS_KEY_PRESS:			
			{						
			#if 1
				FRESULT res = f_open(&fileHandle, "0:/SYE联网.TXT", FA_OPEN_EXISTING | FA_READ);
				if (res != FR_OK)
				{
					LCD_Fill_Rect(100,100,100,100,CL_RED);
					while (1);
				}
				uint32_t bytesread;
				res = f_read(&fileHandle, (void *)buff, sizeof(buff), (void *)&bytesread);
				if (res != FR_OK)
				{
					LCD_Fill_Rect(100,100,100,100,CL_RED);
					while (1);
				}
				
				f_close(&fileHandle);
				
				LCD_Fill_Rect(200,200,100,100,CL_GREEN);
			#endif	
				
			#if 1
				res = f_open(&fileHandle, "0:/SYE_AABBCCDDEEFFGGHHIIJJKKLLMMNN_COPY.TXT", FA_CREATE_ALWAYS | FA_WRITE);
				if (res != FR_OK)
				{
					LCD_Fill_Rect(100,100,100,100,CL_RED);
					while (1);
				}
				uint32_t byteswritten;
				res = f_write(&fileHandle, buff, sizeof(buff), (void *)&byteswritten);
				if (res != FR_OK)
				{
					LCD_Fill_Rect(100,100,100,100,CL_RED);
					while (1);
				}
				
				f_close(&fileHandle);
				
				LCD_Fill_Rect(200,200,100,100,CL_GREEN);
			#endif
				break;
			}
			default:
			{		
				break;
			}
		}			
		ReleaseKeyMsg(g_keyPtr);
	}
}

/*
*********************************************************************************************************
* Function Name : Test_USB
* Description	: 测试USB
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void Test_USB( void )
{
	FIL fileHandle;
	FRESULT res;
	
	uint32_t key = ReadKeyValue(g_keyPtr);
	if (key != KEY_NONE)
	{	
		switch (ReadKeyStatus(g_keyPtr))
		{
			case STATUS_KEY_PRESS:			
			{						
			#if 1
				res = f_open(&fileHandle, "1:/SYE联网.txt", FA_OPEN_EXISTING | FA_READ);
				if (res != FR_OK)
				{
					LCD_Fill_Rect(100,100,100,100,CL_RED);
					while (1);
				}
				uint32_t bytesread;
				res = f_read(&fileHandle, (void *)buff, sizeof(buff), (void *)&bytesread);
				if (res != FR_OK)
				{
					LCD_Fill_Rect(100,100,100,100,CL_RED);
					while (1);
				}
				
				f_close(&fileHandle);
				
				LCD_Fill_Rect(200,200,100,100,CL_GREEN);
			#endif	
				
			#if 1
				res = f_open(&fileHandle, "1:/SYE_AABBCCDDEEFFGGHHIIJJKKLLMMNN_COPY1.txt", FA_CREATE_ALWAYS | FA_WRITE);
				if (res != FR_OK)
				{
					LCD_Fill_Rect(100,100,100,100,CL_RED);
					while (1);
				}

				uint32_t byteswritten;
				res = f_write(&fileHandle, buff, bytesread, (void *)&byteswritten);
				if (res != FR_OK)
				{
					LCD_Fill_Rect(100,100,100,100,CL_RED);
					while (1);
				}
				
				f_close(&fileHandle);
				
				LCD_Fill_Rect(200,200,100,100,CL_GREEN);
			#endif
				break;
			}
			default:
			{		
				break;
			}
		}			
		ReleaseKeyMsg(g_keyPtr);
	}
}

/*
*********************************************************************************************************
* Function Name : Test_ETH
* Description	: 测试ETH
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void Test_ETH( void )
{
#if 0
	uint32_t key = ReadKeyValue(g_keyPtr);
	if (key != KEY_NONE)
	{	
		switch (ReadKeyStatus(g_keyPtr))
		{
			case STATUS_KEY_PRESS:			
			{	
				uint8_t data;
				uint8_t buff[1024];
				uint32_t cnt = 0;
				bool isRdOver = true;
				
				while (1)
				{
					if (UDP_ServerReadByte(&data) == true)
					{
						buff[cnt++] = data;
						isRdOver = false;
					}
					else
					{
						isRdOver = true;
					}
					
					if ((cnt == 1024) || (isRdOver == true))
					{
						UDP_ServerEchoWriteBytes(buff, cnt);
						break;
					}
				}
				break;
			}
			default:
				break;
		}
		ReleaseKeyMsg(g_keyPtr);
	}
#endif

#if 0
	if ( bsp_CheckTimer(TIMING_CORE) )
	{
		uint8_t data;
		uint8_t buff[1024];
		uint32_t cnt = 0;
		bool isRdOver = true;
		
		while (1)
		{
			if (UDP_ServerReadByte(&data) == true)
			{
				buff[cnt++] = data;
				isRdOver = false;
			}
			else
			{
				isRdOver = true;
			}
			
			if ((cnt == 1024) || (isRdOver == true))
			{
				UDP_ServerEchoWriteBytes(buff, cnt);
				break;
			}
		}
	}
#endif	
	
#if 0
	uint32_t key = ReadKeyValue(g_keyPtr);
	if (key != KEY_NONE)
	{	
		switch (ReadKeyStatus(g_keyPtr))
		{
			case STATUS_KEY_PRESS:			
			{	
				if (key == KEY_F1)
				{
					uint8_t remoteIP[4] = {192,168,0,73};
					uint16_t remotePort = 7777;
					
					if (TCP_ClientConnect(remoteIP,remotePort) == false)
					{
						printf("建立连接错误！\r\n");
					}
				}
				else if (key == KEY_F2)
				{
					if (TCP_ClientClose() == false)
					{
						printf("关闭连接错误！\r\n");
					}
				}
				break;
			}
			default:
				break;
		}
		ReleaseKeyMsg(g_keyPtr);
	}
	
	if ( bsp_CheckTimer(TIMING_CORE) )
	{
		uint8_t data;
		uint8_t buff[1024];
		uint32_t cnt = 0;
		bool isRdOver = true;
		
		while (1)
		{
			if (TCP_ClientReadByte(&data) == true)
			{
				buff[cnt++] = data;
				isRdOver = false;
			}
			else
			{
				isRdOver = true;
			}
			
			if ((cnt == 1024) || (isRdOver == true))
			{
				TCP_ClientWriteBytes(buff, cnt);
				break;
			}
		}
	}
#endif	
	
#if 0
	static bool isStart;
	
	uint32_t key = ReadKeyValue(g_keyPtr);
	if (key != KEY_NONE)
	{	
		switch (ReadKeyStatus(g_keyPtr))
		{
			case STATUS_KEY_PRESS:			
			{	
				isStart = true;
				break;
			}
			default:
				break;
		}
	}

	if (isStart == true)
	{
		if ( bsp_CheckTimer(TIMING_CORE) )
		{
			uint8_t remoteIP[4] = {192,168,0,73};
			uint16_t remotePort = 7777;
			
			if (TCP_ClientConnect(remoteIP,remotePort) == false)
			{
				printf("建立连接错误！\r\n");
			}
			
			bsp_DelayMS(100);
			
			if (TCP_ClientClose() == false)
			{
				printf("关闭连接错误！\r\n");
			}
		}
	}
#endif	
}

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

	while (1)
	{
		#if (TEST_KEY == 0x01U)
			Test_Key();
		#endif
		
		#if (TEST_LCD == 0x01U)
			Test_LCD();
		#endif
		
		#if (TEST_SD == 0x01U)
			Test_SD();
		#endif
		
		#if (TEST_USB == 0x01U)
			Test_USB();
		#endif
		
		#if (TEST_ETH == 0x01U)
			Test_ETH();
		#endif
		
		/* 采样循环 */
		if ( bsp_CheckTimer(TIMING_SAMPLE) )
		{
			;
		}

		/* 核心循环 */
		if ( bsp_CheckTimer(TIMING_CORE) )
		{	
			IO_Task();								/* 输入输出任务 	*/
		}
		
		/* 控制循环 */
		if ( bsp_CheckTimer(TIMING_CTRL) )
		{
			;
		}
	}
}



/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
