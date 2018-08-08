/***************************************
          ���ֵ�λ����������
****************************************/


#include "dtp.h"
#include "delay.h"



static u8 add=0;
/*************************
��������blank
�������ܣ��հ׺�������ʱ����
������������
��������ֵ����
**************************/
void blank(void)
{
}


/*************************
��������read_init
�������ܣ���ʼ����ȡ��Ƶ���������
������������
��������ֵ����
**************************/

void read_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

	  RCC_APB2PeriphClockCmd(DEV_CLK	, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
                       
	  GPIO_InitStructure.GPIO_Pin = read_pin;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//ģ����������
	  GPIO_Init(DEV_GPIO, &GPIO_InitStructure);	

    //GPIO_SetBits(DEV_GPIO, read_pin);

}



/*************************
��������dtp_init
�������ܣ���ʼ���������ţ���ǿ�ƽ���λ������
������������
��������ֵ����
**************************/
void dtp_init(void)
{   
	  u8 i=0;
	  GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(DEV_CLK	, ENABLE );	  //ʹ��ADC1ͨ��ʱ�ӡ�
	
	  
                       
	  GPIO_InitStructure.GPIO_Pin = INC_Pin|UD_Pin|CS_Pin;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//ģ����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
	  GPIO_Init(DEV_GPIO, &GPIO_InitStructure);	
	  INC_1;
	  CS_0;
	  UD_0;
	  
    blank();
	
		for(i=100;i>0;i--)
		{
			 INC_0;
       delay_ms(1);
			 INC_1;		
		}
		//INC(0);
		CS_1;
		
		read_init();
}





/*************************
��������dtp_plus
�������ܣ�����ָ�����裬�����λ����ֵ��=num*1010R
����������num
��������ֵ����
**************************/
void dtp_plus(u8 num)
{  
   int i;
	 CS_0;
	 UD_1;	
	 blank();
	 for(i=num;i>0;i--)
   {
			INC_0;
      delay_ms(1);
			INC_1;
		}
	// INC(0);
	 CS_1;
			
	 add = add + num; 
	
}





/*************************
��������dtp_minus
�������ܣ���Сָ�����裬��С��λ����ֵ��=num*1010R
����������num
��������ֵ����
**************************/
void dtp_minus(u8 num)
{
	 int i;
	 CS_0;
	 UD_0;	
	 blank();
	 for(i=num;i>0;i--)
	 {
			INC_0;
      delay_ms(1);
			INC_1;
		}
	 // INC(0);
		CS_1;
		add = add - num; 
  
}



/*************************
��������frequency
�������ܣ���Сָ�����裬��С��λ����ֵ��=num*1010R
����������num
��������ֵ��1��0 1��ʾ�����Ƶ�� 0��ʾ�������Ƶ��
**************************/
int frequency(u32* f)
{  
	 *(f+0) = (int)11000000/(add*1010);
	 if(GPIO_ReadInputDataBit(DEV_GPIO, read_pin)==0) return 1;
   else return 0;
}


