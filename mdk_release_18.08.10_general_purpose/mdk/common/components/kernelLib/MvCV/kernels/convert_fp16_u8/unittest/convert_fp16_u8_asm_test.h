#ifndef _CONVERT_FP16_U8_ASM_TEST_H_
#define _CONVERT_FP16_U8_ASM_TEST_H_

#include "half.h"

extern unsigned int Convert_fp16_u8CycleCount;

void Convert_fp16_u8_asm_test(half* in, unsigned char* out, unsigned int width);

#endif //_CONVERT_FP16_U8_ASM_TEST_H_
