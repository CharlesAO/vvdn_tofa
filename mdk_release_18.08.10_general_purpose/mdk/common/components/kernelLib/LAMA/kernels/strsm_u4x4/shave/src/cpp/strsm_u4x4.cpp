#include <strsm_u4x4.h>

/// strsm_u 4x4 kernel
/// @param[in]  a     - address of input matrix (upper triangular)
/// @param[in]  b     - address of input matrix
/// @param[out] c     - address of the output matrix
/// @param[in]  rs_c  - row stride
/// @param[in]  cs_c  - column stride

void lamaStrsm_u4x4(float *a, float *b, float *c, int rs_c, int cs_c)
{
    int i, j, l, iter;
    float temp[4*4];

    for(i = 0; i < 16; i++)
    	temp[i] = b[i];

    /* strsm_u4x4 */
    /* b  = inv(a) * b;
       c  = b; */
    for(iter = 0; iter < 4; iter++)
    {
        i = 4 - iter - 1;

        for(j = 0; j < 4; ++j)
        {
            float rho = 0.0f;
            for(l = 0; l < iter; l++)
            {
                rho  += a[5*i + 4*l + 4] * temp[4*i + j + 4*l + 4];
            }

            temp[4*i + j] = a[5*i] * (temp[4*i + j] - rho);

            /* Output final result to matrix C. */
            c[i*rs_c + j*cs_c] = temp[4*i + j];
        }
    }
}

