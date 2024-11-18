#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "usart.h"

#define LATCH_PIN GPIO_Pin_0
#define CLOCK_PIN GPIO_Pin_1
#define DATA_PIN GPIO_Pin_2
#define D1_PIN GPIO_Pin_3
#define D2_PIN GPIO_Pin_4
#define D3_PIN GPIO_Pin_5
#define D4_PIN GPIO_Pin_6
#define BUTTON_PIN_R GPIO_Pin_0
#define BUTTON_PIN_L GPIO_Pin_1

GPIO_InitTypeDef gpio;

uint8_t digits[10] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

void GPIO_Config(void);
void ShiftOut(uint8_t data);
void LED7_Write(uint8_t digit, uint8_t num);

uint8_t previous_L = 1;
uint8_t previous_R = 1;
uint16_t State_L = 1;
uint16_t State_R = 1;
uint8_t startTick_L = 0;
uint8_t startTick_R = 0;
int cnt = 0;
uint8_t reading_L;
uint8_t reading_R;
uint8_t startScan;
uint8_t currentDigit = 1;

int main(){
	SystemInit();
	SysTick_Init();
	GPIO_Config();
	USART_Config();
	printf("======================================================\n");
	printf("USART is ready!\n");
	delay(1000);
	printf("Current number: %d\n", cnt);
	while(1){
		if(CharState){
			if(USART_GetString("Increase")){
				cnt = (cnt+1)%10000;
				printf("Current number: %d\n", cnt);
			}
			if(USART_GetString("Decrease")){
				cnt--;
				if(cnt == -1) cnt = 9999;
				printf("Current number: %d\n", cnt);
			}
			CharState = 0;
		}
		
		reading_L = GPIOB->IDR & BUTTON_PIN_L;
		if(reading_L != previous_L) startTick_L = millis();
		if(millis() - startTick_L >= 20){
			if(reading_L != State_L){
				State_L = reading_L;
				if(!State_L){
					cnt--;
					if(cnt == -1) cnt = 9999;
					printf("Current number: %d\n", cnt);
				}
			}
		}
		previous_L = reading_L;
		
		reading_R = GPIOB->IDR & BUTTON_PIN_R;
		if(reading_R != previous_R) startTick_R = millis();
		if(millis() - startTick_R >= 20){
			if(reading_R != State_R){
				State_R = reading_R;
				if(!State_R){
					cnt = (cnt+1)%10000;
					printf("Current number: %d\n", cnt);
				}
			}
		}
		previous_R = reading_R;
		
		startScan = millis();
		if(millis() - startScan >=1){
			startScan = millis();
			switch(currentDigit){
				case 1:
					LED7_Write(1, cnt/1000);
					currentDigit = 2;
					break;
				case 2:
					LED7_Write(2, cnt/100%10);
					currentDigit = 3;
					break;
				case 3:
					LED7_Write(3, cnt/10%10);
					currentDigit = 4;
					break;
				case 4:
					LED7_Write(4, cnt%10);
					currentDigit = 1;
					break;
			}
		}
	}
}

void GPIO_Config(void){
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	//Configuration for PA9: Alternate Function Push-Pull
	gpio.GPIO_Pin = GPIO_Pin_9;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &gpio);
	//Configuration for PA10: Input Floating
	gpio.GPIO_Pin = GPIO_Pin_10;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &gpio);
	//Configuration for PA0, PA1, PA2, PA3, PA4, PA5, PA6: Output Push-Pull
	gpio.GPIO_Pin = LATCH_PIN | CLOCK_PIN | DATA_PIN | D1_PIN | D2_PIN | D3_PIN | D4_PIN;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &gpio);
	//Configuration for PB0: Input Pull-up
	gpio.GPIO_Pin = BUTTON_PIN_L | BUTTON_PIN_R;
	gpio.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &gpio);
}

void ShiftOut(uint8_t data){
	unsigned int i;
	for(i=0; i<8; i++){
		if(data & 0x80) GPIOA->ODR |= DATA_PIN;
		else GPIOA->ODR &= ~DATA_PIN;
		GPIOA->ODR |= CLOCK_PIN;
		GPIOA->ODR &= ~CLOCK_PIN;
		data <<= 1;
	}
}

void LED7_Write(uint8_t digit, uint8_t num){
	GPIOA->ODR &= ~LATCH_PIN;
	ShiftOut(digits[num]);
	GPIOA->ODR |= LATCH_PIN;
	
	GPIOA->ODR |= D1_PIN;
	GPIOA->ODR |= D2_PIN;
	GPIOA->ODR |= D3_PIN;
	GPIOA->ODR |= D4_PIN;
	
	switch(digit){
		case 1:
			GPIOA->ODR &= ~D1_PIN;
			break;
		case 2:
			GPIOA->ODR &= ~D2_PIN;
			break;
		case 3:
			GPIOA->ODR &= ~D3_PIN;
			break;
		case 4:
			GPIOA->ODR &= ~D4_PIN;
			break;
	}
}
