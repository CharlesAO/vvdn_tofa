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
u32 sippInitLut (SippFilter * fptr)
{
    LutParam * param  = (LutParam *)fptr->params;
    u32        retVal = 0;

    retVal  = sippIbufSetup (fptr, 0);
    retVal += sippObufSetup (fptr, 0);

    // One time constants updates
    param->frmDim = (fptr->parents[0]->outputH << SIPP_IMGDIM_SIZE)|fptr->parents[0]->outputW;

    return retVal;
}

//#######################################################################################
// Clearly this is a HW session only function
void sippLoadLut (SippFilter *fptr)
{
    SippHwBuf * targetI;
    SippHwBuf * targetO;
    u32         paramSet;
    u32         data;
    SippHwBuf * srcI  = fptr->iBuf[0];
    SippHwBuf * srcO  = fptr->oBuf[0];
    LutParam  * param = (LutParam*)fptr->params;

    targetI = (SippHwBuf*)I_BASE(SIPP_LUT_ID);
    targetO = (SippHwBuf*)O_BASE(SIPP_LUT_ID);
    paramSet = SIPP_LUT_BASE_ADR;

    // Program I & O buffers
    if (srcI)
    {
        // Patch for channel mode: set number of planes = 1
        // even if processes multiple logical planes; the single plane = multiple channels
        if (param->cfg & (1 << 1))
        {
           SippHwBuf  *iBuf = fptr->iBuf[0];

           iBuf->cfg &= ~(0xF << SIPP_NP_OFFSET); //clear old value, thus set nPlanes = 0+1
        }

        PROG_IO_BUFF(targetI, srcI);
    }
    else
    {
        // Still need to set up planes for an oPipe setup
        SippFilter * par     = fptr->parents[0];
        s32          oBufIdx = fptr->parentOBufIdx[0];

        // Patch for channel mode: set number of planes = 1
        // even if processes multiple logical planes; the single plane = multiple channels
        if (param->cfg & (1 << 1))
        {
            SET_REG_WORD((u32)&targetI->cfg, 0);
        }
        else
        {
            SET_REG_WORD((u32)&targetI->cfg, ((par->nPlanes[oBufIdx]-1) << SIPP_NP_OFFSET));
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

    //LUT specific params
    SET_REG_WORD(paramSet + 0x00,  param->frmDim);   //SIPP_LUT_FRM_DIM_ADR
    SET_REG_WORD(paramSet + 0x08,  param->sizeA);    //SIPP_LUT_SIZES7_0_ADR
    SET_REG_WORD(paramSet + 0x0C,  param->sizeB);    //SIPP_LUT_SIZES15_8_ADR

    // Program the new base (cfgs are programmed in advance as they're DB-ed)
    SET_REG_WORD(I_BASE(SIPP_LUT_LOAD_ID), (u32)param->lut);
    SET_REG_WORD(I_CFG(SIPP_LUT_LOAD_ID), (u32)param->lutFormat << SIPP_FO_OFFSET);

    // Must enable LUT before load !
    data = GET_REG_WORD_VAL(SIPP_CONTROL_ADR);
    SET_REG_WORD(SIPP_CONTROL_ADR, data | (1 << SIPP_LUT_ID));

    SET_REG_WORD(paramSet + 0x04,  param->cfg);      //SIPP_LUT_CFG_ADR

    // Always trigger LUT load
    data = GET_REG_WORD_VAL(SIPP_LUT_CFG_ADR);
    SET_REG_WORD(SIPP_LUT_CFG_ADR, data & (~(1<<14)));
    SET_REG_WORD(SIPP_LUT_CFG_ADR, data | (  1<<14) );

    if (param->cfg & (LUT_CFG_COLOR_CONVERSION_EN_MASK << LUT_CFG_COLOR_CONVERSION_EN_SHIFT))
    {
        SET_REG_WORD(SIPP_LUT_COEFFS1_ADR, param->mat[0] | (param->mat[1] << 16));
        SET_REG_WORD(SIPP_LUT_COEFFS2_ADR, param->mat[2] | (param->mat[3] << 16));
        SET_REG_WORD(SIPP_LUT_COEFFS3_ADR, param->mat[4] | (param->mat[5] << 16));
        SET_REG_WORD(SIPP_LUT_COEFFS4_ADR, param->mat[6] | (param->mat[7] << 16));
        SET_REG_WORD(SIPP_LUT_COEFFS5_ADR, param->mat[8] | (param->offset[0] << 16));
        SET_REG_WORD(SIPP_LUT_COEFFS6_ADR, param->offset[1] | (param->offset[2] << 16));
    }
}

u32 sippGetIBufCtxLut (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (iBufIdx); //hush the compiler warning
    UNUSED (fptr);    //hush the compiler warning

    return SIPP_ICTX11_ADR;

}

u32 sippGetIBufIdsLut (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (iBufIdx); //hush the compiler warning

    // Set the bit pos of a LUT input buffer increment
    return (0x1 << SIPP_LUT_ID);
}

u32 sippGetOBufIdsLut (SippFilter *fptr, u32 oBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (oBufIdx); //hush the compiler warning

    // Set the bit pos of a LUT filter output buffer interrupt
    return (0x1 << SIPP_LUT_ID);
}

void sippSetBufLatenciesLut (SippFilter *fptr)
{
    // Fill in the latencies of each input and output buffer
    if (fptr->iBufCtx[0]) fptr->iBufCtx[0]->consumptionLatency = 0;
    fptr->OBufProdLatency[0]                                   = 0;
    fptr->OBufProdLatency[1]                                   = 0;
}

void sippSetOBufLevelsLut (SippFilter *fptr, eSippObufControl eOBufSetting)
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

        SET_REG_WORD(O_FC(SIPP_LUT_ID), (1<<31) | level);
    }
}


#endif
