#include "Small_slide_move.h"
#include "tim.h"
#include "motor.h"
#include "stdint.h"
#include "usart.h"
//typedef enum { SFinish, SWorking } S_WORK_STATE;
//typedef enum { Left, Right } Direction;
//typedef enum { SLIDE1, SLIDE2, SLIDE3, SLIDE4 } SlideNumber;
//S_WORK_STATE swork_state = SFinish;

#if 0
uint32_t slide_pulse_counts[4] = {0};
extern TIM_HandleTypeDef htim4;

void SmallSlide_pro(uint8_t Slide_num) {
    Small_slide(1, Slide_num);
    HAL_Delay(500);
    Small_slide(0, Slide_num);
    HAL_Delay(500);
}

void Small_slide(uint8_t work_dir, uint8_t Slide_num) {        
    Small_slide_Work(710, work_dir == 0 ? Left : Right, Slide_num);
}

void Small_slide_Work(uint32_t x_pulse_count, Direction direction, uint8_t Slide_num) {
    if (swork_state != SWorking) {
        slide_pulse_counts[Slide_num] = x_pulse_count;
        swork_state = SWorking;
    }

    if (swork_state == SWorking && x_pulse_count != 0) {
        if (direction == Right) {
            Slide_Right(Slide_num);
        } else {
            Slide_Left(Slide_num);
        }
    }
}

void Slide_Right(uint8_t Slide_num) {
    EN_OFF(Slide_num);
    DIR_ON(Slide_num);
    HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_1 + Slide_num);
}

void Slide_Left(uint8_t Slide_num) {
    EN_OFF(Slide_num);
    DIR_OFF(Slide_num);
    HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_1 + Slide_num);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM4) {
        uint8_t channel = htim->Channel - HAL_TIM_ACTIVE_CHANNEL_1;
        if (channel < 4) {
            slide_pulse_counts[channel]--;
            if (slide_pulse_counts[channel] == 0) {
                swork_state = SFinish;
                HAL_TIM_PWM_Stop_IT(&htim4, TIM_CHANNEL_1 + channel);
            }
        }
    }
}

void EN_OFF(uint8_t Slide_num) {
    // Implement enabling off for Slide_num
	switch(Slide_num){
		case 0:
			EN_1_OFF();
			break;
		case 1:
			EN_2_OFF();
			break;
		case 2:
			EN_3_OFF();
			break;
		case 3:
			EN_4_OFF();
			break;
		
		default:break;
	}
}

void DIR_ON(uint8_t Slide_num) {
    // Implement direction on for Slide_num
	switch(Slide_num){
		case 0:
			DIR_1_ON();
			break;
		case 1:
			DIR_2_ON();
			break;
		case 2:
			DIR_3_ON();
			break;
		case 3:
			DIR_4_ON();
			break;
	
		default:break;	
	}
}

void DIR_OFF(uint8_t Slide_num) {
    // Implement direction off for Slide_num
	switch(Slide_num){
		case 0:
			DIR_1_OFF();
			break;
		case 1:
			DIR_2_OFF();
			break;
		case 2:
			DIR_3_OFF();
			break;
		case 3:
			DIR_4_OFF();
			break;
	
		default:break;	
	}
}
#endif




#if 1

enum S_WORK_STATE S1_work_state = SFinish;
enum S_WORK_STATE S2_work_state = SFinish;
enum S_WORK_STATE S3_work_state = SFinish;
enum S_WORK_STATE S4_work_state = SFinish;

uint32_t Slide1_pulse_count = 0,Slide2_pulse_count= 0,Slide3_pulse_count = 0,Slide4_pulse_count= 0;
extern TIM_HandleTypeDef htim4;


void SmallSlide_pro(uint8_t Slide_num)//小滑台左右滑一次
{
	switch(Slide_num){
		case 0:
			Slide_1_move(3300,Left);
			UsartPrintf(DEBUG_UART, "small_1_slide\r\n");
			HAL_Delay(6000);
			Slide_1_move(3300,Right);
			UsartPrintf(DEBUG_UART, "small_1_slide\r\n");
			HAL_Delay(6000);
			break;
		case 1:
			Slide_2_move(3300,Left);
			UsartPrintf(DEBUG_UART, "small_2_slide\r\n");
			HAL_Delay(6000);
			Slide_2_move(3300,Right);
			UsartPrintf(DEBUG_UART, "small_2_slide\r\n");
			HAL_Delay(6000);
			break;
		case 2:
			Slide_3_move(3300,Left);
			UsartPrintf(DEBUG_UART, "small_3_slide\r\n");
			HAL_Delay(6000);
			Slide_3_move(3300,Right);
			UsartPrintf(DEBUG_UART, "small_3_slide\r\n");
			HAL_Delay(6000);
			break;
		case 3:
			Slide_4_move(3300,Left);
			UsartPrintf(DEBUG_UART, "small_4_slide\r\n");
			HAL_Delay(6000);
			Slide_4_move(3300,Right);
			UsartPrintf(DEBUG_UART, "small_4_slide\r\n");
			HAL_Delay(6000);
			break;
	
		default:break;
	}
}
//void Small_slide(uint8_t work_dir,uint8_t Slide_num)//总控制
//{		
//	switch(work_dir)
//		{
//			case 0:
//			{
//				Small_slide_Work(700,Left,Slide_num);//左移45mm
//  			}break;
//			case 1:
//			{
//				Small_slide_Work(700,Right,Slide_num);//右移45mm
//			}break;
//			default:break;
//		}
//}

//void Small_slide_Work(uint32_t x_pulse_count,uint8_t direction,uint8_t Slide_num)
//{
//	if(swork_state != SWorking )//assign_value_flag == 1 &&
//	{
//		switch(Slide_num){
//			case SLIDE1:
//				Slide1_pulse_count = x_pulse_count;
//				break;
//			case SLIDE2:
//				Slide2_pulse_count = x_pulse_count;
//				break;
//			case SLIDE3:
//				Slide3_pulse_count = x_pulse_count;
//				break;
//			case SLIDE4:
//				Slide4_pulse_count = x_pulse_count;
//				break;
//		}
//		swork_state = SWorking;
//	}
//	if(swork_state == SWorking )//&& work_flag == 1
//	{
//		switch(direction)
//		{
//			case Right:
//			{
//				if(x_pulse_count != 0)
//				{
//					switch(Slide_num){
//						case SLIDE1:
//							Slide_1_Right();
//							break;
//						case SLIDE2:
//							Slide_2_Right();
//							break;
//						case SLIDE3:
//							Slide_3_Right();
//							break;
//						case SLIDE4:
//							Slide_4_Right();
//							break;
//						default:break;
//					}
//				}
//			}break;
//			case Left:
//			{
//				if(x_pulse_count != 0)
//				{
//					switch(Slide_num){
//						case SLIDE1:
//							Slide_1_Left();
//							break;
//						case SLIDE2:
//							Slide_2_Left();
//							break;
//						case SLIDE3:
//							Slide_3_Left();
//							break;
//						case SLIDE4:
//							Slide_4_Left();
//							break;
//						default:break;
//					}
//				}
//			}break;
//			default:break;
//		}
//	}
//}

void Slide_1_move(uint32_t pulse_count,uint8_t direction )
{
	if(S1_work_state != SWorking ){
		Slide1_pulse_count = pulse_count;
		S1_work_state = SWorking;
	}
	if(S1_work_state == SWorking)
	{
		switch(direction)
		{
			case Right:
			{
				if(pulse_count != 0)
				{
					Slide_1_Right();
				}
			}break;
			case Left:
			{
				if(pulse_count != 0)
				{
					Slide_1_Left();
				}
			}break;
			default:break;
		}
	}
}
void Slide_2_move(uint32_t pulse_count,uint8_t direction )
{
	if(S2_work_state != SWorking ){
		Slide2_pulse_count = pulse_count;
		S2_work_state = SWorking;
	}
	if(S2_work_state == SWorking)
	{
		switch(direction)
		{
			case Right:
			{
				if(pulse_count != 0)
				{
					Slide_2_Right();
				}
			}break;
			case Left:
			{
				if(pulse_count != 0)
				{
					Slide_2_Left();
				}
			}break;
			default:break;
		}
	}

}
void Slide_3_move(uint32_t pulse_count,uint8_t direction )
{
	if(S3_work_state != SWorking ){
		Slide3_pulse_count = pulse_count;
		S3_work_state = SWorking;
	}
	if(S3_work_state == SWorking)
	{
		switch(direction)
		{
			case Right:
			{
				if(pulse_count != 0)
				{
					Slide_3_Right();
				}
			}break;
			case Left:
			{
				if(pulse_count != 0)
				{
					Slide_3_Left();
				}
			}break;
			default:break;
		}
	}
}

void Slide_4_move(uint32_t pulse_count,uint8_t direction )
{
	if(S4_work_state != SWorking ){
		Slide4_pulse_count = pulse_count;
		S4_work_state = SWorking;
	}
	if(S4_work_state == SWorking)
	{
		switch(direction)
		{
			case Right:
			{
				if(pulse_count != 0)
				{
					Slide_4_Right();
				}
			}break;
			case Left:
			{
				if(pulse_count != 0)
				{
					Slide_4_Left();
				}
			}break;
			default:break;
		}
	}
}

void Slide_1_Right(void)
{
	EN_1_OFF();
	DIR_1_ON();
	HAL_TIM_PWM_Start_IT(&htim4,TIM_CHANNEL_1);
}
void Slide_1_Left(void)
{
	EN_1_OFF();
	DIR_1_OFF();
	HAL_TIM_PWM_Start_IT(&htim4,TIM_CHANNEL_1);
}

void Slide_2_Right(void)
{
	EN_2_OFF();
	DIR_2_ON();
	HAL_TIM_PWM_Start_IT(&htim5,TIM_CHANNEL_1);
}
void Slide_2_Left(void)
{
	EN_2_OFF();
	DIR_2_OFF();
	HAL_TIM_PWM_Start_IT(&htim5,TIM_CHANNEL_1);
}
void Slide_3_Right(void)
{
	EN_3_OFF();
	DIR_3_ON();
	HAL_TIM_PWM_Start_IT(&htim8,TIM_CHANNEL_2);
}
void Slide_3_Left(void)
{
	EN_3_OFF();
	DIR_3_OFF();
	HAL_TIM_PWM_Start_IT(&htim8,TIM_CHANNEL_2);
}

void Slide_4_Right(void)
{
	EN_4_OFF();
	DIR_4_ON();
	HAL_TIM_PWM_Start_IT(&htim20,TIM_CHANNEL_1);
}
void Slide_4_Left(void)
{
	EN_4_OFF();
	DIR_4_OFF();
	HAL_TIM_PWM_Start_IT(&htim20,TIM_CHANNEL_1);
}

#endif

