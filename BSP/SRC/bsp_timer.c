/*
*********************************************************************************************************
* @file    	bsp_timer.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-12 13:15:44
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	定时器源文件
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
#define	HEART_TIM							TIM3
#define HEART_TIM_IRQn                      TIM3_IRQn
#define HEART_TIM_IRQHandler				TIM3_IRQHandler	
#define HEART_TIM_CLK_ENABLE()				__HAL_RCC_TIM3_CLK_ENABLE()

#define	HEART_TIM_Int_Enable()				HAL_TIM_Base_Start_IT(&g_HeartTimHandle)		
#define	HEART_TIM_Int_Disable()				HAL_TIM_Base_Stop_IT(&g_HeartTimHandle)

#define	Systick_Int_Enable()				(SysTick->CTRL)|=(1<<SysTick_CTRL_TICKINT_Pos)
#define	Systick_Int_Disable()				(SysTick->CTRL)&=~(1<<SysTick_CTRL_TICKINT_Pos)

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
/* 这2个全局变量转用于 bsp_DelayMS() 函数 */
static __IO uint32_t g_RunTime;

/* 定于软件定时器结构体变量 */
static SOFT_TMR s_tTmr[TMR_COUNT];
static SOFT_TMR_US TmrUs[TMR_COUNT_US];

static TIM_HandleTypeDef g_HeartTimHandle;


/*
*********************************************************************************************************
*                              				Private function prototypes
*********************************************************************************************************
*/
static void GeneralTimerInit( TIM_TypeDef *TIMx, TIM_HandleTypeDef *TimHandle, \
				uint32_t Prescaler, uint32_t Period );

/*
*********************************************************************************************************
*                              				Private functions
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : bsp_InitTimer
* Description	: 定时器初始化
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_InitTimer(void)
{
	/* 清零所有的软件定时器 */
	for (uint8_t i = 0; i < TMR_COUNT; i++)
	{
		s_tTmr[i].Count = 0;
		s_tTmr[i].PreLoad = 0;
		s_tTmr[i].Flag = 0;
		s_tTmr[i].Mode = TMR_ONCE_MODE;	/* 缺省是1次性工作模式 */
	}

	/** 
	 *	时钟周期	：84000000，表示计数器1秒可以累加到84000000。
	 *	预分频	：84，		表示计数器1秒只需要累加到1000000。
	 *	周期	：1000，	表示计数器只需要累加到1000，花费的时间为1ms，便产生一次更新中断。						
	 */
	GeneralTimerInit(HEART_TIM, &g_HeartTimHandle, 84, 1000);
	
	ECHO(DEBUG_BSP_INIT, "Timer初始化 .......... OK");
}

/*
*********************************************************************************************************
* Function Name : GeneralTimerInit
* Description	: 通用定时器初始化
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void GeneralTimerInit( TIM_TypeDef *TIMx, TIM_HandleTypeDef *TimHandle, \
				uint32_t Prescaler, uint32_t Period )
{
	TimHandle->Instance = TIMx;
	
	TimHandle->Init.Period = Period - 1;
	TimHandle->Init.Prescaler = Prescaler - 1;
	TimHandle->Init.ClockDivision = 0;
	TimHandle->Init.CounterMode = TIM_COUNTERMODE_UP;
	TimHandle->Init.RepetitionCounter = 0;
	if (HAL_TIM_Base_DeInit(TimHandle) != HAL_OK)
	{
		/* Initialization Error */
		bsp_ErrorHandler();
	}
	if(HAL_TIM_Base_Init(TimHandle) != HAL_OK)
	{
		/* Initialization Error */
		bsp_ErrorHandler();
	}

	/*##-2- Start the TIM Base generation in interrupt mode ####################*/
	/* Start Channel1 */
	if(HAL_TIM_Base_Start_IT(TimHandle) != HAL_OK)
	{
		/* Starting Error */
		bsp_ErrorHandler();
	}
}

/*
*********************************************************************************************************
* Function Name : bsp_SoftTimerDec
* Description	: 对所有定时器变量减1
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE void bsp_SoftTimerDec(SOFT_TMR *_tmr)
{
	if (_tmr->Count > 0)
	{
		/* 如果定时器变量减到1则设置定时器到达标志 */
		if (--_tmr->Count == 0)
		{
			_tmr->Flag = 1;

			/* 如果是自动模式，则自动重装计数器 */
			if(_tmr->Mode == TMR_AUTO_MODE)
			{
				_tmr->Count = _tmr->PreLoad;
			}
		}
	}
}

/*
*********************************************************************************************************
* Function Name : HEART_TIM_IRQHandler
* Description	: 中断服务函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void HEART_TIM_IRQHandler(void)
{
#if (uCOS_EN)
	CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();                                          
    CPU_CRITICAL_EXIT();
#endif
	
	HAL_TIM_IRQHandler(&g_HeartTimHandle);	

#if (uCOS_EN)
	OSIntExit(); 
#endif	
} 

/*
*********************************************************************************************************
* Function Name : HAL_TIM_PeriodElapsedCallback
* Description	: 更新中断服务函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == HEART_TIM)
	{	
		for (uint8_t i = 0; i < TMR_COUNT; i++)
		{
			bsp_SoftTimerDec(&s_tTmr[i]);
		}
		
		g_RunTime++;
	}
}

#if 0
/*
*********************************************************************************************************
* Function Name : bsp_DelayUS
* Description	: US延时函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_DelayUS(uint32_t n)
{
    uint32_t ticks;
    uint32_t told;
    uint32_t tnow;
    uint32_t tcnt = 0;
    uint32_t reload;
       
	reload = SysTick->LOAD;                
    ticks = n * (SystemCoreClock / 1000000);	 /* 需要的节拍数 */  
    
    tcnt = 0;
    told = SysTick->VAL;             /* 刚进入时的计数器值 */

    while (1)
    {
        tnow = SysTick->VAL;    
        if (tnow != told)
        {    
            /* SYSTICK是一个递减的计数器 */    
            if (tnow < told)
            {
                tcnt += told - tnow;    
            }
            /* 重新装载递减 */
            else
            {
                tcnt += reload - tnow + told;    
            }        
            told = tnow;

            /* 时间超过/等于要延迟的时间,则退出 */
            if (tcnt >= ticks)
            {
            	break;
            }
        }  
    }
} 

/*
*********************************************************************************************************
* Function Name : bsp_DelayMS
* Description	: MS延时函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_DelayMS(uint32_t n)
{
	while(n--)
	{
		bsp_DelayUS(1000);
	}
}
#else
/*
*********************************************************************************************************
*	函 数 名: bsp_DelayMS
*	功能说明: 为了让底层驱动在带RTOS和裸机情况下有更好的兼容性
*             专门制作一个阻塞式的延迟函数，在底层驱动中ms毫秒延迟主要用于初始化，并不会影响实时性。
*	形    参: n 延迟长度，单位1 ms
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_DelayMS(uint32_t _ulDelayTime)
{
	while (_ulDelayTime--)
	{
		bsp_DelayUS(1000);
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_DelayUS
*	功能说明: 这里的延时采用CPU的内部计数实现，32位计数器
*             	OSSchedLock(&err);
*				bsp_DelayUS(5);
*				OSSchedUnlock(&err); 根据实际情况看看是否需要加调度锁或选择关中断
*	形    参: _ulDelayTime  延迟长度，单位1 us
*	返 回 值: 无
*   说    明: 1. 主频168MHz的情况下，32位计数器计满是2^32/168000000 = 25.565秒
*                建议使用本函数做延迟的话，延迟在1秒以下。  
*             2. 实际通过示波器测试，微妙延迟函数比实际设置实际多运行0.25us左右的时间。
*             下面数据测试条件：
*             （1）. MDK5.15，优化等级0, 不同的MDK优化等级对其没有影响。
*             （2）. STM32F407IGT6
*             （3）. 测试方法：
*				 GPIOI->BSRRL = GPIO_Pin_8;
*				 bsp_DelayUS(10);
*				 GPIOI->BSRRH = GPIO_Pin_8;
*             -------------------------------------------
*                测试                 实际执行
*             bsp_DelayUS(1)          1.2360us
*             bsp_DelayUS(2)          2.256us
*             bsp_DelayUS(3)          3.256us
*             bsp_DelayUS(4)          4.256us
*             bsp_DelayUS(5)          5.276us
*             bsp_DelayUS(6)          6.276us
*             bsp_DelayUS(7)          7.276us
*             bsp_DelayUS(8)          8.276us
*             bsp_DelayUS(9)          9.276us
*             bsp_DelayUS(10)         10.28us
*            3. 两个32位无符号数相减，获取的结果再赋值给32位无符号数依然可以正确的获取差值。
*              假如A,B,C都是32位无符号数。
*              如果A > B  那么A - B = C，这个很好理解，完全没有问题
*              如果A < B  那么A - B = C， C的数值就是0xFFFFFFFF - B + A + 1。这一点要特别注意，正好用于本函数。
*********************************************************************************************************
*/
void bsp_DelayUS(uint32_t _ulDelayTime)
{
    uint32_t tCnt, tDelayCnt;
	uint32_t tStart;
		
	tStart = (uint32_t)CPU_TS_TmrRd();                       /* 刚进入时的计数器值 */
	tCnt = 0;
	tDelayCnt = _ulDelayTime * (SystemCoreClock / 1000000);	 /* 需要的节拍数 */ 		      

	while(tCnt < tDelayCnt)
	{
		tCnt = (uint32_t)CPU_TS_TmrRd() - tStart; /* 求减过程中，如果发生第一次32位计数器重新计数，依然可以正确计算 */	
	}
}
#endif

/*
*********************************************************************************************************
* Function Name : bsp_StartTimer
* Description	: 启动一个定时器
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_StartTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* 打印出错的源代码文件名、函数名称 */
		ECHO(DEBUG_BSP_TIMER,"Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		
		while(1); /* 参数异常，死机等待看门狗复位 */
	}

	HEART_TIM_Int_Disable();  			/* 关中断 */

	s_tTmr[_id].Count = _period;		/* 实时计数器初值 */
	s_tTmr[_id].PreLoad = _period;		/* 计数器自动重装值，仅自动模式起作用 */
	s_tTmr[_id].Flag = 0;				/* 定时时间到标志 */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* 1次性工作模式 */

	HEART_TIM_Int_Enable();  			/* 开中断 */
}

/*
*********************************************************************************************************
* Function Name : bsp_StartAutoTimer
* Description	: 启动一个自动定时器
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_StartAutoTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* 打印出错的源代码文件名、函数名称 */
		ECHO(DEBUG_BSP_TIMER,"Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		
		while(1); /* 参数异常，死机等待看门狗复位 */
	}

	HEART_TIM_Int_Disable();  			/* 关中断 */

	s_tTmr[_id].Count = _period;		/* 实时计数器初值 */
	s_tTmr[_id].PreLoad = _period;		/* 计数器自动重装值，仅自动模式起作用 */
	s_tTmr[_id].Flag = 0;				/* 定时时间到标志 */
	s_tTmr[_id].Mode = TMR_AUTO_MODE;	/* 自动工作模式 */

	HEART_TIM_Int_Enable();  			/* 开中断 */
}

/*
*********************************************************************************************************
* Function Name : bsp_StopTimer
* Description	: 停止一个定时器
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_StopTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		/* 打印出错的源代码文件名、函数名称 */
		ECHO(DEBUG_BSP_TIMER,"Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		
		while(1); /* 参数异常，死机等待看门狗复位 */
	}

	HEART_TIM_Int_Disable();  			/* 关中断 */

	s_tTmr[_id].Count = 0;				/* 实时计数器初值 */
	s_tTmr[_id].Flag = 0;				/* 定时时间到标志 */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* 1次性工作模式 */

	HEART_TIM_Int_Enable();  			/* 开中断 */
}

/*
*********************************************************************************************************
* Function Name : bsp_CheckTimer
* Description	: 检测定时器是否超时
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint8_t bsp_CheckTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		return 0;
	}

	if (s_tTmr[_id].Flag == 1)
	{
		s_tTmr[_id].Flag = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
*********************************************************************************************************
* Function Name : bsp_GetRunTime
* Description	: 获取CPU运行时间
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t bsp_GetRunTime(void)
{
	return g_RunTime;
}

/*
*********************************************************************************************************
* Function Name : bsp_StartTimeUS
* Description	: US级计时开始
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_StartTimeUS(uint8_t _id)
{ 
	TmrUs[_id].startRunTime=bsp_GetRunTime();
	TmrUs[_id].startTick=SysTick->VAL;  
}

/*
*********************************************************************************************************
* Function Name : bsp_GetTimeUS
* Description	: 精确到US计时函数，最长可以计时2147秒,精度1uS
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t bsp_GetTimeUS(uint8_t _id)
{
	uint32_t Us;
	uint32_t endRunTime;
	uint32_t load=0;
	uint32_t endTick;
	
	Systick_Int_Disable();
	
	endTick=SysTick->VAL;
	load=SysTick->LOAD; 
	
	Systick_Int_Enable();
	
	endRunTime=bsp_GetRunTime();
	 
	Us=((endRunTime-TmrUs[_id].startRunTime)*(load+1)+TmrUs[_id].startTick-endTick)/(SystemCoreClock / 1000000L); 	 

	return Us;
}

/*
*********************************************************************************************************
*                              				MSP
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : HAL_TIM_Base_MspInit
* Description	: 定时器MSP初始化
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == HEART_TIM)
	{
		HEART_TIM_CLK_ENABLE();
		
		HAL_NVIC_SetPriority(HEART_TIM_IRQn, 0x0F, 0);
		HAL_NVIC_EnableIRQ(HEART_TIM_IRQn);
	}
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
