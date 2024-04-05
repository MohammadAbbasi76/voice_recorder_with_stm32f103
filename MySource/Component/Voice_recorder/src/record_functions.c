#include "record_functions.h"

void StopRecording()
{
    HAL_TIM_Base_Stop_IT(&htim2);
    VoiceRecorderSt.ADC.StopTimeCounter = (VoiceRecorderSt.ADC.TotallyStopTim + 2);
}

void StartRecording()
{
    // voice.number = NextFreeSpaceInFlash(voice.VoiceRecoredArray);
    VoiceRecorderSt.Voice.number = VoiceRecorderSt.Track;
    if (VoiceRecorderSt.Voice.RecordedArray[VoiceRecorderSt.Voice.number] == 1)
    {
        SevenSegmentDisplay(0);
        RemoveVoice(VoiceRecorderSt.Voice.RecordedArray, VoiceRecorderSt.Voice.number);
    }
    RecordLED_ON();
    SevenSegmentDisplay(VoiceRecorderSt.Voice.number);
    VoiceRecorderSt.Voice.number = VoiceRecorderSt.Track;
    MX_ADC1_Init();
    HAL_ADCEx_Calibration_Start(&hadc1);
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    VoiceRecorderSt.Flag.InterruptSwitch = 1;
    VoiceRecorderSt.Voice.CountOfSavedArray = 0;
    VoiceRecorderSt.Flag.AdcArrayFull = 0;
    HAL_TIM_Base_Start_IT(&htim2);
    while (1)
    {
        if (VoiceRecorderSt.Flag.AdcArrayFull == 1)
        {
            ConversionADCValueToPWMDuty(Buffer1); // may be need RTOS
            save_2k_array(VoiceRecorderSt.Voice.number, VoiceRecorderSt.Voice.CountOfSavedArray, Buffer1);
            VoiceRecorderSt.Voice.CountOfSavedArray++;
            VoiceRecorderSt.Flag.AdcArrayFull = 0;
        }
        if (VoiceRecorderSt.ADC.StopTimeCounter >= (VoiceRecorderSt.ADC.TotallyStopTim + 1))
        {
            VoiceRecorderSt.Voice.RecordedArray[VoiceRecorderSt.Voice.number] = 1;
            SaveDetail(VoiceRecorderSt.Voice.RecordedArray, VoiceRecorderSt.Voice.number, VoiceRecorderSt.Voice.CountOfSavedArray);
            break;
        }
    }
    HAL_TIM_Base_Stop_IT(&htim2);
    // voice.number++;
    // VoiceRecorderSt.Track = voice.number;
    VoiceRecorderSt.Voice.CountOfSavedArray = 0;
    VoiceRecorderSt.ADC.StopTimeCounter = 0;
    HAL_ADC_Stop(&hadc1);
    RecordLED_Off();
    //  HAL_Delay(1000);
}
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