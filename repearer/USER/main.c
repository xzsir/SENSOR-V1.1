/**********************************************************************************
 * 工程名  :GPRS
 * 描述    :GPRS测试程序_动态IP
 * 实验平台:STM32F10X
 * 库版本  :
 * 作者    :泥人通信模块开发平台团队 
 * 博客    :http://nirenelec.blog.163.com
 * 淘宝    :http://shop105683814.taobao.com
**********************************************************************************/

#include "stm32f10x.h"
#include "usart.h"
#include "Led.h"
#include "SysTick.h"
#include "timer.h"
#include "string.h"
#include "sim900a.h"

//#define Buf1_Max 60 					  //串口1缓存长度
#define Buf2_Max 200 					  //串口2缓存长度


char Uart2_Buf[Buf2_Max];

u8 Times=0,First_Int = 0,shijian=0;
u16 Heartbeat=0;

vu8 Timer0_start;	//定时器0延时启动计数器
vu8 Uart2_Start;	//串口2开始接收数据
vu8 Uart2_End;	  //串口2接收数据结束
vu8 Heart_beat;		//发送心跳帧标志位

uint8_t  usart_Tx_data_flag;

/*************  外部函数和变量声明*****************/




/*******************************************************************************
* 函数名 : main 
* 描述   : 主函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 串口2负责与MG323模块通信，串口1用于串口调试，可以避免在下载程序时数据
					 还发送到模块
*******************************************************************************/
int main(void)
{
	SysTick_Init_Config();
	GPIO_Config();

	USART1_Init_Config(9600);
	USART2_Init_Config(9600);
	Timer2_Init_Config();
//	UART1_SendString("GPRS模块GPRS测试程序\r\n");
//	UART1_SendString("GPRS模块在注册网络\r\n");
	Wait_CREG();
//	UART1_SendString("GPRS模块注册成功\r\n");
//	UART1_SendString("GPRS模块开始连接服务器\r\n");
	Set_ATE0();
	Connect_Server();
	UART1_SendString("连接成功\r\n");
	while(1)
	{
		Rec_Server_Data();
		
		if(Heart_beat)
		{
			//Send_OK();
			UART1_SendString("$351\r\n");
		  Heart_beat=0;
		}
		if(usart_Tx_data_flag==1)
		{
	  	Send_OK();
			//UART1_SendString("66666\r\n");
			usart_Tx_data_flag=0;
		}
	}
}

/*******************************************************************************
* 函数名  : USART2_IRQHandler
* 描述    : 串口1中断服务程序
* 输入    : 无
* 返回    : 无 
* 说明    : 
*******************************************************************************/
void USART2_IRQHandler(void)                	
{
			u8 Res=0;
			Res =USART_ReceiveData(USART2);
			Uart2_Buf[First_Int] = Res;  	  //将接收到的字符串存到缓存中
			First_Int++;                			//缓存指针向后移动
			if(First_Int > Buf2_Max)       		//如果缓存满,将缓存指针指向缓存的首地址
			{
				First_Int = 0;
			}    
} 	

/*******************************************************************************
* 函数名  : TIM2_IRQHandler
* 描述    : 定时器2中断断服务函数
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void TIM2_IRQHandler(void)   //TIM3中断
{
	static u8 flag =1;

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志 
		
		if(Timer0_start)
		Times++;
		if(Times > shijian)
		{
			Timer0_start = 0;
			Times = 0;
		}
		
		Heartbeat++;
		if(Heartbeat>9)//每10秒发送心跳帧
		{
			Heartbeat=0;
			Heart_beat=1;
		}
		if(flag)
		{
			flag=0;
		}
		else
		{
			flag=1;
		}
	}	
}
