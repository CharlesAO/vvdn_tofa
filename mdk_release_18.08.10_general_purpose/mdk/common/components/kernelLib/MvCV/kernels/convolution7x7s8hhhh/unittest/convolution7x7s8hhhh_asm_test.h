#ifndef _CONVOLUTION7x7S8HHHH_ASM_TEST_H_
#define _CONVOLUTION7x7S8HHHH_ASM_TEST_H_

#include "half.h"

extern unsigned int convolution7x7s8hhhhCycleCount;
void Convolution7x7s8hhhh_asm_test(half** in, half** out, half conv[49], unsigned int width);

#endif //_CONVOLUTION7x7s8HHHH_ASM_TEST_H_
