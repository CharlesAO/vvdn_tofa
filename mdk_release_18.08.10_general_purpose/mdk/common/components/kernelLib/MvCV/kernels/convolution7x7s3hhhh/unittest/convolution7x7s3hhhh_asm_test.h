#ifndef _CONVOLUTION7x7s3hhhh_ASM_TEST_H_
#define _CONVOLUTION7x7s3hhhh_ASM_TEST_H_

#include "half.h"

extern unsigned int convolution7x7s3hhhhCycleCount;
void Convolution7x7s3hhhh_asm_test(half** in, half** out, half conv[49], unsigned int width);

#endif //_CONVOLUTION7x7s3hhhh_ASM_TEST_H_
