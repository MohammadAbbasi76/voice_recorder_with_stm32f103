
#include"main.h"
#include"print_with_uart"


void UART_Printf(const char *fmt, ...)
{
  char buff[300];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buff, sizeof(buff), fmt, args);
  HAL_UART_Transmit(&huart1, (uint8_t *)buff, strlen(buff),
                    HAL_MAX_DELAY);
  va_end(args);
}