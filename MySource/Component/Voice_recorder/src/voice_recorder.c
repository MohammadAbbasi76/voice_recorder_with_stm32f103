#include "voice_recorder.h"

uint32_t timing_1 = 0;

void VoiceRecorder()
{
  switch (VoiceRecorderSt.State)
  {
  case Init:
  {
    VoiceRecorderInitiation();
    VoiceRecorderSt.State = ReadKeyboardState;
    break;
  }
  case ReadKeyboardState:
  {
    VoiceRecorderSt.State = ReadKeyboardState;
    break;
  }
  case RecordState:
  {
    StartRecording();
    VoiceRecorderSt.State = ReadKeyboardState;
    break;
  }
  case PlayState:
  {
    StartPlaying();
    VoiceRecorderSt.State = ReadKeyboardState;
    break;
  }
  case FlashEraseState:
  {
    FlashErase();
    VoiceRecorderSt.State = ReadKeyboardState;
    break;
  }
  }
}
void RestoreInformationFromFlash()
{
  RestoreDetail(VoiceRecorderSt.Voice.RecordedArray, VoiceRecorderSt.Voice.number, &(VoiceRecorderSt.Voice.CountOfSavedArray));
  if (VoiceRecorderSt.Voice.RecordedArray[0] == 255)
  {
    VoiceRecorderSt.Voice.number = 0;
    for (uint8_t i = 0; i < MaxNumberOfVoice; i++)
      VoiceRecorderSt.Voice.RecordedArray[i] = 0;
    VoiceRecorderSt.Voice.CountOfSavedArray = 0;
  }
}
void VoiceRecorderInitiation()
{
  VoiceRecorderSt.ADC.Counter = 0;
  VoiceRecorderSt.ADC.TotallyStopTim = SampleRate * StopTimeInSec;
  VoiceRecorderSt.ADC.StopTimeCounter = 0;
  VoiceRecorderSt.Voice.CountOfSavedArray = 0;
  VoiceRecorderSt.Voice.number = 0;
  VoiceRecorderSt.Flag.InterruptSwitch = 1;
  VoiceRecorderSt.Flag.PwmArrayEmpty = 0;
  VoiceRecorderSt.Flag.AdcArrayFull = 0;
  VoiceRecorderSt.PWM.Counter = 0;
  memset(VoiceRecorderSt.PWM.CountDataFromTotally, 0x0, sizeof(VoiceRecorderSt.PWM.CountDataFromTotally));
  memset(VoiceRecorderSt.Voice.RecordedArray, 0x0, sizeof(VoiceRecorderSt.Voice.RecordedArray));
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
  VoiceRecorderSt.Track = 1;
  Blinking();
  for (uint8_t i = 0; i < 9; i++)
  {
    HAL_Delay(500);
    SevenSegmentDisplay(i);
    HAL_Delay(500);
  }
  SevenSegmentDisplay(VoiceRecorderSt.Track);
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
  VoiceRecorderInitiation();
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

void InterruptFunc(void)
{
  if (VoiceRecorderSt.Flag.InterruptSwitch == 1)
  {
    ADC_Sampling();
  }
  else
  {
    MakePwmWave();
  }
}
