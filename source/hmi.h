/*
 * hmi.h
 *
 *  Created on: 10 июля 2017 г.
 *      Author: My
 */

#ifndef SOURCE_HMI_H_
#define SOURCE_HMI_H_

typedef enum _hmi_led
{
	kHMI_RedLED = 0,
	kHMI_GreenLED = 1,
} hmi_led_t;

void HMI_Init();
void HMI_LedOn(hmi_led_t led);
void HMI_LedOff(hmi_led_t led);


#endif /* SOURCE_HMI_H_ */
