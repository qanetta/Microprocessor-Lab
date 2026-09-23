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
const uint16_t CLOCK_PINS[12] = {
    GPIO_PIN_4,  // 0: Vị trí 12h
    GPIO_PIN_5,  // 1: Vị trí 1h
    GPIO_PIN_6,  // 2: Vị trí 2h
    GPIO_PIN_7,  // 3: Vị trí 3h
    GPIO_PIN_8,  // 4: Vị trí 4h
    GPIO_PIN_9,  // 5: Vị trí 5h
    GPIO_PIN_10, // 6: Vị trí 6h
    GPIO_PIN_11, // 7: Vị trí 7h
    GPIO_PIN_12, // 8: Vị trí 8h
    GPIO_PIN_13, // 9: Vị trí 9h
    GPIO_PIN_14, // 10: Vị trí 10h
    GPIO_PIN_15  // 11: Vị trí 11h
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void clearAllClock(void) {
    HAL_GPIO_WritePin(GPIOA,
        GPIO_PIN_4  | GPIO_PIN_5  | GPIO_PIN_6  | GPIO_PIN_7  |
        GPIO_PIN_8  | GPIO_PIN_9  | GPIO_PIN_10 | GPIO_PIN_11 |
        GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15,
        GPIO_PIN_SET); // Ghi mức 1 (SET) để tắt toàn bộ LED (Active-Low)
}

void setNumberOnClock(int num) {
    if (num >= 0 && num < 12) {
        HAL_GPIO_WritePin(GPIOA, CLOCK_PINS[num], GPIO_PIN_RESET);
    }
}

void clearNumberOnClock(int num) {
    if (num >= 0 && num < 12) {
        HAL_GPIO_WritePin(GPIOA, CLOCK_PINS[num], GPIO_PIN_SET);
    }
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
  // Tắt toàn bộ 12 LED trước khi bắt đầu (Active-low: SET là TẮT)
    for (int i = 0; i < 12; i++) {
        HAL_GPIO_WritePin(GPIOA, CLOCK_PINS[i], GPIO_PIN_SET);
    }
    int current_led = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	 // 1. Tắt bóng LED ở bước trước
	        int prev_led = (current_led == 0) ? 11 : (current_led - 1);
	        HAL_GPIO_WritePin(GPIOA, CLOCK_PINS[prev_led], GPIO_PIN_SET);

	        // 2. Bật bóng LED hiện tại (Active-low: RESET là BẬT)
	        HAL_GPIO_WritePin(GPIOA, CLOCK_PINS[current_led], GPIO_PIN_RESET);

	        // 3. Tăng chỉ số bóng đèn để chạy vòng tròn 0 -> 11
	        current_led = (current_led + 1) % 12;

	        // 4. Giữ thời gian sáng mỗi bóng để mắt quan sát được
	        HAL_Delay(500); // 0.5 giây chuyển 1 bóng
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
  HAL_GPIO_WritePin(GPIOA, LED_12h_Pin|LED_1h_Pin|LED_2h_Pin|LED_3h_Pin
                          |LED_4h_Pin|LED_5h_Pin|LED_6h_Pin|LED_7h_Pin
                          |LED_8h_Pin|LED_9h_Pin|LED_10h_Pin|LED_11h_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_a_Pin|LED_b_Pin|LED_c_Pin|LED_d_Pin
                          |LED_e_Pin|LED_f_Pin|LED_g_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED_12h_Pin LED_1h_Pin LED_2h_Pin LED_3h_Pin
                           LED_4h_Pin LED_5h_Pin LED_6h_Pin LED_7h_Pin
                           LED_8h_Pin LED_9h_Pin LED_10h_Pin LED_11h_Pin */
  GPIO_InitStruct.Pin = LED_12h_Pin|LED_1h_Pin|LED_2h_Pin|LED_3h_Pin
                          |LED_4h_Pin|LED_5h_Pin|LED_6h_Pin|LED_7h_Pin
                          |LED_8h_Pin|LED_9h_Pin|LED_10h_Pin|LED_11h_Pin;
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
