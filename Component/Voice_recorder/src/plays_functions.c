
#include "plays_functions.h"

void SetupForPlay(uint8_t VoiceNumber)
{
    if (voice_t.WitchVoiceIsRecord[VoiceNumber] == 1)
    {
        voice_t.number = VoiceNumber;
        flag.InterruptSwitch = 0;
        flag.PwmArrayEmpty = 0;
        VoiceArrayReadFromFlash = 0;
        for (int i = 0; i < AdcArraySize; i++)
        {
            Buffer2[i] = 0;
        }
        RestoreDetail(voice_t.WitchVoiceIsRecord, voice_t.number, &(voice_t.ArrayGoToSave));
        restore_2k_array(voice_t.number, VoiceArrayReadFromFlash, Buffer2);
        PWM_t.CountDataFromTotally[voice_t.number] = (uint32_t)((voice_t.ArrayGoToSave) * (AdcArraySize));
        for (int i = 0; i < AdcArraySize; i++)
        {
            Buffer1[i] = Buffer2[i];
        }
    }
    else
    {
        voice_t.number = VoiceNumber;
        flag.InterruptSwitch = 0;
        flag.PwmArrayEmpty = 0;
        VoiceArrayReadFromFlash = 0;
        for (int i = 0; i < AdcArraySize; i++)
        {
            Buffer2[i] = 0;
        }
        for (int i = 0; i < AdcArraySize; i++)
        {
            Buffer1[i] = 0;
            Buffer2[i] = 0;
        }
        StopPlaying();
    }
}
void StopPlaying()
{
    HAL_TIM_Base_Stop_IT(&htim2);
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
    VoiceArrayReadFromFlash = voice_t.ArrayGoToSave;
}


void PlayStateFun()
{
    PalyLED_ON();
    SetupForPlay(WitchVoiceWantToPlay);
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); // Start Pwm signal on PB-6 Pin
    uint8_t flg_use = 0;
    while (1)
    {
        if (VoiceArrayReadFromFlash == voice_t.ArrayGoToSave)
        {
            break;
        }
        if (flag.PwmArrayEmpty == 0)
        {
            if (flg_use == 0)
            {
                VoiceArrayReadFromFlash++;
                restore_2k_array(voice_t.number, VoiceArrayReadFromFlash, Buffer2);
                flg_use = 1;
            }
        }
        else
        {
            for (int i = 0; i < AdcArraySize; i++)
            {
                Buffer1[i] = Buffer2[i];
                Buffer2[i] = 0;
            }
            flag.PwmArrayEmpty = 0;
            flg_use = 0;
        }
    }
    VoiceArrayReadFromFlash = 0;
    HAL_TIM_Base_Stop_IT(&htim2);
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
    PlayLED_OFF();
}