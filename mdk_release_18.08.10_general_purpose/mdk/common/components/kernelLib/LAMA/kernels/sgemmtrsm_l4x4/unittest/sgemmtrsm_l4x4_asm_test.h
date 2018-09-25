#ifndef _SGEMMTRSM_L4X4_ASM_TEST_H_
#define _SGEMMTRSM_L4X4_ASM_TEST_H_

extern unsigned int sgemmtrsm_l4x4CycleCount;
void sgemmtrsm_l4x4_asm_test(s32 k, float *alpha, float *a10, float *a11, float *b01, float *b11, float *c11, s32 rs_c, s32 cs_c);

#endif //_SGEMMTRSM_L4X4_ASM_TEST_H_
