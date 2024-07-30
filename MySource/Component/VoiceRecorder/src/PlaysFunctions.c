#include "PlaysFunctions.h"

/**
 * @brief Prepares the system for playback.
 * This function initializes flags and buffers, and restores data from flash memory if a track is recorded.
 * @return void
 */
void PrepareToPlay()
{
    VoiceRecorderSt.Flag.InterruptSwitch = 0;
    VoiceRecorderSt.Flag.PwmArrayEmpty = 0;
    VoiceRecorderSt.ReadFromFlash = 0;
    if (VoiceRecorderSt.Voice.RecordedArray[VoiceRecorderSt.Track] == 1)
    {
        memset(Buffer1, 0x0, VoiceArraySize);
        W25q_RestoreDetail(VoiceRecorderSt.Voice.RecordedArray, VoiceRecorderSt.Track, &(VoiceRecorderSt.Voice.CountOfSavedArray));
        W25q_RestoreArrayFromFlash(VoiceRecorderSt.Track, VoiceRecorderSt.ReadFromFlash, Buffer1);
        VoiceRecorderSt.PWM.CountDataFromTotally[VoiceRecorderSt.Track] = (uint32_t)((VoiceRecorderSt.Voice.CountOfSavedArray) * (VoiceArraySize));
    }
    else
    {
        memset(Buffer1, 0x0, VoiceArraySize);
        memset(Buffer2, 0x0, VoiceArraySize);
        StopPlaying();
    }
}

/**
 * @brief Stops the playback.
 * This function stops the timers and PWM, and sets the read position to the end of the saved data.
 * @return void
 */
void StopPlaying()
{
    HAL_TIM_Base_Stop_IT(&htim2);
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
    VoiceRecorderSt.ReadFromFlash = VoiceRecorderSt.Voice.CountOfSavedArray;
}

/**
 * @brief Starts the playback process.
 * This function turns on the playback LED, prepares the system for playback, starts timers and PWM, 
 * and manages buffer switching during playback.
 * @return void
 */
void StartPlaying()
{
    PalyLedOn();
    PrepareToPlay();
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); // Start Pwm signal on PB-6 Pin
    uint8_t dataReadyFlag = 0;
    while (1)
    {
        if (VoiceRecorderSt.ReadFromFlash == VoiceRecorderSt.Voice.CountOfSavedArray)
        {
            break;
        }
        if (VoiceRecorderSt.Flag.PwmArrayEmpty == 0)
        {
            if (dataReadyFlag == 0)
            {
                VoiceRecorderSt.ReadFromFlash++;
                W25q_RestoreArrayFromFlash(VoiceRecorderSt.Track, VoiceRecorderSt.ReadFromFlash, Buffer2);
                dataReadyFlag = 1;
            }
        }
        else
        {
            for (int i = 0; i < VoiceArraySize; i++)
            {
                Buffer1[i] = Buffer2[i];
                Buffer2[i] = 0;
            }
            VoiceRecorderSt.Flag.PwmArrayEmpty = 0;
            dataReadyFlag = 0;
        }
    }
    VoiceRecorderSt.ReadFromFlash = 0;
    HAL_TIM_Base_Stop_IT(&htim2);
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
    PlayLedOff();
}

/**
 * @brief Generates the PWM wave for playback.
 * This function sets the PWM compare value based on the buffer and updates the buffer status.
 * @return void
 */
void MakePwmWave()
{
    if (VoiceRecorderSt.Flag.PwmArrayEmpty == 0)
    {
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, (uint16_t)(Buffer1[VoiceRecorderSt.PWM.Counter]));
        VoiceRecorderSt.PWM.Counter++;
        //      VoiceRecorderSt.PWM.CountDataFromTotally[voice.number]--;
        if (VoiceRecorderSt.PWM.Counter == VoiceArraySize)
        {
            VoiceRecorderSt.PWM.Counter = 0;
            VoiceRecorderSt.Flag.PwmArrayEmpty = 1;
        }
    }
}

/**
 * @brief Converts ADC values to PWM duty cycle values.
 * This function converts an array of ADC values to corresponding PWM duty cycle values.
 * @param Array Pointer to the array of ADC values.
 * @return void
 */
void ConversionADCValueToPWMDuty(uint16_t *Array)
{
    uint32_t Temp = 0;
    uint32_t PWM_ARR = __HAL_TIM_GET_AUTORELOAD(&htim3);
    for (int i = 0; i < VoiceArraySize; i++)
    {
        Temp = Array[i];
        Temp = (Temp * 1000) / 4095;
        //        Temp=100-Temp;
        Temp = (PWM_ARR)*Temp;
        Temp = Temp / 1000;
        Array[i] = (uint16_t)(Temp);
    }
}