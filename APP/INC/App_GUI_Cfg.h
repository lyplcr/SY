/*
*********************************************************************************************************
* @file    	App_GUI_Cfg.h
* @author  	SY
* @version 	V1.0.0
* @date    	2016-12-1 12:44:09
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	GUIÅäÖÃÎÄ¼þ
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
#ifndef __APP_GUI_CFG_H
#define __APP_GUI_CFG_H

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
/*
*********************************************************************************************************
*                              				MAIN
*********************************************************************************************************
*/
#define MAIN_BKCOLOR              			0xD0D0D0
#define MAIN_TEXTCOLOR            			GUI_BLACK

#define MAIN_FONT                 			(&GUI_FontSongTi_24X24)
#define MAIN_BORDER               			10
#define MAIN_TITLE_HEIGHT         			67

/*
*********************************************************************************************************
*                              				¿ò¼Ü
*********************************************************************************************************
*/
#define FRAME_BKCOLOR            			0xB0B0B0
#define FRAME_TEXTCOLOR           			GUI_BLACK

#define FRAME_FONT               			(&GUI_FontSongTi_24X24)
#define FRAME_EFFECT              			(&WIDGET_Effect_3D2L)
#define FRAME_BORDER              			FRAME_EFFECT->EffectSize

#define FRAME_TOTAL_WIDTH					(640)
#define FRAME_TOTAL_HEIGHT					(480)
#define FRAME_WIDTH               			(FRAME_TOTAL_WIDTH - (FRAME_BORDER * 2) -\
												(MAIN_BORDER * 2))
#define FRAME_HEIGHT              			(FRAME_TOTAL_HEIGHT - (FRAME_BORDER * 2) -\
												(MAIN_BORDER + MAIN_TITLE_HEIGHT))
												
#define CONST_FRAME_WIDTH               	(FRAME_TOTAL_WIDTH -\
												(MAIN_BORDER * 2))
#define CONST_FRAME_HEIGHT              	(FRAME_TOTAL_HEIGHT -\
												(MAIN_BORDER + MAIN_TITLE_HEIGHT))

/*
*********************************************************************************************************
*                              				¶Ô»°¿ò
*********************************************************************************************************
*/
#define DIALOG_TITLE_HEIGHT        			30

/*
*********************************************************************************************************
*                              				¿ò¼Ü°´Å¥¿Ø¼þ
*********************************************************************************************************
*/
#define FRAME_BUTTON_BKCOLOR0     			FRAME_BKCOLOR           /* Unpressed */
#define FRAME_BUTTON_BKCOLOR1    			0xE0E0E0              	/* Pressed   */
#define FRAME_BUTTON_BKCOLOR2     			GUI_GRAY_C0            	/* Disabled  */
#define FRAME_BUTTON_COLOR0       			GUI_BLACK              	/* Unpressed */
#define FRAME_BUTTON_COLOR1       			GUI_BLACK              	/* Pressed   */
#define FRAME_BUTTON_FONT         			(&GUI_FontSongTi_24X24)
#define FRAME_BUTTON_EFFECT       			(&WIDGET_Effect_3D2L)

/*
*********************************************************************************************************
*                              				¿ò¼Ü±à¼­¿ò¿Ø¼þ
*********************************************************************************************************
*/
#define FRAME_EDIT_BKCOLOR0     			FRAME_BKCOLOR          	/* Disabled  */    	
#define FRAME_EDIT_BKCOLOR1    				GUI_WHITE              	/* Enabled  */	
#define FRAME_EDIT_BKCOLOR2     			GUI_RED            		/* Cursor */
#define FRAME_EDIT_COLOR0       			GUI_BLACK              	/* Disabled  */  
#define FRAME_EDIT_COLOR1       			GUI_BLACK				/* Enabled  */	
#define FRAME_EDIT_COLOR2       			GUI_BLACK				/* Cursor */
#define FRAME_EDIT_FONT         			(&GUI_FontSongTi_24X24)
#define FRAME_EDIT_EFFECT       			(&WIDGET_Effect_3D1L)

/*
*********************************************************************************************************
*                              				¿ò¼ÜÎÄ±¾¿Ø¼þ
*********************************************************************************************************
*/
#define FRAME_TEXT_BKCOLOR0     			FRAME_BKCOLOR          	/* Disabled  */    	
#define FRAME_TEXT_BKCOLOR1    				FRAME_BKCOLOR           /* Enabled  */	
#define FRAME_TEXT_BKCOLOR2     			FRAME_BKCOLOR           /* Cursor */
#define FRAME_TEXT_COLOR0       			GUI_BLACK              	/* Disabled  */  
#define FRAME_TEXT_COLOR1       			GUI_BLACK				/* Enabled  */	
#define FRAME_TEXT_COLOR2       			GUI_BLACK				/* Cursor */
#define FRAME_TEXT_FONT         			(&GUI_FontSongTi_24X24)
#define FRAME_TEXT_EFFECT       			(&WIDGET_Effect_None)

/*
*********************************************************************************************************
*                              				¿ò¼ÜRADIO¿Ø¼þ
*********************************************************************************************************
*/
#define FRAME_RADIO_BKCOLOR0     			FRAME_BKCOLOR          	/* Disabled  */    	
#define FRAME_RADIO_BKCOLOR1    			FRAME_BKCOLOR           /* Enabled  */	
#define FRAME_RADIO_BKCOLOR2     			FRAME_BKCOLOR           /* Cursor */
#define FRAME_RADIO_COLOR0       			GUI_BLACK              	/* Disabled  */  
#define FRAME_RADIO_COLOR1       			GUI_BLACK				/* Enabled  */	
#define FRAME_RADIO_COLOR2       			GUI_BLACK				/* Cursor */
#define FRAME_RADIO_FONT         			(&GUI_FontSongTi_24X24)
#define FRAME_RADIO_EFFECT       			(&WIDGET_Effect_3D2L)

/*
*********************************************************************************************************
*                              				Exported types
*********************************************************************************************************
*/

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


#endif
/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
