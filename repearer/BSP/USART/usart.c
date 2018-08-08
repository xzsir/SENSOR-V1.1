/**********************************************************************************
 * �ļ���  ��usart.c
 * ����    ��usart1 Ӧ�ú�����          
 * ʵ��ƽ̨��NiRen_TwoHeartϵͳ��
 * Ӳ�����ӣ�TXD(PB9)  -> �ⲿ����RXD     
 *           RXD(PB10) -> �ⲿ����TXD      
 *           GND	   -> �ⲿ����GND 
 * ��汾  ��ST_v3.5
**********************************************************************************/

#include "usart.h"	  
#include "string.h"
#include "sim900a.h"
	
vu8 Usart1_R_Buff[USART1_REC_MAXLEN];	//����1���ݽ��ջ����� 
vu8 Usart1_R_State=0;					//����1����״̬
vu16 Usart1_R_Count=0;					//��ǰ�������ݵ��ֽ��� 	  

char recei_data[255];
/*******************************************************************************
* ������  : USART1_Init_Config
* ����    : USART1��ʼ������
* ����    : bound��������(���ã�2400��4800��9600��19200��38400��115200��)
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
void USART1_Init_Config(u32 bound)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	/*ʹ��USART1��GPIOA����ʱ��*/  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	

	/*��λ����1*/
 	USART_DeInit(USART1);  
	
	/*USART1_GPIO��ʼ������*/ 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			//USART1_TXD(PA.9)     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//������������������Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);				//���ÿ⺯���е�GPIO��ʼ����������ʼ��USART1_TXD(PA.9)  
   
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				//USART1_RXD(PA.10)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);					//���ÿ⺯���е�GPIO��ʼ����������ʼ��USART1_RXD(PA.10)


   /*USART1 ��ʼ������*/
	USART_InitStructure.USART_BaudRate = bound;										//���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//����ģʽ����Ϊ�շ�ģʽ
    USART_Init(USART1, &USART_InitStructure);										//��ʼ������1

   /*Usart1 NVIC����*/
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							//����ָ���Ĳ�����ʼ��VIC�Ĵ��� 
	  
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			//ʹ�ܴ���1�����ж�

    USART_Cmd(USART1, ENABLE);                    			//ʹ�ܴ��� 
	USART_ClearFlag(USART1, USART_FLAG_TC);					//���������ɱ�־
}


/*******************************************************************************
* ������  : UART1_SendString
* ����    : USART1�����ַ���
* ����    : *s�ַ���ָ��
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
void UART1_SendString(char* s)
{
	while(*s)//����ַ���������
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET); 
		USART_SendData(USART1 ,*s++);//���͵�ǰ�ַ�
	}
}

/*******************************************************************************
* ������  : USART1_IRQHandler
* ����    : ����1�жϷ������
* ����    : ��
* ����    : �� 
* ˵��    : 1)��ֻ������USART1�жϽ��գ�δ����USART1�жϷ��͡�
*           2)�����յ�0x0d 0x0a(�س���"\r\n")����֡���ݽ������
*******************************************************************************/


/****�����������flag****/
uint8_t usart_Rx_data_flag=0;
uint8_t usart_Rx_control_flag=0;
uint8_t usart_ucTemp;
uint16_t usart_usRxCount=0;
char usart_ucaRxBuf[256];
char usart_compare[2]={'$','>'};
char usart_compare2[4]={'@','3','5','1'};

extern uint8_t  usart_Tx_data_flag;

void USART1_IRQHandler(void)                	
{
	  int i=0;
		if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//���� �����ж�
	  {
		    usart_ucTemp=USART1->DR;//��ȡ���յ�������
        if(usart_ucTemp=='$')    //���յ���ʼ�ַ�
				{
					 usart_Rx_data_flag=1;
				}
				if(usart_ucTemp=='@')    //���յ���ʼ�ַ�
				{
					 usart_Rx_control_flag=1;
				}
				
		}
		
		if(usart_Rx_data_flag)
		{
			if (usart_usRxCount < sizeof(usart_ucaRxBuf))
			{
				usart_ucaRxBuf[usart_usRxCount++] = usart_ucTemp;
				if(usart_usRxCount==4)
				{
				  if((usart_compare[0]==usart_ucaRxBuf[0])&&(usart_compare[1]==usart_ucaRxBuf[1]))
						usart_Rx_data_flag=1;
					else 
         {
				    usart_Rx_data_flag=0;  
				    memset( usart_ucaRxBuf, 0, sizeof(usart_ucaRxBuf));
				 }
				 }
	
				
				 
			}
			else
			{
				usart_usRxCount = 0;
			  memset( usart_ucaRxBuf, 0, sizeof(usart_ucaRxBuf));
			}
			
			/* ���������ַ�����Ϊ���յ�һ������ */
			if (usart_ucTemp == 0x0A && usart_Rx_data_flag)	/* �����ַ� */
			{		
				 usart_Tx_data_flag=1;
         usart_Rx_data_flag=0;
			   usart_usRxCount = 0;
				 UART1_SendString(usart_ucaRxBuf);
				 for(i=0;i<sizeof(usart_ucaRxBuf);i++)
				{
					recei_data[i]=usart_ucaRxBuf[i];
				}
				
        // Usart2_Send(usart_ucaRxBuf);
			   memset( usart_ucaRxBuf, 0, sizeof(usart_ucaRxBuf));
			}
		}		

	
	
    

//		if(usart_Rx_control_flag)
//		{
//			if (usart_usRxCount < sizeof(usart_ucaRxBuf))
//			{
//				usart_ucaRxBuf[usart_usRxCount++] = usart_ucTemp;
//				if(usart_usRxCount==4)
//				{
//				  if((usart_compare2[0]==usart_ucaRxBuf[0])&&(usart_compare2[1]==usart_ucaRxBuf[1])&&(usart_compare2[2]==usart_ucaRxBuf[2])&&(usart_compare2[3]==usart_ucaRxBuf[3]))
//						usart_Rx_control_flag=1;
//					else 
//					 {
//							usart_Rx_control_flag=0;  
//							memset( usart_ucaRxBuf, 0, sizeof(usart_ucaRxBuf));
//					 }
//				 }
//				if(usart_usRxCount>6)
//				{
//					usart_Rx_control_flag=0;
//				}
//		
//			}		
//				else
//				{
//					usart_usRxCount = 0;
//				}
//			
//			/* ���������ַ�����Ϊ���յ�һ������ */
//			if (usart_ucTemp == 0x0A && usart_Rx_control_flag)	/* �����ַ� */
//			{		
//				 //Usart1_Send(usart_ucaRxBuf);
//         usart_Rx_control_flag=0;
//			   usart_usRxCount = 0;
//			   memset( usart_ucaRxBuf, 0, sizeof(usart_ucaRxBuf));
//			}
//			
//		}

} 	

/*******************************************************************************
* ������  : USART1_Init_Config
* ����    : USART1��ʼ������
* ����    : bound��������(���ã�2400��4800��9600��19200��38400��115200��)
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
void USART2_Init_Config(u32 bound)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	/*ʹ��USART2����ʱ��*/  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	/*��λ����2*/
 	USART_DeInit(USART2);  
	
	/*USART2_GPIO��ʼ������*/ 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			//USART2_TXD(PA.2)     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//������������������Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);				//���ÿ⺯���е�GPIO��ʼ����������ʼ��USART1_TXD(PA.9)  
   
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				//USART2_RXD(PA.3)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);					//���ÿ⺯���е�GPIO��ʼ����������ʼ��USART1_RXD(PA.10)


   /*USART2 ��ʼ������*/
	USART_InitStructure.USART_BaudRate = bound;										//���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//����ģʽ����Ϊ�շ�ģʽ
  USART_Init(USART2, &USART_InitStructure);										//��ʼ������2

   /*Usart1 NVIC����*/
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;	//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							//����ָ���Ĳ�����ʼ��VIC�Ĵ��� 
	  
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//ʹ�ܴ���2�����ж�

  USART_Cmd(USART2, ENABLE);                    			//ʹ�ܴ��� 
	USART_ClearFlag(USART2, USART_FLAG_TC);					//���������ɱ�־
}
/*******************************************************************************
* ������  : UART2_SendString
* ����    : USART2�����ַ���
* ����    : *s�ַ���ָ��
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
void UART2_SendString(char* s)
{
	while(*s)//����ַ���������
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); 
		USART_SendData(USART2 ,*s++);//���͵�ǰ�ַ�
	}
}


