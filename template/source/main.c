
#include "stm32f4xx_hal.h"

#define LED_RED_GPIO	GPIOC
#define LED_RED_PIN		GPIO_PIN_7

#define LED_GREEN_GPIO	GPIOB
#define LED_GREEN_PIN	GPIO_PIN_1

#define LED_BLUE_GPIO	GPIOB
#define LED_BLUE_PIN	GPIO_PIN_0

static void SystemClock_Config(void);

struct
{
	GPIO_TypeDef* gpio;
	uint16_t pin;
	uint32_t blink;
} leds[] = {
	{LED_RED_GPIO, LED_RED_PIN, 1},
	{LED_GREEN_GPIO, LED_GREEN_PIN, 2},
	{LED_BLUE_GPIO, LED_BLUE_PIN, 3},
	{NULL, 0, 0}
};


void main(void)
{
	uint32_t i;

	HAL_Init();

	SystemClock_Config();

	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();


	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

	i = 0;
	while(leds[i].gpio != NULL)
	{
		GPIO_InitStruct.Pin = leds[i].pin;
		HAL_GPIO_Init(leds[i].gpio, &GPIO_InitStruct);
		i++;
	}


	while(1)
	{
		i = 0;
		while(leds[i].gpio != NULL)
		{
			uint32_t j;
			for(j=0;j<leds[i].blink;j++)
			{
				HAL_GPIO_WritePin(leds[i].gpio, leds[i].pin, GPIO_PIN_SET);
				HAL_Delay(250);
				HAL_GPIO_WritePin(leds[i].gpio, leds[i].pin, GPIO_PIN_RESET);
				HAL_Delay(250);
			}

			i++;
		}
	}
}

void SysTick_Handler(void)
{
	HAL_IncTick();
}

static void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();

	/* The voltage scaling allows optimizing the power consumption when the device is 
	clocked below the maximum system frequency, to update the voltage scaling value 
	regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/* -1- Select HSE  as system clock source to allow modification of the PLL configuration */
	RCC_ClkInitStruct.ClockType       = RCC_CLOCKTYPE_SYSCLK;
	RCC_ClkInitStruct.SYSCLKSource    = RCC_SYSCLKSOURCE_HSE;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);


	/* -2- Enable HSI Oscillator, select it as PLL source and finally activate the PLL */
	RCC_OscInitStruct.OscillatorType       = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState             = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue  = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState         = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource        = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM             = 16;
	RCC_OscInitStruct.PLL.PLLN             = 160;
	RCC_OscInitStruct.PLL.PLLP             = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ             = 7;
	RCC_OscInitStruct.PLL.PLLR             = 2;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);


	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
	clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
}