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

//出药模块
uint32_t medicine_num = 0; //对应药仓位置，1代表最左边，2代表左边第二，3代表左边第三，4代表左边第四,0代表不出药
uint8_t position_num = 0;//记录滑台位置 ,0代表最左边，1代表左边第二，2代表左边第三，3代表左边第四,0代表不出药
uint8_t flag_complete = 0;  //出药完成flag = 1
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
			if(Med_num != 0){//当前滑台对应的仓库有所需的药品
					flag_band_move = 0;
					flag_small_slide = 0;
					/*此时滑台正对于药仓，不用启动滑台，只需要启动履带*/
					/*显示屏进入仓库界面*/
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
						/*出药滑台启动*/
						Small_slide_move(position_num,Med_num);
						
						/*履带移动*/
						if(flag_small_slide == 1){						
							caterpillar_band_move();
						}				
						/*显示屏进入index界面*/
						Page_turn(page_index);
						
						switch(position_num){
							/*滑台位于仓库1*/
							case 0:
								exist_flag_A10 = 0;
								exist_flag_A9 = 0;							
								break;
							/*滑台位于仓库2*/
							case 1:
								exist_flag_A8 = 0;
								exist_flag_A7 = 0;
								break;
							/*滑台位于仓库3*/
							case 2:
								exist_flag_A6 = 0;
								exist_flag_A5 = 0;
								break;
							/*滑台位于仓库4*/
							case 3:
								exist_flag_A4 = 0;
								exist_flag_A3 = 0;
								break;
						}					
						medicine_num &= ~( 0xf << Atain[position_num]);//对应位与0					
						if(medicine_num == 0){
							/*语音播报:出药成功*/
							UsartPrintf(DEBUG_UART, "medicine_num :%d\r\n",medicine_num);
							voice_uart_printf(Drug_release_complete);
							break;
						}
							
			}
			if(medicine_num > compare_num ){
					position_num++;
					Client_Control_Slide(Right_shift_5);
					UsartPrintf(DEBUG_UART, "右移\r\n");
			}
			else if(medicine_num < compare_num){
					position_num--;
					Client_Control_Slide(Left_shift_5);
					UsartPrintf(DEBUG_UART, "左移\r\n");
			}
		}
	}	
}


void caterpillar_band_move(void){//履带移动函数
		/*履带启动*/
		HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
		/*语音播报:开始出药*/
		voice_uart_printf(Dispensing_medicine);
		/*十秒后履带停止*/
		HAL_Delay(10000);
		HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_2);
		
		flag_band_move = 1;
}
void Small_slide_move(uint8_t position_num,uint16_t Med_num){
	
		switch(position_num){
			case 0:
				/*仓库1滑台启动*/
				/*根据所需药品量选择启动多少次*/
				while(Med_num--){
						SmallSlide_pro(SLIDE1);	
						Digestion_tablet_num--;
				}
				HAL_Delay(2000);//等待10秒 滑台上的药全部滑下
				flag_small_slide = 1;				
				break;
			case 1:
				/*仓库2滑台启动*/
				/*根据所需药品量选择启动多少次*/
				while(Med_num--){
						SmallSlide_pro(SLIDE2);					
						HXZQ_capsule_num--;			
				}
				HAL_Delay(2000);//等待10秒 滑台上的药全部滑下
				flag_small_slide = 1;	
				break;
			case 2:
				/*仓库3滑台启动*/
				/*根据所需药品量选择启动多少次*/
				while(Med_num--){
						SmallSlide_pro(SLIDE3);			
						Vitamin_B_num--;		
				}
				HAL_Delay(2000);//等待10秒 滑台上的药全部滑下
				flag_small_slide = 1;
				break;
				case 3:
				/*仓库4滑台启动*/
				/*根据所需药品量选择启动多少次*/
				while(Med_num--){
						SmallSlide_pro(SLIDE4);	
						Stomach_recovering_num--;
				}
				HAL_Delay(2000);//等待10秒 滑台上的药全部滑下
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
					/*此时滑台正对于药仓，不用启动滑台，只需要启动履带*/
					case 1:
					{
						if(exist_flag_A10 == 1 || exist_flag_A9 == 1)//如果有药品
						{
							/*显示屏进入仓库一界面*/
							Page_turn(page_1);
							
							/*出药舵机启动*/
							
							/*履带启动*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
							/*语音播报:开始出药*/
							voice_uart_printf(Dispensing_medicine);
							/*十秒后履带停止*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*语音播报:出药成功*/
							voice_uart_printf(Drug_release_complete);
							
							/*显示屏进入index界面*/
							Page_turn(page_index);
							
							medicine_num = 0;
							position_num = 1;//记录滑台位置
							flag_complete = 1;
							exist_flag_A10 = 0;
							exist_flag_A9 = 0;
						}
						else
						{
							/*语音播放药品不存在*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 1;
							flag_complete = 1;
						}
					}break;
					/*此时滑台位于出药仓右侧一位，启动滑台向左6.65cm*/
					case 2:
					{
						if(exist_flag_A10 == 1 || exist_flag_A9 == 1)//如果有药品
						{
							/*显示屏进入仓库一界面*/
							Page_turn(page_1);
							
							/*出药舵机启动*/
				
							/*丝杆滑台向左前进6.65cm*/
							Client_Control_Slide(Left_shift_5);
							HAL_Delay(17000);//延迟17秒后开启履带电机
							/*履带启动*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
						/*语音播报:开始出药*/
							voice_uart_printf(Dispensing_medicine);
							/*五秒后履带停止*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*语音播报:出药成功*/
							voice_uart_printf(Drug_release_complete);
							
							/*显示屏进入index界面*/
							Page_turn(page_index);
							
							medicine_num = 0;
							position_num = 1;
							flag_complete = 1;
							exist_flag_A10 = 0;
							exist_flag_A9 = 0;
						}
						else
						{
							/*语音播放药品不存在*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 2;
							flag_complete = 1;
						}
					}break;
					/*此时滑台位于出药仓右侧两位，启动滑台向左13.3cm*/
					case 3:
					{
						if(exist_flag_A10 == 1 || exist_flag_A9 == 1)//如果有药品
						{
							/*显示屏进入仓库一界面*/
							Page_turn(page_1);
							/*出药舵机启动*/
				
							/*丝杆滑台向左前进13.3cm*/
							Client_Control_Slide(Left_shift_10);
							HAL_Delay(34000);//延迟34秒后开启履带电机
							/*履带启动*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
						/*语音播报:开始出药*/
							voice_uart_printf(Dispensing_medicine);
							/*五秒后履带停止*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*语音播报:出药成功*/
							voice_uart_printf(Drug_release_complete);
							
							/*显示屏进入index界面*/
							Page_turn(page_index);
							
							medicine_num = 0;
							position_num = 1;
							flag_complete = 1; 
							exist_flag_A10 = 0;
							exist_flag_A9 = 0;
						}
						else
						{
							/*语音播放药品不存在*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 3;
							flag_complete = 1;
						}	
					}break;
					/*此时滑台位于出药仓右侧三位，启动滑台向左19.95cm*/
					case 4:
					{
						if(exist_flag_A10 == 1 || exist_flag_A9 == 1)//如果有药品
						{
							/*显示屏进入仓库一界面*/
							Page_turn(page_1);
							
							/*出药舵机启动*/
							
							/*丝杆滑台向左前进19.95cm*/
							Client_Control_Slide(Left_shift_15);
							HAL_Delay(51000);//延迟51秒后开启履带电机
							/*履带启动*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
						/*语音播报:开始出药*/
							voice_uart_printf(Dispensing_medicine);
							/*五秒后履带停止*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*语音播报:出药成功*/
							voice_uart_printf(Drug_release_complete);
							
							/*显示屏进入index界面*/
							Page_turn(page_index);
							
							medicine_num = 0;
							position_num = 1;
							flag_complete = 1;
							exist_flag_A10 = 0;
							exist_flag_A9 = 0;
						}
						else
						{
							/*语音播放药品不存在*/
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
					/*此时滑台在指定药仓左侧一位，滑台向右前进6.65cm*/
					case 1:
					{
						if(exist_flag_A8 == 1 || exist_flag_A7 == 1)//如果有药品
						{
							/*显示屏进入仓库二界面*/
							Page_turn(page_2);
							
							/*出药舵机启动*/
							
							/*丝杆滑台向右前进6.65cm*/
							Client_Control_Slide(Right_shift_5);
							HAL_Delay(17000);//延迟17秒后开启履带电机
							
							/*履带启动*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
							/*语音播报:开始出药*/
							voice_uart_printf(Dispensing_medicine);
							/*五秒后履带停止*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*语音播报:出药成功*/
							voice_uart_printf(Drug_release_complete);
							
							/*显示屏进入index界面*/
							Page_turn(page_index);
							
							medicine_num = 0;
							position_num = 2;
							flag_complete = 1;
							exist_flag_A8 = 0;
							exist_flag_A7 = 0;
						}
						else
						{
							/*语音播放药品不存在*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 1;
							flag_complete = 1;	
						}
					}break;
					
					/*此时滑台正对于药仓，不用启动滑台，只需要启动履带*/
					case 2:
					{
						if(exist_flag_A8 == 1 || exist_flag_A7 == 1)//如果有药品
						{
							/*显示屏进入仓库二界面*/
							Page_turn(page_2);
							
							/*出药舵机启动*/
							
							/*履带启动*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
							/*语音播报:开始出药*/
							voice_uart_printf(Dispensing_medicine);
							/*五秒后履带停止*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*语音播报:出药成功*/
							voice_uart_printf(Drug_release_complete);
							
							/*显示屏进入index界面*/
							Page_turn(page_index);
							
							medicine_num = 0;
							position_num = 2;
							flag_complete = 1;
							exist_flag_A8 = 0;
							exist_flag_A7 = 0;
						}
						else
						{
							/*语音播放药品不存在*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 2;
							flag_complete = 1;	
						}
					}break;
					/*此时滑台位于出药仓右侧一位，启动滑台向左6.65cm*/
					case 3:
					{
						if(exist_flag_A8 == 1 || exist_flag_A7 == 1)//如果有药品
						{
							/*显示屏进入仓库二界面*/
							Page_turn(page_2);
							
							/*出药舵机启动*/
							
							/*丝杆滑台向左前进6.65cm*/
							Client_Control_Slide(Left_shift_5);
							HAL_Delay(17000);//延迟17秒后开启履带电机
							/*履带启动*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
						/*语音播报:开始出药*/
							voice_uart_printf(Dispensing_medicine);
							/*五秒后履带停止*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*语音播报:出药成功*/
							voice_uart_printf(Drug_release_complete);
							
							/*显示屏进入index界面*/
							Page_turn(page_index);
							
							medicine_num = 0;
							position_num = 2;
							flag_complete = 1;
							exist_flag_A8 = 0;
							exist_flag_A7 = 0;
						}
						else
						{
							/*语音播放药品不存在*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 3;
							flag_complete = 1;	
						}
					}break;
					/*此时滑台位于出药仓右侧两位，启动滑台向左13.3cm*/
					case 4:
					{
						if(exist_flag_A8 == 1 || exist_flag_A7 == 1)//如果有药品
						{
							/*显示屏进入仓库二界面*/
							Page_turn(page_2);
							
							/*出药舵机启动*/
				
							/*丝杆滑台向左前进13.3cm*/
							Client_Control_Slide(Left_shift_10);
							HAL_Delay(34000);//延迟34秒后开启履带电机
							/*履带启动*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
						/*语音播报:开始出药*/
							voice_uart_printf(Dispensing_medicine);
							/*五秒后履带停止*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*语音播报:出药成功*/
							voice_uart_printf(Drug_release_complete);
							/*显示屏进入index界面*/
							Page_turn(page_index);
							
							medicine_num = 0;
							position_num = 2;
							flag_complete = 1;
							exist_flag_A8 = 0;
							exist_flag_A7 = 0;
						}
						else
						{
							/*语音播放药品不存在*/
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
					/*此时滑台位于药仓左侧两位，滑台向右移动13.3cm*/
					case 1:
					{
						if(exist_flag_A6 == 1 || exist_flag_A5 == 1)//如果有药品
						{
							/*显示屏进入仓库三界面*/
							Page_turn(page_3);
							/*出药舵机启动*/
					
							/*丝杆滑台向右前进13.3cm*/
							Client_Control_Slide(Right_shift_10);
							HAL_Delay(34000);//延迟13秒后开启履带电机
							
							/*履带启动*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
							/*语音播报:开始出药*/
							voice_uart_printf(Dispensing_medicine);
							/*五秒后履带停止*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*语音播报:出药成功*/
							voice_uart_printf(Drug_release_complete);
							/*显示屏进入index界面*/
							Page_turn(page_index);
							medicine_num = 0;
							position_num = 3;
							flag_complete = 1;
							exist_flag_A6 = 0;
							exist_flag_A5 = 0;
						}
						else
						{
							/*语音播放药品不存在*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 1;
							flag_complete = 1;
						}
					}break;
					/*此时滑台位于出药仓左侧一位，启动滑台向右6.65cm*/
					case 2:
					{
						if(exist_flag_A6 == 1 || exist_flag_A5 == 1)//如果有药品
						{
							/*显示屏进入仓库三界面*/
							Page_turn(page_3);
							/*出药舵机启动*/
				
							/*丝杆滑台向右前进6.65cm*/
							Client_Control_Slide(Right_shift_5);
							HAL_Delay(17000);//延迟13秒后开启履带电机
							/*履带启动*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
						/*语音播报:开始出药*/
							voice_uart_printf(Dispensing_medicine);
							/*五秒后履带停止*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*语音播报:出药成功*/
							voice_uart_printf(Drug_release_complete);
							
							/*显示屏进入index界面*/
							Page_turn(page_index);
							medicine_num = 0;
							position_num = 3;
							flag_complete = 1;
							exist_flag_A6 = 0;
							exist_flag_A5 = 0;
						}
						else
						{
							/*语音播放药品不存在*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 2;
							flag_complete = 1;
						}
					}break;
					/*此时滑台正对于药仓，不用启动滑台，只需要启动履带*/
					case 3:
					{
						if(exist_flag_A6 == 1 || exist_flag_A5 == 1)//如果有药品
						{
							/*显示屏进入仓库三界面*/
							Page_turn(page_3);
							
							/*出药舵机启动*/
				
							/*履带启动*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
							/*语音播报:开始出药*/
							voice_uart_printf(Dispensing_medicine);
							/*五秒后履带停止*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*语音播报:出药成功*/
							voice_uart_printf(Drug_release_complete);
							/*显示屏进入index界面*/
							Page_turn(page_index);
							
							medicine_num = 0;
							position_num = 3;
							flag_complete = 1;
							exist_flag_A6 = 0;
							exist_flag_A5 = 0;
						}
						else
						{
							/*语音播放药品不存在*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 3;
							flag_complete = 1;
						}
					}break;
					/*此时滑台位于出药仓右侧一位，启动滑台向左移动6.65cm*/
					case 4:
					{
						if(exist_flag_A6 == 1 || exist_flag_A5 == 1)//如果有药品
						{
							/*显示屏进入仓库三界面*/
							Page_turn(page_3);
							
							/*出药舵机启动*/
				
							/*丝杆滑台向左前进6.65cm*/
							Client_Control_Slide(Left_shift_5);
							HAL_Delay(17000);//延迟13秒后开启履带电机
							/*履带启动*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
						/*语音播报:开始出药*/
							voice_uart_printf(Dispensing_medicine);
							/*五秒后履带停止*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*语音播报:出药成功*/
							voice_uart_printf(Drug_release_complete);
							/*显示屏进入index界面*/
							Page_turn(page_index);
							
							medicine_num = 0;
							position_num = 3;
							flag_complete = 1;
							exist_flag_A6 = 0;
							exist_flag_A5 = 0;
						}
						else
						{
							/*语音播放药品不存在*/
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
					/*此时滑台位于药仓左侧三位，滑台向右移动19.95cm*/
					case 1:
					{
						if(exist_flag_A4 == 1 || exist_flag_A3 == 1)//如果有药品
						{
							/*显示屏进入仓库四界面*/
							Page_turn(page_4);
							/*出药舵机启动*/
							
							/*丝杆滑台向右前进19.95cm*/
							Client_Control_Slide(Right_shift_15);
							HAL_Delay(51000);//延迟39秒后开启履带电机
							/*履带启动*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
							/*语音播报:开始出药*/
							voice_uart_printf(Dispensing_medicine);
							/*五秒后履带停止*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*语音播报:出药成功*/
							voice_uart_printf(Drug_release_complete);
							/*显示屏进入index界面*/
							Page_turn(page_index);
							medicine_num = 0;
							position_num = 4;
							flag_complete = 1;
							exist_flag_A4 = 0;
							exist_flag_A3 = 0;
						}
						else
						{
							/*语音播放药品不存在*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 1;
							flag_complete = 1;
						}
					}break;
					/*此时滑台位于出药仓左侧两位，启动滑台向右13.3cm*/
					case 2:
					{
						if(exist_flag_A4 == 1 || exist_flag_A3 == 1)//如果有药品
						{
							/*显示屏进入仓库四界面*/
							Page_turn(page_4);
							/*出药舵机启动*/
							
							/*丝杆滑台向右前进13.3cm*/
							Client_Control_Slide(Right_shift_10);
							HAL_Delay(34000);//延迟34秒后开启履带电机
							/*履带启动*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
						/*语音播报:开始出药*/
							voice_uart_printf(Dispensing_medicine);
							/*五秒后履带停止*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*语音播报:出药成功*/
							voice_uart_printf(Drug_release_complete);
							/*显示屏进入index界面*/
							Page_turn(page_index);
							
							medicine_num = 0;
							position_num = 4;
							flag_complete = 1;
							exist_flag_A4 = 0;
							exist_flag_A3 = 0;
						}
						else
						{
							/*语音播放药品不存在*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 2;
							flag_complete = 1;
						}
					}break;
					/*此时滑台位于出药仓左侧一位，启动滑台向右6.65cm*/
					case 3:
					{
						if(exist_flag_A4 == 1 || exist_flag_A3 == 1)//如果有药品
						{
							/*显示屏进入仓库四界面*/
							Page_turn(page_4);
							
							/*出药舵机启动*/
				
							/*丝杆滑台向右前进6.65cm*/
							Client_Control_Slide(Right_shift_5);
							HAL_Delay(17000);//延迟17秒后开启履带电机
							/*履带启动*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
						/*语音播报:开始出药*/
							voice_uart_printf(Dispensing_medicine);
							/*五秒后履带停止*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*语音播报:出药成功*/
							voice_uart_printf(Drug_release_complete);
							/*显示屏进入index界面*/
							Page_turn(page_index);
							medicine_num = 0;
							position_num = 4;
							flag_complete = 1;
							exist_flag_A4 = 0;
							exist_flag_A3 = 0;
						}
						else
						{
							/*语音播放药品不存在*/
							voice_uart_printf(Warning_voice);
							medicine_num = 0;
							position_num = 3;
							flag_complete = 1;
						}
					}break;
					/*此时滑台正对于药仓，不用启动滑台，只需要启动履带*/
					case 4:
					{
						if(exist_flag_A4 == 1 || exist_flag_A3 == 1)//如果有药品
						{
							/*显示屏进入仓库四界面*/
							Page_turn(page_4);
							/*出药舵机启动*/
							
							/*履带启动*/
							HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
						/*语音播报:开始出药*/
							voice_uart_printf(Dispensing_medicine);
							/*五秒后履带停止*/
							HAL_Delay(10000);
							HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
							/*语音播报:出药成功*/
							voice_uart_printf(Drug_release_complete);
							/*显示屏进入index界面*/
							Page_turn(page_index);
							
							medicine_num = 0;
							position_num = 4;
							flag_complete = 1;
						}
						else
						{
							/*语音播放药品不存在*/
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
	/*在循环中不断地去接受hour和minute的值*/
	if(set_hour !=0 ||set_minute !=0)
	{
		if(hour == set_hour && minute == set_minute)
		{
			UsartPrintf(DEBUG_UART, "set_time ==  Time\r\n");
			voice_uart_printf(Alarm_clock_reminder);
			//显示屏显示设置时间到
			
			set_hour = 0;
			set_minute = 0;
		}
	} 
	
//	HAL_Delay(1000);
	/*RTC肯定一直在走，去判断RTC与设定的时间是否符合，如果符合就进行相应的操作*/
	/*在进行相应操作完成后，将hour和minute置0*/
}
void Prescription_task(void)
{
	/*判断处方药单RFID*/
	if(exist_flag_A2 == 1)
	{
		/*语音播报：医生药方，出药中*/
		voice_uart_printf(Doctor_prescription);
		HAL_Delay(2000);
		/*一盒健胃消食片*/
		/*一盒藿香正气胶囊*/
		/*一盒维生素B*/
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
			//语音播报：仓库剩余库存为零，请及时补药
			HAL_Delay(2000);
			voice_uart_printf(Inventory_reminder);
			exist_flag_A2 = 0;
		}		
	}
	else if(exist_flag_A1 == 1)
	{
		/*语音播报：医生药方，出药中*/
		voice_uart_printf(Doctor_prescription);
		HAL_Delay(2000);
		/*一盒健胃消食片*/
		/*一盒胃康灵*/
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
			//语音播报：仓库剩余库存为零，请及时补药
			HAL_Delay(2000);
			voice_uart_printf(Inventory_reminder);
			exist_flag_A1 = 0;
		}
	}
}

