#ifndef VOICE_RECORDER_H
#define VOICE_RECORDER_H

#include"type_define.h"
#include"record_functions.h"
#include"plays_functions.h"
#include "w25qxx.h"
#include "write_to_flash.h"
#include"seven_segment_and_LEDS.h"
#include"read_keyboard.h"



void RestFactoryFunction();
void ChooseVoiceForPlay();
void NextTrack();
void DummyFunc();
void InterruptFunc();
void VoiceRecorder();
void VoiceRecorderInitiation();

#endif
