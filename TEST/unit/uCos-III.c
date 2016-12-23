/*
*********************************************************************************************************
* @file    	uCos-III.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-11-4 14:17:15
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	uCos-III测试
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
#include "System_Init.h"
#include "emWinUtils.h"


/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/
#define TEST_SVC							0x00U		//SVC异常
#define TEST_TASK_EXCHANGE					0x00U		//任务切换
#define TEST_TASK_EXCHANGE_FLOAT			0x00U		//浮点任务切换
#define TEST_TASK_UCOS_FLOAT				0x00U		//浮点任务切换
#define TEST_TASK_UCOS_TASKQ				0x00U		//TASKQ
#define TEST_TASK_UCOS_SEM_POST				0x00U		//SEM POST
#define TEST_TASK_UCOS_SEM					0x00U		//信号量
#define TEST_TASK_UCOS_MSG_QUEUE			0x00U		//消息队列
#define TEST_TASK_UCOS_EVENT_FLAG			0x00U		//事件标志组
#define TEST_TASK_UCOS_TIMER				0x00U		//定时器
#define TEST_TASK_UCOS_WAIT_MORE_EVENT		0x00U		//等待多事件
#define TEST_TASK_UCOS_MEMORY_MANAGER		0x00U		//内存管理
#define TEST_TASK_UCOS_ROUND_ROBIN			0x00U		//ROUND ROBIN
#define TEST_TASK_UCOS_EMWIN				0x00U		//EMWIN
#define TEST_TASK_UCOS_EMWIN_FONT			0x00U		//EMWIN FONT
#define TEST_TASK_UCOS_USB					0x00U		//USB
#define TEST_TASK_UCOS_DEBUG_VIEW			0x00U		//DEBUG VIEW

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
extern OS_TCB 		*AppTaskUpdateTCB_Ptr;
extern OS_TCB 		*AppTaskCOMTCB_Ptr;
extern OS_TCB 		*AppTaskUserIFTCB_Ptr;
extern OS_TCB 		*AppTaskGUITCB_Ptr;
extern OS_TCB 		*AppTaskGUIRefreshTCB_Ptr;
extern OS_TCB 		*AppTaskDebugViewTCB_Ptr;

/*
*********************************************************************************************************
*                              				Private function prototypes
*********************************************************************************************************
*/
void DispTaskInfo(void);



/*
*********************************************************************************************************
*                              				Private functions
*********************************************************************************************************
*/
#if (TEST_SVC == 0x01U)
#define TASK_1								(0x00)
#define TASK_2								(0x01)
#define TASK_3								(0x02)

int __svc(TASK_1) SVC_ServiceAdd( int x, int y );
int __svc(TASK_2) SVC_ServiceReduce( int x, int y );

/*
*********************************************************************************************************
*	函 数 名: SVC_Handler_C
*	功能说明: SVC异常C函数
*	形    参：svc_args 堆栈起始地址
*	返 回 值: 无
*********************************************************************************************************
*/
void SVC_Handler_C(unsigned int * svc_args)
{
	// 堆栈内容:
	// r0, r1, r2, r3, r12, r14, the return address and xPSR
	// - Stacked R0 = svc_args[0]
	// - Stacked R1 = svc_args[1]
	// - Stacked R2 = svc_args[2]
	// - Stacked R3 = svc_args[3]
	// - Stacked R12 = svc_args[4]
	// - Stacked LR = svc_args[5]
	// - Stacked PC = svc_args[6]
	// - Stacked xPSR= svc_args[7]
	unsigned int svc_number;
	svc_number = ((char *)svc_args[6])[-2];
	switch(svc_number)
	{
		case TASK_1: svc_args[0] = svc_args[0] + svc_args[1];
			break;
		case TASK_2: svc_args[0] = svc_args[0] - svc_args[1];
			break;
		default: 	// 未知SVC请求
			break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: SVC_Handler
*	功能说明: SVC异常，汇编的封皮用于获取使用的堆栈指针PSP或MSP
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
__asm void SVC_Handler(void)
{
	TST LR, #4 ; Test bit 2 of EXC_RETURN
	ITE EQ
	MRSEQ R0, MSP ; if 0, stacking used MSP, copy to R0
	MRSNE R0, PSP ; if 1, stacking used PSP, copy to R0
	B __cpp(SVC_Handler_C)
	ALIGN 4
}

/*
*********************************************************************************************************
* Function Name : Test_SVC
* Description	: 测试SVC
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void Test_SVC( void )
{
	int a = 100;
	int b = 200;
	int c = 0;
	
	while(1)
	{
		uint32_t key = ReadKeyValue(g_keyPtr);
		if (key != KEY_NONE)
		{
			switch (ReadKeyStatus(g_keyPtr))
			{
				case STATUS_KEY_PRESS:
					switch (key)
					{
						case KEY_F1:
							c = SVC_ServiceAdd(a,b);
							printf("a = %d, b = %d, c = %d\r\n",a, b, c);
							break;
						case KEY_F2:
							c = SVC_ServiceReduce(a,b);
							printf("a = %d, b = %d, c = %d\r\n",a, b, c);
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
			
			ReleaseKeyMsg(g_keyPtr);
		}
	}
}
#endif

#if (TEST_TASK_EXCHANGE == 0x01U)

/* 字访问 */
#define HW32_REG(ADDRESS)  (*((volatile unsigned long  *)(ADDRESS)))
/* 当检测到错误时，用Breakpoint来停止任务(MDK特有的)*/
/* 根据需要可以改成用while(1)来实现 */
#define stop_cpu   __breakpoint(0)

// 计数，用于切换任务
volatile uint32_t systick_count=0;
// 任务堆栈 (8Kbytes each - 1024 x 8 bytes) 这里也可以采用4字节，并将双字的堆栈对齐方式关闭
uint32_t task0_stack[1024], task1_stack[1024], task2_stack[1024], task3_stack[1024];

// 任务切换用到的数据 
uint32_t  curr_task=0;     // 当前执行任务
uint32_t  next_task=1;     // 下一个任务
uint32_t  PSP_array[4];    // 用于在任务切换时记录PSP

static bool isReady;

/*
*********************************************************************************************************
*	函 数 名: task0
*	功能说明: 任务0实现LED1的闪烁
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void task0(void) 
{ 
	while (1) 
	{
		LCD_DrawCircle(100,100,5,CL_RED);
		isReady = true;
	}
}

/*
*********************************************************************************************************
*	函 数 名: task0
*	功能说明: 任务0实现LED1的闪烁
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void task1(void) 
{ 
	while (1) 
	{
		LCD_DrawCircle(100,200,5,CL_RED);
	}
}

/*
*********************************************************************************************************
*	函 数 名: task0
*	功能说明: 任务0实现LED1的闪烁
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void task2(void) 
{ 
	while (1) 
	{
		LCD_DrawCircle(100,300,5,CL_BLUE);
	}
}

/*
*********************************************************************************************************
*	函 数 名: task3
*	功能说明: 任务0实现LED1的闪烁
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void task3(void) 
{ 
	while (1) 
	{
		LCD_DrawCircle(100,400,5,CL_YELLOW);
	}
}

/*
*********************************************************************************************************
*	函 数 名: OS_Start
*	功能说明: 任务启动
*	形    参：无
*	返 回 值: 无
*   说    明：第一次任务切换前，16个寄存器的存储
*                  |-----------------| stack[0]
*                  |     ....        |
*                  |-----------------|
*                  |     ....        |
*                  |-----------------|      
*      低地址      |     ....        |       
*                  |-----------------|       
*         ^        |       R4        |   
*         ^        |-----------------|           
*         ^        |       R5        |          
*         |        |-----------------|           
*         |        |       R6        |          
*         |        |-----------------|           
*         |        |       R7        |           
*         |        |-----------------|            
*         |        |       R8        |                
*         |        |-----------------|                
*         |        |       R9        |
*         |        |-----------------|
*         |        |      R10        |
*         |        |-----------------|
*         |        |      R11        |
*         |        |-----------------|
*         |        |    R0 = p_arg   |  
*         |        |-----------------|
*         |        |       R1        |
*         |        |-----------------|
*         |        |       R2        |
*         |        |-----------------|
*         |        |       R3        |
*         |        |-----------------|
*         |        |       R12       |
*         |        |-----------------|
*         |        |       LR        |
*         |        |-----------------|
*         |        |    PC = task    |
*         |        |-----------------|
*         |        |      xPSR       |
*     高地址       |-----------------|  stack[1024]
*   
*   这里注意一点，执行第一个任务时PSP是指向任务的栈顶，后面三个任务第一次执行都是下面
*   初始化时设置的位置。            
*********************************************************************************************************
*/
void OS_Start(void) 
{
	/* 创建任务0的堆栈 */
	PSP_array[0] = ((unsigned int) task0_stack) + (sizeof task0_stack) - 16*4;
	HW32_REG((PSP_array[0] + (14<<2))) = (unsigned long) task0; /* PC */
	HW32_REG((PSP_array[0] + (15<<2))) = 0x01000000;            /* xPSR */
		
	/* 创建任务1的堆栈 */
	PSP_array[1] = ((unsigned int) task1_stack) + (sizeof task1_stack) - 16*4;
	HW32_REG((PSP_array[1] + (14<<2))) = (unsigned long) task1; /* PC */
	HW32_REG((PSP_array[1] + (15<<2))) = 0x01000000;            /* xPSR */

	/* 创建任务2的堆栈 */
	PSP_array[2] = ((unsigned int) task2_stack) + (sizeof task2_stack) - 16*4;
	HW32_REG((PSP_array[2] + (14<<2))) = (unsigned long) task2; /* PC */
	HW32_REG((PSP_array[2] + (15<<2))) = 0x01000000;            /* xPSR */

	/* 创建任务3的堆栈 */
	PSP_array[3] = ((unsigned int) task3_stack) + (sizeof task3_stack) - 16*4;
	HW32_REG((PSP_array[3] + (14<<2))) = (unsigned long) task3; /* PC */
	HW32_REG((PSP_array[3] + (15<<2))) = 0x01000000;            /* xPSR */
	
	/* 任务0先执行 */
	curr_task = 0; 
	 
	/* 设置PSP指向任务0堆栈的栈顶 */
	__set_PSP((PSP_array[curr_task] + 16*4)); 
	
	/* 设置PendSV中断为最低优先级中断 */
	NVIC_SetPriority(PendSV_IRQn, 0xFF);
 	
	/* 使用堆栈指针，非特权级状态 */
	__set_CONTROL(0x3);
	
	/* 改变CONTROL后执行ISB (architectural recommendation) */
	__ISB();
	
	/* 启动任务0 */
	task0();  
}

/*
*********************************************************************************************************
*	函 数 名: PendSV_Handler
*	功能说明: 任务切换的实现，这里特别的注意一点：
*             当前任务的16个寄存器全部保存后，PSP指向最后一个寄存器。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
__asm void PendSV_Handler(void)
{ 
	// 保存当前任务的内容
	MRS    R0, PSP     // 得到PSP  R0 = PSP
	                   // xPSR, PC, LR, R12, R0-R3已自动保存
	STMDB  R0!,{R4-R11}// 保存R4-R11共8个寄存器得到当前任务堆栈
	LDR    R1,=__cpp(&curr_task)
	LDR    R2,[R1]     // 得到任务ID
	LDR    R3,=__cpp(&PSP_array)
	STR    R0,[R3, R2, LSL #2] // 保存PSP到相应的PSP_array中 R0 = [R3 + R2 << 2]
                               // 左移两位是因为PSP_array是4字节的数组。
	
	// 加载下一个任务的内容
	LDR    R4,=__cpp(&next_task)
	LDR    R4,[R4]     // 得到下一个任务的ID
	STR    R4,[R1]     // 设置 curr_task = next_task
	LDR    R0,[R3, R4, LSL #2] // 从PSP_array中获取PSP的值
	LDMIA  R0!,{R4-R11}// 将任务堆栈中的数值加载到R4-R11中
	MSR    PSP, R0     // 设置PSP指向此任务
	BX     LR          // 返回
                       // xPSR, PC, LR, R12, R0-R3会自动的恢复
	ALIGN  4
}

/*
*********************************************************************************************************
* Function Name : Test_TaskExchange
* Description	: 测试任务切换
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void Interrupt_100ms( void )
{
	if (isReady == false)
	{
		return;
	}

	switch(curr_task) 
	{
		case 0: 
			next_task=1; 
		    break;
		case 1: 
			next_task=2; 
		    break;
		case 2: 
			next_task=3; 
		    break;
		case 3: 
			next_task=0; 
		    break;
		default: 
			next_task=0;
			printf("ERROR:curr_task = %x\n", curr_task);
			stop_cpu;
			break; // 程序不该运行到这里
	}
	if (curr_task!=next_task)
	{ 
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk; // Set PendSV to pending
	}
}

/*
*********************************************************************************************************
* Function Name : Test_TaskExchange
* Description	: 测试任务切换
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void Test_TaskExchange( void )
{
	OS_Start();   

	/* 进入主程序循环体, 进入这里的话说明程序出错 */
	while (1)
	{
		stop_cpu;
	}
}
#endif

#if (TEST_TASK_EXCHANGE_FLOAT == 0x01U)

/* 字访问 */
#define HW32_REG(ADDRESS)  (*((volatile unsigned long  *)(ADDRESS)))
/* 当检测到错误时，用Breakpoint来停止任务(MDK特有的)*/
/* 根据需要可以改成用while(1)来实现 */
#define stop_cpu   __breakpoint(0)

// 计数，用于切换任务
volatile uint32_t systick_count=0;
// 任务堆栈 (8Kbytes each - 1024 x 8 bytes) 这里也可以采用4字节，并将双字的堆栈对齐方式关闭
uint32_t task0_stack[1024], task1_stack[1024], task2_stack[1024], task3_stack[1024];

// 任务切换用到的数据 
uint32_t  curr_task=0;     // 当前执行任务
uint32_t  next_task=1;     // 下一个任务
uint32_t  PSP_array[4];    // 用于在任务切换时记录PSP
uint32_t  svc_exc_return;  // EXC_RETURN use by SVC

void __svc(0x00) OS_Start(void); // OS 初始化

static bool isReady;

/*
*********************************************************************************************************
*	函 数 名: task0
*	功能说明: 任务0实现LED1的闪烁
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void task0(void) 
{ 
	float Temp = 1.111111f;

	while (1) 
	{
		Temp = Temp + 0.000001f;
		printf("Temp = %f\r\n", Temp); 
		isReady = true;
	}
}

/*
*********************************************************************************************************
*	函 数 名: task0
*	功能说明: 任务0实现LED1的闪烁
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void task1(void) 
{ 
	while (1) 
	{
		LCD_DrawCircle(100,200,5,CL_GREEN);
	}
}

/*
*********************************************************************************************************
*	函 数 名: task0
*	功能说明: 任务0实现LED1的闪烁
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void task2(void) 
{ 
	while (1) 
	{
		LCD_DrawCircle(100,300,5,CL_BLUE);
	}
}

/*
*********************************************************************************************************
*	函 数 名: task3
*	功能说明: 任务0实现LED1的闪烁
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void task3(void) 
{ 
	while (1) 
	{
		LCD_DrawCircle(100,400,5,CL_YELLOW);
	}
}

/*
*********************************************************************************************************
*	函 数 名: SVC_Handler_C
*	功能说明: 在SVC异常中运行的C代码
*	形    参：无
*	返 回 值: 无
*   说    明：寄存器在堆栈中的存储顺序
*             *                  |-----------------| stack[0]
*                  |     ....        |
*                  |-----------------|
*                  |     ....        |
*                  |-----------------|      
*      低地址      |     ....        |       
*                  |-----------------|       
*         ^        |       R4        |   
*         ^        |-----------------|           
*         ^        |       R5        |          
*         |        |-----------------|           
*         |        |       R6        |          
*         |        |-----------------|           
*         |        |       R7        |           
*         |        |-----------------|            
*         |        |       R8        |                
*         |        |-----------------|                
*         |        |       R9        |
*         |        |-----------------|
*         |        |      R10        |
*         |        |-----------------|
*         |        |      R11        |
*         |        |-----------------|
*         |        |    R0 = p_arg   |  
*         |        |-----------------|
*         |        |       R1        |
*         |        |-----------------|
*         |        |       R2        |
*         |        |-----------------|
*         |        |       R3        |
*         |        |-----------------|
*         |        |       R12       |
*         |        |-----------------|
*         |        |       LR        |
*         |        |-----------------|
*         |        |    PC = task    |
*         |        |-----------------|
*         |        |      xPSR       |
*      高地址      |-----------------| 
*         |        |    CONTROL      |
*         |        |-----------------|
*         |        |    EXC_RETURN   | stack[1024]
*         |        |-----------------|
*********************************************************************************************************
*/
void SVC_Handler_C(unsigned int * svc_args)
{
	uint8_t svc_number;	
	svc_number = ((char *) svc_args[6])[-2]; // Memory[(Stacked PC)-2]
	switch(svc_number) 
	{
		/* 开始任务调度 */
		case (0): 
			
			/* 创建任务0的堆栈 */
			PSP_array[0] = ((unsigned int) task0_stack) + (sizeof task0_stack) - 18*4;
			HW32_REG((PSP_array[0] + (16<<2))) = (unsigned long) task0; // 初始化PC
			HW32_REG((PSP_array[0] + (17<<2))) = 0x01000000;            // 初始化 xPSR
			HW32_REG((PSP_array[0]          )) = 0xFFFFFFFDUL;          // 初始化 EXC_RETURN
			HW32_REG((PSP_array[0] + ( 1<<2))) = 0x3;// 初始化 CONTROL : unprivileged, PSP, no FP

			/* 创建任务1的堆栈 */
			PSP_array[1] = ((unsigned int) task1_stack) + (sizeof task1_stack) - 18*4;
			HW32_REG((PSP_array[1] + (16<<2))) = (unsigned long) task1; // 初始化 Program Counter
			HW32_REG((PSP_array[1] + (17<<2))) = 0x01000000;            // 初始化 xPSR
			HW32_REG((PSP_array[1]          )) = 0xFFFFFFFDUL;          // 初始化 EXC_RETURN
			HW32_REG((PSP_array[1] + ( 1<<2))) = 0x3;// 初始化 CONTROL : unprivileged, PSP, no FP

			/* 创建任务2的堆栈 */
			PSP_array[2] = ((unsigned int) task2_stack) + (sizeof task2_stack) - 18*4;
			HW32_REG((PSP_array[2] + (16<<2))) = (unsigned long) task2; // 初始化 Program Counter
			HW32_REG((PSP_array[2] + (17<<2))) = 0x01000000;            // 初始化 xPSR
			HW32_REG((PSP_array[2]          )) = 0xFFFFFFFDUL;          // 初始化 EXC_RETURN
			HW32_REG((PSP_array[2] + ( 1<<2))) = 0x3;// 初始化 CONTROL : unprivileged, PSP, no FP

			/* 创建任务3的堆栈 */
			PSP_array[3] = ((unsigned int) task3_stack) + (sizeof task3_stack) - 18*4;
			HW32_REG((PSP_array[3] + (16<<2))) = (unsigned long) task3; // 初始化 Program Counter
			HW32_REG((PSP_array[3] + (17<<2))) = 0x01000000;            // 初始化 xPSR
			HW32_REG((PSP_array[3]          )) = 0xFFFFFFFDUL;          // 初始化 EXC_RETURN
			HW32_REG((PSP_array[3] + ( 1<<2))) = 0x3;// 初始化 CONTROL : unprivileged, PSP, no FP

			curr_task = 0; // 切换到 task #0 
			svc_exc_return = HW32_REG((PSP_array[curr_task])); // 返回线程模式，PSP
			__set_PSP((PSP_array[curr_task] + 10*4));  // PSP = R0
			NVIC_SetPriority(PendSV_IRQn, 0xFF); // 设置PSP为最低优先级
			__set_CONTROL(0x3);                  // 切换到使用PSP,非特权级
			__ISB();       
			break;
		default:
			puts ("ERROR: Unknown SVC service number");
			printf("- SVC number 0x%x\n", svc_number);
			stop_cpu;
			break;
	} 
}

/*
*********************************************************************************************************
*	函 数 名: SVC_Handler
*	功能说明: SVC异常中断服务程序
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
__asm void SVC_Handler(void)
{
	TST    LR, #4                      // 获取使用的MSP还是PSP
	ITE    EQ
	MRSEQ  R0, MSP
	MRSNE  R0, PSP
	LDR    R1, =__cpp(&svc_exc_return) // 保存当前 EXC_RETURN
	STR    LR, [R1]	
	BL     __cpp(SVC_Handler_C)        // 运行 SVC_Handler 的C程序
	LDR    R1, =__cpp(&svc_exc_return) // 加载新的 EXC_RETURN
	LDR    LR, [R1]
	BX     LR
	ALIGN  4
}

/*
*********************************************************************************************************
*	函 数 名: PendSV_Handler
*	功能说明: 任务切换的实现，根据需要加载或者存储浮点寄存器
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
__asm void PendSV_Handler(void)
{ 
	
	// 保存当前任务的寄存器
	MRS      R0, PSP     			// 得到PSP  R0 = PSP
	TST      LR, #0x10   			// 检测bit 4. 如果是0的话，需要保存浮点寄存器
	IT       EQ
	VSTMDBEQ R0!, {S16-S31} 		// 将浮点寄存器入栈
	MOV      R2, LR
	MRS      R3, CONTROL            // R3 = CONTROL
	STMDB    R0!,{R2-R11}			// 保存 LR,CONTROL and R4 to R11 in task stack (10 个寄存器)
	LDR      R1,=__cpp(&curr_task)
	LDR      R2,[R1]     			// 得到当前任务ID
	LDR      R3,=__cpp(&PSP_array)
	STR      R0,[R3, R2, LSL #2]    // 保存PSP到相应的PSP_array中


	// 加载下一个任务堆栈中的数据到相应寄存器
	LDR      R4,=__cpp(&next_task)
	LDR      R4,[R4]     			// 得到下一个任务 ID
	STR      R4,[R1]     			// 设置 curr_task = next_task
	LDR      R0,[R3, R4, LSL #2]    // 从 PSP_array 中加载PSP
	LDMIA    R0!,{R2-R11}			// 加载 LR, CONTROL 和 R4 - R11 从任务堆栈中 (10 个寄存器)
	MOV      LR, R2
	MSR      CONTROL, R3            // CONTROL = R3
	TST      LR, #0x10   			// 检测bit 4. 如果是0的话，需要保存浮点寄存器
	IT       EQ
	VLDMIAEQ R0!, {S16-S31} 		// 加载浮点寄存器
	MSR      PSP, R0     			// PSP =R0
	BX       LR          			// 返回
	ALIGN  4
}

/*
*********************************************************************************************************
* Function Name : Test_TaskExchange
* Description	: 测试任务切换
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void Interrupt_1ms( void )
{
	if (isReady == false)
	{
		return;
	}

	switch(curr_task) 
	{
		case 0: 
			next_task=1; 
		    break;
		case 1: 
			next_task=2; 
		    break;
		case 2: 
			next_task=3; 
		    break;
		case 3: 
			next_task=0; 
		    break;
		default: 
			next_task=0;
			printf("ERROR:curr_task = %x\n", curr_task);
			stop_cpu;
			break; // 程序不该运行到这里
	}
	if (curr_task!=next_task)
	{ 
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk; // Set PendSV to pending
	}
}

/*
*********************************************************************************************************
* Function Name : Test_TaskExchangeFloat
* Description	: 测试任务切换
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void Test_TaskExchangeFloat( void )
{
	OS_Start();

	/* 进入主程序循环体, 进入这里的话说明程序出错 */
	while (1)
	{
		stop_cpu;
	}
}
#endif

#if TEST_TASK_UCOS_FLOAT
/*
*********************************************************************************************************
* Function Name : AppTaskGUI
* Description	: GUI任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  AppTaskGUI (void *p_arg)
{
	(void)p_arg;
	float t = 0.0002;
	
	while (1)
	{
		t += 0.0011f;
	
		BSP_OS_TimeDlyMs(1);
	}
}

/*
*********************************************************************************************************
* Function Name : AppTaskGUIRefresh
* Description	: GUI刷新任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  AppTaskGUIRefresh (void *p_arg)
{
	(void)p_arg;
	double t = 1;
	
	while (1)
	{
		t += 0.0001;
	
		BSP_OS_TimeDlyMs(1);
	}
}
#endif

#if TEST_TASK_UCOS_TASKQ
/*
*********************************************************************************************************
* Function Name : AppTaskUserIF
* Description	: USER IF
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  AppTaskUserIF (void *p_arg)
{
	(void)p_arg;
	uint32_t 	keyValue;
	OS_ERR      err;
	
	while (1)
	{
		keyValue = ReadKeyValue(g_keyPtr);
		if (keyValue != KEY_NONE)
		{
			if (ReadKeyStatus(g_keyPtr) == STATUS_KEY_PRESS)
			{
				OSTaskQPost(AppTaskUpdateTCB_Ptr,
							&keyValue,
							sizeof(keyValue),
							OS_OPT_POST_FIFO,
							&err);
			}
			
			ReleaseKeyMsg(g_keyPtr);
		}
		
		BSP_OS_TimeDlyMs(20);
	}
}

/*
*********************************************************************************************************
* Function Name : AppTaskGUIUpdate
* Description	: GUI更新任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  AppTaskGUIUpdate (void *p_arg)
{
	(void)p_arg;
	OS_MSG_SIZE msgSize;
	OS_ERR      err;
	CPU_TS ts;
	uint32_t keyValue;
	void *pMsg;
	
	while (1)
	{
		pMsg = OSTaskQPend(	0,
							OS_OPT_PEND_BLOCKING,
							&msgSize,
							&ts,
							&err);
		
		if (err == OS_ERR_NONE)
		{
			keyValue = *(uint32_t *)pMsg;
			
			printf("keyValue = %d\r\n", keyValue);
		}
	}
}
#endif

#if (TEST_TASK_UCOS_SEM_POST)
/*
*********************************************************************************************************
* Function Name : AppTaskCOM
* Description	: USER COM
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  AppTaskCOM (void *p_arg)
{
	(void)p_arg;
	uint32_t 	keyValue;
	OS_ERR      err;
	
	while (1)
	{
		OSTaskSemPost(	AppTaskGUIRefreshTCB_Ptr,
						OS_OPT_POST_NONE,
						&err);
	
		BSP_OS_TimeDlyMs(1000);
	}
}

/*
*********************************************************************************************************
* Function Name : AppTaskGUIRefresh
* Description	: GUI刷新任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  AppTaskGUIRefresh (void *p_arg)
{
	(void)p_arg;
	OS_MSG_SIZE msgSize;
	OS_ERR      err;
	CPU_TS ts;
	
	while (1)
	{
		OSTaskSemPend(	0,
						OS_OPT_PEND_BLOCKING,
						&ts,
						&err);
		
		if (err == OS_ERR_NONE)
		{
			DispTaskInfo();
		}
	}
}
#endif

#if (TEST_TASK_UCOS_SEM)
BSP_OS_SEM SEM_SYNC;
BSP_OS_SEM SEM_MUTEX;

/*
*********************************************************************************************************
* Function Name : AppObjCreate
* Description	: 创建任务通讯
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  AppObjCreate (void)
{
	/* 创建同步信号量 */
	BSP_OS_SemCreate(&SEM_SYNC,
					 DEF_FAIL,
					 (CPU_CHAR *)"SEM_SYNC");
	
	/* 创建互斥信号量 */
	BSP_OS_SemCreate(&SEM_MUTEX,
					 DEF_OK,
					 (CPU_CHAR *)"SEM_MUTEX");
}

/*
*********************************************************************************************************
* Function Name : AppTaskUserIF
* Description	: USER IF
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  AppTaskUserIF (void *p_arg)
{
	(void)p_arg;
	uint32_t 	keyValue;
	
	while (1)
	{
		keyValue = ReadKeyValue(g_keyPtr);
		if (keyValue != KEY_NONE)
		{
			if (ReadKeyStatus(g_keyPtr) == STATUS_KEY_PRESS)
			{
				BSP_OS_SemPost(&SEM_SYNC);
			}
			
			ReleaseKeyMsg(g_keyPtr);
		}
		
		BSP_OS_TimeDlyMs(20);
	}
}

/*
*********************************************************************************************************
* Function Name : AppTaskGUIUpdate
* Description	: GUI更新任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  AppTaskGUIUpdate (void *p_arg)
{
	(void)p_arg;
	CPU_BOOLEAN flag;
	
	while (1)
	{
		flag = BSP_OS_SemWait(&SEM_SYNC,
							  0);	
		if (flag == DEF_OK)
		{
			DispTaskInfo();		
		}
	}
}

/*
*********************************************************************************************************
* Function Name : AppTaskCOM
* Description	: USER COM
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  AppTaskCOM (void *p_arg)
{
	(void)p_arg;
	
	while (1)
	{
		BSP_OS_SemWait(&SEM_MUTEX,0);
		printf("TASK：COM\r\n");
		
		BSP_OS_SemPost(&SEM_MUTEX);
		BSP_OS_TimeDlyMs(1000);
	}
}

/*
*********************************************************************************************************
* Function Name : AppTaskGUIRefresh
* Description	: GUI刷新任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  AppTaskGUIRefresh (void *p_arg)
{
	(void)p_arg;
	
	while (1)
	{
		BSP_OS_SemWait(&SEM_MUTEX,0);
		printf("TASK：GUI\r\n");
		
		BSP_OS_SemPost(&SEM_MUTEX);
		BSP_OS_TimeDlyMs(1000);
	}
}
#endif

#if (TEST_TASK_UCOS_MSG_QUEUE)
OS_Q MSG_QUEUE;

/*
*********************************************************************************************************
* Function Name : AppObjCreate
* Description	: 创建任务通讯
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  AppObjCreate (void)
{
	OS_ERR err;
	
	/* 创建消息队列 */
	OSQCreate(&MSG_QUEUE,
			 (CPU_CHAR *)"MSG_QUEUE",
			 5,
			 &err);
}

/*
*********************************************************************************************************
* Function Name : AppTaskCOM
* Description	: USER COM
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  AppTaskCOM (void *p_arg)
{
	(void)p_arg;
	uint32_t keyValue;
	OS_ERR err;
	char buff[5];
	uint8_t value = 0;
	
	while (1)
	{
		keyValue = ReadKeyValue(g_keyPtr);
		if (keyValue != KEY_NONE)
		{
			if (ReadKeyStatus(g_keyPtr) == STATUS_KEY_PRESS)
			{
				buff[0] = value;
				buff[1] = value+1;
				buff[2] = value+2;
				buff[3] = value+3;
				buff[4] = value+4;
				value++;
				
				OSQPost(&MSG_QUEUE,
						(void *)buff,
						5,
						OS_OPT_POST_FIFO,
						&err);
			}
			
			ReleaseKeyMsg(g_keyPtr);
		}	

		BSP_OS_TimeDlyMs(20);	
	}
}

/*
*********************************************************************************************************
* Function Name : AppTaskGUIRefresh
* Description	: GUI刷新任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  AppTaskGUIRefresh (void *p_arg)
{
	(void)p_arg;
	OS_MSG_SIZE msgSize;
	CPU_TS ts;
	OS_ERR err;
	void *msgBody;
	char buff[10];
	
	while (1)
	{
		msgBody = OSQPend(&MSG_QUEUE,
						   0,
						   OS_OPT_PEND_BLOCKING,
						   &msgSize,
						   &ts,
						   &err);
		if (err == OS_ERR_NONE)
		{
			memcpy(buff,msgBody,msgSize);
			for (uint8_t i=0; i<msgSize; ++i)
			{
				printf("msg = %d\r\n",buff[i]);
			}
		}
	}
}
#endif

#if (TEST_TASK_UCOS_EVENT_FLAG)
OS_FLAG_GRP FLAG_GROUP;

/*
*********************************************************************************************************
* Function Name : AppObjCreate
* Description	: 创建任务通讯
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  AppObjCreate (void)
{
	OS_ERR err;
	
	/* 创建事件标志组 */
	OSFlagCreate(&FLAG_GROUP,
				 "FLAG GROUP",
				 0,
				 &err);
}

/*
*********************************************************************************************************
* Function Name : AppTaskCOM
* Description	: USER COM
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  AppTaskCOM (void *p_arg)
{
	(void)p_arg;
	uint32_t keyValue;
	OS_ERR err;
	
	while (1)
	{
		keyValue = ReadKeyValue(g_keyPtr);
		if (keyValue != KEY_NONE)
		{
			if (ReadKeyStatus(g_keyPtr) == STATUS_KEY_PRESS)
			{
				OSFlagPost(&FLAG_GROUP,
						   0x4,
						   OS_OPT_POST_FLAG_SET,
						   &err);
			}
			
			ReleaseKeyMsg(g_keyPtr);
		}	

		BSP_OS_TimeDlyMs(20);	
	}
}

/*
*********************************************************************************************************
* Function Name : AppTaskUserIF
* Description	: USER IF
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  AppTaskUserIF (void *p_arg)
{
	(void)p_arg;
	OS_ERR err;
	
	while (1)
	{
		OSFlagPost(&FLAG_GROUP,
						   0x1,
						   OS_OPT_POST_FLAG_SET,
						   &err);
		
		BSP_OS_TimeDlyMs(1000);
	}
}

/*
*********************************************************************************************************
* Function Name : AppTaskGUIRefresh
* Description	: GUI刷新任务（只有等到一组事件都完成，才发出通知）
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  AppTaskGUIRefresh (void *p_arg)
{
	(void)p_arg;
	OS_MSG_SIZE msgSize;
	OS_ERR err;
	
	while (1)
	{
		OSFlagPend(&FLAG_GROUP,
				   0x5,
				   0,
				   OS_OPT_PEND_FLAG_SET_ALL | OS_OPT_PEND_FLAG_CONSUME,
				   0,
				   &err);
		printf("收到事件标志组！\r\n");
	}
}
#endif

#if (TEST_TASK_UCOS_TIMER)
OS_TMR TIMER;

/*
*********************************************************************************************************
* Function Name : Timer_Callback
* Description	: 定时事件到达回调函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  Timer_Callback (void *p_tmr, void *p_arg)
{
	printf("定时事件到！\r\n");
}

/*
*********************************************************************************************************
* Function Name : AppObjCreate
* Description	: 创建任务通讯(定时周期100ms)
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  AppObjCreate (void)
{
	OS_ERR err;
	
	/* 创建定时器 */
	OSTmrCreate(&TIMER,
				"TIMER",
				50,
				10,
				OS_OPT_TMR_PERIODIC,
				Timer_Callback,
				NULL,
				&err);
	OSTmrStart(&TIMER,&err);
}
#endif

#if (TEST_TASK_UCOS_WAIT_MORE_EVENT)
OS_Q MSG_QUEUE;
BSP_OS_SEM SEM_SYNC;

/*
*********************************************************************************************************
* Function Name : AppObjCreate
* Description	: 创建任务通讯
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  AppObjCreate (void)
{
	OS_ERR err;
	
	/* 创建消息队列 */
	OSQCreate(&MSG_QUEUE,
			 (CPU_CHAR *)"MSG_QUEUE",
			 5,
			 &err);
	
	/* 创建同步信号量 */
	BSP_OS_SemCreate(&SEM_SYNC,
					 DEF_FAIL,
					 (CPU_CHAR *)"SEM_SYNC");
	
}

/*
*********************************************************************************************************
* Function Name : AppTaskCOM
* Description	: USER COM
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  AppTaskCOM (void *p_arg)
{
	(void)p_arg;
	uint32_t keyValue;
	OS_ERR err;
	char buff[5];
	uint8_t value = 0;
	
	while (1)
	{
		keyValue = ReadKeyValue(g_keyPtr);
		if (keyValue != KEY_NONE)
		{
			if (ReadKeyStatus(g_keyPtr) == STATUS_KEY_PRESS)
			{
				if (keyValue == KEY_F1)
				{
					buff[0] = value;
					buff[1] = value+1;
					buff[2] = value+2;
					buff[3] = value+3;
					buff[4] = value+4;
					value++;
					
					OSQPost(&MSG_QUEUE,
							(void *)buff,
							5,
							OS_OPT_POST_FIFO,
							&err);
				} 
				else if (keyValue == KEY_F2)
				{
					BSP_OS_SemPost(&SEM_SYNC);				
				}
			}
			
			ReleaseKeyMsg(g_keyPtr);
		}	

		BSP_OS_TimeDlyMs(20);	
	}
}

/*
*********************************************************************************************************
* Function Name : AppTaskGUIRefresh
* Description	: GUI刷新任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  AppTaskGUIRefresh (void *p_arg)
{
	(void)p_arg;
	OS_ERR err;
	char buff[10];
	OS_PEND_DATA pendDataTLB[2];
	
	while (1)
	{
		pendDataTLB[0].PendObjPtr = (OS_PEND_OBJ *)&MSG_QUEUE;
		pendDataTLB[1].PendObjPtr = (OS_PEND_OBJ *)&SEM_SYNC;
		
		OSPendMulti(pendDataTLB,
					2,
					0,
					OS_OPT_PEND_BLOCKING,
					&err);
		
		if (err == OS_ERR_NONE)
		{
			if (pendDataTLB[0].RdyObjPtr == (OS_PEND_OBJ *)&MSG_QUEUE)
			{				
				memcpy(buff,pendDataTLB[0].RdyMsgPtr,pendDataTLB[0].RdyMsgSize);
				for (uint8_t i=0; i<pendDataTLB[0].RdyMsgSize; ++i)
				{
					printf("msg = %d\r\n",buff[i]);
				}
			}
			
			if (pendDataTLB[1].RdyObjPtr == (OS_PEND_OBJ *)&SEM_SYNC)
			{
				printf("收到同步信号量！\r\n");
			}
		}
	}
}
#endif

#if (TEST_TASK_UCOS_MEMORY_MANAGER)
OS_Q MSG_QUEUE;
OS_MEM MEM_BODY;
uint8_t g_MemPool[5][4];

/*
*********************************************************************************************************
* Function Name : AppObjCreate
* Description	: 创建任务通讯
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  AppObjCreate (void)
{
	OS_ERR err;
	
	/* 创建消息队列 */
	OSQCreate(&MSG_QUEUE,
			 (CPU_CHAR *)"MSG_QUEUE",
			 5,
			 &err);
	OSMemCreate(&MEM_BODY,
				"MEMORY MANAGER",
				g_MemPool,
				5,
				4 * sizeof(uint8_t),
				&err);
}

/*
*********************************************************************************************************
* Function Name : AppTaskCOM
* Description	: USER COM
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  AppTaskCOM (void *p_arg)
{
	(void)p_arg;
	uint32_t keyValue;
	OS_ERR err;
	uint8_t value = 0;
	
	uint8_t *memPtr = OSMemGet(&MEM_BODY,
							   &err);
	if ((memPtr == NULL) || (err != OS_ERR_NONE))
	{
		while (1);
	}
	
	while (1)
	{
		keyValue = ReadKeyValue(g_keyPtr);
		if (keyValue != KEY_NONE)
		{
			if (ReadKeyStatus(g_keyPtr) == STATUS_KEY_PRESS)
			{
				memPtr[0] = value;
				memPtr[1] = value+1;
				memPtr[2] = value+2;
				memPtr[3] = value+3;
				memPtr[4] = value+4;
				value++;
				
				OSQPost(&MSG_QUEUE,
						(void *)memPtr,
						5,
						OS_OPT_POST_FIFO,
						&err);
			}
			
			ReleaseKeyMsg(g_keyPtr);
		}	

		BSP_OS_TimeDlyMs(20);	
	}
}

/*
*********************************************************************************************************
* Function Name : AppTaskGUIRefresh
* Description	: GUI刷新任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  AppTaskGUIRefresh (void *p_arg)
{
	(void)p_arg;
	OS_MSG_SIZE msgSize;
	CPU_TS ts;
	OS_ERR err;
	void *msgBody;
	char buff[10];
	
	while (1)
	{
		msgBody = OSQPend(&MSG_QUEUE,
						   0,
						   OS_OPT_PEND_BLOCKING,
						   &msgSize,
						   &ts,
						   &err);
		if (err == OS_ERR_NONE)
		{
			memcpy(buff,msgBody,msgSize);
			for (uint8_t i=0; i<msgSize; ++i)
			{
				printf("msg = %d\r\n",buff[i]);
			}
		}
	}
}
#endif

#if (TEST_TASK_UCOS_ROUND_ROBIN)
/*
*********************************************************************************************************
* Function Name : AppTaskGUIRefresh
* Description	: GUI刷新任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  AppTaskGUIRefresh (void *p_arg)
{
	(void)p_arg;
	
	while (1)
	{
		for (uint32_t i=0; i<0xFFFFFFFF; ++i)
		{
			for (uint32_t j=0; j<0xFFFFFFFF; ++j)
			{
				float a = 0;
				a++;
			}
		}
		
		BSP_OS_TimeDlyMs(1);
	}
}
#endif

#if (TEST_TASK_UCOS_EMWIN)

#define GUI_FontHZ_SimSun_1616 g_XBF_Font
/*
*********************************************************************************************************
*				                         变量
*********************************************************************************************************
*/
static const char *TaskProfile[7]={" "," "," "," "," "," "," "};
static GRAPH_SCALE_Handle hScaleV;     
static GRAPH_DATA_Handle  ahData[1];   /* GRAPH_DATA 的句柄 */
static GUI_COLOR _aColor[] = {GUI_BLUE};

/*
*********************************************************************************************************
*				                         对话框初始化选项
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { FRAMEWIN_CreateIndirect,  "uCOS-III V3.03.01",           0,     0,  0,  480,272,0,0},
};

/*
*********************************************************************************************************
*	函 数 名: InitDialog
*	功能说明: 初始化函数 
*	形    参：pMsg
*	返 回 值: 无
*********************************************************************************************************
*/
void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    //
    //FRAMEWIN
    //
	FRAMEWIN_SetTextColor(hWin,0x0000ff);
	FRAMEWIN_SetFont(hWin,&GUI_FontHZ_SimSun_1616);
    FRAMEWIN_SetTextAlign(hWin,GUI_TA_VCENTER|GUI_TA_CENTER);
    FRAMEWIN_SetTitleHeight(hWin,25);
}

/*
*********************************************************************************************************
*	函 数 名: _cbCallback
*	功能说明: 主对话框回调函数 
*	形    参：pMsg
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbCallback(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
		/* 重绘消息 */
        case WM_PAINT:
            break;
		
		/* 初始化消息 */
        case WM_INIT_DIALOG:
            InitDialog(pMsg);
            break;
		
		/* 按键消息 */
        case WM_KEY:
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
            {
                case GUI_KEY_ESCAPE:
                    GUI_EndDialog(hWin, 1);
                    break;
                case GUI_KEY_ENTER:
                    GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
		
		/* 通知消息 */
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (Id) 
            {
                case GUI_ID_OK:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
                case GUI_ID_CANCEL:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
            }
            break;

        default:
            WM_DefaultProc(pMsg);
    }
}

/*
*********************************************************************************************************
*				                         任务对话框初始化选项
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateTask[] = {
  { WINDOW_CreateIndirect,    NULL,              0,                   0,   0, 479, 271, FRAMEWIN_CF_MOVEABLE },
  { LISTVIEW_CreateIndirect,   NULL,             GUI_ID_LISTVIEW0,        0,  0,  474,254,0,0}
};

/*
*********************************************************************************************************
*	函 数 名: InitDialog
*	功能说明: 初始化函数 
*	形    参：pMsg
*	返 回 值: 无
*********************************************************************************************************
*/
void InitDialogTask(WM_MESSAGE * pMsg)
{
	WM_HWIN hWin = pMsg->hWin;
	unsigned char i;
	HEADER_Handle hHeader;

    //
    //GUI_ID_LISTVIEW0
    //
	hHeader = LISTVIEW_GetHeader(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0));
	HEADER_SetFont(hHeader,&GUI_FontHZ_SimSun_1616);
	HEADER_SetHeight(hHeader,20);

    LISTVIEW_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0),0,0x000000);
    LISTVIEW_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0),0,0x00ff00);
    LISTVIEW_SetFont(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0),&GUI_FontHZ_SimSun_1616);

    LISTVIEW_AddColumn(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0),60,"优先级",GUI_TA_VCENTER|GUI_TA_LEFT);
    LISTVIEW_AddColumn(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0),60,"堆栈使用",GUI_TA_VCENTER|GUI_TA_LEFT);
    LISTVIEW_AddColumn(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0),60,"堆栈剩余",GUI_TA_VCENTER|GUI_TA_LEFT);
    LISTVIEW_AddColumn(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0),60,"堆栈比",GUI_TA_VCENTER|GUI_TA_LEFT);
    LISTVIEW_AddColumn(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0),60,"CPU",GUI_TA_VCENTER|GUI_TA_LEFT);

    LISTVIEW_AddColumn(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0),100,"名字",GUI_TA_VCENTER|GUI_TA_LEFT);
    
	LISTVIEW_SetColumnWidth(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), 0, 50);
	LISTVIEW_SetColumnWidth(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), 1, 70);
	LISTVIEW_SetColumnWidth(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), 2, 70);
	LISTVIEW_SetColumnWidth(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), 3, 55);
	LISTVIEW_SetColumnWidth(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), 4, 60);
	LISTVIEW_SetColumnWidth(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), 5, 170);
	
	for(i = 0; i < OSTaskQty; i++)
	{
		LISTVIEW_AddRow(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), (GUI_ConstString *)TaskProfile);
		LISTVIEW_SetItemText(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), 0, i, "1");		
		LISTVIEW_SetItemText(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), 1, i, "2");
		LISTVIEW_SetItemText(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), 2, i, "3");		
		LISTVIEW_SetItemText(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), 3, i, "4");
		LISTVIEW_SetItemText(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), 4, i, "5");		
		LISTVIEW_SetItemText(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), 5, i, "6");					
	}
}

/*
*********************************************************************************************************
*	函 数 名: _cbCallback
*	功能说明: 任务对话框回调函数 
*	形    参：pMsg
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbCallbackTask(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
        /* 重绘消息 */
        case WM_PAINT:
            break;
		
		/* 初始化消息 */
        case WM_INIT_DIALOG:
            InitDialogTask(pMsg);
            break;
		
		/* 按键消息 */
        case WM_KEY:
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
            {
                case GUI_KEY_ESCAPE:
                    GUI_EndDialog(hWin, 1);
                    break;
                case GUI_KEY_ENTER:
                    GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
		
		/* 通知消息 */
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (Id) 
            {
                case GUI_ID_OK:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
                case GUI_ID_CANCEL:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
            }
            break;

        default:
            WM_DefaultProc(pMsg);
    }
}

/*
*********************************************************************************************************
*				                         对话框初始化选项
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateCPU[] = {
   { WINDOW_CreateIndirect,    NULL,              0,                   0,   0, 479, 271, FRAMEWIN_CF_MOVEABLE },
   { GRAPH_CreateIndirect,     0,                 GUI_ID_GRAPH0,       0,   0, 466, 231, 0, 0}
};

/*
*********************************************************************************************************
*	函 数 名: InitDialog
*	功能说明: CPU利用率对话框初始化函数 
*	形    参：pMsg
*	返 回 值: 无
*********************************************************************************************************
*/
void InitDialogCPU(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
	WM_HWIN hItem;
	unsigned char i;

  	hItem = WM_GetDialogItem(hWin, GUI_ID_GRAPH0);

	for (i = 0; i < GUI_COUNTOF(_aColor); i++)
	{
		/* 创建成功的话，返回数据句柄 */
		/* 显示的曲线颜色，可以显示的最大数据个数，数据指针，要添加的数据个数 */
		ahData[i] = GRAPH_DATA_YT_Create(_aColor[i], 480, 0, 0);
		
		/* 为绘图控件添加数据对象 */
		GRAPH_AttachData(hItem, ahData[i]);
	}

	GRAPH_SetGridDistY(hItem, 20);
	
	/* 栅格是否可见 */
    GRAPH_SetGridVis(hItem, 1);
	
	/* 固定X轴的栅格 */
    GRAPH_SetGridFixedX(hItem, 1);

	/* 创建和增加垂直范围尺度标签  */
	/* 离左边的尺度位置，对齐方式，垂直或水平标签，标签的间距 */
    hScaleV = GRAPH_SCALE_Create( 20, GUI_TA_RIGHT, GRAPH_SCALE_CF_VERTICAL, 20);
	
	/* 用于设置比例因子 */
	GRAPH_SCALE_SetFactor(hScaleV, 0.5);
	
	/* 设置标签字体颜色 */
    GRAPH_SCALE_SetTextColor(hScaleV, GUI_RED);
	
	/* 将标签添加到垂直方向 */
    GRAPH_AttachScale(hItem, hScaleV);
	
    /* 用于设置左上右下边界 */
	GRAPH_SetBorder(hItem,25,0,0,10);

}

/*
*********************************************************************************************************
*	函 数 名: _cbCallback
*	功能说明: CPU利用率回调函数 
*	形    参：pMsg
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbCallbackCPU(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
        /* 重绘消息 */
        case WM_PAINT:
            break;
		
		/* 初始化消息 */
        case WM_INIT_DIALOG:
            InitDialogCPU(pMsg);
            break;
		
		/* 按键消息 */
        case WM_KEY:
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
            {
                case GUI_KEY_ESCAPE:
                    GUI_EndDialog(hWin, 1);
                    break;
                case GUI_KEY_ENTER:
                    GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
		
		/* 通知消息 */
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (Id) 
            {
                case GUI_ID_OK:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
                case GUI_ID_CANCEL:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
            }
            break;

        default:
            WM_DefaultProc(pMsg);
    }
}

/*
*********************************************************************************************************
*	函 数 名: MainTask
*	功能说明: GUI主函数 
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void MainTask(void) 
{ 
    WM_HWIN hDialog; 
	WM_HWIN hMultiPage, hDialogTemp;
	OS_TCB      *p_tcb;	        /* 定义一个任务控制块指针, TCB = TASK CONTROL BLOCK */
	float CPU;
	char buf[10],i;
	CPU_SR_ALLOC();
	
	OpenXBF("0:/SONG_16X16.xbf");

	/* 初始化 */ 
    WM_SetDesktopColor(GUI_BLUE); 
	
	/* 使能相关控件的皮肤 */
    PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	MULTIPAGE_SetDefaultSkin(MULTIPAGE_SKIN_FLEX);

	/* 创建对话框 */
    hDialog = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
	/* 创建多页控件 */
	hMultiPage = MULTIPAGE_CreateEx(0, 0, WM_GetWindowSizeX(WM_GetClientWindow(hDialog)), WM_GetWindowSizeY(WM_GetClientWindow(hDialog)), WM_GetClientWindow(hDialog), WM_CF_SHOW, 0, 0);
	/* 设置多页控件字体 */
	MULTIPAGE_SetFont(hMultiPage, &GUI_FontHZ_SimSun_1616);		
	/* 创建多页控件第一项 */
	hDialogTemp = GUI_CreateDialogBox(_aDialogCreateCPU, GUI_COUNTOF(_aDialogCreateCPU), &_cbCallbackCPU, WM_UNATTACHED, 0, 0);  		
 	MULTIPAGE_AddPage(hMultiPage, hDialogTemp, "性能");	
	/* 创建多页控件第二项 */
	hDialogTemp = GUI_CreateDialogBox(_aDialogCreateTask, GUI_COUNTOF(_aDialogCreateTask), &_cbCallbackTask, WM_UNATTACHED, 0, 0);   		
	MULTIPAGE_AddPage(hMultiPage, hDialogTemp, "进程");

	while(1)
	{
		/* 选择多页控件的第2页 */
		if(MULTIPAGE_GetSelection(hMultiPage) == 1)
		{
			CPU_CRITICAL_ENTER();
			p_tcb = OSTaskDbgListPtr;
			CPU_CRITICAL_EXIT();
			
			i = 0;
			/* 遍历任务控制块列表(TCB list)，打印所有的任务的优先级和名称 */
			while (p_tcb != (OS_TCB *)0) 
			{
				CPU = (float)p_tcb->CPUUsage / 100;
				sprintf(buf, "%d", p_tcb->Prio);
				LISTVIEW_SetItemText(WM_GetDialogItem(MULTIPAGE_GetWindow(hMultiPage, 1), GUI_ID_LISTVIEW0), 0, i, buf);
				sprintf(buf, "%d", p_tcb->StkUsed);
				LISTVIEW_SetItemText(WM_GetDialogItem(MULTIPAGE_GetWindow(hMultiPage, 1), GUI_ID_LISTVIEW0), 1, i, buf);
				sprintf(buf, "%d", p_tcb->StkFree);
				LISTVIEW_SetItemText(WM_GetDialogItem(MULTIPAGE_GetWindow(hMultiPage, 1), GUI_ID_LISTVIEW0), 2, i, buf);
				sprintf(buf, "%d%%", (p_tcb->StkUsed * 100) / (p_tcb->StkUsed + p_tcb->StkFree));
				LISTVIEW_SetItemText(WM_GetDialogItem(MULTIPAGE_GetWindow(hMultiPage, 1), GUI_ID_LISTVIEW0), 3, i, buf);
				sprintf(buf, "%5.2f%% ", CPU);
				LISTVIEW_SetItemText(WM_GetDialogItem(MULTIPAGE_GetWindow(hMultiPage, 1), GUI_ID_LISTVIEW0), 4, i, buf);
				sprintf(buf, "%s", p_tcb->NamePtr);	
				LISTVIEW_SetItemText(WM_GetDialogItem(MULTIPAGE_GetWindow(hMultiPage, 1), GUI_ID_LISTVIEW0), 5, i, buf);
								 	
				CPU_CRITICAL_ENTER();
			    p_tcb = p_tcb->DbgNextPtr;
			    CPU_CRITICAL_EXIT();
				i++;
			}	
		}
		/* 选择多页控件的第1页 */
		else
		{
			GRAPH_DATA_YT_AddValue(ahData[0], OSStatTaskCPUUsage/50);
		}
		
		/* 延迟时间不能太快，要不会造成刷新listiew控件的时候，这个任务的利用率很高 */
		GUI_Delay(300);	
	}
}
#endif

#if TEST_TASK_UCOS_EMWIN_FONT
/* 变量 */
GUI_XBF_DATA XBF_Data;
GUI_FONT     XBF_Font;
FIL          Fontfile;

/*
*********************************************************************************************************
*	函 数 名: _cbGetData
*	功能说明: 使用XBF字体
*	形    参: Off       要读取数据的起始位置
*             NumBytes  要读取的字节数
*             pVoid     应用定义指针，此处是文件句柄指针
*             pBuffer   用于存储字体数据的地址
*	返 回 值: 0 成功  1 失败
*********************************************************************************************************
*/
static int _cbGetData(U32 Off, U16 NumBytes, void * pVoid, void * pBuffer)
{
	FIL *FontFile;
	uint32_t bw;

	/* 文件句柄 */
	FontFile = (FIL *)pVoid;

	/*
	* 指向数据地址
	*/
	FRESULT result =f_lseek(FontFile, Off);
	if (result != FR_OK) 
	{
		return 1;
	}

	/*
	* 读取数据到buffer
	*/
	result = f_read(FontFile, pBuffer, NumBytes, &bw);
	if (result != FR_OK) 
	{
		return 1; 
	}
	
	return 0;

}

/*
*********************************************************************************************************
*	函 数 名: _ShowXBF
*	功能说明: 使用XBF字体
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void _ShowXBF(void) 
{
	FRESULT result = f_open(&Fontfile, "0:/fonts/SongTi_24X24.xbf", FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (result != FR_OK)
	{
		return;
	}

	/* 创建XBF字体 */
	GUI_XBF_CreateFont(&XBF_Font,       
					 &XBF_Data,        
					 GUI_XBF_TYPE_PROP_AA4_EXT, 
					 _cbGetData,      
					 &Fontfile);  
}

/*
*********************************************************************************************************
*										对话框信息
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { FRAMEWIN_CreateIndirect,  "armfly",            0,                       0,  0,  800,480,FRAMEWIN_CF_MOVEABLE,0},
    { BUTTON_CreateIndirect,    "浙江省杭州市西湖区三墩镇西湖科技园区西园五路",        GUI_ID_BUTTON0,          390,40,360,100,0,0},
    { TEXT_CreateIndirect,      "armfly",        GUI_ID_TEXT0,            5, 10, 300, 70, 0,0},
    { TEXT_CreateIndirect,      "armfly",        GUI_ID_TEXT1,            5, 80,300, 70, 0,0},
    { TEXT_CreateIndirect,      "armfly",        GUI_ID_TEXT2,            5, 160,300, 70, 0,0}
};

/*
*********************************************************************************************************
*	函 数 名: PaintDialog
*	功能说明: 重绘函数
*	形    参：pMsg 消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
void PaintDialog(WM_MESSAGE * pMsg)
{
//    WM_HWIN hWin = pMsg->hWin;
}

/*
*********************************************************************************************************
*	函 数 名: InitDialog
*	功能说明: 对话框初始化 
*	形    参: pMsg 消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    //
    //FRAMEWIN
    //
    FRAMEWIN_SetFont(hWin,&GUI_Font32B_ASCII);
    FRAMEWIN_SetTextAlign(hWin,GUI_TA_VCENTER|GUI_TA_CENTER);
    FRAMEWIN_AddCloseButton(hWin, FRAMEWIN_BUTTON_RIGHT, 0);
    FRAMEWIN_AddMaxButton(hWin, FRAMEWIN_BUTTON_RIGHT, 1);
    FRAMEWIN_AddMinButton(hWin, FRAMEWIN_BUTTON_RIGHT, 2);
    FRAMEWIN_SetTitleHeight(hWin,35);
	
    //
    //GUI_ID_BUTTON0
    //
	_ShowXBF();
	GUI_UC_SetEncodeUTF8();
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON0),&XBF_Font);
	
    //
    //GUI_ID_TEXT0
    //
	TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT0), GUI_RED);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT0),&XBF_Font);
	TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT1), GUI_GREEN);
	TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT1),&XBF_Font);
	TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT2), GUI_BLUE);
	TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT2),&XBF_Font);
	
// 	GUI_UC_SetEncodeNone();
}

/*
*********************************************************************************************************
*	函 数 名: _cbCallback
*	功能说明: 对话框回调函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbCallback(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            PaintDialog(pMsg);
            break;
        case WM_INIT_DIALOG:
            InitDialog(pMsg);
            break;
        case WM_KEY:
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
            {
                case GUI_KEY_ESCAPE:
                    GUI_EndDialog(hWin, 1);
                    break;
                case GUI_KEY_ENTER:
                    GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (Id) 
            {
                case GUI_ID_OK:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
                case GUI_ID_CANCEL:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
        default:
            WM_DefaultProc(pMsg);
    }
}

/*
*********************************************************************************************************
*	函 数 名: MainTask
*	功能说明: GUI主函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void MainTask(void) 
{ 
	/* 初始化 */
	GUI_Init();
    WM_SetDesktopColor(GUI_WHITE);      
    WM_SetCreateFlags(WM_CF_MEMDEV);     
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
    
	while(1)
	{
		uint32_t key = ReadKeyValue(g_keyPtr);
		if (key != KEY_NONE)
		{	
			if (ReadKeyStatus(g_keyPtr) == STATUS_KEY_PRESS)
			{
				GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
			}
			
			ReleaseKeyMsg(g_keyPtr);
		}
		
		GUI_Delay(10);
	}
}

#endif

#if (TEST_TASK_UCOS_USB)


/*
*********************************************************************************************************
*	函 数 名: MainTask
*	功能说明: GUI主函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void MainTask(void) 
{ 
	/* 初始化 */
	GUI_Init();
    WM_SetDesktopColor(GUI_WHITE);      
    WM_SetCreateFlags(WM_CF_MEMDEV);     
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
    
	while(1)
	{
		uint32_t key = ReadKeyValue(g_keyPtr);
		if (key != KEY_NONE)
		{	
			if (ReadKeyStatus(g_keyPtr) == STATUS_KEY_PRESS)
			{
				OS_ERR      err;
				FIL file;
				char buff[200] = "浙江省杭州市西湖科技园区杭州鑫高科技有限公司";
				uint32_t bw;
				
//				OSSchedLock(&err);
				
				FRESULT result = f_open(&file, "1:/123addddddddddaa.txt", FA_WRITE | FA_CREATE_ALWAYS);
				if (result != FR_OK)
				{
					while (1);
				}
				
				result = f_write(&file, buff, strlen(buff)+1, &bw);
				if ((result != FR_OK) || (bw != strlen(buff)+1))
				{
					while (1);
				}
				
				f_close(&file);
				
//				OSSchedUnlock(&err);
			}
			
			ReleaseKeyMsg(g_keyPtr);
		}
		
		GUI_Delay(10);
	}
}
#endif

#if (TEST_TASK_UCOS_DEBUG_VIEW)

#endif

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

	#if (TEST_SVC == 0x01U)
		Test_SVC();
	#endif
	
	#if (TEST_TASK_EXCHANGE == 0x01U)
		Test_TaskExchange();
	#endif
	
	#if (TEST_TASK_EXCHANGE_FLOAT == 0x01U)
		Test_TaskExchangeFloat();
	#endif

	while (1)
	{
		;
	}
}


/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
