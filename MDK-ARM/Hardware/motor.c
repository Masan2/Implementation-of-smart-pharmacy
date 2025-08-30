#include "motor.h"
#include "tim.h"
#include "Small_slide_move.h"
#include "usart.h"

uint8_t key1_flag = 2;
uint8_t work_flag = 1;
uint32_t X_pulse_count,Y_pulse_count;
volatile uint8_t work_state = Finish;
enum WORK_STATE work_last_state = Finish;
extern TIM_HandleTypeDef htim1;

struct RX_ORDER rx_order = {
.flag = 0
};
uint8_t assign_value_flag;
//char lpuart1_rx;



void Client_Control_Slide(uint8_t work_dir)//×Ü¿ØÖÆ
{		
	switch(work_dir)
		{
			case 1:
			{
//				printf("1 Command do\n");
				Slide_Work(1330000,Left);//×óÒÆ6.65cm
  			}break;
			case 2:
			{
//				printf("2 Command do\n");
				Slide_Work(2660000,Left);//×óÒÆ13.3cm
			}break;
			case 3:
			{
//				printf("3 Command do\n");
				Slide_Work(3990000,Left);//×óÒÆ19.95cm
			}break;
			case 4:
			{
//				printf("5 Command do\n");
				Slide_Work(1330000,Right);//ÓÒÒÆ6.65cm
			}break;
			case 5:
			{
//				printf("6 Command do\n");
				Slide_Work(2660000,Right);//ÓÒÒÆ13.3cm
			}break;
			case 6:
			{
//				printf("7 Command do\n");
				Slide_Work(3990000,Right);//ÓÒÒÆ19.95cm
			}break;
			default:break;
		}
}
void Slide_Work(uint32_t x_pulse_count,uint8_t direction)
{
	if(work_state != Working )//assign_value_flag == 1 &&
	{
		X_pulse_count = x_pulse_count;
		work_state = Working;
	}
	if(work_state == Working )//&& work_flag == 1
	{
		switch(direction)
		{
			case Right:
			{
				if(x_pulse_count != 0)
				{
					Slide_X_Right();
				}
			}break;
			case Left:
			{
				if(x_pulse_count != 0)
				{
					Slide_X_Left();
				}
			}break;
			default:break;
		}
	}
}
void Slide_X_Right(void)
{
//	EN_X_OFF();
	DIR_X_ON();
	HAL_TIM_PWM_Start_IT(&htim3,TIM_CHANNEL_1);
}
void Slide_X_Left(void)
{
//	EN_X_OFF();	
	DIR_X_OFF();
	HAL_TIM_PWM_Start_IT(&htim3,TIM_CHANNEL_1);
}
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)//TIM3Í¨µÀ1
{
	if(htim->Instance == TIM3)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			X_pulse_count--;
			if(X_pulse_count == 0)
			{
				UsartPrintf(DEBUG_UART, "motor complete,X_pulse_count:%d\r\n",X_pulse_count);
				work_state = Finish;
				HAL_TIM_PWM_Stop_IT(&htim3,TIM_CHANNEL_1);
			}
		}
	}
	if(htim->Instance == TIM4)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			Slide1_pulse_count--;
			if(Slide1_pulse_count == 0)
			{
				S1_work_state = SFinish;
				HAL_TIM_PWM_Stop_IT(&htim4,TIM_CHANNEL_1);
			}
		}
	}
	if(htim->Instance == TIM5)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			Slide2_pulse_count--;
			if(Slide2_pulse_count == 0)
			{
				S2_work_state = SFinish;
				HAL_TIM_PWM_Stop_IT(&htim5,TIM_CHANNEL_1);
			}
		}
	}
	if(htim->Instance == TIM8)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			Slide3_pulse_count--;
			if(Slide3_pulse_count == 0)
			{
				S3_work_state = SFinish;
				HAL_TIM_PWM_Stop_IT(&htim8,TIM_CHANNEL_2);
			}
		}
	}
	if(htim->Instance == TIM20)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			Slide4_pulse_count--;
			if(Slide4_pulse_count == 0)
			{
				S4_work_state = SFinish;
				HAL_TIM_PWM_Stop_IT(&htim20,TIM_CHANNEL_1);
			}
		}
	}
}




