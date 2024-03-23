#ifndef _PALYS_FUNCTIONS_H
#define _PALYS_FUNCTIONS_H

#include "main.h"
#include "tim.h"
#include "adc.h"
#include "usart.h"
#include "stm32f1xx_hal_gpio.h"
#include "string.h"
#include "w25qxx.h"
#include "Typedef.h"
#include "write_to_flash.h"



void LoopPLayFunc();
void PlayStateFun();
void StopPlaying();
void SetupForPlay(uint8_t VoiceNumber);
#endif