//单片机头文件
#include "stm32g4xx.h"                  // Device header


//硬件驱动
#include "Led.h"


Led_INFO Led_info = {0};




/*
************************************************************
*	函数名称：	Led_Set
*
*	函数功能：	灯泡控制
*
*	入口参数：	status：开关蜂鸣器
*
*	返回参数：	无
*
*	说明：		开-Led_ON		关-Led_OFF
************************************************************
*/
void Led_Set(_Bool status)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,status == Led_ON ? GPIO_PIN_SET : GPIO_PIN_RESET);
	Led_info.Led_Status = status;
	
}
