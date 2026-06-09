/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define HIGH 1
#define LOW 0
#define OFF 0

//Which animantion is used at the beginning. Only select ONE
//#define ANIMATION1
#define ANIMATION2


/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void frontLED_set(uint8_t led);
void mute(uint8_t value);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define btn_gain_Pin GPIO_PIN_0
#define btn_gain_GPIO_Port GPIOF
#define btn_gain_EXTI_IRQn EXTI0_1_IRQn
#define btn_input_Pin GPIO_PIN_1
#define btn_input_GPIO_Port GPIOF
#define btn_input_EXTI_IRQn EXTI0_1_IRQn
#define ext_PSU_en_Pin GPIO_PIN_0
#define ext_PSU_en_GPIO_Port GPIOA
#define ext_mute_Pin GPIO_PIN_1
#define ext_mute_GPIO_Port GPIOA
#define ext_gain_Pin GPIO_PIN_2
#define ext_gain_GPIO_Port GPIOA
#define ext_input_Pin GPIO_PIN_3
#define ext_input_GPIO_Port GPIOA
#define frontLED_grn_Pin GPIO_PIN_5
#define frontLED_grn_GPIO_Port GPIOA
#define frontLED_blu_Pin GPIO_PIN_6
#define frontLED_blu_GPIO_Port GPIOA
#define frontLED_red_Pin GPIO_PIN_7
#define frontLED_red_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
