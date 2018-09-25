#ifndef __CENSUS_MATCHING16_ASM_TEST_H__
#define __CENSUS_MATCHING16_ASM_TEST_H__


extern unsigned int mvcvSubPixelFilterCycleCount;
void mvcvSubPixelFilter_asm_test(unsigned char* inputDisparityMap, unsigned char* inputCostVolume, 
                                 unsigned short* outputDisparityMap, unsigned int width, unsigned char maxDisp, 
                                 unsigned char numFractionalBit, unsigned char * lutToUse);

#endif //__CENSUS_MATCHING16_ASM_TEST_H__
