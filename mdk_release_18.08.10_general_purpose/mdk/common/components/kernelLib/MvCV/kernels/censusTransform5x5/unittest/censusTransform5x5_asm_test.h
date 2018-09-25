#ifndef _CENSUSTRANSFORM5X5_ASM_TEST_H_
#define _CENSUSTRANSFORM5X5_ASM_TEST_H_

#include "half.h"

extern unsigned int censusTransform5x5CycleCount;
void censusTransform5x5_asm_test(unsigned char** in, unsigned int* out, unsigned int width);

#endif //_CENSUSTRANSFORM5X5_ASM_TEST_H_
