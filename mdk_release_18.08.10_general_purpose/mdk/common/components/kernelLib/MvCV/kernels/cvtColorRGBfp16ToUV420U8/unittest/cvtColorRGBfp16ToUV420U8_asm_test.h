#ifndef _cvtColorRGBfp16ToUV420U8_ASM_TEST_H_
#define _cvtColorRGBfpToUV420U8_TEST_H_

extern unsigned int cvtColorRGBfo16ToUV420U8CycleCount;
void cvtColorRGBfp16ToUV420U8_asm_test(half** inputRGB, unsigned char *Uout, unsigned char *Vout,unsigned int width);

#endif //_ccvtColorRGBfp16ToUV420U8_ASM_TEST_H_
