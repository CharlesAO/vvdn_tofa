///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

#include <sipp.h>
#include <sippInternal.h>

// Convolution filter is a HW filter, but not an oPipie filter


#if (defined(SIPP_PC) || defined(MYRIAD2))

//#######################################################################################
//This does NOT touch HW so can safely be called from a pipeline session not active on HW
u32 sippInitConv (SippFilter *fptr)
{
    u32          retVal = 0;
    ConvParam  * param  = (ConvParam*)fptr->params;

    retVal  = sippIbufSetup (fptr, 0);
    retVal += sippObufSetup (fptr, 0);

    //One time constants updates
    param->frmDim = (fptr->parents[0]->outputH << SIPP_IMGDIM_SIZE)|fptr->parents[0]->outputW;

    return retVal;
}

//#######################################################################################
//Filter load is optimized for context-switch: it just copies packed data to destination
//reg sets. Previous approach derived params at load-time; such approach was appropriate
//for single-context case, where the Load was a SETUP-element, not a RUNTIME one
//#######################################################################################
// Clearly this is a HW session only function

void sippLoadConv (SippFilter *fptr)
{
    SippHwBuf *targetI;
    SippHwBuf *targetO;
    u32     paramSet;
    SippHwBuf *srcI  = fptr->iBuf[0];
    SippHwBuf *srcO  = fptr->oBuf[0];
    ConvParam *param = (ConvParam*)fptr->params;

    targetI = (SippHwBuf*)I_BASE(SIPP_CONV_ID);
    targetO = (SippHwBuf*)O_BASE(SIPP_CONV_ID);
    paramSet = SIPP_CONV_BASE_ADR;

    //Program I & O buffers
    PROG_IO_BUFF(targetI, srcI);
    PROG_IO_BUFF(targetO, srcO);

    //Convolution specific params
    SET_REG_WORD(paramSet + 0x00,     param->frmDim);     //SIPP_CONV_FRM_DIM_ADR
    SET_REG_WORD(paramSet + 0x04,     param->cfg);        //SIPP_CONV_CFG_ADR

    //The 5x5 coefs matrix
    //(need to set all params, if the matrix is 3x3, it will be surrounded by zeros)
    SET_REG_WORD(paramSet + 0x10,     param->kernel[ 0]); //SIPP_CONV_COEFF_01_00_ADR
    SET_REG_WORD(paramSet + 0x14,     param->kernel[ 1]); //SIPP_CONV_COEFF_03_02_ADR
    SET_REG_WORD(paramSet + 0x18,     param->kernel[ 2]); //SIPP_CONV_COEFF_04_ADR
    SET_REG_WORD(paramSet + 0x1c,     param->kernel[ 3]); //SIPP_CONV_COEFF_11_10_ADR
    SET_REG_WORD(paramSet + 0x20,     param->kernel[ 4]); //SIPP_CONV_COEFF_13_12_ADR
    SET_REG_WORD(paramSet + 0x24,     param->kernel[ 5]); //SIPP_CONV_COEFF_14_ADR
    SET_REG_WORD(paramSet + 0x28,     param->kernel[ 6]); //SIPP_CONV_COEFF_21_20_ADR
    SET_REG_WORD(paramSet + 0x2c,     param->kernel[ 7]); //SIPP_CONV_COEFF_23_22_ADR
    SET_REG_WORD(paramSet + 0x30,     param->kernel[ 8]); //SIPP_CONV_COEFF_24_ADR
    SET_REG_WORD(paramSet + 0x34,     param->kernel[ 9]); //SIPP_CONV_COEFF_31_30_ADR
    SET_REG_WORD(paramSet + 0x38,     param->kernel[10]); //SIPP_CONV_COEFF_33_32_ADR
    SET_REG_WORD(paramSet + 0x3c,     param->kernel[11]); //SIPP_CONV_COEFF_34_ADR
    SET_REG_WORD(paramSet + 0x40,     param->kernel[12]); //SIPP_CONV_COEFF_41_40_ADR
    SET_REG_WORD(paramSet + 0x44,     param->kernel[13]); //SIPP_CONV_COEFF_43_42_ADR
    SET_REG_WORD(paramSet + 0x48,     param->kernel[14]); //SIPP_CONV_COEFF_44_ADR

    if (param->cfg & (0x1 << CONV_CFG_EVENODD_PIX_SHIFT))
    {
        paramSet = SIPP_CONV_SHADOW_ADR;
        SET_REG_WORD(paramSet + 0x10,     param->shadowKernel[ 0]);
        SET_REG_WORD(paramSet + 0x14,     param->shadowKernel[ 1]);
        SET_REG_WORD(paramSet + 0x18,     param->shadowKernel[ 2]);
        SET_REG_WORD(paramSet + 0x1c,     param->shadowKernel[ 3]);
        SET_REG_WORD(paramSet + 0x20,     param->shadowKernel[ 4]);
        SET_REG_WORD(paramSet + 0x24,     param->shadowKernel[ 5]);
        SET_REG_WORD(paramSet + 0x28,     param->shadowKernel[ 6]);
        SET_REG_WORD(paramSet + 0x2c,     param->shadowKernel[ 7]);
        SET_REG_WORD(paramSet + 0x30,     param->shadowKernel[ 8]);
        SET_REG_WORD(paramSet + 0x34,     param->shadowKernel[ 9]);
        SET_REG_WORD(paramSet + 0x38,     param->shadowKernel[10]);
        SET_REG_WORD(paramSet + 0x3c,     param->shadowKernel[11]);
        SET_REG_WORD(paramSet + 0x40,     param->shadowKernel[12]);
        SET_REG_WORD(paramSet + 0x44,     param->shadowKernel[13]);
        SET_REG_WORD(paramSet + 0x48,     param->shadowKernel[14]);
    }
}

u32 sippGetIBufCtxConv (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (iBufIdx); //hush the compiler warning
    UNUSED (fptr);    //hush the compiler warning

    return SIPP_ICTX13_ADR;
}

u32 sippGetIBufIdsConv (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (iBufIdx); //hush the compiler warning

    // Set the bit pos of a convolution input buffer increment
    return (0x1 << SIPP_CONV_ID);
}

u32 sippGetOBufIdsConv (SippFilter *fptr, u32 oBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (oBufIdx); //hush the compiler warning

    // Set the bit pos of a convolution filter output buffer interrupt
    return (0x1 << SIPP_CONV_ID);
}

void sippSetBufLatenciesConv (SippFilter *fptr)
{
    // Fill in the latencies of each input and output buffer
    if (fptr->iBufCtx[0]) fptr->iBufCtx[0]->consumptionLatency = 0;
    fptr->OBufProdLatency[0]                                   = 0;
}

void sippSetOBufLevelsConv (SippFilter *fptr, eSippObufControl eOBufSetting)
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

        SET_REG_WORD(O_FC(SIPP_CONV_ID), (1<<31) | level);
    }
}


#endif
