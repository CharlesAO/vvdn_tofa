#include <matGet.h>
#include <stdio.h>
#include <string.h>

/// matrix get block
/// @param[in]  in         - structure that contains info regarding width, height, pointer to input matrix
/// @param[out] out       - structure that contains info regarding width, height, pointer to output matrix
/// @param[in]  start_row - index of row where to start from
/// @param[in]  start_col - index of col where to start from

MAT *mvcvMatGet(const MAT *in, MAT *out, int start_row, int start_col)
{
	if (in == NULL || out == NULL)
	{
		printf("input or out put is empty. %d %s\n", __LINE__, __FILE__);
		return NULL;
	}
	if (start_row < 0 || start_col < 0 || out->m + start_row > in->m || out->n + start_col > in->n)
	{
		printf("input is invalid.\n");
		return NULL;
	}
	int i;
	for (i = 0; i < (int)out->m; i++)
	{
		memcpy(&(out->base[i * out->n]), &(in->base[(start_row + i) * in->n +start_col]), sizeof(Real)*out->n);
	}
	return out;
}
