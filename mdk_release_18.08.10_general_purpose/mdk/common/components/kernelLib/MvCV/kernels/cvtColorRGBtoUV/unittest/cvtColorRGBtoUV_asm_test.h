#ifndef _cvtColorRGBtoUV_ASM_TEST_H_
#define _cvtColorRGBtoUV_TEST_H_

extern unsigned int cvtColorRGBtoUVCycleCount;
void cvtColorRGBtoUV_asm_test(unsigned char **inputR, unsigned char **inputG, unsigned char **inputB, unsigned char **Yout, unsigned int width,  unsigned int line);

#endif //_ccvtColorRGBtoUV_ASM_TEST_H_
