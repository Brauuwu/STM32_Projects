#ifndef _DELAY_
#define _DELAY_
#include "stm32f10x.h"                  // Device header

void SysTick_Init(void);
void Systick_Handler(void);
void delay(uint32_t t);
uint32_t millis(void);

#endif
