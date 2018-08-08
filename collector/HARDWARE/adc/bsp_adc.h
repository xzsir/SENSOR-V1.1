#ifndef __ADC_H__
#define	__ADC_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include <stm32f10x.h>

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
// ע�⣺����ADC�ɼ���IO����û�и��ã�����ɼ���ѹ����Ӱ��
/********************ADC����ͨ�������ţ�����**************************/
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd

#define    ADCx                          ADC1
#define    ADC_DMAx_CLK                  RCC_AHBPeriph_DMA1
#define    ADC_CLK                       RCC_APB2Periph_ADC1
#define    ADC_DMA_CHANNEL               DMA1_Channel1

//#define    ADCx                          ADC3
//#define    ADC_DMAx_CLK                  RCC_AHBPeriph_DMA2
//#define    ADC_CLK                       RCC_APB2Periph_ADC3
//#define    ADC_DMA_CHANNEL               DMA2_Channel5

#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOA  
#define    ADC_PORT                      GPIOA

//#define    ADC_PIN                       GPIO_Pin_0         // ���������ؾ��ܿɵ�����(�����ñ)
//#define    ADC_CHANNEL                   ADC_Channel_10     // ���������ؾ��ܿɵ�����(�����ñ)
#define    ADC_PIN                       GPIO_Pin_7        // ���������ع�������(�����ñ)
#define    ADC_CHANNEL                   ADC_Channel_7  // ���������ع�������(�����ñ)

/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
void ADCx_Init(void);

#endif /* __ADC_H__ */

