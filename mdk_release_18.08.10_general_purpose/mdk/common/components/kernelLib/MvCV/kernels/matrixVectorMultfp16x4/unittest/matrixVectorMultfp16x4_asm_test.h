#ifndef _MATRIXVECTORFP16x4_ASM_TEST_H_
#define _MATRIXVECTORFP16x4_ASM_TEST_H_

#define TEST_ARRAY_SIZE (10*16)

extern unsigned int matrixVectorMultfp16x4CycleCount;
void matrixVectorMultfp16x4_asm_test(half** input1, half* input2, unsigned int width, half* output);

#endif //_MATRIXVECTORFP16x4_ASM_TEST_H_
