#ifndef READ_KEYBOARD_H_
#define READ_KEYBOARD_H_


#include"type_define.h"
#include"voice_recorder.h"
#include"record_functions.h"
#include"plays_functions.h"

#define DefaultDebounceTime 300 // in millis

uint16_t ReadKeyBoard(void);
void KeyBoard();
#endif