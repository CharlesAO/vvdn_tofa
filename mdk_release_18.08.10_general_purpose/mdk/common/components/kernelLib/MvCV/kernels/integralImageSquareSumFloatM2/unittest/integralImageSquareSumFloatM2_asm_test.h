#ifndef _INTEGRALIMAGESQUARESUMF_ASM_TEST_H_
#define _INTEGRALIMAGESQUARESUMF_ASM_TEST_H_

extern unsigned int integralImageSquareSumFCycleCount;
void integralImageSquareSumFloat_asm_test(float* out, unsigned char* in, unsigned int runNr, float** prevPointer, unsigned int width);

#endif //_INTEGRALIMAGESQUARESUMF_ASM_TEST_H_
