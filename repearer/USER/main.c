/**********************************************************************************
 * ������  :GPRS
 * ����    :GPRS���Գ���_��̬IP
 * ʵ��ƽ̨:STM32F10X
 * ��汾  :
 * ����    :����ͨ��ģ�鿪��ƽ̨�Ŷ� 
 * ����    :http://nirenelec.blog.163.com
 * �Ա�    :http://shop105683814.taobao.com
**********************************************************************************/

#include "stm32f10x.h"
#include "usart.h"
#include "Led.h"
#include "SysTick.h"
#include "timer.h"
#include "string.h"
#include "sim900a.h"

//#define Buf1_Max 60 					  //����1���泤��
#define Buf2_Max 200 					  //����2���泤��


char Uart2_Buf[Buf2_Max];

u8 Times=0,First_Int = 0,shijian=0;
u16 Heartbeat=0;

vu8 Timer0_start;	//��ʱ��0��ʱ����������
vu8 Uart2_Start;	//����2��ʼ��������
vu8 Uart2_End;	  //����2�������ݽ���
vu8 Heart_beat;		//��������֡��־λ

uint8_t  usart_Tx_data_flag;

/*************  �ⲿ�����ͱ�������*****************/




/*******************************************************************************
* ������ : main 
* ����   : ������
* ����   : 
* ���   : 
* ����   : 
* ע��   : ����2������MG323ģ��ͨ�ţ�����1���ڴ��ڵ��ԣ����Ա��������س���ʱ����
					 �����͵�ģ��
*******************************************************************************/
int main(void)
{
	SysTick_Init_Config();
	GPIO_Config();

	USART1_Init_Config(9600);
	USART2_Init_Config(9600);
	Timer2_Init_Config();
//	UART1_SendString("GPRSģ��GPRS���Գ���\r\n");
//	UART1_SendString("GPRSģ����ע������\r\n");
	Wait_CREG();
//	UART1_SendString("GPRSģ��ע��ɹ�\r\n");
//	UART1_SendString("GPRSģ�鿪ʼ���ӷ�����\r\n");
	Set_ATE0();
	Connect_Server();
	UART1_SendString("���ӳɹ�\r\n");
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
* ������  : USART2_IRQHandler
* ����    : ����1�жϷ������
* ����    : ��
* ����    : �� 
* ˵��    : 
*******************************************************************************/
void USART2_IRQHandler(void)                	
{
			u8 Res=0;
			Res =USART_ReceiveData(USART2);
			Uart2_Buf[First_Int] = Res;  	  //�����յ����ַ����浽������
			First_Int++;                			//����ָ������ƶ�
			if(First_Int > Buf2_Max)       		//���������,������ָ��ָ�򻺴���׵�ַ
			{
				First_Int = 0;
			}    
} 	

/*******************************************************************************
* ������  : TIM2_IRQHandler
* ����    : ��ʱ��2�ж϶Ϸ�����
* ����    : ��
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
void TIM2_IRQHandler(void)   //TIM3�ж�
{
	static u8 flag =1;

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		
		if(Timer0_start)
		Times++;
		if(Times > shijian)
		{
			Timer0_start = 0;
			Times = 0;
		}
		
		Heartbeat++;
		if(Heartbeat>9)//ÿ10�뷢������֡
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
