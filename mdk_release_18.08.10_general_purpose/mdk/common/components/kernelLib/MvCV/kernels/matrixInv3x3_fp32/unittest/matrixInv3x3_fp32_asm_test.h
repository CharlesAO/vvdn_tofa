#ifndef __MATRIX_INV3x3_FP32_ASM_TEST_H_
#define __MATRIX_INV3x3_FP32_ASM_TEST_H_

#include "half.h"

extern unsigned int matrixInv3x3_fp32CycleCount;
void matrixInv3x3_fp32_asm_test(float **in, float **out);

#endif //__MATRIX_INV3x3_FP32_ASM_TEST_H_
