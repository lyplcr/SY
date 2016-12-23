/*
*********************************************************************************************************
* @file    	kernel.h
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-14 16:10:26
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	内核头文件
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
#ifndef __KERNEL_H
#define __KERNEL_H

/*
*********************************************************************************************************
*                              				Exported Includes
*********************************************************************************************************
*/
#include "bsp.h"
#include "key.h"
#include "io.h"
#include "date.h"
#include "GUI.h"
#include "DIALOG.h"
#include "mouse.h"

/*
*********************************************************************************************************
*                              				Exported define
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                              				Exported types
*********************************************************************************************************
*/
typedef bool (*UART_SEND_PACKAGE_T)( uint8_t _ucPort, const uint8_t *_ucaBuf, uint16_t _usLen );
typedef bool (*UART_RECEIVE_PACKAGE_T)( uint8_t _ucPort, const uint8_t *_ucaBuf );


/*
*********************************************************************************************************
*                              				循环任务
*********************************************************************************************************
*/
typedef enum
{
	STATUS_CYCLE_TASK_IDLE = 0,
	STATUS_CYCLE_TASK_ON,
	STATUS_CYCLE_TASK_OFF,
	STATUS_CYCLE_TASK_DONE,
}CYCLE_TASK_STATUS_TypeDef;

typedef struct
{
	/* 由主调函数实现 */
	CYCLE_TASK_STATUS_TypeDef status;					//状态
	uint32_t runPeriod;									//函数运行周期(单位：ms)
	void *devicePtr;									//设备指针
	
	/* 回调函数 */
	void (*taskStartNotify_CallBack)( void *devicePrt );//开始通知
	void (*onDevice_CallBack)( void *devicePrt );		//打开设备
	void (*offDevice_CallBack)( void *devicePrt );		//关闭设备
	void (*taskDoneNotify_CallBack)( void *devicePrt );	//完成通知
	
	/* 由被调函数实现 */
	bool enableFlag;							//使能开关
	bool disableFlag;							//失能开关
	uint32_t executeNums;						//执行次数
	uint32_t onTime;							//打开时间
	uint32_t offTime;							//关闭时间
	uint32_t executeCount;						//执行对象计数器
	uint32_t onCount;							//打开计数器
	uint32_t offCount;							//关闭计数器
}CYCLE_TASK_TypeDef;	

/*
*********************************************************************************************************
*                              				延时停止任务
*********************************************************************************************************
*/
typedef enum
{
	STATUS_DELAY_STOP_TASK_IDLE = 0,
	STATUS_DELAY_STOP_TASK_ON,
	STATUS_DELAY_STOP_TASK_DONE,
}DELAY_STOP_STATUS_TypeDef;

typedef struct
{
	/* 由主调函数实现 */
	DELAY_STOP_STATUS_TypeDef status;						//状态
	uint32_t runPeriod;										//函数运行周期(单位：ms)
	void *devicePrt;										//设备指针
	
	/* 回调函数 */
	void (*taskStartNotify_CallBack)( void *devicePrt );	//开始通知
	void (*onDevice_CallBack)( void *devicePrt );			//打开设备
	void (*offDevice_CallBack)( void *devicePrt );			//关闭设备
	void (*taskDoneNotify_CallBack)( void *devicePrt );		//完成通知
	
	/* 由被调函数实现 */
	bool enableFlag;							//使能开关
	bool disableFlag;							//失能开关
	uint32_t delayTime;							//延时时间
	uint32_t delayCount;						//延时计数器
}DELAY_STOP_TASK_TypeDef;

/*
*********************************************************************************************************
*                              				IO模拟PWM任务
*********************************************************************************************************
*/
typedef enum
{
	PWM_IO_STATUS_IDLE = 0,
	PWM_IO_STATUS_OPEN,
	PWM_IO_STATUS_CLOSE,
	PWM_IO_STATUS_DONE,
}PWM_IO_STATUS_TypeDef;

typedef struct
{
	/* 由主调函数实现 */
	PWM_IO_STATUS_TypeDef status;					//状态
	float runPeriod;								//运行周期
	float outputPeriod;								//输出周期
	uint16_t maxControlDegree;						//最大控制量
	void *devicePrt;								//设备指针
	
	/* 回调函数 */
	void (*taskStartNotify_CallBack)( void *devicePrt );//开始通知
	void (*onDevice_CallBack)( void *devicePrt );		//打开设备
	void (*offDevice_CallBack)( void *devicePrt );		//关闭设备
	void (*taskDoneNotify_CallBack)( void *devicePrt );	//完成通知
	
	/* 由被调函数实现 */
	bool enableFlag;								//使能开关
	bool disableFlag;								//失能开关
	bool continueRunFlag;							//连续运行开关
	uint16_t dutyCycle;								//占空比
	uint32_t PWM_Count;								//PWM计数器
	uint32_t sumCount;								//总执行次数
	uint32_t Reg_CCR;								//比较计数器
}PWM_IO_TypeDef;

/*
*********************************************************************************************************
*                              				开关任务
*********************************************************************************************************
*/
typedef enum
{
	SWITCH_TASK_IDLE = 0,
	SWITCH_TASK_OPEN,
	SWITCH_TASK_OPEN_NOTIFY,
	SWITCH_TASK_CLOSE,
	SWITCH_TASK_CLOSE_NOTIFY,
}SWITCH_TASK_STATUS_TypeDef;

typedef struct
{
	/* 由主调函数实现 */
	SWITCH_TASK_STATUS_TypeDef status;					//状态
	void *devicePrt;									//设备指针
	
	/* 回调函数 */
	bool (*isSwitchOn_CallBack)( void *devicePrt );		//开关是否打开
	void (*onDevice_CallBack)( void *devicePrt );		//打开开关通知
	void (*offDevice_CallBack)( void *devicePrt );		//关闭开关通知
	
	/* 由被调函数实现 */	
}SWITCH_TASK_TypeDef;

/*
*********************************************************************************************************
*                              				看门狗任务
*********************************************************************************************************
*/
typedef enum
{
	WATCH_DOG_TASK_IDLE = 0,
	WATCH_DOG_TASK_WORKING,
	WATCH_DOG_TASK_DONE,
}WATCH_DOG_TASK_STATUS_TypeDef;

typedef struct
{
	/* 由主调函数实现 */
	WATCH_DOG_TASK_STATUS_TypeDef status;			//状态
	uint32_t runPeriod;								//运行周期
	void *devicePrt;								//设备指针
	
	/* 回调函数 */
	void (*timeUpNotify_CallBack)( void *devicePrt );//时间到达通知
	
	/* 由被调函数实现 */
	bool enableFlag;								//使能开关
	bool disableFlag;								//失能开关
	uint32_t decreaseCount;							//递减计数器
	uint32_t countTimeMS;							//计数时间
}WATCH_DOG_TASK_TypeDef;

typedef enum {
	HID_USR_IDLE = 0,
	HID_USR_WAIT,  
	HID_USR_START,
	HID_USR_MOUSE,  
	HID_USR_KEYBOARD,    
	HID_USR_REENUMERATE,
}HID_Usr_State;

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
extern HID_Usr_State HID_HostState;

/*
*********************************************************************************************************
*                              				Exported functions
*********************************************************************************************************
*/
void KERNEL_Init( void );

/*
*********************************************************************************************************
*                              				循环任务
*********************************************************************************************************
*/
void RepeatExecuteTaskInit( CYCLE_TASK_TypeDef *cycleTaskPtr, uint32_t runPeriod,
							void *devicePtr );
void RepeatExecuteTaskDeInit( CYCLE_TASK_TypeDef *cycleTaskPtr );
void SetRepeatExecuteTime( CYCLE_TASK_TypeDef *cycleTaskPtr,\
		uint32_t executeNums, uint32_t onTimeMS, uint32_t offTimeMS );
void CloseRepeatExecuteTask( CYCLE_TASK_TypeDef *cycleTaskPtr );	
void RepeatExecuteTaskCycle( CYCLE_TASK_TypeDef *cycleTaskPtr );
void RegisterRepeatExecuteTaskStartNotify_CallBack( CYCLE_TASK_TypeDef *cycleTaskPtr,\
		void (*taskStartNotify_CallBack)( void *devicePrt ) );
void RegisterRepeatExecuteTaskOnDevice_CallBack( CYCLE_TASK_TypeDef *cycleTaskPtr,\
		void (*onDevice_CallBack)( void *devicePrt ) );
void RegisterRepeatExecuteTaskOffDevice_CallBack( CYCLE_TASK_TypeDef *cycleTaskPtr,\
		void (*offDevice_CallBack)( void *devicePrt ) );
void RegisterRepeatExecuteTaskDoneNotify_CallBack( CYCLE_TASK_TypeDef *cycleTaskPtr,\
		void (*taskDoneNotify_CallBack)( void *devicePrt ) );

/*
*********************************************************************************************************
*                              				延时停止任务
*********************************************************************************************************
*/
void DelayStopTaskInit( DELAY_STOP_TASK_TypeDef *delayStopPtr, uint32_t runPeriod,
						void *devicePrt );
void DelayStopTaskDeInit( DELAY_STOP_TASK_TypeDef *delayStopPtr );
void SetDelayStopTime( DELAY_STOP_TASK_TypeDef *delayStopPtr,\
		uint32_t delayTimeMS );
void CloseDelayStopTask( DELAY_STOP_TASK_TypeDef *delayStopPtr );
bool IsDelayStopTaskRuning( DELAY_STOP_TASK_TypeDef *delayStopPtr );
void DelayStopTaskCycle( DELAY_STOP_TASK_TypeDef *delayStopPtr );
void RegisterDelayStopTaskStartNotify_CallBack( DELAY_STOP_TASK_TypeDef *delayStopPtr,\
		void (*taskStartNotify_CallBack)( void *devicePrt ) );
void RegisterDelayStopTaskOnDevice_CallBack( DELAY_STOP_TASK_TypeDef *delayStopPtr,\
		void (*onDevice_CallBack)( void *devicePrt ) );
void RegisterDelayStopTaskOffDevice_CallBack( DELAY_STOP_TASK_TypeDef *delayStopPtr,\
		void (*offDevice_CallBack)( void *devicePrt ) );
void RegisterDelayStopTaskDoneNotify_CallBack( DELAY_STOP_TASK_TypeDef *delayStopPtr,\
		void (*taskDoneNotify_CallBack)( void *devicePrt ) );

/*
*********************************************************************************************************
*                              				IO模拟PWM任务
*********************************************************************************************************
*/
void PWM_IO_OutputInit( PWM_IO_TypeDef *PWM, float runPeriod, float outputPeriod, 
						uint16_t maxControlDegree, void *devicePrt );
void PWM_IO_OutputDeInit( PWM_IO_TypeDef *PWM );
void PWM_IO_SetOutput( PWM_IO_TypeDef *PWM, uint16_t dutyCycle );
void PWM_IO_CloseOutput( PWM_IO_TypeDef *PWM );
void PWM_IO_OutputCycle( PWM_IO_TypeDef *PWM_ObjPtr );
void RegisterPWM_IO_TaskStartNotify_CallBack( PWM_IO_TypeDef *PWM_IO_Ptr,\
		void (*taskStartNotify_CallBack)( void *devicePrt ) );
void RegisterPWM_IO_TaskOnDevice_CallBack( PWM_IO_TypeDef *PWM_IO_Ptr,\
		void (*onDevice_CallBack)( void *devicePrt ) );
void RegisterPWM_IO_TaskOffDevice_CallBack( PWM_IO_TypeDef *PWM_IO_Ptr,\
		void (*offDevice_CallBack)( void *devicePrt ) );
void RegisterPWM_IO_TaskDoneNotify_CallBack( PWM_IO_TypeDef *PWM_IO_Ptr,\
		void (*taskDoneNotify_CallBack)( void *devicePrt ) );

/*
*********************************************************************************************************
*                              				开关任务
*********************************************************************************************************
*/
void SwitchTaskInit( SWITCH_TASK_TypeDef *switchPtr, void *devicePrt );
void SwitchTaskDeInit( SWITCH_TASK_TypeDef *switchPtr );
void SwitchTask( SWITCH_TASK_TypeDef *switchPtr );
void RegisterSwitchTaskIsSwitchOn_CallBack( SWITCH_TASK_TypeDef *switchPtr,\
		bool (*isSwitchOn_CallBack)( void *devicePrt ) );
void RegisterSwitchTaskOnDevice_CallBack( SWITCH_TASK_TypeDef *switchPtr,\
		void (*onDevice_CallBack)( void *devicePrt ) );
void RegisterSwitchTaskOffDevice_CallBack( SWITCH_TASK_TypeDef *switchPtr,\
		void (*offDevice_CallBack)( void *devicePrt ) );
		
/*
*********************************************************************************************************
*                              				看门狗任务
*********************************************************************************************************
*/
void WatchDogTaskInit( WATCH_DOG_TASK_TypeDef *wwdgPtr, uint32_t runPeriod, 
						void *devicePrt);
void WatchDogTaskDeInit( WATCH_DOG_TASK_TypeDef *wwdgPtr );
void SetWatchDogCounter( WATCH_DOG_TASK_TypeDef *wwdgPtr,\
		uint32_t countTimeMS );
void CloseWatchDogTask( WATCH_DOG_TASK_TypeDef *wwdgPtr );
void RegisterWatchDogTaskTimeUpNotify_CallBack( WATCH_DOG_TASK_TypeDef *wwdgPtr,\
		void (*timeUpNotify_CallBack)( void *devicePrt ) );
void WatchDogTaskCycle( WATCH_DOG_TASK_TypeDef *wwdgPtr );

void HID_HostDetectTask( HID_Usr_State *hidHostPtr );


/*
*********************************************************************************************************
*                              				Exported Includes
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                              				Exported functions
*********************************************************************************************************
*/



#endif
/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
