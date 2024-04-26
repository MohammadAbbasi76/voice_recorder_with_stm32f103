#ifndef READ_KEYBOARD_H_
#define READ_KEYBOARD_H_


#include"type_define.h"
#include"voice_recorder.h"
#include"record_functions.h"
#include"plays_functions.h"

#define DebounceTime 300 // in millis

void ReadKeyBoard(uint16_t GPIO_Pin);
#endif