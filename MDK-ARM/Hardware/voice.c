#include "voice.h"
#include "usart.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
extern UART_HandleTypeDef huart2;//TX:PA14   RX:PA15
static uint8_t voice_uart_tx_buf[125]; /* ATK-MW8266D UART发送缓冲 */

void voice_uart_printf(char *fmt, ...)//重定向usart2
{
    va_list ap;
    uint16_t len;
    
    va_start(ap, fmt);
    vsprintf((char *)voice_uart_tx_buf, fmt, ap);
    va_end(ap);
    
    len = strlen((const char *)voice_uart_tx_buf);
    HAL_UART_Transmit(&huart5, voice_uart_tx_buf, len, HAL_MAX_DELAY);
}

