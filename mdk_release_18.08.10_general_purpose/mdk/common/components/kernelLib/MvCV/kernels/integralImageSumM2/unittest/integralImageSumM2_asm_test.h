#ifndef _INTEGRALIMAGESUM_ASM_TEST_H_
#define _INTEGRALIMAGESUM_ASM_TEST_H_

extern unsigned int integralImageSumCycleCount;
void integralImageSum_asm_test(unsigned int* out, unsigned char* in, unsigned int runNr, unsigned int** prevPointer, unsigned int width);

#endif //_INTEGRALIMAGESUM_ASM_TEST_H_
