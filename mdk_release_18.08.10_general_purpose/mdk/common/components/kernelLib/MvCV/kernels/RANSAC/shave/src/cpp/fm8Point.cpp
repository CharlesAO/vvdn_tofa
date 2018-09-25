///
/// @file fm8Point.cpp
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief  Fundamental matrix computation by 8 Point algo
///

// Local Headers
#include "fm8Point.h"
#include "svd.h"
#include <stdio.h>


//#if 1
#ifdef __PC__
#define mvcvSVD_asm  mvcvSVD
#endif

///////////////////////////////////////////////////////////////////////////////
///  fm8Point - compute fm by 8 Point algo
///////////////////////////////////////////////////////////////////////////////
// svd(A) = [U, S, V']
float buffer_1[9*9];    // A/U matrix mem
float buffer_2[9*9];    // V matrix mem
float *A[9] = {&buffer_1[0], &buffer_1[9], &buffer_1[18],
               &buffer_1[27], &buffer_1[36], &buffer_1[45],
               &buffer_1[54], &buffer_1[63], &buffer_1[72]
              };
float *V[9] = {&buffer_2[0], &buffer_2[9], &buffer_2[18],
               &buffer_2[27], &buffer_2[36], &buffer_2[45],
               &buffer_2[54], &buffer_2[63], &buffer_2[72]
              };
float S[9];
// svd(AA) = [UU, SS, VV']
float buffer_3[3*3];    // AA/UU matrix mem
float buffer_4[3*3];    // VV matrix mem
float *AA[3] = {&buffer_3[0], &buffer_3[3], &buffer_3[6]};
float *VV[3] = {&buffer_4[0], &buffer_4[3], &buffer_4[6]};
float SS[3];
float buffer_5[9] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};



///////////////////////////////////////////////////////////////////////////////
///  Utils: auxiliar functions
///////////////////////////////////////////////////////////////////////////////

// 3x3 matrix multilplication
inline void mat3x3Mult(float A[9], float B[9], float C[9])
{
    int i, j;
    float sum;
    for( i = 0; i < 3; i++ )
    {
        for( j = 0; j < 3; j++ )
        {
            sum = A[i*3]*B[j];
            sum += A[i*3+1]*B[3+j];
            sum += A[i*3+2]*B[6+j];
            C[i*3+j] = sum;
        }
    }
}



void
fm8Point( float *p_1, float *p_2, float *fm)
{
    // 1. Compute system
    for (int i = 0; i < 8; i++)
    {
        A[i][0] = p_2[2*i] * p_1[2*i];          // x' * x
        A[i][1] = p_2[2*i] * p_1[2*i + 1];      // x' * y
        A[i][2] = p_2[2*i];                     // x'
        A[i][3] = p_2[2*i + 1] * p_1[2*i];      // y' * x
        A[i][4] = p_2[2*i + 1] * p_1[2*i + 1];  // y' * y
        A[i][5] = p_2[2*i + 1];                 // y'
        A[i][6] = p_1[2*i];                     // x
        A[i][7] = p_1[2*i + 1];                 // y
        A[i][8] = 1;                            // 1
    }


    A[8][0] = 0;
    A[8][1] = 0;
    A[8][2] = 0;
    A[8][3] = 0;
    A[8][4] = 0;
    A[8][5] = 0;
    A[8][6] = 0;
    A[8][7] = 0;
    A[8][8] = 0;


    // 2. SVD
    //dsvd(A, 8, 9, S, V);
    mvcvSVD_asm(A, S, V, 9);		/*Check the validity*/


//    herr_t H5LTmake_dataset_float ( hid_t loc_id, const char *dset_name, int rank, const hsize_t *dims, const float *buffer );

    // 3. Rank enforcement

    // copy fundamental matrix
    for(int i = 0; i < 9; i++)
    {
        // Column 8 contains singular vector of min singula value
        // SVD issue: check why last two cols are switched (8<->9)
        AA[i/3][i%3] = V[i][8];
    }

    // decompose
    //dsvd(AA, 3, 3, SS, VV);
    mvcvSVD_asm(AA, SS, VV, 3); // OUT: AA = [US, S, V]

    // minimizes the Frobenius norm
    buffer_5[0] = 1/SS[0];
    buffer_5[4] = 1/SS[1];
    buffer_5[8] = 1/SS[2];
    mat3x3Mult(buffer_3, buffer_5, buffer_1); // compute U as US * S^-1
    buffer_5[0] = SS[0];
    buffer_5[4] = SS[1];
    buffer_5[8] = 0;
    mat3x3Mult(buffer_1, buffer_5, buffer_2); // U*new_S
    buffer_1[0] = buffer_4[0];
    buffer_1[1] = buffer_4[3];
    buffer_1[2] = buffer_4[6];
    buffer_1[3] = buffer_4[1];
    buffer_1[4] = buffer_4[4];
    buffer_1[5] = buffer_4[7];
    buffer_1[6] = buffer_4[2];
    buffer_1[7] = buffer_4[5];
    buffer_1[8] = buffer_4[8];
    mat3x3Mult(buffer_2, buffer_1, fm); // U*new_S*V'
}


