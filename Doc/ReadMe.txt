============================================================================================================
																							HAL
============================================================================================================
Ê¹ÓÃHAL¿âÌæ»»Ô­ÓÐ±ê×¼¿âº¯Êý£¬Ðè×¢ÒâÒÔÏÂ£º
	1.1 ÔÚÆô¶¯ÎÄ¼þÖÐ£¬²¢Î´³õÊ¼»¯ÏµÍ³Ê±ÖÓ£¬ÐèÒª½øÈëÓ¦ÓÃ²ãÊÖ¶¯³õÊ¼»¯¡£	
	
	1.2 HAL¿âÐèÒªÓÃµ½µÎ´ð¶¨Ê±Æ÷£¬Òò´Ë£¬ÓÃ»§²»ÄÜÔÙÊ¹ÓÃµÎ´ð¶¨Ê±Æ÷£¬×÷ÎªÓÃ»§²ã¶¨Ê±Æ÷¡£
	
	1.3 ÖÐ¶Ï·þÎñº¯Êý stm32f4xx_it.c/stm32f4xx_it.h£¬ÐèÒª´ÓÆäËûµØ·½¿½±´£¬HAL¿â²»Ìá¹©¡£
	
	1.4 ÅäÖÃÎÄ¼þÐèÒª¿½±´£¬ÎÄ¼þÃûÎªstm32f4xx_hal_conf.h¡£
	
	1.5 uint32_t SystemCoreClock = 16000000; ¸Ã±äÁ¿ÔÚ HAL_RCC_ClockConfig(£©ÖÐ»á±»¸Ä±äÎª168000000;
	
	1.6 ¸Ä±äÉèÖÃÖÐ¶ÏÏòÁ¿±íÆ«ÒÆÁ¿µÄ·½Ê½£¬Ô­À´ÊÇº¯Êý NVIC_SetVectorTable()ÉèÖÃ£¬ÓÉÓÚ¿âº¯ÊýÃ»ÓÐÌá¹©Ïà¹ØAPI£¬Òò´Ë´Ó
		Ô­À´µÄ¿âÖÐÒÆÖ²¹ýÀ´¡£
	
	1.7 ÐÞ¸ÄÊ±ÖÓ£¬ÎÒÃÇµÄÍâ²¿¾§ÕñÎª25MHz£¬¶ø¿âÖÐÄ¬ÈÏÎª8MHz£¬Òò´ËÐèÒªÐÞ¸Ä£º
		£¨1£©ÐÞ¸ÄÎÄ¼þ stm32f4xx_hal_conf.h£¬
			#if !defined  (HSE_VALUE) 
				#define HSE_VALUE    ((uint32_t)8000000) /*!< Value of the External oscillator in Hz */
			#endif /* HSE_VALUE */
			¸ÄÎª
			#if !defined  (HSE_VALUE) 
				#define HSE_VALUE    ((uint32_t)25000000) /*!< Value of the External oscillator in Hz */
			#endif /* HSE_VALUE */
		£¨2£©ÐÞ¸Ä SystemClock_Config() º¯Êý£¬
			*            HSE Frequency(Hz)              = 8000000
			*            PLL_M                          = 8
			*            PLL_N                          = 336
			*            PLL_P                          = 2
			*            PLL_Q                          = 7
			¸ÄÎª
			*            HSE Frequency(Hz)              = 25000000
			*            PLL_M                          = 25
			*            PLL_N                          = 336
			*            PLL_P                          = 2
			*            PLL_Q                          = 4
							
	1.8 ÒÆÖ²USBÊ±£¬UÅÌÐ¡µÆÊ¼ÖÕÎ´ÁÁÆðÀ´£¬ËµÃ÷²»ÄÜÇý¶¯¡£¶Ô±ÈSTM32CubeMAX²úÉúµÄ´úÂë£¬·¢ÏÖÊÇPLL_QÅäÖÃ´íÎó£¬°ÑPLL_Q=4¸ÄÎªPLL_Q=7ºó£¬
		¹Û²ìUSBÖ¸Ê¾µÆÒ»Ö±ÉÁ£¬ËµÃ÷USBÒÑ¾­Çý¶¯£¬µ«ÊÇÓÉÓÚÄ³ÖÖÔ­Òò²¢Î´¹¤×÷Õý³£¡£
		×îÖÕ·¢ÏÖÊÇÔÚ USBH_UserProcess()º¯ÊýÖÐ£¬¼ÓÈë f_mount()º¯Êý£¬¶øÇÒº¯Êý²ÎÊýÎªÁ¢¼´¹ÒÔØ·½Ê½¡£
	
	1.9 SDºÍUÅÌ
		£¨1£©UÅÌ°ÎµôÒÔºóÐèÒªf_mount()
		£¨2£©SD¿¨ºÍUÅÌf_mountÉèÖÃ 0:Do not mount (delayed mount)
		
	2.0 ÒÔÌ«Íø	
		£¨1£©ÒÔÌ«Íø½«½ÓÊÜµ½µÄÊý¾Ý·ÅÈë¶ÓÁÐÖÐ£¬Ó¦ÓÃ³ÌÐò´Ó¶ÓÁÐÖÐ¶ÁÈ¡¡£
		£¨2£©ÒÔÌ«ÍøÖØÐÂ²å°ÎÍøÏßÐèÒªÊ¹ÓÃ ethernetif_update_config()´¦Àí¡£
			
============================================================================================================
													emWin
============================================================================================================			
1¡¢ºº×ÖÏÔÊ¾
	£¨1£©emWinµÄºº×Ö×Ö¿âÊ¹ÓÃ¡°FontCvtST.exe¡±Éú³É¡£
	£¨2£©ºº×Ö×Ö¿â¿ÉÒÔ´æ·ÅÔÚSD¡¢ÍâÖÃFlashµÈÍâÖÃ´æ´¢¿Õ¼ä¡£
	£¨3£©Í¨¹ýÈí¼þ¡°FontCvtST.exe¡±Éú³ÉµÄ×Ö¿âÎªUnicode±àÂë·½Ê½£¬¸Ã±àÂë·½Ê½ÓÐUTF-8£¬UTF-16£¬UTF-32
			3ÖÖ½âÂë·½Ê½£¬Ê¹ÓÃ×Ö¿âÖ®Ç°ÐèÒªµ÷ÓÃº¯Êý GUI_UC_SetEncodeUTF8(); Ê¹ÓÃUTF-8½øÐÐ½âÂë¡£
			Í¬Ê±£¬°üº¬´ýÏÔÊ¾µÄºº×ÖµÄ¡°.cÎÄ¼þ¡±Ò²±ØÐë×ª»»ÎªUTF-8¸ñÊ½£¬·ñÔò²»ÄÜÕý³£ÏÔÊ¾ºº×Ö¡£
	£¨4£©ÓÉÓÚkeil5´æÔÚBUG£¬UTF-8µÄÎÄ¼þÐèÒªÊ¹ÓÃ¼ÇÊÂ±¾Áí´æÎªUTF-8¸ñÊ½£¬Ò»µ©×ª»»ºó£¬²»¿ÉÔÙ±àÒë¡£·ñÔò½«±àÒë³ö´í£¬ÐèÒª
		ÖØÐÂ±£´æ¡£	
	£¨5£©GUI_XBF_CreateFont() ÐèÒª·ÅÔÚ GUI_Init()ºóÃæ¼ÓÔØ£¬·ñÔòÏÔÊ¾ºº×ÖÂÒÂë¡£


2¡¢½øÈëÓ²¼þÖÐ¶Ï£¿
	£¨1£©GUI_Init()ÐèÒª·ÅÔÚGUIÈÎÎñÖÐ£¬µÈµ½GUIÈÎÎñÅÜÆðÀ´ÔÙÈ¥³õÊ¼»¯¡£
	
3¡¢Ê¹ÓÃ²Ù×÷ÏµÍ³£¬emwinÐèÒª×öÒÔÏÂÐÞ¸Ä
	£¨1£©Ê¹ÓÃ´øÓÐOSµÄLib¿â
	£¨2£©ÊµÏÖ GUI_XÄÚµÄÏà¹Øº¯Êý
	£¨3£©ÅäÖÃGUIConf.hÎÄ¼þÖÐ£¬#define GUI_OS              1
	
4¡¢½«×Ö¿â·ÅÈëSD¿¨£¬¼ÓÔØºº×ÖºÜÂý£¿
	£¨1£©²âÊÔ·¢ÏÖ f_lseek() Ã¿Ò»´Îµ÷ÓÃ¶¼»¨·Ñ1ms×óÓÒÊ±¼ä¡£
	£¨2£©·¢ÏÖ²ÉÓÃ¹Ù·½Èí¼þÉú³ÉµÄ×Ö¿âÎªunicode±àÂë·½°¸£¬Ã¿´Î¶¨Î»Ò»¸öºº×Ö£¬ÐèÒªÒÆ¶¯¶à´ÎÎÄ¼þÖ¸Õë£¬Ã¿´Î¶ÁÈ¡µÄ×Ö½Ú²»³¬¹ý4
		¸ö£¬×îÖÕÊ¹ËÙ¶ÈÌá²»ÉÏÈ¥¡£
	£¨3£©Í¨¹ýÒÆÖ²°²¸»À³µÄ×ÖÌåÇý¶¯£¬¿ÉÒÔÖ§³Ö¼ÓÔØgb2312µÄ×Ö¿â
	£¨4£©Í¨¹ý°Ñ³£ÓÃºº×Ö×Ö¿â·ÅÈëÍâÖÃflash£¬Ó¢ÎÄ×Ö¿â·ÅÈëÄÚÖÃflash£¬ÊµÏÖ×ÖÌå¿ìËÙ¼ÓÔØ¡£
	
5¡¢emWinÔõÃ´Ê¹ÓÃGB2312µÄ×Ö¿â£¿
	£¨1£©Ìí¼ÓÎÄ¼þ GUI_UC_EncodeNone.c£¬GUI_UC_FontConfig.c£¬GUI_UC_X.c ÎÄ¼þ
	£¨2£©²»ÄÜÊ¹ÓÃº¯Êý GUI_UC_SetEncodeUTF8();
	£¨3£©Ê¹ÓÃº¯Êý MULTIEDIT_SetFont(hItem, &GUI_FontHZ32); ÉèÖÃ×ÖÌå
	£¨4£©´ýÏÔÊ¾µÄÎÄ±¾¸ñÊ½£¬²»ÄÜÎª UTF-8 ÀàÐÍ
		
			
============================================================================================================
													LCD
============================================================================================================
1¡¢	/* ±ØÐëÓ²¼þ¸´Î»£¬·ñÔòÆÁÄ»¿ÉÄÜ´æÔÚÆ«ÒÆ */
	HAL_GPIO_WritePin(LCD_RST_GPIO_PORT, LCD_RST_GPIO_PIN, GPIO_PIN_RESET);
	bsp_DelayMS(20);
	HAL_GPIO_WritePin(LCD_RST_GPIO_PORT, LCD_RST_GPIO_PIN, GPIO_PIN_SET);
	bsp_DelayMS(20);
	
============================================================================================================
													LwIP
============================================================================================================
1¡¢	/* ±ØÐëÓ²¼þ³õÊ¼»¯ */
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);

============================================================================================================
													uCos-III
============================================================================================================
1¡¢	ÐÞ¸Ä startup_stm32f407xx.s ÎÄ¼þ£º
	£¨1£©½« PendSVHandler ÐÞ¸ÄÎª OS_CPU_PendSVHandler
	£¨2£©½« SysTickHandler ÐÞ¸ÄÎª OS_CPU_SysTickHandler

2¡¢	ÖØÐ´ HAL_InitTick() º¯Êý£¬Ö´ÐÐ uCOS-III ÄÚ²¿µÄµÎ´ð¶¨Ê±Æ÷³õÊ¼»¯º¯Êý
3¡¢	ÔÚ¹³×Óº¯Êý App_OS_TimeTickHook()£¬ÖÐÔö¼Ó HAL_SYSTICK_IRQHandler()
4¡¢ ²âÊÔ¸¡µãÊýÊ±£¬½øÈëÓ²¼þÖÐ¶Ï¡£¹Ù·½ÒÆÖ²µÄ¹¤³ÌÔÚMDK4.7 ÒÔÏÂ°æ±¾ÖÐ¿ÉÒÔÕý³£µÄÔËÐÐ£¬µ«ÊÇÒÔÉÏ°æ±¾²»ÄÜÔËÐÐ¡£Òò´Ë²ÉÓÃ°²¸»À³
	µÄ·½Ê½ÒÆÖ²£º
	£¨1£©ÐÞ¸Äº¯Êý OSTaskStkInit()¡¢OS_CPU_PendSVHandler()
	£¨2£©Êµ²âÒÆÖ²°²¸»À³µÄÎÄ¼þºó£¬²»ÄÜÇÐ»»ÈÎÎñ£¬Ö»ÄÜÔÚ¿ÕÏÐÈÎÎñÖÐÖ´ÐÐ¡£Òò´Ë£¬ÔÝÊ±ÏÈ¹Ø±ÕÓ²¼þFPU£¬ºóÆÚÔÙÏë°ì·¨¡£
5¡¢	ÐÞ¸Äº¯Êý BSP_OS_TimeDlyMs()£¬ÑÓÊ±´íÎó
	ms  = dly_ms / 1000u;
    sec = dly_ms % 1000u;
	¸ÄÎª£º
	ms  = dly_ms % 1000u;
    sec = dly_ms / 1000u;
6¡¢	Ê¹ÓÃÕýµãÔ­×ÓµÄÒÆÖ²·½Ê½£¬¿ÉÒÔ½â¾ö¿ªÆôFPU½øÈëÓ²¼þÖÐ¶ÏµÄÎÊÌâ£º
	£¨1£©ÐÞ¸ÄÆô¶¯ÎÄ¼þstartup_stm32f407xx.s£¬ÐÞ¸Ä£º
	IMPORT  __main

			 LDR     R0, =SystemInit
			 BLX     R0		 
			 LDR     R0, =__main
			 BX      R0
			 ENDP
			 
	Îª£º
	IMPORT  __main

			 LDR     R0, =SystemInit
			 BLX     R0
			 
			 IF {FPU} != "SoftVFP"
											; Enable Floating Point Support at reset for FPU
			 LDR.W   R0, =0xE000ED88         ; Load address of CPACR register
			 LDR     R1, [R0]                ; Read value at CPACR
			 ORR     R1,  R1, #(0xF <<20)    ; Set bits 20-23 to enable CP10 and CP11 coprocessors
											; Write back the modified CPACR value
			 STR     R1, [R0]                ; Wait for store to complete
			 DSB
			
											; Disable automatic FP register content
											; Disable lazy context switch
			 LDR.W   R0, =0xE000EF34         ; Load address to FPCCR register
			 LDR     R1, [R0]
			 AND     R1,  R1, #(0x3FFFFFFF)  ; Clear the LSPEN and ASPEN bits
			 STR     R1, [R0]
			 ISB                             ; Reset pipeline now the FPU is enabled
			 ENDIF
			 
			 LDR     R0, =__main
			 BX      R0
			 ENDP
			 
	£¨2£©²»ÐèÒªÐÞ¸Ä  OSTaskStkInit()¡¢OS_CPU_PendSVHandler()
	£¨3£©ÔÚ"Target-Float Point HardWare"ÖÐÊ¹ÓÃFPU¡£
	£¨4£©ÔÚÃ¿¸öÈÎÎñ´´½¨Ê±£¬¶¼ÐèÒªÌí¼Ó OS_OPT_TASK_SAVE_FP Ñ¡Ïî¡£
	£¨5£©¼´¿ÉÕýÈ·Ö´ÐÐ¸¡µãÊý¡£
	£¨6£©Ô­×ÓµÄ½âÊÍ£º¡°CORTEX-M4ÄÚºËÓÐ¸ö LAazy Stacking¹¦ÄÜ£¬Èç¹ûÊ¹ÓÃFPU¹¦ÄÜµÄ»°ÐèÒª¹Ø±ÕÕâ¸ö¹¦ÄÜ¡±
	£¨7£©FPUÖ»¶Ôµ¥¾«¶È¸¡µãÊýÔËËã£¬ËÙ¶ÈºÜ¿ì¡£¶ÔÓÚË«¾«¶È¸¡µãÊý»¹ÊÇ²ÉÓÃÆÕÍ¨ÔËËã·½Ê½¡£
7¡¢	ÎÊÌâ£ºÊ¹ÓÃFatFs¶ÁÈ¡SD¿¨Ê±£¬·µ»Ø FR_NOT_READY ×´Ì¬£¿
	½â¾ö£º£¨1£©Ìí¼Ó syscall.c ÎÄ¼þ
		  £¨2£©´ò¿ª _FS_REENTRANT Ñ¡Ïî 
		  £¨3£©GUI_Init();³õÊ¼»¯ÔÚGUIÈÎÎñÅÜÆðÀ´ºó²Å³õÊ¼»¯£¬·ñÔò½øÈëÓ²¼þÖÐ¶Ï
		  £¨4£©SD¿¨Çý¶¯ÖÐ£¬SDÖÐ¶ÏÓÅÏÈ¼¶Ò»¶¨Òª´óÓÚSD_DMAÖÐ¶ÏÓÅÏÈ¼¶£¬·ñÔòµ¼ÖÂÖÐ¶ÏÖÐÒ»Ö±µÈ´ýDMA×´Ì¬
		  
============================================================================================================
													USB
============================================================================================================
1¡¢	ÎÊÌâ£ºÔÚuCOS-III»·¾³ÏÂ£¬Ê¹ÓÃUSB¶ÁÐ´Êý¾Ý£¬·µ»Ø¡°´ÅÅÌ´íÎó¡±¡£
	Ô­Òò£ºUSB¶ÁÈ¡Êý¾ÝÊ±£¬Ò»Ö±´¦ÓÚµÈ´ý×´Ì¬£¬Ô­ÒòÊÇ¶àÈÎÎñÇÐ»»£¬µ¼ÖÂµÈ´ý³¬Ê±¡£
	½â¾ö£º½«USBÖÐ¶ÏÓÅÏÈ¼¶ÉèÖÃÎª×î¸ß£¬¿ÉÒÔÔÝÊ±½â¾ö¡£
	
2¡¢ ÎÊÌâ£ºÔÚuCOS-III»·¾³ÏÂ£¬°Î³öUÅÌºó£¬Ê¹ÓÃf_mount(NULL,...); ·µ»Ø´íÎó×´Ì¬£¿
	Ô­Òò£ºFATFSµ÷ÓÃº¯Êý ff_del_syncobj() ·µ»ØÊ§°Ü¡£
	½â¾ö£º²Î¿¼°²¸»À³£¬²¢Î´°ÎµôUÅÌ²¢Î´µ÷ÓÃ¸Ãº¯Êý£¬Òò´ËÒ²²»ÔÙµ÷ÓÃ¡£
	
	

	
¬
