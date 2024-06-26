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
    RestFactory();
    VoiceRecorderSt.State = ReadKeyboardState;
    break;
  }
  }
}
void RestoreInformationFromFlash()
{
  RestoreDetail(VoiceRecorderSt.Voice.RecordedArray, VoiceRecorderSt.Track, &(VoiceRecorderSt.Voice.CountOfSavedArray));
  if (VoiceRecorderSt.Voice.RecordedArray[0] == 255)
  {
    VoiceRecorderSt.Track = 0;
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
  VoiceRecorderSt.Track = 1;
  VoiceRecorderSt.Flag.InterruptSwitch = 1;
  VoiceRecorderSt.Flag.PwmArrayEmpty = 0;
  VoiceRecorderSt.Flag.AdcArrayFull = 0;
  VoiceRecorderSt.PWM.Counter = 0;
  memset(VoiceRecorderSt.PWM.CountDataFromTotally, 0x0, sizeof(VoiceRecorderSt.PWM.CountDataFromTotally));
  memset(VoiceRecorderSt.Voice.RecordedArray, 0x0, sizeof(VoiceRecorderSt.Voice.RecordedArray));
  HAL_ADCEx_Calibration_Start(&hadc1);
  TestingFlash();
  // W25qxx_EraseChip();
  RestoreInformationFromFlash();
  VoiceRecorderSt.Track = 1;
  Blinking();
  SevenSegmentDisplay(VoiceRecorderSt.Track);
}
void RestFactory()
{
  HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_SET);
  TestingFlash();
  W25qxx_EraseChip();
  VoiceRecorderInitiation();
  HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_RESET);
}
void InterruptFunc()
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
