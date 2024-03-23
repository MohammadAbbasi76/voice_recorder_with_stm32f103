

#ifndef STATE_MACHINE
#define STATE_MACHINE

#include "main.h"
#include "tim.h"
#include "adc.h"
#include "usart.h"
#include "stm32f1xx_hal_gpio.h"
#include "string.h"
#include "w25qxx.h"
#include "Typedef.h"
#include "write_to_flash.h"

void interrupt_func(void);
void state_machine(void);
void init_value();
void conv_adc_val_to_pwm_duty(uint16_t *val);
void pwm_part();
void PlayStateFun();
void RecordStatefunc();
void AdcGettingSample();
void ChooseVoiceForPlayfunc();
void choose_AudioOutput(AudioOutput out);
void SetupForPlay(uint8_t VoiceNumber);
void StopPlaying();
void SevenSegmentDisplay(uint8_t number);
void RecordLED_OFF();
void PalyLED_ON();
void RecordLED_OFF();
void RecordLED_ON();
void FlashPrint();
void FlashEraseFunc();
void LoopPLayFunc();
void StopRecording();
void NextPinFunc();
void DummyFunc();
#endif
