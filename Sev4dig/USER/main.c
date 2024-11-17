#include "stm32f10x.h"                  // Device header

#define LATCH_PIN GPIO_Pin_0
#define CLOCK_PIN GPIO_Pin_1
#define DATA_PIN GPIO_Pin_2
#define BUTTON_PIN GPIO_Pin_0
#define D1_PIN GPIO_Pin_3
#define D2_PIN GPIO_Pin_4
#define D3_PIN GPIO_Pin_5
#define D4_PIN GPIO_Pin_6

volatile uint32_t tick = 0 ;
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
void SysTick_Init(void);
void SysTick_Handler(void);
void Delay(uint32_t t);
void shiftOut(uint8_t data);
void LED7_Write(uint8_t digit, uint8_t num);
uint32_t GetTick(void);

uint16_t cnt=0;
uint8_t	previous = 1;
uint8_t State = 0;
uint32_t startTick = 0;

int main(){
	SystemInit();
	GPIO_Config();
	SysTick_Init();
	while(1){
		int reading = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0);
		if(reading != previous) startTick = tick;
		if(tick - startTick >= 20){
			if(reading != State){
				State = reading;
				if(!State) cnt = (cnt+1)%10000;
			}
		}
		previous = reading;
		LED7_Write(1, cnt/1000);
		LED7_Write(2, cnt/100%10);
		LED7_Write(3, cnt/10%10);
		LED7_Write(4, cnt%10);
	}
}

void GPIO_Config(void){
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN;
	gpio.GPIO_Pin = LATCH_PIN | CLOCK_PIN | DATA_PIN | D1_PIN| D2_PIN | D3_PIN | D4_PIN;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);

	gpio.GPIO_Pin = BUTTON_PIN;
	gpio.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &gpio);
}

void SysTick_Init(void){
	SysTick_Config(SystemCoreClock/1000);
}

void SysTick_Handler(void){
	tick++;
}

void Delay(uint32_t t){
	uint32_t startTick = tick;
	while(tick - startTick < t);
}

void shiftOut(uint8_t data){
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
	GPIOA->ODR |= LATCH_PIN;
	shiftOut(digits[num]);
	GPIOA->ODR &= ~LATCH_PIN;
	
	GPIO_WriteBit(GPIOA, D1_PIN, Bit_SET);
	GPIO_WriteBit(GPIOA, D2_PIN, Bit_SET);
	GPIO_WriteBit(GPIOA, D3_PIN, Bit_SET);
	GPIO_WriteBit(GPIOA, D4_PIN, Bit_SET);
	
	switch(digit){
		case 1:
			GPIO_WriteBit(GPIOA, D4_PIN, Bit_RESET);
			break;
		case 2:
			GPIO_WriteBit(GPIOA, D1_PIN, Bit_RESET);
			break;
		case 3:
			GPIO_WriteBit(GPIOA, D2_PIN, Bit_RESET);
			break;
		case 4:
			GPIO_WriteBit(GPIOA, D3_PIN, Bit_RESET);
			break;
	}
	Delay(1);
}

uint32_t GetTick(void){
	return tick;
}
