#include "VoiceRecorderInterface.h"

/**
 * @brief Main function for voice recorder state management.
 * This function handles the state machine for the voice recorder, managing various states such as initialization,
 * reading keyboard input, recording, playing, stopping, switching tracks, and erasing flash.
 * @return void
 */
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
    KeyBoard();
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
  case StopPlayingState:
  {
    StopPlaying();
    VoiceRecorderSt.State = ReadKeyboardState;
    break;
  }
  case StopRecordingState:
  {
    StopRecording();
    VoiceRecorderSt.State = ReadKeyboardState;
    break;
  }
  case NextTrackState:
  {
    NextTrack();
    VoiceRecorderSt.State = ReadKeyboardState;
    break;
  }
  case FlashEraseState:
  {
    RestFactoryFunction();
    VoiceRecorderSt.State = ReadKeyboardState;
    break;
  }
  }
}

/**
 * @brief Restores information from flash memory.
 * This function retrieves saved details from flash memory and updates the track and recorded array information.
 * @return void
 */
void RestoreInformationFromFlash()
{
  W25q_RestoreDetail(VoiceRecorderSt.Voice.RecordedArray, VoiceRecorderSt.Track, &(VoiceRecorderSt.Voice.CountOfSavedArray));
  if (VoiceRecorderSt.Voice.RecordedArray[0] == 255)
  {
    VoiceRecorderSt.Track = 0;
    for (uint8_t i = 0; i < MaxNumberOfVoice; i++)
      VoiceRecorderSt.Voice.RecordedArray[i] = 0;
    VoiceRecorderSt.Voice.CountOfSavedArray = 0;
  }
}

/**
 * @brief Initializes the voice recorder.
 * This function sets up the initial state of the voice recorder, including counters, flags, and hardware calibration.
 * @return void
 */
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
  VoiceRecorderSt.DeviceMode = NormalMode;
  VoiceRecorderSt.PWM.Counter = 0;
  memset(VoiceRecorderSt.PWM.CountDataFromTotally, 0x0, sizeof(VoiceRecorderSt.PWM.CountDataFromTotally));
  memset(VoiceRecorderSt.Voice.RecordedArray, 0x0, sizeof(VoiceRecorderSt.Voice.RecordedArray));
  HAL_ADCEx_Calibration_Start(&hadc1);
  W25q_TestingFlash();
  RestoreInformationFromFlash();
  VoiceRecorderSt.Track = 1;
  Blinking();
  SevenSegmentDisplay(VoiceRecorderSt.Track);
}

/**
 * @brief Restores the factory settings.
 * This function erases the flash memory, reinitializes the voice recorder, and updates the LED indicators.
 * @return void
 */
void RestFactoryFunction()
{
  HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_SET);
  W25q_TestingFlash();
  W25qxx_EraseChip();
  VoiceRecorderInitiation();
  HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_RESET);
}

/**
 * @brief Interrupt service routine.
 * This function handles the interrupt, deciding whether to sample ADC values or generate PWM waves based on the current mode.
 * @return void
 */
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

/**
 * @brief Switches to the next track.
 * This function increments the current track and updates the display. It wraps around if the maximum number of tracks is exceeded.
 * @return void
 */
void NextTrack()
{
  VoiceRecorderSt.Track++;
  if (VoiceRecorderSt.Track > (MaxNumberOfVoice - 1))
  {
    VoiceRecorderSt.Track = 1;
  }
  SevenSegmentDisplay(VoiceRecorderSt.Track);
}
