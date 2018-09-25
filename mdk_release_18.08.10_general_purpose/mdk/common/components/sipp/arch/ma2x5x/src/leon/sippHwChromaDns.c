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

u32 sippInitChrDns (SippFilter *fptr)
{
    u32           retVal = 0;
    ChrDnsParam * param = (ChrDnsParam*)fptr->params;

    retVal  = sippIbufSetup (fptr, 0);
    retVal += sippObufSetup (fptr, 0);

    // One time constants updates
    param->frmDim = (fptr->parents[0]->outputH << SIPP_IMGDIM_SIZE) | fptr->parents[0]->outputW;

    return retVal;
}

//#######################################################################################
// Clearly this is a HW session only function

void sippLoadChrDns (SippFilter *fptr)
{
    SippHwBuf   *targetI;
    SippHwBuf   *targetO;
    SippHwBuf   *srcI = fptr->iBuf[0];
    SippHwBuf   *srcO = fptr->oBuf[0];
    ChrDnsParam *param = (ChrDnsParam*)fptr->params;

    targetI    = (SippHwBuf*)I_BASE(SIPP_CHROMA_ID);
    targetO    = (SippHwBuf*)O_BASE(SIPP_CHROMA_ID);

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

        SET_REG_WORD((u32)&targetI->cfg, ((par->nPlanes[oBufIdx]-1) << SIPP_NP_OFFSET) | (2  << SIPP_FO_OFFSET));
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

    //Chroma Denoise specifics:
    SET_REG_WORD(SIPP_CHROMA_FRM_DIM_ADR, param->frmDim  );
    SET_REG_WORD(SIPP_CHROMA_CFG_ADR, param->cfg     );
    SET_REG_WORD(SIPP_CHROMA_THRESH_ADR, param->thr[0]  );
    SET_REG_WORD(SIPP_CHROMA_THRESH2_ADR, param->thr[1]  );
    SET_REG_WORD(SIPP_CHROMA_GREY_POINT_ADR, param->greyPt  );
    SET_REG_WORD(SIPP_CHROMA_CHROMA_COEFFS_ADR, param->chrCoefs);
}

u32 sippGetIBufCtxChrDns (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (iBufIdx); //hush the compiler warning
    UNUSED (fptr);    //hush the compiler warning

    return SIPP_ICTX9_ADR;
}

u32 sippGetIBufIdsChrDns (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (fptr);   //hush the compiler warning
    UNUSED (iBufIdx); //hush the compiler warning

    // Set the bit pos of a chroma denoise filter input buffer increment
    return (0x1 << SIPP_CHROMA_ID);
}

u32 sippGetOBufIdsChrDns (SippFilter *fptr, u32 oBufIdx)
{
    UNUSED (fptr);   //hush the compiler warning
    UNUSED (oBufIdx); //hush the compiler warning

    // Set the bit pos of a chroma denoise filter output buffer interrupt
    return (0x1 << SIPP_CHROMA_ID);
}

void sippSetBufLatenciesChrDns (SippFilter *fptr)
{
    // Fill in the latencies of each input and output buffer
    if (fptr->iBufCtx[0]) fptr->iBufCtx[0]->consumptionLatency = 0;
    fptr->OBufProdLatency[0]                                   = 10;
}

void sippSetOBufLevelsChrDns (SippFilter *fptr, eSippObufControl eOBufSetting)
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

        SET_REG_WORD(O_FC(SIPP_CHROMA_ID), (1<<31) | level);
    }
}



#endif
