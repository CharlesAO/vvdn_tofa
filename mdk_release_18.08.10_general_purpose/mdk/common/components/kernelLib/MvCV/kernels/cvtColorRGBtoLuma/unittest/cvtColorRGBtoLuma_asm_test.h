#ifndef _cvtColorRGBtoLuma_ASM_TEST_H_
#define _cvtColorRGBtoLuma_TEST_H_

extern unsigned int cvtColorRGBtoLumaCycleCount;
void cvtColorRGBtoLuma_asm_test(unsigned char **inputR, unsigned char **inputG, unsigned char **inputB, unsigned char **Yout, unsigned int width);

#endif //_ccvtColorRGBtoLuma_ASM_TEST_H_
