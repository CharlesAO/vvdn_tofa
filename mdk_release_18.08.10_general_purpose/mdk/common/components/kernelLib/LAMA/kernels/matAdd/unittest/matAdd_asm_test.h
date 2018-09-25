#ifndef _MAT_ADD_ASM_TEST_H_
#define _MAT_ADD_ASM_TEST_H_

typedef	struct
{
	unsigned int	m, n;
	unsigned int	max_m, max_n, max_size;
	float			**me, *base;
} MAT_test;


extern unsigned int matAddCycleCount;
MAT_test *matAdd_asm_test(const MAT_test *A, const MAT_test *B, MAT_test *out);

#endif //_MAT_ADD_ASM_TEST_H_
