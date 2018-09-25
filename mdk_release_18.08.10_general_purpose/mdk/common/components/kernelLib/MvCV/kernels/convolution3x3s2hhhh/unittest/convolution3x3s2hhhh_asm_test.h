#ifndef _CONVOLUTION3x3s2hhhh_ASM_TEST_H_
#define _CONVOLUTION3x3s2hhhh_ASM_TEST_H_

#include "half.h"

extern unsigned int convolution3x3s2hhhhCycleCount;
void Convolution3x3s2hhhh_asm_test(half** in, half** out, half conv[9], unsigned int width);

#endif //_CONVOLUTION3x3s2hhhh_ASM_TEST_H_
