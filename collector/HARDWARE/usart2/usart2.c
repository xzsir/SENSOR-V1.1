#include "usart2.h"
#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "usart.h"
#include "delay.h"
#include "oled.h"
#include "string.h"


/****�����������flag****/
uint8_t Rx_data_flag=0;
uint8_t Rx_control_flag=0;
uint8_t ucTemp;
uint16_t usRxCount=0;
char ucaRxBuf[256];
char compare[4]={'$','3','5','1'};

char compare2[4]={'@','3','5','1'};

extern uint8_t  Tx_data_flag;

/********************************************************************************************
����2���ͺ���
********************************************************************************************/

void Usart2_Send(char *str)
 {	
	 	
	while(*str != '\0')
	{		
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);//�ж��Ƿ������
		USART_SendData(USART2 , *str);
		str++;
	}
}


/********************************************************************************************
�˿ڳ�ʼ������
********************************************************************************************/
void usart2_init(u32 bound)
{		
	GPIO_InitTypeDef GPIO_Usart2;																//����˿ڱ���
	USART_InitTypeDef USART_Usart2;
	
	NVIC_InitTypeDef NVIC_Usart2;																//�����жϿ��������� 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);							//�����ж����ȼ�
	
	NVIC_Usart2.NVIC_IRQChannel = USART2_IRQn;	 								//����Ϊ����1ȫ���ж�
	NVIC_Usart2.NVIC_IRQChannelPreemptionPriority = 1;					//������ռ���ȼ�
	NVIC_Usart2.NVIC_IRQChannelSubPriority = 0;									//���ô����ȼ�
	NVIC_Usart2.NVIC_IRQChannelCmd = ENABLE;										//IRQͨ��ʹ��
	NVIC_Init(&NVIC_Usart2);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);			//ʹ�˿�GPIOAʱ����Ч��ʹ������ʱ��2
		
	//TX   PA2
 	GPIO_Usart2.GPIO_Pin = GPIO_Pin_2 ;													//����GPIO�Ӷ˿ں�	
  GPIO_Usart2.GPIO_Mode = GPIO_Mode_AF_PP;   								  //����Ϊ�����������
  GPIO_Usart2.GPIO_Speed = GPIO_Speed_50MHz; 									//���ö˿����Ƶ��Ϊ50MHz
  GPIO_Init(GPIOA, &GPIO_Usart2);		  												//��ʼ���˿�
	
	//RX   PA3
	GPIO_Usart2.GPIO_Pin = GPIO_Pin_3;													//����GPIO�Ӷ˿ں�
	GPIO_Usart2.GPIO_Mode = GPIO_Mode_IN_FLOATING;							//��������
	GPIO_Init(GPIOA, &GPIO_Usart2);															//��ʼ���˿�
	
	USART_Usart2.USART_BaudRate = bound;													//���ò�����
	USART_Usart2.USART_WordLength = USART_WordLength_8b;				//��������λ
	USART_Usart2.USART_StopBits = USART_StopBits_1;							//����ֹͣλ
	USART_Usart2.USART_Parity = USART_Parity_No ;								//����У��λ
	USART_Usart2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;			//�ر�Ӳ������ģʽ
	USART_Usart2.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//ʹ��
	USART_Init(USART2,&USART_Usart2); 													//��ʼ������
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);							//���ý��������ж�
	USART_Cmd(USART2, ENABLE);																	//��������һ
}


/***********************************
�������ܣ�����2 �жϷ�������ȡ���͵�ָ��
���͸�ʽ����$��ʼ�Իس�����
***********************************/
void USART2_IRQHandler(void)
{

		if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//���� �����ж�
	  {
		    ucTemp=USART2->DR;//��ȡ���յ�������
        if(ucTemp=='$')    //���յ���ʼ�ַ�
				{
					 Rx_data_flag=1;
				}
				if(ucTemp=='@')    //���յ���ʼ�ַ�
				{
					 Rx_control_flag=1;
				}
				
		}
		
		if(Rx_data_flag)
		{
			if (usRxCount < sizeof(ucaRxBuf))
			{
				ucaRxBuf[usRxCount++] = ucTemp;
				if(usRxCount==4)
				{
				  if((compare[0]==ucaRxBuf[0])&&(compare[1]==ucaRxBuf[1])&&(compare[2]==ucaRxBuf[2])&&(compare[3]==ucaRxBuf[3]))
						Rx_data_flag=1;
					else 
         {
				    Rx_data_flag=0;  
				    memset( ucaRxBuf, 0, sizeof(ucaRxBuf));}
				 }
				if(usRxCount>6)
				{
				  Rx_data_flag=0;
					usRxCount=0;
					memset( ucaRxBuf, 0, sizeof(ucaRxBuf));
			  }
				
				 
			}
			else
			{
				usRxCount = 0;
			  memset( ucaRxBuf, 0, sizeof(ucaRxBuf));
			}
			
			/* ���������ַ�����Ϊ���յ�һ������ */
			if (ucTemp == 0x0A && Rx_data_flag)	/* �����ַ� */
			{		
				 Tx_data_flag=1;
         Rx_data_flag=0;
			   usRxCount = 0;
			   memset( ucaRxBuf, 0, sizeof(ucaRxBuf));
			}
		}		

	
	
    

		if(Rx_control_flag)
		{
			if (usRxCount < sizeof(ucaRxBuf))
			{
				ucaRxBuf[usRxCount++] = ucTemp;
				if(usRxCount==4)
				{
				  if((compare2[0]==ucaRxBuf[0])&&(compare2[1]==ucaRxBuf[1])&&(compare2[2]==ucaRxBuf[2])&&(compare2[3]==ucaRxBuf[3]))
						Rx_control_flag=1;
					else 
					 {
							Rx_control_flag=0;  
							memset( ucaRxBuf, 0, sizeof(ucaRxBuf));
					 }
				 }
				if(usRxCount>6)
				{
					Rx_control_flag=0;
				}
		
			}		
				else
				{
					usRxCount = 0;
				}
			
			/* ���������ַ�����Ϊ���յ�һ������ */
			if (ucTemp == 0x0A && Rx_control_flag)	/* �����ַ� */
			{		
				 Usart2_Send(ucaRxBuf);
         Rx_control_flag=0;
			   usRxCount = 0;
			   memset( ucaRxBuf, 0, sizeof(ucaRxBuf));
			}
			
		}

}



