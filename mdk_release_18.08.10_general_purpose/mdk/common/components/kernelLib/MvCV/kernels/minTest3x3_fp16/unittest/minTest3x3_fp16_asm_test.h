#ifndef _MINTEST3X3_ASM_TEST_H_
#define _MINTEST3X3_ASM_TEST_H_
#include "half.h"

extern unsigned int cycleCount;

void minTest3x3_fp16_asm_test(half* inBufferCandidates, half** inBuffer, unsigned int width, unsigned int minLocationsIn[],
        unsigned int minLocationsOut[], unsigned int minCountIn,  unsigned int* minCountOut);

#endif //_MINTEST3X3_ASM_TEST_H_
