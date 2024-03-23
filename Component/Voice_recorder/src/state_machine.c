#include "Typedef.h"
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
    StartRecording();
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

void init_value()
{
  HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_SET);
  /** adc init value*/

  ADC_t.counter = 0;
  flag.AdcArrayFull = 0;
  ADC_t.TotallyStopTim = SampleRate * StopTimeInSec;
  ADC_t.StopTimeCounter = 0;
  voice_t.ArrayGoToSave = 0;
  voice_t.number = 0;
  FLGForVoiceStop = 0;
  for (int i = 0; i < MaxNumberOfVoice; i++)
  {
    voice_t.WitchVoiceIsRecord[i] = 0;
  }
  /**  timer2 state changer flag */
  flag.InterruptSwitch = 1;
  /** first state*/
  state = ReadKeyboardState;
  /**  fris val for keyboard value*/
  InputKey = non_;
  /** pwm init val*/
  PWM_t.counter = 0;
  for (int i = 0; i < MaxNumberOfVoice; i++)
    PWM_t.CountDataFromTotally[i] = 0;
  flag.PwmArrayEmpty = 0;
  state = ReadKeyboardState;
  WitchVoiceWantToPlay = 1;
  ccr_pwm_val = __HAL_TIM_GET_AUTORELOAD(&htim3);
  HAL_ADCEx_Calibration_Start(&hadc1);
  FirstTimeFLG = 1;
  if (!W25qxx_Init())
  {
    while (1)
    {
    }
    // flash have problem !
  }
  //  W25qxx_EraseChip();
  RestoreDetail(voice_t.WitchVoiceIsRecord, voice_t.number, &(voice_t.ArrayGoToSave));
  if (voice_t.WitchVoiceIsRecord[0] == 255)
  {
    voice_t.number = 0;
    for (uint8_t i = 0; i < MaxNumberOfVoice; i++)
      voice_t.WitchVoiceIsRecord[i] = 0;
    voice_t.ArrayGoToSave = 0;
  }
  for (uint8_t i = 0; i < UartArraySize; i++)
    UartReceive[i] = 0;
  HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_RESET);
  HAL_Delay(500);
  HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_SET);
  HAL_Delay(500);
  HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_RESET);
  HAL_Delay(500);
  HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_SET);
  HAL_Delay(500);
  HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_RESET);
  HAL_Delay(500);
  HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_SET);
  HAL_Delay(500);
  HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_RESET);
  for (int i = 0; i < UartArraySize; i++)
    UartReceive[i] = 0;
  LastTimeKeyPress = HAL_GetTick() + 100;
  NumberOfVoiceForLoopPlay = 0;
  SevenSegmentDisplay(WitchVoiceWantToPlay);
}

void ChooseVoiceForPlayfunc()
{
  // HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_SET);
  // HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_SET);
  // voice_t.number = HowManyVoiceIsRecord(
  //     WitchVoiceWannaToPlay, voice_t.WitchVoiceIsRecord);
  voice_t.number = WitchVoiceWantToPlay;
  UART_Printf("voice_t.number =%d\n", voice_t.number);
  SevenSegmentDisplay(voice_t.number);
  if (voice_t.WitchVoiceIsRecord[voice_t.number] == 1)
  {
    SevenSegmentDisplay(voice_t.number);
    flag.InterruptSwitch = 0;
    flag.PwmArrayEmpty = 0;
    VoiceArrayReadFromFlash = 0;
    for (int i = 0; i < AdcArraySize; i++)
    {
      Buffer2[i] = 0;
    }
    RestoreDetail(voice_t.WitchVoiceIsRecord, voice_t.number, &(voice_t.ArrayGoToSave));
    restore_2k_array(voice_t.number, VoiceArrayReadFromFlash, Buffer2);
    PWM_t.CountDataFromTotally[voice_t.number] = (uint32_t)((voice_t.ArrayGoToSave) * (AdcArraySize));
    for (int i = 0; i < AdcArraySize; i++)
    {
      Buffer1[i] = Buffer2[i];
    }
  }
  else
  {
    flag.InterruptSwitch = 0;
    flag.PwmArrayEmpty = 0;
    VoiceArrayReadFromFlash = 0;
    for (int i = 0; i < AdcArraySize; i++)
    {
      Buffer2[i] = 0;
      Buffer1[i] = Buffer2[i];
    }
    StopPlaying();
  }
  // HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_RESET);
  // HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_RESET);
}