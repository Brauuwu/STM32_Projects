#include "stm32f10x.h"                  // Device header
#include "usart.h"
#include "delay.h"

void GPIO_Config(void);
void ADC_Config(void);

GPIO_InitTypeDef gpio;
ADC_InitTypeDef adc;

int main(){
	SystemInit();
	SystemCoreClockUpdate();
	SysTick_Init();
	GPIO_Config();
	USART_Config();
	ADC_Config();
	printf("USART is ready!\n");
	while(1){
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){
			uint16_t adcValue = ADC_GetConversionValue(ADC1);
			printf("Current ADC Value: %.2f\n", adcValue*3.3/4095);
		}
		delay(1000);
	}
}

void GPIO_Config(void){
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	gpio.GPIO_Pin = GPIO_Pin_7;
	gpio.GPIO_Mode = GPIO_Mode_AIN;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);
	//Configuration for PA9: Alternate Function Push-Pull
	gpio.GPIO_Pin = GPIO_Pin_9;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &gpio);
	//Configuration for PA10: Input Floating
	gpio.GPIO_Pin = GPIO_Pin_10;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &gpio);
}

void ADC_Config(void){
	//Enable Clock for ADC1
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	//Configuration for ADC1
	adc.ADC_Mode = ADC_Mode_Independent;
	adc.ADC_ScanConvMode = DISABLE;
	adc.ADC_ContinuousConvMode = DISABLE;
	adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	adc.ADC_DataAlign = ADC_DataAlign_Right;
	adc.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &adc);
	//Set up ADC1
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1, ADC_SampleTime_55Cycles5);
	//Active ADC1
	ADC_Cmd(ADC1, ENABLE);
	//Calibration for ADC1
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
}
