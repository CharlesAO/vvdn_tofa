#ifndef _CVTCOLORRGBTOYUV_ASM_TEST_H_
#define _CVTCOLORRGBTOYUV_ASM_TEST_H_

extern unsigned int cvtColorRGBtoYUVCycleCount;

void RGBtoYUV_asm_test(unsigned char *rgb, unsigned char *y ,unsigned char *u ,unsigned char *v , unsigned int width);

#endif //_CVTCOLORRGBTOYUV_ASM_TEST_H_
