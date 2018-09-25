#include <strsm_l4x4.h>

/// strsm_l 4x4 kernel
/// @param[in]  a     - address of input matrix (lower triangular)
/// @param[in]  b     - address of input matrix
/// @param[out] c     - address of the output matrix
/// @param[in]  rs_c  - row stride
/// @param[in]  cs_c  - column stride

void lamaStrsm_l4x4(float *a, float *b, float *c, int rs_c, int cs_c)
{
    int i, j, l;
    float temp[4*4];

    for(i = 0; i < 16; i++)
    	temp[i] = b[i];

    // c  = inv(a) * b;
    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 4; j++)
        {
            float rho = 0.0F;
            for(l = 0; l < i; l++)
            {
                rho += a[l *4 + i] * temp[l*4 + j];
            }

            temp[i*4 + j] = a[5*i] * (temp[i*4 + j] - rho);

            /* Output final result to matrix C */
            c[i*rs_c + j*cs_c] = temp[i*4 + j];
        }
    }
}