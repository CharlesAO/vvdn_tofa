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

//#####################################################################################
// This does NOT touch HW so can safely be called from a pipeline session not active on HW
// LSC has a single PARENT-FILTER, but 2 input buffers (one is the GAIN MAP)
// nParents = 1

u32 sippInitLsc (SippFilter *fptr)
{
    LscParam * param    = (LscParam *)fptr->params;
    u32        gmPlanes = fptr->nPlanes[0];
    u32        shift    = param->dataFormat; //1:Bayer, 0:Planar
    u32        retVal   = 0;

    // Integer only operations !
    param->fraction = (  ((65536*((param->gmHeight >> shift) - 1)) / ((fptr->outputH >> shift)- 1)) << 16)
                     |(  ((65536*((param->gmWidth  >> shift) - 1)) / ((fptr->outputW >> shift)- 1))      );

    //Compact Input buffer
    retVal = sippIbufSetup (fptr, 0);

    //The second input buffer doesn't map on filter definition,
    //so using some custom setup:
    fptr->iBuf[1] = (SippHwBuf *)sippMemAlloc (&fptr->pPipe->tHeapMCB,
                                               vPoolSchedule,
                                               sizeof(SippHwBuf));

    if (fptr->iBuf[1] == (SippHwBuf *)NULL)
    {
         retVal += 0x1;
    }
    else
    {
        fptr->iBuf[1]->base = (u32)param->gmBase;
        fptr->iBuf[1]->ls   = param->gmWidth * 2;                  // 16bit fixed point format 8.8
        fptr->iBuf[1]->ps   = param->gmWidth * param->gmHeight * 2;
        fptr->iBuf[1]->cfg  = param->gmHeight                      // number of lines
                              | ((gmPlanes-1) << SIPP_NP_OFFSET)   // number of planes
                              | (2            << SIPP_FO_OFFSET);  // 16bit fixed point format 8.8
    }

    // Compact Output buffer
    retVal += sippObufSetup (fptr, 0);

    // One time constants updates
    param->frmDim = (fptr->parents[0]->outputH << SIPP_IMGDIM_SIZE)| fptr->parents[0]->outputW;
    param->gmDim  = (param->gmHeight << SIPP_IMGDIM_SIZE)| param->gmWidth;
    param->cfg    = param->dataFormat | ((param->dataWidth - 1)<<4);

    return retVal;
}

//#####################################################################################
// Clearly this is a HW session only function
void sippLoadLsc (SippFilter *fptr)
{
    SippHwBuf *targetI1, *targetI2;
    SippHwBuf *targetO;
    u32     paramSet;
    SippHwBuf *srcI1 = fptr->iBuf[0]; //image
    SippHwBuf *srcI2 = fptr->iBuf[1]; //gain map
    SippHwBuf *srcO  = fptr->oBuf[0];
    LscParam  *param = (LscParam*)fptr->params;

    targetI1 = (SippHwBuf*)I_BASE(SIPP_LSC_ID);
    targetI2 = (SippHwBuf*)I_BASE(SIPP_LSC_GM_ID);
    targetO  = (SippHwBuf*)O_BASE(SIPP_LSC_ID);
    paramSet = SIPP_LSC_BASE_ADR;

    //Program I & O buffers
    srcI2->base = (u32)param->gmBase;//Internal update of gain mesh base just before programming regs

    if (srcI1)
    {
        PROG_IO_BUFF(targetI1, srcI1); //Input-Img
    }
    else
    {
        // Still need to set up planes for an oPipe setup
        SippFilter * par     = fptr->parents[0];
        s32          oBufIdx = fptr->parentOBufIdx[0];

        SET_REG_WORD((u32)&targetI1->cfg, ((par->nPlanes[oBufIdx]-1) << SIPP_NP_OFFSET));
    }

    PROG_IO_BUFF(targetI2, srcI2); //Input-Gain Map

    if (srcO)
    {
        PROG_IO_BUFF(targetO , srcO ); //Output
    }
    else
    {
        // Still need to set up planes for an oPipe setup
        SET_REG_WORD((u32)&targetO->cfg, ((fptr->nPlanes[0]-1) << SIPP_NP_OFFSET));
    }

    // Custom regs
    SET_REG_WORD(paramSet + 0x00, param->frmDim);   //SIPP_LSC_FRM_DIM_ADR
    SET_REG_WORD(paramSet + 0x04, param->fraction); //SIPP_LSC_FRACTION_ADR
    SET_REG_WORD(paramSet + 0x08, param->gmDim);    //SIPP_LSC_GM_DIM_ADR
    SET_REG_WORD(paramSet + 0x0C, param->cfg);      //SIPP_LSC_CFG_ADR
}

u32 sippGetIBufCtxLsc (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (iBufIdx); //hush the compiler warning
    UNUSED (fptr);    //hush the compiler warning

    return SIPP_ICTX1_ADR;
}

u32 sippGetIBufIdsLsc (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (iBufIdx); //hush the compiler warning

    // Set the bit pos of a lsc filter input buffer increment
    return (0x1 << SIPP_LSC_ID);
}

u32 sippGetOBufIdsLsc (SippFilter *fptr, u32 oBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (oBufIdx); //hush the compiler warning

    // Set the bit pos of a lsc filter output buffer interrupt
    return (0x1 << SIPP_LSC_ID);
}

void sippSetBufLatenciesLsc (SippFilter *fptr)
{
    // Fill in the latencies of each input and output buffer
    if (fptr->iBufCtx[0]) fptr->iBufCtx[0]->consumptionLatency = 0;
    fptr->OBufProdLatency[0]                                   = 0;
}

void sippSetOBufLevelsLsc (SippFilter *fptr, eSippObufControl eOBufSetting)
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

        SET_REG_WORD(O_FC(SIPP_LSC_ID), (1<<31) | level);
    }
}

#endif
