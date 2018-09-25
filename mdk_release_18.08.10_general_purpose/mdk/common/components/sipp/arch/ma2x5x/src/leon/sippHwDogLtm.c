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
//This does NOT touch HW so can safely be called form a pipeline session not active on HW

u32 sippInitDogLtm (SippFilter *fptr)
{
    DogLtmParam * param  = (DogLtmParam *)fptr->params;
    u32           retVal = 0;

    retVal  = sippIbufSetup (fptr, 0);
    retVal += sippObufSetup (fptr, 0);

    // One time constants updates
    param->frmDim = (fptr->parents[0]->outputH << SIPP_IMGDIM_SIZE)|fptr->parents[0]->outputW;

    return retVal;
}

//#######################################################################################
// Clearly this is a HW session only function
void sippLoadDogLtm (SippFilter *fptr)
{
    SippHwBuf   * targetI = (SippHwBuf*)I_BASE(SIPP_DOGL_ID);
    SippHwBuf   * targetO = (SippHwBuf*)O_BASE(SIPP_DOGL_ID);
    SippHwBuf   * srcI0   = fptr->iBuf[0];
    SippHwBuf   * srcI1   = fptr->iBuf[1];
    SippHwBuf   * srcO    = fptr->oBuf[0];

    DogLtmParam * param   = (DogLtmParam*)fptr->params;
    u8          * K0C     = param->dogCoeffs11;
    u8          * K1C     = param->dogCoeffs15;
    u8            mode    = (param->cfg >> DOGL_CFG_MODE_SHIFT) & DOGL_CFG_MODE_MASK;

    //Program I & O buffers
    if (srcI0)
    {
        // This does not necessarily mean we are not oPipe connected on input
        // as this buffer needs to be setup for later reading in certain modes
        // (as well as being streamed to the LLB)
        PROG_IO_BUFF(targetI, srcI0);
    }
    else
    {
        // Still need to set up planes for an oPipe setup
        SippFilter * par     = fptr->parents[0];
        s32          oBufIdx = fptr->parentOBufIdx[0];

        SET_REG_WORD((u32)&targetI->cfg, ((par->nPlanes[oBufIdx]-1) << SIPP_NP_OFFSET));
    }

    if (srcI1)
    {
        // If we were oPipe connected on srcI0 input then this is actually needed
        // Otherwise it is a duplication as this is the same input buffer (and target!)
        // but it does no harm in the latter case
        PROG_IO_BUFF(targetI, srcI1);
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

    // Filter specific params
    SET_REG_WORD(SIPP_DOGL_FRM_DIM_ADR, param->frmDim);
    SET_REG_WORD(SIPP_DOGL_CFG_ADR,     param->cfg);

    // Custom filter regs:
    if (mode != 0x1)
    {
        SET_REG_WORD(SIPP_DOGL_COEF0_ADR, (K0C[3] << 24) | (K0C[2] << 16) | (K0C[1] << 8) | (K0C[0] << 0));
        SET_REG_WORD(SIPP_DOGL_COEF1_ADR, (K1C[1] << 24) | (K1C[0] << 16) | (K0C[5] << 8) | (K0C[4] << 0));
        SET_REG_WORD(SIPP_DOGL_COEF2_ADR, (K1C[5] << 24) | (K1C[4] << 16) | (K1C[3] << 8) | (K1C[2] << 0));
        SET_REG_WORD(SIPP_DOGL_COEF3_ADR, (param->dogStrength    << 16)   | (K1C[7] << 8) | (K1C[6] << 0));
    }

    if (mode & 0x1)
    {
        s32 curve, point, regoff;
        regoff = 0;

        for (curve = 0; curve < 8; curve++)
        {
            for (point = 0; point < 16; point += 2)
            {
                int off = curve + point * 8;
                int a   = param->ltmCurves[off];
                int b   = param->ltmCurves[off+8];
                SET_REG_WORD(SIPP_LTM_COEF_ADR+regoff, (a & 0xfff) | (b & 0xfff) << 16);
                regoff += 4;
            }
        }
    }
}

u32 sippGetIBufCtxDoGLTM (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (iBufIdx); //hush the compiler warning
    UNUSED (fptr);    //hush the compiler warning

    return SIPP_ICTX4_ADR;
}

u32 sippGetIBufIdsDoGLTM (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (iBufIdx); //hush the compiler warning

    // Set the bit pos of a dog filter input buffer increment
    return (0x1 << SIPP_DOGL_ID);
}

u32 sippGetOBufIdsDoGLTM (SippFilter *fptr, u32 oBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (oBufIdx); //hush the compiler warning

    // Set the bit pos of a dog filter output buffer increment
    return (0x1 << SIPP_DOGL_ID);
}

void sippSetBufLatenciesDoGLTM (SippFilter *fptr)
{
    DogLtmParam * param = (DogLtmParam *)fptr->params;
    u8            mode  = (param->cfg >> DOGL_CFG_MODE_SHIFT) & DOGL_CFG_MODE_MASK;

    // Fill in the latencies of each input and output buffer
    if (fptr->iBufCtx[0]) fptr->iBufCtx[0]->consumptionLatency = 0;
    if (fptr->iBufCtx[1]) fptr->iBufCtx[1]->consumptionLatency = 0;
    fptr->OBufProdLatency[0]                                   = (mode != DOGL_MODE_DOG_ONLY) ? 0x1 : 0x0;

}

void sippSetOBufLevelsDoGLTM (SippFilter *fptr, eSippObufControl eOBufSetting)
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

        SET_REG_WORD(O_FC(SIPP_DOGL_ID), (1<<31) | level);
    }
}

void sippSetupUpdateDoGLTM (SippFilter * fptr)
{
    DogLtmParam * param = (DogLtmParam *)fptr->params;
    u8            mode  = (param->cfg >> DOGL_CFG_MODE_SHIFT) & DOGL_CFG_MODE_MASK;

    // We need to establish if downsampling will occur as that will effectively double
    // the vertical kernel size
    if (mode != DOGL_MODE_DOG_ONLY)
    {
        if (fptr->iBufCtx[0])
        {
            if (fptr->iBufCtx[0]->numLinesUsed > 1)
            {
                fptr->iBufCtx[0]->numLinesUsed <<= 0x1;
                fptr->iBufCtx[0]->numLinesUsed++;
            }
        }
        else if (fptr->iBufCtx[1])
        {
            if (fptr->iBufCtx[1]->numLinesUsed > 1)
            {
                fptr->iBufCtx[1]->numLinesUsed <<= 0x1;
                fptr->iBufCtx[1]->numLinesUsed++;
            }
        }

        // We actually read the input buffer twice when this is the case
        // So we need to set it up like a second input
        sippLinkFilter (fptr, fptr->parents[0], 0x1, 0x1);

        // We also need to match the obuf idx with that given by the first connection...
        sippLinkFilterSetOBuf (fptr, fptr->parents[0], fptr->parentOBufIdx[0]);
    }
}

#endif
