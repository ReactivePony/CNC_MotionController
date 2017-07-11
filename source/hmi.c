/*
 * hmi.c
 *
 *  Created on: 10 июля 2017 г.
 *      Author: My
 */

#include "hmi.h"

#include "fsl_gpio.h"


void HMI_Init()
{
	gpio_pin_config_t gpio_pin_config;
	gpio_pin_config.outputLogic = 0;
	gpio_pin_config.pinDirection = kGPIO_DigitalOutput;

	GPIO_PinInit(GPIOB, 1, &gpio_pin_config);
	GPIO_PinInit(GPIOB, 2, &gpio_pin_config);
}

void HMI_LedOn(hmi_led_t led)
{
	switch (led)
	{
		case kHMI_GreenLED:
			GPIO_WritePinOutput(GPIOB, 1, 1);
			break;
		case kHMI_RedLED:
			GPIO_WritePinOutput(GPIOB, 2, 1);
			break;
		default:
			break;
	}
}

void HMI_LedOff(hmi_led_t led)
{
	switch (led)
	{
		case kHMI_GreenLED:
			GPIO_WritePinOutput(GPIOB, 1, 0);
			break;
		case kHMI_RedLED:
			GPIO_WritePinOutput(GPIOB, 2, 0);
			break;
		default:
			break;
	}
}



