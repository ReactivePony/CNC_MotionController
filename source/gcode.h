/*
 * gcode.h
 *
 *  Created on: 10 июля 2017 г.
 *      Author: My
 */

#ifndef SOURCE_GCODE_H_
#define SOURCE_GCODE_H_

typedef enum _gcode_command
{
	kGCODE_G00 = 0,
	kGCODE_G01 = 1,
	kGCODE_G02 = 2,
	kGCODE_G03 = 3,
	kGCODE_G04 = 4,
	kGCODE_G17 = 17,
	kGCODE_G18 = 18,
	kGCODE_G19 = 19,

	kGCODE_NC = 100,

} gcode_command_t;

typedef enum _gcode_param_type
{
	kGCODE_X = 0,
	kGCODE_Y = 1,
	kGCODE_Z = 2,
	kGCODE_P = 3,
	kGCODE_F = 4,
	kGCODE_S = 5,
	kGCODE_R = 6,
	kGCODE_D = 7,
	kGCODE_L = 8,
	kGCODE_I = 9,
	kGCODE_J = 10,
	kGCODE_K = 11,
} gcode_param_type_t;

typedef struct _gcode_param
{
	gcode_param_type_t type;
	double value;
} gcode_param_t;

typedef struct _gcode_frame
{
	gcode_command_t command;
	gcode_param_t *params;
	uint8_t paramsCount;
} gcode_frame_t;

typedef struct _gcode_parse_result
{
	gcode_frame_t* frames;
	uint8_t framesCount;
} gcode_parse_result_t;

gcode_frame_t GCODE_Parse(char* gcode);
void GCODE_ExecuteFrame(gcode_frame_t frame);
void GCODE_ExecuteParams(gcode_param_t params[], uint8_t paramsCount);


#endif /* SOURCE_GCODE_H_ */
