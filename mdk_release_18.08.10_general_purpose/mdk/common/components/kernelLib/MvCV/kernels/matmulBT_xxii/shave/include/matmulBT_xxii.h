#ifndef __MATMULBT_XXII_H__
#define __MATMULBT_XXII_H__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// mvcvMatmulBT_xxii      - matrix multiplication
/// @param[in] A            - first array
/// @param[in] B            - second array
/// @param[out] C           - resulted array
/// @param[in] m            - number of lines
/// @param[in] k            - common dimension
/// @param[in] n            - number of columns
/// @param[in] wA            - rows of array A
/// @param[in] wB            - columns of array B
/// @param[in] wC            - rows of array C
/// @return    Nothing

MVCV_FUNC(void, mvcvMatmulBT_xxii, unsigned char *A, unsigned char *B, unsigned int *C, unsigned int m, unsigned int k, unsigned int n,
    unsigned int wA, unsigned int wB, unsigned int wC)
//!@}

#endif //__MATMULBT_XXII_H__
