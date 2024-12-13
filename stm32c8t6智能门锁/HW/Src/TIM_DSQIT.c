#include "stm32f1xx_hal.h"
#include "TIM_DSQIT.h"
#include "OLED_init.h" 
#include "iic_init.h" 
//Ŀ�꣬����һ���ܵ���ʱ5��Ķ�ʱ���жϺ��������ز��ڴ˺����ʹ��TIM2��

/*ʵ�鱳�����Ѿ�ʹ����8MHZ���ⲿ����ʱ����Ϊ��Ƭ��ʱ�ӣ���SYSTEMCLOCK����Ϊ72MHZ�����
            ��APB1�����Ѿ��������ó���36MHZ����󣩣����Զ�ʱ��2��4��ʱ��Ƶ��Ϊ36MHZ����ʱ��1Ϊ�߼���ʱ����APB2ʱ���йء�
						*/




////	HAL_TIM_Base_Start_IT(&TIM2TypeDef);//�򿪶�ʱ��������Ϊ5s�ʹ����ж�
		
//		HAL_TIM_Base_Stop_IT(&TIM2TypeDef);//�رն�ʱ��		
		
TIM_HandleTypeDef TIM2TypeDef;

void TIM2_DSQ_IT_INIT()
{
	__HAL_RCC_TIM2_CLK_ENABLE();
	
	TIM2TypeDef.Instance = TIM2;
	TIM2TypeDef.Init.CounterMode = TIM_COUNTERMODE_UP;
	TIM2TypeDef.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;//�Ƿ����Զ���װ��(����һ����ɺ��������),���ﲻ��Ҫ����Ϊ����ֻ��Ҫ����һ�ξͺ�
	TIM2TypeDef.Init.Prescaler =7200;
	TIM2TypeDef.Init.Period = 30000;
	//TIM2TypeDef.Init.RepetitionCounter  = �ظ�����ֵ�����߼���ʱ���У�����Ҫ��
	TIM2TypeDef.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; //�˲�����Ϊ1�����ˡ�
	TIM2TypeDef.Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;//4��ͨ����ѡ�񣬣�������Ƚϲ�ʿ�����벶��ģʽ���ã���ͨ�жϲ���Ҫ��
	
	HAL_TIM_Base_Init(&TIM2TypeDef);

	HAL_NVIC_SetPriority(TIM2_IRQn,1,0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	
	__HAL_TIM_CLEAR_FLAG(&TIM2TypeDef,TIM_FLAG_UPDATE);
	
//		HAL_TIM_Base_Start_IT(&TIM2TypeDef);���ڴ˴���
	
	
}




void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim -> Instance == TIM2)
	{

	}

}
