#include "stm32f10x.h"                  // Device header

volatile uint32_t tick = 0;
uint8_t LED_STATE=0;

void GPIO_Config(void);
void UART1_Init(void);
void UART1_SendChar(char c);
void UART1_SendString(const char* str);
void SysTick_Init(void);
void SysTick_Handler(void);
void Delay(uint32_t t);

GPIO_InitTypeDef gpio;
USART_InitTypeDef uart;

int main(){
	SysTick_Init();
	GPIO_Config();
	UART1_Init();
	while(1){
		switch(LED_STATE){
			case 0:
				LED_STATE = 1;
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);
				UART1_SendString("LED turned on!\n");
				Delay(1000);
				break;
			case 1:
				LED_STATE = 0;
				GPIO_SetBits(GPIOC, GPIO_Pin_13);
				UART1_SendString("LED turned off!\n");
				Delay(1000);
				break;
		}
	}
}

void GPIO_Config(){
	//Enable Clock for Port C
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	
	//Configuration for PC13: Output Push-Pull
	gpio.GPIO_Pin = GPIO_Pin_13;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &gpio);
}

void UART1_Init(){
	//Enable Clock for USART1 TX-RX
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN;
	
	//Configuration for PA9(TX): Alternate Output Push-Pull
	gpio.GPIO_Pin = GPIO_Pin_9;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);
	
	//Configuration for PA10(RX): Input Floating
	gpio.GPIO_Pin = GPIO_Pin_10;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &gpio);
	
	//Configuration for USART1
	uart.USART_BaudRate = 9600;
	uart.USART_WordLength = USART_WordLength_8b;
	uart.USART_StopBits = USART_StopBits_1;
	uart.USART_Parity = USART_Parity_No;
	uart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	uart.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	
	//Initialize USART1
	USART_Init(USART1, &uart);
	USART_Cmd(USART1, ENABLE);
}

void UART1_SendChar(char c){
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	USART_SendData(USART1, c);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

void UART1_SendString(const char* str){
	while(*str){
		UART1_SendChar(*str++);
	}
}

void SysTick_Init(){
	SysTick_Config(SystemCoreClock/1000);
}

void SysTick_Handler(){
	tick++;
}

void Delay(uint32_t t){
	uint32_t startTick = tick;
	while(tick - startTick < t);
}
