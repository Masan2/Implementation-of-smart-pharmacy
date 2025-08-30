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
#define FRAME_HEADER 0x5A         // 定义帧头为0x5A
#define MAX_FRAME_LENGTH 512      // 定义数据帧的最大长度

uint8_t uart_buff[MAX_FRAME_LENGTH] = {0};
uint8_t uart_char;
uint8_t receive_num = 0;   
uint8_t receiving_frame = 0;      // 标记是否在接收数据帧
uint16_t expected_length = 0;     // 期望接收到的数据帧长度

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//wifi模块
uint32_t send_time = 0;
unsigned char *dataPtr = NULL;
extern uint8_t Uart1_RxData;
extern uint8_t rx_flag;
//出药模块
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
	
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,500);//不影响RFID
	HAL_UART_Receive_IT(&huart1,(uint8_t *)&Uart1_RxData, 1);//开启串口中断   本句不影响RFID
	HAL_UART_Receive_IT(&huart4,(uint8_t *)&uart_char,1);
	DH11_Task();
	voice_uart_printf(louder);
	HAL_Delay(1000);
	voice_uart_printf(louder);
	HAL_Delay(1000);
	voice_uart_printf(Welcome_voice);//播放欢迎语音
	
	RFID_Init();
	UsartPrintf(DEBUG_UART, "Info: RFID init success\r\n");
	
	//履带电机驱动
	Track_Init();
	UsartPrintf(DEBUG_UART, "Info: track init success\r\n");
	//语音播报：系统链接网络中。。。。。
	HAL_Delay(5000);
	voice_uart_printf(Link_network);
	ESP8266_Init();	//本句影响RFID,连上wifi模块不影响RFID
	while(OneNet_DevLink());//本句不影响RFID
	HAL_Delay(1000);
	OneNET_Subscribe();
	
	//语音播报：链接网络完成
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
		/*检查库存*/
		Warehouse_Management();

		/*上传温湿度信息	给网关*/
		if(uwTick - send_time > 5000)
		{
			DH11_Task();//本句影响RFID
			OneNet_SendData();
			send_time = uwTick;
			ESP8266_Clear();
		}
		
		/*获取网关指令*/
		dataPtr = ESP8266_GetIPD(0);
		if(dataPtr != NULL)	
				OneNet_RevPro(dataPtr);
		  
		/*出药*/
		medicine_pro();                                               
		/*设定时间处理函数*/
		Timed_task(time.Hours,time.Minutes);
		
		/*药方识别处理*/
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
            receiving_frame = 1;   // 标记开始接收数据帧
            receive_num = 0;       // 重置接收计数
            expected_length = 0;   // 重置期望长度
            uart_buff[receive_num++] = uart_char; // 将帧头存入缓冲区
        } else if (receiving_frame) {
            uart_buff[receive_num++] = uart_char; // 存储接收到的字节

            // 确定帧的长度，第六七字节表示帧的长度
					if (receive_num == 7) {
                expected_length = (uart_buff[5] << 8) | uart_buff[6];  // 第六七字节表示帧的长度
            }

            // 检查是否接收到完整帧
            if ((expected_length > 0) && (receive_num == expected_length + 7)) {
//                process_frame(uart_buff, receive_num); // 处理完整的数据帧
                receiving_frame = 0;   // 重置接收标记
                receive_num = 0;       // 重置接收计数
                expected_length = 0;   // 重置期望长度
								if(uart_buff[17]==0x6F&&uart_buff[18]==0xE7&&uart_buff[19]==0x3D&&uart_buff[20]==0x70){
									
								}
								else if(uart_buff[37]==0x50 && uart_buff[40]==0xC2 && uart_buff[41]==0x11)
								{
									
									/*消食片仓储剩余量*/
									exist_flag_A10 = 1;
								}
								else if(uart_buff[37]==0x40 && uart_buff[40]==0xC2 && uart_buff[41]==0x0D)
								{
									
									/*消食片仓储剩余量*/
									exist_flag_A9 = 1;
								}
								else if(uart_buff[37]==0x40 && uart_buff[40]==0xC1 && uart_buff[41]==0xFD)
								{
									
										/*藿香正气胶囊仓储剩余量*/
									exist_flag_A8 = 1;
								}
								else if(uart_buff[37]==0x50 && uart_buff[40]==0xC2 && uart_buff[41]==0x09)
								{
									
									
									/*藿香正气胶囊仓储剩余量*/
									exist_flag_A7 = 1;
								}
								else if(uart_buff[37]==0x50 && uart_buff[40]==0xC2 && uart_buff[41]==0x05)
								{
									
									/*维生素B仓储剩余量*/
									exist_flag_A6 = 1;
								}
								else if(uart_buff[37]==0x40 && uart_buff[40]==0xC2 && uart_buff[41]==0x01)
								{
									
									/*维生素B仓储剩余量*/
									exist_flag_A5 = 1;
								}
								else if(uart_buff[37]==0x40 && uart_buff[40]==0xC2 && uart_buff[41]==0x15)
								{
									
									/*胃康灵仓储剩余量*/
									exist_flag_A4 = 1;
								}
								else if(uart_buff[37]==0x40 && uart_buff[40]==0xC2 && uart_buff[41]==0x19)
								{
									
									/*胃康灵仓储剩余量*/
									exist_flag_A3 = 1;
								}
								else if(uart_buff[37]==0x50 && uart_buff[40]==0xC1 && uart_buff[41]==0xF9)
								{
									/*可用做人员识别*/
									
									exist_flag_A2 = 1;
								}
								else if(uart_buff[37]==0x50 && uart_buff[40]==0xC1 && uart_buff[41]==0xF5)
								{
									/*可用做人员识别*/
									
									exist_flag_A1 = 1;
								}
            }
        }

        // 重新启动UART中断接收
        HAL_UART_Receive_IT(&huart4, &uart_char, 1);
    }
}


/*履带初始化*/
void Track_Init(void)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);//改变方向
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,250);//改变速度
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
