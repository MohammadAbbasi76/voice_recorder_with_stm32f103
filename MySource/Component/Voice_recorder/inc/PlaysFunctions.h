#ifndef _PALYS_FUNCTIONS_H
#define _PALYS_FUNCTIONS_H

#include "TypeDefine.h"



void StartPlaying();
void StopPlaying();
void PrepareToPlay();
void ConversionADCValueToPWMDuty(uint16_t *val);
void MakePwmWave();

#endif