#ifndef _conv3x3fp32Scharr_ASM_TEST_H_
#define _conv3x3fp32Scharr_ASM_TEST_H_

#include <mv_types.h>

extern unsigned int conv3x3fp32ScharrCycleCount;
void conv3x3fp32Scharr_asm_test(unsigned char* src, float* dstX, float* dstY, float* smooth_k, int height, int width);

#endif //_conv3x3fp32Scharr_ASM_TEST_H_
