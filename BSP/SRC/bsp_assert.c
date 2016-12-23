/**
  ******************************************************************************
  * @file    bsp_assert.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-6 13:09:34
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   断言
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"	

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

/* Private typedef -----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* 
	ST库函数使用了C编译器的断言功能，如果定义了USE_FULL_ASSERT，那么所有的ST库函数将检查函数形参
	是否正确。如果不正确将调用 assert_failed() 函数，这个函数是一个死循环，便于用户检查代码。
	
	关键字 __LINE__ 表示源代码行号。
	关键字__FILE__表示源代码文件名。
	
	断言功能使能后将增大代码大小，推荐用户仅在调试时使能，在正式发布软件是禁止。

	用户可以选择是否使能ST固件库的断言供能。使能断言的方法有两种：
	(1) 在C编译器的预定义宏选项中定义USE_FULL_ASSERT。
	(2) 在本文件取消"#define USE_FULL_ASSERT    1"行的注释。	
*/
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
	char showBuff[500];
	
//  /* User can add his own implementation to report the file name and line number*/
//    printf("Wrong parameters value: file %s on line %d\r\n", file, line);

	lcd_clear(BLACK);
	
	GUI_DispStr20At(250,150,RED,COLOR_BACK,"Assert Error!");
	
	GUI_DispStr20At(100,180,COLOR_POINT,COLOR_BACK,"========================================");
	
	usprintf(showBuff,"file: %s",file);
	GUI_DispStr20At(100,210,COLOR_POINT,COLOR_BACK,showBuff);
	
	usprintf(showBuff,"line: %d",line);
	GUI_DispStr20At(100,230,COLOR_POINT,COLOR_BACK,showBuff);	
	
	GUI_DispStr20At(100,260,COLOR_POINT,COLOR_BACK,"========================================");
	
	/* Infinite loop */
	while (1)
	{
	}
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
