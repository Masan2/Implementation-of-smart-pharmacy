#ifndef _SLIDE_MOTOR_H_
#define _SLIDE_MOTOR_H_

#include "main.h"
#include "string.h"
/*********************************电机驱动引脚配置**********************************/
//X
#define EN_X_PORT			GPIOA
#define EN_X_PIN			GPIO_PIN_12

#define DIR_X_PORT		GPIOC
#define DIR_X_PIN			GPIO_PIN_8

#define PWM_X_PORT		GPIOC
#define PWM_X_PIN			GPIO_PIN_0
/**********************************电机驱动引脚使能配置*****************************/
#define EN_X_ON()			HAL_GPIO_WritePin(EN_X_PORT,EN_X_PIN,GPIO_PIN_SET);
#define EN_X_OFF()	  HAL_GPIO_WritePin(EN_X_PORT,EN_X_PIN,GPIO_PIN_RESET);
#define DIR_X_ON()		HAL_GPIO_WritePin(DIR_X_PORT,DIR_X_PIN,GPIO_PIN_RESET);
#define DIR_X_OFF()		HAL_GPIO_WritePin(DIR_X_PORT,DIR_X_PIN,GPIO_PIN_SET);

enum WORK_DIR
{
	Left_shift_5 = 1,
	Left_shift_10 = 2,
	Left_shift_15 = 3,
	Right_shift_5 = 4,
	Right_shift_10 = 5,
	Right_shift_15 = 6,
};
enum WORK_STATE
{
	Working = 0,
	Finish,
};
enum DIRECTION
{
	 Right= 0,
	 Left = 1,
};
struct RX_ORDER
{
	uint8_t flag;
	uint8_t park_number;
	char order_buff[4];
};
void Client_Control_Slide(uint8_t park_number);
void Slide_Work(uint32_t x_pulse_count,uint8_t direction);
void Slide_X_Right(void);
void Slide_X_Left(void);
void Slide_Y_Up(void);
void Slide_Y_Down(void);

void Key_Scan(void);
#endif


