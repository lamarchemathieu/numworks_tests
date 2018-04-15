
#include <stm32f4xx_hal.h>

#define LED_RED_GPIO	GPIOC
#define LED_RED_PIN		GPIO_PIN_7

#define LED_GREEN_GPIO	GPIOB
#define LED_GREEN_PIN	GPIO_PIN_1

#define LED_BLUE_GPIO	GPIOB
#define LED_BLUE_PIN	GPIO_PIN_0

#define UART_TX_GPIO	GPIOD
#define UART_TX_PIN		GPIO_PIN_8
#define UART_TX_AF		GPIO_AF7_USART3

#define UART_RX_GPIO	GPIOC
#define UART_RX_PIN		GPIO_PIN_11
#define UART_RX_AF		GPIO_AF7_USART3

#define UART 			USART3

static UART_HandleTypeDef UartHandle;

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


int main(void)
{
	uint32_t i;

	HAL_Init();

	SystemClock_Config();

	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_USART3_CLK_ENABLE();

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


	GPIO_InitStruct.Pin   = UART_TX_PIN;
	GPIO_InitStruct.Alternate = UART_TX_AF;
	GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(UART_TX_GPIO, &GPIO_InitStruct);

	GPIO_InitStruct.Pin   = UART_RX_PIN;
	GPIO_InitStruct.Alternate = UART_RX_AF;
	GPIO_InitStruct.Mode  = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(UART_RX_GPIO, &GPIO_InitStruct);

	UartHandle.Instance        = UART;

	UartHandle.Init.BaudRate     = 115200;
	UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits     = UART_STOPBITS_1;
	UartHandle.Init.Parity       = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode         = UART_MODE_TX_RX;
	UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&UartHandle);

	while(1)
	{
		i=0;
		while(1)
		{
			const char msg_salut[] = "Hello !\r\n";
			HAL_UART_Transmit(&UartHandle, (uint8_t*)msg_salut, sizeof(msg_salut), 5000);

			HAL_GPIO_TogglePin(leds[i].gpio, leds[i].pin);

			i = i+1;
			if (leds[i].gpio == NULL)
				i = 0;

			//HAL_Delay(100);

			char c;
			if (HAL_UART_Receive(&UartHandle, (uint8_t *)&c, 1, 100) == HAL_OK)
			{	
				char m1[] = "<";
				char m2[] = ">\r\n";
				HAL_UART_Transmit(&UartHandle, (uint8_t*)m1, sizeof(m1), 5000);
				HAL_UART_Transmit(&UartHandle, (uint8_t*)&c, 1, 5000);
				HAL_UART_Transmit(&UartHandle, (uint8_t*)m2, sizeof(m2), 5000);
			}

		}

		/*
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
		*/
	}

	return 0;
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
