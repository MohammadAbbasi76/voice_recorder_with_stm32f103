#ifndef __TYPEDEF_H_
#define __TYPEDEF_H_
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include "main.h"
#include "tim.h"
#include "adc.h"
#include "usart.h"
#include "stm32f1xx_hal_gpio.h"
#include "STM_Log.h"
#include"seven_segment_and_LEDS.h"

#define StopTimeInSec 10

uint16_t Buffer1[VoiceArraySize];
uint16_t Buffer2[VoiceArraySize];

typedef enum
{
    Init,
    RecordState,
    PlayState,
    PauseState,
    ReadKeyboardState,
    FlashEraseState
} state_t;
typedef struct 
{
    uint32_t Counter;
    uint32_t TotallyStopTim;
    uint32_t StopTimeCounter;
} ADC_St;
typedef struct 
{
    uint32_t Counter;
    uint32_t CountDataFromTotally[MaxNumberOfVoice];
} PWM_St;
typedef struct 
{
    uint8_t CountOfSavedArray;
    uint8_t RecordedArray[MaxNumberOfVoice];
} Voice_St;
typedef struct
{
    uint8_t AdcArrayFull;
    uint8_t InterruptSwitch;
    uint8_t PwmArrayEmpty;
} Flags_St;
typedef struct 
{
    ADC_St ADC;
    PWM_St PWM;
    Flags_St Flag;
    Voice_St Voice;
    uint8_t Track;
    uint8_t ReadFromFlash;
    uint8_t State;
}VoiceRecorder_St;
VoiceRecorder_St VoiceRecorderSt;
#endif
