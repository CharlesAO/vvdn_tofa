///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <mv_types.h>
#include <svuCommonShave.h>
#include <moviVectorUtils.h>

#include "Defines.h"
#include "Buffers.h"
#include "Portab.h"

// 1: Includes
// ----------------------------------------------------------------------------

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
//#define A_BUFF_OPT

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------
#ifdef MYRIAD2
void startSGEMMTRSM_RU(kernelInfo_t *kernelInfo, int diagoffb, int startIter, int endIter)
{
    UNUSED(kernelInfo);
    UNUSED(diagoffb);
    UNUSED(startIter);
    UNUSED(endIter);

    printf("SGEMMTRSM_RU is not used!!!\n");

    exit(0);
    return;
}

#else

void mvcvsgemm4x4_asm(int k, float* alpha, float* a, float* b, float* beta, float* c, int rs_c, int cs_c);
void mvcvsgemmtrsm_l4x4_asm(int k, float *alpha, float *a10, float *a11, float *bd01, float *b11, float *c11, int rs_c, int cs_c);

void startSGEMMTRSM_RU(kernelInfo_t *kernelInfo, int diagoffb, int startIter, int endIter)
{
    float *a_cast = (float*)kernelInfo->a;
    float *b_cast = (float*)kernelInfo->b;
    float *c_cast = (float*)kernelInfo->c;
    float alphaValue = *(float*)kernelInfo->alpha;
    int   k      = kernelInfo->k;
    int   k_b0111;
    int   n_iter = kernelInfo->n_iter;
    int   m_iter = kernelInfo->m_iter;
    int   n_left = kernelInfo->n_left;
    int   m_left = kernelInfo->m_left;
    int   rs_c   = kernelInfo->rs_c;
    int   cs_c   = kernelInfo->cs_c;
    int   cs_c1;
    int   rstep_a = kernelInfo->rstep_a;
    int   cstep_b = kernelInfo->cstep_b;

    float* restrict a1;
    float* restrict b1, *bp;
    float* restrict c1;
    float* restrict c11;
    float* restrict a10;
    float* restrict a11;
    float* restrict b01;
    float* restrict b11;

    int   i, j;
    int   m_cur;
    int   n_cur;
    int   rstep_c = rs_c * MR;
    int   cstep_c = cs_c * NR;

    int   rs_ct = 1;
    int   cs_ct = NR;
    float zero = 0.0;
    float minus_one  = -1.0;
    int   bC_CMXTransfer;
    int   b_buff_used;
    int   c_buff_used;
    int   numberTasks = -1;
    int   bLoadA = 1;

    int   diagoffb_j;
    int   k_b01;
    int   off_b01;
    int   off_b11;

    int   lengthIter = endIter - startIter;

    c_cast = c_cast + startIter * rstep_c;
    bC_CMXTransfer = (rs_c == 1) ? 1 : 0;

    if(bC_CMXTransfer == 0)
    {
        rstep_c = rs_c * MR;
        cs_c1   = cs_c;
    }
    else
    {
        rstep_c = MR;
        cs_c1   = 4 * lengthIter;
    }

    b1 = b_cast;
    a_cast = a_cast + startIter * rstep_a;
    c1 = c_cast;

    numberTasks = -1;
    scDmaSetup(++numberTasks, (u8*)a_cast, (u8*)a_buff, 4 * rstep_a * lengthIter);
    diagoffb_j = diagoffb;
    k_b0111   = ( (k) < (-diagoffb_j + NR) ? (k) : (-diagoffb_j + NR) );
    scDmaSetup(++numberTasks, (u8*)b1, (u8*)b_buff[0], 4 * 4 * k_b0111);
    if(bC_CMXTransfer)
    {
        scDmaSetupStrideSrc(++numberTasks, (u8*)c1, (u8*)c_buff[0], 4* lengthIter * 4 * 4, lengthIter * 4 * 4, 4 * cs_c - lengthIter * 4 * 4);
    }
    scDmaStart(numberTasks);
    b_buff_used = 1;
    c_buff_used = 2;
    numberTasks = -1;
    scDmaWaitFinished();

    /* Loop over the n dimension (NR columns at a time). */
    for ( j = 0; j < n_iter; ++j )
    {
        diagoffb_j = diagoffb;

        /* Determine the offset to and length of the panel that was packed
           so we can index into the corresponding location in A. */
        off_b01   = 0;
        k_b0111   = __builtin_shave_cmu_min_i32_rr(k, (-diagoffb_j + NR));
        k_b01     = k_b0111 - NR;
        off_b11   = k_b01;

        if(j + 1 < n_iter)
        {
            /* Copy next panel of B DDR to CMX. */
            {
                int diagoffb_j_1, k_b0111_1;

                diagoffb_j_1 = diagoffb_j - NR;
                k_b0111_1   = ( (k) < (-diagoffb_j_1 + NR) ? (k) : (-diagoffb_j_1 + NR) );
                scDmaSetup(++numberTasks, (u8*)(b1 + k_b0111 * PACKNR), (u8*)b_buff[b_buff_used], 4 * 4 * k_b0111_1);
            }
            if(bC_CMXTransfer)
            {
                /* Copy next panel of C DDR to CMX. */
                scDmaSetupStrideSrc(++numberTasks, (u8*)(c1 + cstep_c), (u8*)c_buff[(c_buff_used + 2) % 3], 4 * lengthIter * 4 * 4, lengthIter * 4 * 4, 4 * cs_c - lengthIter * 4 * 4);
            }
        }
        if(numberTasks >= 0)
        {
            scDmaStart(numberTasks);
        }

        n_cur = (( j != n_iter - 1 || n_left == 0) ? NR : n_left);
        b_buff_used = 1 - b_buff_used;
        c_buff_used = (c_buff_used + 1) % 3;
        a1  = a_buff;
        bp  = b_buff[b_buff_used];
        c11 = (bC_CMXTransfer == 0) ? c1 : c_buff[c_buff_used];

        /* Compute the addresses of the panel B10 and the triangular block B11. */
        b01       = bp;
        b11       = bp + k_b01 * PACKNR;

        /* If the current panel of B intersects the diagonal, use a
        special micro-kernel that performs a fused gemm and trsm.
        If the current panel of B resides above the diagonal, use a
        a regular gemm micro-kernel. Otherwise, if it is below the
        diagonal, it was not packed (because it is implicitly zero)
        and so we do nothing. */
        if ( (!(k <= -diagoffb_j ) &&  !(NR <=  diagoffb_j)))
        {
            /* Loop over the m dimension (MR rows at a time). */
            for ( i = startIter; i < endIter; ++i )
            {
                m_cur = ( ( i != m_iter - 1 || m_left == 0 ) ? MR : m_left );

                /* Compute the addresses of the A10 panel and A11 block. */
                a10  = a1 + off_b01 * PACKMR;
                a11  = a1 + off_b11 * PACKMR;
                //printf("%d %d %d %d\n", j, i, k, k_b01);

                /* Handle interior and edge cases separately. */
                if ( m_cur == MR && n_cur == NR )
                {
                    /* Invoke the fused gemm/trsm micro-kernel. */
                    mvcvsgemmtrsm_l4x4_asm( k_b01,
                                            &alphaValue,
                                            b01,
                                            b11,
                                            a10,
                                            a11,
                                            c11, cs_c1, rs_c);
                }
                else
                {
                    /* Invoke the fused gemm/trsm micro-kernel. */
                    mvcvsgemmtrsm_l4x4_asm( k_b01,
                                            &alphaValue,
                                            b01,
                                            b11,
                                            a10,
                                            a11,
                                            ct, cs_ct, rs_ct);

                    /* Copy the result to the bottom edge of C. */
                    {
                        int i, j;

                        for ( j = 0; j < n_cur; ++j )
                            for ( i = 0; i < m_cur; ++i )
                            {
                                c11[i*rs_c + j*cs_c1] = ct[i*rs_ct + j*cs_ct];
                            };
                    };
                }

                a1  += rstep_a;
                c11 += rstep_c;
            }
        }
        else if (k <= -diagoffb_j)
        {
            /* Loop over the m dimension (MR rows at a time). */
            for ( i = startIter; i < endIter; ++i )
            {
                m_cur = ( ( i != m_iter - 1 || m_left == 0 ) ? MR : m_left );
                //printf("*%d %d %d\n", j, i, k);

                /* Handle interior and edge cases separately. */
                if ( m_cur == MR && n_cur == NR )
                {
                    /* Invoke the gemm micro-kernel. */
                    mvcvsgemm4x4_asm( k,
                                      &minus_one,
                                      bp,
                                      a1,
                                      &alphaValue,
                                      c11, cs_c1, rs_c);
                }
                else
                {
                    /* Invoke the gemm micro-kernel. */
                    mvcvsgemm4x4_asm( k,
                                      &minus_one,
                                      bp,
                                      a1,
                                      &zero,
                                      ct, cs_ct, rs_ct);

                    /* Add the result to the edge of C. */
                    {
                        int i, j;

                        for ( j = 0; j < n_cur; ++j )
                            for ( i = 0; i < m_cur; ++i )
                            {
                                c11[i*rs_c + j*cs_c1] = ct[i*rs_ct + j*cs_ct] + (alphaValue *  c11[i*rs_c + j*cs_c1]);
                            };
                    };
                }

                a1  += rstep_a;
                c11 += rstep_c;
            }
        }

        diagoffb -= NR;
        numberTasks = -1;
        scDmaWaitFinished();

        if(bC_CMXTransfer)
        {
            scDmaSetupStrideDst(++numberTasks, (u8*)c_buff[c_buff_used], (u8*)c1, 4 * lengthIter * 4 * 4, lengthIter * 4 * 4, 4 * cs_c - lengthIter * 4 * 4);
        }
        bLoadA = 0;

        b1 += k_b0111 * PACKNR;
        c1 += cstep_c;
    }

    if(bC_CMXTransfer)
    {
        scDmaStart(numberTasks);
        scDmaWaitFinished();
    }

    exit(0);
    return;
}
#endif
