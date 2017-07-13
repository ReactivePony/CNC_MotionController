/*
 * motion.c
 *
 *  Created on: 04 июля 2017 г.
 *      Author: My
 */

#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_pit.h"

#include "arm_math.h"

#include "motion_config.h"
#include "motion.h"
#include "interp.h"
#include "misc.h"

motion_state_t motion_state = kMOTION_Idle;
motion_speed_mode_t motion_speed_mode = kMOTION_Normal;
motion_plane_t motion_plane = kMOTION_PlaneXY;

int32_t currentStepX = 0;
int32_t currentStepY = 0;
int32_t currentStepZ = 0;
int32_t currentStepA = 0;
int32_t currentStepB = 0;

int32_t destStepX = 0;
int32_t destStepY = 0;
int32_t destStepZ = 0;
int32_t destStepA = 0;
int32_t destStepB = 0;

int32_t originStepX = 0;
int32_t originStepY = 0;
int32_t originStepZ = 0;
int32_t originStepA = 0;
int32_t originStepB = 0;

int32_t centerStepX = 0;
int32_t centerStepY = 0;
int32_t centerStepZ = 0;
int32_t centerStepA = 0;
int32_t centerStepB = 0;

int32_t deltaStepX = 0;
int32_t deltaStepY = 0;
int32_t deltaStepZ = 0;

uint16_t stepsPerMillimeter = (200 * STEP_DIVIDER) / 2;

uint32_t maxSpeed = 2000;
uint32_t normalSpeed = 0;
uint32_t targetSpeed = 0;
uint32_t currentSpeed = 0;
uint32_t accel = 25;

interp_result_t interp_result = {0, 0, 0, 0 ,0 ,0};

void MOTION_StepTimer()
{
	pit_config_t pit_config;

	PIT_GetDefaultConfig(&pit_config);
	PIT_Init(PIT, &pit_config);
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, SystemCoreClock / 500);
	PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

	EnableIRQ(PIT0_IRQn);

	PIT_StartTimer(PIT, kPIT_Chnl_0);
}

void PIT0_IRQHandler() // Step Timer Interrupt
{
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, PIT_TFLG_TIF_MASK);

	if(motion_speed_mode == kMOTION_Normal)
		targetSpeed = normalSpeed;
	else if(motion_speed_mode == kMOTION_Rapid)
		targetSpeed = maxSpeed * stepsPerMillimeter / 60;

	deltaStepX = fabs(destStepX - originStepX) - fabs(currentStepX - originStepX);
	deltaStepY = fabs(destStepY - originStepY) - fabs(currentStepY - originStepY);
	deltaStepZ = fabs(destStepZ - originStepZ) - fabs(currentStepZ - originStepZ);

	switch (motion_state)
	{
		case kMOTION_Idle:
			interp_result.errX = 0;
			interp_result.errY = 0;
			interp_result.errZ = 0;
			break;

		case kMOTION_Linear:
			if(deltaStepX > 0 || deltaStepY > 0 || deltaStepZ > 0)
				interp_result = INTERP_LinearCalcStep(destStepX, destStepY, destStepZ, originStepX, originStepY, originStepZ, interp_result.errX, interp_result.errY, interp_result.errZ);
			else
				motion_state = kMOTION_Idle;
			break;

		case kMOTION_CircleCW:
			if(deltaStepX > 0 || deltaStepY > 0)
				interp_result = INTERP_CircleCWCalcStep(currentStepX, currentStepY, centerStepX, centerStepY, interp_result.errX);
			else
				motion_state = kMOTION_Idle;
			break;

		case kMOTION_CircleCCW:
			if(deltaStepX > 0 || deltaStepY > 0)
				interp_result = INTERP_CircleCCWCalcStep(currentStepX, currentStepY, centerStepX, centerStepY, interp_result.errX);
			else
				motion_state = kMOTION_Idle;
			break;
		default:
			break;
	}

	if(motion_state != kMOTION_Idle)
	{
		if(interp_result.stepX > 0)
		{
			MOTION_MakeStep(kMOTION_AxisX, kMOTION_Direction_CW);
			currentStepX += 1;
		}
		else if(interp_result.stepX < 0)
		{
			MOTION_MakeStep(kMOTION_AxisX, kMOTION_Direction_CCW);
			currentStepX -= 1;
		}

		if(interp_result.stepY > 0)
		{
			MOTION_MakeStep(kMOTION_AxisY, kMOTION_Direction_CW);
			currentStepY += 1;
		}
		else if(interp_result.stepY < 0)
		{
			MOTION_MakeStep(kMOTION_AxisY, kMOTION_Direction_CCW);
			currentStepY -= 1;
		}

		if(interp_result.stepZ > 0)
		{
			MOTION_MakeStep(kMOTION_AxisZ, kMOTION_Direction_CW);
			currentStepZ += 1;
		}
		else if(interp_result.stepZ < 0)
		{
			MOTION_MakeStep(kMOTION_AxisZ, kMOTION_Direction_CCW);
			currentStepZ -= 1;
		}
	}
}

void MOTION_AccelTimer()
{
	pit_config_t pit_config;

	PIT_GetDefaultConfig(&pit_config);
	PIT_Init(PIT, &pit_config);
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, SystemCoreClock / 1000);
	PIT_EnableInterrupts(PIT, kPIT_Chnl_1, kPIT_TimerInterruptEnable);

	EnableIRQ(PIT1_IRQn);

	PIT_StartTimer(PIT, kPIT_Chnl_1);
}

void PIT1_IRQHandler() // Accel Timer Interrupt
{
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, PIT_TFLG_TIF_MASK);

	float currentVectorLenght = sqrtf(powf(destStepX - currentStepX, 2) + powf(destStepY - currentStepY, 2) + powf(destStepZ - currentStepZ, 2));
	float originVectorLenght = sqrt(powf(destStepX - originStepX, 2) + powf(destStepY - originStepY, 2) + powf(destStepZ - originStepZ, 2));

	if(motion_state != kMOTION_Idle)
	{
		if(currentVectorLenght >= originVectorLenght - targetSpeed / accel)
		{
			currentSpeed = (originVectorLenght - currentVectorLenght + 1) * targetSpeed / (targetSpeed / accel) + 500;
		}
		else if(currentVectorLenght < currentSpeed / accel)
		{
			currentSpeed = currentVectorLenght * targetSpeed / (targetSpeed / accel) + 500;
		}

		PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, SystemCoreClock / currentSpeed);
	}
}

void MOTION_InitPins()
{
	gpio_pin_config_t gpio_pin_config;
	gpio_pin_config.outputLogic = 0;
	gpio_pin_config.pinDirection = kGPIO_DigitalOutput;

	GPIO_PinInit(GPIOC, 4, &gpio_pin_config);
	GPIO_PinInit(GPIOC, 5, &gpio_pin_config);
	GPIO_PinInit(GPIOC, 6, &gpio_pin_config);
	GPIO_PinInit(GPIOC, 7, &gpio_pin_config);
	GPIO_PinInit(GPIOC, 8, &gpio_pin_config);
	GPIO_PinInit(GPIOC, 9, &gpio_pin_config);
	GPIO_PinInit(GPIOC, 10, &gpio_pin_config);
	GPIO_PinInit(GPIOC, 11, &gpio_pin_config);
}

void MOTION_Enable()
{
	MOTION_StepTimer();
	MOTION_AccelTimer();

	GPIO_WritePinOutput(GPIOC, 10, 1); //ENABLE
}

void MOTION_Disable()
{
	GPIO_WritePinOutput(GPIOC, 10, 0); //DISABLE
}

void MOTION_MakeStep(motion_axis_t axis, motion_direction_t direction)
{
	switch (axis)
	{
		case kMOTION_AxisX:
			GPIO_WritePinOutput(GPIOC, 5, direction); //CW_X
			GPIO_WritePinOutput(GPIOC, 4, 1); //CLK_X
			Delay(1);
			GPIO_WritePinOutput(GPIOC, 4, 0); //CLK_X
			break;
		case kMOTION_AxisY:
			GPIO_WritePinOutput(GPIOC, 7, direction); //CW_Y
			GPIO_WritePinOutput(GPIOC, 6, 1); //CLK_Y
			Delay(1);
			GPIO_WritePinOutput(GPIOC, 6, 0); //CLK_Y
			break;
		case kMOTION_AxisZ:
			GPIO_WritePinOutput(GPIOC, 9, direction); //CW_Z
			GPIO_WritePinOutput(GPIOC, 8, 1); //CLK_Z
			Delay(1);
			GPIO_WritePinOutput(GPIOC, 8, 0); //CLK_Z
			break;
		default:
			break;
	}
}

void MOTION_SetState(motion_state_t state)
{
	motion_state = state;
}

void MOTION_SetDestX(float destX)
{
	destStepX = destX * stepsPerMillimeter;

	originStepX = currentStepX;
	originStepY = currentStepY;
	originStepZ = currentStepZ;
}

void MOTION_SetDestY(float destY)
{
	destStepY = destY * stepsPerMillimeter;

	originStepX = currentStepX;
	originStepY = currentStepY;
	originStepZ = currentStepZ;
}

void MOTION_SetDestZ(float destZ)
{
	destStepZ = destZ * stepsPerMillimeter;

	originStepX = currentStepX;
	originStepY = currentStepY;
	originStepZ = currentStepZ;
}

void MOTION_SetCenterX(float centerX)
{
    centerStepX = centerX * stepsPerMillimeter;
}

void MOTION_SetCenterY(float centerY)
{
    centerStepY = centerY * stepsPerMillimeter;
}

void MOTION_SetCenterZ(float centerZ)
{
	centerStepZ = centerZ * stepsPerMillimeter;
}

motion_state_t MOTION_GetState()
{
	return motion_state;
}

void MOTION_SetSpeed(uint32_t speed)
{
	normalSpeed = speed * stepsPerMillimeter / 60;
	targetSpeed = normalSpeed;
}

void MOTION_SetSpeedMode(motion_speed_mode_t speed_mode)
{
	motion_speed_mode = speed_mode;
}

void MOTION_SetPlane(motion_plane_t plane)
{
	motion_plane = plane;
}





























