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
u32 sippInitGenChr (SippFilter *fptr)
{
    GenChrParam * param    = (GenChrParam*)fptr->params;
    u32           retVal;

    retVal  = sippIbufSetup (fptr, 0);
    retVal += sippObufSetup (fptr, 0);

    //One time constants updates
    param->frmDim = (fptr->parents[0]->outputH << SIPP_IMGDIM_SIZE)|fptr->parents[0]->outputW;

    return retVal;
}

//#######################################################################################
// Clearly this is a HW session only function

void sippLoadGenChr (SippFilter *fptr)
{
    SippHwBuf   *targetI = (SippHwBuf*)I_BASE(SIPP_CGEN_ID);
    SippHwBuf   *targetO = (SippHwBuf*)O_BASE(SIPP_CGEN_ID);
    u32          paramSet;
    SippHwBuf   *srcI  = fptr->iBuf[0];
    SippHwBuf   *srcO  = fptr->oBuf[0];
    GenChrParam *param = (GenChrParam*)fptr->params;

    // Am I oPipe connected on Input or Output?
    // Program I & O buffers
    if (srcI)
    {
        // Can assume we are not oPipe connected
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

    paramSet = SIPP_CGEN_BASE_ADR;

    // Filter specific params
    SET_REG_WORD(paramSet + 0x00, param->frmDim);   //SIPP_CGEN_FRM_DIM_ADR
    SET_REG_WORD(paramSet + 0x04, param->cfg);      //SIPP_CGEN_CFG_ADR
    SET_REG_WORD(paramSet + 0x08, param->yCoefs);   //SIPP_CGEN_LUMA_COEFFS_ADR
    SET_REG_WORD(paramSet + 0x0C, param->chrCoefs); //SIPP_CGEN_GEN_CHR_COEFFS_ADR
}

u32 sippGetIBufCtxGenChroma (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (iBufIdx); //hush the compiler warning
    UNUSED (fptr);    //hush the compiler warning

    return SIPP_ICTX7_ADR;
}

u32 sippGetIBufIdsGenChroma (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (iBufIdx); //hush the compiler warning

    // Set the bit pos of a chroma generation input buffer increment
    return (0x1 << SIPP_CGEN_ID);
}

u32 sippGetOBufIdsGenChroma (SippFilter *fptr, u32 oBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (oBufIdx); //hush the compiler warning

    // Set the bit pos of a chroma generation filter output buffer interrupt
    return (0x1 << SIPP_CGEN_ID);
}

void sippSetBufLatenciesGenChroma (SippFilter *fptr)
{
    // Fill in the latencies of each input and output buffer
    if (fptr->iBufCtx[0]) fptr->iBufCtx[0]->consumptionLatency = 0;
    fptr->OBufProdLatency[0]                                   = 0;
}

void sippSetOBufLevelsGenChroma (SippFilter *fptr, eSippObufControl eOBufSetting)
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

        SET_REG_WORD(O_FC(SIPP_CGEN_ID), (1<<31) | level);
    }
}

#endif
