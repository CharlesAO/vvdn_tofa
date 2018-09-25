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
u32 sippInitHarris (SippFilter *fptr)
{
    HarrisParam * param = (HarrisParam*)fptr->params;
    u32           retVal;

    retVal  = sippIbufSetup (fptr, 0);
    retVal += sippObufSetup (fptr, 0);

    // One time constants updates
    param->frmDim = (fptr->parents[0]->outputH << SIPP_IMGDIM_SIZE)|fptr->parents[0]->outputW;

    return retVal;
}

//#######################################################################################
//clearly this is a HW session only function
void sippLoadHarris (SippFilter *fptr)
{
    SippHwBuf   *targetI;
    SippHwBuf   *targetO;
    u32          paramSet;
    SippHwBuf   *srcI  = fptr->iBuf[0];
    SippHwBuf   *srcO  = fptr->oBuf[0];
    HarrisParam *param = (HarrisParam*)fptr->params;

    targetI = (SippHwBuf*)I_BASE(SIPP_HARRIS_ID);
    targetO = (SippHwBuf*)O_BASE(SIPP_HARRIS_ID);
    paramSet = SIPP_HARRIS_BASE_ADR;

    //Program I & O buffers
    PROG_IO_BUFF(targetI, srcI);
    PROG_IO_BUFF(targetO, srcO);

    //Harris specific params
    u32 *kVal = (u32*)&param->kValue;
    SET_REG_WORD(paramSet + 0x00,     param->frmDim); //SIPP_HARRIS_FRM_DIM_ADR
    SET_REG_WORD(paramSet + 0x04,     param->cfg);    //SIPP_HARRIS_CFG_ADR
    SET_REG_WORD(paramSet + 0x08,     *kVal );        //SIPP_HARRIS_K_ADR
}

u32 sippGetIBufCtxHarris (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (iBufIdx); //hush the compiler warning
    UNUSED (fptr);    //hush the compiler warning

    return SIPP_ICTX14_ADR;
}

u32 sippGetIBufIdsHarris (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (iBufIdx); //hush the compiler warning

    // Set the bit pos of a Harris filter input buffer increment
    return (0x1 << SIPP_HARRIS_ID);
}

u32 sippGetOBufIdsHarris (SippFilter *fptr, u32 oBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (oBufIdx); //hush the compiler warning

    // Set the bit pos of a Harris filter output buffer interrupt
    return (0x1 << SIPP_HARRIS_ID);
}

void sippSetBufLatenciesHarris (SippFilter *fptr)
{
    // Fill in the latencies of each input and output buffer
    if (fptr->iBufCtx[0]) fptr->iBufCtx[0]->consumptionLatency = 0;
    fptr->OBufProdLatency[0]                                   = 0;
}

void sippSetOBufLevelsHarris (SippFilter *fptr, eSippObufControl eOBufSetting)
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

        SET_REG_WORD(O_FC(SIPP_HARRIS_ID), (1<<31) | level);
    }
}


#endif
