/***************
*文件名：main.c
*编辑者：夏志勇
*时间：2018-5-14
***************/


#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "gprs.h"
#include "usart2.h"
#include  "oled.h"
#include  "dev.h"
#include  "ds18b20.h"
#include  "mpu6050.h"
#include  "bsp_i2c.h"
#include  "ano_dt.h"
#include "data_handle.h"
#include "bsp_GeneralTIM.h" 
#include "fft.h"
#include "bsp_adc.h"
#include <string.h>



/****定义设备ID****/
const char ID1 = 100;
const char ID2 = 100;



char pre_pack[255];

/****定义60s计数参数****/
uint16_t timer_count=0;

/*****数据发送标志位******/
uint8_t  Tx_data_flag=0;

int main(void)
{
    short Accel[3];
    short accel[1];
	  short temp[1];
	  short fre[1]; 
	  char pack[255];
	  char i=0;

    delay_init();
    uart_init(9600);
	  usart2_init(9600);
	  /****设置1ms周期中断****/
	  GENERAL_TIMx_Configuration();
	  /****初始化mpu6050****/
	  i2c_CfgGpio(); 
		MPU6050_Init();
		/****MPU6050自检****/
		if(MPU6050ReadID() == 0)
		{	
		  	while(1);
		}
		
		/****初始化并自检温度传感器****/
		if(DS18B20_Init() == 1)
		{
        while(1);  
		}
		
		/****初始化并自检声音传感器****/	
		ADCx_Init();

	  while(1)
    {

			if(timer_count == 2000)
			{
					timer_count = 0;
					MPU6050ReadAcc(Accel);
					accel[0]=Accel[0]*196/32767;
					temp[0]= DS18B20_Get_Temp();
					fre[0] = frequency();
				
					//采集声音信息 
					//将数据打包，如果串口收到发送指令，将数据发送至中转器
					data_pack(temp,accel,fre,pack);
					for(i=0;i<sizeof(pack);i++)
					{
						pre_pack[i]= pack[i];
					
					}

		  }

		  if(Tx_data_flag==1)
		  { 
			  Usart2_Send(pre_pack);
				USART_SendData(USART2 ,0x0d);
				USART_SendData(USART2 ,0x0a);
				Tx_data_flag=0;
				memset( pre_pack, 0, sizeof(pre_pack));
			}
			
			memset( pack, 0, sizeof(pack));
			//delay_ms(1000);
	  }

} 
 



