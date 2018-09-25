#include <matSet.h>
#include <stdio.h>

///matrix set block
/// @param[in]  in   - structure that contains info regarding width, height, pointer to first matrix
/// @param[out] out  - structure that contains info regarding width, height, pointer to output matrix
/// @param[in] start_row   - index of starting row of block to set
/// @param[in] start_col   - index of starting col of block to set
/// @param[in] length_row   - length of rows
/// @param[in] length_col   - length of cols

MAT *mvcvMatSet(float scalar, MAT *in, int start_row, int start_col, int length_row, int length_col)
{
	if (in == NULL)
	{
		printf("input is empty. %d %s\n", __LINE__, __FILE__);
		return NULL;
	}
	if (start_row < 0 || start_col < 0 || length_row + start_row > (int)in->m || length_col + start_col > (int)in->n)
	{
		printf("input is invalid.\n");
		return NULL;
	}
	int i, j;
	for (i = 0; i < length_row; i++)
	{
		for (j = 0; j < length_col; j++)
		{
			in->base[(start_row + i) * in->n + start_col + j] = scalar;
		}
	}
	return in;
}
