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

#define StopTimeInSec 10

uint16_t Buffer1[VoiceArraySize];
uint16_t Buffer2[VoiceArraySize];
uint8_t WitchVoiceWantToPlay;
uint8_t VoiceArrayReadFromFlash;
uint8_t state;
uint32_t LastTimeKeyPress;

typedef enum
{
    RecordState,
    PlayState,
    ChooseTrack,
    PauseState,
    ReadKeyboardState,
    FlashEraseState
} state_t;

typedef enum
{
    TurnOffSpeaker,
    TurnOnSpeaker
} AudioOutput;
struct ADC_ptr
{
    uint32_t counter;
    uint32_t TotallyStopTim;
    uint32_t StopTimeCounter;
} ADC_t;
struct PWM_ptr
{
    uint32_t counter;
    uint32_t CountDataFromTotally[MaxNumberOfVoice];

} PWM_t;
struct Voice_ptr
{
    uint8_t ArrayGoToSave;
    uint8_t number;
    uint8_t WitchVoiceIsRecord[MaxNumberOfVoice];
} voice_t;
struct Flags_ptr
{
    uint8_t AdcArrayFull;
    uint8_t InterruptSwitch;
    uint8_t PwmArrayEmpty;
} flag;

#endif
