#include"Typedef.h"
#include "state_machine.h"
#include "main.h"
#include "tim.h"
#include "adc.h"
#include "usart.h"
#include "stm32f1xx_hal_gpio.h"
#include "string.h"
#include "w25qxx.h"

uint32_t timing_1 = 0;


void state_machine()
{
  switch (state)
  {
  case ReadKeyboardState:
  {
    state = ReadKeyboardState;
    break;
  }
  case RecordState:
  {
    timing_1 = 0;
    timing_1 = HAL_GetTick();
    RecordStatefunc();
    timing_1 = HAL_GetTick() - timing_1;
    state = ReadKeyboardState;
    break;
  }
  case ChooseVoiceForPlay:
  {
    ChooseVoiceForPlayfunc();
    state = ReadKeyboardState;
    break;
  }
  case PlayState:
  {
    timing_1 = 0;
    timing_1 = HAL_GetTick();
    PlayStateFun();
    timing_1 = HAL_GetTick() - timing_1;
    state = ReadKeyboardState;
    break;
  }
  case FlashEraseState:
  {
    FlashEraseFunc();
    state = ReadKeyboardState;
    break;
  }
  case PLayLoop:
  {
    LoopPLayFunc();
    state = ReadKeyboardState;
    break;
  }
  }
}
