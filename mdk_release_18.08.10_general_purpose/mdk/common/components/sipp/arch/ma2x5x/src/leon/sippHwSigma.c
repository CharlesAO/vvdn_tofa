///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

#include <sipp.h>
#include <sippInternal.h>

//#######################################################################################
// This does NOT touch HW so can safely be called from a pipeline session not active on HW
u32 sippInitSigma (SippFilter * fptr)
{
    u32          retVal = 0;
    SigmaParam * param  = (SigmaParam*)fptr->params;

    retVal  = sippIbufSetup (fptr, 0);
    retVal += sippObufSetup (fptr, 0);

    // One time constants updates
    param->frmDim = (fptr->parents[0]->outputH << SIPP_IMGDIM_SIZE)|fptr->parents[0]->outputW;

    return retVal;
}

//#######################################################################################
// Clearly this is a HW session only function

void sippLoadSigma (SippFilter *fptr)
{
    SippHwBuf  *targetI = (SippHwBuf*)I_BASE(SIPP_SIGMA_ID);
    SippHwBuf  *targetO = (SippHwBuf*)O_BASE(SIPP_SIGMA_ID);
    SippHwBuf  *srcI    = fptr->iBuf[0];
    SippHwBuf  *srcO    = fptr->oBuf[0];
    SigmaParam *param   = (SigmaParam*)fptr->params;

    u32      b[4]; // black level

    // Am I oPipe connected on Input or Output?
    // Program I & O buffers
    if (srcI)
    {
        // Can assume we are not oPipe connected on input
        PROG_IO_BUFF(targetI, srcI);
    }
    else
    {
        // Still need to set up planes for an oPipe setup
        SippFilter * par     = fptr->parents[0];
        s32          oBufIdx = fptr->parentOBufIdx[0];

        SET_REG_WORD((u32)&targetI->cfg, ((par->nPlanes[oBufIdx]-1) << SIPP_NP_OFFSET));
    }

    if (srcO)
    {
        PROG_IO_BUFF(targetO, srcO);
    }
    else
    {
        // Still need to set up planes for an oPipe setup
        SET_REG_WORD((u32)&targetO->cfg, ((fptr->nPlanes[0]-1) << SIPP_NP_OFFSET));
    }

    // Filter specific setup
    SET_REG_WORD(SIPP_SIGMA_FRM_DIM_ADR     , param->frmDim);
    SET_REG_WORD(SIPP_SIGMA_THRESH_P0P1_ADR , param->thresh[0]);
    SET_REG_WORD(SIPP_SIGMA_THRESH_P2P3_ADR , param->thresh[1]);
    SET_REG_WORD(SIPP_SIGMA_CFG_ADR         , param->cfg);

    // Figure out black level pixels' order based on BayerPattern
    if ((param->cfg >> SIGMA_DNS_CFG_FORMAT_SHIFT) & SIGMA_DNS_CFG_FORMAT_MASK)
    {
        sippUtilOrderPixels (param->bayerPattern,
                             param->blcGR,
                             param->blcR,
                             param->blcB,
                             param->blcGB,
                             b);
    }
    else
    {
        b[0] = param->blcGR; b[1] = param->blcR; b[2] = param->blcB; b[3] = param->blcGB;
    }

    SET_REG_WORD(SIPP_SIGMA_BLACK01_ADR, b[0] | b[1] << 16);
    SET_REG_WORD(SIPP_SIGMA_BLACK23_ADR, b[2] | b[3] << 16);
}

u32 sippGetIBufCtxSigma (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (iBufIdx); //hush the compiler warning
    UNUSED (fptr);    //hush the compiler warning

    return SIPP_ICTX0_ADR;

}

u32 sippGetIBufIdsSigma (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (iBufIdx); //hush the compiler warning

    // Set the bit pos of a sigma filter input buffer increment
    return (0x1 << SIPP_SIGMA_ID);
}

u32 sippGetOBufIdsSigma (SippFilter *fptr, u32 oBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (oBufIdx); //hush the compiler warning

    // Set the bit pos of a sigma filter output buffer interrupt
    return (0x1 << SIPP_SIGMA_ID);
}

void sippSetBufLatenciesSigma (SippFilter *fptr)
{
    // Fill in the latencies of each input and output buffer
    fptr->iBufCtx[0]->consumptionLatency = 0;
    fptr->OBufProdLatency[0]             = 0;

    // Potentially update funcAsk[idx] to a latency tolerant version
    if (fptr->iBufCtx[0]->consumptionLatency)
    {
      //  fptr->funcAsk[0] = sippSchReqMap2Lat (fptr->funcAsk[0]);
    }
}

void sippSetOBufLevelsSigma (SippFilter *fptr, eSippObufControl eOBufSetting)
{
    u32 level = 0;
    u32 numLinesIter = fptr->linesPerIter;

    if (fptr->nLines[0])
    {
        switch (eOBufSetting)
        {
            case eSIPP_SET_OBUF_SPACE_FULL  : level = fptr->nLines[0]; break;
            case eSIPP_SET_OBUF_SPACE_EMPTY : level = 0; break;
            case eSIPP_SET_OBUF_SPACE_ITER  : level = fptr->nLines[0] - numLinesIter; break;
        }

        SET_REG_WORD(O_FC(SIPP_SIGMA_ID), (1<<31) | level);
    }
}


