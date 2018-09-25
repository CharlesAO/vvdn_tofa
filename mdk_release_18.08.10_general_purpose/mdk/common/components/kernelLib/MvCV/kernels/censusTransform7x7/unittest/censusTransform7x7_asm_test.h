#ifndef _CENSUSTRANSFORM7x7_ASM_TEST_H_
#define _CENSUSTRANSFORM7x7_ASM_TEST_H_

#include "half.h"

extern unsigned int censusTransform7x7CycleCount;
void censusTransform7x7_asm_test(unsigned char** in, unsigned long long* out, unsigned int width);

#endif //_CENSUSTRANSFORM7x7_ASM_TEST_H_
