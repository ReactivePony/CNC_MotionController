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
    int8_t stepA;
    int8_t stepB;
    int32_t F;
} interp_result_t;


interp_result_t INTERP_LinearCalcStep(int32_t destStepA, int32_t destStepB, int32_t originStepA, int32_t originStepB, int32_t F);
interp_result_t INTERP_CircleCWCalcStep(int32_t currentStepA, int32_t currentStepB, int32_t centerStepA, int32_t centerStepB, int32_t F);
interp_result_t INTERP_CircleCCWCalcStep(int32_t currentStepA, int32_t currentStepB, int32_t centerStepA, int32_t centerStepB, int32_t F);


#endif /* SOURCE_INTERP_H_ */
