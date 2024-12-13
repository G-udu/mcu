#include "stm32f1xx_hal.h"
#include "Rcc_HSE.h"
#include "stdio.h"
#include "iic_init.h" 
#include "oled_init.h" 
#include "ttp229_keyboard.h"
#include "at24c02.h" 
#include "state.h"
#include "duoji_SG90.h"
#include "uart_init.h"
#include "zw_101.h"
#include "TIM_DSQIT.h"
//#include "stm32f1xx_it.h" 
/*����ʽ����������
����ttp229bsfоƬ��ģ���Ѿ���ƺã�ʹ�ô���ͨ�ţ���16�����ã�
ʹ��PB12��Ϊ��sclʱ���߽ӿڣ�PB13��Ϊsdo�����߽ӿڣ�ûʹ��uart3���ܣ��˾������Ǽ򵥵�scl���sda�ĵ���Э�飩
*/

/*at24c02�洢��
ʹ��iicͨ�š���ʵ��ʹ��iic2��pb10���Ӵ洢����scl��pb11���Ӵ洢����sda
*/

/*���
��ʵ��ͨ��tim��ʱ����oc����ȽϹ��ܣ�ͨ��pwm������ƶ����ת���Ƕ�
f103c8ϵ�����е�������Ʒ������ֻ��һ��PWM��ʱ�������� 3�����ڣ�������ͨ��ʱ��������iic������SPI��
��ʵ��ʹ�ø߼���ʱ��TIM1��ͨ��1���ж�����ƣ���Ƭ����PinA8���ţ�
*/

/*ָ��ģ��
����ͨ�ţ�ʹ��uart1��pa9����ָ��ģ���rx��pa10����ָ��ģ���tx��ָ��ģ��Ĭ�ϲ�����Ϊ57600bps
���߽�vcc������Ϊuart_tx,�ӵ�Ƭ�����ڵ�rx������Ϊuart_rx,�ӵ�Ƭ�����ڵ�tx�����߽�GND�����������߲��ӡ�

�����޸�ָ��ģ��������ý��ߣ�ָ��ģ���tx�Ӵ��ڵ�rx��ģ���rx�Ӵ��ڵ�tx��������3.3v��vcc�̽ӣ�Ȼ��GND�ӵ�Ƭ��������ָ��ģ��vcc�ӵ�Ƭ��������GND�ӵ�Ƭ������

��ָ��ģ�鴮��ͨ��У���룺���˰�ͷ��оƬ��ַ��������Ӿ���У����
*/
//���ݼ��̵ĵȴ������ɿ���Ҫ˼����Ŀǰ����ʱ����300ms��֤300ms��ֻ����һ��

//��Ҫ��zwģ�����ָ֤�ƺ��������һ����ʱ���ļ�ʱ���ܣ���Ϊ�����ָ��Ű��뻹������ȥ���ͻῨ��������Ҫ��ʱ����ǰ����

void main()
{
	HAL_Init();
	Rcc_HSE();
	at24c02_init();      //�洢��iic��ʼ��
	OLED_Init();         //oled��ʾ����ʼ��
	keyboard_gpio_init();//����ͨ�ż��̳�ʼ��
//	TIM2_DSQ_IT_INIT();//��ʱ��2���ó�ʼ��
//	SG90_Control();//������Ƴ�ʼ��
//	Servo_SetAngle(0);
	zw_init(57600);//uart1_init(57600);
	
	
	while(1)
	{
	State_Door();
	}

}
