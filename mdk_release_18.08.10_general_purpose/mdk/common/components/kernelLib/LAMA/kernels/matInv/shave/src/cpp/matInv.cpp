
# include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cassert>
#include "matInv.h"


#define MATRIX_BUFFER_SIZE         (1024*33)

static char matrix_buffer[MATRIX_BUFFER_SIZE];
float mat_inv_temp[100 * 100] = { 0.0f };

#define STATUS_REPORT(status_type, module_status_id) \
        printf("ERROR CODE: %d; MODULE ID: %d; %d %s", status_type, module_status_id, __LINE__, NULL)
//#endif

static unsigned int matrix_buffer_offset = 0;
static MAT    *A_cp = NULL;
static MAT *out1=NULL;
static PERM    *P = NULL;

void *allocate_memory(int size)
{
    void *ret_val = NULL;

    int actual_size = (size + 7)&(~7);     //round up to 8 boundary

    if (matrix_buffer_offset + actual_size > MATRIX_BUFFER_SIZE)
    {
        return (void *)NULL;
    }

    ret_val = (void *)&matrix_buffer[matrix_buffer_offset];

    matrix_buffer_offset += actual_size;
    return ret_val;
}

MAT *m_get(unsigned int m_dim, unsigned int n_dim)
{
    MAT *mat = NULL;
    unsigned int i = 0;

    mat = (MAT *)allocate_memory(sizeof(MAT));

    if (NULL == mat)
    {
        printf("allocate memory error. %d %s\n", __LINE__, __FILE__);
        return NULL;
    }

    mat->m = m_dim; mat->max_m = m_dim;
    mat->n = n_dim; mat->max_n = n_dim;

    //    printf("mat->m ,mat->n %d %d \n",mat->m,mat->n);
    mat->max_size = m_dim * n_dim;

    mat->base = (Real *)allocate_memory(m_dim * n_dim * sizeof(Real));
    if (NULL == mat->base)

    {
        printf("allocate memory error. %d %s\n", __LINE__, __FILE__);
        return NULL;
    }
    else
    {
        memset(mat->base, 0, sizeof(Real)* m_dim * n_dim);
    }

    mat->me = (Real **)allocate_memory(m_dim * sizeof(Real *));
    if (NULL == mat->me)
    {
        printf("allocate memory error. %d %s\n", __LINE__, __FILE__);
        return NULL;
    }

    for (i = 0; i < m_dim; ++i)
    {
        mat->me[i] = &(mat->base[i * n_dim]);
    }

    return mat;
}


MAT *m_resize(MAT *mat, unsigned int m_dim, unsigned int n_dim)
{
    unsigned int i;

    if (NULL == mat)
    {
        mat = m_get(m_dim, n_dim);
    }
    else
    {
        if (m_dim > mat->max_m || n_dim > mat->max_n)
        {
            printf("%d %d %d %d\n", m_dim, n_dim, mat->max_m,mat->max_n);
            printf("dimensions do not match, %d %s\n", __LINE__, __FILE__);
            return NULL;
        }
        if (m_dim != mat->m || n_dim != mat->n)
        {

            mat->m = m_dim;
            mat->n = n_dim;

            for (i = 0; i < m_dim; i++)
                mat->me[i] = &(mat->base[i*n_dim]);


            for (i = 0; i < m_dim; ++i)
                memset(&mat->base[i * n_dim], 0, sizeof(Real)* n_dim);
        }
    }

    return mat;
}

MAT    *m_copy(const MAT *in, MAT *out)
{
    if ((in == NULL) || (out == NULL))
    {
        printf("input or output is empty. %d %s\n", __LINE__, __FILE__);
        return NULL;
    }
    if ((in->m != out->m) || (in->n != out->n))
    {
        printf("mat dim donot match. %d %s\n", __LINE__, __FILE__);
        return NULL;
    }
    if (in == out)
    {
        return out;
    }
    memcpy(out->base,in->base,sizeof(Real)*out->m*out->n);

    return out;
}


PERM *px_get(int size)
{
    PERM    *permute;
    int    i;

    if (size < 0)
    {
        printf("input size cannot be negative.\n");
        return NULL;
    }

    permute = (PERM    *)allocate_memory(sizeof(PERM));
    if (NULL == permute)
    {
        printf("allocate memory failed.\n");
        return NULL;
    }

    permute->size = permute->max_size = size;
    permute->pe = (unsigned int    *)allocate_memory(sizeof(Real)* size);
    if (NULL == permute->pe)
    {
        printf("allocate memory failed.\n");
        return NULL;
    }

    for (i = 0; i < size; i++){
        permute->pe[i] = i;
    }

    return (permute);
}

PERM *px_resize(PERM *perm, unsigned int new_size)
{
    unsigned int i;

    if (NULL == perm)
    {

        perm = px_get(new_size);
    }
    else
    {
        if (new_size > perm->max_size)
        {
            printf("size do not match.\n");
            return NULL;
        }


        if (perm->size < new_size)
        {
            for (i = perm->size; i < new_size; i++)
                perm->pe[i] = i;
        }
        else
        {
            for (i = 0; i < new_size; i++)
                perm->pe[i] = i;
        }

        perm->size = new_size;
    }

    return perm;
}

/* This function decomposes the matrix 'A' into L, U, and P. If successful,
 * the L and the U are stored in 'A', and information about the pivot in 'P'.
 * The diagonal elements of 'L' are all 1, and therefore they are not stored. */
void LUPdecompose(MAT *A, PERM *P, MAT *LU)
{
    unsigned  int i, j, k, kmax = 0, T;
    Real max, t;

    /* Finding the pivot of the LUP decomposition. */
    for(i=0; i< A->m; i++) {

        P->pe[i] = i; //Initializing.
    }

    for(k=0; k<A->m; k++)
    {

        max = -1;
        for(i=k; i<A->n; i++)
        {
            t = A->base[P->pe[i]*A->m+k];
            if(t < 0) t *= -1; //Abosolute value of 't'.

            if(t > max)
            {

                max = t;
                kmax = i;

            }
        }
        if(max == 0)
        {
            printf("\nLUPdecompose(): ERROR: A singular matrix is supplied.\n"\
                    "\tRefusing to proceed any further.\n");
            //   return -1;
        }

        /* Exchanging the rows according to the pivot determined above. */
        T = P->pe[kmax];
        P->pe[kmax] = P->pe[k];
        P->pe[k] = T;
        for(i=0; i<A->m; i++)
        {
            LU->base[k*A->m+i] = A->base[P->pe[k]*A->m+i];
        }
    }
    for(k=0; k<A->m-1; k++)
    {
        for(i=k+1; i<A->n; i++) //Performing substraction to decompose A as LU.
        {
            LU->base[i*A->m+k] = LU->base[i*A->m+k]/LU->base[k*A->m+k];
            for(j=k+1; j<A->m; j+=4) {
                LU->base[i*A->m+j] -= LU->base[i*A->m+k]*LU->base[k*A->m+j];
                if ((j+1) < A->m)
                    LU->base[i*A->m+(j+1)] -= LU->base[i*A->m+k]*LU->base[k*A->m+(j+1)];
                if ((j+2) < A->m)
                    LU->base[i*A->m+(j+2)] -= LU->base[i*A->m+k]*LU->base[k*A->m+(j+2)];
                if ((j+3) < A->m)
                    LU->base[i*A->m+(j+3)] -= LU->base[i*A->m+k]*LU->base[k*A->m+(j+3)];
            }
        }
    } //Now, 'A' contains the L (without the diagonal elements, which are all 1)
    //and the U.

}




/* This function calculates the inverse of the LUP decomposed matrix 'LU' and pivoting
 * information stored in 'P'. The inverse is returned through the matrix 'LU' itselt.
 * 'B', X', and 'Y' are used as temporary spaces. */
void LUPinverse( MAT *LU, PERM *P, MAT *B)
{

    int n, m;
    unsigned int i,j;
    Real t[B->m];
    Real X4[(B->m+1)*4], Y4[(B->n+1)*4];
    Real LUDinv[B->m];

    //Initializing X and Y.
    for(n=0; (unsigned int)n<B->m; n++)
        LUDinv[n] = (Real)1.0f/LU->base[n*B->m+n];

    for(i=0; i<B->n; i++)
    {
        for(j=0; j<B->m; j++)
            B->base[i*B->n+j] = 0;
        B->base[i * B->n + i]=1;
    }

    /* Solving LUX = Pe, in order to calculate the inverse of 'A'. Here, 'e' is a column
     * vector of the identity matrix of size 'size-1'. Solving for all 'e'. */

    for (i = 0; i < B->m; i += 4)
    {

        for ( n = 0; (unsigned int)n < B->n; n++) {
            t[0] = 0;
            t[1] = 0;
            t[2] = 0;
            t[3] = 0;

            for (m = 0; m <= n - 1; m++) {
                t[0] += LU->base[n * LU->n + m] * Y4[m * 4 + 0];
                t[1] += LU->base[n * LU->n + m] * Y4[m * 4 + 1];
                t[2] += LU->base[n * LU->n + m] * Y4[m * 4 + 2];
                t[3] += LU->base[n * LU->n + m] * Y4[m * 4 + 3];
            }

            Y4[n * 4 + 0] = B->base[P->pe[n] * B->n + i + 0] - t[0];
            Y4[n * 4 + 1] = B->base[P->pe[n] * B->n + i  + 1] - t[1];
            Y4[n * 4 + 2] = B->base[P->pe[n] * B->n + i  + 2] - t[2];
            Y4[n * 4 + 3] = B->base[P->pe[n] * B->n + i  + 3] - t[3];

        }

        for (n = B->m - 1; n >= 0; n--)
        {
            t[0] = 0;
            t[1] = 0;
            t[2] = 0;
            t[3] = 0;

            for (m = n + 1; (unsigned int)m < B->n; m++) {
                t[0] += LU->base[n * LU->n + m] * X4[4 * m + 0];
                t[1] += LU->base[n * LU->n + m] * X4[4 * m + 1];
                t[2] += LU->base[n * LU->n + m] * X4[4 * m + 2];
                t[3] += LU->base[n * LU->n + m] * X4[4 * m + 3];
            }

            X4[n * 4 + 0] = (Y4[n * 4 + 0] - t[0]) * LUDinv[n];
            X4[n * 4 + 1] = (Y4[n * 4 + 1] - t[1]) * LUDinv[n];
            X4[n * 4 + 2] = (Y4[n * 4 + 2] - t[2]) * LUDinv[n];
            X4[n * 4 + 3] = (Y4[n * 4 + 3] - t[3]) * LUDinv[n];

        }          //Now, X contains the solution.


        for(j = 0; j<B->m; j++) 
        {
            B->base[j * B->m + i+0] = X4[j*4+0];
        }//Copying 'X' into the same row of 'B'.
        if ((i+1)<B->m) for(j = 0; j<B->m; j++)
        {
         B->base[j * B->m + i+1] = X4[j*4+1];
        }
        if ((i+2)<B->m) for(j = 0; j<B->m; j++) 
         { 
             B->base[j * B->m + i+2] = X4[j*4+2];
         }
        if ((i+3)<B->m) for(j = 0; j<B->m; j++) 
         { 
             B->base[j * B->m + i+3] = X4[j*4+3];
         }

    } //Now, 'B' the transpose of the inverse of 'A'.
}

MAT    *mvcvMatInverseLU(MAT *A, MAT *out)
{
    //unsigned int i=0;
    if (NULL == A)
    {
        printf("input pointer is NULL.\n");
        return NULL;
    }
    if (A->m != A->n || out->m != A->m || out->n != A->n)
    {
        printf("input dim do not match.\n");
        return NULL;
    }
    A_cp = NULL;
    A_cp = m_resize(A_cp, A->m, A->n);
    A_cp = m_copy(A, A_cp);
    out1=m_resize(out1,out->m,out->n);
    P = px_resize(P, A->m);
    LUPdecompose(A_cp,P, out1);
    LUPinverse( out1,P, out);

    return out;
}
