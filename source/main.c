#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "fsl_gpio.h"

#include "gcode.h"
#include "motion.h"
#include "hmi.h"
#include "misc.h"


void main(void)
{
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitDebugConsole();

	HMI_Init();

	MOTION_InitPins();
	MOTION_Enable();

	HMI_LedOn(kHMI_GreenLED);

	uint8_t framesCount = 9;

	char* str[framesCount];
	char gcode[32];

	str[0] = "F1000";
	str[1] = "G00 X10 Y-10";
	str[2] = "G01 X10 Y0 I0 J0";
	str[3] = "G03 X0 Y10 I0 J0";
	str[4] = "X-10 Y0 I0 J0";
	str[5] = "X0 Y-10 I0 J0";
	str[6] = "X10 Y0";
	str[7] = "G01 X10 Y10";
	str[8] = "G00 X0 Y0";

	while(true)
	{
		for(int i = 0; i < framesCount; i++)
		{
			while(MOTION_GetState() != kMOTION_Idle);
			memset(&gcode, 0, 32);
			strncpy(gcode, str[i], strlen(str[i]));

			GCODE_ExecuteFrame(GCODE_Parse(gcode));
		}

		while(MOTION_GetState() != kMOTION_Idle);

		MOTION_Disable();
		HMI_LedOff(kHMI_GreenLED);
		HMI_LedOn(kHMI_RedLED);

		break;
	}
}















