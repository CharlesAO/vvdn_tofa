#ifndef _CONVOLUTION7x7S4HHHH_ASM_TEST_H_
#define _CONVOLUTION7x7S4HHHH_ASM_TEST_H_

#include "half.h"

extern unsigned int convolution7x7s4hhhhCycleCount;
void Convolution7x7s4hhhh_asm_test(half** in, half** out, half conv[49], unsigned int width);

#endif //_CONVOLUTION7x7S4HHHH_ASM_TEST_H_
