#ifndef _CONVERT_U8_FP16_ASM_TEST_H_
#define _CONVERT_U8_FP16_ASM_TEST_H_

#include "half.h"

extern unsigned int Convert_u8_fp16CycleCount;

void Convert_u8_fp16_asm_test(unsigned char* in, half* out, unsigned int width);

#endif //_CONVERT_U8_FP16_ASM_TEST_H_
