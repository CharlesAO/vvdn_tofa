#ifndef _BILINEAR_INTERPOLATION_ALIGNED_U16_ASM_TEST_H_
#define _BILINEAR_INTERPOLATION_ALIGNED_U16_ASM_TEST_H_

#include <mv_types.h>

#define MAX_SIZE 5000

extern unsigned int bilinearInterpolation16uCycleCount;
void bilinearInterpolationAligned16u_asm_test(unsigned char* src, int src_step, unsigned char* dst, int dst_step, unsigned int* win_size, float* center);

#endif //_BILINEAR_INTERPOLATION_U16_ASM_TEST_H_
