#include "stm32f1xx_hal.h"
#include "uart_init.h"
UART_HandleTypeDef uart1;


void uart1_init(uint32_t baudrate)//PA9 USART1_tx ;PA10 USART1_rx
{
	uart1.Instance = USART1;          //ѡ���õĴ��ڵĻ���ַ
	uart1.Init.BaudRate = baudrate;    //Լ���Ĳ�����
	uart1.Init.WordLength = UART_WORDLENGTH_8B;//һ����֡8�ֽ�
	uart1.Init.Mode = UART_MODE_TX_RX;//���ͺͽ���ģʽ
	uart1.Init.Parity = UART_PARITY_NONE;//����ҪУ��λ
	uart1.Init.StopBits = UART_STOPBITS_1;//һ��ֹͣλ
	uart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;//Ӳ�������ƹر�
	uart1.Init.OverSampling = UART_OVERSAMPLING_16;//�˴����֣�������f100ϵ����Ҫ�Ķ�
	
	HAL_UART_Init(&uart1);

}


void uart2_init()//PA2 USART2_tx ;PA3 USART2_rx
{


}



void uart3_init()//PB10 USART3_tx ;PB11 USART3_rx
{


}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIO_InitTypeDefA9;//GPIOA9A10�ṹ��
	GPIO_InitTypeDef GPIO_InitTypeDefA10;

	if(huart->Instance == USART1)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_USART1_CLK_ENABLE();
		
		GPIO_InitTypeDefA9.Mode = GPIO_MODE_AF_PP;
		GPIO_InitTypeDefA9.Pin = GPIO_PIN_9;
		GPIO_InitTypeDefA9.Speed = GPIO_SPEED_FREQ_MEDIUM;

		GPIO_InitTypeDefA10.Mode = GPIO_MODE_AF_INPUT;
		GPIO_InitTypeDefA10.Pin = GPIO_PIN_10;
		GPIO_InitTypeDefA10.Pull = GPIO_NOPULL;
		
		HAL_GPIO_Init(GPIOA,&GPIO_InitTypeDefA9);
		HAL_GPIO_Init(GPIOA,&GPIO_InitTypeDefA10);
		
	}
}