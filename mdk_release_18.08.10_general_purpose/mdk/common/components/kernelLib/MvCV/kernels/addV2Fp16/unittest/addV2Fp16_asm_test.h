#ifndef _ADDV2FP16_ASM_TEST_H_
#define _ADDV2FP16_ASM_TEST_H_

extern unsigned int AddV2Fp16CycleCount;

void AddV2Fp16_asm_test(half** dst, half** input, u32 width);

#endif //_ADDV2FP16_H_
