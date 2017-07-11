/*
 * misc.c
 *
 *  Created on: 04 ���� 2017 �.
 *      Author: My
 */

#include "fsl_common.h"

#include "misc.h"


void Delay(uint16_t usec)
{
	for(int i = 0; i < SystemCoreClock / 10000000 * usec; i++)
	{
		__asm("NOP");
	}
}


