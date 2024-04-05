
#ifndef _SEVEN_SEGMENT_AND_LED_DERIVER_H
#define _SEVEN_SEGMENT_AND_LED_DERIVER_H
#include "main.h"
#include "stm32f1xx_hal_gpio.h"


void SevenSegmentDisplay(uint8_t number);
void RecordLedOff();
void RecordLedOn();
void PalyLedOn();
void PlayLedOff();
void Blinking();
#endif
