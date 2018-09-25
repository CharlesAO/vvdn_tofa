#ifndef __MAT_Set_H__
#define __MAT_Set_H__
#include <mv_types.h>
#include <mvcv_macro.h>


typedef float Real;

/* matrix definition */
typedef	struct
{
	unsigned int	m, n;
	unsigned int	max_m, max_n, max_size;
	Real			**me, *base;
} MAT;

//!@{
///matrix set block
/// @param[in]  in   - structure that contains info regarding width, height, pointer to first matrix
/// @param[out] out  - structure that contains info regarding width, height, pointer to output matrix
/// @param[in] start_row   - index of starting row of block to set
/// @param[in] start_col   - index of starting col of block to set
/// @param[in] length_row   - length of rows
/// @param[in] length_col   - length of cols

MVCV_FUNC(MAT, *mvcvMatSet, float scalar, MAT *in, int start_row, int start_col, int length_row, int length_col)
//!@}

#endif //__MAT_Set_H__
