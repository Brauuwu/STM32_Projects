#ifndef _USART_
#define _USART_
#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "string.h"

extern uint8_t CharState;

void USART_Config(void);
void USART_SendChar(char c);
void USART_SendStr(char* str);
void USART1_IRQHandler(void);
uint8_t USART_GetString(char *str);

#endif
