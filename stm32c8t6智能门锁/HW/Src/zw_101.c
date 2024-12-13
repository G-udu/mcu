#include "stm32f1xx_hal.h"
#include "zw_101.h"
#include "uart_init.h"
#include "state.h"
#include "TIM_DSQIT.h"
#include "iic_init.h" 
#include "oled_init.h" 
/*ָ��ģ��ͨ����Ƭ������1��PA9 tx��PA10 rx���뵥Ƭ��ͨ��
	1.�Զ�ע��ģ�壺
	
	ָ���룺EF 01 FF FF FF FF 01 00 08 31 (2byte id��) ��1byte ¼���������2byte  ��������2byte SUMУ��ͣ�

	¼������̶�Ϊ2�� ��02���������̶�Ϊ��00 1D����LED ��ȡͼ��ɹ�����ע������У���Ҫ��ģ�鷵�عؼ����裬
																										������ ID �ţ�
																										ע��ʱ�����ָ�Ʋɼ������У�Ҫ����ָ�뿪���ܽ�����һ��ָ��ͼ��ɼ�

	����id1ָ����(17�ֽ�)�� EF 01 FF FF FF FF 01 00 08 31 00 01 02 00 1D 00 5A
													EF 01 FF FF FF FF 01 00 08 31 00 02 02 00 1D 00 5B
													EF 01 FF FF FF FF 01 00 08 31 00 03 02 00 1D 00 5C
													EF 01 FF FF FF FF 01 00 08 31 00 04 02 00 1D 00 5D
													EF 01 FF FF FF FF 01 00 08 31 00 05 02 00 1D 00 5E

*/



extern uint8_t registfinger_buff1[17] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x08,0x31,0x00,0x01,0x02,0x00,0x1D,0x00,0x5A};
uint8_t registfingerReturn_buff1[14];

	
	
extern uint8_t checkfinger_buff[17] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x08,0x32,0x02,0xFF,0xFF,0x00,0x01,0x02,0x3C};
uint8_t checkfingerRerurn_buff1[51] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};



extern uint8_t light_shank[16] =      {0xef,0x01,0xff,0xff,0xff,0xff,0x01,0x00,0x07,0x3c,0x02,0x04,0x04,0x02,0x00,0x50};

void zw_init(uint32_t baudrate)
{
	uart1_init(baudrate);
}


/* ��    ����ָ��ע�ắ��
 * ��    ������
 * �� �� ֵ��registfingerReturn_buff1[9]   0x00�ɹ���0x01ʧ��
 * ע�������
 */
uint8_t regist_finger()
{
	
	uint8_t returnstate = 0;
	HAL_UART_Transmit(&uart1,registfinger_buff1,17,HAL_MAX_DELAY);//���ڽṹ��ָ�룬�������ݰ�ָ�룬���ݰ���С����ʱʱ��
	
	HAL_UART_Receive(&uart1,registfingerReturn_buff1,14,HAL_MAX_DELAY);
	
	return registfingerReturn_buff1[9];
}




/* ��    ������ָ֤���Ƿ�ͨ��(��Ƭ����ָ��ģ�鷢����ָ֤������)��
 * ��    ������
 * �� �� ֵ�������2����ʾ�ɹ���13��ʾ����
 *
 * ע��������͵İ�����17���ֽ�
 *            Ӧ���1�����������֤�������ָ��ģ���ڵȴ�ָ��ʱ��Ӧ���Ϊ��
 *                                          EF 01 FF FF FF FF 07 00 08 00 00 00 FF 00 00 01 0E ȷ����00������00��ID��00FF
 * 																					��ʾָ��Ϸ�����֤�ɹ��������趨��ʱ���ڵȴ�ָ��¼����֤��
 * 
 * 
 */



uint8_t check_finger()
{
	uint16_t value = 0;
	uint8_t checkfingerRerurn_buff1[51] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	

	HAL_UART_Transmit(&uart1,checkfinger_buff,17,4000);//���ڽṹ��ָ�룬�������ݰ�ָ�룬���ݰ���С����ʱʱ��

	
	if(checkfingerRerurn_buff1[17] != 0xEF)//�˴���ʾ��ָ�Ʒ���ȥ��
	{

		HAL_UART_Receive(&uart1,checkfingerRerurn_buff1,51,5000);//����״̬��
		
		if(checkfingerRerurn_buff1[32] == 0x01 && checkfingerRerurn_buff1[33] == 0x35)
		{
			value = 1;                                                                   //��ʾ��ʱ��ָ�ƺܾ�ûʶ������� 
		}
		if(checkfingerRerurn_buff1[43] == 0x00 && checkfingerRerurn_buff1[44] == 0x05)
		{
			value = 2;                                                                  //��ʾָ�Ʊȶ���֤�ɹ�
		}
		if(checkfingerRerurn_buff1[43] == 0x09 && checkfingerRerurn_buff1[44] == 0x05)
		{
			value = 3;                                                                  //��ʾָ�Ʊȶ���֤ʧ��
		}
		else
		{
		value = 4;
		}
	}

	

	return value;
	
}

/* ��    ��������ָ��ģ���Ƿ�������������
 * ��    ����
 * �� �� ֵ��
 * ע����������������������˸һ�κ�
 */
void zw_101_test()
{
	HAL_UART_Transmit(&uart1,light_shank,16,HAL_MAX_DELAY);//���ڽṹ��ָ�룬�������ݰ�ָ�룬���ݰ���С����ʱʱ��
}


/* ��    ����ָ�ƽ����ж�
 * ��    ����
 * �� �� ֵ��
 * ע�����
 */


//void ZW_INIT_IT()
//{
//	__HAL_RCC_GPIOA_CLK_ENABLE();
//	
//	GPIO_InitTypeDef GPIO_InitTypeDefA1;
//	GPIO_InitTypeDefA1.Mode = GPIO_MODE_IT_FALLING;
//	GPIO_InitTypeDefA1.Pin = GPIO_PIN_1;
//	GPIO_InitTypeDefA1.PULL = GPIO_PULLUP;
//	
//	HAL_GPIO_Init(GPIOA,&GPIO_InitTypeDefA1);
//	
//	HAL_NVIC_SetPriority(EXTI1_IRQn,4,0);
//	HAL_NVIC_EnableIRQ(EXTI1_IRQn);

//}

//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//	uint32_t i;
//	switch(GPIO_Pin)
//	{
//		case	GPIO_PIN_1:
//									if(check_finger() == 1 || check_finger() == 2)
//									{
//										state = 0;
//									}
//									if(check_finger() == 3)
//									{
//										state = 4;
//									}							
//									break;
//	}
//}