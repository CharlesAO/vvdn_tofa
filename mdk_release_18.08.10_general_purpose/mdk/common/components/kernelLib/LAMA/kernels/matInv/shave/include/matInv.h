#ifndef _MAT_INV_H_
#define _MAT_INV_H_

#include <mv_types.h>
#include <mvcv_macro.h>


typedef float Real;

typedef	struct
{
	unsigned int	size, max_size, *pe;
} PERM;

/* matrix definition */
typedef	struct
{
	unsigned int	m, n;
	unsigned int	max_m, max_n, max_size;
	Real			**me, *base;
} MAT;


//!@{
/// Matrix Invers kernel - computes the inverse of a given input matrix
/// @param[in]  in  - input matrix with info regarding it
/// @param[out] out - output matrix with info regarding it

MVCV_FUNC(MAT, *mvcvMatInverseLU, MAT*in, MAT *out);
//to be continue (QR)


#endif
