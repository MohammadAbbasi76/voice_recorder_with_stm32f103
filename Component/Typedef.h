#ifndef __TYPEDEF_H1_
#define __TYPEDEF_H1_
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include "main.h"

#define AdcArraySize 2000
#define StopTimeInSec 10
#define MaxNumberOfVoice 10
#define SampleRate 25000
#define Print
#define DebounceTime 400 // in millis


uint16_t Buffer1[AdcArraySize];
uint16_t Buffer2[AdcArraySize];
uint8_t WitchVoiceWantToPlay;
uint8_t VoiceArrayReadFromFlash;
uint8_t state;
uint32_t LastTimeKeyPress;

typedef enum
{
    RecordState,
    PlayState,
    ChooseVoice,
    PauseState,
    ReadKeyboardState,
    FlashEraseState
} state_t;

typedef enum
{
    M66_mic_in,
    Speaker
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