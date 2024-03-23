#ifndef _SEVEN_SEGMENT_AND_LED_DERIVER
#define _SEVEN_SEGMENT_AND_LED_DERIVER
#include "main.h"
#include "tim.h"
#include "adc.h"
#include "usart.h"
#include "stm32f1xx_hal_gpio.h"
#include "string.h"
#include "w25qxx.h"
#include "Typedef.h"
#include "write_to_flash.h"


void SevenSegmentDisplay(uint8_t number);
void RecordLED_OFF();
void RecordLED_ON();
void PalyLED_ON();
void PlayLED_OFF();
void Blinking();
#ifndef