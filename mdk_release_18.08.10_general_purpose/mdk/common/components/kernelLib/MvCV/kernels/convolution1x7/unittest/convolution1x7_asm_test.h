#ifndef _CONVOLUTION1x7_ASM_TEST_H_
#define _CONVOLUTION1x7_ASM_TEST_H_

#include "half.h"

extern unsigned int convolution1x7CycleCount;
void Convolution1x7_asm_test(unsigned char** in, unsigned char** out, half* conv, unsigned int width);

#endif //_CONVOLUTION1x7_ASM_TEST_H_
