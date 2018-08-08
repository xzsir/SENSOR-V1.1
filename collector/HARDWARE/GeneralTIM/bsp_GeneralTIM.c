
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "bsp_GeneralTIM.h" 

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ͨ�ö�ʱ�� TIMx,x[2,3,4,5]�ж����ȼ�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void GENERAL_TIMx_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
  /* �����ж���Ϊ0 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
  /* �����ж���Դ */
  NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQ;	
  /* ���������ȼ�Ϊ 0 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
  /* ������ռ���ȼ�Ϊ3 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	 /*��ʱ��ʹ�� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * �ж�����Ϊ = 1/(72MHZ /72) * 1000 = 1ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> �ж� ��TIMxCNT����Ϊ0���¼��� 
 */
/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{  TIM_Prescaler         ����
 *	TIM_CounterMode			   TIMx,x[6,7]û�У���������
 *  TIM_Period             ����
 *  TIM_ClockDivision      TIMx,x[6,7]û�У���������
 *  TIM_RepetitionCounter  TIMx,x[1,8,15,16,17]����
 *}TIM_TimeBaseInitTypeDef; 
 *-----------------------------------------------------------------------------
 */
void GENERAL_TIMx_Configuration(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  /* ����TIMx_CLK,x[2,3,4,5],���ڲ�ʱ��CK_INT=72M */
  GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK, ENABLE);
  
  /* ͨ�ö�ʱ�� TIMx,x[2,3,4,5]�ж����ȼ����� */
  GENERAL_TIMx_NVIC_Configuration();
  
  /* �Զ���װ�ؼĴ����ܵ�ֵ(����ֵ) */
  TIM_TimeBaseStructure.TIM_Period=1000;
  /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж�
     ʱ��Ԥ��Ƶ��Ϊ71����������������ʱ��CK_CNT = CK_INT / (71+1)=1M */
  TIM_TimeBaseStructure.TIM_Prescaler= 71;
  /* ʱ�ӷ�Ƶ���� ��û���õ������ù� */
  //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  /* ����������ģʽ��������ʱ��TIM6��TIM7ֻ�����ϼ�����û�м���ģʽ�����ã����ù� */
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
  /* �ظ���������ֵ��ͨ�ö�ʱ��û�У����ù� */
  //TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
  /* ��ʼ����ʱ��TIMx, x[2,3,4,5] */
  TIM_TimeBaseInit(GENERAL_TIMx, &TIM_TimeBaseStructure);

  /* ����������жϱ�־λ */
  TIM_ClearITPendingBit(GENERAL_TIMx, TIM_IT_Update);
  /* �����������ж� */
  TIM_ITConfig(GENERAL_TIMx,TIM_IT_Update,ENABLE);

  /* ʹ�ܼ�����: */
  TIM_Cmd(GENERAL_TIMx, ENABLE); 
}
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
