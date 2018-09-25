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
#define DMA_TASKS 3

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------
void mvcvsgemm4x4_asm(int k, float* alpha, float* a, float* b, float* beta, float* c, int rs_c, int cs_c);
void mvcvsgemmtrsm_u4x4_asm(int k, float *alpha, float *a12, float *a11, float *bd21, float *b11, float *c11, int rs_c, int cs_c);

void startSGEMMTRSM_LU(kernelInfo_t *kernelInfo, int diagoffa, int startIter, int endIter)
{
    float *a_cast    = (float*)kernelInfo->a;
    float alphaValue = *(float*)kernelInfo->alpha;
    int   k          = kernelInfo->k;
    int   k_a1112;
    int   n_iter = kernelInfo->n_iter;
    int   m_iter = kernelInfo->m_iter;
    int   n_left = kernelInfo->n_left;
    int   m_left = kernelInfo->m_left;
    int   rs_c   = kernelInfo->rs_c;
    int   cs_c   = kernelInfo->cs_c;
    int   rstep_a = kernelInfo->rstep_a;
    int   cstep_b = kernelInfo->cstep_b;

    float* restrict a1;
    float* restrict b1, *bp;
    float* restrict c1;
    float* restrict c11;
    float* restrict b11;
    float* restrict a12;
    float* restrict a11;
    float* restrict bp21;
    float* restrict a2;

    int   i, j, ib;
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

    int   diagoffa_i;
    int   k_a11;
    int   k_a12;
    int   off_a11;
    int   off_a12;
    int   lengthCLine = 0;
    int   lengthCStride = 0;
#if (defined MYRIAD1)
    /* Dummy variables */
    u32   task[DMA_TASKS];
    u32   *ref[DMA_TASKS];

#elif (defined MYRIAD2)

#ifdef CVMA2480
 //   DMA_TRANSACTION_LIST *ref[DMA_TASKS];
 //    DMA_TRANSACTION_LIST task[DMA_TASKS];
//     ShDrvCmxDmaTransactionHnd id1;
     ShDrvCmxDmaInitialize(NULL);
#else
    DMA_TRANSACTION_LIST task[DMA_TASKS];
    DMA_TRANSACTION_LIST *ref[DMA_TASKS];
    u32                  id1 = dmaInitRequester(1);
#endif
#endif

    /* Check if not general stride... */
#ifdef DDR_BEHAVIOUR
    bC_CMXTransfer = 0;
#else
    bC_CMXTransfer = ((rs_c == 1) || (cs_c == 1));
#endif

    if(bC_CMXTransfer == 0)
    {
        rstep_c = rs_c * MR;
    }
    else
    {
        if(rs_c == 1)
        {
            lengthCLine = m_iter * 4 * 4;
            lengthCStride = 4 * cs_c;
            rstep_c = MR;
            cs_c   = 4 * m_iter;
        }
        else
        {
            lengthCLine = 4 * 4;
            lengthCStride = 4 * rs_c;
            rstep_c = 4 * MR;
            rs_c   = MR;
        }
#if (defined MYRIAD1)
        /* DMA transfer issue on Myriad 1 where padding is used instead of stride */
        lengthCStride -= lengthCLine;
#endif
    }

    c1 = (float*)(kernelInfo->c) + startIter * cstep_c;
    b1 = (float*)(kernelInfo->b) + startIter * cstep_b;
#ifdef A_BUFF_OPT
    DMA_CREATE_TRANSACTION(ref, numberTasks, task, id1, kernelInfo->a, a_buff, 4 * rstep_a * 2);
#else /*A_BUFF_OPT*/
#if 1
    /* Copy A matrix from DDR heap to CMX. */
    {
        int lengthA = 0;

        diagoffa_i = diagoffa;
        for(ib = 0; ib < m_iter; ++ib)
        {
            i = m_iter - 1 - ib;

            diagoffa_i = diagoffa + i*MR;

            if ((!(MR <= -diagoffa_i) && !(k <= diagoffa_i)))
            {
                off_a11 = ( (diagoffa_i) > (0) ? (diagoffa_i) : (0) );
                k_a1112 = k - off_a11;;
                lengthA += k_a1112 * PACKMR;
            }
            else if((MR <= -diagoffa_i))
            {
                lengthA += rstep_a;
            }
        }
        DMA_CREATE_TRANSACTION(ref, numberTasks, task, id1, kernelInfo->a, a_buff, 4 * lengthA);
    }
#else
    DMA_CREATE_TRANSACTION(ref, numberTasks, task, id1, kernelInfo->a, a_buff, 4 * rstep_a * m_iter);
#endif
#endif /*A_BUFF_OPT*/

    DMA_CREATE_TRANSACTION(ref, numberTasks, task, id1, b1, b_buff[0], 4 * 4 * k);
    DMA_LINK_TWO_TASKS(ref, numberTasks);
    if(bC_CMXTransfer)
    {
        DMA_CREATE_TRANSACTION_SRC_STRIDE(ref, numberTasks, task, id1, c1, c_buff[0], 4*m_iter * 4 * 4, lengthCLine, lengthCStride);
        DMA_LINK_TWO_TASKS(ref, numberTasks);
    }
    DMA_START_LIST_TASK(ref[0], numberTasks);
    DMA_WAIT_TASK(ref[0]);
    b_buff_used = 1;
    c_buff_used = 2;
    numberTasks = -1;

    /* Loop over the n dimension (NR columns at a time). */
    for(j = startIter; j < endIter; j++)
    {
        if(j + 1 < endIter)
        {
            /* Copy next panel of B DDR to CMX. */
            DMA_CREATE_TRANSACTION(ref, numberTasks, task, id1, (b1 + cstep_b), b_buff[b_buff_used], 4 * 4 * k);
            if(numberTasks > 0)
            {
                DMA_LINK_TWO_TASKS(ref, numberTasks);
            }
            if(bC_CMXTransfer)
            {
                /* Copy next panel of C DDR to CMX. */
                DMA_CREATE_TRANSACTION_SRC_STRIDE(ref, numberTasks, task, id1, (c1 + cstep_c), c_buff[(c_buff_used + 2) % 3], 4*m_iter * 4 * 4, lengthCLine, lengthCStride);
                DMA_LINK_TWO_TASKS(ref, numberTasks);
            }
        }
        if(numberTasks >= 0)
        {
            DMA_START_LIST_TASK(ref[0], numberTasks);
        }

        n_cur = (( j != n_iter - 1 || n_left == 0) ? NR : n_left);
        b_buff_used = 1 - b_buff_used;
        c_buff_used = (c_buff_used + 1) % 3;
#ifdef DDR_BEHAVIOUR
        a1  = a_cast;
        bp  = b1;
#else
        a1  = a_buff;
        bp  = b_buff[b_buff_used];
#endif
        c11  = (bC_CMXTransfer == 0) ? c1 : c_buff[c_buff_used];

        c11 += (m_iter-1)*rstep_c;

        /* Loop over the m dimension (MR rows at a time). */
        for ( ib = 0; ib < m_iter; ++ib )
        {
            i          = m_iter - 1 - ib;
            diagoffa_i = diagoffa + i*MR;

            m_cur = ( ( ib != 0 || m_left == 0 ) ? MR : m_left );

            /* If the current panel of A intersects the diagonal, use a
            special micro-kernel that performs a fused gemm and trsm.
            If the current panel of A resides above the diagonal, use a
            a regular gemm micro-kernel. Otherwise, if it is below the
            diagonal, it was not packed (because it is implicitly zero)
            and so we do nothing. */
            if ((!(MR <= -diagoffa_i ) && !(k <=  diagoffa_i )))
            {
                /* Compute various offsets into and lengths of parts of A. */
                off_a11 = ( (diagoffa_i) > (0) ? (diagoffa_i) : (0) );
                k_a1112 = k - off_a11;;
                k_a11   = MR;
                k_a12   = k_a1112 - MR;
                off_a12 = off_a11 + k_a11;

                /* Compute the addresses of the triangular block A11 and the panel A12. */
                a11  = a1;
                a12  = a1 + k_a11 * PACKMR;

                /* Now compute the corresponding addresses in Bd. */
                bp21 = bp + off_a12 * NR;

                /* Index into b1 to locate the MR x NR block of b1 that will be
                updated by the trsm subproblem. */
                b11  = bp + off_a11 * PACKNR;

                /* Compute the addresses of the next panels of A and B. */
                a2 = a1 + k_a1112 * PACKMR;
                if ( ib == m_iter - 1 )
                {
                    a2 = a_cast;
                }

                /* Handle interior and edge cases separately. */
                if ( m_cur == MR && n_cur == NR )
                {
                    /* Invoke the fused gemm/trsm micro-kernel. */
                    mvcvsgemmtrsm_u4x4_asm( k_a12,
                                            &alphaValue,
                                            a12,
                                            a11,
                                            bp21,
                                            b11,
                                            c11, rs_c, cs_c);
                }
                else
                {
                    /* Invoke the fused gemm/trsm micro-kernel. */
                    mvcvsgemmtrsm_u4x4_asm( k_a12,
                                            &alphaValue,
                                            a12,
                                            a11,
                                            bp21,
                                            b11,
                                            ct, rs_ct, cs_ct);

                    /* Copy the result to the bottom edge of C. */
                    {
                        int i, j;

                        for ( j = 0; j < n_cur; ++j )
                            for ( i = 0; i < m_cur; ++i )
                            {
                                c11[i*rs_c + j*cs_c] = ct[i*rs_ct + j*cs_ct];
                            };
                    };
                }

#ifndef DDR_BEHAVIOUR
                /* Copy back the result of b11. */
                {
                    float4 *bDDR = (float4*)&b1[off_a11 * PACKNR];
                    float4 *bCMX = (float4*)b11;

                    bDDR[0] = bCMX[0];
                    bDDR[1] = bCMX[1];
                    bDDR[2] = bCMX[2];
                    bDDR[3] = bCMX[3];
                };
#endif
                a1 += k_a1112 * PACKMR;
            }
            else if (MR <= -diagoffa_i)
            {
                /* Compute the addresses of the next panels of A and B. */
                a2 = a1 + rstep_a;
                if ( ib == m_iter - 1 )
                {
                    a2 = a_cast;
                }

                /* Handle interior and edge cases separately. */
                if ( m_cur == MR && n_cur == NR )
                {
                    /* Invoke the gemm micro-kernel. */
                    mvcvsgemm4x4_asm( k,
                                      &minus_one,
                                      a1,
                                      bp,
                                      &alphaValue,
                                      c11, rs_c, cs_c);
                }
                else
                {
                    /* Invoke the gemm micro-kernel. */
                    mvcvsgemm4x4_asm( k,
                                      &minus_one,
                                      a1,
                                      bp,
                                      &zero,
                                      ct, rs_ct, cs_ct);

                    /* Add the result to the edge of C. */
                    {
                        int i, j;

                        for ( j = 0; j < n_cur; ++j )
                            for ( i = 0; i < m_cur; ++i )
                            {
                                c11[i*rs_c + j*cs_c] = ct[i*rs_ct + j*cs_ct] + ((alphaValue) * c11[i*rs_c + j*cs_c]);
                            };
                    };
                }

                a1 += rstep_a;
            }

            c11 -= rstep_c;
#ifdef A_BUFF_OPT
            if(bLoadA == 1)
            {
                if(numberTasks >= 0)
                {
                    DMA_WAIT_TASK(ref[0]);
                    numberTasks = -1;
                }
                if(ib + 2 < m_iter)
                {
                    DMA_CREATE_TRANSACTION(ref, numberTasks, task, id1, &a_cast[(ib + 2) * rstep_a], &a_buff[(ib + 2) * rstep_a], 4 * rstep_a);
                    DMA_START_LIST_TASK(ref[0], numberTasks);
                }
            }
#endif
        }

        if(numberTasks >= 0)
        {
            DMA_WAIT_TASK(ref[0]);
            numberTasks = -1;
        }
        if(bC_CMXTransfer)
        {
            DMA_CREATE_TRANSACTION_DST_STRIDE(ref, numberTasks, task, id1, c_buff[c_buff_used], c1, 4*m_iter * 4 * 4, lengthCLine, lengthCStride);
        }
        c1 += cstep_c;
        b1 += cstep_b;
        bLoadA = 0;
    }
    if(bC_CMXTransfer)
    {
        DMA_START_LIST_TASK(ref[0], numberTasks);
        DMA_WAIT_TASK(ref[0]);
    }

    exit(0);
    return;
}

