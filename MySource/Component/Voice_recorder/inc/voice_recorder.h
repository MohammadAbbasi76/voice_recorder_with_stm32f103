#ifndef VOICE_RECORDER
#define VOICE_RECORDER

#include"type_define.h"
#include"record_functions.h"
#include"plays_functions.h"
#include "w25qxx.h"
#include "write_to_flash.h"
#include"seven_segment_and_LEDS.h"

void ADCSampling();
void AudioOutputControl(AudioOutput out);


void FlashPrint();
void FlashEraseFunc();


void ConversionADCValueToPWMDuty(uint16_t *val);
void MakePWM_Wave();
void ChooseVoiceForPlay();
void NextTrack();
void DummyFunc();
void interrupt_func(void);
void state_machine(void);
void init_value();



#endif
