#include <matQRDecomp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


/// @param[in]  m        - structure that contains info regarding width, height, pointer to input matrix
/// @param[in] Q0, Q1   - structure that contains info regarding width, height, pointer to temporary matrices needed for computation
/// @param[in] Z0, Z1   - structure that contains info regarding width, height, pointer to temporary matrices needed for computation
/// @param[out] Q        - structure that contains info regarding width, height, pointer to Q matrix
/// @param[out] R        - structure that contains info regarding width, height, pointer to R matrix


static float refl_vec[MAX_DIM] __attribute__ ((aligned (16)))  = { 0.0f };

static MAT *m_copy(const MAT *in, MAT *out);
static MAT *m_mlt(const MAT *A, const MAT *B, MAT *out);
static MAT *m_transp(const MAT *in, MAT *out);

void mvcvMatQRDecomp(MAT* m, MAT *Q, MAT *R, MAT *q0, MAT *q1, MAT *z0, MAT *z1)
{
	int height = m->m;
	int width = m->n;

	float sum = 0;
	int i,j,k;

	m_copy(m, z0);

	for (int i = 0; i < height; i++)
	{
		refl_vec[i] = z0->base[i*width];
	}
	for (int i = 0; i < height; i++)
		sum += refl_vec[i]*refl_vec[i];

	float N1 = sqrt(sum);
	sum -= refl_vec[0]*refl_vec[0];
	if (m->base[0] < 0)
		N1 = -N1;
	refl_vec[0] += N1;
	sum += refl_vec[0]*refl_vec[0];
	float N2 = -2.0f/sum;

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < height; j++)
		{
			q0->base[i*height + j] =  refl_vec[i]*refl_vec[j];
			q0->base[i*height + j] *= N2;
		}

		q0->base[i*height + i] += 1;
	}

	m_mlt(q0,z0, z1);
	m_copy(q0,Q);
	m_copy(z1,z0);

	for (k = 1; k < height-1 && k < width; k++)
	{
		sum = 0.0;
		for (i = k; i < width;i++)
			z0->base[(k-1)*width + i] = 0;
		z0->base[(k-1)*width + k-1] = 1;

		for (i = k; i < height; i++)
		{
			z0->base[i*width + k-1] = 0;
			refl_vec[i] = z0->base[i*width + k];
			sum += refl_vec[i]*refl_vec[i];
		}

		for (i = 0; i < k; i++) refl_vec[i] = 0;

		N1 = sqrt(sum);
		sum -= refl_vec[k]*refl_vec[k];
		if (m->base[k*width+k] < 0 )
			N1 = -N1;
		refl_vec[k] += N1;
		sum += refl_vec[k]*refl_vec[k];
		N2 = -2.0f/sum;

		for (i = 0; i < height; i++)
		{
			for (j = 0; j < height ; j++)
			{
				q0->base[i*height+j] = refl_vec[i]*refl_vec[j];
				q0->base[i*height+j] *= N2;
			}
			q0->base[i*height+i] += 1;
		}

		m_mlt(q0,z0,z1);
		m_mlt(q0,Q,q1);
		m_copy(q1,Q);
		m_copy(z1,z0);
	}

	m_mlt(Q,m,R);
	m_transp(Q, q0);
	m_copy(q0,Q);

}

/* copy matrix */
MAT *m_copy(const MAT *in, MAT *out)
{
    if (in == out)
    {
        return out;
    }
    memcpy(out->base,in->base,sizeof(Real)*out->m*out->n);
    return out;
}

/* matrix multiply matrix */
MAT *m_mlt(const MAT *A, const MAT *B, MAT *out)
{
    int i, j, k, out_m = out->m, out_n = out->n, A_n = A->n;
    for (i = 0; i < out_m; i++)
    {
        for (j = 0; j < out_n; j++)
        {
            out->base[i*out_n + j] = 0;
            for (k = 0; k < A_n; k++)
            {
                out->base[i*out_n + j] += A->base[i * A_n + k] * B->base[k * B->n + j];
            }
        }
    }
    return out;
}

/* matrix transpose */
MAT *m_transp(const MAT *in, MAT *out)
{
    int i, j, m = out->m, n = out->n;
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            out->base[i * n + j] = in->base[j * m + i];
        }
    }
    return out;
}
