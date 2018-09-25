#ifndef _MAT_DECOMP_ASM_TEST_H_
#define _MAT_DECOMP_ASM_TEST_H_

typedef	struct
{
	unsigned int	m, n;
	unsigned int	max_m, max_n, max_size;
	float			**me, *base;
} MAT_test;


extern unsigned int matQRDecompCycleCount;
void matQRDecomp_asm_test(MAT_test *A, MAT_test *B, MAT_test *out);

#endif //_MAT_DECOMP_ASM_TEST_H_
