/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define VoiceArraySize 2000
#define MaxNumberOfVoice 10
#define SampleRate 25000
#define Print
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
  extern TIM_HandleTypeDef htim2;
  extern UART_HandleTypeDef huart1;
  extern ADC_HandleTypeDef hadc1;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define FLASH_CS_Pin GPIO_PIN_4
#define FLASH_CS_GPIO_Port GPIOA
#define Relay_SW_Pin GPIO_PIN_7
#define Relay_SW_GPIO_Port GPIOA
#define BCD_D_Pin GPIO_PIN_10
#define BCD_D_GPIO_Port GPIOB
#define Pause_Key_Pin GPIO_PIN_11
#define Pause_Key_GPIO_Port GPIOB
#define Play_Key_Pin GPIO_PIN_12
#define Play_Key_GPIO_Port GPIOB
#define Record_Key_Pin GPIO_PIN_13
#define Record_Key_GPIO_Port GPIOB
#define Next_Key_Pin GPIO_PIN_14
#define Next_Key_GPIO_Port GPIOB
#define BCD_C_Pin GPIO_PIN_8
#define BCD_C_GPIO_Port GPIOA
#define BCD_B_Pin GPIO_PIN_11
#define BCD_B_GPIO_Port GPIOA
#define BCD_A_Pin GPIO_PIN_12
#define BCD_A_GPIO_Port GPIOA
#define Record_LED_Pin GPIO_PIN_6
#define Record_LED_GPIO_Port GPIOB
#define Play_LED_Pin GPIO_PIN_7
#define Play_LED_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
