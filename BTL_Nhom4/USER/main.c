#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "usart.h"
#include "TM1637.h"

#define TM1637_CLK_PIN    GPIO_Pin_0
#define TM1637_DIO_PIN    GPIO_Pin_1
#define BUTTON_PIN    		GPIO_Pin_1

GPIO_InitTypeDef gpio;
void GPIO_Config(void);

int num = 0;
uint32_t start = 0;
uint8_t CountFlag = 0;
uint8_t ButtonState = 1;

int main(){
	SysTick_Init();
	GPIO_Config();
	USART_Config();
	TM1637_brightness(7);
	TM1637_clearDisplay();
	printf("USART is ready!\n");
	while(1){
		if(CharState){
			if(USART_GetString("Start")){
				CountFlag = 1;
				printf("Start Counting!\n");
			}
			if(USART_GetString("Stop")){
				CountFlag = 0;
				printf("Stop Counting!\n");
			}
			CharState = 0;
		}
		
		if(GPIO_ReadInputDataBit(GPIOB, BUTTON_PIN) == 0){
			delay(20);
			if(ButtonState){
				ButtonState = !ButtonState;
				if(!ButtonState){
					num = 0;
					CountFlag = 0;
					printf("Reset Counting!\n");
				}
			}
		}
		else ButtonState = 1;
		
		if(CountFlag && millis() - start >= 50){
			start = millis();
			num = (num+1)%10000;
		}
		TM1637_display_all(num);
	}
}

void GPIO_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	gpio.GPIO_Pin = TM1637_CLK_PIN | TM1637_DIO_PIN;
	gpio.GPIO_Mode = GPIO_Mode_Out_OD;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);
	
	gpio.GPIO_Pin = BUTTON_PIN;
	gpio.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &gpio);
	
	gpio.GPIO_Pin = GPIO_Pin_9;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &gpio);
	
	gpio.GPIO_Pin = GPIO_Pin_10;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &gpio);
}
