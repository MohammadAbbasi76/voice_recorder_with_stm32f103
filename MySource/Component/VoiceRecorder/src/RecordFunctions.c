#include "RecordFunctions.h"

/**
 * @brief Stops the recording process.
 * This function stops the recording timer and updates the stop time counter.
 * @return void
 */
void StopRecording()
{
  HAL_TIM_Base_Stop_IT(&htim2);
  VoiceRecorderSt.ADC.StopTimeCounter = (VoiceRecorderSt.ADC.TotallyStopTim + 2);
}

/**
 * @brief Configures ADC for recording.
 * This function initializes and calibrates the ADC, then starts it and waits for conversion.
 * @return void
 */
void AdcConfigForRecord()
{
  MX_ADC1_Init();
  HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_ADC_Start(&hadc1);
  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
}

/**
 * @brief Prepares the system to start recording.
 * This function turns on the recording LED, configures ADC, and sets initial recording state.
 * @return void
 */
void PrepareToRecord()
{
  RecordLedOn();
  RecordLedOn();
  AdcConfigForRecord();
  VoiceRecorderSt.Flag.InterruptSwitch = 1;
  VoiceRecorderSt.Voice.CountOfSavedArray = 0;
  VoiceRecorderSt.Flag.AdcArrayFull = 0;
  HAL_TIM_Base_Start_IT(&htim2);
}

/**
 * @brief Starts the recording process.
 * This function prepares the system to record and enters a loop to handle ADC sampling and data storage.
 * @return void
 */
void StartRecording()
{
  PrepareToRecord();
  while (true)
  {
    if (VoiceRecorderSt.Flag.AdcArrayFull == 1)
    {
      ConversionADCValueToPWMDuty(Buffer1);
      W25q_SaveBufferArray(VoiceRecorderSt.Track, VoiceRecorderSt.Voice.CountOfSavedArray, Buffer1);
      VoiceRecorderSt.Voice.CountOfSavedArray++;
      VoiceRecorderSt.Flag.AdcArrayFull = 0;
    }
    if (VoiceRecorderSt.ADC.StopTimeCounter >= (VoiceRecorderSt.ADC.TotallyStopTim + 1))
    {
      VoiceRecorderSt.Voice.RecordedArray[VoiceRecorderSt.Track] = 1;
      W25q_SaveDetail(VoiceRecorderSt.Voice.RecordedArray, VoiceRecorderSt.Track, VoiceRecorderSt.Voice.CountOfSavedArray);
      break;
    }
  }
  HAL_TIM_Base_Stop_IT(&htim2);
  VoiceRecorderSt.Voice.CountOfSavedArray = 0;
  VoiceRecorderSt.ADC.StopTimeCounter = 0;
  HAL_ADC_Stop(&hadc1);
  RecordLedOff();
}

/**
 * @brief Handles ADC sampling.
 * This function samples ADC values, updates buffers, and sets flags when buffers are full.
 * @return void
 */
void ADC_Sampling()
{
  if (VoiceRecorderSt.ADC.StopTimeCounter == VoiceRecorderSt.ADC.TotallyStopTim)
  {
    VoiceRecorderSt.ADC.StopTimeCounter++;
    for (int i = 0; i < VoiceArraySize; i++)
    {
      Buffer1[i] = Buffer2[i];
      Buffer2[i] = 0;
    }
    VoiceRecorderSt.ADC.Counter = 0;
    VoiceRecorderSt.Flag.AdcArrayFull = 1;
  }
  else
  {
    // Buffer2[adc_stru.Counter] = HAL_ADC_GetValue(&hadc1);
    Buffer2[VoiceRecorderSt.ADC.Counter] = HAL_ADC_GetValue(&hadc1);
    VoiceRecorderSt.ADC.Counter++;
    VoiceRecorderSt.ADC.StopTimeCounter++;
    if (VoiceRecorderSt.ADC.Counter == (VoiceArraySize))
    {
      VoiceRecorderSt.ADC.Counter = 0;
      for (int i = 0; i < VoiceArraySize; i++)
      {
        Buffer1[i] = Buffer2[i];
        Buffer2[i] = 0;
      }
      VoiceRecorderSt.Flag.AdcArrayFull = 1;
    }
  }
}