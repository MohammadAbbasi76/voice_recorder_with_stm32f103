

#ifndef STATE_MACHINE
#define STATE_MACHINE

#include"type_define.h"
#include"record_functions.h"
#include"plays_functions.h"


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
