/****************************************Copyright (c)**************************************************
**                               	杭州鑫高科技有限公司
**                                   
**                                 
** 文   件   名: ustring.c
** 最后修改日期: 2014/4/23 12:47:45
** 描        述: 字符串函数
** 版	     本: V1.0
** 主  控  芯 片: 	 晶振频率:	
** IDE:MDK4.12
**********************************************************************************************************/
#include "utils.h"


/**********************************************************************
functionName:uint08 bcd2val_8(uint08 bcd)
description:BCD码到值的转换
**********************************************************************/
ErrorStatus bcd2val_8(uint8_t bcd,uint8_t *val)
{
	if((bcd&0x0f)>9)
	return ERROR;
	if((bcd>>4)>9) 
	return ERROR;	  		
	*val=(bcd>>4)*10+(bcd&0x0f);
	return SUCCESS;
}

/**********************************************************************
functionName:uint08 val2bcd_8(uint08 val)
description:值到BCD码的转换
**********************************************************************/ 
uint8_t val2bcd_8(uint8_t val)
{
	return ((val/10)<<4|(val%10));
} 

/**********************************************************************
functionName:uint16 val2bcd_16(uint16 val)
description:值到BCD码的转换16位
**********************************************************************/ 
uint16_t val2bcd_16(uint16_t val)
{
	uint16_t bcd=0x0000;
	uint16_t temp16u;
	uint8_t i;
	for(i=0;i<4;i++)
	{
		temp16u=val%10;
		temp16u<<=(i<<2);
		bcd|=temp16u;
		val/=10;
	}		
	return bcd;
} 

/**********************************************************************
functionName:ErrorStatus bcd2val_16(uint16_t bcd,uint16_t *val)
description:值到BCD码的转换16位
**********************************************************************/  
ErrorStatus bcd2val_16(uint16_t bcd,uint16_t *val)
{ 
    uint8_t i;
    uint16_t temp16u; 
    uint16_t temp_val=0; 
    for(i=0;i<4;i++) 
    { 
        temp_val*= 10; 
        temp16u=(bcd & 0xf000) >> 12;
        if(temp16u>9)
        return 	ERROR;  
        temp_val+=temp16u; 
        bcd <<= 4; 
    } 
    *val=temp_val; 
 	return SUCCESS;
} 

/**********************************************************************
functionName:uint32_t val2bcd_32(uint32_t val)
description:值到BCD码的转换32位
**********************************************************************/  
uint32_t val2bcd_32(uint32_t val)
{
	uint32_t bcd=0x00000000;
	uint32_t temp32u;
	uint8_t i;
	for(i=0;i<8;i++) 
	{
		temp32u=val%10;
		temp32u<<=(i<<2);
		bcd|=temp32u;
		val/=10; 
	}
	return bcd;
}

/**********************************************************************
functionName:ErrorStatus bcd2val_32(uint32_t bcd,uint32_t *val) 
description:BCD码到值的转换32位
**********************************************************************/  
ErrorStatus bcd2val_32(uint32_t bcd,uint32_t *val) 
{ 
    uint8_t i;    
    uint32_t temp32u; 
    uint32_t temp_val=0; 
    for(i=0;i<8;i++) 
    { 
        temp_val*= 10; 
        temp32u=(bcd & 0xf0000000) >> 28;
        if(temp32u>9)
        return 	ERROR; 
        temp_val+=temp32u; 
        bcd <<= 4; 
    } 
    *val=temp_val;
    return SUCCESS;
}

/**********************************************************************
functionName:ErrorStatus bcd2str_16(uint16_t bcd,char *str)
description:BCD码到字符串的转换16位
**********************************************************************/  
ErrorStatus bcd2str_16(uint16_t bcd,char *str)
{
	char *s;
	char tempc;
	uint8_t i;
	s=str;
	for(i=0;i<4;i++)
	{
		tempc=(char)(bcd>>12)+'0';
		if(tempc>'9')
		return 	ERROR;
		bcd<<=4;
		*s=tempc;
		s++;
	}
	*s=0;
	return SUCCESS;
} 

/**********************************************************************
functionName:ErrorStatus bcd2str_32(uint32_t bcd,char *str)
description:BCD码到字符串的转换16位
**********************************************************************/  
ErrorStatus bcd2str_32(uint32_t bcd,char *str)
{
	char *s;
	char tempc;
	uint8_t i;
	s=str;
	for(i=0;i<8;i++)
	{
		tempc=(char)(bcd>>28)+'0';
		if(tempc>'9')
		return 	ERROR;
		bcd<<=4;
		*s=tempc;
		s++;
	}
	*s=0;
	return SUCCESS;
}

const char hex_code[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

/**********************************************************************
functionName:void hex2str_16(uint16_t hex,char *str)
description: 将32位的数以16进制的方式转换成字符串
**********************************************************************/  
void hex2str_32(uint32_t hex,char *str)
{	
	uint8_t i;
	char *s;
	s=str;
	for(i=0;i<8;i++)
	{ 
		*s=hex_code[(hex>>28)&0x0000000f];
		hex<<=4;
		s++;
	}
	*s=0;
}

/**********************************************************************
functionName:void hex2str_16(uint16_t hex,char *str)
description: 将16位的数以16进制的方式转换成字符串
**********************************************************************/  
void hex2str_16(uint16_t hex,char *str)
{	
	uint8_t i;
	char *s;
	s=str;
	for(i=0;i<4;i++)
	{ 
		*s=hex_code[(hex>>12)&0x000f];
		hex<<=4;
		s++;
	}
	*s=0;
}

/**********************************************************************
functionName:void hex2str_8(uint08 hex,char *str)
description: 将8位的数以16进制的方式转换成字符串
**********************************************************************/  
void hex2str_8(uint8_t hex,char *str)
{ 
	str[0]=hex_code[hex>>4];
	str[1]=hex_code[hex&0x0F];
	str[2]=0;
}


/**********************************************************************
functionName:ErrorStatus str2bcd_32(uint32 *bcd,char *str)
description:将字符串转换为bcd码，如果字符串的8个字符不合法，返回ERROR
**********************************************************************/  
ErrorStatus str2bcd_32(uint32_t *bcd,char *str)
{
	uint8_t i;
	char tempc;
	*bcd=0x00000000; 
	for(i=0;i<8;i++)
	{
		if(str[i]>='0'&&(str[i]<='9'))
		{
			tempc=str[i]-'0';
			*bcd<<=4;
			*bcd+=tempc;
		}
		else
		return ERROR;  	
	}
	return SUCCESS;
}

/**********************************************************************
functionName:TestStatus  buffcmp(uint08 *buff1,uint08 *buff2,uint16 len)
description:比较指定长度的两串数据是否相同
**********************************************************************/	 
TestStatus  buffcmp(uint8_t *buff1,uint8_t *buff2,uint16_t len)
{ 
	while(len--)
	{
		if(*buff1!=*buff2)
		return FAILED;	
		buff1++;
		buff2++;
	}
	return PASSED;
} 


/**********************************************************************
functionName:uint16 float2str(float f,char *str,uint08 int_len,uint08 dec_len)
description: 本函数实现的目的：
1、把浮点数转换成指定整数长度的，指定小数长度的字符串，中间带小数点
2、整数部分不能大于6位，否者整数显示为6位
3、整数部分不能小于2位，否者整数显示为2位.
4、小数部分不能大于3位，否者小数显示为3位.
5、如果小数部分为0，只显示整数部分
6、如果实际整数长度>指定整数长度，则不转换小数部分，只显示整数部分,
7、整数部分长度如果小于指定长度，前面补上空格。
8、如果数字在0~（-1）之间的小数，整数部分算作2位，0和‘-’号
9、小数最多只能是3位
10、如果整数部分数字大于999999，或者小于-99999，则显示------
11、小数部分经过四舍五入后显示
**********************************************************************/ 
uint16_t float2str(float f,char *str,uint8_t int_len,uint8_t dec_len)
{
	uint16_t cnt;
	uint16_t cnt_all;
	int32_t intg;		
	int32_t dec;
	char format[4];
	uint8_t i=0;   
	char str_buff[51] = {0};
	
	if(int_len>6)
	int_len=6;	
	else if(int_len<2)
	int_len=2;		 
	format[0]='%';
	format[1]=int_len+'0'; 
	format[2]='d';					
	format[3]=0;	//整数部分
	
	if((f>999999.5f)||(f<-99999.5f))		//如果很大的数字则
	{
		for(i=0;i<(int_len+dec_len+1);i++)
		*(str_buff+i)='-';	 
		*(str_buff+i)=0;
		strcpy(str,str_buff);
		
		return(int_len+dec_len+1);
	}
	switch(dec_len)
	{ 	
		case 0:
		{
			if(f>0) 
			f+=0.501f;
			else 
			f-=0.501f;
			
			intg=(int32_t)f; 
			cnt=usprintf(str_buff,format,intg);
			strcpy(str,str_buff);
			
			return cnt;
		}	
		case 1:
		{
			if(f>0) 
			f+=0.0501f;
			else 
			f-=0.0501f;
			break;
		}	
		case 2:
		{
			if(f>0) 
			f+=0.00501f;
			else 
			f-=0.00501f;
			break;
		}
		default:
		{
			dec_len=3;
			if(f>0) 
			f+=0.000501f;
			else
			f-=0.000501f;
			break;
		}
	}	
	intg=(int32_t)f;	
	dec=fabs((f-intg))*1000;				//小数部分*1000   	 	
	cnt=usprintf(str_buff,format,intg);
	if(cnt>int_len)		//只显示整数部分
	{
		format[1]=int_len+1+dec_len+'0'; 
		cnt=usprintf(str_buff,format,intg);
		strcpy(str,str_buff);
		
		return cnt;
	} 
	if((f<0)&&(!intg)) 		//0~（-1）的数字加‘-’号
	str_buff[int_len-2]='-';    
	str_buff[cnt]='.'; 
	cnt_all=usprintf(&(str_buff[cnt+1]),"%03d",dec); 
	cnt_all+=cnt+1;
	switch(dec_len)
	{ 	
		case 1:
		{
			cnt_all-=2;
			str_buff[cnt_all]=0;
			break;
		}	
		case 2:
		{ 
			cnt_all-=1;
			str_buff[cnt_all]=0;
			break;
		}
		default:	 
		break;
	} 
	strcpy(str,str_buff);
	
	return cnt_all;
} 


/**********************************************************************
functionName:float str2float(char *str)
description: 字符串转换为浮点数字的函数
**********************************************************************/ 
float str2float(char *str)
{
	uint32_t dec;
	int32_t intg;
	const char *ppc[1];
	char *s=str; 
	float f; 
	uint8_t i=0;
	uint8_t flag;
	char buff[4];
	intg=ustrtoul(str,ppc,10);
	if(**ppc!='.')
	return (float)intg;
	else
	{
		while(*s!='.')		
		*s++;
		*s++;
		flag=0;
		for(i=0;i<3;i++)
		{
			buff[i]=s[i];	
			if(!flag)
			{
				if((buff[i]<'0')||(buff[i]>'9'))
				{
					flag=1;
					buff[i]='0';	
				}	
			}	
			else
			buff[i]='0';	
		} 
		buff[3]=0;
		dec=ustrtoul(buff,0,10);
	}	
	dec+=5;	
	dec/=10;

	if(str[0]=='-')
	f=(float)intg-dec/100.0f;	
	else 
	f=(float)intg+dec/100.0f; 
	return f; 
}


/******************* (C) COPYRIGHT 2012 XinGao Tech *****END OF FILE****/
