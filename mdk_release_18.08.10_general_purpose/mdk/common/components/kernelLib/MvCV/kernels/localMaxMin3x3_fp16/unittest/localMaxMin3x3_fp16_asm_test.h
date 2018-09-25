#ifndef _MINMAXKERNEL_ASM_TEST_H_
#define _MINMAXKERNEL_ASM_TEST_H_
#include "half.h"

extern unsigned int cycleCount;

void localMaxMin3x3_fp16_asm_test(half** inBuffer, unsigned int width, unsigned int maxLocationList[],
        unsigned int minLocationList[], unsigned int* minCount,  unsigned int* maxCount);

#endif //_MINMAXKERNEL_ASM_TEST_H_
