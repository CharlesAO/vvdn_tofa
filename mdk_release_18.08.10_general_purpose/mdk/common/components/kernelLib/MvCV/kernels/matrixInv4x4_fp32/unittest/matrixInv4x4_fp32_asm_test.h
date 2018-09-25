#ifndef __MATRIX_INV4x4_FP32_ASM_TEST_H_
#define __MATRIX_INV4x4_FP32_ASM_TEST_H_

#include "half.h"

extern unsigned int matrixInv4x4_fp32CycleCount;
void matrixInv4x4_fp32_asm_test(float *in, float *out);

#endif //__MATRIX_INV4x4_FP32_ASM_TEST_H_
