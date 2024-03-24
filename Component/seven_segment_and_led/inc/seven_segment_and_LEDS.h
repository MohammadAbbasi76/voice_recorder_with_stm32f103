#ifndef _SEVEN_SEGMENT_AND_LED_DERIVER
#define _SEVEN_SEGMENT_AND_LED_DERIVER
#include "main.h"
#include "stm32f1xx_hal_gpio.h"
#include "type_define.h"



void SevenSegmentDisplay(uint8_t number);
void RecordLED_OFF();
void RecordLED_ON();
void PalyLED_ON();
void PlayLED_OFF();
void Blinking();
#ifndef