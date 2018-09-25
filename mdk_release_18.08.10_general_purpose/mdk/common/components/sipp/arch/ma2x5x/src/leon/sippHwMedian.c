///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

#include <sipp.h>
#include <sippInternal.h>

#if (defined(SIPP_PC) || defined(MYRIAD2))

//#######################################################################################
//This does NOT touch HW so can safely be called from a pipeline session not active on HW
u32 sippInitMed (SippFilter * fptr)
{
    MedParam * param  = (MedParam *)fptr->params;
    u32        retVal = 0;

    retVal = sippIbufSetup (fptr, 0);
    if (fptr->parents[1]) retVal += sippIbufSetup (fptr, 1);
    retVal += sippObufSetup (fptr, 0);

    // One time constants updates
    param->frmDim = (fptr->parents[0]->outputH << SIPP_IMGDIM_SIZE)|fptr->parents[0]->outputW;

    return retVal;
}

//#######################################################################################
// Clearly this is a HW session only function

void sippLoadMed (SippFilter * fptr)
{
    SippHwBuf *targetI;
    SippHwBuf *targetO;
    SippHwBuf *srcChr = fptr->iBuf[0];
    SippHwBuf *srcLum = fptr->iBuf[1];
    SippHwBuf *srcO   = fptr->oBuf[0];
    MedParam  *param  = (MedParam*)fptr->params;

    targetI  = (SippHwBuf*)I_BASE(SIPP_MED_ID);
    targetO  = (SippHwBuf*)O_BASE(SIPP_MED_ID);

    if (srcChr)
    {
        // Can assume we are not oPipe connected
        PROG_IO_BUFF(targetI, srcChr);
    }
    else
    {
        // Still need to set up planes for an oPipe setup
        SippFilter * par     = fptr->parents[0];
        s32          oBufIdx = fptr->parentOBufIdx[0];

        SET_REG_WORD((u32)&targetI->cfg, ((par->nPlanes[oBufIdx]-1) << SIPP_NP_OFFSET));
    }

    if (srcLum)
    {
        if ((param->cfg >> 28) & 0x1)
        {
            SippFilter * chromaPar   = fptr->parents[0];
            SippFilter * lumaPar     = fptr->parents[1];
            u32          oChrBufIdx  = fptr->parentOBufIdx[0];
            u32          oLumaBufIdx = fptr->parentOBufIdx[1];

            targetI  = (SippHwBuf*)I_BASE(SIPP_MED_LUMA_ID);

            // Need to force the number of luma planes used in alpha blending to be the same as the number of chroma planes
            // The sipp framework may not be told the correct value of number of planes here as that would waste memory
            // since for median filter luma buffer is actually 1 plane, but filter
            // reads each line once for each plane of chroma, set plane stride to 0
            srcLum->cfg  =  lumaPar->nLines[oLumaBufIdx]                                 |   // number of lines
                            ((chromaPar->nPlanes[oChrBufIdx]-1) << SIPP_NP_OFFSET)       |   // number of planes
                            ((1 << (lumaPar->bpp[oLumaBufIdx] >> 4)) << SIPP_FO_OFFSET);     // format
            srcLum->ps   = 0;

            PROG_IO_BUFF(targetI, srcLum);
        }
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

    //Median specific params
    SET_REG_WORD(SIPP_MED_FRM_DIM_ADR,    param->frmDim);
    SET_REG_WORD(SIPP_MED_CFG_ADR,        param->cfg);
    SET_REG_WORD(SIPP_MED_LUMA_ALPHA_ADR, param->lumaAlpha);
}

u32 sippGetIBufCtxMed (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning

    if (iBufIdx == 0)
    {
        return SIPP_ICTX8_ADR;
    }
    else
    {
        return SIPP_ICTX21_ADR;
    }
}

u32 sippGetIBufIdsMed (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED(fptr); //hush the compiler warning

    // Set the bit pos of a median input buffer increment
    if (iBufIdx == 0) return (0x1 << SIPP_MED_ID);
    if (iBufIdx == 1) return (0x1 << SIPP_MED_LUMA_ID);
    return 0;
}

u32 sippGetOBufIdsMed (SippFilter *fptr, u32 oBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (oBufIdx); //hush the compiler warning

    // Set the bit pos of a median filter output buffer interrupt
    return (0x1 << SIPP_MED_ID);
}

void sippSetBufLatenciesMedian (SippFilter *fptr)
{
    // Fill in the latencies of each input and output buffer
    if (fptr->iBufCtx[0]) fptr->iBufCtx[0]->consumptionLatency = 0;
    if (fptr->iBufCtx[1]) fptr->iBufCtx[1]->consumptionLatency = 0;
    fptr->OBufProdLatency[0]                                   = 0;
}

void sippSetOBufLevelsMedian (SippFilter *fptr, eSippObufControl eOBufSetting)
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

        SET_REG_WORD(O_FC(SIPP_MED_ID), (1<<31) | level);
    }
}

void sippSetupUpdateMedian (SippFilter * fptr)
{
    if (fptr->nParents == 2)
    {
        // If we need to sub-sample the luma we need a different scheduler
        MedParam * medCfg       = (MedParam *)fptr->params;

        if ((medCfg->cfg >> 28) & 0x1)
        {
            // Override the number of lines used from this input buffer
            fptr->iBufCtx[0x1]->numLinesUsed = ((medCfg->cfg >> 28) & 0x1) ? 0x2 : 0x1;

        }
    }
}


#endif
