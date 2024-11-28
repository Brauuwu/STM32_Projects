#include "stm32f10x.h"                  // Device header
#include "usart.h"
#include "stdio.h"
#include "string.h"
#include "ctype.h"

#define MAX 100

char GetChar;
uint8_t CharState = 0;
uint8_t CharCount = 0;
char CharRes[MAX];

USART_InitTypeDef usart;

void USART_Config(void);
void USART_SendChar(char c);
void USART_SendStr(char* str);
void USART1_IRQHandler(void);
uint8_t USART_GetString(char *str);
char *USART_GetNum(void);

struct __FILE{
	int dummy;
};
FILE __stdout;
int fputc(int ch, FILE* f){
	USART_SendChar(ch);
	return ch;
}

void USART_Config(void){
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	usart.USART_BaudRate = 9600;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	usart.USART_Parity = USART_Parity_No;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &usart);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_EnableIRQ(USART1_IRQn);
	
	USART_Cmd(USART1, ENABLE);
}

void USART_SendChar(char c){
	USART_SendData(USART1, c);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void USART_SendStr(char *str){
	while(str != NULL){
		USART_SendChar(*str++);
	}
}

void USART1_IRQHandler(void){
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		GetChar = USART_ReceiveData(USART1);
		if(GetChar == '\n'){
			CharState = 1;
			CharRes[CharCount] = NULL;
			CharCount = 0;
		}
		else CharRes[CharCount++] = GetChar;
	}
}

uint8_t checkdigit(char *str){
	uint8_t n = strlen(str), i;
	if(0 >= n || n >= 5) return 0;
	for(i=0; i<n; i++){
		if(!isdigit(str[i])) return 0;
	}
	return 1;
}

uint8_t USART_GetString(char *str){
	if(strstr(CharRes, str)) return 1;
	return 0;
}

char *USART_GetNum(void){
	if(checkdigit(CharRes)) return CharRes;
	return NULL;
}
