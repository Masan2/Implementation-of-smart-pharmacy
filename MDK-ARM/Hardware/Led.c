//��Ƭ��ͷ�ļ�
#include "stm32g4xx.h"                  // Device header


//Ӳ������
#include "Led.h"


Led_INFO Led_info = {0};




/*
************************************************************
*	�������ƣ�	Led_Set
*
*	�������ܣ�	���ݿ���
*
*	��ڲ�����	status�����ط�����
*
*	���ز�����	��
*
*	˵����		��-Led_ON		��-Led_OFF
************************************************************
*/
void Led_Set(_Bool status)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,status == Led_ON ? GPIO_PIN_SET : GPIO_PIN_RESET);
	Led_info.Led_Status = status;
	
}
