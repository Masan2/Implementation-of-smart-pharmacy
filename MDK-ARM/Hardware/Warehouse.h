#ifndef _WAREHOUSE_H
#define _WAREHOUSE_H
#include "main.h"



extern uint8_t exist_flag_A10;//消食片标签
extern uint8_t exist_flag_A9;//消食片标签                                                           

extern uint8_t exist_flag_A8;//藿香正气胶囊标签
extern uint8_t exist_flag_A7;//藿香正气胶囊标签

extern uint8_t exist_flag_A6;//维生素B标签
extern uint8_t exist_flag_A5;//维生素B标签

extern uint8_t exist_flag_A4;//胃康灵标签
extern uint8_t exist_flag_A3;//胃康灵标签

extern uint8_t jian_flag_A10;//消食片库存减一次
extern uint8_t jian_flag_A9;//消食片库存减一次

extern uint8_t jian_flag_A8;//藿香正气胶囊库存减一次
extern uint8_t jian_flag_A7;//藿香正气胶囊库存减一次

extern uint8_t jian_flag_A6;//维生素B库存减一次
extern uint8_t jian_flag_A5;//维生素B库存减一次

extern uint8_t jian_flag_A4;//胃康灵库存减一次
extern uint8_t jian_flag_A3;//胃康灵库存减一次

extern uint8_t exist_flag_A2;//药方
extern uint8_t exist_flag_A1;//药方

extern uint8_t Digestion_tablet_num;//消食片库存量
extern uint8_t HXZQ_capsule_num;//藿香正气胶囊库存量
extern uint8_t Vitamin_B_num;//维生素B库存量
extern uint8_t Stomach_recovering_num;//胃康灵库存量

extern uint8_t jia_flag_A10;//消食片库存加一次
extern uint8_t jia_flag_A9;
extern uint8_t jia_flag_A8;//藿香正气胶囊库存加一次
extern uint8_t jia_flag_A7;
extern uint8_t jia_flag_A6;//维生素B库存加一次
extern uint8_t jia_flag_A5;
extern uint8_t jia_flag_A4;//胃康灵库存加一次
extern uint8_t jia_flag_A3;

void Warehouse_Management(void);
void RFID_pro(void);
void RFID_Init(void);



#endif





