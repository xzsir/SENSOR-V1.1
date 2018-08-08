 /*******************
*�ļ����֣�gprs.c
*�ļ�������GPRS���ӷ��������������ݵ�������
*�༭ʱ�䣺2018-5-14
*******************/


#include "gprs.h"
#include "stm32f10x.h"
#include "delay.h"
#include "stdio.h"
#include "usart.h"

void GPRS_Init(void)
{
	printf("AT\r\n");//����
	delay_ms(1000);  //ģ����Ӧʱ��
	
	printf("AT&F\r\n");//�ָ��������� -- ������9600
	delay_ms(1000);
	
	printf("AT+CGCLASS=\"B\"\r\n");//�����ƶ�̨���
	delay_ms(1000);
	
	//�����ƶ���������
	printf("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");
	delay_ms(1000);
	//ѡ��GPRS��Ϊ�������
	printf("AT+CGATT=1\r\n");
	delay_ms(1000);
	
	//���ñ��ص�TCP�˿ں�
	printf("AT+CLPORT=\"TCP\",\"2000\"\r\n");
	delay_ms(1000);
}

/*
ipaddr -- ip��ַ
ipport -- IP�Ķ˿ں�
id     -- ���
temperature -- �¶�ֵ
hudimity -- ʪ��ֵ
*/
//GPRS_LinkAndSend("123.207.17.225","5555",25,t,h);
void GPRS_LinkAndSend(u8 *ipaddr,u8 *ipport,u8 id,u8 temperature,u8 humidity)
{
	//����TCP/IP����
	printf("AT+CIPSTART=\"TCP\",\"%s\",\"%s\"\r\n",ipaddr,ipport);
	delay_ms(3000);
	
	//���ͷ�������ָ��
	printf("AT+CIPSEND\r\n");
	delay_ms(5000);
	//�������ݣ�Э���ʽ # �豸�� �¶�ֵ ʪ��ֵ
	printf("#");
	printf("%c",id);
	printf("%c",temperature);
	printf("%c",humidity);
	printf("%c",0x1a);
	delay_ms(2000);
	//�ر�TCP/IP����
	printf("AT+CIPCLOSE=1\r\n");
	delay_ms(1000);
	//�ر��ƶ�����
	printf("AT+CIPSHUT\r\n");
	delay_ms(2000);
}


//����ʱ��GPRS���Ӻ���
//������ִ��ʱ�����κ�delay����
//���ⲿ�ṩʱ����
//�ڲ�ֻ�Ǽ���ִ�в�ͬ��GSM����
//TTL��ƽ��GSMģ���TX�ӿ������RX��GSMģ���RX�ӿ������TX
void GPRS_Connect (u8 *ipaddr,u8 *ipport,u8 id,u8 temperature,u8 humidity)
{
	static u32 i = 0, State = 0, StateDelay = 2;
	i++;
	
	//ÿ����5�μ���ִ��һ��GSM����
	if (i == StateDelay)
	{
		switch (State)
		{
			case 0:
				printf("AT\r\n");//����
				StateDelay ++;
				break;
			case 1:
				printf("AT&F\r\n");//�ָ��������� -- ������9600
				StateDelay ++;
				break;
			case 2:
				printf("AT+CGCLASS=\"B\"\r\n");//�����ƶ�̨���
				StateDelay ++;
				break;
			case 3:
				printf("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");//�����ƶ���������
				StateDelay ++;
				break;
			case 4:
				printf("AT+CGATT=1\r\n");//ѡ��GPRS��Ϊ�������
				StateDelay ++;
				break;
			case 5:
				printf("AT+CLPORT=\"TCP\",\"2000\"\r\n");//���ñ��ص�TCP�˿ں�
				StateDelay ++;
				break;
			case 6:
				printf("AT+CIPSTART=\"TCP\",\"%s\",\"%s\"\r\n",ipaddr,ipport);//����TCP/IP����
				StateDelay += 3;
				break;
			case 7:
				printf("AT+CIPSEND\r\n");//���ͷ�������ָ��
				StateDelay += 5;
				break;
			case 8:
				//�������ݣ�Э���ʽ # �豸�� �¶�ֵ ʪ��ֵ
				printf("#");
				printf("%c",id);
				printf("%c",temperature);
				printf("%c",humidity);
				printf("%c",0x1a);
				StateDelay += 2;
				break;
			case 9:
				printf("AT+CIPCLOSE=1\r\n");//�ر�TCP/IP����
				StateDelay ++;
				break;
			case 10:
				printf("AT+CIPSHUT\r\n");//�ر��ƶ�����
				StateDelay += 2;
				break;
			default:
				break;
		}
		State ++;
		if (State > 10) State = 6;//��ʼ����ص�GSM������ظ����ͣ�ֻ�ڿ�������һ��
	}
}

void send_message(int num)
{
	printf("AT+CSCS=\"GSM\"\r\n");
	delay_ms(500);
	printf("AT+CMGF=1\r\n");
	delay_ms(500);	
	printf("AT+CMGS=\"15001923517\"\r\n");
	delay_ms(500);
	if(num==0){printf("Temperature Warning!\x1A\r\n");}
	if(num==1){printf("Invade Warning!\x1A\r\n");}
	if(num==2){printf("Gas Warning!\x1A\r\n");}
	delay_ms(500);
}

void call(void)
{
	//while(1)
	{
	printf("AT\r\n");
	delay_ms(500);
	printf("ATD1xxxxxxxxxx;\r\n");
	delay_ms(500);
	}
}





