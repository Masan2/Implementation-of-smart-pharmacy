#include "stm32g4xx.h"                  // Device header
#include "esp8266.h"
#include "usart.h"
#include "string.h"
#include "Warehouse.h"




uint8_t exist_flag_A10 = 0;//消食片标签
uint8_t exist_flag_A9 = 0;//消食片标签

uint8_t exist_flag_A8 = 0;//维生素C标签
uint8_t exist_flag_A7 = 0;//维生素C标签

uint8_t exist_flag_A6 = 0;//维生素B标签
uint8_t exist_flag_A5 = 0;//维生素B标签

uint8_t exist_flag_A4 = 0;//板蓝根标签
uint8_t exist_flag_A3 = 0;//板蓝根标签

uint8_t exist_flag_A2 = 0;//药方
uint8_t exist_flag_A1 = 0;//药方

uint8_t jian_flag_A10 = 0;//消食片库存减一次
uint8_t jian_flag_A9 = 0;
uint8_t jian_flag_A8 = 0;//藿香正气胶囊库存减一次
uint8_t jian_flag_A7 = 0;
uint8_t jian_flag_A6 = 0;//维生素B库存减一次
uint8_t jian_flag_A5 = 0;
uint8_t jian_flag_A4 = 0;//胃康灵库存减一次
uint8_t jian_flag_A3 = 0;

uint8_t jia_flag_A10 = 0;//消食片库存加一次
uint8_t jia_flag_A9 = 0;
uint8_t jia_flag_A8 = 0;//藿香正气胶囊库存加一次
uint8_t jia_flag_A7 = 0;
uint8_t jia_flag_A6 = 0;//维生素B库存加一次
uint8_t jia_flag_A5 = 0;
uint8_t jia_flag_A4 = 0;//胃康灵库存加一次
uint8_t jia_flag_A3 = 0;


uint8_t Digestion_tablet_num = 0;//消食片库存量
uint8_t HXZQ_capsule_num = 0;//藿香正气胶囊库存量
uint8_t Vitamin_B_num = 0;//维生素B库存量
uint8_t Stomach_recovering_num = 0;//胃康灵库存量

uint32_t Refresh_time = 0;

void Warehouse_Management(void)
{
//	if(uwTick - Refresh_time > 5000)
//	{
		/*计算消食片库存*/
		if(exist_flag_A10 == 1 && jia_flag_A10 == 0)
		{
			Digestion_tablet_num++;
			jia_flag_A10 = 1;
		}
		else if(exist_flag_A9 == 1 && jia_flag_A9 == 0)
		{
			Digestion_tablet_num++;
			jia_flag_A9 = 1;
		}
		/*计算藿香正气胶囊库存*/
		else if(exist_flag_A8 == 1 && jia_flag_A8 == 0)
		{
			HXZQ_capsule_num++;
			jia_flag_A8 = 1;
		}
		else if(exist_flag_A7 == 1 && jia_flag_A7 == 0)
		{
			HXZQ_capsule_num++;
			jia_flag_A7 = 1;
		}
		/*计算维生素B库存*/
		else if(exist_flag_A6 == 1 && jia_flag_A6 == 0)
		{
			Vitamin_B_num++;
			jia_flag_A6 = 1;  
		}
		else if(exist_flag_A5 == 1 && jia_flag_A5 == 0)
		{
			Vitamin_B_num++;
			jia_flag_A5 = 1;
		}
		/*计算胃康灵库存*/
		else if(exist_flag_A4 == 1 && jia_flag_A4 == 0)
		{
			Stomach_recovering_num++;
			jia_flag_A4 = 1;  
		}
		else if(exist_flag_A3 == 1 && jia_flag_A3 == 0)
		{
			Stomach_recovering_num++;
			jia_flag_A3 = 1; 
		}
		
		
//		/*计算消食片库存*/
//		if(Digestion_tablet_num != 0)
//		{
//			if(exist_flag_A10 == 0 && jian_flag_A10 == 0)
//			{
//				Digestion_tablet_num--;
//				jian_flag_A10 = 1;
//			}
////			else if(exist_flag_A9 == 0 && jian_flag_A9 == 0)
////			{
////				Digestion_tablet_num--;
////				jian_flag_A9 = 1;
////			}
//		}
//		
//		/*计算藿香正气胶囊库存*/
//		if(HXZQ_capsule_num != 0)
//		{
//			if(exist_flag_A8 == 0 && jian_flag_A8 == 0)
//			{
//				HXZQ_capsule_num--;
//				jian_flag_A8 = 1;
//			}
////			else if(exist_flag_A7 == 0 && jian_flag_A7 == 0)
////			{
////				HXZQ_capsule_num--;
////				jian_flag_A7 = 1;
////			}
//		}
//		
//		/*计算维生素B库存*/
//		if(Vitamin_B_num !=0 )
//		{
//			if(exist_flag_A6 == 0 && jian_flag_A6 == 0)
//			{
//				Vitamin_B_num--;
//				jian_flag_A6 = 1;
//			}
////			else if(exist_flag_A5 == 0 && jian_flag_A5 == 0)
////			{
////				Vitamin_B_num--;
////				jian_flag_A5 = 1;
////			}
//		}
//	
//		/*计算胃康灵库存*/
//		if(Stomach_recovering_num != 0)
//		{
//			if(exist_flag_A4 == 0 && jian_flag_A4 == 0)
//			{
//				Stomach_recovering_num--;
//				jian_flag_A4 = 1;  
//			}
////			else if(exist_flag_A3 == 0 && jian_flag_A3 == 0)
////			{
////				Stomach_recovering_num--;
////				jian_flag_A3 = 1; 
////			}
//		}
		
//		Refresh_time = uwTick;
////	}	
}

void RFID_Init(void)
{
	//5A 00 01 01 02 00 01 02 11 EA
	//更改波特率为9600
//	const uint8_t butell_cmd[10] = {0x5A,0x00,0x01,0x01,0x02,0x00,0x01,0x00,0x11,0xEA};
//	HAL_UART_Transmit(&huart4,butell_cmd,13,100);
	
//	HAL_Delay(5000);
//	MX_USART_UART_Init(&huart4,UART4,9600);
//	HAL_Delay(100);
	//通过串口发送读指令初始化RFID 5A 00 01 01 14 00 04 5A A5 A5 5A 81 83
//	const uint8_t chongqi_cmd[13] = {0x5A,0x00,0x01,0x01,0x14,0x00,0x04,0x5A,0xA5,0xA5,0x5A,0x81,0x83};
//	HAL_UART_Transmit(&huart4,chongqi_cmd,13,100);
//	
//	HAL_Delay(10000);//等待10sRFID重启 5A 00 01 02 10 00 08 00 00 00 AB 01 02 00 06 81 C2
	
	//5A 00 01 01 01 00 00 EB D5
	const uint8_t cmd1[] = {0x5A,0X00,0X01,0X01,0X01,0X00,0X00,0XEB,0XD5};
	HAL_UART_Transmit(&huart4,cmd1,9,100);
	HAL_Delay(2000);
	
	//[5A 00 01 02 00 00 00 47 39]
	const uint8_t cmd2[] = {0x5A,0X00,0X01,0X02,0X00,0X00,0X00,0X47,0X39};
	HAL_UART_Transmit(&huart4,cmd2,9,100);
	HAL_Delay(2000);
	
	//[5A 00 01 02 FF 00 00 88 5A]
	const uint8_t cmd3[] = {0x5A,0X00,0X01,0X02,0XFF,0X00,0X00,0X88,0X5A};
	HAL_UART_Transmit(&huart4,cmd3,9,100);
	HAL_Delay(2000);
	
	const uint8_t read_cmd[17] = {0x5A,0x00,0x01,0x02,0x10,0x00,0x08,0x00,0x00,0x00,0x01,0x01,0x02,0x00,0x06,0xED,0x08}; //待发送的16进制数组
	HAL_UART_Transmit(&huart4,read_cmd,17,100);
	HAL_Delay(2000);
	
}