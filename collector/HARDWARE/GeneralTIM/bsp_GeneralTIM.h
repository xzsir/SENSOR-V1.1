#ifndef __GENERAL_TIM_H__
#define __GENERAL_TIM_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include <stm32f10x.h>

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
/********************ͨ�ö�ʱ��TIM�������壬ֻ��TIM2 & TIM3 & TIM4 & TIM5************/
#define GENERAL_TIMx                 TIM2
#define GENERAL_TIM_APBxClock_FUN    RCC_APB1PeriphClockCmd
#define GENERAL_TIM_CLK              RCC_APB1Periph_TIM2
#define GENERAL_TIM_IRQ              TIM2_IRQn
#define GENERAL_TIM_INT_FUN          TIM2_IRQHandler

//#define GENERAL_TIMx                 TIM3
//#define GENERAL_TIM_APBxClock_FUN    RCC_APB1PeriphClockCmd
//#define GENERAL_TIM_CLK              RCC_APB1Periph_TIM3
//#define GENERAL_TIM_IRQ              TIM3_IRQn
//#define GENERAL_TIM_INT_FUN          TIM3_IRQHandler

//#define GENERAL_TIMx                 TIM4
//#define GENERAL_TIM_APBxClock_FUN    RCC_APB1PeriphClockCmd
//#define GENERAL_TIM_CLK              RCC_APB1Periph_TIM4
//#define GENERAL_TIM_IRQ              TIM4_IRQn
//#define GENERAL_TIM_INT_FUN          TIM4_IRQHandler

//#define GENERAL_TIMx                 TIM5
//#define GENERAL_TIM_APBxClock_FUN    RCC_APB1PeriphClockCmd
//#define GENERAL_TIM_CLK              RCC_APB1Periph_TIM5
//#define GENERAL_TIM_IRQ              TIM5_IRQn
//#define GENERAL_TIM_INT_FUN          TIM5_IRQHandler

/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
void GENERAL_TIMx_Configuration(void);

#endif	/* __GENERAL_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
