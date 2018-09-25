#ifndef __THRESHOLD_DISPARITY_ASM_TEST__
#define __THRESHOLD_DISPARITY_ASM_TEST__

extern unsigned int mvcvThresholdDisparityCycleCount;
void mvcvThresholdDisparity_asm_test(unsigned char* input, unsigned char* output, unsigned int* countThresPixels, 
                                     unsigned int width, unsigned int threshold,
                	                 unsigned int setValue);

#endif //__THRESHOLD_DISPARITY_ASM_TEST__
