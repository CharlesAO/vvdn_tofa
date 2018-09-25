#ifndef _cvtColorRGBtoNV12_ASM_TEST_H_
#define _cvtColorRGBtoNV12_TEST_H_

extern unsigned int cvtColorRGBtoNV12CycleCount;
void cvtColorRGBtoNV12_asm_test(unsigned char **inputR, unsigned char **inputG, unsigned char **inputB, unsigned char **Yout, unsigned char **UVout,  unsigned int width, unsigned int line);

#endif //_ccvtColorRGBtoNV12_ASM_TEST_H_
