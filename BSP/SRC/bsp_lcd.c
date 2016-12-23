/*
*********************************************************************************************************
* @file    	bsp_lcd.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-19 12:44:36
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	LCD驱动源文件
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
#include "bsp.h"


/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/


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
/* 下面3个变量，主要用于使程序同时支持不同的屏 */
uint16_t g_ChipID = IC_8875;		/* 驱动芯片ID */
uint16_t g_LcdHeight = 480;			/* 显示屏分辨率-高度 */
uint16_t g_LcdWidth = 800;			/* 显示屏分辨率-宽度 */
uint8_t s_ucBright;					/* 背光亮度参数 */
uint8_t g_LcdDirection;				/* 显示方向.0，1，2，3 */

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
* Function Name : bsp_InitLCD
* Description	: 初始化LCD
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_InitLCD( void )
{
	RA8875_InitHard();	
	LCD_ClrScr(CL_BLACK);				/* 清屏，显示全黑 */
	LCD_SetBackLight(BRIGHT_DEFAULT);	 /* 打开背光，设置为缺省亮度 */
	
	ECHO(DEBUG_BSP_INIT, "LCD初始化 .......... OK");
}

/*
*********************************************************************************************************
*	函 数 名: LCD_GetHeight
*	功能说明: 读取LCD分辨率之高度
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint16_t LCD_GetHeight(void)
{
	return g_LcdHeight;
}

/*
*********************************************************************************************************
*	函 数 名: LCD_GetWidth
*	功能说明: 读取LCD分辨率之宽度
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint16_t LCD_GetWidth(void)
{
	return g_LcdWidth;
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DispOn
*	功能说明: 打开显示
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DispOn(void)
{
	RA8875_DispOn();
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DispOff
*	功能说明: 关闭显示
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DispOff(void)
{
	RA8875_DispOff();
}

/*
*********************************************************************************************************
*	函 数 名: LCD_ClrScr
*	功能说明: 根据输入的颜色值清屏
*	形    参: _usColor : 背景色
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_ClrScr(uint16_t _usColor)
{
	RA8875_ClrScr(_usColor);
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DispStr
*	功能说明: 在LCD指定坐标（左上角）显示一个字符串
*	形    参:
*		_usX : X坐标
*		_usY : Y坐标
*		_ptr  : 字符串指针
*		_tFont : 字体结构体，包含颜色、背景色(支持透明)、字体代码、文字间距等参数
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont)
{
	LCD_DispStrEx(_usX, _usY, _ptr, _tFont, 0, 0);
}

/*
*********************************************************************************************************
*	函 数 名: LCD_GetFontWidth
*	功能说明: 读取字体的宽度（像素单位)
*	形    参:
*		_tFont : 字体结构体，包含颜色、背景色(支持透明)、字体代码、文字间距等参数
*	返 回 值: 字体的宽度（像素单位)
*********************************************************************************************************
*/
uint16_t LCD_GetFontWidth(FONT_T *_tFont)
{
	uint16_t font_width = 16;

	switch (_tFont->FontCode)
	{
		case FC_ST_12:
			font_width = 12;
			break;

		case FC_ST_16:
		case FC_RA8875_16:			
			font_width = 16;
			break;
			
		case FC_RA8875_24:			
		case FC_ST_24:
			font_width = 24;
			break;
			
		case FC_ST_32:
		case FC_RA8875_32:	
			font_width = 32;
			break;			
	}
	return font_width;
}

/*
*********************************************************************************************************
*	函 数 名: LCD_GetFontHeight
*	功能说明: 读取字体的高度（像素单位)
*	形    参:
*		_tFont : 字体结构体，包含颜色、背景色(支持透明)、字体代码、文字间距等参数
*	返 回 值: 字体的宽度（像素单位)
*********************************************************************************************************
*/
uint16_t LCD_GetFontHeight(FONT_T *_tFont)
{
	uint16_t height = 16;

	switch (_tFont->FontCode)
	{
		case FC_ST_12:
			height = 12;
			break;

		case FC_ST_16:
		case FC_RA8875_16:			
			height = 16;
			break;
			
		case FC_RA8875_24:			
		case FC_ST_24:
			height = 24;
			break;
			
		case FC_ST_32:
		case FC_RA8875_32:	
			height = 32;
			break;			
	}
	return height;
}

/*
*********************************************************************************************************
*	函 数 名: LCD_GetStrWidth
*	功能说明: 计算字符串宽度(像素单位)
*	形    参:
*		_ptr  : 字符串指针
*		_tFont : 字体结构体，包含颜色、背景色(支持透明)、字体代码、文字间距等参数
*	返 回 值: 无
*********************************************************************************************************
*/
uint16_t LCD_GetStrWidth(char *_ptr, FONT_T *_tFont)
{
	char *p = _ptr;
	uint16_t width = 0;
	uint8_t code1, code2;
	uint16_t font_width;

	font_width = LCD_GetFontWidth(_tFont);

	while (*p != 0)
	{
		code1 = *p;	/* 读取字符串数据， 该数据可能是ascii代码，也可能汉字代码的高字节 */
		if (code1 < 0x80)	/* ASCII */
		{
			switch(_tFont->FontCode)
			{
				case FC_RA8875_16:
					font_width = 8;
					break;
				
				case FC_RA8875_24:
					font_width = 12;
					break;
				
				case FC_RA8875_32:
					font_width = 16;
					break;
				
				case FC_ST_12:
					font_width = 6;
					break;

				case FC_ST_16:		
					font_width = 8;
					break;
					
				case FC_ST_24:			
					font_width = 12;
					break;
					
				case FC_ST_32:
					font_width = 16;
					break;
				
				default:
					font_width = 8;
					break;					
			}
			
		}
		else	/* 汉字 */
		{
			code2 = *++p;
			if (code2 == 0)
			{
				break;
			}
			font_width = LCD_GetFontWidth(_tFont);
			
		}
		width += font_width;
		p++;
	}

	return width;
}

/*
*********************************************************************************************************
*	函 数 名: _LCD_ReadAsciiDot
*	功能说明: 读取1个ASCII字符的点阵数据
*	形    参:
*		_code : ASCII字符的编码，1字节。1-128
*		_fontcode ：字体代码
*		_pBuf : 存放读出的字符点阵数据
*	返 回 值: 文字宽度
*********************************************************************************************************
*/
static void _LCD_ReadAsciiDot(uint8_t _code, uint8_t _fontcode, uint8_t *_pBuf)
{
	const uint8_t *pAscDot;
	uint8_t font_bytes = 0;

	pAscDot = 0;
	switch (_fontcode)
	{
		case FC_ST_12:		/* 12点阵 */
			font_bytes = 24;
			pAscDot = g_Ascii12;	
			break;
		
		case FC_ST_24:
		case FC_ST_32:
		case FC_ST_16:
			/* 缺省是16点阵 */
			font_bytes = 32;
			pAscDot = g_Ascii16;
			break;
		
		case FC_RA8875_16:
		case FC_RA8875_24:
		case FC_RA8875_32:
			return;
	}	

	/* 将CPU内部Flash中的ascii字符点阵复制到buf */
	memcpy(_pBuf, &pAscDot[_code * (font_bytes / 2)], (font_bytes / 2));	
}

/*
*********************************************************************************************************
*	函 数 名: _LCD_ReadHZDot
*	功能说明: 读取1个汉字的点阵数据
*	形    参:
*		_code1, _cod2 : 汉字内码. GB2312编码
*		_fontcode ：字体代码
*		_pBuf : 存放读出的字符点阵数据
*	返 回 值: 无
*********************************************************************************************************
*/
static void _LCD_ReadHZDot(uint8_t _code1, uint8_t _code2,  uint8_t _fontcode, uint8_t *_pBuf)
{
	#ifdef USE_SMALL_FONT	/* 使用CPU 内部Flash 小字库 */
		uint8_t *pDot;
		uint8_t font_bytes = 0;
		uint32_t address;
		uint16_t m;

		pDot = 0;	/* 仅仅用于避免告警 */
		switch (_fontcode)
		{
			case FC_ST_12:		/* 12点阵 */
				font_bytes = 24;
				pDot = (uint8_t *)NULL;	
				break;
			
			case FC_ST_16:
				font_bytes = 32;
				pDot = (uint8_t *)NULL;
				break;
	
			case FC_ST_24:
				font_bytes = 72;
				pDot = (uint8_t *)NULL;
				break;			
				
			case FC_ST_32:	
				font_bytes = 128;
				pDot = (uint8_t *)NULL;
				break;						
			
			case FC_RA8875_16:
			case FC_RA8875_24:
			case FC_RA8875_32:
				return;
		}	

		m = 0;
		while(1)
		{
			address = m * (font_bytes + 2);
			m++;
			if ((_code1 == pDot[address + 0]) && (_code2 == pDot[address + 1]))
			{
				address += 2;
				memcpy(_pBuf, &pDot[address], font_bytes);
				break;
			}
			else if ((pDot[address + 0] == 0xFF) && (pDot[address + 1] == 0xFF))
			{
				/* 字库搜索完毕，未找到，则填充全FF */
				memset(_pBuf, 0xFF, font_bytes);
				break;
			}
		}
	#else	/* 用全字库 */
		uint8_t *pDot = 0;
		uint8_t font_bytes = 0;
			
		switch (_fontcode)
		{
			case FC_ST_12:		/* 12点阵 */
				font_bytes = 24;
				pDot = (uint8_t *)NULL;	
				break;
			
			case FC_ST_16:
				font_bytes = 32;
				pDot = (uint8_t *)NULL;
				break;
	
			case FC_ST_24:
				font_bytes = 72;
				pDot = (uint8_t *)NULL;
				break;			
				
			case FC_ST_32:	
				font_bytes = 128;
				pDot = (uint8_t *)NULL;
				break;						
			
			case FC_RA8875_16:
			case FC_RA8875_24:
			case FC_RA8875_32:
				return;
		}			
	
		/* 此处需要根据字库文件存放位置进行修改 */
		if (_code1 >=0xA1 && _code1 <= 0xA9 && _code2 >=0xA1)
		{
			pDot += ((_code1 - 0xA1) * 94 + (_code2 - 0xA1)) * font_bytes;
		}
		else if (_code1 >=0xB0 && _code1 <= 0xF7 && _code2 >=0xA1)
		{
			pDot += ((_code1 - 0xB0) * 94 + (_code2 - 0xA1) + 846) * font_bytes;
		}
		memcpy(_pBuf, pDot, font_bytes);
	#endif
}
			
/*
*********************************************************************************************************
*	函 数 名: LCD_DispStrEx
*	功能说明: 在LCD指定坐标（左上角）显示一个字符串。 增强型函数。支持左\中\右对齐，支持定长清屏。
*	形    参:
*		_usX : X坐标
*		_usY : Y坐标
*		_ptr  : 字符串指针
*		_tFont : 字体结构体，包含颜色、背景色(支持透明)、字体代码、文字间距等参数。可以指定RA8875字库显示汉字
*		_Width : 字符串显示区域的宽度. 0 表示不处理留白区域，此时_Align无效
*		_Align :字符串在显示区域的对齐方式，
*				ALIGN_LEFT = 0,
*				ALIGN_CENTER = 1,
*				ALIGN_RIGHT = 2
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DispStrEx(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont, uint16_t _Width,
	uint8_t _Align)
{
	uint32_t i;
	uint8_t code1;
	uint8_t code2;
	uint8_t buf[32 * 32 / 8];	/* 最大支持24点阵汉字 */
	uint8_t width;
	uint16_t m;
	uint8_t font_width = 0;
	uint8_t font_height = 0;
	uint16_t x, y;
	uint16_t offset;
	uint16_t str_width;	/* 字符串实际宽度  */
	uint8_t ra8875_use = 0;
	uint8_t ra8875_font_code = 0;

	switch (_tFont->FontCode)
	{
		case FC_ST_12:		/* 12点阵 */
			font_height = 12;
			font_width = 12;
			break;
		
		case FC_ST_16:
			font_height = 16;
			font_width = 16;
			break;

		case FC_ST_24:
			font_height = 24;
			font_width = 24;
			break;
						
		case FC_ST_32:	
			font_height = 32;
			font_width = 32;
			break;					
		
		case FC_RA8875_16:
			ra8875_font_code = RA_FONT_16;
			ra8875_use = 1;	/* 表示用RA8875字库 */
			break;
			
		case FC_RA8875_24:
			ra8875_font_code = RA_FONT_24;
			ra8875_use = 1;	/* 表示用RA8875字库 */
			break;
						
		case FC_RA8875_32:
			ra8875_font_code = RA_FONT_32;
			ra8875_use = 1;	/* 表示用RA8875字库 */
			break;
	}
	
	str_width = LCD_GetStrWidth(_ptr, _tFont);	/* 计算字符串实际宽度(RA8875内部ASCII点阵宽度为变长 */
	offset = 0;
	if (_Width > str_width)
	{
		if (_Align == ALIGN_RIGHT)	/* 右对齐 */
		{
			offset = _Width - str_width;
		}
		else if (_Align == ALIGN_CENTER)	/* 左对齐 */
		{
			offset = (_Width - str_width) / 2;
		}
		else	/* 左对齐 ALIGN_LEFT */
		{
			;
		}
	}

	/* 左侧填背景色, 中间对齐和右边对齐  */
	if (offset > 0)
	{
		LCD_Fill_Rect(_usX, _usY, LCD_GetFontHeight(_tFont), offset,  _tFont->BackColor);
		_usX += offset;
	}
	
	/* 右侧填背景色 */
	if (_Width > str_width)
	{
		LCD_Fill_Rect(_usX + str_width, _usY, LCD_GetFontHeight(_tFont), _Width - str_width - offset,  _tFont->BackColor);
	}
	
	if (ra8875_use == 1)	/* 使用RA8875外挂的字库芯片 */
	{
		RA8875_SetFrontColor(_tFont->FrontColor);			/* 设置字体前景色 */
		RA8875_SetBackColor(_tFont->BackColor);				/* 设置字体背景色 */
		RA8875_SetFont(ra8875_font_code, 0, _tFont->Space);	/* 字体代码，行间距，字间距 */
		RA8875_DispStr(_usX, _usY, _ptr);
	}
	else	/* 使用CPU内部字库. 点阵信息由CPU读取 */
	{
		/* 开始循环处理字符 */
		while (*_ptr != 0)
		{
			code1 = *_ptr;	/* 读取字符串数据， 该数据可能是ascii代码，也可能汉字代码的高字节 */
			if (code1 < 0x80)
			{
				/* 将ascii字符点阵复制到buf */
				//memcpy(buf, &pAscDot[code1 * (font_bytes / 2)], (font_bytes / 2));
				_LCD_ReadAsciiDot(code1, _tFont->FontCode, buf);	/* 读取ASCII字符点阵 */
				width = font_width / 2;
			}
			else
			{
				code2 = *++_ptr;
				if (code2 == 0)
				{
					break;
				}
				/* 读1个汉字的点阵 */
				_LCD_ReadHZDot(code1, code2, _tFont->FontCode, buf);
				width = font_width;
			}
	
			y = _usY;
			/* 开始刷LCD */
			for (m = 0; m < font_height; m++)	/* 字符高度 */
			{
				x = _usX;
				for (i = 0; i < width; i++)	/* 字符宽度 */
				{
					if ((buf[m * ((2 * width) / font_width) + i / 8] & (0x80 >> (i % 8 ))) != 0x00)
					{
						LCD_PutPixel(x, y, _tFont->FrontColor);	/* 设置像素颜色为文字色 */
					}
					else
					{
						if (_tFont->BackColor != CL_MASK)	/* 透明色 */
						{
							LCD_PutPixel(x, y, _tFont->BackColor);	/* 设置像素颜色为文字背景色 */
						}
					}
	
					x++;
				}
				y++;
			}
	
			if (_tFont->Space > 0)
			{
				/* 如果文字底色按_tFont->usBackColor，并且字间距大于点阵的宽度，那么需要在文字之间填充(暂时未实现) */
			}
			_usX += width + _tFont->Space;	/* 列地址递增 */
			_ptr++;			/* 指向下一个字符 */
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: LCD_PutPixel
*	功能说明: 画1个像素
*	形    参:
*			_usX,_usY : 像素坐标
*			_usColor  : 像素颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor)
{
	RA8875_PutPixel(_usX, _usY, _usColor);
}

/*
*********************************************************************************************************
*	函 数 名: LCD_GetPixel
*	功能说明: 读取1个像素
*	形    参:
*			_usX,_usY : 像素坐标
*			_usColor  : 像素颜色
*	返 回 值: RGB颜色值
*********************************************************************************************************
*/
uint16_t LCD_GetPixel(uint16_t _usX, uint16_t _usY)
{
	return RA8875_GetPixel(_usX, _usY);
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawLine
*	功能说明: 采用 Bresenham 算法，在2点间画一条直线。
*	形    参:
*			_usX1, _usY1 : 起始点坐标
*			_usX2, _usY2 : 终止点Y坐标
*			_usColor     : 颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint16_t _usColor)
{
	RA8875_DrawLine(_usX1 , _usY1 , _usX2, _usY2 , _usColor);
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawPoints
*	功能说明: 采用 Bresenham 算法，绘制一组点，并将这些点连接起来。可用于波形显示。
*	形    参:
*			x, y     : 坐标数组
*			_usColor : 颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint16_t _usColor)
{
	uint16_t i;

	for (i = 0 ; i < _usSize - 1; i++)
	{
		LCD_DrawLine(x[i], y[i], x[i + 1], y[i + 1], _usColor);
	}
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawRect
*	功能说明: 绘制水平放置的矩形。
*	形    参:
*			_usX,_usY: 矩形左上角的坐标
*			_usHeight : 矩形的高度
*			_usWidth  : 矩形的宽度
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor)
{
	RA8875_DrawRect(_usX, _usY, _usHeight, _usWidth, _usColor);
}

/*
*********************************************************************************************************
*	函 数 名: LCD_Fill_Rect
*	功能说明: 用一个颜色值填充一个矩形。【emWin 中有同名函数 LCD_FillRect，因此加了下划线区分】
*	形    参:
*			_usX,_usY: 矩形左上角的坐标
*			_usHeight : 矩形的高度
*			_usWidth  : 矩形的宽度
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_Fill_Rect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor)
{
	RA8875_FillRect(_usX, _usY, _usHeight, _usWidth, _usColor);
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawCircle
*	功能说明: 绘制一个圆，笔宽为1个像素
*	形    参:
*			_usX,_usY  : 圆心的坐标
*			_usRadius  : 圆的半径
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor)
{
	RA8875_DrawCircle(_usX, _usY, _usRadius, _usColor);
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawBMP
*	功能说明: 在LCD上显示一个BMP位图，位图点阵扫描次序: 从左到右，从上到下
*	形    参:
*			_usX, _usY : 图片的坐标
*			_usHeight  : 图片高度
*			_usWidth   : 图片宽度
*			_ptr       : 图片点阵指针
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr)
{
	RA8875_DrawBMP(_usX, _usY, _usHeight, _usWidth, _ptr);
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawWin
*	功能说明: 在LCD上绘制一个窗口
*	形    参: 结构体指针
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawWin(WIN_T *_pWin)
{
#if 1
	uint16_t TitleHegiht;

	TitleHegiht = 20;

	/* 绘制窗口外框 */
	LCD_DrawRect(_pWin->Left, _pWin->Top, _pWin->Height, _pWin->Width, WIN_BORDER_COLOR);
	LCD_DrawRect(_pWin->Left + 1, _pWin->Top + 1, _pWin->Height - 2, _pWin->Width - 2, WIN_BORDER_COLOR);

	/* 窗口标题栏 */
	LCD_Fill_Rect(_pWin->Left + 2, _pWin->Top + 2, TitleHegiht, _pWin->Width - 4, WIN_TITLE_COLOR);

	/* 窗体填充 */
	LCD_Fill_Rect(_pWin->Left + 2, _pWin->Top + TitleHegiht + 2, _pWin->Height - 4 - TitleHegiht,
		_pWin->Width - 4, WIN_BODY_COLOR);

	LCD_DispStr(_pWin->Left + 3, _pWin->Top + 2, _pWin->pCaption, _pWin->Font);
#else
	if (g_ChipID == IC_8875)
	{
		uint16_t TitleHegiht;

		TitleHegiht = 28;

		/* 绘制窗口外框 */
		RA8875_DrawRect(_pWin->Left, _pWin->Top, _pWin->Height, _pWin->Width, WIN_BORDER_COLOR);
		RA8875_DrawRect(_pWin->Left + 1, _pWin->Top + 1, _pWin->Height - 2, _pWin->Width - 2, WIN_BORDER_COLOR);

		/* 窗口标题栏 */
		RA8875_FillRect(_pWin->Left + 2, _pWin->Top + 2, TitleHegiht, _pWin->Width - 4, WIN_TITLE_COLOR);

		/* 窗体填充 */
		RA8875_FillRect(_pWin->Left + 2, _pWin->Top + TitleHegiht + 2, _pWin->Height - 4 - TitleHegiht, _pWin->Width - 4, WIN_BODY_COLOR);

		//RA8875_SetFont(_pWin->Font.FontCode, 0, 0);
		RA8875_SetFont(RA_FONT_24, 0, 0);

		RA8875_SetBackColor(WIN_TITLE_COLOR);
		RA8875_SetFrontColor(WIN_CAPTION_COLOR);
		RA8875_DispStr(_pWin->Left + 3, _pWin->Top + 2, _pWin->Caption);
	}
	else
	{
		;
	}
#endif
}


/*
*********************************************************************************************************
*	函 数 名: LCD_DrawIcon
*	功能说明: 在LCD上绘制一个图标，四角自动切为弧脚
*	形    参: _pIcon : 图标结构
*			  _tFont : 字体属性
*			  _ucFocusMode : 焦点模式。0 表示正常图标  1表示选中的图标
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawIcon(const ICON_T *_tIcon, FONT_T *_tFont, uint8_t _ucFocusMode)
{
	const uint16_t *p;
	uint16_t usNewRGB;
	uint16_t x, y;		/* 用于记录窗口内的相对坐标 */

	p = _tIcon->pBmp;
	for (y = 0; y < _tIcon->Height; y++)
	{
		for (x = 0; x < _tIcon->Width; x++)
		{
			usNewRGB = *p++;	/* 读取图标的颜色值后指针加1 */
			/* 将图标的4个直角切割为弧角，弧角外是背景图标 */
			if ((y == 0 && (x < 6 || x > _tIcon->Width - 7)) ||
				(y == 1 && (x < 4 || x > _tIcon->Width - 5)) ||
				(y == 2 && (x < 3 || x > _tIcon->Width - 4)) ||
				(y == 3 && (x < 2 || x > _tIcon->Width - 3)) ||
				(y == 4 && (x < 1 || x > _tIcon->Width - 2)) ||
				(y == 5 && (x < 1 || x > _tIcon->Width - 2))	||

				(y == _tIcon->Height - 1 && (x < 6 || x > _tIcon->Width - 7)) ||
				(y == _tIcon->Height - 2 && (x < 4 || x > _tIcon->Width - 5)) ||
				(y == _tIcon->Height - 3 && (x < 3 || x > _tIcon->Width - 4)) ||
				(y == _tIcon->Height - 4 && (x < 2 || x > _tIcon->Width - 3)) ||
				(y == _tIcon->Height - 5 && (x < 1 || x > _tIcon->Width - 2)) ||
				(y == _tIcon->Height - 6 && (x < 1 || x > _tIcon->Width - 2))
				)
			{
				;
			}
			else
			{
				if (_ucFocusMode != 0)	/* 1表示选中的图标 */
				{
					/* 降低原始像素的亮度，实现图标被激活选中的效果 */
					uint16_t R,G,B;
					uint16_t bright = 15;

					/* rrrr rggg gggb bbbb */
					R = (usNewRGB & 0xF800) >> 11;
					G = (usNewRGB & 0x07E0) >> 5;
					B =  usNewRGB & 0x001F;
					if (R > bright)
					{
						R -= bright;
					}
					else
					{
						R = 0;
					}
					if (G > 2 * bright)
					{
						G -= 2 * bright;
					}
					else
					{
						G = 0;
					}
					if (B > bright)
					{
						B -= bright;
					}
					else
					{
						B = 0;
					}
					usNewRGB = (R << 11) + (G << 5) + B;
				}

				LCD_PutPixel(x + _tIcon->Left, y + _tIcon->Top, usNewRGB);
			}
		}
	}

	/* 绘制图标下的文字 */
	{
		uint16_t len;
		uint16_t width;

		len = strlen(_tIcon->Text);

		if  (len == 0)
		{
			return;	/* 如果图标文本长度为0，则不显示 */
		}

		/* 计算文本的总宽度 */
		if (_tFont->FontCode == FC_ST_12)		/* 12点阵 */
		{
			width = 6 * (len + _tFont->Space);
		}
		else	/* FC_ST_16 */
		{
			width = 8 * (len + _tFont->Space);
		}


		/* 水平居中 */
		x = (_tIcon->Left + _tIcon->Width / 2) - width / 2;
		y = _tIcon->Top + _tIcon->Height + 2;
		LCD_DispStr(x, y, (char *)_tIcon->Text, _tFont);
	}
}

/*
*********************************************************************************************************
*	函 数 名: LCD_Blend565
*	功能说明: 对像素透明化 颜色混合
*	形    参: src : 原始像素
*			  dst : 混合的颜色
*			  alpha : 透明度 0-32
*	返 回 值: 无
*********************************************************************************************************
*/
uint16_t LCD_Blend565(uint16_t src, uint16_t dst, uint8_t alpha)
{
	uint32_t src2;
	uint32_t dst2;

	src2 = ((src << 16) |src) & 0x07E0F81F;
	dst2 = ((dst << 16) | dst) & 0x07E0F81F;
	dst2 = ((((dst2 - src2) * alpha) >> 5) + src2) & 0x07E0F81F;
	return (dst2 >> 16) | dst2;
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawIcon32
*	功能说明: 在LCD上绘制一个图标, 带有透明信息的位图(32位， RGBA). 图标下带文字
*	形    参: _pIcon : 图标结构
*			  _tFont : 字体属性
*			  _ucFocusMode : 焦点模式。0 表示正常图标  1表示选中的图标
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawIcon32(const ICON_T *_tIcon, FONT_T *_tFont, uint8_t _ucFocusMode)
{
	const uint8_t *p;
	uint16_t usOldRGB, usNewRGB;
	int16_t x, y;		/* 用于记录窗口内的相对坐标 */
	uint8_t R1,G1,B1,A;	/* 新像素色彩分量 */
	uint8_t R0,G0,B0;	/* 旧像素色彩分量 */

	p = (const uint8_t *)_tIcon->pBmp;
	p += 54;		/* 直接指向图像数据区 */

	/* 按照BMP位图次序，从左至右，从上至下扫描 */
	for (y = _tIcon->Height - 1; y >= 0; y--)
	{
		for (x = 0; x < _tIcon->Width; x++)
		{
			B1 = *p++;
			G1 = *p++;
			R1 = *p++;
			A = *p++;	/* Alpha 值(透明度)，0-255, 0表示透明，1表示不透明, 中间值表示透明度 */

			if (A == 0x00)	/* 需要透明,显示背景 */
			{
				;	/* 不用刷新背景 */
			}
			else if (A == 0xFF)	/* 完全不透明， 显示新像素 */
			{
				usNewRGB = RGB(R1, G1, B1);
				if (_ucFocusMode == 1)
				{
					usNewRGB = LCD_Blend565(usNewRGB, CL_YELLOW, 10);
				}
				LCD_PutPixel(x + _tIcon->Left, y + _tIcon->Top, usNewRGB);
			}
			else 	/* 半透明 */
			{
				/* 计算公式： 实际显示颜色 = 前景颜色 * Alpha / 255 + 背景颜色 * (255-Alpha) / 255 */
				usOldRGB = LCD_GetPixel(x + _tIcon->Left, y + _tIcon->Top);
				//usOldRGB = 0xFFFF;
				R0 = RGB565_R(usOldRGB);
				G0 = RGB565_G(usOldRGB);
				B0 = RGB565_B(usOldRGB);

				R1 = (R1 * A) / 255 + R0 * (255 - A) / 255;
				G1 = (G1 * A) / 255 + G0 * (255 - A) / 255;
				B1 = (B1 * A) / 255 + B0 * (255 - A) / 255;
				usNewRGB = RGB(R1, G1, B1);
				if (_ucFocusMode == 1)
				{
					usNewRGB = LCD_Blend565(usNewRGB, CL_YELLOW, 10);
				}
				LCD_PutPixel(x + _tIcon->Left, y + _tIcon->Top, usNewRGB);
			}
		}
	}

	/* 绘制图标下的文字 */
	{
		uint16_t len;
		uint16_t width;

		len = strlen(_tIcon->Text);

		if  (len == 0)
		{
			return;	/* 如果图标文本长度为0，则不显示 */
		}

		/* 计算文本的总宽度 */
		if (_tFont->FontCode == FC_ST_12)		/* 12点阵 */
		{
			width = 6 * (len + _tFont->Space);
		}
		else	/* FC_ST_16 */
		{
			width = 8 * (len + _tFont->Space);
		}


		/* 水平居中 */
		x = (_tIcon->Left + _tIcon->Width / 2) - width / 2;
		y = _tIcon->Top + _tIcon->Height + 2;
		LCD_DispStr(x, y, (char *)_tIcon->Text, _tFont);
	}
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawBmp32
*	功能说明: 在LCD上绘制一个32位的BMP图, 带有透明信息的位图(32位， RGBA)
*	形    参: _usX, _usY : 显示坐标
*			  _usHeight, _usWidth : 图片高度和宽度
*			  _pBmp : 图片数据（带BMP文件头）
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawBmp32(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint8_t *_pBmp)
{
	const uint8_t *p;
	uint16_t usOldRGB, usNewRGB;
	int16_t x, y;		/* 用于记录窗口内的相对坐标 */
	uint8_t R1,G1,B1,A;	/* 新像素色彩分量 */
	uint8_t R0,G0,B0;	/* 旧像素色彩分量 */

	p = (const uint8_t *)_pBmp;
	p += 54;		/* 直接指向图像数据区 */

	/* 按照BMP位图次序，从左至右，从上至下扫描 */
	for (y = _usHeight - 1; y >= 0; y--)
	{
		for (x = 0; x < _usWidth; x++)
		{
			B1 = *p++;
			G1 = *p++;
			R1 = *p++;
			A = *p++;	/* Alpha 值(透明度)，0-255, 0表示透明，1表示不透明, 中间值表示透明度 */

			if (A == 0x00)	/* 需要透明,显示背景 */
			{
				;	/* 不用刷新背景 */
			}
			else if (A == 0xFF)	/* 完全不透明， 显示新像素 */
			{
				usNewRGB = RGB(R1, G1, B1);
				//if (_ucFocusMode == 1)
				//{
				//	usNewRGB = Blend565(usNewRGB, CL_YELLOW, 10);
				//}
				LCD_PutPixel(x + _usX, y + _usY, usNewRGB);
			}
			else 	/* 半透明 */
			{
				/* 计算公式： 实际显示颜色 = 前景颜色 * Alpha / 255 + 背景颜色 * (255-Alpha) / 255 */
				usOldRGB = LCD_GetPixel(x + _usX, y + _usY);
				R0 = RGB565_R(usOldRGB);
				G0 = RGB565_G(usOldRGB);
				B0 = RGB565_B(usOldRGB);

				R1 = (R1 * A) / 255 + R0 * (255 - A) / 255;
				G1 = (G1 * A) / 255 + G0 * (255 - A) / 255;
				B1 = (B1 * A) / 255 + B0 * (255 - A) / 255;
				usNewRGB = RGB(R1, G1, B1);
				//if (_ucFocusMode == 1)
				//{
				//	usNewRGB = Blend565(usNewRGB, CL_YELLOW, 10);
				//}
				LCD_PutPixel(x + _usX, y + _usY, usNewRGB);
			}
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawLabel
*	功能说明: 绘制一个文本标签
*	形    参: 结构体指针
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawLabel(LABEL_T *_pLabel)
{
#if 1
	char dispbuf[256];
	uint16_t i;
	uint16_t NewLen;

	NewLen = strlen(_pLabel->pCaption);

	if (NewLen > _pLabel->MaxLen)
	{
		LCD_DispStr(_pLabel->Left, _pLabel->Top, _pLabel->pCaption, _pLabel->Font);
		_pLabel->MaxLen = NewLen;
	}
	else
	{
		for (i = 0; i < NewLen; i++)
		{
			dispbuf[i] = _pLabel->pCaption[i];
		}
		for (; i < _pLabel->MaxLen; i++)
		{
			dispbuf[i] = ' ';		/* 末尾填充空格 */
		}
		dispbuf[i] = 0;
		LCD_DispStr(_pLabel->Left, _pLabel->Top, dispbuf, _pLabel->Font);
	}
#else
	if (g_ChipID == IC_8875)
	{
		RA8875_SetFont(_pLabel->Font->FontCode, 0, 0);	/* 设置32点阵字体，行间距=0，字间距=0 */

		RA8875_SetBackColor(_pLabel->Font->BackColor);
		RA8875_SetFrontColor(_pLabel->Font->FrontColor);

		RA8875_DispStr(_pLabel->Left, _pLabel->Top, _pLabel->Caption);
	}
	else
	{

	}
#endif
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawCheckBox
*	功能说明: 绘制一个检查框
*	形    参: 结构体指针
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawCheckBox(CHECK_T *_pCheckBox)
{
#if 1
	uint16_t x, y;

	/* 目前只做了16点阵汉字的大小 */

	/* 绘制外框 */
	x = _pCheckBox->Left;
	LCD_DrawRect(x, _pCheckBox->Top, CHECK_BOX_H, CHECK_BOX_W, CHECK_BOX_BORDER_COLOR);
	LCD_DrawRect(x + 1, _pCheckBox->Top + 1, CHECK_BOX_H - 2, CHECK_BOX_W - 2, CHECK_BOX_BORDER_COLOR);
	LCD_Fill_Rect(x + 2, _pCheckBox->Top + 2, CHECK_BOX_H - 4, CHECK_BOX_W - 4, CHECK_BOX_BACK_COLOR);

	/* 绘制文本标签 */
	x = _pCheckBox->Left + CHECK_BOX_W + 2;
	y = _pCheckBox->Top + CHECK_BOX_H / 2 - 8;
	LCD_DispStr(x, y, _pCheckBox->pCaption, _pCheckBox->Font);

	if (_pCheckBox->Checked)
	{
		FONT_T font;

	    font.FontCode = FC_ST_16;
		font.BackColor = CL_MASK;
		font.FrontColor = CHECK_BOX_CHECKED_COLOR;	/* 钩的颜色 */
		font.Space = 0;
		x = _pCheckBox->Left;
		LCD_DispStr(x + 3, _pCheckBox->Top + 3, "√", &font);
	}
#else
	if (g_ChipID == IC_8875)
	{
		uint16_t x;

		RA8875_SetFont(_pCheckBox->Font.FontCode, 0, 0);	/* 设置32点阵字体，行间距=0，字间距=0 */

		/* 绘制标签 */
		//RA8875_SetBackColor(_pCheckBox->Font.BackColor);
		RA8875_SetBackColor(WIN_BODY_COLOR);
		RA8875_SetFrontColor(_pCheckBox->Font.FrontColor);
		RA8875_DispStr(_pCheckBox->Left, _pCheckBox->Top, _pCheckBox->Caption);

		/* 绘制外框 */
		x = _pCheckBox->Left + _pCheckBox->Width - CHECK_BOX_W;
		RA8875_DrawRect(x, _pCheckBox->Top, CHECK_BOX_H, CHECK_BOX_W, CHECK_BOX_BORDER_COLOR);
		RA8875_DrawRect(x + 1, _pCheckBox->Top + 1, CHECK_BOX_H - 2, CHECK_BOX_W - 2, CHECK_BOX_BORDER_COLOR);
		RA8875_FillRect(x + 2, _pCheckBox->Top + 2, CHECK_BOX_H - 4, CHECK_BOX_W - 4, CHECK_BOX_BACK_COLOR);

		if (_pCheckBox->Checked)
		{
			RA8875_SetBackColor(CHECK_BOX_BACK_COLOR);
			RA8875_SetFrontColor(CL_RED);
			RA8875_DispStr(x + 3, _pCheckBox->Top + 3, "√");
		}
	}
	else
	{

	}
#endif

}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawEdit
*	功能说明: 在LCD上绘制一个编辑框
*	形    参: _pEdit 编辑框结构体指针
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawEdit(EDIT_T *_pEdit)
{
#if 1
	uint16_t len, x, y;
	uint8_t width;

	/* 仿XP风格，平面编辑框 */
	LCD_DrawRect(_pEdit->Left, _pEdit->Top, _pEdit->Height, _pEdit->Width, EDIT_BORDER_COLOR);
	LCD_Fill_Rect(_pEdit->Left + 1, _pEdit->Top + 1, _pEdit->Height - 2, _pEdit->Width - 2, EDIT_BACK_COLOR);

	/* 文字居中 */
	if (_pEdit->Font->FontCode == FC_ST_12)
	{
		width = 6;
	}
	else
	{
		width = 8;
	}
	len = strlen(_pEdit->pCaption);
	x = _pEdit->Left +  (_pEdit->Width - len * width) / 2;
	y = _pEdit->Top + _pEdit->Height / 2 - width;

	LCD_DispStr(x, y, _pEdit->pCaption, _pEdit->Font);
#else
	if (g_ChipID == IC_8875)
	{
		uint16_t len, x;

		/* 仿XP风格，平面编辑框 */
		RA8875_DrawRect(_pEdit->Left, _pEdit->Top, _pEdit->Height, _pEdit->Width, EDIT_BORDER_COLOR);
		RA8875_FillRect(_pEdit->Left + 1, _pEdit->Top + 1, _pEdit->Height - 2, _pEdit->Width - 2, EDIT_BACK_COLOR);

		RA8875_SetFont(_pEdit->Font.FontCode, 0, 0);	/* 设置32点阵字体，行间距=0，字间距=0 */
		RA8875_SetFrontColor(_pEdit->Font.FrontColor);
		RA8875_SetBackColor(EDIT_BACK_COLOR);

		/* 文字居中 */
		len = strlen(_pEdit->Caption);
		x = (_pEdit->Width - len * 16) / 2;

		RA8875_DispStr(_pEdit->Left + x, _pEdit->Top + 3, _pEdit->Caption);
	}
	else
	{
		//SPFD5420_DrawCircle(_usX, _usY, _usRadius, _usColor);
	}
#endif
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawEdit
*	功能说明: 在LCD上绘制一个编辑框
*	形    参:
*			_usX, _usY : 图片的坐标
*			_usHeight  : 图片高度
*			_usWidth   : 图片宽度
*			_ptr       : 图片点阵指针
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawButton(BUTTON_T *_pBtn)
{
#if 1
	uint16_t x, y, h;
	FONT_T font;	/* 按钮激活时，需要更改字体设置，因此需要一个变量来保存 */

	font.FontCode = _pBtn->Font->FontCode;
	font.FrontColor = _pBtn->Font->FrontColor;
	font.BackColor = _pBtn->Font->BackColor;
	font.Space = _pBtn->Font->Space;	
			
	if (_pBtn->Focus == 1)
	{
		font.BackColor = BUTTON_ACTIVE_COLOR;
	}
	else
	{
		/* 按钮的背景色统一管理，不能单独指定 */
		font.BackColor = BUTTON_BACK_COLOR;
	}
	
	/* 仿XP风格，平面编辑框 */
	LCD_DrawRect(_pBtn->Left, _pBtn->Top, _pBtn->Height, _pBtn->Width, BUTTON_BORDER_COLOR);
	LCD_DrawRect(_pBtn->Left + 1, _pBtn->Top + 1, _pBtn->Height - 2, _pBtn->Width - 2, BUTTON_BORDER1_COLOR);
	LCD_DrawRect(_pBtn->Left + 2, _pBtn->Top + 2, _pBtn->Height - 4, _pBtn->Width - 4, BUTTON_BORDER2_COLOR);

	h =  LCD_GetFontHeight(&font);
	x = _pBtn->Left + 3;
	y = _pBtn->Top + _pBtn->Height / 2 - h / 2;		
	
	LCD_Fill_Rect(_pBtn->Left + 3, _pBtn->Top + 3, _pBtn->Height - 6, _pBtn->Width - 6, font.BackColor);	/* 选中后的底色 */
	LCD_DispStrEx(x, y, _pBtn->pCaption, &font, _pBtn->Width - 6, ALIGN_CENTER);	/* 水平居中 */		

#else
	if (g_ChipID == IC_8875)
	{
		uint16_t len, x, y;

		if (_pBtn->Focus == 1)
		{
			/* 仿XP风格，平面编辑框 */
			RA8875_DrawRect(_pBtn->Left, _pBtn->Top, _pBtn->Height, _pBtn->Width, BUTTON_BORDER_COLOR);
			RA8875_DrawRect(_pBtn->Left + 1, _pBtn->Top + 1, _pBtn->Height - 2, _pBtn->Width - 2, BUTTON_BORDER1_COLOR);
			RA8875_DrawRect(_pBtn->Left + 2, _pBtn->Top + 2, _pBtn->Height - 4, _pBtn->Width - 4, BUTTON_BORDER2_COLOR);

			RA8875_FillRect(_pBtn->Left + 3, _pBtn->Top + 3, _pBtn->Height - 6, _pBtn->Width - 6, BUTTON_ACTIVE_COLOR);

			RA8875_SetBackColor(BUTTON_ACTIVE_COLOR);
		}
		else
		{
			/* 仿XP风格，平面编辑框 */
			RA8875_DrawRect(_pBtn->Left, _pBtn->Top, _pBtn->Height, _pBtn->Width, BUTTON_BORDER_COLOR);
			RA8875_DrawRect(_pBtn->Left + 1, _pBtn->Top + 1, _pBtn->Height - 2, _pBtn->Width - 2, BUTTON_BORDER1_COLOR);
			RA8875_DrawRect(_pBtn->Left + 2, _pBtn->Top + 2, _pBtn->Height - 4, _pBtn->Width - 4, BUTTON_BORDER2_COLOR);

			RA8875_FillRect(_pBtn->Left + 3, _pBtn->Top + 3, _pBtn->Height - 6, _pBtn->Width - 6, BUTTON_BACK_COLOR);

			RA8875_SetBackColor(BUTTON_BACK_COLOR);
		}

		#if 1	/* 按钮文字字体和颜色固定 */
			if (strcmp(_pBtn->Caption, "←") == 0)	/* 退格键特殊处理 */
			{
				/* 退格键符号是单像素笔画，太细了不协调，因此特殊处理 */
				RA8875_SetFont(RA_FONT_16, 0, 0);
				RA8875_SetFrontColor(CL_BLACK);
				RA8875_SetTextZoom(RA_SIZE_X2, RA_SIZE_X2);	/* 放大2倍 */
			}
			else
			{
				RA8875_SetFont(RA_FONT_16, 0, 0);
				RA8875_SetFrontColor(CL_BLACK);
				RA8875_SetTextZoom(RA_SIZE_X1, RA_SIZE_X1);	/* 放大1倍 */
			}
		#else	/* 按钮文字字体和颜色有应用程序指定 */
			RA8875_SetFont(_pBtn->Font.FontCode, 0, 0);
			RA8875_SetFrontColor(_pBtn->Font.FrontColor);
		#endif

		/* 文字居中 */
		len = strlen(_pBtn->Caption);

		/* 此处统计不等宽字符有问题。暂时特殊处理下 */
		if (len != 3)
		{
			x = _pBtn->Left + (_pBtn->Width - len * 16) / 2;
		}
		else
		{
			x = _pBtn->Left + (_pBtn->Width - len * 20) / 2;
		}

		/* 对特殊字符特殊处理 */
		if ((len == 1) && (_pBtn->Caption[0] == '.'))
		{
			y = _pBtn->Top + 3;
			x += 3;
		}
		else
		{
			y = _pBtn->Top + 3;
		}

		RA8875_DispStr(x, y, _pBtn->Caption);

		RA8875_SetTextZoom(RA_SIZE_X1, RA_SIZE_X1);	/* 还原放大1倍 */
	}
#endif
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawGroupBox
*	功能说明: 在LCD上绘制一个分组框
*	形    参: _pBox 分组框
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawGroupBox(GROUP_T *_pBox)
{
	uint16_t x, y;

	/* 画阴影线 */
	LCD_DrawRect(_pBox->Left + 1, _pBox->Top + 5, _pBox->Height, _pBox->Width - 1, CL_BOX_BORDER2);

	/* 画主框线 */
	LCD_DrawRect(_pBox->Left, _pBox->Top + 4, _pBox->Height, _pBox->Width - 1, CL_BOX_BORDER1);

	/* 显示分组框标题（文字在左上角） */
	x = _pBox->Left + 9;
	y = _pBox->Top;
	LCD_DispStr(x, y, _pBox->pCaption, _pBox->Font);
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DispControl
*	功能说明: 绘制控件
*	形    参: _pControl 控件指针
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DispControl(void *_pControl)
{
	uint8_t id;

	id = *(uint8_t *)_pControl;	/* 读取ID */

	switch (id)
	{
		case ID_ICON:
			//void LCD_DrawIcon(const ICON_T *_tIcon, FONT_T *_tFont, uint8_t _ucFocusMode);
			break;

		case ID_WIN:
			LCD_DrawWin((WIN_T *)_pControl);
			break;

		case ID_LABEL:
			LCD_DrawLabel((LABEL_T *)_pControl);
			break;

		case ID_BUTTON:
			LCD_DrawButton((BUTTON_T *)_pControl);
			break;

		case ID_CHECK:
			LCD_DrawCheckBox((CHECK_T *)_pControl);
			break;

		case ID_EDIT:
			LCD_DrawEdit((EDIT_T *)_pControl);
			break;

		case ID_GROUP:
			LCD_DrawGroupBox((GROUP_T *)_pControl);
			break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: LCD_SetBackLight
*	功能说明: 初始化控制LCD背景光的GPIO,配置为PWM模式。
*			当关闭背光时，将CPU IO设置为浮动输入模式（推荐设置为推挽输出，并驱动到低电平)；将TIM3关闭 省电
*	形    参: _bright 亮度，0是灭，255是最亮
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_SetBackLight(uint8_t _bright)
{
	s_ucBright =  _bright;	/* 保存背光值 */

	RA8875_SetBackLight(_bright);
}

/*
*********************************************************************************************************
*	函 数 名: LCD_GetBackLight
*	功能说明: 获得背光亮度参数
*	形    参: 无
*	返 回 值: 背光亮度参数
*********************************************************************************************************
*/
uint8_t LCD_GetBackLight(void)
{
	return s_ucBright;
}
/*
*********************************************************************************************************
*	函 数 名: LCD_SetDirection
*	功能说明: 设置显示屏显示方向（横屏 竖屏）
*	形    参: 显示方向代码 0 横屏正常, 1=横屏180度翻转, 2=竖屏, 3=竖屏180度翻转
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_SetDirection(uint8_t _dir)
{
	g_LcdDirection =  _dir;		/* 保存在全局变量 */

	RA8875_SetDirection(_dir);
}


/*
*********************************************************************************************************
*	函 数 名: LCD_ButtonTouchDown
*	功能说明: 判断按钮是否被按下. 检查触摸坐标是否在按钮的范围之内。并重绘按钮。
*	形    参:  _btn : 按钮对象
*			  _usX, _usY: 触摸坐标
*	返 回 值: 1 表示在范围内
*********************************************************************************************************
*/
uint8_t LCD_ButtonTouchDown(BUTTON_T *_btn, uint16_t _usX, uint16_t _usY)
{
	if ((_usX > _btn->Left) && (_usX < _btn->Left + _btn->Width)
		&& (_usY > _btn->Top) && (_usY < _btn->Top + _btn->Height))
	{
//		BUTTON_BEEP();	/* 按键提示音 bsp_tft_lcd.h 文件开头可以使能和关闭 */
		_btn->Focus = 1;
		LCD_DrawButton(_btn);
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: LCD_ButtonTouchRelease
*	功能说明: 判断按钮是否被触摸释放. 并重绘按钮。在触摸释放事件中被调用。
*	形    参:  _btn : 按钮对象
*			  _usX, _usY: 触摸坐标
*	返 回 值: 1 表示在范围内
*********************************************************************************************************
*/
uint8_t LCD_ButtonTouchRelease(BUTTON_T *_btn, uint16_t _usX, uint16_t _usY)
{
	_btn->Focus = 0;
	LCD_DrawButton(_btn);

	if ((_usX > _btn->Left) && (_usX < _btn->Left + _btn->Width)
		&& (_usY > _btn->Top) && (_usY < _btn->Top + _btn->Height))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: LCD_InitButton
*	功能说明: 初始化按钮结构体成员。
*	形    参:  _x, _y : 坐标
*			  _h, _w : 高度和宽度
*			  _pCaption : 按钮文字
*			  _pFont : 按钮字体
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_InitButton(BUTTON_T *_btn, uint16_t _x, uint16_t _y, uint16_t _h, uint16_t _w, char *_pCaption, FONT_T *_pFont)
{
	_btn->Left = _x;
	_btn->Top = _y;
	_btn->Height = _h;
	_btn->Width = _w;
	_btn->pCaption = _pCaption;	
	_btn->Font = _pFont;
	_btn->Focus = 0;
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
