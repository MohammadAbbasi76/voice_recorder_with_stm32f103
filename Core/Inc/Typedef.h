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
#define UartArraySize 3
#define UartTimeDataExpire 4 // in millis
#define M66DelayTime 3000    // in millis
#define PlayLoopTime 3000    // in millis
#define Print
#define DebounceTime 400 // in millis


uint16_t Buffer1[AdcArraySize];
uint16_t Buffer2[AdcArraySize];
uint8_t UartReceive[UartArraySize];
uint8_t InputKey;
uint8_t WitchVoiceWantToPlay;
uint8_t VoiceArrayReadFromFlash;
uint8_t state;
uint32_t ccr_pwm_val;
uint8_t FirstTimeFLG;
uint8_t VoiceForPlayLoop;
uint8_t FLGForVoiceStop;
uint32_t LastTimeKeyPress;
uint8_t NumberOfVoiceForLoopPlay;

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
    FlashEraseState,
    PLayLoop
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
