
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"


void main(void);

/*
void SystemInit(void)
{
	main();
}
*/

void delay(void)
{
	uint32_t i;
	for(i=0;i<1000000;i++)
	{
		__NOP();
	}

}

void main(void)
{
	
	LL_AHB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR | LL_AHB1_GRP1_PERIPH_GPIOB | LL_AHB1_GRP1_PERIPH_GPIOC);

	LL_GPIO_InitTypeDef gpio;
	LL_GPIO_StructInit(&gpio);
	gpio.Mode = LL_GPIO_MODE_OUTPUT;
	gpio.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	gpio.Pull = LL_GPIO_PULL_NO;
	gpio.Speed = LL_GPIO_SPEED_FREQ_HIGH;

	gpio.Pin = LL_GPIO_PIN_7;
	LL_GPIO_Init(GPIOC, &gpio);

	gpio.Pin = LL_GPIO_PIN_1;
	LL_GPIO_Init(GPIOB, &gpio);

	gpio.Pin = LL_GPIO_PIN_0;
	LL_GPIO_Init(GPIOB, &gpio);

	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_7);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_1);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0);

	while(1)
	{
		LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_7);
		delay();
		LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_7);
		LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_1);
		delay();
		LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_1);
		LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_0);
		delay();
		LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0);
	}
}