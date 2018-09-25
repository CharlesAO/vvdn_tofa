#include <sgemmtrsm_l4x4.h>

/// sgemmtrsm_l 4x4 kernel
/// @param[in]  k     - the number of columns of a10 and rows of b01
/// @param[in]  alpha - the address of a scalar to be applied to b11
/// @param[in]  a10   - 4 x k submatrix of the packed (column-stored) micro-panel of A, situated to the left of the 4 x 4 triangular submatrix a11
/// @param[in]  a11   - 4 x 4 lower triangular submatrix within the packed (column-stored) micro-panel of matrix A, situated to the right of a10
/// @param[in]  b01   - k x 4 submatrix of the packed (row-stored) micro-panel of B, situated above the 4 x 4 block b11
/// @param[in]  b11   - 4 x 4 submatrix of the packed (row-stored) micro-panel of B, situated below b01
/// @param[out] c11   - 4 x 4 submatrix of matrix C, stored according to rsc and csc
/// @param[in]  rs_c  - row stride of c11
/// @param[in]  cs_c  - column stride of c11
void mvcvsgemmtrsm_l4x4(s32   k,
                        float *alpha,
                        float *a10,
                        float *a11,
                        float *b01,
                        float *b11,
                        float *c11,
                        s32   rs_c,
                        s32   cs_c)
{
    float ab[4 * 4];
    float rho11;
    s32   i, j, l;

    /* sgemm4x4*/
    /* b11 = alpha * b11 - a10 * b01; */
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
                ab[4*j + i] += a10[i] * b01[j];
            }
        }
        a10 += 4;
        b01 += 4;
    }

    for(j = 0; j < 4; j++)
    {
        for(i = 0; i < 4; i++)
        {
            b11[i*4 + j] = (*alpha) * b11[i*4 + j] - ab[j*4 + i];
        }
    }

    /* strsm_l4x4 */
    /* b11  = inv(a11) * b11;
       c11  = b11; */
    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 4; j++)
        {
            rho11 = 0.0F;
            for(l = 0; l < i; l++)
            {
                rho11 += a11[l *4 + i] * b11[l*4 + j];
            }

            b11[i*4 + j] = a11[5*i] * (b11[i*4 + j] - rho11);

            /* Output final result to matrix C. */
            c11[i*rs_c + j*cs_c] = b11[i*4 + j];
        }
    }
}
