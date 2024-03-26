#include "voice_recorder.h"

uint32_t timing_1 = 0;

void VoiceRecorder()
{
  switch (state)
  {
  case ReadKeyboardState:
  {
    state = ReadKeyboardState;
    break;
  }
  case ChooseTrack:
  {
    ChooseVoiceForPlay();
    state = PlayState;
    break;
  }
  case RecordState:
  {
    StartRecording();
    state = ReadKeyboardState;
    break;
  }
  case PlayState:
  {
    StartPlaying();
    state = ReadKeyboardState;
    break;
  }
  case FlashEraseState:
  {
    FlashErase();
    state = ReadKeyboardState;
    break;
  }
  }
}
void RestoreInformationFromFlash()
{
  RestoreDetail(voice_t.WitchVoiceIsRecord, voice_t.number, &(voice_t.ArrayGoToSave));
  if (voice_t.WitchVoiceIsRecord[0] == 255)
  {
    voice_t.number = 0;
    for (uint8_t i = 0; i < MaxNumberOfVoice; i++)
      voice_t.WitchVoiceIsRecord[i] = 0;
    voice_t.ArrayGoToSave = 0;
  }
}
void init_value()
{
  ADC_t.counter = 0;
  ADC_t.TotallyStopTim = SampleRate * StopTimeInSec;
  ADC_t.StopTimeCounter = 0;
  voice_t.ArrayGoToSave = 0;
  voice_t.number = 0;
  flag.InterruptSwitch = 1;
  flag.PwmArrayEmpty = 0;
  flag.AdcArrayFull = 0;
  PWM_t.counter = 0;
  memset(PWM_t.CountDataFromTotally, 0x0, MaxNumberOfVoice);
  memset(voice_t.WitchVoiceIsRecord, 0x0, MaxNumberOfVoice);
  HAL_ADCEx_Calibration_Start(&hadc1);
  if (!W25qxx_Init())
  {
    while (1)
    {
    }
    // flash have problem !
  }
  //  W25qxx_EraseChip();
  RestoreInformationFromFlash();
  state = ReadKeyboardState;
  WitchVoiceWantToPlay = 1;
  Blinking();
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
    memset(Buffer1, 0x0, VoiceArraySize);
    RestoreDetail(voice_t.WitchVoiceIsRecord, voice_t.number, &(voice_t.ArrayGoToSave));
    RestoreArrayFromFlash(voice_t.number, VoiceArrayReadFromFlash, Buffer1);
    PWM_t.CountDataFromTotally[voice_t.number] = (uint32_t)((voice_t.ArrayGoToSave) * (VoiceArraySize));
  }
  else
  {

    StopPlaying();
  }
}

void FlashErase()
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

void AudioOutputControl(AudioOutput AudioOutputValue)
{
  if (AudioOutputValue == TurnOnSpeaker)
  {
    HAL_GPIO_WritePin(Relay_SW_GPIO_Port, Relay_SW_Pin, GPIO_PIN_SET);
  }
  else if (AudioOutputValue == TurnOffSpeaker)
  {
    HAL_GPIO_WritePin(Relay_SW_GPIO_Port, Relay_SW_Pin, GPIO_PIN_RESET);
  }
}

void NextTrack()
{
  WitchVoiceWantToPlay++;
  // if (WitchVoiceWannaToPlay > (FindFreeSpceInflash(voice.WitchVoiceIsRecord) + 1))
  if (WitchVoiceWantToPlay > (MaxNumberOfVoice - 1))
  {
    WitchVoiceWantToPlay = 1;
  }
  SevenSegmentDisplay(WitchVoiceWantToPlay);
  state = ChooseTrack;
}

void ADCSampling()
{
  if (ADC_t.StopTimeCounter == ADC_t.TotallyStopTim)
  {
    ADC_t.StopTimeCounter++;
    for (int i = 0; i < VoiceArraySize; i++)
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
    if (ADC_t.counter == (VoiceArraySize))
    {
      ADC_t.counter = 0;
      for (int i = 0; i < VoiceArraySize; i++)
      {
        Buffer1[i] = Buffer2[i];
        Buffer2[i] = 0;
      }
      flag.AdcArrayFull = 1;
    }
  }
}
void InterruptFunc(void)
{
  if (flag.InterruptSwitch == 1)
  {
    ADCSampling();
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
    if (PWM_t.counter == VoiceArraySize)
    {
      PWM_t.counter = 0;
      flag.PwmArrayEmpty = 1;
    }
  }
}

void ConversionADCValueToPWMDuty(uint16_t *val)
{
  uint32_t Temp = 0;
  uint32_t PWM_ARR = __HAL_TIM_GET_AUTORELOAD(&htim3);
  for (int i = 0; i < VoiceArraySize; i++)
  {
    Temp = val[i];
    Temp = (Temp * 1000) / 4095;
    //        Temp=100-Temp;
    Temp = (PWM_ARR)*Temp;
    Temp = Temp / 1000;
    val[i] = (uint16_t)(Temp);
  }
}
