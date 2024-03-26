#ifndef  _RECORD_FUNCTION_H
#define _RECORD_FUNCTION_H
#include "main.h"
#include "tim.h"
#include "adc.h"
#include "usart.h"
#include "stm32f1xx_hal_gpio.h"
#include "string.h"
#include "w25qxx.h"
#include "type_define.h"
#include "write_to_flash.h"

void StopRecording();
void StartRecording();

#endif