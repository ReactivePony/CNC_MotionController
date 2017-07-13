/*
 * interp.c
 *
 *  Created on: 10 июля 2017 г.
 *      Author: MB
 */

#include "fsl_common.h"

#include "arm_math.h"

#include "interp.h"

interp_result_t interp_result;
interp_linear_result_t interp_linear_result;


interp_linear_result_t INTERP_Linear3dCalcStep(int32_t destStepX, int32_t destStepY, int32_t destStepZ, int32_t originStepX, int32_t originStepY, int32_t originStepZ, int32_t errX, int32_t errY, int32_t errZ)
{
	interp_linear_result.stepX = 0;
	interp_linear_result.stepY = 0;
	interp_linear_result.stepZ = 0;

	if(errX < 0)
	{
		errX += fabs(destStepY) + fabs(destStepZ);

		if(destStepX >= 0)
			interp_linear_result.stepX = 1;
		else
			interp_linear_result.stepX = -1;
	}
	else
		errX -= fabs(destStepX);

	if(errY < 0)
	{
		errY += fabs(destStepX) + fabs(destStepZ);

		if(destStepY >= 0)
			interp_linear_result.stepY = 1;
		else
			interp_linear_result.stepY = -1;
	}
	else
		errY -= fabs(destStepY);

	if(errZ < 0)
	{
		errZ += fabs(destStepX) + fabs(destStepY);

		if(destStepZ >= 0)
			interp_linear_result.stepZ = 1;
		else
			interp_linear_result.stepZ = -1;
	}
	else
		errZ -= fabs(destStepZ);

	interp_linear_result.errX = errX;
	interp_linear_result.errY = errY;
	interp_linear_result.errZ = errZ;

	return interp_linear_result;
}

interp_result_t INTERP_LinearCalcStep(int32_t destStepA, int32_t destStepB, int32_t originStepA, int32_t originStepB, int32_t F)
{
	interp_result.stepA = 0;
	interp_result.stepB = 0;

	if(destStepA - originStepA >= 0 && destStepB - originStepB >= 0) // QUADRANT 1
	{
		if(F > 0)
		{
			F = F - fabsf(destStepB - originStepB);
			interp_result.stepA = 1;
		}
		else
		{
			F = F + fabsf(destStepA - originStepA);
			interp_result.stepB = 1;
		}
	}
	else if(destStepA - originStepA < 0 && destStepB - originStepB >= 0) // QUADRANT 2
	{
		if(F > 0)
		{
			F = F - fabsf(destStepB - originStepB);
			interp_result.stepA = -1;
		}
		else
		{
			F = F + fabsf(destStepA - originStepA);
			interp_result.stepB = 1;
		}
	}
	else if (destStepA - originStepA < 0 && destStepB - originStepB < 0) // QUADRANT 3
	{
		if(F > 0)
		{
			F = F - fabsf(destStepB - originStepB);
			interp_result.stepA = -1;
		}
		else
		{
			F = F + fabsf(destStepA - originStepA);
			interp_result.stepB = -1;
		}
	}
	else if(destStepA - originStepA >= 0 && destStepB - originStepB < 0) // QUADRANT 4
	{
		if(F > 0)
		{
			F = F - fabsf(destStepB - originStepB);
			interp_result.stepA = 1;
		}
		else
		{
			F = F + fabsf(destStepA - originStepA);
			interp_result.stepB = -1;
		}
	}

	interp_result.F = F;

	return interp_result;
}

interp_result_t INTERP_CircleCWCalcStep(int32_t currentStepA, int32_t currentStepB, int32_t centerStepA, int32_t centerStepB, int32_t F)
{
	interp_result.stepA = 0;
	interp_result.stepB = 0;

    if(currentStepA - centerStepA >= 0 && currentStepB - centerStepB >= 0) // QUADRANT 1
    {
		if(F >= 0)
		{
			F = F - 2 * (currentStepA - centerStepA) + 1;
			interp_result.stepA = 1;
		}
		else
		{
			F = F + 2 * (currentStepB - centerStepB) + 1;
			interp_result.stepB = -1;
		}
    }
    else if(currentStepA - centerStepA < 0 && currentStepB - centerStepB >= 0) // QUADRANT 2
    {
		if(F >= 0)
		{
			F = F + 2 * (currentStepA - centerStepA) + 1;
			interp_result.stepA = 1;
		}
		else
		{
			F = F + 2 * (currentStepB - centerStepB) + 1;
			interp_result.stepB = 1;
		}
    }
    else if (currentStepA - centerStepA < 0 && currentStepB - centerStepB < 0) // QUADRANT 3
    {
		if(F >= 0)
		{
			F = F + 2 * (currentStepA - centerStepA) + 1;
			interp_result.stepA = -1;
		}
		else
		{
			F = F - 2 * (currentStepB - centerStepB) + 1;
			interp_result.stepB = 1;
		}
    }
    else if(currentStepA - centerStepA >= 0 && currentStepB - centerStepB < 0) // QUADRANT 4
    {
		if(F >= 0)
		{
			F = F - 2 * (currentStepA - centerStepA) + 1;
			interp_result.stepA = -1;
		}
		else
		{
			F = F - 2 * (currentStepB - centerStepB) + 1;
			interp_result.stepB = -1;
		}
    }

	interp_result.F = F;

	return interp_result;
}

interp_result_t INTERP_CircleCCWCalcStep(int32_t currentStepA, int32_t currentStepB, int32_t centerStepA, int32_t centerStepB, int32_t F)
{
	interp_result.stepA = 0;
	interp_result.stepB = 0;

	if(currentStepA - centerStepA >= 0 && currentStepB - centerStepB >= 0) // QUADRANT 1
    {
		if(F >= 0)
		{
			F = F - 2 * (currentStepA - centerStepA) + 1;
			interp_result.stepA = -1;
		}
		else
		{
			F = F + 2 * (currentStepB - centerStepB) + 1;
			interp_result.stepB = 1;
		}
    }
	else if(currentStepA - centerStepA < 0 && currentStepB - centerStepB >= 0) // QUADRANT 2
    {
		if(F >= 0)
		{
			F = F + 2 * (currentStepA - centerStepA) + 1;
			interp_result.stepA = -1;
		}
		else
		{
			F = F + 2 * (currentStepB - centerStepB) + 1;
			interp_result.stepB = -1;
		}
    }
	else if (currentStepA - centerStepA < 0 && currentStepB - centerStepB < 0) // QUADRANT 3
    {
		if(F >= 0)
		{
			F = F + 2 * (currentStepA - centerStepA) + 1;
			interp_result.stepA = 1;
		}
		else
		{
			F = F - 2 * (currentStepB - centerStepB) + 1;
			interp_result.stepB = -1;
		}
    }
	else if(currentStepA - centerStepA >= 0 && currentStepB - centerStepB < 0) // QUADRANT 4
    {
		if(F >= 0)
		{
			F = F - 2 * (currentStepA - centerStepA) + 1;
			interp_result.stepA = 1;
		}
		else
		{
			F = F - 2 * (currentStepB - centerStepB) + 1;
			interp_result.stepB = 1;
		}
    }

	interp_result.F = F;

	return interp_result;
}



