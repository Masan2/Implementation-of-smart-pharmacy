#include "main.h"
#include "stm32g4xx.h"                  // Device header
#include "voice.h"
#include "tim.h"
#include "task.h"
#include "motor.h"
#include "display.h"
#include "Warehouse.h"
#include "rtc.h"
#include "stdio.h"
#include "usart.h"
#include "onenet.h"
#include "Small_slide_move.h"

//��ҩģ��
uint32_t medicine_num = 0; //��Ӧҩ��λ�ã�1��������ߣ�2������ߵڶ���3������ߵ�����4������ߵ���,0������ҩ
uint8_t position_num = 0;//��¼��̨λ�� ,0��������ߣ�1������ߵڶ���2������ߵ�����3������ߵ���,0������ҩ
uint8_t flag_complete = 0;  //��ҩ���flag = 1
uint32_t compare_num;
uint8_t Atain[4] = {0,4,8,12};
uint8_t flag_band_move = 0;
uint8_t flag_small_slide = 0;
extern uint8_t work_state;
extern uint32_t X_pulse_count,Y_pulse_count;

void medicine_pro(void)
{
	uint8_t Med_num = 0;
	while(medicine_num != 0){
		if(work_state == Finish){
			compare_num = 0xf << Atain[position_num];
			Med_num = ((medicine_num >> Atain[position_num]) & 0xF);
			if(Med_num != 0){//��ǰ��̨��Ӧ�Ĳֿ��������ҩƷ
					flag_band_move = 0;
					flag_small_slide = 0;
					/*��ʱ��̨������ҩ�֣�����������̨��ֻ��Ҫ�����Ĵ�*/
					/*��ʾ������ֿ����*/
						switch(position_num){
							case 0:
								Page_turn(page_1);
								break;
							case 1:
								Page_turn(page_2);
								break;
							case 2:
								Page_turn(page_3);
								break;
							case 3:
								Page_turn(page_4);
								break;
							default:break;
						}			
						/*��ҩ��̨����*/
						Small_slide_move(position_num,Med_num);
						
						/*�Ĵ��ƶ�*/
						if(flag_small_slide == 1){						
							caterpillar_band_move();
						}				
						/*��ʾ������index����*/
						Page_turn(page_index);
						
						switch(position_num){
							/*��̨λ�ڲֿ�1*/
							case 0:
								exist_flag_A10 = 0;
								exist_flag_A9 = 0;							
								break;
							/*��̨λ�ڲֿ�2*/
							case 1:
								exist_flag_A8 = 0;
								exist_flag_A7 = 0;
								break;
							/*��̨λ�ڲֿ�3*/
							case 2:
								exist_flag_A6 = 0;
								exist_flag_A5 = 0;
								break;
							/*��̨λ�ڲֿ�4*/
							case 3:
								exist_flag_A4 = 0;
								exist_flag_A3 = 0;
								break;
						}					
						medicine_num &= ~( 0xf << Atain[position_num]);//��Ӧλ��0					
						if(medicine_num == 0){
							/*��������:��ҩ�ɹ�*/
							UsartPrintf(DEBUG_UART, "medicine_num :%d\r\n",medicine_num);
							voice_uart_printf(Drug_release_complete);
							break;
						}
							
			}
			if(medicine_num > compare_num ){
					position_num++;
					Client_Control_Slide(Right_shift_5);
					UsartPrintf(DEBUG_UART, "����\r\n");
			}
			else if(medicine_num < compare_num){
					position_num--;
					Client_Control_Slide(Left_shift_5);
					UsartPrintf(DEBUG_UART, "����\r\n");
			}
		}
	}	
}


void caterpillar_band_move(void){//�Ĵ��ƶ�����
		/*�Ĵ�����*/
		HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
		/*��������:��ʼ��ҩ*/
		voice_uart_printf(Dispensing_medicine);
		/*ʮ����Ĵ�ֹͣ*/
		HAL_Delay(10000);
		HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_2);
		
		flag_band_move = 1;
}
void Small_slide_move(uint8_t position_num,uint16_t Med_num){
	
		switch(position_num){
			case 0:
				/*�ֿ�1��̨����*/
				/*��������ҩƷ��ѡ���������ٴ�*/
				while(Med_num--){
						SmallSlide_pro(SLIDE1);	
						Digestion_tablet_num--;
				}
				HAL_Delay(2000);//�ȴ�10�� ��̨�ϵ�ҩȫ������
				flag_small_slide = 1;				
				break;
			case 1:
				/*�ֿ�2��̨����*/
				/*��������ҩƷ��ѡ���������ٴ�*/
				while(Med_num--){
						SmallSlide_pro(SLIDE2);					
						HXZQ_capsule_num--;			
				}
				HAL_Delay(2000);//�ȴ�10�� ��̨�ϵ�ҩȫ������
				flag_small_slide = 1;	
				break;
			case 2:
				/*�ֿ�3��̨����*/
				/*��������ҩƷ��ѡ���������ٴ�*/
				while(Med_num--){
						SmallSlide_pro(SLIDE3);			
						Vitamin_B_num--;		
				}
				HAL_Delay(2000);//�ȴ�10�� ��̨�ϵ�ҩȫ������
				flag_small_slide = 1;
				break;
				case 3:
				/*�ֿ�4��̨����*/
				/*��������ҩƷ��ѡ���������ٴ�*/
				while(Med_num--){
						SmallSlide_pro(SLIDE4);	
						Stomach_recovering_num--;
				}
				HAL_Delay(2000);//�ȴ�10�� ��̨�ϵ�ҩȫ������
				flag_small_slide = 1;
				break;
			}
}
#if 0
void medicine_pro(void)
{	
	if(flag_complete == 0)
	{
		switch(medicine_num)
		{	
			case 1:
			{
				switch(position_num)
				{
					/*��ʱ��̨������ҩ�֣�����������̨��ֻ��Ҫ�����Ĵ�*/
					case 1:
					{
						if(exist_flag_A10 == 1 || exist_flag_A9 == 1)//�����ҩƷ
						{
							/*��ʾ������ֿ�һ����*/
							Page_turn(page_1);
							
							/*��ҩ�������*/
							
							/*�Ĵ�����*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
							/*��������:��ʼ��ҩ*/
							voice_uart_printf(Dispensing_medicine);
							/*ʮ����Ĵ�ֹͣ*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*��������:��ҩ�ɹ�*/
							voice_uart_printf(Drug_release_complete);
							
							/*��ʾ������index����*/
							Page_turn(page_index);
							
							medicine_num = 0;
							position_num = 1;//��¼��̨λ��
							flag_complete = 1;
							exist_flag_A10 = 0;
							exist_flag_A9 = 0;
						}
						else
						{
							/*��������ҩƷ������*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 1;
							flag_complete = 1;
						}
					}break;
					/*��ʱ��̨λ�ڳ�ҩ���Ҳ�һλ��������̨����6.65cm*/
					case 2:
					{
						if(exist_flag_A10 == 1 || exist_flag_A9 == 1)//�����ҩƷ
						{
							/*��ʾ������ֿ�һ����*/
							Page_turn(page_1);
							
							/*��ҩ�������*/
				
							/*˿�˻�̨����ǰ��6.65cm*/
							Client_Control_Slide(Left_shift_5);
							HAL_Delay(17000);//�ӳ�17������Ĵ����
							/*�Ĵ�����*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
						/*��������:��ʼ��ҩ*/
							voice_uart_printf(Dispensing_medicine);
							/*������Ĵ�ֹͣ*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*��������:��ҩ�ɹ�*/
							voice_uart_printf(Drug_release_complete);
							
							/*��ʾ������index����*/
							Page_turn(page_index);
							
							medicine_num = 0;
							position_num = 1;
							flag_complete = 1;
							exist_flag_A10 = 0;
							exist_flag_A9 = 0;
						}
						else
						{
							/*��������ҩƷ������*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 2;
							flag_complete = 1;
						}
					}break;
					/*��ʱ��̨λ�ڳ�ҩ���Ҳ���λ��������̨����13.3cm*/
					case 3:
					{
						if(exist_flag_A10 == 1 || exist_flag_A9 == 1)//�����ҩƷ
						{
							/*��ʾ������ֿ�һ����*/
							Page_turn(page_1);
							/*��ҩ�������*/
				
							/*˿�˻�̨����ǰ��13.3cm*/
							Client_Control_Slide(Left_shift_10);
							HAL_Delay(34000);//�ӳ�34������Ĵ����
							/*�Ĵ�����*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
						/*��������:��ʼ��ҩ*/
							voice_uart_printf(Dispensing_medicine);
							/*������Ĵ�ֹͣ*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*��������:��ҩ�ɹ�*/
							voice_uart_printf(Drug_release_complete);
							
							/*��ʾ������index����*/
							Page_turn(page_index);
							
							medicine_num = 0;
							position_num = 1;
							flag_complete = 1; 
							exist_flag_A10 = 0;
							exist_flag_A9 = 0;
						}
						else
						{
							/*��������ҩƷ������*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 3;
							flag_complete = 1;
						}	
					}break;
					/*��ʱ��̨λ�ڳ�ҩ���Ҳ���λ��������̨����19.95cm*/
					case 4:
					{
						if(exist_flag_A10 == 1 || exist_flag_A9 == 1)//�����ҩƷ
						{
							/*��ʾ������ֿ�һ����*/
							Page_turn(page_1);
							
							/*��ҩ�������*/
							
							/*˿�˻�̨����ǰ��19.95cm*/
							Client_Control_Slide(Left_shift_15);
							HAL_Delay(51000);//�ӳ�51������Ĵ����
							/*�Ĵ�����*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
						/*��������:��ʼ��ҩ*/
							voice_uart_printf(Dispensing_medicine);
							/*������Ĵ�ֹͣ*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*��������:��ҩ�ɹ�*/
							voice_uart_printf(Drug_release_complete);
							
							/*��ʾ������index����*/
							Page_turn(page_index);
							
							medicine_num = 0;
							position_num = 1;
							flag_complete = 1;
							exist_flag_A10 = 0;
							exist_flag_A9 = 0;
						}
						else
						{
							/*��������ҩƷ������*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 4;
							flag_complete = 1;	
						}
					}break;
				}
			}break;
			case 2:
			{
				switch(position_num)
				{
					/*��ʱ��̨��ָ��ҩ�����һλ����̨����ǰ��6.65cm*/
					case 1:
					{
						if(exist_flag_A8 == 1 || exist_flag_A7 == 1)//�����ҩƷ
						{
							/*��ʾ������ֿ������*/
							Page_turn(page_2);
							
							/*��ҩ�������*/
							
							/*˿�˻�̨����ǰ��6.65cm*/
							Client_Control_Slide(Right_shift_5);
							HAL_Delay(17000);//�ӳ�17������Ĵ����
							
							/*�Ĵ�����*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
							/*��������:��ʼ��ҩ*/
							voice_uart_printf(Dispensing_medicine);
							/*������Ĵ�ֹͣ*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*��������:��ҩ�ɹ�*/
							voice_uart_printf(Drug_release_complete);
							
							/*��ʾ������index����*/
							Page_turn(page_index);
							
							medicine_num = 0;
							position_num = 2;
							flag_complete = 1;
							exist_flag_A8 = 0;
							exist_flag_A7 = 0;
						}
						else
						{
							/*��������ҩƷ������*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 1;
							flag_complete = 1;	
						}
					}break;
					
					/*��ʱ��̨������ҩ�֣�����������̨��ֻ��Ҫ�����Ĵ�*/
					case 2:
					{
						if(exist_flag_A8 == 1 || exist_flag_A7 == 1)//�����ҩƷ
						{
							/*��ʾ������ֿ������*/
							Page_turn(page_2);
							
							/*��ҩ�������*/
							
							/*�Ĵ�����*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
							/*��������:��ʼ��ҩ*/
							voice_uart_printf(Dispensing_medicine);
							/*������Ĵ�ֹͣ*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*��������:��ҩ�ɹ�*/
							voice_uart_printf(Drug_release_complete);
							
							/*��ʾ������index����*/
							Page_turn(page_index);
							
							medicine_num = 0;
							position_num = 2;
							flag_complete = 1;
							exist_flag_A8 = 0;
							exist_flag_A7 = 0;
						}
						else
						{
							/*��������ҩƷ������*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 2;
							flag_complete = 1;	
						}
					}break;
					/*��ʱ��̨λ�ڳ�ҩ���Ҳ�һλ��������̨����6.65cm*/
					case 3:
					{
						if(exist_flag_A8 == 1 || exist_flag_A7 == 1)//�����ҩƷ
						{
							/*��ʾ������ֿ������*/
							Page_turn(page_2);
							
							/*��ҩ�������*/
							
							/*˿�˻�̨����ǰ��6.65cm*/
							Client_Control_Slide(Left_shift_5);
							HAL_Delay(17000);//�ӳ�17������Ĵ����
							/*�Ĵ�����*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
						/*��������:��ʼ��ҩ*/
							voice_uart_printf(Dispensing_medicine);
							/*������Ĵ�ֹͣ*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*��������:��ҩ�ɹ�*/
							voice_uart_printf(Drug_release_complete);
							
							/*��ʾ������index����*/
							Page_turn(page_index);
							
							medicine_num = 0;
							position_num = 2;
							flag_complete = 1;
							exist_flag_A8 = 0;
							exist_flag_A7 = 0;
						}
						else
						{
							/*��������ҩƷ������*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 3;
							flag_complete = 1;	
						}
					}break;
					/*��ʱ��̨λ�ڳ�ҩ���Ҳ���λ��������̨����13.3cm*/
					case 4:
					{
						if(exist_flag_A8 == 1 || exist_flag_A7 == 1)//�����ҩƷ
						{
							/*��ʾ������ֿ������*/
							Page_turn(page_2);
							
							/*��ҩ�������*/
				
							/*˿�˻�̨����ǰ��13.3cm*/
							Client_Control_Slide(Left_shift_10);
							HAL_Delay(34000);//�ӳ�34������Ĵ����
							/*�Ĵ�����*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
						/*��������:��ʼ��ҩ*/
							voice_uart_printf(Dispensing_medicine);
							/*������Ĵ�ֹͣ*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*��������:��ҩ�ɹ�*/
							voice_uart_printf(Drug_release_complete);
							/*��ʾ������index����*/
							Page_turn(page_index);
							
							medicine_num = 0;
							position_num = 2;
							flag_complete = 1;
							exist_flag_A8 = 0;
							exist_flag_A7 = 0;
						}
						else
						{
							/*��������ҩƷ������*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 4;
							flag_complete = 1;	
						}
					}break;
				}
			}break;
			case 3:
			{
				switch(position_num)
				{
					/*��ʱ��̨λ��ҩ�������λ����̨�����ƶ�13.3cm*/
					case 1:
					{
						if(exist_flag_A6 == 1 || exist_flag_A5 == 1)//�����ҩƷ
						{
							/*��ʾ������ֿ�������*/
							Page_turn(page_3);
							/*��ҩ�������*/
					
							/*˿�˻�̨����ǰ��13.3cm*/
							Client_Control_Slide(Right_shift_10);
							HAL_Delay(34000);//�ӳ�13������Ĵ����
							
							/*�Ĵ�����*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
							/*��������:��ʼ��ҩ*/
							voice_uart_printf(Dispensing_medicine);
							/*������Ĵ�ֹͣ*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*��������:��ҩ�ɹ�*/
							voice_uart_printf(Drug_release_complete);
							/*��ʾ������index����*/
							Page_turn(page_index);
							medicine_num = 0;
							position_num = 3;
							flag_complete = 1;
							exist_flag_A6 = 0;
							exist_flag_A5 = 0;
						}
						else
						{
							/*��������ҩƷ������*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 1;
							flag_complete = 1;
						}
					}break;
					/*��ʱ��̨λ�ڳ�ҩ�����һλ��������̨����6.65cm*/
					case 2:
					{
						if(exist_flag_A6 == 1 || exist_flag_A5 == 1)//�����ҩƷ
						{
							/*��ʾ������ֿ�������*/
							Page_turn(page_3);
							/*��ҩ�������*/
				
							/*˿�˻�̨����ǰ��6.65cm*/
							Client_Control_Slide(Right_shift_5);
							HAL_Delay(17000);//�ӳ�13������Ĵ����
							/*�Ĵ�����*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
						/*��������:��ʼ��ҩ*/
							voice_uart_printf(Dispensing_medicine);
							/*������Ĵ�ֹͣ*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*��������:��ҩ�ɹ�*/
							voice_uart_printf(Drug_release_complete);
							
							/*��ʾ������index����*/
							Page_turn(page_index);
							medicine_num = 0;
							position_num = 3;
							flag_complete = 1;
							exist_flag_A6 = 0;
							exist_flag_A5 = 0;
						}
						else
						{
							/*��������ҩƷ������*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 2;
							flag_complete = 1;
						}
					}break;
					/*��ʱ��̨������ҩ�֣�����������̨��ֻ��Ҫ�����Ĵ�*/
					case 3:
					{
						if(exist_flag_A6 == 1 || exist_flag_A5 == 1)//�����ҩƷ
						{
							/*��ʾ������ֿ�������*/
							Page_turn(page_3);
							
							/*��ҩ�������*/
				
							/*�Ĵ�����*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
							/*��������:��ʼ��ҩ*/
							voice_uart_printf(Dispensing_medicine);
							/*������Ĵ�ֹͣ*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*��������:��ҩ�ɹ�*/
							voice_uart_printf(Drug_release_complete);
							/*��ʾ������index����*/
							Page_turn(page_index);
							
							medicine_num = 0;
							position_num = 3;
							flag_complete = 1;
							exist_flag_A6 = 0;
							exist_flag_A5 = 0;
						}
						else
						{
							/*��������ҩƷ������*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 3;
							flag_complete = 1;
						}
					}break;
					/*��ʱ��̨λ�ڳ�ҩ���Ҳ�һλ��������̨�����ƶ�6.65cm*/
					case 4:
					{
						if(exist_flag_A6 == 1 || exist_flag_A5 == 1)//�����ҩƷ
						{
							/*��ʾ������ֿ�������*/
							Page_turn(page_3);
							
							/*��ҩ�������*/
				
							/*˿�˻�̨����ǰ��6.65cm*/
							Client_Control_Slide(Left_shift_5);
							HAL_Delay(17000);//�ӳ�13������Ĵ����
							/*�Ĵ�����*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
						/*��������:��ʼ��ҩ*/
							voice_uart_printf(Dispensing_medicine);
							/*������Ĵ�ֹͣ*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*��������:��ҩ�ɹ�*/
							voice_uart_printf(Drug_release_complete);
							/*��ʾ������index����*/
							Page_turn(page_index);
							
							medicine_num = 0;
							position_num = 3;
							flag_complete = 1;
							exist_flag_A6 = 0;
							exist_flag_A5 = 0;
						}
						else
						{
							/*��������ҩƷ������*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 4;
							flag_complete = 1;
						}
					}break;
				}
			}break;
			case 4:
			{
				switch(position_num)
				{
					/*��ʱ��̨λ��ҩ�������λ����̨�����ƶ�19.95cm*/
					case 1:
					{
						if(exist_flag_A4 == 1 || exist_flag_A3 == 1)//�����ҩƷ
						{
							/*��ʾ������ֿ��Ľ���*/
							Page_turn(page_4);
							/*��ҩ�������*/
							
							/*˿�˻�̨����ǰ��19.95cm*/
							Client_Control_Slide(Right_shift_15);
							HAL_Delay(51000);//�ӳ�39������Ĵ����
							/*�Ĵ�����*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
							/*��������:��ʼ��ҩ*/
							voice_uart_printf(Dispensing_medicine);
							/*������Ĵ�ֹͣ*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*��������:��ҩ�ɹ�*/
							voice_uart_printf(Drug_release_complete);
							/*��ʾ������index����*/
							Page_turn(page_index);
							medicine_num = 0;
							position_num = 4;
							flag_complete = 1;
							exist_flag_A4 = 0;
							exist_flag_A3 = 0;
						}
						else
						{
							/*��������ҩƷ������*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 1;
							flag_complete = 1;
						}
					}break;
					/*��ʱ��̨λ�ڳ�ҩ�������λ��������̨����13.3cm*/
					case 2:
					{
						if(exist_flag_A4 == 1 || exist_flag_A3 == 1)//�����ҩƷ
						{
							/*��ʾ������ֿ��Ľ���*/
							Page_turn(page_4);
							/*��ҩ�������*/
							
							/*˿�˻�̨����ǰ��13.3cm*/
							Client_Control_Slide(Right_shift_10);
							HAL_Delay(34000);//�ӳ�34������Ĵ����
							/*�Ĵ�����*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
						/*��������:��ʼ��ҩ*/
							voice_uart_printf(Dispensing_medicine);
							/*������Ĵ�ֹͣ*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*��������:��ҩ�ɹ�*/
							voice_uart_printf(Drug_release_complete);
							/*��ʾ������index����*/
							Page_turn(page_index);
							
							medicine_num = 0;
							position_num = 4;
							flag_complete = 1;
							exist_flag_A4 = 0;
							exist_flag_A3 = 0;
						}
						else
						{
							/*��������ҩƷ������*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 2;
							flag_complete = 1;
						}
					}break;
					/*��ʱ��̨λ�ڳ�ҩ�����һλ��������̨����6.65cm*/
					case 3:
					{
						if(exist_flag_A4 == 1 || exist_flag_A3 == 1)//�����ҩƷ
						{
							/*��ʾ������ֿ��Ľ���*/
							Page_turn(page_4);
							
							/*��ҩ�������*/
				
							/*˿�˻�̨����ǰ��6.65cm*/
							Client_Control_Slide(Right_shift_5);
							HAL_Delay(17000);//�ӳ�17������Ĵ����
							/*�Ĵ�����*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
						/*��������:��ʼ��ҩ*/
							voice_uart_printf(Dispensing_medicine);
							/*������Ĵ�ֹͣ*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*��������:��ҩ�ɹ�*/
							voice_uart_printf(Drug_release_complete);
							/*��ʾ������index����*/
							Page_turn(page_index);
							medicine_num = 0;
							position_num = 4;
							flag_complete = 1;
							exist_flag_A4 = 0;
							exist_flag_A3 = 0;
						}
						else
						{
							/*��������ҩƷ������*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 3;
							flag_complete = 1;
						}
					}break;
					/*��ʱ��̨������ҩ�֣�����������̨��ֻ��Ҫ�����Ĵ�*/
					case 4:
					{
						if(exist_flag_A4 == 1 || exist_flag_A3 == 1)//�����ҩƷ
						{
							/*��ʾ������ֿ��Ľ���*/
							Page_turn(page_4);
							/*��ҩ�������*/
							
							/*�Ĵ�����*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
						/*��������:��ʼ��ҩ*/
							voice_uart_printf(Dispensing_medicine);
							/*������Ĵ�ֹͣ*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*��������:��ҩ�ɹ�*/
							voice_uart_printf(Drug_release_complete);
							/*��ʾ������index����*/
							Page_turn(page_index);
							
							medicine_num = 0;
							position_num = 4;
							flag_complete = 1;
						}
						else
						{
							/*��������ҩƷ������*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 4;
							flag_complete = 1;
						}
					}break; 
				} 
			}break;
			default:break;
		}
	}
}
#endif
void Timed_task(int hour,int minute)
{
	/*��ѭ���в��ϵ�ȥ����hour��minute��ֵ*/
	if(set_hour !=0 ||set_minute !=0)
	{
		if(hour == set_hour && minute == set_minute)
		{
			UsartPrintf(DEBUG_UART, "set_time ==  Time\r\n");
			voice_uart_printf(Alarm_clock_reminder);
			//��ʾ����ʾ����ʱ�䵽
			
			set_hour = 0;
			set_minute = 0;
		}
	} 
	
//	HAL_Delay(1000);
	/*RTC�϶�һֱ���ߣ�ȥ�ж�RTC���趨��ʱ���Ƿ���ϣ�������Ͼͽ�����Ӧ�Ĳ���*/
	/*�ڽ�����Ӧ������ɺ󣬽�hour��minute��0*/
}
void Prescription_task(void)
{
	/*�жϴ���ҩ��RFID*/
	if(exist_flag_A2 == 1)
	{
		/*����������ҽ��ҩ������ҩ��*/
		voice_uart_printf(Doctor_prescription);
		HAL_Delay(2000);
		/*һ�н�θ��ʳƬ*/
		/*һ��޽����������*/
		/*һ��ά����B*/
		if(Digestion_tablet_num != 0 && HXZQ_capsule_num != 0 && Vitamin_B_num !=0)
		{
			medicine_num = 273;
//			medicine_pro();//0000 0001 0001 0001 = 273;
			exist_flag_A10 = 0;
			exist_flag_A9 = 0;
			exist_flag_A8 = 0;
			exist_flag_A7 = 0;
			exist_flag_A6 = 0;
			exist_flag_A5 = 0;
			
			exist_flag_A2 = 0;
		}
		else
		{
			//�����������ֿ�ʣ����Ϊ�㣬�뼰ʱ��ҩ
			HAL_Delay(2000);
			voice_uart_printf(Inventory_reminder);
			exist_flag_A2 = 0;
		}		
	}
	else if(exist_flag_A1 == 1)
	{
		/*����������ҽ��ҩ������ҩ��*/
		voice_uart_printf(Doctor_prescription);
		HAL_Delay(2000);
		/*һ�н�θ��ʳƬ*/
		/*һ��θ����*/
		if(Digestion_tablet_num != 0 && Stomach_recovering_num !=0 ){
			medicine_num = 4097;
//			medicine_pro();//0001 0000 0000 0001 = 4097
			exist_flag_A10 = 0;
			exist_flag_A9 = 0;
			exist_flag_A4 = 0;
			exist_flag_A3 = 0;
			exist_flag_A1 = 0;
		}
		else{
			//�����������ֿ�ʣ����Ϊ�㣬�뼰ʱ��ҩ
			HAL_Delay(2000);
			voice_uart_printf(Inventory_reminder);
			exist_flag_A1 = 0;
		}
	}
}

