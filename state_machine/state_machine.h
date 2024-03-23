

#ifndef  STATE_MACHINE
#define STATE_MACHINE




#include"main.h"
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
void UartReceiveFnc();
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
