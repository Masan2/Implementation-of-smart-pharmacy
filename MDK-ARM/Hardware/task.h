#ifndef _TASK_H
#define _TASK_H
#include "stm32g4xx.h"                  // Device header


extern uint32_t medicine_num; //��Ӧҩ��λ�ã�1��������ߣ�2������ߵڶ���3������ߵ�����4������ߵ���,0������ҩ
extern uint8_t position_num ;
extern uint8_t flag_complete;

void medicine_pro(void);
void Prescription_task(void);
void Timed_task(int hour,int minute);
void Small_slide_move(uint8_t position_num,uint16_t Med_num);
void caterpillar_band_move(void);
#endif



