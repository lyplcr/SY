#include <stddef.h>           /* needed for definition of NULL */
#include "GUI_Private.h"
#include "ff.h"
#include "bsp.h"
#include "emWinUtils.h"

/* 字模数据的暂存数组,以单个字模的最大字节数为设定值 */
#define MAX_SUPPORT_FONT_SIZE  			32
#define BYTES_PER_FONT      (MAX_SUPPORT_FONT_SIZE * MAX_SUPPORT_FONT_SIZE / 8)		
static U8 GUI_FontDataBuf[BYTES_PER_FONT];

extern const char GUI_FontSongTi_ASCII_8X16[];
extern const char GUI_FontSongTi_ASCII_12X24[];
extern const char GUI_FontSongTi_ASCII_16X32[];

/*---------------------------------------------------------------------------*/
/*字库外部函数部分-----------------------------------------------------------*/
void GUI_X_GetFontData(char *font, U32 oft, U8 *ptr, U16 bytes)
{
	FIL fsrc;
	UINT bw;
	FRESULT ret;
	
    ret = f_open(&fsrc, font, FA_OPEN_EXISTING | FA_READ); 
    ret = f_lseek(&fsrc,oft);
    ret = f_read(&fsrc, ptr, bytes, &bw); 
    ret = f_close(&fsrc); 
	
	ret = ret;
}

static void GUI_GetDataFromMemory(const GUI_FONT_PROP GUI_UNI_PTR *pProp, U16P c)
{
	U16 BytesPerFont;
	U32 offset;
	uint8_t high,low;

	char *font = (char *)pProp->paCharInfo->pData;
	
	/* 每个字模的数据字节数 */
	BytesPerFont = GUI_pContext->pAFont->YSize * pProp->paCharInfo->BytesPerLine;
	if (BytesPerFont > BYTES_PER_FONT)
	{
		BytesPerFont = BYTES_PER_FONT;
	}
	
	bool isFlashFont = false; 
	uint32_t baseAddr = 0;
		
	/**
	 *	英文 16/24/32 	号使用内置FLASH字库
	 *	中文 16/24 	 	号使用外置FLASH字库
	 *	中文 32			号使用SD卡字库
	 */
	if (c < 0x80)                                                                
	{
		offset = c * BytesPerFont;
			
		const char *pFontBase = NULL;		
		switch (GUI_pContext->pAFont->YSize)
		{
			case 16:
				pFontBase = GUI_FontSongTi_ASCII_8X16;
				break;
			case 24:
				pFontBase = GUI_FontSongTi_ASCII_12X24;
				break;
			case 32:
				pFontBase = GUI_FontSongTi_ASCII_16X32;
				break;
			default:
				pFontBase = GUI_FontSongTi_ASCII_8X16;
				break;
		}	
		memcpy(GUI_FontDataBuf, pFontBase+offset, BytesPerFont);
		
		return;
	}
	else                                                                          
	{
		high = (c & 0xff) - 0xa0;
		low = (c >> 8) - 0xa0;

		offset = ( (94*(high-1)) + (low-1) )* BytesPerFont;
		if (GUI_pContext->pAFont->YSize == 16)
		{
			isFlashFont = true;
			baseAddr = FONT_CH_SONGTI_16X16_ADDR;
		}
		else if (GUI_pContext->pAFont->YSize == 24)
		{
			isFlashFont = true;
			baseAddr = FONT_CH_SONGTI_24X24_ADDR;
		}
	}
	
	if (isFlashFont == true)
	{
		sf_ReadBuffer(GUI_FontDataBuf, baseAddr+offset, BytesPerFont);
	}
	else
	{
		GUI_X_GetFontData(font, offset, GUI_FontDataBuf, BytesPerFont);
	}
}

void GUIPROP_X_DispChar(U16P c)  
{
    int BytesPerLine;
    GUI_DRAWMODE DrawMode = GUI_pContext->TextMode;
    const GUI_FONT_PROP GUI_UNI_PTR *pProp = GUI_pContext->pAFont->p.pProp;
   
	//搜索定位字库数据信息  
    for (; pProp; pProp = pProp->pNext)                                          
    {
        if ((c >= pProp->First) && (c <= pProp->Last))break;
    }
	
    if (pProp)
    {
        GUI_DRAWMODE OldDrawMode;
        const GUI_CHARINFO GUI_UNI_PTR * pCharInfo = pProp->paCharInfo;
        GUI_GetDataFromMemory(pProp, c);//取出字模数据
        BytesPerLine = pCharInfo->BytesPerLine;                
        OldDrawMode  = LCD_SetDrawMode(DrawMode);
        LCD_DrawBitmap(GUI_pContext->DispPosX, GUI_pContext->DispPosY,
                       pCharInfo->XSize, GUI_pContext->pAFont->YSize,
                       GUI_pContext->pAFont->XMag, GUI_pContext->pAFont->YMag,
                       1,     /* Bits per Pixel */
                       BytesPerLine,
                       &GUI_FontDataBuf[0],
                       &LCD_BKCOLORINDEX);
		
        /* Fill empty pixel lines */
        if (GUI_pContext->pAFont->YDist > GUI_pContext->pAFont->YSize)
        {
            int YMag = GUI_pContext->pAFont->YMag;
            int YDist = GUI_pContext->pAFont->YDist * YMag;
            int YSize = GUI_pContext->pAFont->YSize * YMag;
            if (DrawMode != LCD_DRAWMODE_TRANS)
            {
                LCD_COLOR OldColor = GUI_GetColor();
                GUI_SetColor(GUI_GetBkColor());
                LCD_FillRect(GUI_pContext->DispPosX, GUI_pContext->DispPosY + YSize,
                             GUI_pContext->DispPosX + pCharInfo->XSize,
                             GUI_pContext->DispPosY + YDist);
                GUI_SetColor(OldColor);
            }
        }
        LCD_SetDrawMode(OldDrawMode); /* Restore draw mode */
        GUI_pContext->DispPosX += pCharInfo->XDist * GUI_pContext->pAFont->XMag;
    }
}

/*********************************************************************
*
*       GUIPROP_GetCharDistX
*/
int GUIPROP_X_GetCharDistX(U16P c, int * pSizeX)
{
    const GUI_FONT_PROP GUI_UNI_PTR * pProp = GUI_pContext->pAFont->p.pProp;  
    for (; pProp; pProp = pProp->pNext)                                        
    {
        if ((c >= pProp->First) && (c <= pProp->Last))break;
    }
    return (pProp) ? (pProp->paCharInfo)->XSize * GUI_pContext->pAFont->XMag : 0;
}

