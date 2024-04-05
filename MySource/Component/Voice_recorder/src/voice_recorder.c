#include "voice_recorder.h"

uint32_t timing_1 = 0;

void VoiceRecorder()
{
  switch (state)
  {
  case Init:
  {
    VoiceRecorderInitiation();
    state = ReadKeyboardState;
    break;
  }
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
  RestoreDetail(VoiceRecorderSt.Voice.RecodedArray, VoiceRecorderSt.Voice.number, &(VoiceRecorderSt.Voice.CountOfArraySaved));
  if (VoiceRecorderSt.Voice.RecodedArray[0] == 255)
  {
    VoiceRecorderSt.Voice.number = 0;
    for (uint8_t i = 0; i < MaxNumberOfVoice; i++)
      VoiceRecorderSt.Voice.RecodedArray[i] = 0;
    VoiceRecorderSt.Voice.CountOfArraySaved = 0;
  }
}
void VoiceRecorderInitiation()
{
  VoiceRecorderSt.ADC.Counter = 0;
  VoiceRecorderSt.ADC.TotallyStopTim = SampleRate * StopTimeInSec;
  VoiceRecorderSt.ADC.StopTimeCounter = 0;
  VoiceRecorderSt.Voice.CountOfArraySaved = 0;
  VoiceRecorderSt.Voice.number = 0;
  VoiceRecorderSt.Flag.InterruptSwitch = 1;
  VoiceRecorderSt.Flag.PwmArrayEmpty = 0;
  VoiceRecorderSt.Flag.AdcArrayFull = 0;
  VoiceRecorderSt.PWM.Counter = 0;
  memset(VoiceRecorderSt.PWM.CountDataFromTotally, 0x0, sizeof(VoiceRecorderSt.PWM.CountDataFromTotally));
  memset(VoiceRecorderSt.Voice.RecodedArray, 0x0, sizeof(VoiceRecorderSt.Voice.RecodedArray));
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
  WitchVoiceWantToPlay = 1;
  Blinking();
  SevenSegmentDisplay(WitchVoiceWantToPlay);
}

void ChooseVoiceForPlay()
{
  VoiceRecorderSt.Voice.number = WitchVoiceWantToPlay;
  UART_Printf("VoiceRecorderSt.Voice.number =%d\n", VoiceRecorderSt.Voice.number);
  if (VoiceRecorderSt.Voice.RecodedArray[VoiceRecorderSt.Voice.number] == 1)
  {
    SevenSegmentDisplay(VoiceRecorderSt.Voice.number);
    VoiceRecorderSt.Flag.InterruptSwitch = 0;
    VoiceRecorderSt.Flag.PwmArrayEmpty = 0;
    VoiceArrayReadFromFlash = 0;
    memset(Buffer1, 0x0, sizeof(Buffer1));
    RestoreDetail(VoiceRecorderSt.Voice.RecodedArray, VoiceRecorderSt.Voice.number, &(VoiceRecorderSt.Voice.CountOfArraySaved));
    RestoreArrayFromFlash(VoiceRecorderSt.Voice.number, VoiceArrayReadFromFlash, Buffer1);
    VoiceRecorderSt.PWM.CountDataFromTotally[VoiceRecorderSt.Voice.number] = (uint32_t)((VoiceRecorderSt.Voice.CountOfArraySaved) * (VoiceArraySize));
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
    MakePWM_Wave();
  }
}
