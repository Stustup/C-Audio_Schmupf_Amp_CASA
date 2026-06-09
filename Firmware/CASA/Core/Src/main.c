/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "PCA9536.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_gpio.h"
#include <stdint.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define RED 1
#define GREEN 2
#define BLUE 3

#define BOOTUP_TIME 2500
#define BLINK_TIME 250
#define BLINK_TIME_PCA 100
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */
//Button flags
volatile uint8_t btnFlag_gain  = 0;
volatile uint8_t btnFlag_input = 0;

uint8_t PCA9536_defaultOutput = 0x0A;

uint32_t Timestamp_ms;
uint32_t Timestamp_ms_led;
uint32_t Timestamp_ms_pca;
uint8_t firstBoot = 1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  //Set red led for boot
  frontLED_set(RED);

  //Setup the IO Expander via I2C
  PCA9536 pca9536;

  PCA9536_Init(&pca9536, &hi2c1);
  PCA9536_configPinOutput(&pca9536);                                                            //Set pinmode to output

  //Get timestamp for boot sequence
  Timestamp_ms = HAL_GetTick();
  Timestamp_ms_led = Timestamp_ms;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  //Boot animation and init
  uint8_t _counter_PCA = 0; 

  #ifdef ANIMATION1
  uint8_t _animationArray1[4] = {0x0E, 0x0D, 0x0B, 0x07};
  #endif

  #ifdef ANIMATION2
  uint8_t _animationArray2[8] = {0x0F, 0x0E, 0x0C, 0x08, 0x00, 0x08, 0x0C, 0x0E};
  #endif

  while(firstBoot) {

    uint32_t _current_Timestamp_ms = HAL_GetTick();

    //Blink red led as long as analog circuitry needs to settle
    if(_current_Timestamp_ms >= Timestamp_ms_led+BLINK_TIME) {
      HAL_GPIO_TogglePin(frontLED_red_GPIO_Port, frontLED_red_Pin);
      Timestamp_ms_led = Timestamp_ms_led+BLINK_TIME;
    }

    //Cute little animation for bootup time
    if(_current_Timestamp_ms >= Timestamp_ms_pca+BLINK_TIME_PCA) {

      #ifdef ANIMATION1
      PCA9536_write(&pca9536, PCA9536_outputRegister, &_animationArray1[_counter_PCA]);
      
      if(_counter_PCA > 3) _counter_PCA = 0;
      #endif
      
      #ifdef ANIMATION2
      PCA9536_write(&pca9536, PCA9536_outputRegister, &_animationArray2[_counter_PCA]);
      
      if(_counter_PCA > 7) _counter_PCA = 0;
      #endif

      _counter_PCA++;
      Timestamp_ms_pca = Timestamp_ms_pca+BLINK_TIME_PCA;
    } 

    //When settled, set front power led to green, deactivate mute and turn off first_boot-mode
    if(_current_Timestamp_ms > Timestamp_ms+BOOTUP_TIME) {
      frontLED_set(GREEN);
      mute(0);
      firstBoot = 0;
    }
  }

  PCA9536_write(&pca9536, PCA9536_outputRegister, &PCA9536_defaultOutput);      //Sets the defualt output (gain 0 and input 0)

  while (1)
  {

    if(btnFlag_gain) {
      PCA9536_switchPort(&pca9536, 0);
      HAL_GPIO_WritePin(ext_gain_GPIO_Port, ext_gain_Pin, pca9536._port0);
      btnFlag_gain = 0;
    }

    if(btnFlag_input) {
      PCA9536_switchPort(&pca9536, 1);
      HAL_GPIO_WritePin(ext_input_GPIO_Port, ext_input_Pin, pca9536._port1);
      btnFlag_input = 0;
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00201D2B;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, ext_PSU_en_Pin|ext_gain_Pin|ext_input_Pin|frontLED_grn_Pin
                          |frontLED_blu_Pin|frontLED_red_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ext_mute_GPIO_Port, ext_mute_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : btn_gain_Pin btn_input_Pin */
  GPIO_InitStruct.Pin = btn_gain_Pin|btn_input_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : ext_PSU_en_Pin ext_mute_Pin ext_gain_Pin ext_input_Pin
                           frontLED_grn_Pin frontLED_blu_Pin frontLED_red_Pin */
  GPIO_InitStruct.Pin = ext_PSU_en_Pin|ext_mute_Pin|ext_gain_Pin|ext_input_Pin
                          |frontLED_grn_Pin|frontLED_blu_Pin|frontLED_red_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/**
 * @brief Sets front leds, only one colour at a time
 * @param led: 
 *  1 -> RED
 *  2 -> GREEN
 *  3 -> BLUE
 */
void frontLED_set(uint8_t led) {
  switch (led) {
    case 1: 
      HAL_GPIO_WritePin(frontLED_red_GPIO_Port, frontLED_red_Pin, 1);
      HAL_GPIO_WritePin(frontLED_grn_GPIO_Port, frontLED_grn_Pin, 0);
      HAL_GPIO_WritePin(frontLED_blu_GPIO_Port, frontLED_blu_Pin, 0);
      break;
    case 2: 
      HAL_GPIO_WritePin(frontLED_red_GPIO_Port, frontLED_red_Pin, 0);
      HAL_GPIO_WritePin(frontLED_grn_GPIO_Port, frontLED_grn_Pin, 1);
      HAL_GPIO_WritePin(frontLED_blu_GPIO_Port, frontLED_blu_Pin, 0);
      break;
    case 3: 
      HAL_GPIO_WritePin(frontLED_red_GPIO_Port, frontLED_red_Pin, 0);
      HAL_GPIO_WritePin(frontLED_grn_GPIO_Port, frontLED_grn_Pin, 0);
      HAL_GPIO_WritePin(frontLED_blu_GPIO_Port, frontLED_blu_Pin, 1);
      break;
    default:
      HAL_GPIO_WritePin(frontLED_red_GPIO_Port, frontLED_red_Pin, 0);
      HAL_GPIO_WritePin(frontLED_grn_GPIO_Port, frontLED_grn_Pin, 0);
      HAL_GPIO_WritePin(frontLED_blu_GPIO_Port, frontLED_blu_Pin, 0);
      break;
  }
}

/**
 * @brief Sets the mute relay
 * @param value: 
 * 0 -> mute off
 * 1 -> mute on
 */
void mute(uint8_t value) {
  HAL_GPIO_WritePin(ext_mute_GPIO_Port, ext_mute_Pin, value);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if(GPIO_Pin == btn_gain_Pin) btnFlag_gain = 1;
  if(GPIO_Pin == btn_input_Pin) btnFlag_input = 1;
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
#ifdef USE_FULL_ASSERT
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
