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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>

#include "debounced_switch.h"
#include "edge_fsm.h"
#include "blink_control.h"
#include "timer_period_manager.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
DebouncedSwitch debounced_button1, debounced_button2;
EdgeDetector edge_detector1, edge_detector2;
BlinkControl blink_control_led1, blink_control_led2;
TimerPeriodManagerFSM period_manager1, period_manager2;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//	void UART_SendString(char* str) {
//		HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
//	}
//
//void send_edge_detector_status(EdgeFSMState edge_state) {
//	char buffer[50];  // Buffer for the string output
//	const char *edge_state_str;
//
//	// Map EdgeFSMState to a string
//	switch (edge_state) {
//		case IDLE_HIGH:
//		case IDLE_LOW:
//			edge_state_str = "NO_EDGE";
//			break;
//		case RISING_EDGE:
//			edge_state_str = "RISING_EDGE";
//			break;
//		case FALLING_EDGE:
//			edge_state_str = "FALLING_EDGE";
//			break;
//		default:
//			edge_state_str = "UNKNOWN";
//	}
//
//	// Format the message with the edge state string
//	sprintf(buffer, "Edge Detector State: %s\r\n", edge_state_str);
//
//	// Send the buffer over UART
//	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
//}
//
//void print_debounced_switch_state_uart(DebouncedSwitch *debounced_switch) {
//	char buffer[50];
//	const char *state_str;
//
//	// Get the debounced switch state as a string
//	switch (debounced_switch->fsm.currentState) {
//		case SWITCH_IDLE:
//			state_str = "SWITCH_IDLE";
//			break;
//		case SWITCH_PRESSED:
//			state_str = "SWITCH_PRESSED";
//			break;
//		case SWITCH_RELEASED:
//			state_str = "SWITCH_RELEASED";
//			break;
//		default:
//			state_str = "UNKNOWN_STATE";
//			break;
//	}
//
//	// Format the string into the buffer
//	sprintf(buffer, "Debounced Switch State: %s\r\n", state_str);
//
//	// Send the buffer over UART
//	HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);
//}
//
//void send_blink_control_status(BlinkControl *blink_control) {
//    char buffer[50];  // Buffer for the string output
//    const char *blink_state_str;
//
//    // Map BlinkState to a string
//    switch (blink_control->fsm.currentState) {
//        case LED_OFF:
//            blink_state_str = "LED_OFF";
//            break;
//        case LED_ON:
//            blink_state_str = "LED_ON";
//            break;
//        default:
//            blink_state_str = "UNKNOWN_STATE";
//            break;
//    }
//
//    // Format the message with the blink state string
//    sprintf(buffer, "Blink Control State: %s\r\n", blink_state_str);
//
//    // Send the buffer over UART
//    HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
//}

// Send TimerPeriodManagerFSM state over UART
void send_timer_period_manager_fsm_status(TimerPeriodManagerFSM *period_manager) {
    char buffer[100];
    const char *state_str;

    // Map FSM states to strings
    switch (period_manager->fsm.currentState) {
        case PERIOD_1_STATE:
            state_str = "PERIOD_1_STATE";
            break;
        case PERIOD_2_STATE:
            state_str = "PERIOD_2_STATE";
            break;
        case PERIOD_3_STATE:
            state_str = "PERIOD_3_STATE";
            break;
        default:
            state_str = "UNKNOWN_STATE";
            break;
    }

    // Format the string
    sprintf(buffer, "TimerPeriodManagerFSM State: %s\r\n", state_str);

    // Send the buffer over UART
	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);

}

void print_timer_expiration_period(Timer *timer) {
    char buffer[50];
    // Format the string with the timer's expiration period in milliseconds
    sprintf(buffer, "Timer Expiration Period: %lu ms\r\n", timer->duration_ms);

	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  // Initialize debounced buttons
  debounced_switch_init(&debounced_button1, SW_1_GPIO_Port, SW_1_Pin);
  debounced_switch_init(&debounced_button2, SW_2_GPIO_Port, SW_2_Pin);
  blink_control_init(&blink_control_led1, LED_1_GPIO_Port, LED_1_Pin, 0);
  blink_control_init(&blink_control_led2, LED_2_GPIO_Port, LED_2_Pin, 0);
  edge_detector_init(&edge_detector1, &debounced_button1);
  edge_detector_init(&edge_detector2, &debounced_button2);
  timer_period_manager_fsm_init(&period_manager1, &blink_control_led1.blink_timer, &edge_detector1);
  timer_period_manager_fsm_init(&period_manager2, &blink_control_led2.blink_timer, &edge_detector2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	debounced_switch_update(&debounced_button1);
	debounced_switch_update(&debounced_button2);
    edge_detector_update(&edge_detector1);
    edge_detector_update(&edge_detector2);
    blink_control_update(&blink_control_led1);
    blink_control_update(&blink_control_led2);
    timer_period_manager_fsm_update(&period_manager1);
    timer_period_manager_fsm_update(&period_manager2);
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
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_1_Pin */
  GPIO_InitStruct.Pin = LED_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_2_Pin */
  GPIO_InitStruct.Pin = LED_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SW_1_Pin SW_2_Pin */
  GPIO_InitStruct.Pin = SW_1_Pin|SW_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
  * @brief  Retargets the C library printf function to the USART.
  *   None
  * @retval None
  */

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
