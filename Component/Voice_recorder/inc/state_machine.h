

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



void AdcGettingSample();



void choose_AudioOutput(AudioOutput out);


void FlashPrint();
void FlashEraseFunc();


void ConversionADCValueToPWMDuty(uint16_t *val);
void MakePWM_Wave();
void ChooseVoiceForPlay();
void NextPinFunc();
void DummyFunc();
void interrupt_func(void);
void state_machine(void);
void init_value();



#endif
