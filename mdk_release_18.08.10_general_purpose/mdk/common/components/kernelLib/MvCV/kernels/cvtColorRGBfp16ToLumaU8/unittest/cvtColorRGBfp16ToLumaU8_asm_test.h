#ifndef _cvtColorRGBfp16ToLuma_ASM_TEST_H_
#define _cvtColorRGBfp16ToLuma_TEST_H_

extern unsigned int cvtColorRGBtoLumaCycleCount;
void cvtColorRGBfp16ToLumaU8_asm_test(half **inputRGB, unsigned char *Yout, unsigned int width);

#endif //_ccvtColorRGBfp16ToLuma_ASM_TEST_H_
