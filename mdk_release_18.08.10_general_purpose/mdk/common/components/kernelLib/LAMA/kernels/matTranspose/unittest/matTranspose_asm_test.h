#ifndef _MAT_Transpose_ASM_TEST_H_
#define _MAT_Transpose_ASM_TEST_H_

typedef	struct
{
	unsigned int	m, n;
	unsigned int	max_m, max_n, max_size;
	float			**me, *base;
} MAT_test;


extern unsigned int matTransposeCycleCount;
MAT_test *matTranspose_asm_test(const MAT_test *A, MAT_test *out);

#endif //_MAT_Transpose_ASM_TEST_H_
