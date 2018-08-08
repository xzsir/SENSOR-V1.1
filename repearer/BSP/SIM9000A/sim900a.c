#include "sim900a.h"
#include "string.h"
#include "stm32f10x.h"
#include "usart.h"
#include "SysTick.h"

#define Buf2_Max 200 					  //串口2缓存长度
/*************	本地常量声明	**************/
const char *string = "AT+CIPSTART=\"TCP\",\"47.93.49.83\",443";	//IP登录服务器

/*************  本地变量声明	**************/
//char Uart1_Buf[Buf1_Max];
extern char Uart2_Buf[Buf2_Max];
extern  u8 Times,First_Int,shijian;
extern u16 Heartbeat;
extern vu8 Timer0_start;	//定时器0延时启动计数器
extern vu8 Uart2_Start;	//串口2开始接收数据
extern vu8 Uart2_End;	  //串口2接收数据结束
extern vu8 Heart_beat;		//发送心跳帧标志位
extern char recei_data[255];

/*******************************************************************************
* 函数名 : CLR_Buf2
* 描述   : 清除串口2缓存数据
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void CLR_Buf2(void)
{
	u16 k;
	for(k=0;k<Buf2_Max;k++)      //将缓存内容清零
	{
		Uart2_Buf[k] = 0x00;
	}
    First_Int = 0;              //接收字符串的起始存储位置
}

/*******************************************************************************
* 函数名 : Find
* 描述   : 判断缓存中是否含有指定的字符串
* 输入   : 
* 输出   : 
* 返回   : unsigned char:1 找到指定字符，0 未找到指定字符 
* 注意   : 
*******************************************************************************/

u8 Find(char *a)
{ 
  if(strstr(Uart2_Buf,a)!=NULL)
	    return 1;
	else
			return 0;
}

/*******************************************************************************
* 函数名 : Second_AT_Command
* 描述   : 发送AT指令函数
* 输入   : 发送数据的指针、发送等待时间(单位：S)
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/

void Second_AT_Command(char *b,char *a,u8 wait_time)         
{
	u8 i;
	char *c;
	c = b;										//保存字符串地址到c
	CLR_Buf2(); 
  i = 0;
	while(i == 0)                    
	{
		if(!Find(a)) 
		{
			if(Timer0_start == 0)
			{
				b = c;							//将字符串地址给b
				for (; *b!='\0';b++)
				{
					while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
					USART_SendData(USART2,*b);//UART2_SendData(*b);
				}
				UART2_SendLR();	
				Times = 0;
				shijian = wait_time;
				Timer0_start = 1;
		   }
    }
 	  else
		{
			i = 1;
			Timer0_start = 0;
		}
	}
	CLR_Buf2(); 
}

/*******************************************************************************
* 函数名 : Wait_CREG
* 描述   : 等待模块注册成功
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Wait_CREG(void)
{
	u8 i;
	u8 k;
	i = 0;
	CLR_Buf2();
  while(i == 0)        			
	{
		CLR_Buf2();        
		UART2_SendString("AT+CREG?");
		UART2_SendLR();
		Delay_nMs(5000);  						
	    for(k=0;k<Buf2_Max;k++)      			
    	{
			if(Uart2_Buf[k] == ':')
			{
				if((Uart2_Buf[k+4] == '1')||(Uart2_Buf[k+4] == '5'))
				{
					i = 1;
					UART1_SendLR();
				  break;
				}
			}
		}
		UART1_SendString("注册中.....");
	}
}

/*******************************************************************************
* 函数名 : Set_ATE0
* 描述   : 取消回显
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Set_ATE0(void)
{
	Second_AT_Command("ATE0","OK",3);								//取消回显		
}
/*******************************************************************************
* 函数名 : Connect_Server
* 描述   : GPRS连接服务器函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Connect_Server(void)
{
	UART2_SendString("AT+CIPCLOSE=1");	//关闭连接
  Delay_nMs(100);
	Second_AT_Command("AT+CIPSHUT","SHUT OK",2);		//关闭移动场景
	Second_AT_Command("AT+CGCLASS=\"B\"","OK",2);//设置GPRS移动台类别为B,支持包交换和数据交换 
	Second_AT_Command("AT+CGDCONT=1,\"IP\",\"CMNET\"","OK",2);//设置PDP上下文,互联网接协议,接入点等信息
	Second_AT_Command("AT+CGATT=1","OK",2);//附着GPRS业务
	Second_AT_Command("AT+CIPCSGP=1,\"CMNET\"","OK",2);//设置为GPRS连接模式
	Second_AT_Command("AT+CIPHEAD=1","OK",2);//设置接收数据显示IP头(方便判断数据来源,仅在单路连接有效)
	Second_AT_Command((char*)string,"OK",5);
	Delay_nMs(100);
	CLR_Buf2();
}
/*******************************************************************************
* 函数名 : Rec_Server_Data
* 描述   : 接收服务器数据函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Rec_Server_Data(void)
{
	if(strstr(Uart2_Buf,"+IPD")!=NULL)   		//若缓存字符串中含有^SISR
	{	
		Heartbeat=0;	//清除心跳帧计数器
		Heart_beat=0;
		Delay_nMs(100);
		if(strstr(Uart2_Buf,"onled")!=NULL)
		{
			//LED1_ON();
		}
		else if(strstr(Uart2_Buf,"offled")!=NULL)
		{
		//	LED1_OFF();
		}
		UART1_SendString("收到新信息：\r\n");
		UART1_SendString(Uart2_Buf);
		CLR_Buf2();
		Heart_beat=1;//发送应答数据，告诉服务器收到数据		
	}
}
/*******************************************************************************
* 函数名 : Send_OK
* 描述   : 发送数据应答服务器的指令，该函数在有两功能
					1：接收到服务器的数据后，应答服务器
					2：服务器无下发数据时，每隔10秒发送一帧心跳，保持与服务器连接
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Send_OK(void)
{
	
	strcat (recei_data,"\32\0");
	
	Second_AT_Command("AT+CIPSEND",">",2);
	
	Second_AT_Command(recei_data,"SEND OK",8);;			//回复OK 
	memset( recei_data, 0, sizeof(recei_data));
	
	
}
