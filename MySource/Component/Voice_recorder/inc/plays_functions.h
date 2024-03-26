#ifndef _PALYS_FUNCTIONS_H
#define _PALYS_FUNCTIONS_H

#include "main.h"
#include "w25qxx.h"
#include "type_define.h"
#include "write_to_flash.h"




void PlayStateFun();
void StopPlaying();
void SetupForPlay(uint8_t VoiceNumber);
#endif