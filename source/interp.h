/*
 * interp.h
 *
 *  Created on: 10 июля 2017 г.
 *      Author: My
 */

#ifndef SOURCE_INTERP_H_
#define SOURCE_INTERP_H_

typedef struct _interp_result
{
    int8_t stepX;
    int8_t stepY;
    int32_t F;
} interp_result_t;


interp_result_t INTERP_LinearCalcStep(int32_t destStepX, int32_t destStepY, int32_t originStepX, int32_t originStepY, int32_t F);
interp_result_t INTERP_CircleCWCalcStep(int32_t currentStepX, int32_t currentStepY, int32_t centerStepX, int32_t centerStepY, int32_t F);
interp_result_t INTERP_CircleCCWCalcStep(int32_t currentStepX, int32_t currentStepY, int32_t centerStepX, int32_t centerStepY, int32_t F);


#endif /* SOURCE_INTERP_H_ */
