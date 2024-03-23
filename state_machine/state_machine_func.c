#include "state_machine.h"
#include "main.h"
#include "tim.h"
#include "adc.h"
#include "usart.h"
#include "stm32f1xx_hal_gpio.h"
#include "string.h"
#include "w25qxx.h"
#include"Typedef.h"
extern uint16_t Buffer1[AdcArraySize];
extern uint16_t Buffer2[AdcArraySize];
extern struct adc_ adc_stru;
extern struct pwm_ pwm_stru;
extern struct voice_struc voice;
extern struct flags flag;
extern uint8_t InputKey;
extern uint8_t WitchVoiceWantToPlay;
extern uint8_t state;
extern uint32_t ccr_pwm_val;

extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart1;
extern ADC_HandleTypeDef hadc1;
extern uint32_t UartFLush_T1;
void UartFunNeed();
void init_value()
{
  HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_SET);
  /** adc init value*/

  adc_stru.counter = 0;
  flag.AdcArryFull = 0;
  adc_stru.TotallyStopTim = SampleRate * StopTimeInSec;
  adc_stru.StopTimeCounter = 0;
  voice.ArrayGoToSave = 0;
  voice.number = 0;
  FLGForVoiceStop = 0;
  for (int i = 0; i < MaxNumberOfVoice; i++)
  {
    voice.WitchVoiceIsRecord[i] = 0;
  }
  /**  timer2 state changer flag */
  flag.InterruptSwitch = 1;
  /** first state*/
  state = ReadKeyboardState;
  /**  fris val for keyboard value*/
  InputKey = non_;
  /** pwm init val*/
  pwm_stru.counter = 0;
  for (int i = 0; i < MaxNumberOfVoice; i++)
    pwm_stru.CountDataFromTotally[i] = 0;
  flag.PwmArrayEmpty = 0;
  state = ReadKeyboardState;
  WitchVoiceWantToPlay = 1;
  ccr_pwm_val = __HAL_TIM_GET_AUTORELOAD(&htim3);
  HAL_ADCEx_Calibration_Start(&hadc1);
  FirstTimeFLG = 1;
  if (!W25qxx_Init())
  {
    while (1)
    {
    }
    // flash have problem !
  }
  //  W25qxx_EraseChip();
  RestoreDetail(voice.WitchVoiceIsRecord, voice.number, &(voice.ArrayGoToSave));
  if (voice.WitchVoiceIsRecord[0] == 255)
  {
    voice.number = 0;
    for (uint8_t i = 0; i < MaxNumberOfVoice; i++)
      voice.WitchVoiceIsRecord[i] = 0;
    voice.ArrayGoToSave = 0;
  }
  for (uint8_t i = 0; i < UartArraySize; i++)
    UartReceive[i] = 0;
  HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_RESET);
  HAL_Delay(500);
  HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_SET);
  HAL_Delay(500);
  HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_RESET);
  HAL_Delay(500);
  HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_SET);
  HAL_Delay(500);
  HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_RESET);
  HAL_Delay(500);
  HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_SET);
  HAL_Delay(500);
  HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_RESET);
  UartFLush_T1 = HAL_GetTick() + UartTimeDataExpire + 900;
  for (int i = 0; i < UartArraySize; i++)
    UartReceive[i] = 0;
  LastTimeKeyPress = HAL_GetTick() + 100;
  NumberOfVoiceForLoopPlay = 0;
  SevenSegmentDisplay(WitchVoiceWantToPlay);
}
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
void UartReceiveFnc()
{
  HAL_UART_Transmit(&huart1, UartReceive, UartArraySize, 100);
  if (state != RecordState)
  {
    if ((strcmp((char *)UartReceive, "!P1") == 0) ||
        (strcmp((char *)UartReceive, "!P2") == 0) ||
        (strcmp((char *)UartReceive, "!P3") == 0) ||
        (strcmp((char *)UartReceive, "!P4") == 0) ||
        (strcmp((char *)UartReceive, "!P5") == 0) ||
        (strcmp((char *)UartReceive, "!P6") == 0) ||
        (strcmp((char *)UartReceive, "!P7") == 0) ||
        (strcmp((char *)UartReceive, "!P8") == 0) ||
        (strcmp((char *)UartReceive, "!P9") == 0) ||
        (strcmp((char *)UartReceive, "!P0") == 0))
    {
      NumberOfVoiceForLoopPlay = (uint8_t)(UartReceive[2] - '0');
      state = PLayLoop;
    }
    else if (strcmp((char *)UartReceive, "!ES") == 0)
    {
      state = FlashEraseState;
    }
    else if (strcmp((char *)UartReceive, "!PS") == 0)
    {
      FLGForVoiceStop = 1;
      StopPlaying();
    }
    else if (strcmp((char *)UartReceive, "!GR") == 0)
    {
      state = M66State;
    }
  }
  for (int i = 0; i < UartArraySize; i++)
    UartReceive[i] = 0;
  __HAL_UART_FLUSH_DRREGISTER(&huart1);
}

void UartFunNeed()
{
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
  adc_stru.StopTimeCounter = (adc_stru.TotallyStopTim + 2);
}
void StopPlaying()
{
  HAL_TIM_Base_Stop_IT(&htim2);
  HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
  VoiceArrayReadFromFlash = voice.ArrayGoToSave;
}
void DummyFunc()
{
  int a = 0;
  for (int i = 0; i < 10000; i++)
  {
    a++;
  }
}
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
void SetupForPlay(uint8_t VoiceNumber)
{
  if (voice.WitchVoiceIsRecord[VoiceNumber] == 1)
  {
    voice.number = VoiceNumber;
    flag.InterruptSwitch = 0;
    flag.PwmArrayEmpty = 0;
    VoiceArrayReadFromFlash = 0;
    for (int i = 0; i < AdcArraySize; i++)
    {
      Buffer2[i] = 0;
    }
    RestoreDetail(voice.WitchVoiceIsRecord, voice.number, &(voice.ArrayGoToSave));
    restore_2k_array(voice.number, VoiceArrayReadFromFlash, Buffer2);
    pwm_stru.CountDataFromTotally[voice.number] = (uint32_t)((voice.ArrayGoToSave) * (AdcArraySize));
    for (int i = 0; i < AdcArraySize; i++)
    {
      Buffer1[i] = Buffer2[i];
    }
  }
  else
  {
    voice.number = VoiceNumber;
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
void ChooseVoiceForPlayfunc()
{
  // HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_SET);
  // HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_SET);
  // voice.number = HowManyVoiceIsRecord(
  //     WitchVoiceWannaToPlay, voice.WitchVoiceIsRecord);
  voice.number = WitchVoiceWantToPlay;
  UART_Printf("voice.number =%d\n", voice.number);
  SevenSegmentDisplay(voice.number);
  if (voice.WitchVoiceIsRecord[voice.number] == 1)
  {
    SevenSegmentDisplay(voice.number);
    flag.InterruptSwitch = 0;
    flag.PwmArrayEmpty = 0;
    VoiceArrayReadFromFlash = 0;
    for (int i = 0; i < AdcArraySize; i++)
    {
      Buffer2[i] = 0;
    }
    RestoreDetail(voice.WitchVoiceIsRecord, voice.number, &(voice.ArrayGoToSave));
    restore_2k_array(voice.number, VoiceArrayReadFromFlash, Buffer2);
    pwm_stru.CountDataFromTotally[voice.number] = (uint32_t)((voice.ArrayGoToSave) * (AdcArraySize));
    for (int i = 0; i < AdcArraySize; i++)
    {
      Buffer1[i] = Buffer2[i];
    }
  }
  else
  {
    flag.InterruptSwitch = 0;
    flag.PwmArrayEmpty = 0;
    VoiceArrayReadFromFlash = 0;
    for (int i = 0; i < AdcArraySize; i++)
    {
      Buffer2[i] = 0;
      Buffer1[i] = Buffer2[i];
    }
    StopPlaying();
  }
  // HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_RESET);
  // HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_RESET);
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
void RecordStatefunc()
{
  // voice.number = NextFreeSpaceInFlash(voice.WitchVoiceIsRecord);
  voice.number = WitchVoiceWantToPlay;
  if (voice.WitchVoiceIsRecord[voice.number] == 1)
  {
    SevenSegmentDisplay(0);
    RemoveVoice(voice.WitchVoiceIsRecord, voice.number);
  }
  RecordLED_ON();
  SevenSegmentDisplay(voice.number);
  voice.number = WitchVoiceWantToPlay;
  MX_ADC1_Init();
  HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_ADC_Start(&hadc1);
  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
  flag.InterruptSwitch = 1;
  voice.ArrayGoToSave = 0;
  flag.AdcArryFull = 0;
  HAL_TIM_Base_Start_IT(&htim2);
  while (1)
  {
    if (flag.AdcArryFull == 1)
    {
      conv_adc_val_to_pwm_duty(Buffer1); // may be need RTOS
      save_2k_array(voice.number, voice.ArrayGoToSave, Buffer1);
      voice.ArrayGoToSave++;
      flag.AdcArryFull = 0;
    }
    if (adc_stru.StopTimeCounter >= (adc_stru.TotallyStopTim + 1))
    {
      voice.WitchVoiceIsRecord[voice.number] = 1;
      SaveDetail(voice.WitchVoiceIsRecord, voice.number, voice.ArrayGoToSave);
      break;
    }
  }
  HAL_TIM_Base_Stop_IT(&htim2);
  // voice.number++;
  // WitchVoiceWantToPlay = voice.number;
  voice.ArrayGoToSave = 0;
  adc_stru.StopTimeCounter = 0;
  HAL_ADC_Stop(&hadc1);
  RecordLED_OFF();
  //  HAL_Delay(1000);
}
void AdcGettingSample()
{
  if (adc_stru.StopTimeCounter == adc_stru.TotallyStopTim)
  {
    adc_stru.StopTimeCounter++;
    for (int i = 0; i < AdcArraySize; i++)
    {
      Buffer1[i] = Buffer2[i];
      Buffer2[i] = 0;
    }

    adc_stru.counter = 0;
    flag.AdcArryFull = 1;
  }
  else
  {
    // Buffer2[adc_stru.counter] = HAL_ADC_GetValue(&hadc1);
    Buffer2[adc_stru.counter] = ADC_read();
    adc_stru.counter++;
    adc_stru.StopTimeCounter++;
    if (adc_stru.counter == (AdcArraySize))
    {
      adc_stru.counter = 0;
      for (int i = 0; i < AdcArraySize; i++)
      {
        Buffer1[i] = Buffer2[i];
        Buffer2[i] = 0;
      }
      flag.AdcArryFull = 1;
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
    __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, (uint16_t)(Buffer1[pwm_stru.counter]));
    pwm_stru.counter++;
    //      pwm_stru.CountDataFromTotally[voice.number]--;
    if (pwm_stru.counter == AdcArraySize)
    {
      pwm_stru.counter = 0;
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
void PlayStateFun()
{
  PalyLED_ON();
  SetupForPlay(WitchVoiceWantToPlay);
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); // Start Pwm signal on PB-6 Pin
  uint8_t flg_use = 0;
  while (1)
  {
    if (VoiceArrayReadFromFlash == voice.ArrayGoToSave)
    {
      break;
    }
    if (flag.PwmArrayEmpty == 0)
    {
      if (flg_use == 0)
      {
        VoiceArrayReadFromFlash++;
        restore_2k_array(voice.number, VoiceArrayReadFromFlash, Buffer2);
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
