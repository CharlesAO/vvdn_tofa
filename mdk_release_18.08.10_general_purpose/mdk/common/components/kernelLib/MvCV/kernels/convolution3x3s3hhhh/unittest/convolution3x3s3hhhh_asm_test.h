#ifndef _CONVOLUTION3X3s3hhhh_ASM_TEST_H_
#define _CONVOLUTION3X3s3hhhh_ASM_TEST_H_

#include "half.h"

extern unsigned int Convolution3x3s3hhhhCycleCount;
void Convolution3x3s3hhhh_asm_test(half** in, half** out, half conv[9], unsigned int width);

#endif 