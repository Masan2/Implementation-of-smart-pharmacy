#ifndef _TASK_H
#define _TASK_H
#include "stm32g4xx.h"                  // Device header


extern uint32_t medicine_num; //对应药仓位置，1代表最左边，2代表左边第二，3代表左边第三，4代表左边第四,0代表不出药
extern uint8_t position_num ;
extern uint8_t flag_complete;

void medicine_pro(void);
void Prescription_task(void);
void Timed_task(int hour,int minute);
void Small_slide_move(uint8_t position_num,uint16_t Med_num);
void caterpillar_band_move(void);
#endif



