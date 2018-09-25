#ifndef _MAT_Minus_ASM_TEST_H_
#define _MAT_Minus_ASM_TEST_H_

typedef	struct
{
	unsigned int	m, n;
	unsigned int	max_m, max_n, max_size;
	float			**me, *base;
} MAT_test;


extern unsigned int matMinusCycleCount;
MAT_test *matMinus_asm_test(const MAT_test *A, const MAT_test *B, MAT_test *out);

#endif //_MAT_Minus_ASM_TEST_H_
