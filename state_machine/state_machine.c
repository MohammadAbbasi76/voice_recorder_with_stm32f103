
#include "state_machine.h"
#include "main.h"
#include "tim.h"
#include "adc.h"
#include "usart.h"
#include "stm32f1xx_hal_gpio.h"
#include "string.h"
#include "w25qxx.h"
extern uint16_t TempArry[AdcArraySize];
extern uint16_t TempArry1[AdcArraySize];
extern struct adc_ adc_stru;
extern struct pwm_ pwm_stru;
extern struct voice_struc voice;
extern struct flags flag;
extern uint8_t InputKey;
extern uint8_t WitchVoiceWannaToPlay;
extern uint8_t state;
extern uint32_t ccr_pwm_val;
uint32_t timing_1 = 0;
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart1;
extern ADC_HandleTypeDef hadc1;
extern uint32_t UartFLush_T1;
void state_machine()
{
  switch (state)
  {
  case ReadKeyboardState:
  {
	  int a=(UartFLush_T1 - (HAL_GetTick()));
    if (a < 1)
    {
      for (int i = 0; i < UartArraySize; i++)
        UartReceive[i] = 0;
      HAL_UART_Abort_IT(&huart1);
      __HAL_UART_FLUSH_DRREGISTER(&huart1);
      UartFLush_T1 = HAL_GetTick() + UartTimeDataExpire;
    }
    HAL_UART_Receive_IT(&huart1, UartReceive, UartArraySize);
    state = ReadKeyboardState;
    break;
  }
  case UartDataReceive:
  {
    HAL_UART_Receive_IT(&huart1, UartReceive, UartArraySize);
    state = ReadKeyboardState;
    break;
  }
  case RecordState:
  {
    timing_1 = 0;
    timing_1 = HAL_GetTick();
    RecordStatefunc();
    timing_1 = HAL_GetTick() - timing_1;
    //  FlashPrint();
    state = UartDataReceive;
    break;
  }
  case ChooseVoiceForPlay:
  {
    ChooseVoiceForPlayfunc();
    state = UartDataReceive;
    break;
  }
  case PlayState:
  {
    timing_1 = 0;
    timing_1 = HAL_GetTick();
    PlayStateFun();
    timing_1 = HAL_GetTick() - timing_1;
    state = UartDataReceive;
    break;
  }
  case M66State:
  {
    HAL_GPIO_WritePin(GPIOB, M66_on_off_key_Pin, GPIO_PIN_SET);
    HAL_Delay(M66DelayTime);
    HAL_GPIO_WritePin(GPIOB, M66_on_off_key_Pin, GPIO_PIN_RESET);
    break;
  }
  case FlashEraseState:
  {
    FlashEraseFunc();
    state = UartDataReceive;
    break;
  }
  case PLayLoop:
  {
    HAL_UART_Receive_IT(&huart1, UartReceive, UartArraySize);
    LoopPLayFunc();
    state = UartDataReceive;
    break;
  }
  }
}
