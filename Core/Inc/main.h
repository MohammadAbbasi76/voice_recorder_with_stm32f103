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
#define AdcArraySize 2000
#define StopTimeInSec 10
#define MaxNumberOfVoice 10
#define SampleRate 25000
#define UartArraySize 3
#define UartTimeDataExpire 4  //in millis
#define M66DelayTime  3000 // in millis
#define PlayLoopTime 3000 //in millis
#define Print
#define DebounceTime 400 //in millis
uint16_t TempArry[AdcArraySize];
uint16_t TempArry1[AdcArraySize];
uint8_t InputKey;
uint8_t WitchVoiceWannaToPlay;
uint8_t VoiceArrayReadFromFlash;
uint8_t state ;
uint32_t ccr_pwm_val;
uint8_t FirstTimeFLG;
uint8_t UartReceive[UartArraySize];
uint32_t UartFLush_T1;
uint8_t VoiceForPlayLoop;
uint8_t FLGForVoiceStop;
uint32_t LastTimeKeyPress;
uint8_t NumberofVoiceForLoopPlay;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
typedef enum
{
  record_key,
  play_key,
  pause_key,
  next_,
  back,
  non_
} keys;

typedef enum
{
  RecordState,
 PlayState,
  ChooseVoiceForPlay,
  PauseState,
  ReadKeyboardState,
  UartDataReceive,
  M66State,
  FlashEraseState,
  PLayLoop
} state_;

typedef enum
{

	M66_mic_in,
	Speaker
}AudioOutput;
uint16_t ADC_read();
void UART_Printf(const char *fmt, ...);
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */


struct adc_
{
  uint32_t counter;
  uint32_t TotallyStopTim;
  uint32_t StopTimeCounter;
} adc_stru;
struct pwm_
{
  uint32_t counter;
  uint32_t CountDataFromTotally[MaxNumberOfVoice];

} pwm_stru ;
struct voice_struc
{
  uint8_t ArrayGoToSave;
  uint8_t number;
  uint8_t WitchVoiceIsRecord[MaxNumberOfVoice];
} voice;
struct flags
{
  uint8_t AdcArryFull;
  uint8_t InterruptSwitch;
  uint8_t PwmArrayEmpty;
} flag;
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define FLASH_CS_Pin GPIO_PIN_4
#define FLASH_CS_GPIO_Port GPIOA
#define Relay_SW_Pin GPIO_PIN_7
#define Relay_SW_GPIO_Port GPIOA
#define BCD_D_Pin GPIO_PIN_10
#define BCD_D_GPIO_Port GPIOB
#define pause_Pin GPIO_PIN_11
#define pause_GPIO_Port GPIOB
#define pause_EXTI_IRQn EXTI15_10_IRQn
#define play_Pin GPIO_PIN_12
#define play_GPIO_Port GPIOB
#define play_EXTI_IRQn EXTI15_10_IRQn
#define recod_Pin GPIO_PIN_13
#define recod_GPIO_Port GPIOB
#define recod_EXTI_IRQn EXTI15_10_IRQn
#define Next_Pin GPIO_PIN_14
#define Next_GPIO_Port GPIOB
#define Next_EXTI_IRQn EXTI15_10_IRQn
#define M66_on_off_key_Pin GPIO_PIN_15
#define M66_on_off_key_GPIO_Port GPIOB
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
