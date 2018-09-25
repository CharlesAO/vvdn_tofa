#include <sgemm4x4.h>

/// sgemm 4x4 kernel
/// @param[in] k     - number of columns of A and rows of B
/// @param[in] alpha - address of a scalar to the A * B product.
/// @param[in] a     - 4 x k "micro-panel" matrix stored in packed (column-stored)
/// @param[in] b     - k x 4 "micro-panel" matrix in packed (row-stored) format
/// @param[in] beta  - address of a scalar to the input value of matrix C
/// @param[out] c    - address of a matrix C of dimension 4 x 4, stored according to rsc and csc.
/// @param[in] rs_c  - row stride of matrix C
/// @param[in] cs_c  - column stride of matrix C
void mvcvsgemm4x4(s32   k,
                 float *alpha,
                 float *a,
                 float *b,
                 float *beta,
                 float *c,
                 s32   rs_c,
                 s32   cs_c)
{
    s32   i, j, l;
    float ab[4 * 4];

    for(i = 0; i < 4 * 4; i++)
    {
        ab[i] = 0.0F;
    }

    for(l = 0; l < k; l++)
    {
        for(j = 0; j < 4; j++)
        {
            for(i = 0; i < 4; i++)
            {
                ab[4*j + i] += a[i] * b[j];
            }
        }
        a += 4;
        b += 4;
    }

    for(j = 0; j < 4; j++)
    {
        for(i = 0; i < 4; i++)
        {
            c[i*rs_c + j*cs_c] = (*alpha) * ab[i*1 + j*4] + (*beta) * c[i*rs_c + j*cs_c];
        }
    }
}
