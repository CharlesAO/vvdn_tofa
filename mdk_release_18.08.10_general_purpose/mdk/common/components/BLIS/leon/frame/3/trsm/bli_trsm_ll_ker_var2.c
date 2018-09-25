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
                           doff_t  diagoffa,
                           dim_t   m,
                           dim_t   n,
                           dim_t   k,
                           void*   alpha,
                           void*   a, inc_t rs_a, inc_t cs_a, inc_t ps_a,
                           void*   b, inc_t rs_b, inc_t cs_b, inc_t ps_b,
                           void*   c, inc_t rs_c, inc_t cs_c
                         );

static FUNCPTR_T GENARRAY(ftypes,trsm_ll_ker_var2);


void bli_trsm_ll_ker_var2( obj_t*  a,
                           obj_t*  b,
                           obj_t*  c,
                           trsm_t* cntl )
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

	void*     buf_alpha;

	FUNCPTR_T f;


	// Grab the address of the internal scalar buffer for the scalar
	// attached to B.
	buf_alpha = bli_obj_internal_scalar_buffer( *b );

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
	   buf_c, rs_c, cs_c );
}

#ifdef SHAVE_BLIS
void bli_strsm_ll_ker_var2(
        doff_t  diagoffa,
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
    UNUSED(cs_a);
    UNUSED(rs_b);
    UNUSED(cs_b);
    UNUSED(ps_a);

    /* Alias constants to shorter names. */
    const dim_t  MR     = BLIS_DEFAULT_MR_S;
    const dim_t  NR     = BLIS_DEFAULT_NR_S;
    const dim_t  PACKMR = BLIS_PACKDIM_MR_S;

    float        *c_cast = c;
    dim_t        m_iter, m_left;
    dim_t        n_iter, n_left;
    dim_t        i;
    dim_t        rstep_a;
    dim_t        cstep_b;
    int          shaveIdx;
    int          shaveNr;
    int          stepIter, startIter, endIter, modIter;
    kernelInfo_t kernelInfo;
#ifdef SHAVE_PERFORMANCE_ONLY
    double       countCycles;
#endif

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
    */

    /* If any dimension is zero, return immediately. */
    if ( ( (m) == 0 || (n) == 0 || (k) == 0 ) ) return;

    /* Safeguard: If matrix A is above the diagonal, it is implicitly zero.
    So we do nothing. */
    if ( ( ( doff_t )m <= -diagoffa ) ) return;

    /* If there is a zero region above where the diagonal of A intersects the
    left edge of the block, adjust the pointer to C and treat this case as
    if the diagonal offset were zero. This skips over the region (in
    increments of MR) that was not packed. (Note we skip in increments of
    MR since that is how the region would have been skipped by packm.) */
    if ( diagoffa < 0 )
    {
        i        = ( -diagoffa / MR ) * MR;
        m        = m - i;
        diagoffa = -diagoffa % MR;
        c_cast   = c_cast + (i  )*rs_c;
    }

    /* Check the k dimension, which needs to be a multiple of MR. If k
    isn't a multiple of MR, we adjust it higher to satisfy the micro-
    kernel, which is expecting to perform an MR x MR triangular solve.
    This adjustment of k is consistent with what happened when A was
    packed: all of its bottom/right edges were zero-padded, and
    furthermore, the panel that stores the bottom-right corner of the
    matrix has its diagonal extended into the zero-padded region (as
    identity). This allows the trsm of that bottom-right panel to
    proceed without producing any infs or NaNs that would infect the
    "good" values of the corresponding block of B. */
    if( k % MR != 0 ) k += MR - ( k % MR );

    /* NOTE: We don't need to check that m is a multiple of PACKMR since we
    know that the underlying buffer was already allocated to have an m
    dimension that is a multiple of PACKMR, with the region between the
    last row and the next multiple of MR zero-padded accordingly. */

    /* Compute number of primary and leftover components of the m and n
    dimensions. */
    n_iter = n / NR;
    n_left = n % NR;

    m_iter = m / MR;
    m_left = m % MR;

    if ( n_left ) ++n_iter;
    if ( m_left ) ++m_iter;

    /* Determine some increments used to step through A, B, and C. */
    rstep_a = k * PACKMR;
    cstep_b = ps_b;

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
    stepIter  = n_iter/numberShaves;
    modIter   = n_iter%numberShaves;
    kernelInfo.a       = a;
    kernelInfo.b       = b;
    kernelInfo.c       = (void*)c_cast;
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
    startIter = 0;
    endIter = stepIter;

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
            sc = OsDrvSvuStartShaveCC(&handler[shaveIdx], entryPointsSGEMMTRSM_LL[shaveNr], "iiii", (u32)&kernelInfo, diagoffa, startIter, endIter);
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

        swcStartShaveCC(shaveNr, entryPointsSGEMMTRSM_LL[shaveNr], "iiii", (u32)&kernelInfo, diagoffa, startIter, endIter);
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
                           doff_t  diagoffa, \
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
	ctype           ct[ PASTEMAC(ch,mr) * \
	                    PASTEMAC(ch,nr) ] \
	                    __attribute__((aligned(BLIS_STACK_BUF_ALIGN_SIZE))); \
	const inc_t     rs_ct      = 1; \
	const inc_t     cs_ct      = PASTEMAC(ch,mr); \
\
	/* Alias constants to shorter names. */ \
	const dim_t     MR         = PASTEMAC(ch,mr); \
	const dim_t     NR         = PASTEMAC(ch,nr); \
	const dim_t     PACKMR     = PASTEMAC(ch,packmr); \
	const dim_t     PACKNR     = PASTEMAC(ch,packnr); \
\
	ctype* restrict zero       = PASTEMAC(ch,0); \
	ctype* restrict minus_one  = PASTEMAC(ch,m1); \
	ctype* restrict a_cast     = a; \
	ctype* restrict b_cast     = b; \
	ctype* restrict c_cast     = c; \
	ctype* restrict alpha_cast = alpha; \
	ctype* restrict b1; \
	ctype* restrict c1; \
\
	doff_t          diagoffa_i; \
	dim_t           m_iter, m_left; \
	dim_t           n_iter, n_left; \
	dim_t           m_cur; \
	dim_t           n_cur; \
	dim_t           k_a1011; \
	dim_t           k_a10; \
	dim_t           off_a10; \
	dim_t           off_a11; \
	dim_t           i, j; \
	dim_t           rstep_a; \
	dim_t           cstep_b; \
	dim_t           rstep_c, cstep_c; \
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
	/* Safeguard: If matrix A is above the diagonal, it is implicitly zero.
	   So we do nothing. */ \
	if ( bli_is_strictly_above_diag_n( diagoffa, m, k ) ) return; \
\
	/* If there is a zero region above where the diagonal of A intersects the
	   left edge of the block, adjust the pointer to C and treat this case as
	   if the diagonal offset were zero. This skips over the region (in
	   increments of MR) that was not packed. (Note we skip in increments of
	   MR since that is how the region would have been skipped by packm.) */ \
	if ( diagoffa < 0 ) \
	{ \
		i        = ( -diagoffa / MR ) * MR; \
		m        = m - i; \
		diagoffa = -diagoffa % MR; \
		c_cast   = c_cast + (i  )*rs_c; \
	} \
\
	/* Check the k dimension, which needs to be a multiple of MR. If k
	   isn't a multiple of MR, we adjust it higher to satisfy the micro-
	   kernel, which is expecting to perform an MR x MR triangular solve.
	   This adjustment of k is consistent with what happened when A was
	   packed: all of its bottom/right edges were zero-padded, and
	   furthermore, the panel that stores the bottom-right corner of the
	   matrix has its diagonal extended into the zero-padded region (as
	   identity). This allows the trsm of that bottom-right panel to
	   proceed without producing any infs or NaNs that would infect the
	   "good" values of the corresponding block of B. */ \
	if ( k % MR != 0 ) k += MR - ( k % MR ); \
\
	/* NOTE: We don't need to check that m is a multiple of PACKMR since we
	   know that the underlying buffer was already allocated to have an m
	   dimension that is a multiple of PACKMR, with the region between the
	   last row and the next multiple of MR zero-padded accordingly. */ \
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
		c11 = c1 + (0  )*rstep_c; \
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
			/* If the current panel of A intersects the diagonal, use a
			   special micro-kernel that performs a fused gemm and trsm.
			   If the current panel of A resides below the diagonal, use a
			   a regular gemm micro-kernel. Otherwise, if it is above the
			   diagonal, it was not packed (because it is implicitly zero)
			   and so we do nothing. */ \
			if ( bli_intersects_diag_n( diagoffa_i, MR, k ) ) \
			{ \
				ctype* restrict a10; \
				ctype* restrict a11; \
				ctype* restrict b01; \
				ctype* restrict b11; \
				ctype* restrict a2; \
\
				/* Compute various offsets into and lengths of parts of A. */ \
				off_a10 = 0; \
				k_a1011 = bli_min( k, diagoffa_i + MR ); \
				k_a10   = k_a1011 - MR; \
				off_a11 = k_a10; \
\
				/* Compute the addresses of the panel A10 and the triangular
				   block A11. */ \
				a10 = a1; \
				a11 = a1 + k_a10 * PACKMR; \
\
				/* Compute the addresses of the panel B01 and the block
				   B11. */ \
				b01 = b1 + off_a10 * PACKNR; \
				b11 = b1 + off_a11 * PACKNR; \
\
				/* Compute the addresses of the next panels of A and B. */ \
				a2 = a1 + k_a1011 * PACKMR; \
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
				bli_auxinfo_set_ps_a( k_a1011 * PACKMR, aux ); \
\
				/* Handle interior and edge cases separately. */ \
				if ( m_cur == MR && n_cur == NR ) \
				{ \
					/* Invoke the fused gemm/trsm micro-kernel. */ \
					PASTEMAC(ch,gemmtrsmukr)( k_a10, \
					                          alpha_cast, \
					                          a10, \
					                          a11, \
					                          b01, \
					                          b11, \
					                          c11, rs_c, cs_c, \
					                          &aux ); \
				} \
				else \
				{ \
					/* Invoke the fused gemm/trsm micro-kernel. */ \
					PASTEMAC(ch,gemmtrsmukr)( k_a10, \
					                          alpha_cast, \
					                          a10, \
					                          a11, \
					                          b01, \
					                          b11, \
					                          ct, rs_ct, cs_ct, \
					                          &aux ); \
\
					/* Copy the result to the bottom edge of C. */ \
					PASTEMAC(ch,copys_mxn)( m_cur, n_cur, \
					                        ct,  rs_ct, cs_ct, \
					                        c11, rs_c,  cs_c ); \
				} \
\
				a1 += k_a1011 * PACKMR; \
			} \
			else if ( bli_is_strictly_below_diag_n( diagoffa_i, MR, k ) ) \
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
					PASTEMAC(ch,gemmukr)( k, \
					                      minus_one, \
					                      a1, \
					                      b1, \
					                      alpha_cast, \
					                      c11, rs_c, cs_c, \
					                      &aux ); \
				} \
				else \
				{ \
					/* Invoke the gemm micro-kernel. */ \
					PASTEMAC(ch,gemmukr)( k, \
					                      minus_one, \
					                      a1, \
					                      b1, \
					                      zero, \
					                      ct, rs_ct, cs_ct, \
					                      &aux ); \
\
					/* Add the result to the edge of C. */ \
					PASTEMAC(ch,xpbys_mxn)( m_cur, n_cur, \
					                        ct,  rs_ct, cs_ct, \
					                        alpha_cast, \
					                        c11, rs_c,  cs_c ); \
				} \
\
				a1 += rstep_a; \
			} \
\
			c11  += rstep_c; \
		} \
\
		b1 += cstep_b; \
		c1 += cstep_c; \
	} \
\
/*
PASTEMAC(ch,fprintm)( stdout, "trsm_ll_ker_var2: a1 (diag)", MR, k_a1011, a1, 1, MR, "%5.2f", "" ); \
PASTEMAC(ch,fprintm)( stdout, "trsm_ll_ker_var2: a11 (diag)", MR, MR, a11, 1, MR, "%5.2f", "" ); \
PASTEMAC(ch,fprintm)( stdout, "trsm_ll_ker_var2: b1 (diag)", k_a1011, NR, bp_i, NR, 1, "%5.2f", "" );  \
PASTEMAC(ch,fprintm)( stdout, "trsm_ll_ker_var2: bp11 (diag)", MR, NR, bp11, NR, 1, "%5.2f", "" );  \
*/ \
\
/*
PASTEMAC(ch,fprintm)( stdout, "trsm_ll_ker_var2: a1 (ndiag)", MR, k, a1, 1, MR, "%5.2f", "" ); \
PASTEMAC(ch,fprintm)( stdout, "trsm_ll_ker_var2: b1 (ndiag)", k, NR, bp, NR, 1, "%5.2f", "" ); \
*/ \
}

INSERT_GENTFUNC_BASIC2( trsm_ll_ker_var2, GEMMTRSM_L_UKERNEL, GEMM_UKERNEL )
#endif
