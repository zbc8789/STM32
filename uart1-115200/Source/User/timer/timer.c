#include "timer.h"

extern vu8 Timer0_start;
extern u8 shijian;
extern u8 Times;
extern u16 Heartbeat;
extern vu8 Heart_beat;		//��������֡��־λ

/*******************************************************************************
* ������  : Timer2_Init_Config
* ����    : Timer2��ʼ������
* ����    : ��
* ���    : ��
* ����    : �� 
* ˵��    : 1s��ʱ
*******************************************************************************/
void Timer2_Init_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		//ʹ��Timer2ʱ��
	
	TIM_TimeBaseStructure.TIM_Period = 9999;					//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ(������10000Ϊ1s)
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;					//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ(10KHz�ļ���Ƶ��)
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//����ʱ�ӷָ�:TDTS = TIM_CKD_DIV1
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);				//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	/*�ж����ȼ�NVIC����*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;				//TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//�����ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ��IRQͨ��
	NVIC_Init(&NVIC_InitStructure); 							//��ʼ��NVIC�Ĵ���
	 
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE ); 				//ʹ��TIM2ָ�����ж�
	
	TIM_Cmd(TIM2, ENABLE);  									//ʹ��TIMx����
}	 


/*******************************************************************************
* ������  : TIM2_IRQHandler
* ����    : ��ʱ��2�ж϶Ϸ�����
* ����    : ��
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
void TIM2_IRQHandler(void)   //TIM3�ж�
{
	static u8 flag =1;

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		
		if(Timer0_start)
		Times++;
		if(Times > shijian)
		{
			Timer0_start = 0;
			Times = 0;
		}
		
		Heartbeat++;
		if(Heartbeat>9)//ÿ10�뷢������֡
		{
			Heartbeat=0;
			Heart_beat=1;
		}
	}	
}



