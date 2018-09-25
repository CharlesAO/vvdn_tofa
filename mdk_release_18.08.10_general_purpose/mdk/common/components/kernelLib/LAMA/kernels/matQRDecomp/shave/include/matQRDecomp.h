#ifndef __MAT_DECOMP_H__
#define __MAT_DECOMP_H__
#include <mv_types.h>
#include <mvcv_macro.h>


#define MAX_DIM 48
#define MAX_HEIGHT 48
#define MAX_WIDTH 48

#define TEST_MATRIX_HEIGHT MAX_HEIGHT
#define TEST_MATRIX_WIDTH MAX_WIDTH

typedef float Real;

/* matrix definition */
typedef	struct
{
	unsigned int	m, n;
	unsigned int	max_m, max_n, max_size;
	Real			**me, *base;
} MAT;

//!@{
///matrix QR decomposition - Householder method
/// @param[in]  m        - structure that contains info regarding width, height, pointer to input matrix
/// @param[in] Q0, Q1   - structure that contains info regarding width, height, pointer to temporary matrices needed for computation
/// @param[in] Z0, Z1   - structure that contains info regarding width, height, pointer to temporary matrices needed for computation
/// @param[out] Q        - structure that contains info regarding width, height, pointer to Q matrix
/// @param[out] R        - structure that contains info regarding width, height, pointer to R matrix

MVCV_FUNC(void, mvcvMatQRDecomp, MAT *A, MAT *Q, MAT *R, MAT *q0, MAT *q1, MAT *z0, MAT *z1);

//!@}

#endif //__MAT_DECOMP_H__
