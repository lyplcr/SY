/*
*********************************************************************************************************
* @file    	key.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-14 16:10:26
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	按键源文件
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
#include "kernel.h"
#include "SeqQueue.h"


/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/
#define KEY_COUNT							28			//按键个数
#define KEY_QUEUE_DEEP						10			//键值队列深度

/*
*********************************************************************************************************
*                              				Private typedef
*********************************************************************************************************
*/
typedef enum
{
	STATUS_DETECT_IDLE = 0,					/* 空闲 */
	STATUS_DETECT_PRESS_DOWN_FILTER,		/* 按下滤波 */
	STATUS_DETECT_PRESS_DOWN,				/* 按下 */
	STATUS_DETECT_CONTINUE_PRESS_DOWN,		/* 长按 */
	STATUS_DETECT_HIGH_SPEED_MODE,			/* 高速模式 */
	STATUS_DETECT_PRESS_UP_FILTER,			/* 释放滤波 */
	STATUS_DETECT_PRESS_UP,					/* 释放 */
}KEY_DETECT_STATUS_TypeDef;

typedef struct
{
	uint32_t keyValue;						/* 键值 */
	KEY_STATUS_TypeDef status;				/* 状态 */
}KEY_MSG_TypeDef;

/* 单个按键的结构体 */
typedef struct
{
	uint32_t keyValue;						/* 键值 */
	KEY_STATUS_TypeDef status;				/* 状态 */
	KEY_DETECT_STATUS_TypeDef detectStatus;	/* 检测状态 */
	bool (*isPress_CallBack)( void );		/* 是否按下 */
	
	uint32_t pressTime;						/* 按键按下时间 */
	uint32_t filterTime;					/* 滤波时间 */
	
	uint32_t longPressTime;					/* 长按时间 （配置为0表示关闭长按）*/
	uint32_t repeatCount;					/* 连按计数器 */
	uint32_t repeatNums;					/* 连按次数 */
	
	uint32_t highSpeedTime;					/* 高速模式时间（配置为0表示关闭高速模式） */
}__KEY_T;

struct tagKEY_TypeDef
{
	uint32_t keyNums;						/* 键值个数 */
	__KEY_T key[KEY_COUNT];					/* 按键 */
	SEQUEUE_TypeDef msgQueue;				/* 键值消息队列 */
	
	void (*notifyKeyPress_CallBack)( struct tagKEY_TypeDef *keyPtr, uint32_t keyValue );		/* 按键按下通知 */
	void (*notifyKeyLongPress_CallBack)( struct tagKEY_TypeDef *keyPtr, uint32_t keyValue );	/* 按键长按通知 */
	void (*notifyKeyHighSpeed_CallBack)( struct tagKEY_TypeDef *keyPtr, uint32_t keyValue );	/* 按键高速模式通知 */
	void (*notifyKeyLeave_CallBack)( struct tagKEY_TypeDef *keyPtr, uint32_t keyValue );		/* 按键离开通知 */
};

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
__STATIC_INLINE bool IsPressKey_0( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_0) );
}
__STATIC_INLINE bool IsPressKey_1( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_1) );
}
__STATIC_INLINE bool IsPressKey_2( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_2) );
}
__STATIC_INLINE bool IsPressKey_3( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_3) );
}
__STATIC_INLINE bool IsPressKey_4( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_4) );
}
__STATIC_INLINE bool IsPressKey_5( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_5) );
}
__STATIC_INLINE bool IsPressKey_6( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_6) );
}
__STATIC_INLINE bool IsPressKey_7( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_7) );
}
__STATIC_INLINE bool IsPressKey_8( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_8) );
}
__STATIC_INLINE bool IsPressKey_9( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_9) );
}
__STATIC_INLINE bool IsPressKey_UP( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_UP) );
}
__STATIC_INLINE bool IsPressKey_DOWN( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_DOWN) );
}
__STATIC_INLINE bool IsPressKey_LEFT( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_LEFT) );
}
__STATIC_INLINE bool IsPressKey_RIGHT( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_RIGHT) );
}
__STATIC_INLINE bool IsPressKey_ENTER( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_ENTER) );
}
__STATIC_INLINE bool IsPressKey_ESC( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_ESC) );
}
__STATIC_INLINE bool IsPressKey_BACK( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_BACK) );
}
__STATIC_INLINE bool IsPressKey_ADD( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_ADD) );
}
__STATIC_INLINE bool IsPressKey_POINT( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_POINT) );
}
__STATIC_INLINE bool IsPressKey_RUN( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_RUN) );
}
__STATIC_INLINE bool IsPressKey_STOP( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_STOP) );
}
__STATIC_INLINE bool IsPressKey_TARE( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_TARE) );
}
__STATIC_INLINE bool IsPressKey_SHIFT( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_SHIFT) );
}
__STATIC_INLINE bool IsPressKey_PRINT( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_PRINT) );
}
__STATIC_INLINE bool IsPressKey_F1( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_F1) );
}
__STATIC_INLINE bool IsPressKey_F2( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_F2) );
}
__STATIC_INLINE bool IsPressKey_F3( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_F3) );
}
__STATIC_INLINE bool IsPressKey_F4( void )
{
	return ( READ_BIT(BSP_KEY_VALUE(), BIT_KEY_F4) );
}


/*
*********************************************************************************************************
*                              				Private variables
*********************************************************************************************************
*/
static KEY_MSG_TypeDef g_keyQueue[KEY_QUEUE_DEEP];
static KEY_TypeDef g_key;
KEY_TypeDef * const g_keyPtr = &g_key;

/*
*********************************************************************************************************
*                              				Private function prototypes
*********************************************************************************************************
*/
static void __Key_Init( KEY_TypeDef *keyPtr );


/*
*********************************************************************************************************
*                              				Private functions
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : Key_Init
* Description	: 按键初始化
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void Key_Init( void )
{
	__Key_Init(g_keyPtr);
	
	ECHO(DEBUG_KERNEL_INIT, "按键初始化 .......... OK");
}

/*
*********************************************************************************************************
* Function Name : __BindKey
* Description	: 绑定按键
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static bool __BindKey( KEY_TypeDef *keyPtr,  uint32_t keyIndex, uint32_t keyValue,\
				bool (*isPress_CallBack)( void ) )
{
	if(keyIndex >= keyPtr->keyNums)
	{
		return false;
	}
	
	keyPtr->key[keyIndex].keyValue = keyValue;
	keyPtr->key[keyIndex].isPress_CallBack = isPress_CallBack;
	
	return true;
}

/*
*********************************************************************************************************
* Function Name : __FindKeyIndex
* Description	: 查找键值索引
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE bool __FindKeyIndex( KEY_TypeDef *keyPtr, uint32_t keyValue, uint32_t *keyIndex )
{
	for (uint32_t i=0; i<keyPtr->keyNums; ++i)
	{
		if (keyValue == keyPtr->key[i].keyValue)
		{
			*keyIndex = i;
			
			return true;
		}
	}
	
	return false;
}

/*
*********************************************************************************************************
* Function Name : __ConfigKeyTimes
* Description	: 配置键值时间
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool ConfigKeyTimes( KEY_TypeDef *keyPtr, uint32_t keyValue, \
		uint32_t longPressTime, uint32_t repeatNums, uint32_t highSpeedTime )
{
	uint32_t keyIndex = 0;	
	if (__FindKeyIndex(keyPtr, keyValue, &keyIndex) == false)
	{
		return false;
	}
	
	keyPtr->key[keyIndex].longPressTime = longPressTime;
	keyPtr->key[keyIndex].repeatNums = repeatNums;
	keyPtr->key[keyIndex].highSpeedTime = highSpeedTime;
	
	return true;
}

/*
*********************************************************************************************************
* Function Name : ConfigKeyFilterTimes
* Description	: 配置按键滤波时间
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool ConfigKeyFilterTimes( KEY_TypeDef *keyPtr, uint32_t keyValue, uint32_t filterTime )
{
	uint32_t keyIndex = 0;	
	if (__FindKeyIndex(keyPtr, keyValue, &keyIndex) == false)
	{
		return false;
	}
	
	keyPtr->key[keyIndex].filterTime = filterTime;
	
	return true;
}

/*
*********************************************************************************************************
* Function Name : __NotifyKeyPress_CallBack
* Description	: 按键按下通知回调函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void __NotifyKeyPress_CallBack( KEY_TypeDef *keyPtr, uint32_t keyValue )
{
	(void)keyPtr;
	
	SetRepeatExecuteTime(GetBeepHandle(),\
		KEY_BEEP_KEY_CYCLE_NUMS,KEY_BEEP_KEY_ON_TIME_MS,KEY_BEEP_KEY_OFF_TIME_MS);
}

/*
*********************************************************************************************************
* Function Name : __Key_Init
* Description	: 按键初始化
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void __Key_Init( KEY_TypeDef *keyPtr )
{
	memset(keyPtr, NULL, sizeof(KEY_TypeDef));
	
	keyPtr->keyNums = KEY_COUNT;
	CreateSeqQueue(&keyPtr->msgQueue,g_keyQueue,KEY_QUEUE_DEEP);
	
	__BindKey(keyPtr, 0, 	KEY_VALUE_0, 		IsPressKey_0);
	__BindKey(keyPtr, 1, 	KEY_VALUE_1, 		IsPressKey_1);
	__BindKey(keyPtr, 2, 	KEY_VALUE_2, 		IsPressKey_2);
	__BindKey(keyPtr, 3, 	KEY_VALUE_3, 		IsPressKey_3);
	__BindKey(keyPtr, 4, 	KEY_VALUE_4, 		IsPressKey_4);
	__BindKey(keyPtr, 5, 	KEY_VALUE_5, 		IsPressKey_5);
	__BindKey(keyPtr, 6, 	KEY_VALUE_6, 		IsPressKey_6);
	__BindKey(keyPtr, 7, 	KEY_VALUE_7, 		IsPressKey_7);
	__BindKey(keyPtr, 8, 	KEY_VALUE_8, 		IsPressKey_8);
	__BindKey(keyPtr, 9, 	KEY_VALUE_9, 		IsPressKey_9);
	__BindKey(keyPtr, 10, 	KEY_VALUE_UP, 		IsPressKey_UP);
	__BindKey(keyPtr, 11, 	KEY_VALUE_DOWN, 	IsPressKey_DOWN);
	__BindKey(keyPtr, 12, 	KEY_VALUE_LEFT, 	IsPressKey_LEFT);
	__BindKey(keyPtr, 13, 	KEY_VALUE_RIGHT, 	IsPressKey_RIGHT);
	__BindKey(keyPtr, 14, 	KEY_VALUE_ENTER, 	IsPressKey_ENTER);
	__BindKey(keyPtr, 15, 	KEY_VALUE_ESC, 		IsPressKey_ESC);
	__BindKey(keyPtr, 16, 	KEY_VALUE_BACK, 	IsPressKey_BACK);
	__BindKey(keyPtr, 17, 	KEY_VALUE_ADD, 		IsPressKey_ADD);
	__BindKey(keyPtr, 18, 	KEY_VALUE_POINT, 	IsPressKey_POINT);
	__BindKey(keyPtr, 19, 	KEY_VALUE_RUN, 		IsPressKey_RUN);
	__BindKey(keyPtr, 20, 	KEY_VALUE_STOP, 	IsPressKey_STOP);
	__BindKey(keyPtr, 21, 	KEY_VALUE_TARE, 	IsPressKey_TARE);
	__BindKey(keyPtr, 22, 	KEY_VALUE_SHIFT, 	IsPressKey_SHIFT);
	__BindKey(keyPtr, 23, 	KEY_VALUE_PRINT, 	IsPressKey_PRINT);
	__BindKey(keyPtr, 24, 	KEY_VALUE_F1, 		IsPressKey_F1);
	__BindKey(keyPtr, 25, 	KEY_VALUE_F2, 		IsPressKey_F2);
	__BindKey(keyPtr, 26, 	KEY_VALUE_F3,	 	IsPressKey_F3);
	__BindKey(keyPtr, 27, 	KEY_VALUE_F4, 		IsPressKey_F4);
	
	for (uint32_t key=KEY_VALUE_START; key<=KEY_VALUE_END; ++key)
	{
		ConfigKeyTimes(keyPtr, key, \
			DEFAULT_KEY_LONG_PRESS_TIME_MS, DEFAULT_KEY_REPEAT_NUMS, DEFAULT_KEY_HIGH_SPEED_TIME_MS);
		ConfigKeyFilterTimes(keyPtr, key, 20);
	}
	
	#if (ENABLE_KEY_BEEP == 0x01U)	
		keyPtr->notifyKeyPress_CallBack 	= __NotifyKeyPress_CallBack;
		keyPtr->notifyKeyLongPress_CallBack = __NotifyKeyPress_CallBack;
		keyPtr->notifyKeyHighSpeed_CallBack = __NotifyKeyPress_CallBack;
		keyPtr->notifyKeyLeave_CallBack 	= NULL;
	#else
		keyPtr->notifyKeyPress_CallBack 	= NULL;
		keyPtr->notifyKeyLongPress_CallBack = NULL;
		keyPtr->notifyKeyHighSpeed_CallBack = NULL;
		keyPtr->notifyKeyLeave_CallBack 	= NULL;
	#endif
}

/*
*********************************************************************************************************
* Function Name : PushSeqQueueKey_CallBack
* Description	: 键值入队回调函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE void PushSeqQueueKey_CallBack( void *base, uint32_t offset, void *dataIn )
{
	KEY_MSG_TypeDef *dataPtr = dataIn;
	KEY_MSG_TypeDef *basePtr = base;
	
	memcpy(basePtr+offset, dataPtr, sizeof(KEY_MSG_TypeDef));
}

/*
*********************************************************************************************************
* Function Name : PopSeqQueueKey_CallBack
* Description	: 键值出队回调函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE void PopSeqQueueKey_CallBack( void *base, uint32_t offset, void *dataOut )
{
	KEY_MSG_TypeDef *dataPtr = dataOut;
	KEY_MSG_TypeDef *basePtr = base;
	
	memcpy(dataPtr, basePtr+offset, sizeof(KEY_MSG_TypeDef));
}

/*
*********************************************************************************************************
* Function Name : KeyDetectTask
* Description	: 按键检测任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void KeyDetectTask( KEY_TypeDef *keyPtr, uint32_t keyValue )
{
	uint32_t keyIndex = 0;	
	if (__FindKeyIndex(keyPtr, keyValue, &keyIndex) == false)
	{
		return;
	}
	
	__KEY_T *this = &keyPtr->key[keyIndex];
	
	switch ( this->detectStatus )
	{
		case STATUS_DETECT_IDLE:
			this->pressTime = 0;
			this->status = STATUS_KEY_NONE;
			if ((this->isPress_CallBack) && (this->isPress_CallBack() == true))
			{
				this->pressTime++;
				this->detectStatus = STATUS_DETECT_PRESS_DOWN_FILTER;
			}
			break;
		case STATUS_DETECT_PRESS_DOWN_FILTER:		
			this->pressTime++;
			if (this->pressTime >= this->filterTime)
			{
				if (this->isPress_CallBack() == false)
				{
					this->detectStatus = STATUS_DETECT_IDLE;
					
					return;
				}
				
				/* 按键按下 */
				this->status = STATUS_KEY_PRESS;
				{
					KEY_MSG_TypeDef msg;
					msg.keyValue = this->keyValue;
					msg.status = this->status;
					PushSeqQueue(&keyPtr->msgQueue, &msg,PushSeqQueueKey_CallBack);				
				}
				if (keyPtr->notifyKeyPress_CallBack)
				{
					keyPtr->notifyKeyPress_CallBack(keyPtr, this->keyValue);
				}
				this->detectStatus = STATUS_DETECT_PRESS_DOWN;
			}		
			break;
		case STATUS_DETECT_PRESS_DOWN:
			if (this->isPress_CallBack() == true)
			{
				this->pressTime++;
				if ((this->longPressTime != 0) && (this->pressTime >= this->longPressTime))
				{
					this->status = STATUS_KEY_LONG;
					{
						KEY_MSG_TypeDef msg;
						msg.keyValue = this->keyValue;
						msg.status = this->status;
						PushSeqQueue(&keyPtr->msgQueue, &msg,PushSeqQueueKey_CallBack);				
					}				
					this->repeatCount = 1;
					this->pressTime = 0;
					if (keyPtr->notifyKeyLongPress_CallBack)
					{
						keyPtr->notifyKeyLongPress_CallBack(keyPtr, this->keyValue);
					}
					this->detectStatus = STATUS_DETECT_CONTINUE_PRESS_DOWN;
				}				
			}
			else
			{
				this->pressTime = 0;
				this->detectStatus = STATUS_DETECT_PRESS_UP_FILTER;
			}
			break;
		case STATUS_DETECT_CONTINUE_PRESS_DOWN:
			if (this->isPress_CallBack() == true)
			{
				this->pressTime++;
				if ((this->longPressTime != 0) && (this->pressTime >= this->longPressTime))
				{		
					this->pressTime = 0;
					this->repeatCount++;
					this->status = STATUS_KEY_LONG;
					{
						KEY_MSG_TypeDef msg;
						msg.keyValue = this->keyValue;
						msg.status = this->status;
						PushSeqQueue(&keyPtr->msgQueue, &msg,PushSeqQueueKey_CallBack);				
					}	
					if (keyPtr->notifyKeyLongPress_CallBack)
					{
						keyPtr->notifyKeyLongPress_CallBack(keyPtr, this->keyValue);
					}
					
					if ((this->repeatNums != 0) && (this->repeatCount >= this->repeatNums))
					{							
						if (this->highSpeedTime != 0)
						{
							this->detectStatus = STATUS_DETECT_HIGH_SPEED_MODE;
						}
					}								
				}				
			}
			else
			{
				this->pressTime = 0;
				this->detectStatus = STATUS_DETECT_PRESS_UP_FILTER;
			}
			break;
		case STATUS_DETECT_HIGH_SPEED_MODE:
			if (this->isPress_CallBack() == true)
			{
				this->pressTime++;
				if ((this->highSpeedTime != 0) && (this->pressTime >= this->highSpeedTime))
				{
					this->pressTime = 0;
					this->status = STATUS_KEY_HIGH_SPEED;
					{
						KEY_MSG_TypeDef msg;
						msg.keyValue = this->keyValue;
						msg.status = this->status;
						PushSeqQueue(&keyPtr->msgQueue, &msg,PushSeqQueueKey_CallBack);				
					}

					if (keyPtr->notifyKeyHighSpeed_CallBack)
					{
						keyPtr->notifyKeyHighSpeed_CallBack(keyPtr, this->keyValue);
					}
				}					
			}
			else
			{
				this->pressTime = 0;
				this->detectStatus = STATUS_DETECT_PRESS_UP_FILTER;
			}
			break;
		case STATUS_DETECT_PRESS_UP_FILTER:
			this->pressTime++;
			if (this->pressTime >= this->filterTime)
			{
				this->status = STATUS_KEY_LEAVE;
				{
					KEY_MSG_TypeDef msg;
					msg.keyValue = this->keyValue;
					msg.status = this->status;
					PushSeqQueue(&keyPtr->msgQueue, &msg,PushSeqQueueKey_CallBack);				
				}
				this->detectStatus = STATUS_DETECT_PRESS_UP;
			}
			break;
		case STATUS_DETECT_PRESS_UP:
			this->detectStatus = STATUS_DETECT_IDLE;
			break;
		default:
			this->detectStatus = STATUS_DETECT_IDLE;
			break;
	}
}

/*
*********************************************************************************************************
* Function Name : KeyScanCycleTask
* Description	: 按键扫描循环任务（需要周期性执行）
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void KeyScanCycleTask( KEY_TypeDef *keyPtr )
{
	for (uint32_t key=KEY_VALUE_START; key<=KEY_VALUE_END; ++key)
	{
		KeyDetectTask(keyPtr,key);
	}
}

/*
*********************************************************************************************************
* Function Name : ClearKeyQueue
* Description	: 清空键值队列
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void ClearKeyQueue( KEY_TypeDef *keyPtr )
{
	ClearSeqQueue(&keyPtr->msgQueue);
}

/*
*********************************************************************************************************
* Function Name : ReadKeyValue
* Description	: 读取键值
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t ReadKeyValue( KEY_TypeDef *keyPtr )
{
	KEY_MSG_TypeDef msg = {
	.keyValue = KEY_VALUE_NONE,
	.status = STATUS_KEY_NONE,
	};
	
	GetSeqQueueElement(&keyPtr->msgQueue, &msg, PopSeqQueueKey_CallBack);
	
	return msg.keyValue;
}

/*
*********************************************************************************************************
* Function Name : ReadKeyStatus
* Description	: 读取按键状态
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
KEY_STATUS_TypeDef ReadKeyStatus( KEY_TypeDef *keyPtr )
{
	KEY_MSG_TypeDef msg = {
	.keyValue = KEY_VALUE_NONE,
	.status = STATUS_KEY_NONE,
	};
	
	GetSeqQueueElement(&keyPtr->msgQueue, &msg, PopSeqQueueKey_CallBack);
	
	return msg.status;
}

/*
*********************************************************************************************************
* Function Name : ReleaseKeyMsg
* Description	: 释放按键消息
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool ReleaseKeyMsg( KEY_TypeDef *keyPtr )
{
	KEY_MSG_TypeDef msg = {
	.keyValue = KEY_VALUE_NONE,
	.status = STATUS_KEY_NONE,
	};
	
	if (PopSeqQueue(&keyPtr->msgQueue, &msg, PopSeqQueueKey_CallBack) == STATUS_DATA_STRUCT_TRUE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
