/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "rtc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "esp8266.h"               
#include "onenet.h"
#include "dht11.h"
#include "voice.h"
#include "motor.h"
#include "task.h"
#include "string.h"
#include "stdio.h"
#include "warehouse.h"
#include "Small_slide_move.h"
#include "display.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define FRAME_HEADER 0x5A         // ����֡ͷΪ0x5A
#define MAX_FRAME_LENGTH 512      // ��������֡����󳤶�

uint8_t uart_buff[MAX_FRAME_LENGTH] = {0};
uint8_t uart_char;
uint8_t receive_num = 0;   
uint8_t receiving_frame = 0;      // ����Ƿ��ڽ�������֡
uint16_t expected_length = 0;     // �������յ�������֡����

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//wifiģ��
uint32_t send_time = 0;
unsigned char *dataPtr = NULL;
extern uint8_t Uart1_RxData;
extern uint8_t rx_flag;
//��ҩģ��
uint32_t med_tick = 0;

//RTC
RTC_DateTypeDef date;
RTC_TimeTypeDef time;

uint8_t Send_buff[20];
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

void RFID_pro(void);
void Track_Init(void);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_LPUART1_UART_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART3_UART_Init();
  MX_RTC_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM8_Init();
  MX_TIM20_Init();
  /* USER CODE BEGIN 2 */
	
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,500);//��Ӱ��RFID
	HAL_UART_Receive_IT(&huart1,(uint8_t *)&Uart1_RxData, 1);//���������ж�   ���䲻Ӱ��RFID
	HAL_UART_Receive_IT(&huart4,(uint8_t *)&uart_char,1);
	DH11_Task();
	voice_uart_printf(louder);
	HAL_Delay(1000);
	voice_uart_printf(louder);
	HAL_Delay(1000);
	voice_uart_printf(Welcome_voice);//���Ż�ӭ����
	
	RFID_Init();
	UsartPrintf(DEBUG_UART, "Info: RFID init success\r\n");
	
	//�Ĵ��������
	Track_Init();
	UsartPrintf(DEBUG_UART, "Info: track init success\r\n");
	//����������ϵͳ���������С���������
	HAL_Delay(5000);
	voice_uart_printf(Link_network);
	ESP8266_Init();	//����Ӱ��RFID,����wifiģ�鲻Ӱ��RFID
	while(OneNet_DevLink());//���䲻Ӱ��RFID
	HAL_Delay(1000);
	OneNET_Subscribe();
	
	//���������������������
	HAL_Delay(2000);
	voice_uart_printf(Link_network_compete);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	

  while (1)
  {
//		SmallSlide_pro(SLIDE3);
		HAL_RTC_GetTime(&hrtc,&time,RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc,&date,RTC_FORMAT_BIN);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		/*�����*/
		Warehouse_Management();

		/*�ϴ���ʪ����Ϣ	������*/
		if(uwTick - send_time > 5000)
		{
			DH11_Task();//����Ӱ��RFID
			OneNet_SendData();
			send_time = uwTick;
			ESP8266_Clear();
		}
		
		/*��ȡ����ָ��*/
		dataPtr = ESP8266_GetIPD(0);
		if(dataPtr != NULL)	
				OneNet_RevPro(dataPtr);
		  
		/*��ҩ*/
		medicine_pro();                                               
		/*�趨ʱ�䴦����*/
		Timed_task(time.Hours,time.Minutes);
		
		/*ҩ��ʶ����*/
		Prescription_task();
	}    
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV2;
  RCC_OscInitStruct.PLL.PLLN = 25;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == UART4) {
        if (uart_char == FRAME_HEADER && !receiving_frame) {
            receiving_frame = 1;   // ��ǿ�ʼ��������֡
            receive_num = 0;       // ���ý��ռ���
            expected_length = 0;   // ������������
            uart_buff[receive_num++] = uart_char; // ��֡ͷ���뻺����
        } else if (receiving_frame) {
            uart_buff[receive_num++] = uart_char; // �洢���յ����ֽ�

            // ȷ��֡�ĳ��ȣ��������ֽڱ�ʾ֡�ĳ���
					if (receive_num == 7) {
                expected_length = (uart_buff[5] << 8) | uart_buff[6];  // �������ֽڱ�ʾ֡�ĳ���
            }

            // ����Ƿ���յ�����֡
            if ((expected_length > 0) && (receive_num == expected_length + 7)) {
//                process_frame(uart_buff, receive_num); // ��������������֡
                receiving_frame = 0;   // ���ý��ձ��
                receive_num = 0;       // ���ý��ռ���
                expected_length = 0;   // ������������
								if(uart_buff[17]==0x6F&&uart_buff[18]==0xE7&&uart_buff[19]==0x3D&&uart_buff[20]==0x70){
									
								}
								else if(uart_buff[37]==0x50 && uart_buff[40]==0xC2 && uart_buff[41]==0x11)
								{
									
									/*��ʳƬ�ִ�ʣ����*/
									exist_flag_A10 = 1;
								}
								else if(uart_buff[37]==0x40 && uart_buff[40]==0xC2 && uart_buff[41]==0x0D)
								{
									
									/*��ʳƬ�ִ�ʣ����*/
									exist_flag_A9 = 1;
								}
								else if(uart_buff[37]==0x40 && uart_buff[40]==0xC1 && uart_buff[41]==0xFD)
								{
									
										/*޽���������Ҳִ�ʣ����*/
									exist_flag_A8 = 1;
								}
								else if(uart_buff[37]==0x50 && uart_buff[40]==0xC2 && uart_buff[41]==0x09)
								{
									
									
									/*޽���������Ҳִ�ʣ����*/
									exist_flag_A7 = 1;
								}
								else if(uart_buff[37]==0x50 && uart_buff[40]==0xC2 && uart_buff[41]==0x05)
								{
									
									/*ά����B�ִ�ʣ����*/
									exist_flag_A6 = 1;
								}
								else if(uart_buff[37]==0x40 && uart_buff[40]==0xC2 && uart_buff[41]==0x01)
								{
									
									/*ά����B�ִ�ʣ����*/
									exist_flag_A5 = 1;
								}
								else if(uart_buff[37]==0x40 && uart_buff[40]==0xC2 && uart_buff[41]==0x15)
								{
									
									/*θ����ִ�ʣ����*/
									exist_flag_A4 = 1;
								}
								else if(uart_buff[37]==0x40 && uart_buff[40]==0xC2 && uart_buff[41]==0x19)
								{
									
									/*θ����ִ�ʣ����*/
									exist_flag_A3 = 1;
								}
								else if(uart_buff[37]==0x50 && uart_buff[40]==0xC1 && uart_buff[41]==0xF9)
								{
									/*��������Աʶ��*/
									
									exist_flag_A2 = 1;
								}
								else if(uart_buff[37]==0x50 && uart_buff[40]==0xC1 && uart_buff[41]==0xF5)
								{
									/*��������Աʶ��*/
									
									exist_flag_A1 = 1;
								}
            }
        }

        // ��������UART�жϽ���
        HAL_UART_Receive_IT(&huart4, &uart_char, 1);
    }
}


/*�Ĵ���ʼ��*/
void Track_Init(void)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);//�ı䷽��
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,250);//�ı��ٶ�
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
