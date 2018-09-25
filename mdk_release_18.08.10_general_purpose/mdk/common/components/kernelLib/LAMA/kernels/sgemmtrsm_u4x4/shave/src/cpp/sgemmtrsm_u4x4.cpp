#include <sgemmtrsm_u4x4.h>

/// sgemmtrsm_u 4x4 kernel
/// @param[in]  k     - the number of columns of a12 and rows of b21
/// @param[in]  alpha - the address of a scalar to be applied to b11
/// @param[in]  a12   - 4 x k submatrix of the packed (column-stored) micro-panel of A, situated to the right of the 4 x 4 triangular submatrix a11
/// @param[in]  a11   - 4 x 4 upper triangular submatrix within the packed (column-stored) micro-panel of matrix A, situated to the left of a12
/// @param[in]  b21   - k x 4 submatrix of the packed (row-stored) micro-panel of B, situated below the 4 x 4 block b11
/// @param[in]  b11   - 4 x 4 submatrix of the packed (row-stored) micro-panel of B, situated above b21
/// @param[out] c11   - 4 x 4 submatrix of matrix C, stored according to rsc and csc
/// @param[in]  rs_c  - row stride of c11
/// @param[in]  cs_c  - column stride of c11
void mvcvsgemmtrsm_u4x4(s32   k,
                        float *alpha,
                        float *a12,
                        float *a11,
                        float *b21,
                        float *b11,
                        float *c11,
                        s32   rs_c,
                        s32   cs_c)
{
    float ab[4 * 4];
    float rho11;
    s32   iter, i, j, l;

    /* sgemm_4x4 */
    /* b11 = alpha * b11 - a12 * b21; */
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
               ab[4*j + i] += a12[i] * b21[j];
           }
        }
        a12 += 4;
        b21 += 4;
    }

    for(j = 0; j < 4; j++)
    {
        for(i = 0; i < 4; i++)
        {
            b11[i*4 + j] = -ab[i + j*4] + (*alpha) * b11[i*4 + j];
        }
    }

    /* strsm_u4x4 */
    /* b11  = inv(a11) * b11;
       c11  = b11; */
    for(iter = 0; iter < 4; iter++)
    {
        i = 4 - iter - 1;

        for(j = 0; j < 4; ++j)
        {
            rho11 = 0.0F;
            for(l = 0; l < iter; l++)
            {
                rho11  += a11[5*i + 4*l + 4] * b11[4*i + j + 4*l + 4];
            }

            b11[4*i + j] = a11[5*i] * (b11[4*i + j] - rho11);
            /* Output final result to matrix C. */
            c11[i*rs_c + j*cs_c] = b11[4*i + j];
        }
    }
}
