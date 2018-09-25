#ifndef _CONVOLUTION9x9S8HHHH_ASM_TEST_H_
#define _CONVOLUTION9x9S8HHHH_ASM_TEST_H_

#include "half.h"

extern unsigned int convolution9x9s8hhhhCycleCount;
void Convolution9x9s8hhhh_asm_test(half** in, half** out, half conv[81], unsigned int width);

#endif //_CONVOLUTION9x9S8HHHH_ASM_TEST_H_
