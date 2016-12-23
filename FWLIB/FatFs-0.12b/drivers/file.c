/*
*********************************************************************************************************
* @file    	file.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-11-1 08:43:55
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	文件相关操作源文件
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
#include "file.h"


/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/


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


/*
*********************************************************************************************************
*                              				Private function prototypes
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private functions
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : CreatFolder
* Description	: 创建文件夹
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
FRESULT CreatFolder( const char * const pPath )
{
	uint32_t i = 0;
	uint32_t pathIndex = 0;
	char pathBuff[100];
	FRESULT fresult;
	
	while (pPath[i] != NULL)
	{	
		if (pPath[i] == '/')
		{
			pathBuff[pathIndex] = NULL;
			
			fresult = f_mkdir(pathBuff); 
			if ((fresult!=FR_OK) && (fresult!=FR_EXIST))
			{									
				break;
			}
			
			pathBuff[pathIndex++] = '/';
		}
		else
		{
			pathBuff[pathIndex++] = pPath[i];
		}
		
		i++;
	}
	
	return fresult;
}

/*
*********************************************************************************************************
* Function Name : CopyFileTime
* Description	: 复制文件时间
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static FRESULT CopyFileTime( const char * const pSourcePath, const char * const pTargetPath )
{
	FILINFO file_info;
	FRESULT fresult;
	
	fresult = f_stat(pSourcePath,&file_info);
	if (fresult != FR_OK)
	{									
		return fresult;
	}
	
	fresult = f_utime(pTargetPath,&file_info);
	
	return fresult;
}

/*
*********************************************************************************************************
* Function Name : CopyFileFromDevices
* Description	: 从设备拷贝文件
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
ErrorStatus CopyFileFromDevices( const char * const pSourcePath, const char * const pTargetPath )
{
	FRESULT fresult;
	FIL sourceFileObj; 
	FIL targetFileObj; 
	uint32_t br; 
	const uint16_t ONCE_READ_SIZE = 512;
	uint8_t fileBuff[ONCE_READ_SIZE];
	uint32_t totalReadSize = 0;
	uint32_t totalWriteSize = 0;
	
	fresult = f_open(&sourceFileObj,pSourcePath,FA_OPEN_EXISTING|FA_READ);
	if (fresult != FR_OK)
	{
		f_close(&sourceFileObj);
		
		return ERROR;
	}
	
	fresult = f_open(&targetFileObj,pTargetPath,FA_CREATE_ALWAYS|FA_WRITE);
	if (fresult == FR_NO_PATH)
	{
		fresult = CreatFolder(pTargetPath);		
		fresult = f_open(&targetFileObj,pTargetPath,FA_CREATE_ALWAYS|FA_WRITE);
		if (fresult != FR_OK)
		{
			f_close(&sourceFileObj);
			f_close(&targetFileObj);
		
			return ERROR;
		}	
	}
	else if (fresult != FR_OK)
	{
		f_close(&sourceFileObj);
		f_close(&targetFileObj);
	
		return ERROR;
	}

	totalReadSize = f_size(&sourceFileObj);
	
	do{
		uint32_t realReadSize = 0;
		
		fresult = f_read(&sourceFileObj,fileBuff,ONCE_READ_SIZE,&br); 
		if (fresult != FR_OK)
		{
			break;
		}
		realReadSize = br;
		
		fresult = f_write(&targetFileObj,fileBuff,realReadSize,&br);
		if (fresult != FR_OK)
		{
			break;
		}		
		totalWriteSize += br;	
		
	} while (br != 0);
	
	f_close(&sourceFileObj);
	f_close(&targetFileObj);
	
	if (fresult != FR_OK)
	{
		return ERROR;
	}
	
	if (totalReadSize != totalWriteSize)
	{
		return ERROR;
	}
	
	fresult = CopyFileTime(pSourcePath,pTargetPath);
	if (fresult != FR_OK)
	{
		return ERROR;
	}
	
	return SUCCESS;
}

/*
*********************************************************************************************************
* Function Name : CopyFileBody
* Description	: 从设备拷贝文件体
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static ErrorStatus CopyFileBody( const char * const pSourceFolderPath, const char * const pTargetFolderPath,\
				const char * const pSourceFileName )
{
	const uint8_t FILE_PATH_MAX_LEN = 100;
	char sourcePathName[FILE_PATH_MAX_LEN];
	char targetPathName[FILE_PATH_MAX_LEN];
	uint16_t pathLen = 0;
	
	pathLen = strlen(pSourceFolderPath) + strlen(pSourceFileName) + 1;	//+1：中间有一个'/'符号
	if (pathLen >= FILE_PATH_MAX_LEN)
	{
		return ERROR; 
	}
	
	pathLen = strlen(pTargetFolderPath) + strlen(pSourceFileName) + 1;	//+1：中间有一个'/'符号
	if (pathLen >= FILE_PATH_MAX_LEN)
	{
		return ERROR; 
	}
	
	strcpy(sourcePathName,pSourceFolderPath);
	strcat(sourcePathName,"/");
	strcat(sourcePathName,pSourceFileName);
	
	strcpy(targetPathName,pTargetFolderPath);
	strcat(targetPathName,"/");
	strcat(targetPathName,pSourceFileName);

	if (ERROR == CopyFileFromDevices(sourcePathName,targetPathName) )
	{
		return ERROR;
	}
	
	return SUCCESS;
}

/*
*********************************************************************************************************
* Function Name : CopyFolderBody
* Description	: 从设备拷贝文件夹体
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static ErrorStatus CopyFolderBody( const char * const pSourceFolderPath, const char * const pTargetFolderPath,\
				const char * const pSourceFolderName, void (*GUI_CallBack)( const char *FolderNamePtr, const char *FileNamePtr ) )
{
	const uint8_t FILE_PATH_MAX_LEN = 100;
	char sourcePathName[FILE_PATH_MAX_LEN];
	char targetPathName[FILE_PATH_MAX_LEN];
	uint16_t pathLen = 0;
	
	pathLen = strlen(pSourceFolderPath) + strlen(pSourceFolderName) + 1;	//+1：中间有一个'/'符号
	if (pathLen >= FILE_PATH_MAX_LEN)
	{
		return ERROR; 
	}
	
	pathLen = strlen(pTargetFolderPath) + strlen(pSourceFolderName) + 1;	//+1：中间有一个'/'符号
	if (pathLen >= FILE_PATH_MAX_LEN)
	{
		return ERROR; 
	}
	
	strcpy(sourcePathName,pSourceFolderPath);
	strcat(sourcePathName,"/");
	strcat(sourcePathName,pSourceFolderName);
	
	strcpy(targetPathName,pTargetFolderPath);
	strcat(targetPathName,"/");
	strcat(targetPathName,pSourceFolderName);

	if (ERROR == CopyFolderFromDevices(sourcePathName,targetPathName,GUI_CallBack) )
	{
		return ERROR;
	}
	
	return SUCCESS;
}

/*
*********************************************************************************************************
* Function Name : CopyFolderFromDevices
* Description	: 从设备拷贝文件夹（路径不能以'/'结尾）
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
ErrorStatus CopyFolderFromDevices( const char * const pSourceFolderPath, const char * const pTargetFolderPath, \
				void (*GUI_CallBack)( const char *FolderNamePtr, const char *FileNamePtr ) )
{
	DIR dir_object;  
	FILINFO sourceFileInfo; 
	FRESULT fresult;
	
	fresult = f_opendir(&dir_object,pSourceFolderPath); 	
	if (fresult != FR_OK)
	{
		return ERROR; 
	}
	
	for (;;)
	{
		fresult = f_readdir(&dir_object,&sourceFileInfo);		
		if (fresult != FR_OK)
		{
			return ERROR; 
		}
		
		/* 文件夹内文件读取完毕 */
		if ( !sourceFileInfo.fname[0] )	
		{			
			break;	
		}
		
		{
			const char *pName = sourceFileInfo.fname;			
			
			if (sourceFileInfo.fattrib & AM_ARC)		//是个文件
			{		
				/* 回调函数，执行用户程序 */
				if (GUI_CallBack != NULL)
				{
					GUI_CallBack(NULL,pName);	
				}
				
				if ( CopyFileBody(pSourceFolderPath,pTargetFolderPath,pName) == ERROR)
				{
					return ERROR;
				}
			}
			else if (sourceFileInfo.fattrib & AM_DIR)		//是个文件夹
			{		
				/* 回调函数，执行用户程序 */
				if (GUI_CallBack != NULL)
				{
					GUI_CallBack(pName,NULL);
				}
				
				if ( CopyFolderBody(pSourceFolderPath,pTargetFolderPath,pName,GUI_CallBack) == ERROR)
				{
					return ERROR;
				}
			}
		}
	}
	
	return SUCCESS;
}

/*
*********************************************************************************************************
* Function Name : DeleteFileFromDevices
* Description	: 从设备删除文件
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
ErrorStatus DeleteFileFromDevices( const char * const pTargetPath )
{
	FRESULT fresult = f_unlink(pTargetPath); 	
	
	if (fresult == FR_OK)
	{
		return SUCCESS;
	}
	else
	{
		return ERROR;
	}
}

/*
*********************************************************************************************************
* Function Name : DeleteFileBody
* Description	: 从设备删除文件体
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static ErrorStatus DeleteFileBody( const char * const pTargetFolderPath,\
						const char * const pTargetFileName )
{
	const uint8_t FILE_PATH_MAX_LEN = 100;
	char targetPathName[FILE_PATH_MAX_LEN];
	uint16_t pathLen = 0;
	
	pathLen = strlen(pTargetFolderPath) + strlen(pTargetFileName) + 1;	//+1：中间有一个'/'符号
	if (pathLen >= FILE_PATH_MAX_LEN)
	{
		return ERROR; 
	}
	
	strcpy(targetPathName,pTargetFolderPath);
	strcat(targetPathName,"/");
	strcat(targetPathName,pTargetFileName);

	if (ERROR == DeleteFileFromDevices(targetPathName) )
	{
		return ERROR;
	}
	
	return SUCCESS;
}

/*
*********************************************************************************************************
* Function Name : DeleteFolderBody
* Description	: 从设备删除文件夹体
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static ErrorStatus DeleteFolderBody( const char * const pTargetFolderPath,\
				const char * const pTargetFolderName, void (*GUI_CallBack)( const char *FolderNamePtr, const char *FileNamePtr ) )
{
	const uint8_t FILE_PATH_MAX_LEN = 100;
	char targetPathName[FILE_PATH_MAX_LEN];
	uint16_t pathLen = 0;
	
	pathLen = strlen(pTargetFolderPath) + strlen(pTargetFolderName) + 1;	//+1：中间有一个'/'符号
	if (pathLen >= FILE_PATH_MAX_LEN)
	{
		return ERROR; 
	}
	
	strcpy(targetPathName,pTargetFolderPath);
	strcat(targetPathName,"/");
	strcat(targetPathName,pTargetFolderName);

	if (ERROR == DeleteFolderFromDevices(targetPathName,GUI_CallBack) )
	{
		return ERROR;
	}
	
	return SUCCESS;
}

/*
*********************************************************************************************************
* Function Name : DeleteFolderFromDevices
* Description	: 从设备删除文件夹（路径不能以'/'结尾）
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
ErrorStatus DeleteFolderFromDevices( const char * const pTargetFolderPath, \
				void (*GUI_CallBack)( const char *FolderNamePtr, const char *FileNamePtr ) )
{
	DIR dir_object;  
	FILINFO targetFileInfo; 
	FRESULT fresult;
	
	fresult = f_opendir(&dir_object,pTargetFolderPath); 	
	if (fresult != FR_OK)
	{
		return ERROR; 
	}
	
	for (;;)
	{
		fresult = f_readdir(&dir_object,&targetFileInfo);		
		if (fresult != FR_OK)
		{
			return ERROR; 
		}
		
		/* 文件夹内文件读取完毕 */
		if ( !targetFileInfo.fname[0] )	
		{	
			/* 0表示根目录，不能删除 */
			if (dir_object.obj.sclust != 0)
			{
				if (DeleteFileFromDevices(pTargetFolderPath) == ERROR)
				{
					return ERROR;
				}
			}					
			break;	
		}

		{
			const char *pName = targetFileInfo.fname;
			
			if (targetFileInfo.fattrib & AM_ARC)		//是个文件
			{			
				/* 回调函数，执行用户程序 */
				if (GUI_CallBack != NULL)
				{
					GUI_CallBack(NULL,pName);
				}
				
				if ( DeleteFileBody(pTargetFolderPath,pName) == ERROR)
				{
					return ERROR;
				}
			}
			else if (targetFileInfo.fattrib & AM_DIR)		//是个文件夹
			{	
				/* 回调函数，执行用户程序 */
				if (GUI_CallBack != NULL)
				{
					GUI_CallBack(pName,NULL);
				}
				
				if ( DeleteFolderBody(pTargetFolderPath,pName,GUI_CallBack) == ERROR)
				{
					return ERROR;
				}
			}
		}
	}
	
	return SUCCESS;
}

/*
*********************************************************************************************************
* Function Name : FileStreamRead
* Description	: 读取文件流
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
FRESULT FileStreamRead( const char *path, uint8_t *rxBodyPtr, uint32_t size )
{
	FRESULT fresult;
	FIL file_obj;  
	uint32_t br; 
	
	fresult = f_open(&file_obj,path,FA_OPEN_EXISTING|FA_READ); 
	if (fresult != FR_OK)
	{
		return fresult;
	}
	
	fresult = f_read(&file_obj,rxBodyPtr,size,&br);
	
	f_close(&file_obj);
		
	return fresult;
}

/*
*********************************************************************************************************
* Function Name : FileStreamWrite
* Description	: 写入文件流
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
FRESULT FileStreamWrite( const char *path, uint8_t *txBodyPtr, uint32_t size )
{
	FRESULT fresult;
	FIL file_obj;  
	uint32_t br; 
	
	fresult = CreatFolder(path);
	if ((fresult!=FR_EXIST) && (fresult!=FR_OK))
	{		
		return fresult;
	}

	fresult = f_open(&file_obj,path,FA_CREATE_ALWAYS|FA_WRITE); 
	if (fresult != FR_OK)
	{
		return fresult;
	}
	
	fresult = f_write(&file_obj,txBodyPtr,size,&br);	
	f_close(&file_obj);
		
	return fresult;	
}

/*
*********************************************************************************************************
* Function Name : FileStreamWriteAppend
* Description	: 追加写入文件流
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
FRESULT FileStreamWriteAppend( const char *path, uint8_t *txBodyPtr, uint32_t size )
{
	FRESULT fresult;
	FIL obj; 
	uint32_t br;
	
	fresult = f_open(&obj, path, FA_OPEN_EXISTING | FA_WRITE);
	if (fresult != FR_OK)
	{
		fresult = CreatFolder(path);
		if ((fresult != FR_EXIST) && (fresult != FR_OK))
		{		
			return fresult;
		}
		
		fresult = f_open(&obj, path, FA_OPEN_ALWAYS | FA_WRITE);
		if (fresult != FR_OK)
		{
			return fresult;
		}
	}
	
	f_lseek(&obj, f_size(&obj));
	fresult = f_write(&obj, txBodyPtr, size, &br);
	f_close(&obj);
	
	return fresult;
}


/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
