/*

   BLIS    
   An object-based framework for developing high-performance BLAS-like
   libraries.

   Copyright (C) 2014, The University of Texas

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    - Neither the name of The University of Texas nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
   HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "blis.h"
#include "mv_types.h"

#define FUNCPTR_T gemm_fp

typedef void (*FUNCPTR_T)(
                           doff_t  diagoffa,
                           dim_t   m,
                           dim_t   n,
                           dim_t   k,
                           void*   alpha,
                           void*   a, inc_t rs_a, inc_t cs_a, inc_t ps_a,
                           void*   b, inc_t rs_b, inc_t cs_b, inc_t ps_b,
                           void*   beta,
                           void*   c, inc_t rs_c, inc_t cs_c
                         );

static FUNCPTR_T GENARRAY(ftypes,trmm_lu_ker_var2);


void bli_trmm_lu_ker_var2( obj_t*  a,
                           obj_t*  b,
                           obj_t*  c,
                           trmm_t* cntl )
{
    UNUSED(cntl);

	num_t     dt_exec   = bli_obj_execution_datatype( *c );

	doff_t    diagoffa  = bli_obj_diag_offset( *a );

	dim_t     m         = bli_obj_length( *c );
	dim_t     n         = bli_obj_width( *c );
	dim_t     k         = bli_obj_width( *a );

	void*     buf_a     = bli_obj_buffer_at_off( *a );
	inc_t     rs_a      = bli_obj_row_stride( *a );
	inc_t     cs_a      = bli_obj_col_stride( *a );
	inc_t     ps_a      = bli_obj_panel_stride( *a );

	void*     buf_b     = bli_obj_buffer_at_off( *b );
	inc_t     rs_b      = bli_obj_row_stride( *b );
	inc_t     cs_b      = bli_obj_col_stride( *b );
	inc_t     ps_b      = bli_obj_panel_stride( *b );

	void*     buf_c     = bli_obj_buffer_at_off( *c );
	inc_t     rs_c      = bli_obj_row_stride( *c );
	inc_t     cs_c      = bli_obj_col_stride( *c );

	obj_t     scalar_a;
	obj_t     scalar_b;

	void*     buf_alpha;
	void*     buf_beta;

	FUNCPTR_T f;


	// Detach and multiply the scalars attached to A and B.
	bli_obj_scalar_detach( a, &scalar_a );
	bli_obj_scalar_detach( b, &scalar_b );
	bli_mulsc( &scalar_a, &scalar_b );

	// Grab the addresses of the internal scalar buffers for the scalar
	// merged above and the scalar attached to C.
	buf_alpha = bli_obj_internal_scalar_buffer( scalar_b );
	buf_beta  = bli_obj_internal_scalar_buffer( *c );

	// Index into the type combination array to extract the correct
	// function pointer.
	f = ftypes[dt_exec];

	// Invoke the function.
	f( diagoffa,
	   m,
	   n,
	   k,
	   buf_alpha,
	   buf_a, rs_a, cs_a, ps_a,
	   buf_b, rs_b, cs_b, ps_b,
	   buf_beta,
	   buf_c, rs_c, cs_c );
}


#undef  GENTFUNC
#define GENTFUNC( ctype, ch, varname, ukrname ) \
\
void PASTEMAC(ch,varname)( \
                           doff_t  diagoffa, \
                           dim_t   m, \
                           dim_t   n, \
                           dim_t   k, \
                           void*   alpha, \
                           void*   a, inc_t rs_a, inc_t cs_a, inc_t ps_a, \
                           void*   b, inc_t rs_b, inc_t cs_b, inc_t ps_b, \
                           void*   beta, \
                           void*   c, inc_t rs_c, inc_t cs_c \
                         ) \
{ \
    UNUSED(rs_a); \
    UNUSED(cs_a); \
    UNUSED(ps_a); \
    UNUSED(cs_b); \
	/* Temporary C buffer for edge cases. */ \
	ctype           ct[ PASTEMAC(ch,mr) * \
	                    PASTEMAC(ch,nr) ] \
	                    __attribute__((aligned(BLIS_STACK_BUF_ALIGN_SIZE))); \
	const inc_t     rs_ct      = 1; \
	const inc_t     cs_ct      = PASTEMAC(ch,mr); \
\
	/* Alias some constants to shorter names. */ \
	const dim_t     MR         = PASTEMAC(ch,mr); \
	const dim_t     NR         = PASTEMAC(ch,nr); \
	const dim_t     PACKMR     = PASTEMAC(ch,packmr); \
\
	ctype* restrict one        = PASTEMAC(ch,1); \
	ctype* restrict zero       = PASTEMAC(ch,0); \
	ctype* restrict a_cast     = a; \
	ctype* restrict b_cast     = b; \
	ctype* restrict c_cast     = c; \
	ctype* restrict alpha_cast = alpha; \
	ctype* restrict beta_cast  = beta; \
	ctype* restrict b1; \
	ctype* restrict c1; \
\
	doff_t          diagoffa_i; \
	dim_t           m_iter, m_left; \
	dim_t           n_iter, n_left; \
	dim_t           m_cur; \
	dim_t           n_cur; \
	dim_t           k_a1112; \
	dim_t           off_a1112; \
	dim_t           i, j; \
	inc_t           rstep_a; \
	inc_t           cstep_b; \
	inc_t           rstep_c, cstep_c; \
	auxinfo_t       aux; \
\
	/*
	   Assumptions/assertions:
         rs_a == 1
	     cs_a == GEMM_MR
	     ps_a == stride to next row panel of A
         rs_b == GEMM_NR
	     cs_b == 1
	     ps_b == stride to next column panel of B
         rs_c == (no assumptions)
	     cs_c == (no assumptions)
	*/ \
\
	/* If any dimension is zero, return immediately. */ \
	if ( bli_zero_dim3( m, n, k ) ) return; \
\
	/* Safeguard: If the current block of A is entirely below the diagonal,
	   it is implicitly zero. So we do nothing. */ \
	if ( bli_is_strictly_below_diag_n( diagoffa, m, k ) ) return; \
\
	/* If there is a zero region to the left of where the diagonal of A
	   intersects the top edge of the block, adjust the pointer to B and
	   treat this case as if the diagonal offset were zero. Note that we
	   don't need to adjust the pointer to A since packm would have simply
	   skipped over the region that was not stored. */ \
	if ( diagoffa > 0 ) \
	{ \
		i        = diagoffa; \
		k        = k - i; \
		diagoffa = 0; \
		b_cast   = b_cast + (i  )*rs_b; \
	} \
\
	/* If there is a zero region below where the diagonal of A intersects the
	   right side of the block, shrink it to prevent "no-op" iterations from
	   executing. */ \
	if ( -diagoffa + k < m ) \
	{ \
		m = -diagoffa + k; \
	} \
\
	/* For consistency with the trsm macro-kernels, we inflate k to be a
	   multiple of MR, if necessary. This is needed because we typically
	   use the same packm variant for trmm as for trsm, and trsm has this
	   constraint that k must be a multiple of MR so that it can safely
	   handle bottom-right corner edges of the triangle. */ \
	if ( k % MR != 0 ) k += MR - ( k % MR ); \
\
	/* Clear the temporary C buffer in case it has any infs or NaNs. */ \
	PASTEMAC(ch,set0s_mxn)( MR, NR, \
	                        ct, rs_ct, cs_ct ); \
\
	/* Compute number of primary and leftover components of the m and n
	   dimensions. */ \
	n_iter = n / NR; \
	n_left = n % NR; \
\
	m_iter = m / MR; \
	m_left = m % MR; \
\
	if ( n_left ) ++n_iter; \
	if ( m_left ) ++m_iter; \
\
	/* Determine some increments used to step through A, B, and C. */ \
	rstep_a = k * PACKMR; \
\
	cstep_b = ps_b; \
\
	rstep_c = rs_c * MR; \
	cstep_c = cs_c * NR; \
\
	/* Save the panel stride of B to the auxinfo_t object. */ \
	bli_auxinfo_set_ps_b( ps_b, aux ); \
\
	b1 = b_cast; \
	c1 = c_cast; \
\
	/* Loop over the n dimension (NR columns at a time). */ \
	for ( j = 0; j < n_iter; ++j ) \
	{ \
		ctype* restrict a1; \
		ctype* restrict c11; \
		ctype* restrict b2; \
\
		a1  = a_cast; \
		c11 = c1; \
\
		n_cur = ( bli_is_not_edge_f( j, n_iter, n_left ) ? NR : n_left ); \
\
		/* Initialize our next panel of B to be the current panel of B. */ \
		b2 = b1; \
\
		/* Loop over the m dimension (MR rows at a time). */ \
		for ( i = 0; i < m_iter; ++i ) \
		{ \
			diagoffa_i = diagoffa + ( doff_t )i*MR; \
\
			m_cur = ( bli_is_not_edge_f( i, m_iter, m_left ) ? MR : m_left ); \
\
			/* If the current panel of A intersects the diagonal, scale C
			   by beta. If it is strictly above the diagonal, scale by one.
			   This allows the current macro-kernel to work for both trmm
			   and trmm3. */ \
			if ( bli_intersects_diag_n( diagoffa_i, MR, k ) ) \
			{ \
				ctype* restrict b1_i; \
				ctype* restrict a2; \
\
				/* Determine the offset to and length of the panel that was
				   packed so we can index into the corresponding location in
				   b1. */ \
				off_a1112 = bli_max( diagoffa_i, 0 ); \
				k_a1112   = k - off_a1112; \
\
				b1_i = b1 + off_a1112 * NR; \
\
				/* Compute the addresses of the next panels of A and B. */ \
				a2 = a1 + k_a1112 * PACKMR; \
				if ( bli_is_last_iter( i, m_iter ) ) \
				{ \
					a2 = a_cast; \
					b2 = b1 + cstep_b; \
					if ( bli_is_last_iter( j, n_iter ) ) \
						b2 = b_cast; \
				} \
\
				/* Save addresses of next panels of A and B to the auxinfo_t
				   object. */ \
				bli_auxinfo_set_next_a( a2, aux ); \
				bli_auxinfo_set_next_b( b2, aux ); \
\
				/* Save the panel stride of the current panel of A to the
				   auxinfo_t object. */ \
				bli_auxinfo_set_ps_a( k_a1112 * PACKMR, aux ); \
\
				/* Handle interior and edge cases separately. */ \
				if ( m_cur == MR && n_cur == NR ) \
				{ \
					/* Invoke the gemm micro-kernel. */ \
					PASTEMAC(ch,ukrname)( k_a1112, \
					                      alpha_cast, \
					                      a1, \
					                      b1_i, \
					                      beta_cast, \
					                      c11, rs_c, cs_c, \
					                      &aux ); \
				} \
				else \
				{ \
					/* Copy edge elements of C to the temporary buffer. */ \
					PASTEMAC(ch,copys_mxn)( m_cur, n_cur, \
					                        c11, rs_c,  cs_c, \
					                        ct,  rs_ct, cs_ct ); \
\
					/* Invoke the gemm micro-kernel. */ \
					PASTEMAC(ch,ukrname)( k_a1112, \
					                      alpha_cast, \
					                      a1, \
					                      b1_i, \
					                      beta_cast, \
					                      ct, rs_ct, cs_ct, \
					                      &aux ); \
\
					/* Copy the result to the edge of C. */ \
					PASTEMAC(ch,copys_mxn)( m_cur, n_cur, \
					                        ct,  rs_ct, cs_ct, \
					                        c11, rs_c,  cs_c ); \
				} \
\
				a1 += k_a1112 * PACKMR; \
			} \
			else if ( bli_is_strictly_above_diag_n( diagoffa_i, MR, k ) ) \
			{ \
				ctype* restrict a2; \
\
				/* Compute the addresses of the next panels of A and B. */ \
				a2 = a1 + rstep_a; \
				if ( bli_is_last_iter( i, m_iter ) ) \
				{ \
					a2 = a_cast; \
					b2 = b1 + cstep_b; \
					if ( bli_is_last_iter( j, n_iter ) ) \
						b2 = b_cast; \
				} \
\
				/* Save addresses of next panels of A and B to the auxinfo_t
				   object. */ \
				bli_auxinfo_set_next_a( a2, aux ); \
				bli_auxinfo_set_next_b( b2, aux ); \
\
				/* Save the panel stride of the current panel of A to the
				   auxinfo_t object. */ \
				bli_auxinfo_set_ps_a( rstep_a, aux ); \
\
				/* Handle interior and edge cases separately. */ \
				if ( m_cur == MR && n_cur == NR ) \
				{ \
					/* Invoke the gemm micro-kernel. */ \
					PASTEMAC(ch,ukrname)( k, \
					                      alpha_cast, \
					                      a1, \
					                      b1, \
					                      one, \
					                      c11, rs_c, cs_c, \
					                      &aux ); \
				} \
				else \
				{ \
					/* Invoke the gemm micro-kernel. */ \
					PASTEMAC(ch,ukrname)( k, \
					                      alpha_cast, \
					                      a1, \
					                      b1, \
					                      zero, \
					                      ct, rs_ct, cs_ct, \
					                      &aux ); \
\
					/* Add the result to the edge of C. */ \
					PASTEMAC(ch,adds_mxn)( m_cur, n_cur, \
					                       ct,  rs_ct, cs_ct, \
					                       c11, rs_c,  cs_c ); \
				} \
\
				a1 += rstep_a; \
			} \
\
			c11 += rstep_c; \
		} \
\
		b1 += cstep_b; \
		c1 += cstep_c; \
	} \
\
/*PASTEMAC(ch,fprintm)( stdout, "trmm_lu_ker_var2: a1", MR, k_a1112, a1, 1, MR, "%4.1f", "" );*/ \
/*PASTEMAC(ch,fprintm)( stdout, "trmm_lu_ker_var2: b1", k_a1112, NR, b1_i, NR, 1, "%4.1f", "" );*/ \
}

INSERT_GENTFUNC_BASIC( trmm_lu_ker_var2, GEMM_UKERNEL )

