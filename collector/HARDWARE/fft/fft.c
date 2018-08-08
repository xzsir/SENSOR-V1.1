
#include "stm32f10x.h"
#include "bsp_adc.h"
#include "fft.h"
#include "stm32_dsp.h"
#include "table_fft.h"
#include <math.h>
#include "usart.h"

#define NPT 256
#define PI2 6.28318530717959
#define Fs 44800

long lBufInArray[NPT];
long lBufOutArray[NPT];
long lBufMagArray[NPT];
extern __IO uint16_t ADC_ConvertedValue;


/******************************************************************
��������:InitBufInArray()
��������:ģ��������ݣ����������а���3��Ƶ�����Ҳ�(350Hz��8400Hz��18725Hz)
����˵��:
��    ע:��lBufInArray�����У�ÿ�����ݵĸ�16λ�洢�������ݵ�ʵ����
          ��16λ�洢�������ݵ��鲿(����Ϊ0)
��������:����԰ ���ɵ�Ȼ��http://www.cnblogs.com/menlsh/��
*******************************************************************/
void InitBufInArray(void)
{
    unsigned short i;
    float fx;
    for(i=0; i<NPT; i++)
    {
        fx = 1500*ADC_ConvertedValue*3.3/4096;
        lBufInArray[i] = ((signed short)fx) << 16;
    }
}

/******************************************************************
��������:GetPowerMag()
��������:�������г����ֵ
����˵��:
������ע:�Ƚ�lBufOutArray�ֽ��ʵ��(X)���鲿(Y)��Ȼ������ֵ(sqrt(X*X+Y*Y)
��������:����԰ ���ɵ�Ȼ��http://www.cnblogs.com/menlsh/��
*******************************************************************/
void GetPowerMag(void)
{
    signed short lX,lY;
    float X,Y,Mag;
    unsigned short i;
    for(i=0; i<NPT/2; i++)
    { 
		  	//printf("ad is %ld\n",lBufInArray[i]);
        lX  = (lBufOutArray[i] << 16) >> 16;
        lY  = (lBufOutArray[i] >> 16);
        X = NPT * ((float)lX) / 32768;
        Y = NPT * ((float)lY) / 32768;
        Mag = sqrt(X * X + Y * Y) / NPT;
        if(i == 0)
            lBufMagArray[i] = (unsigned long)(Mag * 32768);
        else
            lBufMagArray[i] = (unsigned long)(Mag * 65536);

     }
}


int frequency(void)
{
	int i=0,n=0;
	long max=0;
	int fre=0; 
	
	InitBufInArray();
	cr4_fft_256_stm32(lBufOutArray, lBufInArray, NPT);
	GetPowerMag();
	max = lBufMagArray[7];
  n=7;
	for(i=7;i<80;i++)
	{

		if(max<lBufMagArray[i+1])
		{
		   max = lBufMagArray[i+1];
			 n=i;
		}	    
	}
  fre=n*186;

  return fre;	

}
