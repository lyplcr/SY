/*
*********************************************************************************************************
* @file    	bsp_ra8875.h
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-20 09:53:57
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	RA8875驱动头文件
*********************************************************************************************************
* @attention
*
* 
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Define to prevent recursive inclusion
*********************************************************************************************************
*/
#ifndef __BSP_RA8875_H
#define __BSP_RA8875_H

/*
*********************************************************************************************************
*                              				Exported Includes
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                              				Exported define
*********************************************************************************************************
*/
#define	RA_HARD_8080_16_EN
#define	RA_HARD_SPI_EN

#define RA8875_BASE		((uint32_t)(0x60000000 | 0x00000000))
#define RA8875_RAM_ADDR	RA8875_BASE
#define RA8875_REG		*(__IO uint16_t *)(RA8875_BASE +  (1 << (16 + 1)))	/* FSMC 16位总线模式下，FSMC_A16口线对应物理地址A17 */
#define RA8875_RAM		*(__IO uint16_t *)(RA8875_BASE)

#ifdef RA_HARD_SPI_EN	/* 硬件 SPI 界面 (需要改变RA8875屏上的2个电阻位置) */
	/*
	【1】安富莱STM32-V5开发板,STM32F407IGT6
		PD3/LCD_BUSY		--- 触摸芯片忙       （RA8875屏是RA8875芯片的忙信号)
		PF6/LCD_PWM			--- LCD背光PWM控制  （RA8875屏无需此脚，背光由RA8875控制)

		PI10/TP_NCS			--- 触摸芯片的片选		(RA8875屏无需SPI接口触摸芯片）
		PB3/SPI3_SCK		--- 触摸芯片SPI时钟		(RA8875屏无需SPI接口触摸芯片）
		PB4/SPI3_MISO		--- 触摸芯片SPI数据线MISO(RA8875屏无需SPI接口触摸芯片）
		PB5/SPI3_MOSI		--- 触摸芯片SPI数据线MOSI(RA8875屏无需SPI接口触摸芯片）

		PC3/TP_INT			--- 触摸芯片中断 （对于RA8875屏，是RA8875输出的中断)
	*/
	#define RA8875_CS_0()	( (void)0 )
	#define RA8875_CS_1()	( (void)0 )

	#define SPI_WRITE_DATA	0x00
	#define SPI_READ_DATA	0x40
	#define SPI_WRITE_CMD	0x80
	#define SPI_READ_STATUS	0xC0

	static uint8_t SPI_ShiftByte(uint8_t _ucByte);
#endif

	
/*
*********************************************************************************************************
*                              				Exported types
*********************************************************************************************************
*/
/* 定义RA8875的访问接口类别. V5程序仅支持 RA_HARD_SPI 和 RA_HARD_8080_16 */
enum
{
	RA_SOFT_8080_8 = 0,	/* 软件8080接口模式, 8bit */
	RA_SOFT_SPI,	   	/* 软件SPI接口模式 */
	RA_HARD_SPI,	   	/* 硬件SPI接口模式 */
	RA_HARD_8080_16		/* 硬件8080接口,16bit */
};

enum
{
	RA_FONT_16 = 0,		/* RA8875 字体 16点阵 */
	RA_FONT_24 = 1,		/* RA8875 字体 24点阵 */
	RA_FONT_32 = 2		/* RA8875 字体 32点阵 */
};

/* 文字放大参数 */
enum
{
	RA_SIZE_X1 = 0,		/* 原始大小 */
	RA_SIZE_X2 = 1,		/* 放大2倍 */
	RA_SIZE_X3 = 2,		/* 放大3倍 */
	RA_SIZE_X4 = 3		/* 放大4倍 */
};

/*
*********************************************************************************************************
*                              				Exported constants
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Exported macro
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Exported variables
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Exported functions
*********************************************************************************************************
*/
void RA8875_Delaly1us(void);
void RA8875_Delaly1ms(void);
uint16_t RA8875_ReadID(void);
void RA8875_WriteCmd(uint8_t _ucRegAddr);
void RA8875_WriteData(uint8_t _ucRegValue);
uint8_t RA8875_ReadData(void);
void RA8875_WriteData16(uint16_t _usRGB);
uint16_t RA8875_ReadData16(void);
uint8_t RA8875_ReadStatus(void);
uint32_t RA8875_GetDispMemAddr(void);
uint8_t RA8875_ReadBusy(void);

/* 可供外部模块调用的函数 */
void RA8875_InitHard(void);
void RA8875_WriteReg(uint8_t _ucRegAddr, uint8_t _ucRegValue);
uint8_t RA8875_ReadReg(uint8_t _ucRegAddr);
uint16_t RA8875_ReadID(void);
uint8_t RA8875_IsBusy(void);
void RA8875_Layer1Visable(void);
void RA8875_Layer2Visable(void);
void RA8875_DispOn(void);
void RA8875_DispOff(void);
void RA8875_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor);
uint16_t RA8875_GetPixel(uint16_t _usX, uint16_t _usY);
void RA8875_SetFrontColor(uint16_t _usColor);
void RA8875_SetBackColor(uint16_t _usColor);
void RA8875_SetFont(uint8_t _ucFontType, uint8_t _ucLineSpace, uint8_t _ucCharSpace);
void RA8875_SetTextZoom(uint8_t _ucHSize, uint8_t _ucVSize);
void RA8875_DispAscii(uint16_t _usX, uint16_t _usY, char *_ptr);
void RA8875_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr);
void RA8875_ClrScr(uint16_t _usColor);
void RA8875_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr);
void RA8875_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint16_t _usColor);
void RA8875_DrawRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor);
void RA8875_FillRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor);
void RA8875_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor);
void RA8875_FillCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor);
void RA8875_SetDispWin(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth);
void RA8875_QuitWinMode(void);
void RA8875_CtrlGPO(uint8_t _pin, uint8_t _value);
void RA8875_SetBackLight(uint8_t _bright);

void RA8875_TouchInit(void);
uint16_t RA8875_TouchReadX(void);
uint16_t RA8875_TouchReadY(void);

void RA8875_InitSPI(void);
void RA8875_Sleep(void);

/* 下面3个函数用于直接写显存，比如摄像头直接DMA到显示器. 不支持SPI接口模式 */
void RA8875_StartDirectDraw(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth);
void RA8875_QuitDirectDraw(void);

void RA8875_SetDirection(uint8_t _ucDir);

/* 用于emWin的接口函数 */
void RA8875_PutPixelGUI(uint16_t _usX, uint16_t _usY, uint16_t _usColor);
uint16_t RA8875_GetPixelGUI(uint16_t _usX, uint16_t _usY);
void RA8875_DrawHLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usColor);
void RA8875_DrawHColorLine(uint16_t _usX1 , uint16_t _usY1, uint16_t _usWidth, uint16_t *_pColor);
void RA8875_DrawVLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usY2 , uint16_t _usColor);
void RA8875_RTERect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t _usColor); 


#endif
/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
