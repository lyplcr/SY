#include "GUI.h"

extern void GUIPROP_X_DispChar(U16P c);
extern int GUIPROP_X_GetCharDistX(U16P c, int * pSizeX);

/*
*********************************************************************************************************
*                              				16ºÅ×ÖÌå
*********************************************************************************************************
*/
GUI_CONST_STORAGE GUI_CHARINFO GUI_FontHZ16_CharInfo[2] =  
{    
    {  8,    8,  1, (void *)"0:/asc/song16.zk"},      
    {  16,  16,  2, (void *)"0:/hz/song16.zk"},          
};

GUI_CONST_STORAGE GUI_FONT_PROP GUI_FontHZ16_PropHZ= {
      0xA1A1,  
      0xFEFE,  
      &GUI_FontHZ16_CharInfo[1],
      (void *)0,  
};
GUI_CONST_STORAGE  GUI_FONT_PROP GUI_FontHZ16_PropASC= {
      0x0000,  
      0x007F,  
      &GUI_FontHZ16_CharInfo[0],
      (void GUI_CONST_STORAGE *)&GUI_FontHZ16_PropHZ,  
};

GUI_CONST_STORAGE  GUI_FONT GUI_FontSongTi_16X16 =  
{
      GUI_FONTTYPE_PROP_USER,
      16,  
      16,  
      1,  
      1,  
      (void GUI_CONST_STORAGE *)&GUI_FontHZ16_PropASC
};

/*
*********************************************************************************************************
*                              				24ºÅ×ÖÌå
*********************************************************************************************************
*/
GUI_CONST_STORAGE GUI_CHARINFO GUI_FontHZ24_CharInfo[2] =  
{    
    {  12,  12,  2, (void *)"0:/asc/song24.zk"},      
    {  24,  24,  3, (void *)"0:/hz/song24.zk"},          
};

GUI_CONST_STORAGE GUI_FONT_PROP GUI_FontHZ24_PropHZ= {
      0xA1A1,  
      0xFEFE,  
      &GUI_FontHZ24_CharInfo[1],
      (void *)0,  
};
GUI_CONST_STORAGE  GUI_FONT_PROP GUI_FontHZ24_PropASC= {
      0x0000,  
      0x007F,  
      &GUI_FontHZ24_CharInfo[0],
      (void GUI_CONST_STORAGE *)&GUI_FontHZ24_PropHZ,  
};

GUI_CONST_STORAGE  GUI_FONT GUI_FontSongTi_24X24 =  
{
      GUI_FONTTYPE_PROP_USER,
      24,  
      24,  
      1,  
      1,  
      (void GUI_CONST_STORAGE *)&GUI_FontHZ24_PropASC
};

/*
*********************************************************************************************************
*                              				32ºÅ×ÖÌå
*********************************************************************************************************
*/
GUI_CONST_STORAGE GUI_CHARINFO GUI_FontHZ32_CharInfo[2] =  
{    
    {  16, 16, 2, (void *)"0:/asc/song32.zk"},      
    {  32, 32, 4, (void *)"0:/hz/song32.zk"},          
};

GUI_CONST_STORAGE GUI_FONT_PROP GUI_FontHZ32_PropHZ= {
      0xA1A1,  
      0xFEFE,  
      &GUI_FontHZ32_CharInfo[1],
      (void *)0,  
};
GUI_CONST_STORAGE  GUI_FONT_PROP GUI_FontHZ32_PropASC= {
      0x0000,  
      0x007F,  
      &GUI_FontHZ32_CharInfo[0],
      (void GUI_CONST_STORAGE *)&GUI_FontHZ32_PropHZ,  
};

GUI_CONST_STORAGE  GUI_FONT GUI_FontSongTi_32X32 =  
{
      GUI_FONTTYPE_PROP_USER,
      32,  
      32,  
      1,  
      1,  
      (void GUI_CONST_STORAGE *)&GUI_FontHZ32_PropASC
};
