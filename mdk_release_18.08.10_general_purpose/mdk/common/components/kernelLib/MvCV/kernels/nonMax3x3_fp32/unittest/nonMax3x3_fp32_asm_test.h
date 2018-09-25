#ifndef _NONMAX_3x3_FP32_ASM_TEST_H_
#define _NONMAX_3x3_FP32_ASM_TEST_H_

extern unsigned int nonMax3x3_fp32CycleCount;
void nonMax3x3_fp32_asm_test(unsigned short* candPos, unsigned int candCount, float* corners1, float* corners2, float* corners3, unsigned short* candOutPos, unsigned int* candOutCount);
#endif //_NONMAX_3x3_FP32_ASM_TEST_H_
