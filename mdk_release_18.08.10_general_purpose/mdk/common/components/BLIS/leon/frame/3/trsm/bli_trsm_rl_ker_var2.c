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
#include "Helpers.h"
#include "mv_types.h"
#if defined(__RTEMS__)
#include "OsDrvSvu.h"
#include "OsDrvShaveL2c.h"
#endif

#define FUNCPTR_T gemm_fp
#if defined(__RTEMS__)
static osDrvSvuHandler_t handler[16];
#endif

typedef void (*FUNCPTR_T)(
                           doff_t  diagoffb,
                           dim_t   m,
                           dim_t   n,
                           dim_t   k,
                           void*   alpha,
                           void*   a, inc_t rs_a, inc_t cs_a, inc_t ps_a,
                           void*   b, inc_t rs_b, inc_t cs_b, inc_t ps_b,
                           void*   c, inc_t rs_c, inc_t cs_c
                         );

static FUNCPTR_T GENARRAY(ftypes,trsm_rl_ker_var2);


void bli_trsm_rl_ker_var2( obj_t*  a,
                           obj_t*  b,
                           obj_t*  c,
                           trsm_t* cntl )
{
    UNUSED(cntl);

	num_t     dt_exec   = bli_obj_execution_datatype( *c );

	doff_t    diagoffb  = bli_obj_diag_offset( *b );

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

	void*     buf_alpha;

	FUNCPTR_T f;


	// Grab the address of the internal scalar buffer for the scalar
	// attached to B.
	buf_alpha = bli_obj_internal_scalar_buffer( *a );

	// Index into the type combination array to extract the correct
	// function pointer.
	f = ftypes[dt_exec];

	// Invoke the function.
	f( diagoffb,
	   m,
	   n,
	   k,
	   buf_alpha,
	   buf_a, rs_a, cs_a, ps_a,
	   buf_b, rs_b, cs_b, ps_b,
	   buf_c, rs_c, cs_c );
}

#ifdef SHAVE_BLIS
void bli_strsm_rl_ker_var2(
        doff_t  diagoffb,
        dim_t   m,
        dim_t   n,
        dim_t   k,
        void*   alpha,
        void*   a, inc_t rs_a, inc_t cs_a, inc_t ps_a,
        void*   b, inc_t rs_b, inc_t cs_b, inc_t ps_b,
        void*   c, inc_t rs_c, inc_t cs_c
)
{
    UNUSED(rs_a);
    UNUSED(rs_b);
    UNUSED(cs_b);
    UNUSED(ps_b);

    /* Alias constants to shorter names. */
    const dim_t     MR         = BLIS_DEFAULT_MR_S;
    const dim_t     NR         = BLIS_DEFAULT_NR_S;
    const dim_t     PACKMR     = BLIS_PACKDIM_MR_S;
    const dim_t     PACKNR     = BLIS_PACKDIM_NR_S;

    float* restrict a_cast     = a;

    dim_t           m_iter, m_left;
    dim_t           n_iter, n_left;
    dim_t           j;
    dim_t           rstep_a;
    dim_t           cstep_b;
    kernelInfo_t    kernelInfo;
    int             shaveIdx;
    int             shaveNr;
    int             stepIter, startIter, endIter, modIter;
#ifdef SHAVE_PERFORMANCE_ONLY
    double          countCycles;
#endif

    /*
    Assumptions/assertions:
    rs_a == 1
    cs_a == GEMM_NR
    ps_a == stride to next row panel of A
    rs_b == GEMM_MR
    cs_b == 1
    ps_b == stride to next column panel of B
    rs_c == (no assumptions)
    cs_c == (no assumptions)
    */

    /* If any dimension is zero, return immediately. */
    if ( ( (m) == 0 || (n) == 0 || (k) == 0 ) ) return;

    /* Safeguard: If the current panel of B is entirely above its diagonal,
    it is implicitly zero. So we do nothing. */
    if ( ( ( doff_t )k <= -diagoffb ) ) return;

    /* If there is a zero region above where the diagonal of B intersects
    the left edge of the panel, adjust the pointer to A and treat this
    case as if the diagonal offset were zero. Note that we don't need to
    adjust the pointer to B since packm would have simply skipped over
    the region that was not stored. */
    if ( diagoffb < 0 )
    {
        j        = -diagoffb;
        k        = k - j;
        diagoffb = 0;
        a_cast   = a_cast + (j  )*cs_a;
    }

    /* If there is a zero region to the right of where the diagonal
    of B intersects the bottom of the panel, shrink it so that
    we can index to the correct place in C (corresponding to the
    part of the panel of B that was packed).
    NOTE: This is NOT being done to skip over "no-op" iterations,
    as with the trsm_lu macro-kernel. This MUST be done for correct
    execution because we use n (via n_iter) to compute diagonal and
    index offsets for backwards movement through B. */
    if ( diagoffb + k < n )
    {
        n = diagoffb + k;
    }

    /* Check the k dimension, which needs to be a multiple of NR. If k
    isn't a multiple of NR, we adjust it higher to satisfy the micro-
    kernel, which is expecting to perform an NR x NR triangular solve.
    This adjustment of k is consistent with what happened when B was
    packed: all of its bottom/right edges were zero-padded, and
    furthermore, the panel that stores the bottom-right corner of the
    matrix has its diagonal extended into the zero-padded region (as
    identity). This allows the trsm of that bottom-right panel to
    proceed without producing any infs or NaNs that would infect the
    "good" values of the corresponding block of A. */
    if ( k % NR != 0 ) k += NR - ( k % NR );

    /* NOTE: We don't need to check that n is a multiple of PACKNR since we
    know that the underlying buffer was already allocated to have an n
    dimension that is a multiple of PACKNR, with the region between the
    last column and the next multiple of NR zero-padded accordingly. */

    /* Compute number of primary and leftover components of the m and n
    dimensions. */
    n_iter = n / NR;
    n_left = n % NR;

    m_iter = m / MR;
    m_left = m % MR;

    if ( n_left ) ++n_iter;
    if ( m_left ) ++m_iter;

    /* Determine some increments used to step through A, B, and C. */
    rstep_a = ps_a;
    cstep_b = k * PACKNR;

    /* Check limitations */
    if((rs_c != 1) && (cs_c != 1))
    {
        printf("Warning: (rs_c != 1) && (cs_c != 1)\n");
    }
    if(m_iter > MAX_M_ITER)
    {
        printf("Error: m_iter %lld is greater than MAX_M_ITER\n", (u64)m_iter);
        return;
    }
    if(k > MAX_K)
    {
        printf("Error: k %lld is greater than MAX_K\n", (u64)k);
        return;
    }
    if(rstep_a > k*PACKMR)
    {
        printf("Error: k*PACKMR %lld is bigger than rstep_a %ld\n", (u64)k*PACKMR, rstep_a);
        return;
    }

#ifdef SHAVE_PERFORMANCE_ONLY
    countCycles = bli_clock();
#endif
    stepIter  = m_iter/numberShaves;
    modIter   = m_iter%numberShaves;
    startIter = 0;
    endIter   = stepIter;
    kernelInfo.a       = (void*)a_cast;
    kernelInfo.b       = b;
    kernelInfo.c       = c;
    kernelInfo.alpha   = alpha;
    kernelInfo.beta    = 0;
    kernelInfo.k       = k;
    kernelInfo.n_iter  = n_iter;
    kernelInfo.m_iter  = m_iter;
    kernelInfo.n_left  = n_left;
    kernelInfo.m_left  = m_left;
    kernelInfo.rs_c    = rs_c;
    kernelInfo.cs_c    = cs_c;
    kernelInfo.rstep_a = rstep_a;
    kernelInfo.cstep_b = cstep_b;


#if defined(__RTEMS__)
rtems_cache_flush_entire_data();
int sc;
u32 running;
    for (shaveIdx = 0; shaveIdx < numberShaves; shaveIdx++)
    {
        shaveNr = listShaves[shaveIdx];
        sc = OsDrvSvuOpenShave(&handler[shaveIdx], listShaves[shaveIdx], 0);
        if (sc == OS_MYR_DRV_SUCCESS)
        {
            sc = OsDrvSvuResetShave(&handler[shaveIdx]);
            if(sc)
                exit(sc);
            sc = OsDrvSvuSetAbsoluteDefaultStack(&handler[shaveIdx]);
            if(sc)
                exit(sc);
            sc = OsDrvSvuStartShaveCC(&handler[shaveIdx], entryPointsSGEMMTRSM_RL[shaveNr], "iiii", (u32)&kernelInfo, diagoffb, startIter, endIter);
            if(sc)
                exit(sc);
        startIter = endIter;
        /* Check last SHAVE */
        if(shaveIdx == numberShaves - modIter - 1)
        {
            stepIter += 1;
        }
        endIter += stepIter;
        }
        else {
            printf("cannot open shave %d\n", shaveIdx);
            printf("error code %d\n", sc);
            exit(sc);
        }
       }
    sc = OsDrvSvuWaitShaves(numberShaves, handler, OS_DRV_SVU_WAIT_FOREVER, &running);
    if(sc)
    {
		exit(sc);
    }
    for (shaveIdx = 0; shaveIdx < numberShaves; shaveIdx++){
        sc = OsDrvSvuCloseShave(&handler[shaveIdx]);
            if(sc)
               {
                printf("cannot close shave %d\n", shaveIdx); 
                printf("error code %d\n", sc);
                exit(sc);
               } 
    }
    OsDrvShaveL2cFlushInvPart(1, OS_DRV_SHAVE_L2C_FLUSH_INV);
#else
    for (shaveIdx = 0; shaveIdx < numberShaves; shaveIdx++)
    {
        shaveNr = listShaves[shaveIdx];
        swcResetShave(shaveNr);
        swcSetAbsoluteDefaultStack(shaveNr);

        swcStartShaveCC(shaveNr, entryPointsSGEMMTRSM_RL[shaveNr], "iiii", (u32)&kernelInfo, diagoffb, startIter, endIter);
        startIter = endIter;
        /* Check last SHAVE */
        if(shaveIdx == numberShaves - modIter - 1)
        {
            stepIter += 1;
        }
        endIter += stepIter;
    }
    swcWaitShaves(numberShaves, listShaves);
   #ifdef MYRIAD2
    DrvShaveL2CachePartitionFlushAndInvalidate(0);
   #endif
#endif

#ifdef SHAVE_PERFORMANCE_ONLY
    time_shave += (bli_clock() - countCycles);
#endif
}

#else
#undef  GENTFUNC
#define GENTFUNC( ctype, ch, varname, gemmtrsmukr, gemmukr ) \
\
void PASTEMAC(ch,varname)( \
                           doff_t  diagoffb, \
                           dim_t   m, \
                           dim_t   n, \
                           dim_t   k, \
                           void*   alpha, \
                           void*   a, inc_t rs_a, inc_t cs_a, inc_t ps_a, \
                           void*   b, inc_t rs_b, inc_t cs_b, inc_t ps_b, \
                           void*   c, inc_t rs_c, inc_t cs_c \
                         ) \
{ \
	/* Temporary C buffer for edge cases. */ \
	ctype           ct[ PASTEMAC(ch,nr) * \
	                    PASTEMAC(ch,mr) ] \
	                    __attribute__((aligned(BLIS_STACK_BUF_ALIGN_SIZE))); \
	const inc_t     rs_ct      = 1; \
	const inc_t     cs_ct      = PASTEMAC(ch,nr); \
\
	/* Alias constants to shorter names. */ \
	const dim_t     MR         = PASTEMAC(ch,nr); \
	const dim_t     NR         = PASTEMAC(ch,mr); \
	const dim_t     PACKMR     = PASTEMAC(ch,packnr); \
	const dim_t     PACKNR     = PASTEMAC(ch,packmr); \
\
	ctype* restrict zero       = PASTEMAC(ch,0); \
	ctype* restrict minus_one  = PASTEMAC(ch,m1); \
	ctype* restrict a_cast     = a; \
	ctype*          b_cast     = b; \
	ctype*          c_cast     = c; \
	ctype* restrict alpha_cast = alpha; \
	ctype* restrict b1; \
	ctype* restrict c1; \
\
	doff_t          diagoffb_j; \
	dim_t           m_iter, m_left; \
	dim_t           n_iter, n_left; \
	dim_t           m_cur; \
	dim_t           n_cur; \
	dim_t           k_b1121; \
	dim_t           k_b11; \
	dim_t           k_b21; \
	dim_t           off_b11; \
	dim_t           off_b21; \
	dim_t           i, j, jb; \
	dim_t           rstep_a; \
	dim_t           cstep_b; \
	dim_t           rstep_c, cstep_c; \
	auxinfo_t       aux; \
\
	/*
	   Assumptions/assertions:
         rs_a == 1
	     cs_a == GEMM_NR
	     ps_a == stride to next row panel of A
         rs_b == GEMM_MR
	     cs_b == 1
	     ps_b == stride to next column panel of B
         rs_c == (no assumptions)
	     cs_c == (no assumptions)
	*/ \
\
	/* If any dimension is zero, return immediately. */ \
	if ( bli_zero_dim3( m, n, k ) ) return; \
\
	/* Safeguard: If the current panel of B is entirely above its diagonal,
	   it is implicitly zero. So we do nothing. */ \
	if ( bli_is_strictly_above_diag_n( diagoffb, k, n ) ) return; \
\
	/* If there is a zero region above where the diagonal of B intersects
	   the left edge of the panel, adjust the pointer to A and treat this
	   case as if the diagonal offset were zero. Note that we don't need to
	   adjust the pointer to B since packm would have simply skipped over
	   the region that was not stored. */ \
	if ( diagoffb < 0 ) \
	{ \
		j        = -diagoffb; \
		k        = k - j; \
		diagoffb = 0; \
		a_cast   = a_cast + (j  )*cs_a; \
	} \
\
	/* If there is a zero region to the right of where the diagonal
	   of B intersects the bottom of the panel, shrink it so that
	   we can index to the correct place in C (corresponding to the
	   part of the panel of B that was packed).
	   NOTE: This is NOT being done to skip over "no-op" iterations,
	   as with the trsm_lu macro-kernel. This MUST be done for correct
	   execution because we use n (via n_iter) to compute diagonal and
	   index offsets for backwards movement through B. */ \
	if ( diagoffb + k < n ) \
	{ \
		n = diagoffb + k; \
	} \
\
	/* Check the k dimension, which needs to be a multiple of NR. If k
	   isn't a multiple of NR, we adjust it higher to satisfy the micro-
	   kernel, which is expecting to perform an NR x NR triangular solve.
	   This adjustment of k is consistent with what happened when B was
	   packed: all of its bottom/right edges were zero-padded, and
	   furthermore, the panel that stores the bottom-right corner of the
	   matrix has its diagonal extended into the zero-padded region (as
	   identity). This allows the trsm of that bottom-right panel to
	   proceed without producing any infs or NaNs that would infect the
	   "good" values of the corresponding block of A. */ \
	if ( k % NR != 0 ) k += NR - ( k % NR ); \
\
	/* NOTE: We don't need to check that n is a multiple of PACKNR since we
	   know that the underlying buffer was already allocated to have an n
	   dimension that is a multiple of PACKNR, with the region between the
	   last column and the next multiple of NR zero-padded accordingly. */ \
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
	rstep_a = ps_a; \
\
	cstep_b = k * PACKNR; \
\
	rstep_c = rs_c * MR; \
	cstep_c = cs_c * NR; \
\
	/* Save the panel stride of A to the auxinfo_t object.
	   NOTE: We swap the values for A and B since the triangular
	   "A" matrix is actually contained within B. */ \
	bli_auxinfo_set_ps_b( ps_a, aux ); \
\
	b1 = b_cast; \
	c1 = c_cast; \
\
	/* Loop over the n dimension (NR columns at a time). */ \
	for ( jb = 0; jb < n_iter; ++jb ) \
	{ \
		ctype* restrict a1; \
		ctype* restrict c11; \
		ctype* restrict b11; \
		ctype* restrict b21; \
		ctype* restrict b2; \
\
		j          = n_iter - 1 - jb; \
		diagoffb_j = diagoffb - ( doff_t )j*NR; \
		a1         = a_cast; \
		c11        = c1 + (n_iter-1)*cstep_c; \
\
		n_cur = ( bli_is_not_edge_b( jb, n_iter, n_left ) ? NR : n_left ); \
\
		/* Compute various offsets into and lengths of parts of B. */ \
		off_b11   = bli_max( -diagoffb_j, 0 ); \
		k_b1121   = k - off_b11; \
		k_b11     = NR; \
		k_b21     = k_b1121 - NR; \
		off_b21   = off_b11 + k_b11; \
\
		/* Compute the addresses of the triangular block B11 and the
		   panel B21. */ \
		b11       = b1; \
		b21       = b1 + k_b11 * PACKNR; \
\
		/* Initialize our next panel of B to be the current panel of B. */ \
		b2 = b1; \
\
		/* Save the panel stride of B to the auxinfo_t object.
		   NOTE: We swap the values for A and B since the triangular
		   "A" matrix is actually contained within B. */ \
		bli_auxinfo_set_ps_a( k_b1121 * PACKNR, aux ); \
\
		/* If the current panel of B intersects the diagonal, use a
		   special micro-kernel that performs a fused gemm and trsm.
		   If the current panel of B resides below the diagonal, use a
		   a regular gemm micro-kernel. Otherwise, if it is above the
		   diagonal, it was not packed (because it is implicitly zero)
		   and so we do nothing. */ \
		if ( bli_intersects_diag_n( diagoffb_j, k, NR ) ) \
		{ \
			/* Loop over the m dimension (MR rows at a time). */ \
			for ( i = 0; i < m_iter; ++i ) \
			{ \
				ctype* restrict a11; \
				ctype* restrict a12; \
				ctype* restrict a2; \
\
				m_cur = ( bli_is_not_edge_f( i, m_iter, m_left ) ? MR : m_left ); \
\
				/* Compute the addresses of the A11 block and A12 panel. */ \
				a11  = a1 + off_b11 * PACKMR; \
				a12  = a1 + off_b21 * PACKMR; \
\
				/* Compute the addresses of the next panels of A and B. */ \
                a2 = a1 + rstep_a; \
				if ( bli_is_last_iter( i, m_iter ) ) \
				{ \
					a2 = a_cast; \
					b2 = b1 + k_b1121 * PACKNR; \
					if ( bli_is_last_iter( jb, n_iter ) ) \
						b2 = b_cast; \
				} \
\
				/* Save addresses of next panels of A and B to the auxinfo_t
				   object. NOTE: We swap the values for A and B since the
				   triangular "A" matrix is actually contained within B. */ \
				bli_auxinfo_set_next_a( b2, aux ); \
				bli_auxinfo_set_next_b( a2, aux ); \
\
				/* Handle interior and edge cases separately. */ \
				if ( m_cur == MR && n_cur == NR ) \
				{ \
					/* Invoke the fused gemm/trsm micro-kernel. */ \
					PASTEMAC(ch,gemmtrsmukr)( k_b21, \
					                          alpha_cast, \
					                          b21, \
					                          b11, \
					                          a12, \
					                          a11, \
					                          c11, cs_c, rs_c, \
					                          &aux ); \
				} \
				else \
				{ \
					/* Invoke the fused gemm/trsm micro-kernel. */ \
					PASTEMAC(ch,gemmtrsmukr)( k_b21, \
					                          alpha_cast, \
					                          b21, \
					                          b11, \
					                          a12, \
					                          a11, \
					                          ct, cs_ct, rs_ct, \
					                          &aux ); \
\
					/* Copy the result to the bottom edge of C. */ \
					PASTEMAC(ch,copys_mxn)( m_cur, n_cur, \
					                        ct,  rs_ct, cs_ct, \
					                        c11, rs_c,  cs_c ); \
				} \
\
				a1  += rstep_a; \
				c11 += rstep_c; \
			} \
		} \
		else if ( bli_is_strictly_below_diag_n( diagoffb_j, k, NR ) ) \
		{ \
			/* Loop over the m dimension (MR rows at a time). */ \
			for ( i = 0; i < m_iter; ++i ) \
			{ \
				ctype* restrict a2; \
\
				m_cur = ( bli_is_not_edge_f( i, m_iter, m_left ) ? MR : m_left ); \
\
				/* Compute the addresses of the next panels of A and B. */ \
                a2 = a1 + rstep_a; \
				if ( bli_is_last_iter( i, m_iter ) ) \
				{ \
					a2 = a_cast; \
					b2 = b1 + cstep_b; \
					if ( bli_is_last_iter( jb, n_iter ) ) \
						b2 = b_cast; \
				} \
\
				/* Save addresses of next panels of A and B to the auxinfo_t
				   object. NOTE: We swap the values for A and B since the
				   triangular "A" matrix is actually contained within B. */ \
				bli_auxinfo_set_next_a( b2, aux ); \
				bli_auxinfo_set_next_b( a2, aux ); \
\
				/* Handle interior and edge cases separately. */ \
				if ( m_cur == MR && n_cur == NR ) \
				{ \
					/* Invoke the gemm micro-kernel. */ \
					PASTEMAC(ch,gemmukr)( k, \
					                      minus_one, \
					                      b1, \
					                      a1, \
					                      alpha_cast, \
					                      c11, cs_c, rs_c, \
					                      &aux ); \
				} \
				else \
				{ \
					/* Invoke the gemm micro-kernel. */ \
					PASTEMAC(ch,gemmukr)( k, \
					                      minus_one, \
					                      b1, \
					                      a1, \
					                      zero, \
					                      ct, cs_ct, rs_ct, \
					                      &aux ); \
\
					/* Add the result to the edge of C. */ \
					PASTEMAC(ch,xpbys_mxn)( m_cur, n_cur, \
					                        ct,  rs_ct, cs_ct, \
					                        alpha_cast, \
					                        c11, rs_c,  cs_c ); \
				} \
\
				a1  += rstep_a; \
				c11 += rstep_c; \
			} \
		} \
\
		b1 += k_b1121 * PACKNR; \
		c1 -= cstep_c; \
	} \
}

INSERT_GENTFUNC_BASIC2( trsm_rl_ker_var2, GEMMTRSM_U_UKERNEL, GEMM_UKERNEL )
#endif
