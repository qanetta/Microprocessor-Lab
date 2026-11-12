/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "scheduler.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// LED noi keo xuong (+3.3V -> R -> LED -> chan MCU): chan = 0 thi LED sang
#define LED_ON		GPIO_PIN_RESET
#define LED_OFF		GPIO_PIN_SET

// Task in timestamp moi 10 ms chi chay trong 1 s dau roi bi xoa (demo SCH_Delete_Task,
// tranh 100 dong/giay lam tran Virtual Terminal)
#define PRINT_10MS_DURATION_MS	1000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
static uint32_t id_print_10ms = NO_TASK_ID;		// giu ID de xoa task sau nay
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// Chuyen printf ra USART1 (syscalls.c: _write -> __io_putchar tung ky tu)
int __io_putchar(int ch){
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 10);
	return ch;
}

/* ---------- 5 task tuan hoan (yeu cau chinh cua lab) ---------- */
// Dao LED roi in: task nao dang chay, tai thoi diem nao, LED sang hay tat
static void led_task(GPIO_TypeDef *port, uint16_t pin, const char *name){
	HAL_GPIO_TogglePin(port, pin);
	printf("[%-5s] t = %5lu ms -> LED %s\r\n", name, SCH_Get_Time(),
	       (HAL_GPIO_ReadPin(port, pin) == LED_ON) ? "sang" : "tat");
}
static void task_led_05s(void){ led_task(LED_05S_GPIO_Port, LED_05S_Pin, "0.5s"); }
static void task_led_1s(void)  { led_task(LED_1S_GPIO_Port,  LED_1S_Pin,  "1s");   }
static void task_led_15s(void){ led_task(LED_15S_GPIO_Port, LED_15S_Pin, "1.5s"); }
static void task_led_2s(void)  { led_task(LED_2S_GPIO_Port,  LED_2S_Pin,  "2s");   }
static void task_led_25s(void){ led_task(LED_25S_GPIO_Port, LED_25S_Pin, "2.5s"); }

/* ---------- Demo theo muc Demonstration cua de ---------- */
// Callback moi 10 ms: in gia tri get_time
static void task_print_10ms(void){
	printf("[10ms ] t = %5lu ms\r\n", SCH_Get_Time());
}

// Timeout 500 ms chay song song voi task 10 ms; LED_DEBUG lam nhip tim
static void task_print_500ms(void){
	HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
	printf("[500ms] t = %5lu ms\r\n", SCH_Get_Time());
}

// One-shot: xoa task 10 ms theo ID (demo SCH_Delete_Task)
static void oneshot_stop_10ms(void){
	uint8_t ok = SCH_Delete_Task(id_print_10ms);
	printf("[one-shot] t = %5lu ms: xoa task 10ms (ID %lu) -> %s\r\n",
	       SCH_Get_Time(), id_print_10ms, ok ? "OK" : "KHONG TIM THAY");
}

// One-shot them truoc vong while: in timestamp khi het han.
// B1 va B2 cung hen 3000 ms, B1 them truoc -> phai in truoc (FIFO)
static void oneshot_A(void) { printf("[one-shot A ] t = %5lu ms (hen 2000 ms)\r\n", SCH_Get_Time()); }
static void oneshot_B1(void){ printf("[one-shot B1] t = %5lu ms (hen 3000 ms, them truoc B2)\r\n", SCH_Get_Time()); }
static void oneshot_B2(void){ printf("[one-shot B2] t = %5lu ms (hen 3000 ms, them sau B1)\r\n", SCH_Get_Time()); }
static void oneshot_C(void) { printf("[one-shot C ] t = %5lu ms (hen 4500 ms)\r\n", SCH_Get_Time()); }
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
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  setvbuf(stdout, NULL, _IONBF, 0);		// tat bo dem stdout: in ra ngay, khong doi '\n'
  printf("\r\n=== Lab 4: Cooperative scheduler, tick %d ms ===\r\n", SCH_TICK_MS);

  SCH_Init();

  // 5 task tuan hoan 0.5 s, 1 s, 1.5 s, 2 s, 2.5 s (don vi tick, SCH_MS doi ms -> tick)
  SCH_Add_Task(task_led_05s, SCH_MS(500),  SCH_MS(500));
  SCH_Add_Task(task_led_1s,  SCH_MS(1000), SCH_MS(1000));
  SCH_Add_Task(task_led_15s, SCH_MS(1500), SCH_MS(1500));
  SCH_Add_Task(task_led_2s,  SCH_MS(2000), SCH_MS(2000));
  SCH_Add_Task(task_led_25s, SCH_MS(2500), SCH_MS(2500));

  // Demo: callback moi 10 ms + timeout 500 ms chay song song
  id_print_10ms = SCH_Add_Task(task_print_10ms, 1, 1);
  SCH_Add_Task(task_print_500ms, SCH_MS(500), SCH_MS(500));
  SCH_Add_Task(oneshot_stop_10ms, SCH_MS(PRINT_10MS_DURATION_MS), 0);

  // One-shot: delay du dai de vong while chay truoc khi chung het han
  SCH_Add_Task(oneshot_A,  SCH_MS(2000), 0);
  SCH_Add_Task(oneshot_B1, SCH_MS(3000), 0);
  SCH_Add_Task(oneshot_B2, SCH_MS(3000), 0);
  SCH_Add_Task(oneshot_C,  SCH_MS(4500), 0);

  HAL_TIM_Base_Start_IT(&htim2);		// bat tick sau khi da them du task
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  SCH_Dispatch_Tasks();
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED_05S_Pin|LED_1S_Pin|LED_15S_Pin|LED_2S_Pin
                          |LED_25S_Pin|LED_DEBUG_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : LED_05S_Pin LED_1S_Pin LED_15S_Pin LED_2S_Pin
                           LED_25S_Pin LED_DEBUG_Pin */
  GPIO_InitStruct.Pin = LED_05S_Pin|LED_1S_Pin|LED_15S_Pin|LED_2S_Pin
                          |LED_25S_Pin|LED_DEBUG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
// ISR chi goi SCH_Update (O(1)); moi task chay o vong while qua SCH_Dispatch_Tasks
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (htim->Instance == TIM2){
		SCH_Update();
	}
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
