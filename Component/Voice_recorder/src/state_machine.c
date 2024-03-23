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
    ChooseVoiceForPlay();
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
  }
}

void init_value()
{
  /** adc init value*/
  ADC_t.counter = 0;
  flag.AdcArrayFull = 0;
  ADC_t.TotallyStopTim = SampleRate * StopTimeInSec;
  ADC_t.StopTimeCounter = 0;
  voice_t.ArrayGoToSave = 0;
  voice_t.number = 0;
  FLGForVoiceStop = 0;
  flag.InterruptSwitch = 1;
  state = ReadKeyboardState;
  InputKey = non_;
  PWM_t.counter = 0;
  flag.PwmArrayEmpty = 0;
  state = ReadKeyboardState;
  WitchVoiceWantToPlay = 1;
  memset(voice_t.WitchVoiceIsRecord, 0x0, MaxNumberOfVoice);
  memset(PWM_t.CountDataFromTotally, 0x0, MaxNumberOfVoice);
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
  Blinking();
  LastTimeKeyPress = HAL_GetTick() + 100;
  NumberOfVoiceForLoopPlay = 0;
  SevenSegmentDisplay(WitchVoiceWantToPlay);
}

void ChooseVoiceForPlay()
{
  voice_t.number = WitchVoiceWantToPlay;
  UART_Printf("voice_t.number =%d\n", voice_t.number);
  SevenSegmentDisplay(voice_t.number);
  if (voice_t.WitchVoiceIsRecord[voice_t.number] == 1)
  {
    SevenSegmentDisplay(voice_t.number);
    flag.InterruptSwitch = 0;
    flag.PwmArrayEmpty = 0;
    VoiceArrayReadFromFlash = 0;
    memset(Buffer2,0x0,AdcArraySize);
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
    memset(Buffer2,0x0,AdcArraySize);
    memset(Buffer1,0x0,AdcArraySize);
    StopPlaying();
  }
}

void FlashEraseFunc()
{
  if (!W25qxx_Init())
  {
    while (1)
    {
    }
  }
  HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_SET);
  W25qxx_EraseChip();
  init_value();
  HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_RESET);
}

void DummyFunc()
{
  int a = 0;
  for (int i = 0; i < 10000; i++)
  {
    a++;
  }
}

void choose_AudioOutput(AudioOutput out)
{
  if (out == Speaker)
  {
    HAL_GPIO_WritePin(Relay_SW_GPIO_Port, Relay_SW_Pin, GPIO_PIN_SET);
  }
  else if (out == M66_mic_in)
  {
    HAL_GPIO_WritePin(Relay_SW_GPIO_Port, Relay_SW_Pin, GPIO_PIN_RESET);
  }
}

void NextPinFunc()
{
  int a = (LastTimeKeyPress - HAL_GetTick());
  if (a < 5)
  {
    LastTimeKeyPress = 0;
    WitchVoiceWantToPlay++;
    // if (WitchVoiceWannaToPlay > (FindFreeSpceInflash(voice.WitchVoiceIsRecord) + 1))
    if (WitchVoiceWantToPlay > (MaxNumberOfVoice - 1))
    {
      WitchVoiceWantToPlay = 1;
    }
    SevenSegmentDisplay(WitchVoiceWantToPlay);
    state = ChooseVoiceForPlay;
    LastTimeKeyPress = DebounceTime + HAL_GetTick();
  }
}

void AdcGettingSample()
{
  if (ADC_t.StopTimeCounter == ADC_t.TotallyStopTim)
  {
    ADC_t.StopTimeCounter++;
    for (int i = 0; i < AdcArraySize; i++)
    {
      Buffer1[i] = Buffer2[i];
      Buffer2[i] = 0;
    }
    ADC_t.counter = 0;
    flag.AdcArrayFull = 1;
  }
  else
  {
    // Buffer2[adc_stru.counter] = HAL_ADC_GetValue(&hadc1);
    Buffer2[ADC_t.counter] = ADC_read();
    ADC_t.counter++;
    ADC_t.StopTimeCounter++;
    if (ADC_t.counter == (AdcArraySize))
    {
      ADC_t.counter = 0;
      for (int i = 0; i < AdcArraySize; i++)
      {
        Buffer1[i] = Buffer2[i];
        Buffer2[i] = 0;
      }
      flag.AdcArrayFull = 1;
    }
  }
}
void interrupt_func(void)
{
  if (flag.InterruptSwitch == 1)
  {
    AdcGettingSample();
  }
  else
  {
    MakePWM_Wave();
  }
}
void MakePWM_Wave()
{
  if (flag.PwmArrayEmpty == 0)
  {
    __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, (uint16_t)(Buffer1[PWM_t.counter]));
    PWM_t.counter++;
    //      PWM_t.CountDataFromTotally[voice.number]--;
    if (PWM_t.counter == AdcArraySize)
    {
      PWM_t.counter = 0;
      flag.PwmArrayEmpty = 1;
    }
  }
}


void ConversionADCValueToPWMDuty(uint16_t *val)
{
  uint32_t Temp = 0;
  uint32_t PWM_ARR=__HAL_TIM_GET_AUTORELOAD(&htim3);
  for (int i = 0; i < AdcArraySize; i++)
  {
    Temp = val[i];
    Temp = (Temp * 1000) / 4095;
    //        Temp=100-Temp;
    Temp = (PWM_ARR)*Temp;
    Temp = Temp / 1000;
    val[i] = (uint16_t)(Temp);
  }
}
