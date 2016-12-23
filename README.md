# SY
---

### *2016/12/23 9:51:08*

## HAL

使用HAL库替换原有标准库函数，需注意以下：
+	在启动文件中，并未初始化系统时钟，需要进入应用层手动初始化。	
	
+	HAL库需要用到滴答定时器，因此，用户不能再使用滴答定时器，作为用户层定时器。
	
+	中断服务函数 stm32f4xx_it.c/stm32f4xx_it.h，需要从其他地方拷贝，HAL库不提供。
	
+	配置文件需要拷贝，文件名为stm32f4xx_hal_conf.h。
	
+	uint32_t SystemCoreClock = 16000000; 该变量在 HAL_RCC_ClockConfig(）中会被改变为168000000;
	
+	改变设置中断向量表偏移量的方式，原来是函数 NVIC_SetVectorTable()设置，由于库函数没有提供相关API，因此从
	原来的库中移植过来。
	
+	修改时钟，我们的外部晶振为25MHz，而库中默认为8MHz，因此需要修改：
	
	```	
	（1）修改文件 stm32f4xx_hal_conf.h，
		#if !defined  (HSE_VALUE) 
			#define HSE_VALUE    ((uint32_t)8000000) /*!< Value of the External oscillator in Hz */
		#endif /* HSE_VALUE */
		改为
		#if !defined  (HSE_VALUE) 
			#define HSE_VALUE    ((uint32_t)25000000) /*!< Value of the External oscillator in Hz */
		#endif /* HSE_VALUE */
	（2）修改 SystemClock_Config() 函数，
		*            HSE Frequency(Hz)              = 8000000
		*            PLL_M                          = 8
		*            PLL_N                          = 336
		*            PLL_P                          = 2
		*            PLL_Q                          = 7
		改为
		*            HSE Frequency(Hz)              = 25000000
		*            PLL_M                          = 25
		*            PLL_N                          = 336
		*            PLL_P                          = 2
		*            PLL_Q                          = 4
	```
						
+	移植USB时，U盘小灯始终未亮起来，说明不能驱动。对比STM32CubeMAX产生的代码，发现是PLL_Q配置错误，把PLL_Q=4改为PLL_Q=7后，
	观察USB指示灯一直闪，说明USB已经驱动，但是由于某种原因并未工作正常。
	最终发现是在 USBH_UserProcess()函数中，加入 f_mount()函数，而且函数参数为立即挂载方式。
	
+	SD和U盘
	（1）U盘拔掉以后需要f_mount()
	（2）SD卡和U盘f_mount设置 0:Do not mount (delayed mount)
		
+	以太网	
	（1）以太网将接受到的数据放入队列中，应用程序从队列中读取。
	（2）以太网重新插拔网线需要使用 ethernetif_update_config()处理。

## emWin

+	汉字显示
	（1）emWin的汉字字库使用“FontCvtST.exe”生成。
	（2）汉字字库可以存放在SD、外置Flash等外置存储空间。
	（3）通过软件“FontCvtST.exe”生成的字库为Unicode编码方式，该编码方式有UTF-8，UTF-16，UTF-32
			3种解码方式，使用字库之前需要调用函数 GUI_UC_SetEncodeUTF8(); 使用UTF-8进行解码。
			同时，包含待显示的汉字的“.c文件”也必须转换为UTF-8格式，否则不能正常显示汉字。
	（4）由于keil5存在BUG，UTF-8的文件需要使用记事本另存为UTF-8格式，一旦转换后，不可再编译。否则将编译出错，需要
		重新保存。	
	（5）GUI_XBF_CreateFont() 需要放在 GUI_Init()后面加载，否则显示汉字乱码。


+	进入硬件中断？
	（1）GUI_Init()需要放在GUI任务中，等到GUI任务跑起来再去初始化。
	
+	使用操作系统，emwin需要做以下修改
	（1）使用带有OS的Lib库
	（2）实现 GUI_X内的相关函数
	（3）配置GUIConf.h文件中，#define GUI_OS              1
	
+	将字库放入SD卡，加载汉字很慢？
	（1）测试发现 f_lseek() 每一次调用都花费1ms左右时间。
	（2）发现采用官方软件生成的字库为unicode编码方案，每次定位一个汉字，需要移动多次文件指针，每次读取的字节不超过4
		个，最终使速度提不上去。
	（3）通过移植安富莱的字体驱动，可以支持加载gb2312的字库
	（4）通过把常用汉字字库放入外置flash，英文字库放入内置flash，实现字体快速加载。
	
+	emWin怎么使用GB2312的字库？
	（1）添加文件 GUI_UC_EncodeNone.c，GUI_UC_FontConfig.c，GUI_UC_X.c 文件
	（2）不能使用函数 GUI_UC_SetEncodeUTF8();
	（3）使用函数 MULTIEDIT_SetFont(hItem, &GUI_FontHZ32); 设置字体
	（4）待显示的文本格式，不能为 UTF-8 类型

## LCD

+	问题：屏幕显示内容发生偏移？
	解决：
	```
	/* 必须硬件复位，否则屏幕可能存在偏移 */
	HAL_GPIO_WritePin(LCD_RST_GPIO_PORT, LCD_RST_GPIO_PIN, GPIO_PIN_RESET);
	bsp_DelayMS(20);
	HAL_GPIO_WritePin(LCD_RST_GPIO_PORT, LCD_RST_GPIO_PIN, GPIO_PIN_SET);
	bsp_DelayMS(20);
	```

##　LwIP

+	问题：不能接收数据？
	解决：

	```
	/* 必须硬件初始化 */
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
	```

##	uCOS-III

+	修改 startup_stm32f407xx.s 文件：
	（1）将 PendSVHandler 修改为 OS_CPU_PendSVHandler
	（2）将 SysTickHandler 修改为 OS_CPU_SysTickHandler

+	重写 HAL_InitTick() 函数，执行 uCOS-III 内部的滴答定时器初始化函数
+	在钩子函数 App_OS_TimeTickHook()，中增加 HAL_SYSTICK_IRQHandler()
+	测试浮点数时，进入硬件中断。官方移植的工程在MDK4.7 以下版本中可以正常的运行，但是以上版本不能运行。因此采用安富莱
	的方式移植：
	（1）修改函数 OSTaskStkInit()、OS_CPU_PendSVHandler()
	（2）实测移植安富莱的文件后，不能切换任务，只能在空闲任务中执行。因此，暂时先关闭硬件FPU，后期再想办法。
+	修改函数 BSP_OS_TimeDlyMs()，延时错误
	ms  = dly_ms / 1000u;
    sec = dly_ms % 1000u;
	改为：
	ms  = dly_ms % 1000u;
    sec = dly_ms / 1000u;
+	使用正点原子的移植方式，可以解决开启FPU进入硬件中断的问题：
	（1）修改启动文件startup_stm32f407xx.s，修改：
	```

	IMPORT  __main

			 LDR     R0, =SystemInit
			 BLX     R0		 
			 LDR     R0, =__main
			 BX      R0
			 ENDP
	```
		 
	为：

	```
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
	```
		 
	（2）不需要修改  OSTaskStkInit()、OS_CPU_PendSVHandler()
	（3）在"Target-Float Point HardWare"中使用FPU。
	（4）在每个任务创建时，都需要添加 OS_OPT_TASK_SAVE_FP 选项。
	（5）即可正确执行浮点数。
	（6）原子的解释：“CORTEX-M4内核有个 LAazy Stacking功能，如果使用FPU功能的话需要关闭这个功能”
	（7）FPU只对单精度浮点数运算，速度很快。对于双精度浮点数还是采用普通运算方式。
+	问题：使用FatFs读取SD卡时，返回 FR_NOT_READY 状态？
	解决：
		（1）添加 syscall.c 文件
		（2）打开 _FS_REENTRANT 选项 
		（3）GUI_Init();初始化在GUI任务跑起来后才初始化，否则进入硬件中断
		（4）SD卡驱动中，SD中断优先级一定要大于SD_DMA中断优先级，否则导致中断中一直等待DMA状态

## USB

+	问题：在uCOS-III环境下，使用USB读写数据，返回“磁盘错误”。
	原因：USB读取数据时，一直处于等待状态，原因是多任务切换，导致等待超时。
	解决：将USB中断优先级设置为最高，可以暂时解决。
	
+	问题：在uCOS-III环境下，拔出U盘后，使用f_mount(NULL,...); 返回错误状态？
	原因：FATFS调用函数 ff_del_syncobj() 返回失败。
	解决：参考安富莱，并未拔掉U盘并未调用该函数，因此也不再调用。
