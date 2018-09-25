#ifndef _cvtColorRGBtoLumaNV12_ASM_TEST_H_
#define _cvtColorRGBtoLumaNV12_TEST_H_

extern unsigned int cvtColorRGBtoLumaNV12CycleCount;
void cvtColorRGBtoLumaNV12_asm_test(unsigned char *inputR, unsigned char *inputG, unsigned char *inputB, unsigned char *Yout, float mat[3*3], float offset[1*3], unsigned int width);

#endif //_ccvtColorRGBtoLumaNV12_ASM_TEST_H_
