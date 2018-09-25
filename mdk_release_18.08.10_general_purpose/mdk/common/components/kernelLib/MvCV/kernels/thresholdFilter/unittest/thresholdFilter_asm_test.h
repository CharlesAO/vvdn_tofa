#ifndef _THRASHOLD_FILTER_ASM_TEST_H_
#define _THRASHOLD_FILTER_ASM_TEST_H_

#include <mv_types.h>

extern unsigned int thresholdFilterCycleCount;
void thresholdFilter_asm_test(float *cornerScores, float threshold, unsigned int width, 
								unsigned int posOffset, unsigned short *candPos, unsigned int *candCount);
#endif //_THRASHOLD_FILTER_ASM_TEST_H_

