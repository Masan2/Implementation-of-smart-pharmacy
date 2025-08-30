#ifndef _SMALL_SLIDE_MOVE_
#define _SMALL_SLIDE_MOVE_

#include "main.h"
#include "string.h"


/**********************************电机驱动引脚使能配置*****************************/
#define EN_1_OFF()		HAL_GPIO_WritePin(GPIOA,Slide_1_EN_Pin,GPIO_PIN_RESET);
#define EN_2_OFF()		HAL_GPIO_WritePin(GPIOC,Slide_2_EN_Pin,GPIO_PIN_RESET);
#define EN_3_OFF()		HAL_GPIO_WritePin(GPIOB,Slide_3_EN_Pin,GPIO_PIN_RESET);
#define EN_4_OFF()		HAL_GPIO_WritePin(GPIOB,Slide_4_EN_Pin,GPIO_PIN_RESET);


#define DIR_1_ON()		HAL_GPIO_WritePin(GPIOC,Slide_1_DIR_Pin,GPIO_PIN_RESET);
#define DIR_1_OFF()		HAL_GPIO_WritePin(GPIOC,Slide_1_DIR_Pin,GPIO_PIN_SET);

#define DIR_2_ON()		HAL_GPIO_WritePin(GPIOC,Slide_2_DIR_Pin,GPIO_PIN_SET);
#define DIR_2_OFF()		HAL_GPIO_WritePin(GPIOC,Slide_2_DIR_Pin,GPIO_PIN_RESET);

#define DIR_3_ON()		HAL_GPIO_WritePin(GPIOB,Slide_3_DIR_Pin,GPIO_PIN_SET);
#define DIR_3_OFF()		HAL_GPIO_WritePin(GPIOB,Slide_3_DIR_Pin,GPIO_PIN_RESET);

#define DIR_4_ON()		HAL_GPIO_WritePin(GPIOB,Slide_4_DIR_Pin,GPIO_PIN_RESET);
#define DIR_4_OFF()		HAL_GPIO_WritePin(GPIOB,Slide_4_DIR_Pin,GPIO_PIN_SET);

extern uint32_t Slide1_pulse_count,Slide2_pulse_count,Slide3_pulse_count,Slide4_pulse_count;

enum SLIDE_NUM
{
	SLIDE1 = 0,
	SLIDE2 = 1,
	SLIDE3 = 2,
	SLIDE4 = 3,
};

enum S_WORK_STATE
{
	SWorking = 0,
	SFinish = 1,
};

extern enum S_WORK_STATE swork_state;
extern enum S_WORK_STATE S1_work_state;
extern enum S_WORK_STATE S2_work_state;
extern enum S_WORK_STATE S3_work_state;
extern enum S_WORK_STATE S4_work_state;
//typedef enum { SFinish, SWorking } S_WORK_STATE;
//typedef enum { Left, Right } Direction;
//typedef enum { SLIDE1, SLIDE2, SLIDE3, SLIDE4 } SlideNumber;
//extern S_WORK_STATE swork_state;

void Slide_1_Right(void);
void Slide_1_Left(void);

void Slide_2_Right(void);
void Slide_2_Left(void);

void Slide_3_Right(void);
void Slide_3_Left(void);

void Slide_4_Right(void);
void Slide_4_Left(void);

void Small_slide_Work(uint32_t x_pulse_count,uint8_t direction,uint8_t Slide_num);
void Small_slide(uint8_t work_dir,uint8_t Slide_num);
void SmallSlide_pro(uint8_t Slide_num);


void DIR_ON(uint8_t Slide_num);
void DIR_OFF(uint8_t Slide_num);
void EN_OFF(uint8_t Slide_num);
void Slide_Left(uint8_t Slide_num);
void Slide_Right(uint8_t Slide_num);
//void Small_slide_Work(uint32_t x_pulse_count, Direction direction, uint8_t Slide_num);

void Slide_4_move(uint32_t pulse_count,uint8_t direction);
void Slide_3_move(uint32_t pulse_count,uint8_t direction);
void Slide_2_move(uint32_t pulse_count,uint8_t direction);
void Slide_1_move(uint32_t pulse_count,uint8_t direction);
#endif
