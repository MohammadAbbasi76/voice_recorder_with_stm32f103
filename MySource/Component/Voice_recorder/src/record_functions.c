#include "record_functions.h"

void StopRecording()
{
    HAL_TIM_Base_Stop_IT(&htim2);
    ADC_t.StopTimeCounter = (ADC_t.TotallyStopTim + 2);
}

void StartRecording()
{
    // voice.number = NextFreeSpaceInFlash(voice.WitchVoiceIsRecord);
    voice_t.number = WitchVoiceWantToPlay;
    if (voice_t.WitchVoiceIsRecord[voice_t.number] == 1)
    {
        SevenSegmentDisplay(0);
        RemoveVoice(voice_t.WitchVoiceIsRecord, voice_t.number);
    }
    RecordLED_ON();
    SevenSegmentDisplay(voice_t.number);
    voice_t.number = WitchVoiceWantToPlay;
    MX_ADC1_Init();
    HAL_ADCEx_Calibration_Start(&hadc1);
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    flag.InterruptSwitch = 1;
    voice_t.ArrayGoToSave = 0;
    flag.AdcArrayFull = 0;
    HAL_TIM_Base_Start_IT(&htim2);
    while (1)
    {
        if (flag.AdcArrayFull == 1)
        {
            ConversionADCValueToPWMDuty(Buffer1); // may be need RTOS
            save_2k_array(voice_t.number, voice_t.ArrayGoToSave, Buffer1);
            voice_t.ArrayGoToSave++;
            flag.AdcArrayFull = 0;
        }
        if (ADC_t.StopTimeCounter >= (ADC_t.TotallyStopTim + 1))
        {
            voice_t.WitchVoiceIsRecord[voice_t.number] = 1;
            SaveDetail(voice_t.WitchVoiceIsRecord, voice_t.number, voice_t.ArrayGoToSave);
            break;
        }
    }
    HAL_TIM_Base_Stop_IT(&htim2);
    // voice.number++;
    // WitchVoiceWantToPlay = voice.number;
    voice_t.ArrayGoToSave = 0;
    ADC_t.StopTimeCounter = 0;
    HAL_ADC_Stop(&hadc1);
    RecordLED_OFF();
    //  HAL_Delay(1000);
}