#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "stdio.h"
#include "string.h"

#define MAX 100
char GetChar;
int CharState;
int CharCnt = 0;
char CharRes[MAX];

void GPIO_Config(void);
void USART_Config(void);
void USART1_IRQHandler(void);
void USART_SendChar(char c);
void USART_SendStr(char* str);

struct __FILE{
	int dummy;
};
FILE __stdout;

int fputc(int ch, FILE *f){
	USART_SendChar(ch);
	return ch;
}

GPIO_InitTypeDef gpio;
USART_InitTypeDef usart;

int main(){
	SystemInit();
	GPIO_Config();
	USART_Config();
	SysTick_Init();
	printf("USART is ready!!\n");
	while(1){
		if(CharState){
			if(strstr(CharRes, "ON") != NULL){
				GPIOC->ODR &= ~GPIO_Pin_13;
				printf("LED is on!!\n");
				delay(200);
			}
			if(strstr(CharRes, "OFF") != NULL){
				GPIOC->ODR |= GPIO_Pin_13;
				printf("LED is off!!\n");
				delay(200);
			}
			CharState = 0;
		}
	}
}

void GPIO_Config(void){
	//Enable Clock for GPIOA, GPIOC
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	//Configuration for PA9: Alternate Function Push-Pull
	gpio.GPIO_Pin = GPIO_Pin_9;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &gpio);
	//Configuration for PA10: Input Floating
	gpio.GPIO_Pin = GPIO_Pin_10;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &gpio);
	//Configuration for PC13: Output Push-Pull
	gpio.GPIO_Pin = GPIO_Pin_13;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &gpio);
	GPIOC->ODR |= GPIO_Pin_13;
}

void USART_Config(void){
	//Enable Clock for USART1
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	//Configuration for USART1
	usart.USART_BaudRate = 9600;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	usart.USART_Parity = USART_Parity_No;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &usart);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//Enable USART1 Global Interrupt
	NVIC_EnableIRQ(USART1_IRQn);
	
	USART_Cmd(USART1, ENABLE);
}

void USART_SendChar(char c){
	USART_SendData(USART1, c);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void USART_SendStr(char* str){
	while(str){
		USART_SendChar(*str++);
	}
}

void USART1_IRQHandler(void){
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		GetChar = USART_ReceiveData(USART1);
		if(GetChar == '\n'){
			CharState = 1;
			CharRes[CharCnt] = NULL;
			CharCnt = 0;
		}
		else CharRes[CharCnt++] = GetChar;
	}
}
