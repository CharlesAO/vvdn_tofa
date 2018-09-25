#include "matmulBT_xxii.h"
#include <math.h>

//#define ADD_C_AT_THE_END    

unsigned char mult_u8f(const unsigned char a, const unsigned char b)
{
    u64 u_res = (u64)floor(((double)(a * b) / 255) + 0.5);
    unsigned char mult = (unsigned char)((unsigned int)u_res);
    return mult;
}


void mvcvMatmulBT_xxii(unsigned char *A, unsigned char *B, unsigned int *C, unsigned int m, unsigned int k, unsigned int n,
    unsigned int wA, unsigned int wB, unsigned int wC)
{
    unsigned int row_out;
    unsigned int col_out;
    unsigned int common_dim;
    unsigned int acc;

    for (row_out = 0; row_out < m; row_out++)
    {
        for (col_out = 0; col_out < n; col_out++)
        {

#ifdef ADD_C_AT_THE_END
            acc = 0;
#else
            acc = C[row_out * wC + col_out];
#endif
            for (common_dim = 0; common_dim < k; common_dim++)
            {
                unsigned char a = A[row_out * wA + common_dim];
                unsigned char b = B[col_out * wB + common_dim];

                unsigned char mult = (unsigned char)mult_u8f(a, b);
                acc += mult;
            }
#ifdef ADD_C_AT_THE_END
            C[row_out * wC + col_out] += acc;
#else
            C[row_out * wC + col_out] = acc;
#endif
        }
    }
}
