#ifndef _WAREHOUSE_H
#define _WAREHOUSE_H
#include "main.h"



extern uint8_t exist_flag_A10;//��ʳƬ��ǩ
extern uint8_t exist_flag_A9;//��ʳƬ��ǩ                                                           

extern uint8_t exist_flag_A8;//޽���������ұ�ǩ
extern uint8_t exist_flag_A7;//޽���������ұ�ǩ

extern uint8_t exist_flag_A6;//ά����B��ǩ
extern uint8_t exist_flag_A5;//ά����B��ǩ

extern uint8_t exist_flag_A4;//θ�����ǩ
extern uint8_t exist_flag_A3;//θ�����ǩ

extern uint8_t jian_flag_A10;//��ʳƬ����һ��
extern uint8_t jian_flag_A9;//��ʳƬ����һ��

extern uint8_t jian_flag_A8;//޽���������ҿ���һ��
extern uint8_t jian_flag_A7;//޽���������ҿ���һ��

extern uint8_t jian_flag_A6;//ά����B����һ��
extern uint8_t jian_flag_A5;//ά����B����һ��

extern uint8_t jian_flag_A4;//θ�������һ��
extern uint8_t jian_flag_A3;//θ�������һ��

extern uint8_t exist_flag_A2;//ҩ��
extern uint8_t exist_flag_A1;//ҩ��

extern uint8_t Digestion_tablet_num;//��ʳƬ�����
extern uint8_t HXZQ_capsule_num;//޽���������ҿ����
extern uint8_t Vitamin_B_num;//ά����B�����
extern uint8_t Stomach_recovering_num;//θ��������

extern uint8_t jia_flag_A10;//��ʳƬ����һ��
extern uint8_t jia_flag_A9;
extern uint8_t jia_flag_A8;//޽���������ҿ���һ��
extern uint8_t jia_flag_A7;
extern uint8_t jia_flag_A6;//ά����B����һ��
extern uint8_t jia_flag_A5;
extern uint8_t jia_flag_A4;//θ�������һ��
extern uint8_t jia_flag_A3;

void Warehouse_Management(void);
void RFID_pro(void);
void RFID_Init(void);



#endif





