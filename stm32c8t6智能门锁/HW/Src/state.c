#include "stm32f1xx_hal.h"
#include "Rcc_HSE.h"
#include "stdio.h"
#include "iic_init.h" 
#include "oled_init.h" 
#include "ttp229_keyboard.h"
#include "at24c02.h" 
#include "duoji_SG90.h" 
#include "state.h"
#include "zw_101.h"
#include "uart_init.h"
#include "TIM_DSQIT.h"
/*
״̬���ƣ�
1.��һ���ϵ磬��ʼ������1234
2.��¼����ʱ����ֱ���������룬Ҳ����ʱ���ַ���ָ��ʶ�����ϣ�����������ֻ�����λ��ᣬָ�ƴ�������ν
3.ϵͳ����Ҫ������������ܽ��룬�����޸ĵ�¼����͹���ָ��.

3.ϵͳ->����ָ�ƣ����ָ�ƣ�ɾ��ָ��
*/
/*--------------------------------------------------------------------------------------------------------------------*/

static uint8_t state;  // 0���ϵ绽�ѵĳ�ʼ״̬����״̬����ѡ���¼��ϵͳѡ�
                   // 1����ʾ�����˵�¼״̬
                   // 2����ʾ������ϵͳ�޸�״̬
                   // 3: ��ʾ������ȷ�ˣ�����
                   // 4: ��ʾ�����޸������״̬
									 // 5����ʾ�ܽ����޸�ָ��״̬
									 // 6: ��ʾ�����ָ��
									 // 7: ��ʾɾ��ָ��
									 //
									 //
static uint8_t OLEDClear_State = 0;          //������ֹ�ظ���������
static uint8_t num = 0;                 //��ʾ��������ʱ���������������
uint8_t readkey;                     //������ȡ������ֵ
uint16_t value;                       //������LOGIN�����е�����������ĸ����ֱ��һ��ʮ���Ƶ�4λ��  ,Ҫע�ⷶΧ������uint16_t 
uint8_t Value[4] = {0};       // LOGIN()������������ʱ�洢��������������
static uint8_t state0 = 0;//��״̬0��ENTER_STATE()�����У������뿪��while�����ı�־��
static uint8_t state2 = 0;//��״̬2��SYSTEM()�����У������뿪��while�����ı�־��
static uint8_t state5 = 0;//��״̬5��SET_Pringer()�����У������뿪��while�����ı�־��
uint8_t state1_2;//����ENTER_STATE()������SYSTEM()����������һ����־��

static uint8_t num_N = 0;                 //��ʾ����������ʱ���������������
uint16_t value_N;               //������SYSTEM_SET_PASSWORD()�����е�����������ĸ����ֱ��һ��ʮ���Ƶ�4λ��  ,Ҫע�ⷶΧ������uint16_t 
uint8_t Value_N[4] = {0};       // SYSTEM_SET_PASSWORD()������������ʱ�洢��������������




			uint8_t x = 0;
/* ��    ������¼��ʼ����.
 * ��    ����
 * �� �� ֵ��
 * ע������� state ��̬״̬�� state ����0ʱ 
 *					�˺�����Ҫ����ʾ����ѡ���¼��ϵͳ�������ܹ�ѡ��ͬʱ����state״̬��
 *             
 *
 */
void ENTER_STATE()
{
	value_N = 0;value = 0;
	state2 = 0;
	
	while(state == 0)
	{
		OLED_Clear();   
	
		OLEDClear_State = 3;
	
		while(state0 == 0)                 //ֻҪ״̬��state2����0���Ͳ����뿪���while�������޸�state2��ֵ�����뿪��whileѭ��
		{
			readkey = TTP229_ReadKey();
			HAL_Delay(300);                            //!!!!!!!!!!!!!!!!!!!!!!!!!�ȴ������ɿ�����Ҫ������
		
			if(state1_2 % 2 == 0)              //���state1��2ȡ�����0��
			{
				if(OLEDClear_State != 0)      //״̬������ִֻ��һ�������Ĳ�������Ȼ��whileѭ�������һֱ������˸
				{
					OLED_Clear();                   
					OLEDClear_State = 0;
					OLED_ShowString(2, 6, "log in");
					state = 1;   
				}
                                 //state״̬���ı䣬�����޷�����whileѭ��
			}
		
			else if(state1_2 % 2 == 1)                        //���state1��2ȡ�����1��
			{
				if(OLEDClear_State != 1)
				{
					OLED_Clear();                    
					OLEDClear_State = 1;
					OLED_ShowString(2, 6, "system");
					state = 2;
				}

			}
		
			if(readkey == 67){state1_2++;}              //����c����,state1��״̬�ı�,��Ļ�ֱ���ʾlogin��systemѡ��
			else if(readkey == 65){state0 = 1;}       //����A����,state2��״̬�ı�,�����뿪���whileѭ�����ҽ�����һ��״̬
	}
	}

}


/* ��    ������¼����
 * ��    ����
 * �� �� ֵ��
 * ע���������������state״̬��Ϊ1���˳�����state״̬��Ϊ3
 */

void LOGIN()
{
while(state == 1)//���ܿ�״̬������1ʱ��һֱѭ��ִ�д�����
{
	
			OLED_Clear();				
			OLED_ShowString(1,5,"PUT FINGER");


			check_finger();		//�������ָ�Ʒ���ȥ��������������ֱ�ӿ���
			uint8_t fg = check_finger();
			switch(fg)
			{			
			case 2:
				OLED_Clear();				
				OLED_ShowString(2,5,"SUCCESS");
				HAL_Delay(2000);
				//������
				state = 0;
				num = 5;
				break;

					
			case 3:
				OLED_Clear();				
				OLED_ShowString(2,5,"ERROR");
				HAL_Delay(1000);
				num = 0;
				state = 0;
				break;
			
			case 4:
				OLED_Clear();				
				OLED_ShowString(2,5,"TIMEOUT");
				HAL_Delay(1000);
				num = 5;
				state = 0;
			}
	                                        

	while (num < 4) 
	{
		
		if(OLEDClear_State != 1)
		{
			OLED_Clear();
			OLEDClear_State = 1;
			
			OLED_ShowString(1,1,"enter");
			OLED_ShowString(2,1,"password");
		}
		
    readkey = TTP229_ReadKey(); // ���ð�����ȡ����

		if (readkey != 0) //���֮ǰ��ȡ�м�ֵ 
		{
			Value[num] = readkey;   // �洢����ֵ��������һ��λ��		
			OLED_ShowNum(4,num+1,Value[num],1);
			
			HAL_Delay(300);         // ��ʱ300ms��ֹ�ظ�����		
			num++;                  //num++,׼����һ������
						
			if(readkey == 66)//��Bȫѡɾ��,���һص�num = 0,��������
			{
				num = 0;
				Value[0] = 0;Value[1] = 0;Value[2] = 0;Value[3] = 0;
				OLED_ShowNum(4,1,0,4);
			}		
		}

	}		
	
	
	
	while(num == 4)
	{
		if(OLEDClear_State != 2)
		{
			OLED_Clear();
			OLED_ShowString(2,4,"enter by A");
			OLEDClear_State = 2;                       //        ��ֹ�ظ�����������˸
			
			value = Value[0] * 1000 + Value[1] * 100 + Value[2] * 10 + Value[3];//�����������������һ��ʮ���Ƶ���λ��������ȶ�
		}
		
	
		if(TTP229_ReadKey() == 65 && value !=  at24c02_readpage(0xA0))//�������������ʱ
		{
			if(OLEDClear_State != 3)
			{
				OLED_Clear();

				OLED_ShowNum(1,1,value,5);

				OLED_ShowString(2,5,"ERROR");
				OLED_ShowString(3,5,"try again");
				HAL_Delay(1000);
				OLEDClear_State = 3;//��ʾִֻ��һ�Σ���ֹ�ظ�ִ��OLED_Clear������Ļ��˸
				num = 0;            //��num = 1���ص��˺�����һ��whileѭ��������������
			}
		}
		if(TTP229_ReadKey() == 65 && value ==  at24c02_readpage(0xA0))//�������A�ż������������洢������һ��
		{
			
			if(OLEDClear_State != 4)
			{
			OLED_Clear();	
			OLED_ShowString(2,3,"Deblocking");
			OLED_ShowString(3,5,"Success!");
			OLEDClear_State = 4;//��ʾִֻ��һ�Σ���ֹ�ظ�ִ��OLED_Clear������Ļ��˸
			HAL_Delay(1000);
			}
			num = 5;               //������ȷ�ˣ���num����5����ѭ��
			state = 3;             //������ȷ�ˣ��޸�״̬��
			HAL_Delay(1000);
			state = 0;
		}	
	}			
}
}	


/* ��    ����ϵͳ���ú���
 * ��    ����
 * �� �� ֵ��
 * ע����������������ܿ�״̬��stateΪ2�� �뿪�������ܿ�״̬��stateΪ0ʱ���������˵�
 */

void SYSTEM()
{
	while(state == 2)
	{
		state1_2 = 2;//�Ժ�������2ȡ�࣬�ж�0��1�����������޸��������ָ�Ƶ�״̬
		uint8_t OLEDClear_State1 = 3;
	
		while(state2 == 0)                 //ֻҪ״̬��state3����0���Ͳ����뿪���while�������޸�state3��ֵ�����뿪��whileѭ��
		{
			readkey = TTP229_ReadKey();
			HAL_Delay(300);                           
		
			if(state1_2 % 2 == 0)              //���state1��2ȡ�����0��
			{
				if(OLEDClear_State != 0)      //״̬������ִֻ��һ�������Ĳ�������Ȼ��whileѭ�������һֱ������˸
				{
					OLED_Clear();  
					OLED_ShowString(1, 1, "SET PASSWORD");
					OLED_ShowString(2, 1, "SET FINGER");
					OLEDClear_State = 0;
				}
				OLED_ShowString(1, 14, "<-");
				state = 4;                                    //state״̬���ı䣬�����޷�����whileѭ��
			}
			
			else if(state1_2 % 2 == 1)                        //���state1��2ȡ�����1��
			{
				if(OLEDClear_State != 1)
				{
					OLED_Clear();  
					OLED_ShowString(1, 1, "SET PASSWORD");
					OLED_ShowString(2, 1, "SET FINGER");					
					OLEDClear_State = 1;
				}
				OLED_ShowString(2, 14, "<-");
				state = 5;
			}
		
			if(readkey == 67){state1_2++;}              //����c����,state1��״̬�ı�,��Ļ�ֱ���ʾlogin��systemѡ��
			else if(readkey == 65){state2 = 1;}       //����A����,state3��״̬�ı�,�����뿪���whileѭ�����ҽ�����һ��״̬
	}
	}   
}		


/* ��    ����ϵͳ�����������޸ĺ���
 * ��    ����
 * �� �� ֵ��
 * ע�����state����4ʱ���룬����0ʱ��ʾ������ɷ������˵�
 */
void SYSTEM_SET_PASSWORD()
{
	while(state == 4)
	{
		OLED_Clear();			

		while (num_N < 4) 
		{
			if(OLEDClear_State != 7)
			{
				OLED_Clear();
				OLEDClear_State = 7;
				OLED_ShowString(1,1,"new password");
			}
			readkey = TTP229_ReadKey(); // ���ð�����ȡ����
			if (readkey != 0) //���֮ǰ��ȡ�м�ֵ 
			{
				Value_N[num_N] = readkey;   // �洢����ֵ��������һ��λ��		
				OLED_ShowNum(4,num_N+1,Value_N[num_N],1);
			
				HAL_Delay(300);         // ��ʱ300ms��ֹ�ظ�����		
				num_N++;                  //num++,׼����һ������
			}
		}	
	
		while(num_N == 4)
		{
			if(OLEDClear_State != 5)
			{
				OLED_Clear();
				OLED_ShowString(2,4,"ensure by A");
				OLEDClear_State = 5;                       //        ��ֹ�ظ�����������˸
			
				value_N = Value_N[0] * 1000 + Value_N[1] * 100 + Value_N[2] * 10 + Value_N[3];//�����������������һ��ʮ���Ƶ���λ��������ȶ�
			}
			if(TTP229_ReadKey() == 65)//�������Aȷ��
			{
				if(OLEDClear_State != 6)
				{
					OLED_Clear();
					OLED_ShowString(2,4,"SUCCESS");
					OLEDClear_State = 6;                       //        ��ֹ�ظ�����������˸
					at24c02_writepage(0xA0,value_N);	
					HAL_Delay(1000); 
				}	
				
				state = 0;
				num_N = 5;
			}
		}
	}	
}

/* ��    �����޸�ָ�ƿ��ƺ�������ӣ�ɾ����
 * ��    ����
 * �� �� ֵ��
 * ע���������״̬��state����5ʱ����˺�������״̬������0ʱ���������˵�
 */

void SYSTEM_SET_FINGER()
{
	while(state == 5)
	{
		state1_2 = 2;//�Ժ�������2ȡ�࣬�ж�0��1�����������޸��������ָ�Ƶ�״̬
		OLEDClear_State = 3;
		
		while(state5 == 0)                 //ֻҪ״̬��state3����0���Ͳ����뿪���while�������޸�state3��ֵ�����뿪��whileѭ��
		{
			readkey = TTP229_ReadKey();
			HAL_Delay(300);                           
	
			if(state1_2 % 2 == 0)              //���state1��2ȡ�����0��
			{
				if(OLEDClear_State != 0)      //״̬������ִֻ��һ�������Ĳ�������Ȼ��whileѭ�������һֱ������˸
				{
					OLED_Clear();  
					OLED_ShowString(1, 1, "Add Finger");
					OLED_ShowString(2, 1, "Del Finger");
					OLEDClear_State = 0;
				}
				OLED_ShowString(1, 14, "<-");
				state = 6;                                    //state״̬���ı䣬�����޷�����whileѭ��
			}
			
			else if(state1_2 % 2 == 1)                        //���state1��2ȡ�����1��
			{
				if(OLEDClear_State != 1)
				{
					OLED_Clear();  
					OLED_ShowString(1, 1, "ADD Finger");
					OLED_ShowString(2, 1, "DEL Finger");			
					OLEDClear_State = 1;
				}
				OLED_ShowString(2, 14, "<-");
				state = 7;
			}
		
			if(readkey == 67){state1_2++;}              //����c����,state1��״̬�ı�,��Ļ�ֱ���ʾADD��DELѡ��
			else if(readkey == 65){state5 = 1;}       //����A����,state3��״̬�ı�,�����뿪���whileѭ�����ҽ�����һ��״̬
	}
	}
}

/* ��    �������ָ�ƺ���
 * ��    ����
 * �� �� ֵ��
 * ע�����
 */

//void SYSTEM_ADD_FINGER()
//{
//	uint8_t adFngrNum = 0;
//	uint8_t AdFngrNum = 0;
//	while(state == 6)
//	{
//		OLED_Clear();
//		OLED_ShowString(1,1,"fngr.No1");
//		OLED_ShowString(2,1,"fngr.No2");	
//		OLED_ShowString(3,1,"fngr.No3");			
//		
//		while(state6 != 1)
//		{
//			if(TTP229_ReadKey() == 67)
//			{
//				OLED_ShowString(adFngrNum,14,"<-");
//				adFngrNum++;
//				AdFngrNum++;
//				if(adFngrNum > 3){adFngrNum = 0;}
//				if(AdFngrNum > 3){AdFngrNum = 0;}
//			}
//			if(TTP229_ReadKey() == 65){state6 = 1;}
//		switch (AdFngrNum)
//		{
//			case 0:
//			case 1:
//			case 2: 
//		}
//		
//		}	
//	
//	}
//}












/*-------------------------------------------------------------------------------------------------------------------------*/	
/* ��    ����״̬�ܿغ���
 * ��    ������
 * �� �� ֵ����
 * ע�����1.
*            
 *           
 *
 *
 *
 *
 */
void State_Door()
{
	ENTER_STATE();
	LOGIN();//
	SYSTEM();
	SYSTEM_SET_PASSWORD();
	SYSTEM_SET_FINGER();
}	
	
	
