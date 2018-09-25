///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

#include <sipp.h>
#include <sippInternal.h>

// Edge Operator filter is a HW filter, but not an oPipe filter

#if (defined(SIPP_PC) || defined(MYRIAD2))

//#######################################################################################
//This does NOT touch HW so can safely be called from a pipeline session not active on HW

u32 sippInitEdgeOp (SippFilter *fptr)
{
    EdgeParam * param  = (EdgeParam*)fptr->params;
    u32         retVal = 0;

    retVal  = sippIbufSetup (fptr, 0);
    retVal += sippObufSetup (fptr, 0);

    // One time constants updates
    param->frmDim = (fptr->parents[0]->outputH << SIPP_IMGDIM_SIZE)|
                     fptr->parents[0]->outputW;

    return retVal;
}

//#######################################################################################
// Clearly this is a HW session only function
void sippLoadEdgeOp (SippFilter *fptr)
{
    SippHwBuf *targetI;
    SippHwBuf *targetO;
    u32     paramSet;
    SippHwBuf *srcI  = fptr->iBuf[0];
    SippHwBuf *srcO  = fptr->oBuf[0];
    EdgeParam *param = (EdgeParam*)fptr->params;

    //shadow regs are gone...
    targetI = (SippHwBuf*)I_BASE(SIPP_EDGE_OP_ID);
    targetO = (SippHwBuf*)O_BASE(SIPP_EDGE_OP_ID);
    paramSet = SIPP_EDGE_OP_BASE_ADR;

    //Program I & O buffers
    PROG_IO_BUFF(targetI, srcI);
    PROG_IO_BUFF(targetO, srcO);

    //Edge Operator specific params
    SET_REG_WORD(paramSet + 0x00, param->frmDim);   //SIPP_EDGE_OP_FRM_DIM_ADR
    SET_REG_WORD(paramSet + 0x04, param->cfg   );   //SIPP_EDGE_OP_CFG_ADR
    SET_REG_WORD(paramSet + 0x08, param->xCoeff);   //SIPP_EDGE_OP_XCOEFF_ADR
    SET_REG_WORD(paramSet + 0x0C, param->yCoeff);   //SIPP_EDGE_OP_YCOEFF_ADR
}

u32 sippGetIBufCtxEdgeOp (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (iBufIdx); //hush the compiler warning
    UNUSED (fptr);    //hush the compiler warning

    return SIPP_ICTX12_ADR;
}

u32 sippGetIBufIdsEdgeOp (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (iBufIdx); //hush the compiler warning

    // Set the bit pos of a edge op input buffer increment
    return (0x1 << SIPP_EDGE_OP_ID);
}

u32 sippGetOBufIdsEdgeOp (SippFilter *fptr, u32 oBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (oBufIdx); //hush the compiler warning

    // Set the bit pos of a edge op filter output buffer interrupt
    return (0x1 << SIPP_EDGE_OP_ID);
}

void sippSetBufLatenciesEdgeOp (SippFilter *fptr)
{
    // Fill in the latencies of each input and output buffer
    if (fptr->iBufCtx[0]) fptr->iBufCtx[0]->consumptionLatency = 0;
    fptr->OBufProdLatency[0]                                   = 0;
}

void sippSetOBufLevelsEdgeOp (SippFilter *fptr, eSippObufControl eOBufSetting)
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

        SET_REG_WORD(O_FC(SIPP_EDGE_OP_ID), (1<<31) | level);
    }
}


#endif
