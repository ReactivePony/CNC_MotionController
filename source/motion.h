/*
 * motion.h
 *
 *  Created on: 04 июля 2017 г.
 *      Author: My
 */

#ifndef SOURCE_MOTION_H_
#define SOURCE_MOTION_H_


typedef enum _motion_state
{
	kMOTION_Idle = 0,
	kMOTION_Linear = 1,
	kMOTION_CircleCW = 2,
	kMOTION_CircleCCW = 3,

} motion_state_t;

typedef enum _motion_speed_mode
{
	kMOTION_Normal = 0,
	kMOTION_Rapid = 1,
} motion_speed_mode_t;

typedef enum _motion_direction
{
	kMOTION_Direction_CCW = 0,
	kMOTION_Direction_CW = 1,
} motion_direction_t;

typedef enum _motion_axis
{
	kMOTION_AxisX = 0,
	kMOTION_AxisY = 1,
	kMOTION_AxisZ = 2,
} motion_axis_t;

typedef enum _motion_plane
{
	kMOTION_PlaneXY = 0,
	kMOTION_PlaneXZ = 1,
	kMOTION_PlaneYZ = 2,

} motion_plane_t;

void MOTION_InitPins();
void MOTION_Enable();
void MOTION_Disable();

void MOTION_MakeStep(motion_axis_t axis, motion_direction_t direction);

void MOTION_SetState(motion_state_t state);
motion_state_t MOTION_GetState();

void MOTION_SetDestX(float destX);
void MOTION_SetDestY(float destY);
void MOTION_SetDestZ(float destZ);

void MOTION_SetCenterX(float centerX);
void MOTION_SetCenterY(float centerY);
void MOTION_SetCenterZ(float centerZ);

void MOTION_SetSpeed(uint32_t speed);
void MOTION_SetSpeedMode(motion_speed_mode_t speed_mode);

void MOTION_SetPlane(motion_plane_t plane);


#endif /* SOURCE_MOTION_H_ */
