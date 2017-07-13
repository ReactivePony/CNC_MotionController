/*
 * interp.c
 *
 *  Created on: 10 èşëÿ 2017 ã.
 *      Author: MB
 */

#include "fsl_common.h"

#include "arm_math.h"

#include "interp.h"

interp_result_t interp_result;

interp_result_t INTERP_LinearCalcStep(int32_t destStepX, int32_t destStepY, int32_t destStepZ, int32_t originStepX, int32_t originStepY, int32_t originStepZ, int32_t errX, int32_t errY, int32_t errZ)
{
	interp_result.stepX = 0;
	interp_result.stepY = 0;
	interp_result.stepZ = 0;

	if(errX < 0)
	{
		errX += fabs(destStepY - originStepY) + fabs(destStepZ - originStepZ);

		if(destStepX - originStepX > 0)
			interp_result.stepX = 1;
		else
			interp_result.stepX = -1;
	}
	else
		errX -= fabs(destStepX - originStepX);

	if(errY < 0)
	{
		errY += fabs(destStepX - originStepX) + fabs(destStepZ - originStepZ);

		if(destStepY - originStepY > 0)
			interp_result.stepY = 1;
		else
			interp_result.stepY = -1;
	}
	else
		errY -= fabs(destStepY - originStepY);

	if(errZ < 0)
	{
		errZ += fabs(destStepX - originStepX) + fabs(destStepY - originStepY);

		if(destStepZ - originStepZ > 0)
			interp_result.stepZ = 1;
		else
			interp_result.stepZ = -1;
	}
	else
		errZ -= fabs(destStepZ - originStepZ);

	interp_result.errX = errX;
	interp_result.errY = errY;
	interp_result.errZ = errZ;

	return interp_result;
}

interp_result_t INTERP_CircleCWCalcStep(int32_t currentstepX, int32_t currentstepY, int32_t centerstepX, int32_t centerstepY, int32_t F)
{
	interp_result.stepX = 0;
	interp_result.stepY = 0;

    if(currentstepX - centerstepX >= 0 && currentstepY - centerstepY >= 0) // QUADRANT 1
    {
		if(F >= 0)
		{
			F = F - 2 * (currentstepX - centerstepX) + 1;
			interp_result.stepX = 1;
		}
		else
		{
			F = F + 2 * (currentstepY - centerstepY) + 1;
			interp_result.stepY = -1;
		}
    }
    else if(currentstepX - centerstepX < 0 && currentstepY - centerstepY >= 0) // QUADRANT 2
    {
		if(F >= 0)
		{
			F = F + 2 * (currentstepX - centerstepX) + 1;
			interp_result.stepX = 1;
		}
		else
		{
			F = F + 2 * (currentstepY - centerstepY) + 1;
			interp_result.stepY = 1;
		}
    }
    else if (currentstepX - centerstepX < 0 && currentstepY - centerstepY < 0) // QUADRANT 3
    {
		if(F >= 0)
		{
			F = F + 2 * (currentstepX - centerstepX) + 1;
			interp_result.stepX = -1;
		}
		else
		{
			F = F - 2 * (currentstepY - centerstepY) + 1;
			interp_result.stepY = 1;
		}
    }
    else if(currentstepX - centerstepX >= 0 && currentstepY - centerstepY < 0) // QUADRANT 4
    {
		if(F >= 0)
		{
			F = F - 2 * (currentstepX - centerstepX) + 1;
			interp_result.stepX = -1;
		}
		else
		{
			F = F - 2 * (currentstepY - centerstepY) + 1;
			interp_result.stepY = -1;
		}
    }

	interp_result.errX = F;

	return interp_result;
}

interp_result_t INTERP_CircleCCWCalcStep(int32_t currentstepX, int32_t currentstepY, int32_t centerstepX, int32_t centerstepY, int32_t F)
{
	interp_result.stepX = 0;
	interp_result.stepY = 0;

	if(currentstepX - centerstepX >= 0 && currentstepY - centerstepY >= 0) // QUADRANT 1
    {
		if(F >= 0)
		{
			F = F - 2 * (currentstepX - centerstepX) + 1;
			interp_result.stepX = -1;
		}
		else
		{
			F = F + 2 * (currentstepY - centerstepY) + 1;
			interp_result.stepY = 1;
		}
    }
	else if(currentstepX - centerstepX < 0 && currentstepY - centerstepY >= 0) // QUADRANT 2
    {
		if(F >= 0)
		{
			F = F + 2 * (currentstepX - centerstepX) + 1;
			interp_result.stepX = -1;
		}
		else
		{
			F = F + 2 * (currentstepY - centerstepY) + 1;
			interp_result.stepY = -1;
		}
    }
	else if (currentstepX - centerstepX < 0 && currentstepY - centerstepY < 0) // QUADRANT 3
    {
		if(F >= 0)
		{
			F = F + 2 * (currentstepX - centerstepX) + 1;
			interp_result.stepX = 1;
		}
		else
		{
			F = F - 2 * (currentstepY - centerstepY) + 1;
			interp_result.stepY = -1;
		}
    }
	else if(currentstepX - centerstepX >= 0 && currentstepY - centerstepY < 0) // QUADRANT 4
    {
		if(F >= 0)
		{
			F = F - 2 * (currentstepX - centerstepX) + 1;
			interp_result.stepX = 1;
		}
		else
		{
			F = F - 2 * (currentstepY - centerstepY) + 1;
			interp_result.stepY = 1;
		}
    }

	interp_result.errX = F;

	return interp_result;
}



