#ifndef _DILATE3X3_ASM_TEST_H_
#define _DILATE3X3_ASM_TEST_H_

extern unsigned int Dilate3x3CycleCount;

void Dilate3x3_asm_test(unsigned char** src, unsigned char** dst, unsigned char** kernel, unsigned int width);

#endif //_DILATE3X3_ASM_TEST_H_
