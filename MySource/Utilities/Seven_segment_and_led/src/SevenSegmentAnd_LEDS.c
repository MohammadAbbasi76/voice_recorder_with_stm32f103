#include "SevenSegmentAnd_LEDS.h"
void Blinking()
{
  for (uint8_t i = 0; i < 3; i++)
  {
    HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_SET);
    HAL_Delay(500);
    HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_RESET);
    HAL_Delay(500);
  }
}
void SevenSegmentDisplay(uint8_t number)
{
  uint8_t Temp = 0;
  Temp = number & 0b00000001;
  if (Temp == 1)
    HAL_GPIO_WritePin(BCD_A_GPIO_Port, BCD_A_Pin, GPIO_PIN_SET);
  else
    HAL_GPIO_WritePin(BCD_A_GPIO_Port, BCD_A_Pin, GPIO_PIN_RESET);
  Temp = 0;
  Temp = number & 0b00000010;
  if (Temp == 2)
    HAL_GPIO_WritePin(BCD_B_GPIO_Port, BCD_B_Pin, GPIO_PIN_SET);
  else
    HAL_GPIO_WritePin(BCD_B_GPIO_Port, BCD_B_Pin, GPIO_PIN_RESET);
  Temp = 0;
  Temp = number & 0b00000100;
  if (Temp == 4)
    HAL_GPIO_WritePin(BCD_C_GPIO_Port, BCD_C_Pin, GPIO_PIN_SET);
  else
    HAL_GPIO_WritePin(BCD_C_GPIO_Port, BCD_C_Pin, GPIO_PIN_RESET);
  Temp = 0;
  Temp = number & 0b00001000;
  if (Temp == 8)
    HAL_GPIO_WritePin(BCD_D_GPIO_Port, BCD_D_Pin, GPIO_PIN_SET);
  else
    HAL_GPIO_WritePin(BCD_D_GPIO_Port, BCD_D_Pin, GPIO_PIN_RESET);
}
void RecordLedOn()
{
  HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_SET);
}
void RecordLedOff()
{
  HAL_GPIO_WritePin(GPIOB, Record_LED_Pin, GPIO_PIN_RESET);
}
void PalyLedOn()
{
  HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_SET);
}
void PlayLedOff()
{
  HAL_GPIO_WritePin(GPIOB, Play_LED_Pin, GPIO_PIN_RESET);
}