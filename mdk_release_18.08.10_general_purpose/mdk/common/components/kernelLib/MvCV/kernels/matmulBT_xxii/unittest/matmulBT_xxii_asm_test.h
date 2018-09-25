#ifndef __MATMULBT_XXII_ASM_TEST_H__
#define __MATMULBT_XXII_ASM_TEST_H__

extern unsigned int mvcvMatmulBT_xxiiCycleCount;
void mvcvMatmulBT_xxii_asm_test(unsigned char *A, unsigned char *B, unsigned int *C, unsigned int m, unsigned int k, unsigned int n,
    unsigned int wA, unsigned int wB, unsigned int wC);

#endif //__MATMULBT_XXII_ASM_TEST_H__
