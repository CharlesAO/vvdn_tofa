#ifndef __THRESHOLD_DISPARITY_FP16_ASM_TEST__
#define __THRESHOLD_DISPARITY_FP16_ASM_TEST__

extern unsigned int mvcvThresholdDisparityFp16CycleCount;
void mvcvThresholdDisparityFp16_asm_test(unsigned char* input, half* output, unsigned int* countThresPixels, 
                                     unsigned int width, unsigned int threshold,
                	                 half setValue);

#endif //__THRESHOLD_DISPARITY_FP16_ASM_TEST__
