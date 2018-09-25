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
u32 sippInitSharpen (SippFilter *fptr)
{
    UsmParam * param  = (UsmParam *)fptr->params;
    u32        retVal = 0x0;

    retVal  = sippIbufSetup (fptr, 0);
    retVal += sippObufSetup (fptr, 0);

    // One time constants updates
    param->frmDim = (fptr->parents[0]->outputH << SIPP_IMGDIM_SIZE)|fptr->parents[0]->outputW;

    return retVal;
}

//#######################################################################################
// Clearly this is a HW session only function

void sippLoadSharpen (SippFilter *fptr)
{
    SippHwBuf *targetI, *targetO;
    u32        paramSet;
    SippHwBuf *srcI  = fptr->iBuf[0];
    SippHwBuf *srcO  = fptr->oBuf[0];
    UsmParam  *param = (UsmParam*)fptr->params;

    targetI  = (SippHwBuf*)I_BASE(SIPP_SHARPEN_ID);
    targetO  = (SippHwBuf*)O_BASE(SIPP_SHARPEN_ID);
    paramSet = SIPP_SHARPEN_BASE_ADR;

    // Program I & O buffers
    if (srcI)
    {
        // Can assume we are not oPipe connected on input
        PROG_IO_BUFF(targetI, srcI);
    }
    else
    {
        // Still need to set up planes for an oPipe setup
        // And because of http://dub30/bugzilla/show_bug.cgi?id=21574#c3
        // format in this case
        SippFilter * par     = fptr->parents[0];
        s32          oBufIdx = fptr->parentOBufIdx[0];

        SET_REG_WORD((u32)&targetI->cfg, ((par->nPlanes[oBufIdx]-1) << SIPP_NP_OFFSET) | ((1<<(par->bpp[oBufIdx] >> 4)) << SIPP_FO_OFFSET));
    }

    // Output is always in CMX
    PROG_IO_BUFF(targetO, srcO);

    // Unsharp specific:
    SET_REG_WORD(paramSet + 0x00, param->frmDim);     //SIPP_SHARPEN_FRM_DIM_ADR
    SET_REG_WORD(paramSet + 0x04, param->cfg   );     //SIPP_SHARPEN_CFG_ADR
    SET_REG_WORD(paramSet + 0x08, param->strength);   //SIPP_SHARPEN_STREN_ADR
    SET_REG_WORD(paramSet + 0x0c, param->clip);       //SIPP_SHARPEN_CLIP_ADR
    SET_REG_WORD(paramSet + 0x10, param->limit);      //SIPP_SHARPEN_LIMIT_ADR
    SET_REG_WORD(paramSet + 0x14, param->rgnStop01);  //SIPP_SHARPEN_RANGETOP_1_0_ADR
    SET_REG_WORD(paramSet + 0x18, param->rgnStop23);  //SIPP_SHARPEN_RANGETOP_3_2_ADR

    // "If a kernel size smaller than the maximum is used,
    //  the corresponding filter coefficients should be programmed to zero."
    SET_REG_WORD(paramSet + 0x1C, param->coef01);     //SIPP_SHARPEN_GAUSIAN_1_0_ADR
    SET_REG_WORD(paramSet + 0x20, param->coef23);     //SIPP_SHARPEN_GAUSIAN_3_2_ADR
}

u32 sippGetIBufCtxSharpen (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (iBufIdx); //hush the compiler warning
    UNUSED (fptr);    //hush the compiler warning

    return SIPP_ICTX6_ADR;
}

u32 sippGetIBufIdsSharpen (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (iBufIdx); //hush the compiler warning

    // Set the bit pos of a sharpen input buffer increment
    return (0x1 << SIPP_SHARPEN_ID);
}

u32 sippGetOBufIdsSharpen (SippFilter *fptr, u32 oBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (oBufIdx); //hush the compiler warning

    // Set the bit pos of a sharpen filter output buffer interrupt
    return (0x1 << SIPP_SHARPEN_ID);
}

void sippSetBufLatenciesSharpen (SippFilter *fptr)
{
    // Fill in the latencies of each input and output buffer
    if (fptr->iBufCtx[0]) fptr->iBufCtx[0]->consumptionLatency = 0;
    fptr->OBufProdLatency[0]                                   = 0;
}

void sippSetOBufLevelsSharpen (SippFilter *fptr, eSippObufControl eOBufSetting)
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

        SET_REG_WORD(O_FC(SIPP_SHARPEN_ID), (1<<31) | level);
    }
}


#endif
