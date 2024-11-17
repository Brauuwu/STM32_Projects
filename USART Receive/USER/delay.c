#include "stm32f10x.h"                  // Device header
#include "delay.h"

volatile uint32_t tick = 0;

void SysTick_Init(void){
	SysTick_Config(SystemCoreClock/1000);
}

void SysTick_Handler(void){
	tick++;
}

void delay(uint32_t t){
	uint32_t startTick = tick;
	while(tick - startTick < t);
}

uint32_t millis(void){
	return tick;
}
