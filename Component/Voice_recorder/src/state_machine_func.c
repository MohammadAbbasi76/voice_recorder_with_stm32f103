#include "state_machine.h"




void FlashEraseFunc()
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
  init_value();
  HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_RESET);
}
void RecordLED_ON()
{
  HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_SET);
}
void RecordLED_OFF()
{
  HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_RESET);
}
void PalyLED_ON()
{
  HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_SET);
}
void PlayLED_OFF()
{
  HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_RESET);
}
void SevenSegmentDisplay(uint8_t number)
{
  uint8_t a = 0;
  a = number & 0b00000001;
  if (a == 1)
    HAL_GPIO_WritePin(BCD_A_GPIO_Port, BCD_A_Pin, GPIO_PIN_SET);
  else
    HAL_GPIO_WritePin(BCD_A_GPIO_Port, BCD_A_Pin, GPIO_PIN_RESET);
  a = 0;
  a = number & 0b00000010;
  if (a == 2)
    HAL_GPIO_WritePin(BCD_B_GPIO_Port, BCD_B_Pin, GPIO_PIN_SET);
  else
    HAL_GPIO_WritePin(BCD_B_GPIO_Port, BCD_B_Pin, GPIO_PIN_RESET);
  a = 0;
  a = number & 0b00000100;
  if (a == 4)
    HAL_GPIO_WritePin(BCD_C_GPIO_Port, BCD_C_Pin, GPIO_PIN_SET);
  else
    HAL_GPIO_WritePin(BCD_C_GPIO_Port, BCD_C_Pin, GPIO_PIN_RESET);
  a = 0;
  a = number & 0b00001000;
  if (a == 8)
    HAL_GPIO_WritePin(BCD_D_GPIO_Port, BCD_D_Pin, GPIO_PIN_SET);
  else
    HAL_GPIO_WritePin(BCD_D_GPIO_Port, BCD_D_Pin, GPIO_PIN_RESET);
}
void StopRecording()
{
  HAL_TIM_Base_Stop_IT(&htim2);
  ADC_t.StopTimeCounter = (ADC_t.TotallyStopTim + 2);
}

void DummyFunc()
{
  int a = 0;
  for (int i = 0; i < 10000; i++)
  {
    a++;
  }
}


void choose_AudioOutput(AudioOutput out)
{
  if (out == Speaker)
  {
    HAL_GPIO_WritePin(Relay_SW_GPIO_Port, Relay_SW_Pin, GPIO_PIN_SET);
  }
  else if (out == M66_mic_in)
  {
    HAL_GPIO_WritePin(Relay_SW_GPIO_Port, Relay_SW_Pin, GPIO_PIN_RESET);
  }
}

void NextPinFunc()
{
  int a = (LastTimeKeyPress - HAL_GetTick());
  if (a < 5)
  {
    LastTimeKeyPress = 0;
    WitchVoiceWantToPlay++;
    // if (WitchVoiceWannaToPlay > (FindFreeSpceInflash(voice.WitchVoiceIsRecord) + 1))
    if (WitchVoiceWantToPlay > (MaxNumberOfVoice - 1))
    {
      WitchVoiceWantToPlay = 1;
    }
    SevenSegmentDisplay(WitchVoiceWantToPlay);
    state = ChooseVoiceForPlay;
    LastTimeKeyPress = DebounceTime + HAL_GetTick();
  }
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
      conv_adc_val_to_pwm_duty(Buffer1); // may be need RTOS
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
void AdcGettingSample()
{
  if (ADC_t.StopTimeCounter == ADC_t.TotallyStopTim)
  {
    ADC_t.StopTimeCounter++;
    for (int i = 0; i < AdcArraySize; i++)
    {
      Buffer1[i] = Buffer2[i];
      Buffer2[i] = 0;
    }

    ADC_t.counter = 0;
    flag.AdcArrayFull = 1;
  }
  else
  {
    // Buffer2[adc_stru.counter] = HAL_ADC_GetValue(&hadc1);
    Buffer2[ADC_t.counter] = ADC_read();
    ADC_t.counter++;
    ADC_t.StopTimeCounter++;
    if (ADC_t.counter == (AdcArraySize))
    {
      ADC_t.counter = 0;
      for (int i = 0; i < AdcArraySize; i++)
      {
        Buffer1[i] = Buffer2[i];
        Buffer2[i] = 0;
      }
      flag.AdcArrayFull = 1;
    }
  }
}
void interrupt_func(void)
{
  if (flag.InterruptSwitch == 1)
  {
    AdcGettingSample();
  }
  else
  {
    pwm_part();
  }
}
void pwm_part()
{
  if (flag.PwmArrayEmpty == 0)
  {
    __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, (uint16_t)(Buffer1[PWM_t.counter]));
    PWM_t.counter++;
    //      PWM_t.CountDataFromTotally[voice.number]--;
    if (PWM_t.counter == AdcArraySize)
    {
      PWM_t.counter = 0;
      flag.PwmArrayEmpty = 1;
    }
  }
}
#ifdef Print
void FlashPrint()
{
  uint8_t a = 0;
  for (int k = 0; k < 120; k++)
  {
    restore_2k_array(0, a, Buffer2);
    a++;
    for (int i = 0; i < 2000; i++)
      UART_Printf("%d\n", Buffer2[i]);
  }
}
#endif

void conv_adc_val_to_pwm_duty(uint16_t *val)
{
  uint32_t a = 0;
  for (int i = 0; i < AdcArraySize; i++)
  {
    a = val[i];
    a = (a * 1000) / 4095;
    //        a=100-a;
    a = (ccr_pwm_val)*a;
    a = a / 1000;
    val[i] = (uint16_t)(a);
  }
}
