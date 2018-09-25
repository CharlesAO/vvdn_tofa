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

u32 sippInitDbyr (SippFilter *fptr)
{
    DbyrParam * param    = (DbyrParam*)fptr->params;
    u32         numOBufs = 0;
    u32         retVal   = 0;

    retVal = sippIbufSetup (fptr, 0); // Compact Input buffer

    if (param->thresh & DEBAYER_RGB_EN(0x1))
    {
        // Setup RGB output
        retVal += sippObufSetup (fptr, 0); // Compact Output buffer
        numOBufs++;
    }

    if (param->thresh & DEBAYER_LUMA_EN(0x1))
    {
        // Setup Luma output
        retVal += sippObufSetup (fptr, numOBufs); // Compact Output buffer
    }

    // One time constants updates
    param->frmDim = (fptr->parents[0]->outputH << SIPP_IMGDIM_SIZE)|fptr->parents[0]->outputW;

    return retVal;
}

//#######################################################################################
// Clearly this is a HW session only function

void sippLoadDbyr (SippFilter *fptr)
{
    SippHwBuf * targetI;
    SippHwBuf * targetORgb;
    SippHwBuf * targetOLuma;
    u32         paramSet;
    SippHwBuf * srcI     = fptr->iBuf[0];
    SippHwBuf * srcRgbO  = fptr->oBuf[0];
    SippHwBuf * srcLumaO;
    DbyrParam * param    = (DbyrParam*)fptr->params;
    u32         numOBufs = 0;

    targetI      = (SippHwBuf*)I_BASE(SIPP_DBYR_ID);
    targetORgb   = (SippHwBuf*)O_BASE(SIPP_DBYR_ID);
    targetOLuma  = (SippHwBuf*)O_BASE(SIPP_DBYR_LUMA_ID);

    paramSet = SIPP_DBYR_BASE_ADR;

    // Program I & O buffers
    if (srcI)
    {
        PROG_IO_BUFF(targetI, srcI);
    }
    else
    {
        // Still need to set up planes for an oPipe setup
        SippFilter * par     = fptr->parents[0];
        s32          oBufIdx = fptr->parentOBufIdx[0];

        SET_REG_WORD((u32)&targetI->cfg, ((par->nPlanes[oBufIdx]-1) << SIPP_NP_OFFSET));
    }

    if (param->thresh & DEBAYER_RGB_EN(0x1))
    {
        if (srcRgbO)
        {
            PROG_IO_BUFF(targetORgb, srcRgbO);
        }
        else
        {
            // Still need to set up planes for an oPipe setup
            SET_REG_WORD((u32)&targetORgb->cfg, ((fptr->parents[0]->nPlanes[0]-1) << SIPP_NP_OFFSET));
        }
        numOBufs++;
    }

    if (param->thresh & DEBAYER_LUMA_EN(0x1))
    {
        srcLumaO = fptr->oBuf[numOBufs];
        if (srcLumaO)
        {
            PROG_IO_BUFF(targetOLuma, srcLumaO);
        }
        else
        {
            // Still need to set up planes for an oPipe setup
            SET_REG_WORD((u32)&targetOLuma->cfg, ((fptr->nPlanes[1]-1) << SIPP_NP_OFFSET));
        }
    }

    //Debayer specific params
    SET_REG_WORD(paramSet + 0x00, param->frmDim);   //SIPP_DBYR_FRM_DIM_ADR
    SET_REG_WORD(paramSet + 0x04, param->cfg);      //SIPP_DBYR_CFG_ADR

    // For now, if LUMA output is enabled ensure the luma write client is enabled
    // In theory, if the DoGLTM was in DoG only mode and we were not alpha blending
    // in the chroma median filter thyis need not be the case
    if (param->thresh & (1<<26))
    {
        param->thresh |= (1<<27);
    }

    SET_REG_WORD(paramSet + 0x08, param->thresh);   //SIPP_DBYR_THRES_ADR
    SET_REG_WORD(paramSet + 0x10, param->dewormCfg);//SIPP_DBYR_DEWORM_ADR

    SET_REG_WORD(SIPP_DBYR_LUMA_COEFS_ADR, param->lumaWeight);
}

u32 sippGetIBufCtxDbyr (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (iBufIdx); //hush the compiler warning
    UNUSED (fptr);    //hush the compiler warning

    return SIPP_ICTX3_ADR;
}

u32 sippGetIBufIdsDbyr (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (iBufIdx); //hush the compiler warning

    // Set the bit pos of a debayer filter input buffer increment
    return (0x1 << SIPP_DBYR_ID);
}

u32 sippGetOBufIdsDbyr (SippFilter *fptr, u32 oBufIdx)
{
    u32         oBufIDMask = 0;
    DbyrParam * param      = (DbyrParam *)fptr->params;

    // Set the bit pos of a debayer filter output buffer interrupt(s)
    if (oBufIdx == 0)
    {
        if (param->thresh & DEBAYER_RGB_EN(0x1))
        {
            oBufIDMask |= (0x1 << SIPP_DBYR_ID);
        }
        else if (param->thresh & DEBAYER_LUMA_EN(0x1))
        {
            oBufIDMask |= (0x1 << SIPP_DBYR_LUMA_ID);
        }
    }

    if (oBufIdx == 1)
    {
        // If function is being asked about a 2nd Obuf, then 2 must be in use
        // so RGB will be buf 0. Check only luma here
        if (param->thresh & DEBAYER_LUMA_EN(0x1))
        {
            oBufIDMask |= (0x1 << SIPP_DBYR_LUMA_ID);
        }
    }

    return oBufIDMask;
}

void sippSetBufLatenciesDbyr (SippFilter *fptr)
{
    // Fill in the latencies of each input and output buffer
    if (fptr->iBufCtx[0]) fptr->iBufCtx[0]->consumptionLatency = 0;
    fptr->OBufProdLatency[0]                                   = 0;
    fptr->OBufProdLatency[1]                                   = 0;

}

void sippSetOBufLevelsDbyr (SippFilter *fptr, eSippObufControl eOBufSetting)
{
    u32 level = 0;
    u32 numLinesIter = fptr->linesPerIter;

    if (fptr->nLines[1])
    {
        switch (eOBufSetting)
        {
            case eSIPP_SET_OBUF_SPACE_FULL  : level = fptr->nLines[1]; break;
            case eSIPP_SET_OBUF_SPACE_EMPTY : level = 0; break;
            case eSIPP_SET_OBUF_SPACE_ITER  : level = fptr->nLines[1] - numLinesIter; break;
        }

        SET_REG_WORD(O_FC(SIPP_DBYR_LUMA_ID), (1<<31) | level);
    }

    if (fptr->nLines[0])
    {
        DbyrParam * param      = (DbyrParam *)fptr->params;

        switch (eOBufSetting)
        {
            case eSIPP_SET_OBUF_SPACE_FULL  : level = fptr->nLines[0]; break;
            case eSIPP_SET_OBUF_SPACE_EMPTY : level = 0; break;
            case eSIPP_SET_OBUF_SPACE_ITER  : level = fptr->nLines[0] - numLinesIter; break;
        }

        if (param->thresh & DEBAYER_RGB_EN(0x1))
        {
            SET_REG_WORD(O_FC(SIPP_DBYR_ID), (1<<31) | level);
        }
        else
        {
            SET_REG_WORD(O_FC(SIPP_DBYR_LUMA_ID), (1<<31) | level);
        }
    }
}
#endif
