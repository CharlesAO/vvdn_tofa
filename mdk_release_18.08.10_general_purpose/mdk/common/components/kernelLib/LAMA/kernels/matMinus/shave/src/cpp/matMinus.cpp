#include <matMinus.h>
#include <stdio.h>

///matrix substraction
/// @param[in]  A   - structure that contains info regarding width, height, pointer to first matrix
/// @param[in]  B   - structure that contains info regarding width, height, pointer to second matrix
/// @param[out] out - structure that contains info regarding width, height, pointer to output matrix

MAT *mvcvMatMinus(const MAT *A, const MAT *B, MAT *out)
{
	if ((A == NULL) || (B == NULL) || (out == NULL))
	{
		printf("input or output is empty. %d %s\n", __LINE__, __FILE__);
		return NULL;
	}
	if ((A->m != B->m) || (A->n != B->n) || (A->m != out->m) || (A->n != out->n))
	{
		printf("mat dim donot match. %d %s\n", __LINE__, __FILE__);
		return NULL;
	}
	int i, j, m = out->m, n = out->n;
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			out->base[i * out->n + j] = A->base[i  * A->n +  j] - B->base[i  * out->n +  j];
		}			
	}
	return out;
}
