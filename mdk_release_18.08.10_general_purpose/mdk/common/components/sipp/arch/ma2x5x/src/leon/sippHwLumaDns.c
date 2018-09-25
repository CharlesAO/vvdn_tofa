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
u32 sippInitLumaDns(SippFilter *fptr)
{
    YDnsParam * param  = (YDnsParam*)fptr->params;
    u32         retVal = 0;

    retVal  = sippIbufSetup(fptr, 0);
    retVal += sippObufSetup(fptr, 0);

    // One time constants updates
    param->frmDim = (fptr->parents[0]->outputH << SIPP_IMGDIM_SIZE)|fptr->parents[0]->outputW;

    return retVal;
}

//#######################################################################################
//clearly this is a HW session only function

void sippLoadLumaDns (SippFilter *fptr)
{
    SippHwBuf *targetI;
    SippHwBuf *targetO;
    u32     paramSet, data;
    SippHwBuf *srcI  = fptr->iBuf[0];
    SippHwBuf *srcO  = fptr->oBuf[0];
    YDnsParam *param = (YDnsParam*)fptr->params;

    targetI  = (SippHwBuf*)I_BASE(SIPP_LUMA_ID);
    targetO  = (SippHwBuf*)O_BASE(SIPP_LUMA_ID);
    paramSet = SIPP_LUMA_BASE_ADR;

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

    /////////////////////////////////////////////////////////////
    // Filter specific setup

    // Setup Cosine 4th law LUT
    #ifdef SIPP_PC
    if (param->distCfg)
    #endif
    SET_REG_WORD(I_BASE(SIPP_LUMA_C4LUT_ID), param->distCfg);

    //Custom params:
    SET_REG_WORD(paramSet + 0x00, param->frmDim     );  //SIPP_LUMA_FRM_DIM_ADR
    SET_REG_WORD(paramSet + 0x04, param->cfg        );  //SIPP_LUMA_CFG_ADR

    //Initial load of distance-lut
    if (param->distCfg)
    {
    	data = GET_REG_WORD_VAL(SIPP_LUMA_CFG_ADR);
        SET_REG_WORD(SIPP_LUMA_CFG_ADR, data & (~(1<<24)));
        SET_REG_WORD(SIPP_LUMA_CFG_ADR, data | (  1<<24) );
    }

    SET_REG_WORD(paramSet + 0x08, param->gaussLut[0]);  //SIPP_LUMA_LUT0700_ADR
    SET_REG_WORD(paramSet + 0x0c, param->gaussLut[1]);  //SIPP_LUMA_LUT1508_ADR
    SET_REG_WORD(paramSet + 0x10, param->gaussLut[2]);  //SIPP_LUMA_LUT2316_ADR
    SET_REG_WORD(paramSet + 0x14, param->gaussLut[3]);  //SIPP_LUMA_LUT3124_ADR
    SET_REG_WORD(paramSet + 0x18, param->f2         );  //SIPP_LUMA_F2LUT_ADR
    SET_REG_WORD(paramSet + 0x1C, param->gammaLut[0]);  //SIPP_LUMA_GAMMA_LUT0_ADR
    SET_REG_WORD(paramSet + 0x20, param->gammaLut[1]);  //SIPP_LUMA_GAMMA_LUT1_ADR
    SET_REG_WORD(paramSet + 0x24, param->gammaLut[2]);  //SIPP_LUMA_GAMMA_LUT2_ADR
    SET_REG_WORD(paramSet + 0x28, param->gammaLut[3]);  //SIPP_LUMA_GAMMA_LUT3_ADR
    SET_REG_WORD(paramSet + 0x2C, param->gammaLut[4]);  //SIPP_LUMA_GAMMA_LUT4_ADR
    SET_REG_WORD(paramSet + 0x30, 0x0);                 //SIPP_LUMA_GAMMA_LUT4_ADR
    SET_REG_WORD(paramSet + 0x34, param->fullFrmDim);   //SIPP_FULL_FRM_DIM
}

u32 sippGetIBufCtxLumDns (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (iBufIdx); //hush the compiler warning
    UNUSED (fptr);    //hush the compiler warning

    return SIPP_ICTX5_ADR;
}

u32 sippGetIBufIdsLumDns (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (iBufIdx); //hush the compiler warning

    // Set the bit pos of a luma denoise filter input buffer increment
    return (0x1 << SIPP_LUMA_ID);
}

u32 sippGetOBufIdsLumDns (SippFilter *fptr, u32 oBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (oBufIdx); //hush the compiler warning

    // Set the bit pos of a luma denoise filter output buffer interrupt
    return (0x1 << SIPP_LUMA_ID);
}

void sippSetBufLatenciesLuma (SippFilter *fptr)
{
    // Fill in the latencies of each input and output buffer
    if (fptr->iBufCtx[0]) fptr->iBufCtx[0]->consumptionLatency = 0;
    fptr->OBufProdLatency[0]                                   = 0;
}

void sippSetOBufLevelsLuma (SippFilter *fptr, eSippObufControl eOBufSetting)
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

        SET_REG_WORD(O_FC(SIPP_LUMA_ID), (1<<31) | level);
    }
}

#endif
