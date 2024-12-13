#include "stm32f1xx_hal.h"
#include "duoji_SG90.h"

TIM_HandleTypeDef TIM_HandleSG90;         //ʱ����Ԫ�����ṹ��
TIM_OC_InitTypeDef TIM_OC_InitOC1;        //����ȽϽṹ��

void SG90_Control()
{
/*---------------------------------------------------------------------------------------------------------------
		   TIM_HandleTypeDef TIM_HandleSG90;         
             ʱ����Ԫ�����ṹ���Ա*/
	
	TIM_HandleSG90.Instance = TIM1;                                       //ָ����Ҫʹ�õĶ�ʱ���ļĴ�������ַ��ָ��
	TIM_HandleSG90.Channel = HAL_TIM_ACTIVE_CHANNEL_1;                    //��ǰ�����ͨ��
	TIM_HandleSG90.Init.CounterMode = TIM_COUNTERMODE_UP;                 //����ģʽ
	TIM_HandleSG90.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;           //ʱ�ӷ�Ƶ����         72mhz
	TIM_HandleSG90.Init.Prescaler  = 72-1;                              //Ԥ��Ƶֵ
	TIM_HandleSG90.Init.Period = 20000-1;                               //�Զ���װ��ֵ
	TIM_HandleSG90.Init.RepetitionCounter = 0;                            //�ظ�����ֵ
	TIM_HandleSG90.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;//�Զ�����ֵ
//DMA_HandleTypeDef  DMA�ṹ�壻 Lock ��������State ״̬��
	
	HAL_TIM_PWM_Init(&TIM_HandleSG90);
	
/*---------------------------------------------------------------------------------------------------------------
	     TIM_OC_InitTypeDef TIM_OC_InitOC1; 
             ����ȽϽṹ���Ա*/ 

	
	TIM_OC_InitOC1.OCMode = TIM_OCMODE_PWM1;                      //����Ƚϵ�pwmģʽ
	TIM_OC_InitOC1.Pulse = 0;                                     //ָ��Ҫ���ص�����ȽϼĴ���������ֵ
	TIM_OC_InitOC1.OCPolarity = TIM_OCPOLARITY_HIGH;              //ָ���������
	
//����δ�õ���Ա��OCNPolarity ����������ԣ� OCFastMode ����ģʽ״̬��OCIdleState����״̬�����pin״̬�� OCNIdleState����״̬�����pin״̬��
/*---------------------------------------------------------------------------------------------------------------*/
	
//	HAL_TIM_PWM_Init(&TIM_HandleSG90);
	HAL_TIM_PWM_ConfigChannel(&TIM_HandleSG90,&TIM_OC_InitOC1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&TIM_HandleSG90,TIM_CHANNEL_1);
}


void Servo_SetAngle(float Angle)
{
	__HAL_TIM_SET_COMPARE(&TIM_HandleSG90,TIM_CHANNEL_1,Angle / 180 * 2000 + 500);//����ռ�ձ�
}


void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim -> Instance == TIM1)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();               
		__HAL_RCC_TIM1_CLK_ENABLE();
		
		GPIO_InitTypeDef GPIO_InitPA8;

		GPIO_InitPA8.Mode = GPIO_MODE_AF_PP;
		GPIO_InitPA8.Pin = GPIO_PIN_8;
		GPIO_InitPA8.Speed = GPIO_SPEED_FREQ_MEDIUM;

		HAL_GPIO_Init(GPIOA,&GPIO_InitPA8);
		
	}

}