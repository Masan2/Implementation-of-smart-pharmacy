#include "stm32g4xx.h"                  // Device header
#include "main.h"
#include "usart.h"
#include "display.h"
#include "stdio.h"
#include "string.h"
#include "dht11.h"
#include "warehouse.h"


uint8_t disp_buf[50];

void Page_turn(uint8_t page_num)
{
	switch(page_num)
	{
		/*开机界面*/
		case page_start:
		{
			sprintf((char *)disp_buf,"page start\xff\xff\xff");
			HAL_UART_Transmit(&huart3,disp_buf,strlen(disp_buf),50);
		}break;
		
		/*main界面*/
		case page_main:
		{
			sprintf((char *)disp_buf,"page main\xff\xff\xff");
			HAL_UART_Transmit(&huart3,disp_buf,strlen(disp_buf),50);
	
		}break;
		
		/*总仓储界面*/
		case page_index:
		{
			sprintf((char *)disp_buf,"page index\xff\xff\xff");
			HAL_UART_Transmit(&huart3,disp_buf,strlen(disp_buf),50);
			
		}break;
		
		/*仓库一界面*/
		case page_1:
		{
			sprintf((char *)disp_buf,"page num1\xff\xff\xff");
			HAL_UART_Transmit(&huart3,disp_buf,strlen(disp_buf),50);
			Send_message();
			sprintf((char *)disp_buf,"n0.val=%d\xff\xff\xff",Digestion_tablet_num);
			HAL_UART_Transmit(&huart3,disp_buf,strlen(disp_buf),50);
		}break;
		
		/*仓库二界面*/
		case page_2:
		{
			sprintf((char *)disp_buf,"page num2\xff\xff\xff");
			HAL_UART_Transmit(&huart3,disp_buf,strlen(disp_buf),50);
			Send_message();
			sprintf((char *)disp_buf,"n0.val=%d\xff\xff\xff",HXZQ_capsule_num);
			HAL_UART_Transmit(&huart3,disp_buf,strlen(disp_buf),50);
		}break;
		
		/*仓库三界面*/
		case page_3:
		{
			sprintf((char *)disp_buf,"page num3\xff\xff\xff");
			HAL_UART_Transmit(&huart3,disp_buf,strlen(disp_buf),50);
			Send_message();
			sprintf((char *)disp_buf,"n0.val=%d\xff\xff\xff",Vitamin_B_num);
			HAL_UART_Transmit(&huart3,disp_buf,strlen(disp_buf),50);
		}break;
		
		/*仓库四界面*/
		case page_4:
		{
			sprintf((char *)disp_buf,"page num4\xff\xff\xff");
			HAL_UART_Transmit(&huart3,disp_buf,strlen(disp_buf),50);
			Send_message();
			sprintf((char *)disp_buf,"n0.val=%d\xff\xff\xff",Stomach_recovering_num);
			HAL_UART_Transmit(&huart3,disp_buf,strlen(disp_buf),50);
		}break;
		
		
		default:break;
	}
}

void Send_message(void)
{
	sprintf((char *)disp_buf,"n1.val=%d\xff\xff\xff",23);
	HAL_UART_Transmit(&huart3,disp_buf,strlen(disp_buf),50);
	sprintf((char *)disp_buf,"n2.val=%d\xff\xff\xff",61);
	HAL_UART_Transmit(&huart3,disp_buf,strlen(disp_buf),50);
}

void Page_num6(void)//链接网络中
{
	sprintf((char *)disp_buf,"page num6\xff\xff\xff");
	HAL_UART_Transmit(&huart3,disp_buf,strlen(disp_buf),50);
}
void Page_num7(void)//链接网络完成
{
	sprintf((char *)disp_buf,"page num7\xff\xff\xff");
	HAL_UART_Transmit(&huart3,disp_buf,strlen(disp_buf),50);
}

