#include "ano_dt.h"
#include "usart.h"	 

void USART1_Send_Char(u8 c)
{   	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); //????,??????   
	USART_SendData(USART1,c);  
} 

//������Э�鷢������
void usart_send_ANO(u8 fun,u8*data,u8 len)
{
	u8 sum=0;
	u8 i;
	
	USART1_Send_Char(0Xaa); sum+=0Xaa;
	USART1_Send_Char(0Xaa); sum+=0Xaa;
	USART1_Send_Char(fun);  sum+=fun;
	USART1_Send_Char(len);  sum+=len;
	for(i=0;i<len;i++)
	{
	USART1_Send_Char(data[i]);	sum+=data[i];
	}
	USART1_Send_Char(sum);    	sum=0;
}
//���;Ÿ�ͨ��������
void ANO_send_data(short D1,short D2,short D3,short D4,short D5,short D6,short D7,short D8,short D9)
{
	u8 tbuf[18]; 
	tbuf[0]=(D1>>8)&0XFF;
	tbuf[1]=D1&0XFF;
	tbuf[2]=(D2>>8)&0XFF;
	tbuf[3]=D2&0XFF;
	tbuf[4]=(D3>>8)&0XFF;
	tbuf[5]=D3&0XFF; 
	tbuf[6]=(D4>>8)&0XFF;  
	tbuf[7]=D4&0XFF;
	tbuf[8]=(D5>>8)&0XFF;
	tbuf[9]=D5&0XFF;
	tbuf[10]=(D6>>8)&0XFF;
	tbuf[11]=D6&0XFF;	
	tbuf[12]=(D7>>8)&0XFF;
	tbuf[13]=D7&0XFF;
	tbuf[14]=(D8>>8)&0XFF;
	tbuf[15]=D8&0XFF;
	tbuf[16]=(D9>>8)&0XFF;
	tbuf[17]=D9&0XFF;

	usart_send_ANO(0XF1,tbuf,18);//?????,0XAF
}  
//����λ��������̬��
void ano_dt(short roll,short pitch,short yaw,int high)
{
	u8 tbuf[12]; 
	tbuf[0]=(roll>>8)&0XFF;
	tbuf[1]=roll&0XFF;
	tbuf[2]=(pitch>>8)&0XFF;
	tbuf[3]=pitch&0XFF;
	tbuf[4]=(yaw>>8)&0XFF;
	tbuf[5]=yaw&0XFF; 
	tbuf[6]=(high>>24)&0XFF;
	tbuf[7]=(high>>16)&0XFF;
	tbuf[8]=(high>>8)&0XFF;
	tbuf[9]=high&0XFF;
	tbuf[10]=1;
	tbuf[11]=1;

	usart_send_ANO(0X01,tbuf,12);//

}
