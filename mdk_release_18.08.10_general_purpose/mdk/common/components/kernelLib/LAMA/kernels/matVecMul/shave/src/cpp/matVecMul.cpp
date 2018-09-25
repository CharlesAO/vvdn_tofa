#include <matVecMul.h>
#include <stdio.h>

/// vector matrix multiplication kernel
/// Calculates C = C + (A*B') [A*B if B is stored as a transpose matrix]
/// Where A is 1*K, B is K*N and C is 1*N;
/// @param[in]  A - input vector of size 1*K
/// @param[in]  B - input matrix of size K*N
/// @param[out] C - input/output vector of size 1*N
/// @param[in]  K - width of A vector
/// @param[in]  N - number of columns of matrix B

void mvcvMatVecMul(float *A, float *B, float *C, int K, int N)
{
    int vecrow = 0;
    int matcol = 0;
    float acc = 0;
    for (vecrow = 0; vecrow < N; vecrow++)
    {
        acc = C[vecrow];
        for (matcol = 0; matcol < K; matcol++)
        {    //K
            acc += A[matcol] * B[vecrow*K + matcol];
        }
        C[vecrow] = acc;
         // printf("\nacc = %f\n", acc);
    }
}
