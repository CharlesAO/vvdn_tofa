#ifndef SOLVER_H_INCLUDED
#define SOLVER_H_INCLUDED

/* operations count **********************************************************/

unsigned long long n_add, n_mul, n_div, n_sqrt;
unsigned long long n_scan; // number of element scans (GJ only)

/* Core functions ************************************************************/

// perform T = A'A
void syrk_tn_l( float *T_, const float *A_, int n );

// perform c = A'b
void gemv_t( float *c, const float *A_, const float *b, int n );

// perform L <= LL'=A
void potrf_ln( float *L_, const float *A_, int n );

// solve Ly = b
void trsv_ln( float *y, const float *L_, const float *b, int n );

// solve L'x = y
void trsv_lt( float *x, const float *U_, const float *y, int n );

// perform B = A^-1
void gauss( float *B_, const float *A_, int n );

/* Verification functions ****************************************************/

// perform x = Ab
void gemv( float *x, const float *A, const float *b, int n );

// compute ||a-b||
double norm( const float *a, const float *b, int n );

/* Helper functions **********************************************************/

// random data generator, y:=[-1.0 .. 1.0)
void genf( float*, int, float min, float max ); 

// perform C = Ab
void gemv_n( float *c, const float *A, const float *b, int n );

// check if NxN matrix is symmetric, or lower triangular
int issymm( const float*, int );                   
int isltm( const float*, int );                   

// print N vector
void printv( const char*, const float*, int );     
// print NxN matrix
void printm( const char*, const float*, int );     

// save N vector
void savev( const float*, int, const char* );
// save NxN matrix
void savem( const float*, int, const char* );


#endif
