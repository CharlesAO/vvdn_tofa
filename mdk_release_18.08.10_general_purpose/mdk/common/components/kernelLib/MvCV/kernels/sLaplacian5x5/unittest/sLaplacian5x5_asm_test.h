#ifndef _sLaplacian5x5_ASM_TEST_H_
#define _sLaplacian5x5_ASM_TEST_H_

#include "half.h"

extern unsigned int sLaplacian5x5CycleCount;
void sLaplacian5x5_asm_test(unsigned char** in, unsigned char** out, unsigned int width);

#endif //_sLaplacian5x5_ASM_TEST_H_
