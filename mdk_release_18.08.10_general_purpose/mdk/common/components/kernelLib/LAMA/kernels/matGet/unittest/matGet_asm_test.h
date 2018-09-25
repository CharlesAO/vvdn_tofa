#ifndef _MAT_Get_ASM_TEST_H_
#define _MAT_Get_ASM_TEST_H_

typedef	struct
{
	unsigned int	m, n;
	unsigned int	max_m, max_n, max_size;
	float			**me, *base;
} MAT_test;


extern unsigned int matGetCycleCount;
MAT_test *matGet_asm_test(const MAT_test *in, MAT_test *out, int start_row, int start_col);

#endif //_MAT_Get_ASM_TEST_H_
