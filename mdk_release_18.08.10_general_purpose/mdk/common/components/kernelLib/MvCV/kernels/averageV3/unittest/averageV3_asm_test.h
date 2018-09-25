#ifndef _AverageV3_ASM_TEST_H_
#define _AverageV3_ASM_TEST_H_

#include "half.h"

extern unsigned int AverageV3CycleCount;
void AverageV3_asm_test(half** in, half** out, unsigned int width);

#endif //_AverageV3_ASM_TEST_H_
