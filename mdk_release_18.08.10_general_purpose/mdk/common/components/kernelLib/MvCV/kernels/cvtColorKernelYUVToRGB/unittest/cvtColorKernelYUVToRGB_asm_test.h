#ifndef _CVTCOLORKERNELYUVTORGB_ASM_TEST_H_
#define _CVTCOLORKERNELYUVTORGB_ASM_TEST_H_

extern unsigned int cvtColorKernelYUVToRGBCycleCount;

void cvtColorKernelYUVToRGB_asm_test(unsigned char *y, unsigned char *u ,unsigned char *v ,unsigned char *rgb , unsigned int width);

#endif //_CVTCOLORKERNELYUVTORGB_ASM_TEST_H_
