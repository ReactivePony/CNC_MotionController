/*
 * gcode.c
 *
 *  Created on: 10 июля 2017 г.
 *      Author: My
 */

#include <stdlib.h>
#include <stdio.h>

#include "fsl_common.h"

#include "gcode.h"
#include "motion.h"

gcode_command_t command = kGCODE_NC;

void GCODE_ExecuteFrame(gcode_frame_t gcode_frame)
{
	if(gcode_frame.command != kGCODE_NC)
		command = gcode_frame.command;

	GCODE_ExecuteParams(gcode_frame.params, gcode_frame.paramsCount);

	switch (command)
	{
		case kGCODE_G00:
			MOTION_SetSpeedMode(kMOTION_Rapid);
			MOTION_SetState(kMOTION_Linear);
			break;
		case kGCODE_G01:
			MOTION_SetSpeedMode(kMOTION_Normal);
			MOTION_SetState(kMOTION_Linear);
			break;
		case kGCODE_G02:
			MOTION_SetSpeedMode(kMOTION_Normal);
			MOTION_SetState(kMOTION_CircleCW);
			break;
		case kGCODE_G03:
			MOTION_SetSpeedMode(kMOTION_Normal);
			MOTION_SetState(kMOTION_CircleCCW);
			break;
		default:
			break;
	}
}

void GCODE_ExecuteParams(gcode_param_t params[], uint8_t paramsCount)
{
	for(int i = 0; i < paramsCount; i++)
	{
		switch (params[i].type)
		{
			case kGCODE_X:
				MOTION_SetDestX(params[i].value);
				break;
			case kGCODE_Y:
				MOTION_SetDestY(params[i].value);
				break;
			case kGCODE_Z:
				MOTION_SetDestZ(params[i].value);
				break;
			case kGCODE_P:
				break;
			case kGCODE_F:
				MOTION_SetSpeed(params[i].value);
				break;
			case kGCODE_S:
				break;
			case kGCODE_R:
				break;
			case kGCODE_D:
				break;
			case kGCODE_L:
				break;
			case kGCODE_I:
				MOTION_SetCenterX(params[i].value);
				break;
			case kGCODE_J:
				MOTION_SetCenterY(params[i].value);
				break;
			case kGCODE_K:
				MOTION_SetCenterZ(params[i].value);
				break;
			default:
				break;
		}
	}
}

gcode_frame_t GCODE_Parse(char* gcode)
{
	gcode_frame_t frame;
	gcode_command_t command = kGCODE_NC;
	gcode_param_t params[8];
	uint8_t paramCount = 0;

	char *buffer[16];
	char *segment;

	buffer[0] = strtok(gcode, " ");

	for(int i = 1; i < 16; i++)
	{
		segment = strtok(NULL, " ");

		if(segment != NULL)
			buffer[i] = segment;
		else
			buffer[i] = "\0";
	}

	for(int i = 0; i < 16; i++)
	{
		if(strstr(buffer[i], "G") != NULL)
		{
			char str[strlen(buffer[i]) - 1];

			strncpy(str, buffer[i] + 1, strlen(buffer[i]) - 1);

			if(atoi(str) == 0)
				command = kGCODE_G00;
			else if(atoi(str) == 1)
				command = kGCODE_G01;
			else if(atoi(str) == 2)
				command = kGCODE_G02;
			else if(atoi(str) == 3)
				command = kGCODE_G03;
		}

		if(strstr(buffer[i], "X") != NULL)
		{
			char str[strlen(buffer[i])];

			strncpy(str, buffer[i] + 1, strlen(buffer[i]) - 1);

			params[paramCount].type = kGCODE_X;
			params[paramCount].value = atof(str);

			paramCount++;
		}

		if(strstr(buffer[i], "Y") != NULL)
		{
			char str[strlen(buffer[i])];

			strncpy(str, buffer[i] + 1, strlen(buffer[i]) - 1);

			params[paramCount].type = kGCODE_Y;
			params[paramCount].value = atof(str);

			paramCount++;
		}

		if(strstr(buffer[i], "F") != NULL)
		{
			char str[strlen(buffer[i])];

			strncpy(str, buffer[i] + 1, strlen(buffer[i]) - 1);

			params[paramCount].type = kGCODE_F;
			params[paramCount].value = atof(str);

			paramCount++;
		}
	}

	frame.command = command;
	frame.params = params;
	frame.paramsCount = paramCount;

	return frame;
}






















