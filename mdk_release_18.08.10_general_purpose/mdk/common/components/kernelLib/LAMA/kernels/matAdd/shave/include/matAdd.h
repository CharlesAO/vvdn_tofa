#ifndef __MAT_ADD_H__
#define __MAT_ADD_H__
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
///matrix addition
/// @param[in]  A   - structure that contains info regarding width, height, pointer to first matrix
/// @param[in]  B   - structure that contains info regarding width, height, pointer to second matrix
/// @param[out] out - structure that contains info regarding width, height, pointer to output matrix

MVCV_FUNC(MAT, *mvcvMatAdd, const MAT *A, const MAT *B, MAT *out)
//!@}

#endif //__MAT_ADD_H__
