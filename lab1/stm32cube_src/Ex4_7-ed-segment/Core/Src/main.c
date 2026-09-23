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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum{
	AUTO_RED_GREEN = 0, //r1 - g6
	AUTO_RED_YELLOW, // r1 - y5
	AUTO_GREEN_RED, // g3 - r4
	AUTO_YELLOW_RED // y2 - r4
}TrafficState;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void setTrafficLight1(int red, int yellow, int green) {
    HAL_GPIO_WritePin(RED_LED_1_GPIO_Port, RED_LED_1_Pin, red ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(YELLOW_LED_2_GPIO_Port, YELLOW_LED_2_Pin, yellow ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GREEN_LED_3_GPIO_Port, GREEN_LED_3_Pin, green ? GPIO_PIN_RESET : GPIO_PIN_SET);
}
// way
void setTrafficLight2(int red, int yellow, int green) {
    HAL_GPIO_WritePin(RED_LED_4_GPIO_Port, RED_LED_4_Pin, red ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(YELLOW_LED_5_GPIO_Port, YELLOW_LED_5_Pin, yellow ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GREEN_LED_6_GPIO_Port, GREEN_LED_6_Pin, green ? GPIO_PIN_RESET : GPIO_PIN_SET);
}
void display7SEG(int num) {
    // Mảng mã hóa cho LED 7 đoạn Anode chung (g f e d c b a)
    // 0: BẬT (RESET), 1: TẮT (SET)
    const uint8_t segment_patterns[10] = {
        0x3F, // 0: 0111111 -> sáng a,b,c,d,e,f
        0x06, // 1: 0000110 -> sáng b,c
        0x5B, // 2: 1011011 -> sáng a,b,d,e,g
        0x4F, // 3: 1001111 -> sáng a,b,c,d,g
        0x66, // 4: 1100110 -> sáng b,c,f,g
        0x6D, // 5: 1101101 -> sáng a,c,d,f,g
        0x7D, // 6: 1111101 -> sáng a,c,d,e,f,g
        0x07, // 7: 0000111 -> sáng a,b,c
        0x7F, // 8: 1111111 -> sáng a,b,c,d,e,f,g
        0x6F  // 9: 1101111 -> sáng a,b,c,d,f,g
    };

    if (num < 0 || num > 9) return;

    uint8_t pattern = segment_patterns[num];

    // Ghi trạng thái ra từng chân từ PB0 (a) đến PB6 (g)
    // Anode chung: Bit = 1 thì xuất mức 0 (RESET) để đèn sáng
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, (pattern & (1 << 0)) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, (pattern & (1 << 1)) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, (pattern & (1 << 2)) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, (pattern & (1 << 3)) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, (pattern & (1 << 4)) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, (pattern & (1 << 5)) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, (pattern & (1 << 6)) ? GPIO_PIN_RESET : GPIO_PIN_SET);
}
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
  /* USER CODE BEGIN 2 */
  TrafficState state = AUTO_RED_GREEN;
  int timer = 3;
  // Khởi tạo hiển thị trạng thái ban đầu
  setTrafficLight1(1, 0, 0);
  setTrafficLight2(0, 0, 1);

  int counter = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(counter >= 10) counter = 0;
	  display7SEG(counter++);
	  HAL_Delay(1000);
	  timer--;
	      if (timer <= 0) {
	          switch (state) {
	              case AUTO_RED_GREEN: // �?ang �?�? 1 - Xanh 2 -> Chuyển sang �?�? 1 - Vàng 2 (2s)
	                  state = AUTO_RED_YELLOW;
	                  timer = 2;
	                  setTrafficLight1(1, 0, 0);
	                  setTrafficLight2(0, 1, 0);
	                  break;

	              case AUTO_RED_YELLOW: // �?ang �?�? 1 - Vàng 2 -> Chuyển sang Xanh 1 - �?�? 2 (3s)
	                  state = AUTO_GREEN_RED;
	                  timer = 3;
	                  setTrafficLight1(0, 0, 1);
	                  setTrafficLight2(1, 0, 0);
	                  break;

	              case AUTO_GREEN_RED: // �?ang Xanh 1 - �?�? 2 -> Chuyển sang Vàng 1 - �?�? 2 (2s)
	                  state = AUTO_YELLOW_RED;
	                  timer = 2;
	                  setTrafficLight1(0, 1, 0);
	                  setTrafficLight2(1, 0, 0);
	                  break;

	              case AUTO_YELLOW_RED: // Hết chu kỳ -> Quay lại �?�? 1 - Xanh 2 (3s)
	                  state = AUTO_RED_GREEN;
	                  timer = 3;
	                  setTrafficLight1(1, 0, 0);
	                  setTrafficLight2(0, 0, 1);
	                  break;
	          }
	      }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, RED_LED_1_Pin|YELLOW_LED_2_Pin|GREEN_LED_3_Pin|RED_LED_4_Pin
                          |YELLOW_LED_5_Pin|GREEN_LED_6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_a_Pin|LED_b_Pin|LED_c_Pin|LED_d_Pin
                          |LED_e_Pin|LED_f_Pin|LED_g_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : RED_LED_1_Pin YELLOW_LED_2_Pin GREEN_LED_3_Pin RED_LED_4_Pin
                           YELLOW_LED_5_Pin GREEN_LED_6_Pin */
  GPIO_InitStruct.Pin = RED_LED_1_Pin|YELLOW_LED_2_Pin|GREEN_LED_3_Pin|RED_LED_4_Pin
                          |YELLOW_LED_5_Pin|GREEN_LED_6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_a_Pin LED_b_Pin LED_c_Pin LED_d_Pin
                           LED_e_Pin LED_f_Pin LED_g_Pin */
  GPIO_InitStruct.Pin = LED_a_Pin|LED_b_Pin|LED_c_Pin|LED_d_Pin
                          |LED_e_Pin|LED_f_Pin|LED_g_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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
