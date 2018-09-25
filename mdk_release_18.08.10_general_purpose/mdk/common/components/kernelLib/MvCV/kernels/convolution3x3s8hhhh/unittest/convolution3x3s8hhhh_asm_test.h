#ifndef _CONVOLUTION3X3S8HHHH_ASM_TEST_H_
#define _CONVOLUTION3X3S8HHHH_ASM_TEST_H_

#include "half.h"

extern unsigned int convolution3x3s8hhhhCycleCount;
void Convolution3x3s8hhhh_asm_test(half** in, half** out, half conv[9], unsigned int width);

#endif //_CONVOLUTION3X3S8HHHH_ASM_TEST_H_
