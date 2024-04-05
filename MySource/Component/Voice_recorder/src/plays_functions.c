#include "plays_functions.h"

void PrepareForPlay(uint8_t VoiceNumber)
{
    if (VoiceRecorderSt.Voice.RecodedArray[VoiceNumber] == 1)
    {
        VoiceRecorderSt.Voice.number = VoiceNumber;
        VoiceRecorderSt.Flag.InterruptSwitch = 0;
        VoiceRecorderSt.Flag.PwmArrayEmpty = 0;
        VoiceArrayReadFromFlash = 0;
        for (int i = 0; i < VoiceArraySize; i++)
        {
            Buffer2[i] = 0;
        }
        RestoreDetail(VoiceRecorderSt.Voice.RecodedArray, VoiceRecorderSt.Voice.number, &(VoiceRecorderSt.Voice.CountOfArraySaved));
        RestoreArrayFromFlash(VoiceRecorderSt.Voice.number, VoiceArrayReadFromFlash, Buffer2);
        VoiceRecorderSt.PWM.CountDataFromTotally[VoiceRecorderSt.Voice.number] = (uint32_t)((VoiceRecorderSt.Voice.CountOfArraySaved) * (VoiceArraySize));
        for (int i = 0; i < VoiceArraySize; i++)
        {
            Buffer1[i] = Buffer2[i];
        }
    }
    else
    {
        VoiceRecorderSt.Voice.number = VoiceNumber;
        VoiceRecorderSt.Flag.InterruptSwitch = 0;
        VoiceRecorderSt.Flag.PwmArrayEmpty = 0;
        VoiceArrayReadFromFlash = 0;
        for (int i = 0; i < VoiceArraySize; i++)
        {
            Buffer2[i] = 0;
        }
        for (int i = 0; i < VoiceArraySize; i++)
        {
            Buffer1[i] = 0;
            Buffer2[i] = 0;
        }
        StopPlaying();
    }
}
void StopPlaying()
{
    // VoiceRecorderSt.Flag.InterruptSwitch = 0;
    // VoiceRecorderSt.Flag.PwmArrayEmpty = 0;
    // VoiceArrayReadFromFlash = 0;
    // memset(Buffer2, 0x0, VoiceArraySize);
    // memset(Buffer1, 0x0, VoiceArraySize);
    HAL_TIM_Base_Stop_IT(&htim2);
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
    VoiceArrayReadFromFlash = VoiceRecorderSt.Voice.CountOfArraySaved;
}

void StartPlaying()
{
    PalyLED_ON();
    PrepareForPlay(WitchVoiceWantToPlay);
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); // Start Pwm signal on PB-6 Pin
    uint8_t dataReadyFlag = 0;
    while (1)
    {
        if (VoiceArrayReadFromFlash == VoiceRecorderSt.Voice.CountOfArraySaved)
        {
            break;
        }
        if (VoiceRecorderSt.Flag.PwmArrayEmpty == 0)
        {
            if (dataReadyFlag == 0)
            {
                VoiceArrayReadFromFlash++;
                RestoreArrayFromFlash(VoiceRecorderSt.Voice.number, VoiceArrayReadFromFlash, Buffer2);
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
    VoiceArrayReadFromFlash = 0;
    HAL_TIM_Base_Stop_IT(&htim2);
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
    PlayLED_OFF();
}

void NextTrack()
{
    WitchVoiceWantToPlay++;
    // if (WitchVoiceWannaToPlay > (FindFreeSpceInflash(voice.RecodedArray) + 1))
    if (WitchVoiceWantToPlay > (MaxNumberOfVoice - 1))
    {
        WitchVoiceWantToPlay = 1;
    }
    SevenSegmentDisplay(WitchVoiceWantToPlay);
    state = ChooseTrack;
}

void MakePWM_Wave()
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