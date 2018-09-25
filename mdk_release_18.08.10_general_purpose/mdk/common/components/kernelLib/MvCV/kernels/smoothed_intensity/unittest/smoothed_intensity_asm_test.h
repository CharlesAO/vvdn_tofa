#ifndef _SMOOTHED_INTENSITY_ASM_TEST_H_
#define _SMOOTHED_INTENSITY_ASM_TEST_H_

extern unsigned int mvcvSmoothed_intensityCycleCount;

void mvcvSmoothed_intensity_asm_test(float kp_x, float kp_y, u8* integral,
                                     int integral_stride, float* pattern_pnt_lst,
                                     int pattern_pnt_sz, unsigned short* intensities,
                                     unsigned int width, unsigned int height);

#endif //_SMOOTHED_INTENSITY_ASM_TEST_H_
