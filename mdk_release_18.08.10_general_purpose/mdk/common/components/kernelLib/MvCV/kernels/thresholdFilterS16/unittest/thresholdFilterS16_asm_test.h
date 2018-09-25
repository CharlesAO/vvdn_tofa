#ifndef _THRESHOLD_FILTERS16_ASM_TEST_H_
#define _THRESHOLD_FILTERS16_ASM_TEST_H_

#include <mv_types.h>

extern unsigned int thresholdFilterS16CycleCount;
void thresholdFilterS16_asm_test(short *line, unsigned int threshold, unsigned int width, 
								 unsigned int *indexList, unsigned int *count);
								
								
#endif //_THRESHOLD_FILTERS16_ASM_TEST_H_

