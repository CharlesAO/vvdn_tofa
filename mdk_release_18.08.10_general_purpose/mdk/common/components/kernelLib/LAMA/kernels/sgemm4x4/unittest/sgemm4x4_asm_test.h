#ifndef _SGEMM4X4_ASM_TEST_H_
#define _SGEMM4X4_ASM_TEST_H_

extern unsigned int sgemm4x4CycleCount;
void sgemm4x4_asm_test(s32 k, float *alpha, float *a, float *b, float *beta, float *c, s32 rs_c, s32 cs_c);

#endif //_SGEMM4X4_ASM_TEST_H_
