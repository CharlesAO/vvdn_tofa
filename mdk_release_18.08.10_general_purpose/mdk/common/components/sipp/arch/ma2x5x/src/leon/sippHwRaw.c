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

u32 sippInitRaw (SippFilter * fptr)
{
    RawParam * param  = (RawParam *)fptr->params;
    u32        retVal = 0;

    retVal  = sippIbufSetup (fptr, 0);
    retVal += sippObufSetup (fptr, 0);

    // One time constants updates
    param->frmDim = (fptr->parents[0]->outputH << SIPP_IMGDIM_SIZE)|fptr->parents[0]->outputW;

    return retVal;
}

//#######################################################################################
// Clearly this is a HW session only function

void sippLoadRaw (SippFilter *fptr)
{
    SippHwBuf * targetI;
    SippHwBuf * targetO;
    SippHwBuf * srcI  = fptr->iBuf[0];
    SippHwBuf * srcO  = fptr->oBuf[0];
    RawParam  * param = (RawParam*)fptr->params;
    u32         gainSat[4];

    targetI  = (SippHwBuf*)I_BASE(SIPP_RAW_ID);
    targetO  = (SippHwBuf*)O_BASE(SIPP_RAW_ID);

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

    if (srcO)
    {
        PROG_IO_BUFF(targetO, srcO);
    }
    else
    {
        // Still need to set up planes for an oPipe setup
        SET_REG_WORD((u32)&targetO->cfg, ((fptr->nPlanes[0]-1) << SIPP_NP_OFFSET));
    }

    // RAW specific params
    SET_REG_WORD(SIPP_RAW_FRM_DIM_ADR, param->frmDim);
    SET_REG_WORD(SIPP_RAW_CFG_ADR, param->cfg);
    SET_REG_WORD(SIPP_GRGB_PLATO_ADR, param->grgbPlat);
    SET_REG_WORD(SIPP_GRGB_SLOPE_ADR, param->grgbDecay);
    SET_REG_WORD(SIPP_BAD_PIXEL_CFG_ADR, param->badPixCfg);

    SET_REG_WORD(SIPP_RAW_STATS_PLANES_ADR, param->statsPlanes);
    SET_REG_WORD(SIPP_STATS_FRM_DIM_ADR, param->statsFrmDim);

    if ((param->cfg >> RAW_CFG_FORMAT_SHIFT) & RAW_CFG_FORMAT_MASK)
    {
        sippUtilOrderPixels (((param->cfg >> RAW_CFG_BAYER_PATTERN_SHIFT ) & RAW_CFG_BAYER_PATTERN_MASK),
                             param->gainSat[0],
                             param->gainSat[1],
                             param->gainSat[2],
                             param->gainSat[3],
                             gainSat);
    }
    else
    {
        gainSat[0] = param->gainSat[0]; gainSat[1] = param->gainSat[1]; gainSat[2] = param->gainSat[2]; gainSat[3] = param->gainSat[3];
    }

    SET_REG_WORD(SIPP_GAIN_SATURATE_0_ADR, gainSat[0]);
    SET_REG_WORD(SIPP_GAIN_SATURATE_1_ADR, gainSat[1]);
    SET_REG_WORD(SIPP_GAIN_SATURATE_2_ADR, gainSat[2]);
    SET_REG_WORD(SIPP_GAIN_SATURATE_3_ADR, gainSat[3]);


    // see line 823 registersMyriad2v2.h
    // Only base address and config regs are defined for input buffers [26:23]
    // which are
    // Following are output buffer IDs
    // #define SIPP_AF_STATS_ID   24  // RAW AF statistics
    // #define SIPP_LUMA_HIST_ID  25  // RAW Luma histogram
    // #define SIPP_RGB_HIST_ID   26  // RAW RGB histogram

    if (param->cfg & RAW_CFG_AWB_STATS_EN(1))
    {
        SET_REG_WORD(STATS_AE_BASE_ADR, (u32)param->statsBase);
        SET_REG_WORD(SIPP_STATS_PATCH_CFG_ADR, param->statsPatchCfg);
        SET_REG_WORD(SIPP_STATS_PATCH_START_ADR, param->statsPatchStart);
        SET_REG_WORD(SIPP_STATS_PATCH_SKIP_ADR, param->statsPatchSkip);
        SET_REG_WORD(SIPP_RAW_STATS_THRESHOLDS_ADR, param->statsThresh);
    }

    //========================================================================================
    // Auto Focus
    if (param->cfg & RAW_CFG_AF_STATS_EN(1))
    {
        u32 cf;

        for(cf = 0; cf <= 10; cf++) //11 coefs
        {
            SET_REG_WORD(SIPP_AF_F1_COEF_00_ADR + (cf << 0x2), param->afF1coefs[cf]);
            SET_REG_WORD(SIPP_AF_F2_COEF_00_ADR + (cf << 0x2), param->afF2coefs[cf]);
        }

        SET_REG_WORD(SIPP_AF_MIN_THRESHOLD_ADR, param->afMinThresh);
        SET_REG_WORD(SIPP_AF_SUBTRACT_ADR, param->afSubtract);

        // Config patches
        SET_REG_WORD(SIPP_AF_PATCH_CFG_ADR, param->afPatchCfg);
        SET_REG_WORD(SIPP_AF_PATCH_START_ADR, param->afPatchStart);

        SET_REG_WORD(STATS_AF_BASE_ADR, (u32)param->afStatsBase);
    }

    if (param->cfg & RAW_CFG_LUMA_HIST_EN(1))
    {
        // Do the stats buffers not need the
        // CFG, LS, PS or IR regissters set?
        SET_REG_WORD(STATS_HIST_LUMA_BASE_ADR, (u32)param->histLumaBase);
    }

    if (param->cfg & RAW_CFG_RGB_HIST_EN(1))
    {
        SET_REG_WORD(STATS_HIST_RGB_BASE_ADR, (u32)param->histRgbBase);
    }
}

u32 sippGetIBufCtxRaw (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (iBufIdx); //hush the compiler warning
    UNUSED (fptr);    //hush the compiler warning

    return SIPP_ICTX2_ADR;
}

u32 sippGetIBufIdsRaw (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (fptr);   //hush the compiler warning
    UNUSED (iBufIdx); //hush the compiler warning

    // Set the bit pos of a raw filter input buffer increment
    return (0x1 << SIPP_RAW_ID);
}

u32 sippGetOBufIdsRaw (SippFilter *fptr, u32 oBufIdx)
{
    UNUSED (fptr);   //hush the compiler warning
    UNUSED (oBufIdx); //hush the compiler warning

    // Set the bit pos of a raw filter output buffer interrupt
    return (0x1 << SIPP_RAW_ID);
}

void sippSetBufLatenciesRaw (SippFilter *fptr)
{
    // Fill in the latencies of each input and output buffer
    if (fptr->iBufCtx[0]) fptr->iBufCtx[0]->consumptionLatency = 0;
    fptr->OBufProdLatency[0]                                   = 0;
}

void sippSetOBufLevelsRaw (SippFilter *fptr, eSippObufControl eOBufSetting)
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

        SET_REG_WORD(O_FC(SIPP_RAW_ID), (1<<31) | level);
    }
}


#endif
