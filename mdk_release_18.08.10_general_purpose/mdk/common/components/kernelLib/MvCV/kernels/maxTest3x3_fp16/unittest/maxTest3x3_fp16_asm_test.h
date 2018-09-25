#ifndef _MAXTEST3x3_ASM_TEST_H_
#define _MAXTEST3x3_ASM_TEST_H_
#include "half.h"

extern unsigned int cycleCount;

void maxTest3x3_fp16_asm_test(half* inBufferCandidates, half** inBuffer, unsigned int width, unsigned int maxLocationsIn[],
                              unsigned int maxLocationsOut[], unsigned int maxCountIn,  unsigned int* maxCountOut);

#endif //_MAXTEST3x3_ASM_TEST_H_
