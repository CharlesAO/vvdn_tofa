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

//########################################################################################
//This does NOT touch HW so can safely be called from a pipeline session not active on HW

u32 sippInitColComb (SippFilter * fptr)
{
    ColCombParam * param  = (ColCombParam*)fptr->params;
    u32            retVal = 0;

    retVal  = sippIbufSetup(fptr, 0); //Compact Input buffer : Luma
    retVal += sippIbufSetup(fptr, 1); //Compact Input buffer : Chroma planes
    retVal += sippObufSetup(fptr, 0); //Compact Output buffer

    //One time constants updates
    param->frmDim = (fptr->outputH << SIPP_IMGDIM_SIZE)|fptr->outputW;

    return retVal;
}

//########################################################################################

void sippLoadColComb (SippFilter *fptr)
{
    SippHwBuf *srcI1     = fptr->iBuf[0];
    SippHwBuf *srcI2     = fptr->iBuf[1];
    SippHwBuf *srcO      = fptr->oBuf[0];
    SippHwBuf *targetI1  = (SippHwBuf*)I_BASE(SIPP_CC_ID);
    SippHwBuf *targetI2  = (SippHwBuf*)I_BASE(SIPP_CC_CHROMA_ID);
    SippHwBuf *targetLut = (SippHwBuf*)I_BASE(SIPP_CC_3DLUT_ID);
    SippHwBuf *targetO   = (SippHwBuf*)O_BASE(SIPP_CC_ID);
    ColCombParam *param  = (ColCombParam*)fptr->params;

    //Program new I & O buffers
    if (srcI1)
    {
        PROG_IO_BUFF(targetI1, srcI1);
    }
    else
    {
        // Still need to set up planes for an oPipe setup
        // Should add extra control to be sure this is the luma parent...
        SippFilter * par     = fptr->parents[0];
        s32          oBufIdx = fptr->parentOBufIdx[0];

        SET_REG_WORD((u32)&targetI1->cfg, ((par->nPlanes[oBufIdx]-1) << SIPP_NP_OFFSET));
    }

    if (srcI2)
    {
        PROG_IO_BUFF(targetI2, srcI2); //Input-Ref
    }
    else
    {
        // Still need to set up planes for an oPipe setup
        // SHould add extra control to be sure this is the chroma parent...
        SippFilter * par     = fptr->parents[1];
        s32          oBufIdx = fptr->parentOBufIdx[1];

        SET_REG_WORD((u32)&targetI2->cfg, ((par->nPlanes[oBufIdx]-1) << SIPP_NP_OFFSET));
    }

    if (srcO)
    {
        PROG_IO_BUFF(targetO , srcO ); //Output
    }
    else
    {
        // Still need to set up planes for an oPipe setup
        SET_REG_WORD((u32)&targetO->cfg, ((fptr->nPlanes[0]-1) << SIPP_NP_OFFSET) | (2  << SIPP_FO_OFFSET));
    }

    // Set up the SIPP_CC_3DLUT_ID
    if (!((param->cfg >> CC_CFG_3DLUT_BYPASS_SHIFT) & CC_CFG_3DLUT_BYPASS_MASK))
    {
        if (param->threeDLut)
        {
            SET_REG_WORD((u32)&targetLut->base, (u32)param->threeDLut);
            SET_REG_WORD((u32)&targetLut->cfg,  ((param->lutFormat & SIPP_FO_MASK) << SIPP_FO_OFFSET));
        }
    }

    //Color Comb specific params:
    SET_REG_WORD(SIPP_CC_FRM_DIM_ADR, param->frmDim);
    SET_REG_WORD(SIPP_CC_CFG_ADR    , param->cfg);
    SET_REG_WORD(SIPP_CC_KRGB0_ADR  , param->krgb[0]);
    SET_REG_WORD(SIPP_CC_KRGB1_ADR  , param->krgb[1]);
    SET_REG_WORD(SIPP_CC_CCM10_ADR  , param->ccm[0]);
    SET_REG_WORD(SIPP_CC_CCM32_ADR  , param->ccm[1]);
    SET_REG_WORD(SIPP_CC_CCM54_ADR  , param->ccm[2]);
    SET_REG_WORD(SIPP_CC_CCM76_ADR  , param->ccm[3]);
    SET_REG_WORD(SIPP_CC_CCM8_ADR   , param->ccm[4]);
    SET_REG_WORD(SIPP_CC_OFFSETS_ADR, param->ccOffs);
}

u32 sippGetIBufCtxColComb (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED(fptr); //hush the compiler warning

    if (iBufIdx == 0)
    {
        return SIPP_ICTX10_ADR;
    }
    else
    {
        return SIPP_ICTX22_ADR;
    }
}

u32 sippGetIBufIdsColComb (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED(fptr); //hush the compiler warning

    // Do we really need to enforce the luma buffer being the first input buffer now?
    if (iBufIdx == 0) return (0x1 << SIPP_CC_ID);
    if (iBufIdx == 1) return (0x1 << SIPP_CC_CHROMA_ID);
    return 0;
}

u32 sippGetOBufIdsColComb (SippFilter *fptr, u32 oBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (oBufIdx); //hush the compiler warning

    // Set the bit pos of a color combination filter output buffer interrupt
    return (0x1 << SIPP_CC_ID);
}

void sippSetBufLatenciesColourComb (SippFilter *fptr)
{
    // Fill in the latencies of each input and output buffer
    if (fptr->iBufCtx[0]) fptr->iBufCtx[0]->consumptionLatency = 0;
    if (fptr->iBufCtx[1]) fptr->iBufCtx[1]->consumptionLatency = 0;
    fptr->OBufProdLatency[0]                                   = 0;
}

void sippSetOBufLevelsColourComb (SippFilter *fptr, eSippObufControl eOBufSetting)
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

        SET_REG_WORD(O_FC(SIPP_CC_ID), (1<<31) | level);
    }
}


#endif
