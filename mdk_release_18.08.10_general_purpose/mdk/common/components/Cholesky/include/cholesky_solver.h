
#ifndef SHAVE_CHOLESKY_SOLVER_H
#define SHAVE_CHOLESKY_SOLVER_H

#define ENTRYPOINT

/// @brief This will left-multiply b, by the transposed of matrix A: c := A' * b
/// @param[out]  c      - output matrix
/// @param[in]   A      - input matrix
/// @param[in]   b      - array
/// @param[in]   n      - size of matrix
/// @param[in]   wA     - size of matrix element
void gemvl_t( float *c, const float *A, const float *b, int n, int wA );

///@brief This will multiply A transposed, by itself (normal), updating only the lower half of the matrix (below the main diagonal): A|lower := A' * A
/// @param[in]    A      - input matrix ?
/// @param[out]   cA     - output matrix
/// @param[in]    n      - size of matrix
/// @param[in]    wA     - size of matrix element
void syrk_tn_l( float* A, float* cA, int n, int sA );

///@brief This will perform Cholesky decomposition of the lower triangular matrix, storing the result as-is (normal: also to the lower half): A := L such that L * L' == A|original
/// @param[in]   L       - input matrix
/// @param[in]   n       - size of matrix
/// @param[in]   sL      - size of matrix element
void potrf_ln( float *L, int n, int sL );

///@brief This will solve the triangular equation with the coefficients on the left side, A being lower-triangular, and not transposed (normal): y = b \ A (from: A * y = b, compute y = A^(-1) * b)
/// @param[out]  y      - array with results ( y = A^(-1) * b)
/// @param[in]   L      - input lower-triangular matrix
/// @param[in]   b      - array of solution 
/// @param[in]   n      - size of matrix
/// @param[in]   sL     - size of matrix element
void trsvl_ln( float *y,  float *L, float *b, int n, int sL );

///@brief This will solve the triangular equation with the coefficients on the left side, A being lower-triangular, and transposed: y = b \ A’ (from: A' * y = b, compute y = (A')^(-1) * b )
/// @param[out]  x      - array with results ( y = (A')^(-1) * b )
/// @param[in]   L      - input lower-triangular matrix
/// @param[in]   b      - array of solution 
/// @param[in]   n      - size of matrix
/// @param[in]   sL     - size of matrix element
void trsvl_lt( float *x, const float *L,float* y, int n, int sL );



#endif


