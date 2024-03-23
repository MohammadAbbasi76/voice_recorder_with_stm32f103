
#include"plays_functions.h"


void LoopPLayFunc()
{
  uint32_t TT1 = 0;
  TT1 = HAL_GetTick() + PlayLoopTime;
  while (1)
  {
    if (FLGForVoiceStop == 1)
      break;
    if (((TT1 - HAL_GetTick()) < 5))
    {
      break;
    }
    SetupForPlay(NumberOfVoiceForLoopPlay);
    PlayStateFun();
    SetupForPlay(9);
    PlayStateFun();
  }
  FLGForVoiceStop = 0;
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