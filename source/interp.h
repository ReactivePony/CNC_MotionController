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
    int8_t stepZ;
    int32_t errX;
    int32_t errY;
    int32_t errZ;
} interp_result_t;


interp_result_t INTERP_LinearCalcStep(int32_t destStepX, int32_t destStepY, int32_t destStepZ, int32_t originStepX, int32_t originStepY, int32_t originStepZ, int32_t errX, int32_t errY, int32_t errZ);
interp_result_t INTERP_CircleCWCalcStep(int32_t currentStepA, int32_t currentStepB, int32_t centerStepA, int32_t centerStepB, int32_t F);
interp_result_t INTERP_CircleCCWCalcStep(int32_t currentStepA, int32_t currentStepB, int32_t centerStepA, int32_t centerStepB, int32_t F);


#endif /* SOURCE_INTERP_H_ */
