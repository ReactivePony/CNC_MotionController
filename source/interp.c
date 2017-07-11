/*
 * interp.c
 *
 *  Created on: 10 июля 2017 г.
 *      Author: My
 */

#include "fsl_common.h"

#include "arm_math.h"

#include "interp.h"

interp_result_t interp_result;

interp_result_t INTERP_LinearCalcStep(int32_t destStepX, int32_t destStepY, int32_t originStepX, int32_t originStepY, int32_t F)
{
	interp_result.stepX = 0;
	interp_result.stepY = 0;

	if(destStepX - originStepX >= 0 && destStepY - originStepY >= 0) // QUADRANT 1
	{
		if(F > 0)
		{
			F = F - fabsf(destStepY - originStepY);
			interp_result.stepX = 1;
		}
		else
		{
			F = F + fabsf(destStepX - originStepX);
			interp_result.stepY = 1;
		}
	}
	else if(destStepX - originStepX < 0 && destStepY - originStepY >= 0) // QUADRANT 2
	{
		if(F > 0)
		{
			F = F - fabsf(destStepY - originStepY);
			interp_result.stepX = -1;
		}
		else
		{
			F = F + fabsf(destStepX - originStepX);
			interp_result.stepY = 1;
		}
	}
	else if (destStepX - originStepX < 0 && destStepY - originStepY < 0) // QUADRANT 3
	{
		if(F > 0)
		{
			F = F - fabsf(destStepY - originStepY);
			interp_result.stepX = -1;
		}
		else
		{
			F = F + fabsf(destStepX - originStepX);
			interp_result.stepY = -1;
		}
	}
	else if(destStepX - originStepX >= 0 && destStepY - originStepY < 0) // QUADRANT 4
	{
		if(F > 0)
		{
			F = F - fabsf(destStepY - originStepY);
			interp_result.stepX = 1;
		}
		else
		{
			F = F + fabsf(destStepX - originStepX);
			interp_result.stepY = -1;
		}
	}

	interp_result.F = F;

	return interp_result;
}

interp_result_t INTERP_CircleCWCalcStep(int32_t currentStepX, int32_t currentStepY, int32_t centerStepX, int32_t centerStepY, int32_t F)
{
	interp_result.stepX = 0;
	interp_result.stepY = 0;

    if(currentStepX - centerStepX >= 0 && currentStepY - centerStepY >= 0) // QUADRANT 1
    {
		if(F >= 0)
		{
			F = F - 2 * (currentStepX - centerStepX) + 1;
			interp_result.stepX = 1;
		}
		else
		{
			F = F + 2 * (currentStepY - centerStepY) + 1;
			interp_result.stepY = -1;
		}
    }
    else if(currentStepX - centerStepX < 0 && currentStepY - centerStepY >= 0) // QUADRANT 2
    {
		if(F >= 0)
		{
			F = F + 2 * (currentStepX - centerStepX) + 1;
			interp_result.stepX = 1;
		}
		else
		{
			F = F + 2 * (currentStepY - centerStepY) + 1;
			interp_result.stepY = 1;
		}
    }
    else if (currentStepX - centerStepX < 0 && currentStepY - centerStepY < 0) // QUADRANT 3
    {
		if(F >= 0)
		{
			F = F + 2 * (currentStepX - centerStepX) + 1;
			interp_result.stepX = -1;
		}
		else
		{
			F = F - 2 * (currentStepY - centerStepY) + 1;
			interp_result.stepY = 1;
		}
    }
    else if(currentStepX - centerStepX >= 0 && currentStepY - centerStepY < 0) // QUADRANT 4
    {
		if(F >= 0)
		{
			F = F - 2 * (currentStepX - centerStepX) + 1;
			interp_result.stepX = -1;
		}
		else
		{
			F = F - 2 * (currentStepY - centerStepY) + 1;
			interp_result.stepY = -1;
		}
    }

	interp_result.F = F;

	return interp_result;
}

interp_result_t INTERP_CircleCCWCalcStep(int32_t currentStepX, int32_t currentStepY, int32_t centerStepX, int32_t centerStepY, int32_t F)
{
	interp_result.stepX = 0;
	interp_result.stepY = 0;

	if(currentStepX - centerStepX >= 0 && currentStepY - centerStepY >= 0) // QUADRANT 1
    {
		if(F >= 0)
		{
			F = F - 2 * (currentStepX - centerStepX) + 1;
			interp_result.stepX = -1;
		}
		else
		{
			F = F + 2 * (currentStepY - centerStepY) + 1;
			interp_result.stepY = 1;
		}
    }
	else if(currentStepX - centerStepX < 0 && currentStepY - centerStepY >= 0) // QUADRANT 2
    {
		if(F >= 0)
		{
			F = F + 2 * (currentStepX - centerStepX) + 1;
			interp_result.stepX = -1;
		}
		else
		{
			F = F + 2 * (currentStepY - centerStepY) + 1;
			interp_result.stepY = -1;
		}
    }
	else if (currentStepX - centerStepX < 0 && currentStepY - centerStepY < 0) // QUADRANT 3
    {
		if(F >= 0)
		{
			F = F + 2 * (currentStepX - centerStepX) + 1;
			interp_result.stepX = 1;
		}
		else
		{
			F = F - 2 * (currentStepY - centerStepY) + 1;
			interp_result.stepY = -1;
		}
    }
	else if(currentStepX - centerStepX >= 0 && currentStepY - centerStepY < 0) // QUADRANT 4
    {
		if(F >= 0)
		{
			F = F - 2 * (currentStepX - centerStepX) + 1;
			interp_result.stepX = 1;
		}
		else
		{
			F = F - 2 * (currentStepY - centerStepY) + 1;
			interp_result.stepY = 1;
		}
    }

	interp_result.F = F;

	return interp_result;
}



