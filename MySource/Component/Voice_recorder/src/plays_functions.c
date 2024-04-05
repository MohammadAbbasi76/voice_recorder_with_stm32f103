#include "plays_functions.h"

void PrepareForPlay(uint8_t VoiceNumber)
{
    if (VoiceRecorderSt.Voice.RecordedArray[VoiceNumber] == 1)
    {
        VoiceRecorderSt.Voice.number = VoiceNumber;
        VoiceRecorderSt.Flag.InterruptSwitch = 0;
        VoiceRecorderSt.Flag.PwmArrayEmpty = 0;
        VoiceRecorderSt.ReadFromFlash = 0;
        memset(Buffer1, 0x0, VoiceArraySize);
        RestoreDetail(VoiceRecorderSt.Voice.RecordedArray, VoiceRecorderSt.Voice.number, &(VoiceRecorderSt.Voice.CountOfSavedArray));
        RestoreArrayFromFlash(VoiceRecorderSt.Voice.number, VoiceRecorderSt.ReadFromFlash, Buffer1);
        VoiceRecorderSt.PWM.CountDataFromTotally[VoiceRecorderSt.Voice.number] = (uint32_t)((VoiceRecorderSt.Voice.CountOfSavedArray) * (VoiceArraySize));
    }
    else
    {
        VoiceRecorderSt.Voice.number = VoiceNumber;
        VoiceRecorderSt.Flag.InterruptSwitch = 0;
        VoiceRecorderSt.Flag.PwmArrayEmpty = 0;
        VoiceRecorderSt.ReadFromFlash = 0;
        memset(Buffer1, 0x0, VoiceArraySize);
        memset(Buffer2, 0x0, VoiceArraySize);
        StopPlaying();
    }
}
void StopPlaying()
{
    // VoiceRecorderSt.Flag.InterruptSwitch = 0;
    // VoiceRecorderSt.Flag.PwmArrayEmpty = 0;
    // VoiceRecorderSt.ReadFromFlash = 0;
    // memset(Buffer2, 0x0, VoiceArraySize);
    // memset(Buffer1, 0x0, VoiceArraySize);
    HAL_TIM_Base_Stop_IT(&htim2);
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
    VoiceRecorderSt.ReadFromFlash = VoiceRecorderSt.Voice.CountOfSavedArray;
}

void StartPlaying()
{
    PalyLedOn();
    PrepareForPlay(VoiceRecorderSt.Track);
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
                RestoreArrayFromFlash(VoiceRecorderSt.Voice.number, VoiceRecorderSt.ReadFromFlash, Buffer2);
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