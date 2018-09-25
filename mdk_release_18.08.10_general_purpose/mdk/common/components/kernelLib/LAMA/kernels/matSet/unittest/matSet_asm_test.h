#ifndef _MAT_Set_ASM_TEST_H_
#define _MAT_Set_ASM_TEST_H_

typedef	struct
{
	unsigned int	m, n;
	unsigned int	max_m, max_n, max_size;
	float			**me, *base;
} MAT_test;


extern unsigned int matSetCycleCount;
MAT_test *matSet_asm_test(float scalar, MAT_test *in, int start_row, int start_col, int length_row, int length_col);

#endif //_MAT_Set_ASM_TEST_H_
