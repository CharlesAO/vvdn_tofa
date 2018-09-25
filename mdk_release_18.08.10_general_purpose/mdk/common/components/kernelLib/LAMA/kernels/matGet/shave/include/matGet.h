#ifndef __MAT_Get_H__
#define __MAT_Get_H__
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
/// matrix get block
/// @param[in]  in         - structure that contains info regarding width, height, pointer to input matrix
/// @param[out] out       - structure that contains info regarding width, height, pointer to output matrix
/// @param[in]  start_row - index of row where to start from
/// @param[in]  start_col - index of col where to start from

MVCV_FUNC(MAT, *mvcvMatGet, const MAT *in, MAT *out, int start_row, int start_col)
//!@}

#endif //__MAT_Get_H__
