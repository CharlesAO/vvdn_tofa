#include <matTranspose.h>
#include <stdio.h>

///matrix transpose
/// @param[in]  A    - structure that contains info regarding width, height, pointer to first matrix
/// @param[out] out  - structure that contains info regarding width, height, pointer to output matrix

MAT *mvcvMatTranspose(const MAT *in, MAT *out)
{
	if ((in == NULL) || (out == NULL))
	{
		printf("input or output is empty. %d %s\n", __LINE__, __FILE__);
		return NULL;
	}
	if (in == out)
	{
		printf("input and output's address cannot the same. %d %s\n", __LINE__, __FILE__);
		return NULL;
	}
	if (in->base == out->base)
	{
		printf("input and output's base address cannot the same. %d %s\n", __LINE__, __FILE__);
		return NULL;
	}
	if ((in->m != out->n) || (in->n != out->m))
	{
		printf("mat dim donot match. %d %s\n", __LINE__, __FILE__);
		return NULL;
	}
	int i, j, m = out->m, n = out->n;
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			out->base[i * out->n + j] = in->base[j  * in->n + i];
		}
	}
	return out;
}
