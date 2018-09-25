#ifndef __DOWNSAMPLEBILINEARLINE_ASM_TEST_H__
#define __DOWNSAMPLEBILINEARLINE_ASM_TEST_H__


extern unsigned int mvcvDownsampleBilinearLineCycleCount;
void mvcvDownsampleBilinearLine_asm_test(unsigned short *inLine, unsigned int *tempLine, unsigned short *outLine, unsigned int inWidth, unsigned int select);

#endif //__DOWNSAMPLEBILINEARLINE_ASM_TEST_H__
