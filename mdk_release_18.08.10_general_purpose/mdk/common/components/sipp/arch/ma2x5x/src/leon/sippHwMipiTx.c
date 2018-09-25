///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

#include <sipp.h>
#include <sippInternal.h>

//#######################################################################################
//This does NOT touch HW so can safely be called from a pipeline session not active on HW
u32 sippInitMipiTx (SippFilter * fptr)
{
    MipiTxParam * param  = (MipiTxParam *)fptr->params;
    u32           retVal = 0;

    retVal = sippIbufSetup (fptr, 0);

    // One time constants updates
    param->frmDim = (fptr->parents[0]->outputH << SIPP_IMGDIM_SIZE)|fptr->parents[0]->outputW;

    return retVal;
}

//#######################################################################################
// Clearly this is a HW session only function

void sippLoadMipiTx (SippFilter * fptr)
{
    SippHwBuf *   targetI;
    UInt32        paramBase;
    SippHwBuf *   srcI    = fptr->iBuf[0];
    UInt32        curUnit = fptr->unit - SIPP_MIPI_TX0_ID;
    MipiTxParam * param   = (MipiTxParam*)fptr->params;

    // Figure out In-Buf and Config params addresses given that we have 2 distinct MIPI-TX filters
    targetI = (SippHwBuf*)(I_BASE(SIPP_MIPI_TX0_ID) + 0x1C * curUnit);

    paramBase = SIPP_MIPI_TX0_BASE_ADR + 0x80 * curUnit;

    // Program I buffer
    PROG_IO_BUFF(targetI, srcI);

    // Mipi-TX specific params
    SET_REG_WORD(paramBase + 0x00,     param->frmDim       ); //SIPP_MIPI_TX0_FRM_DIM_ADR
    SET_REG_WORD(paramBase + 0x04,     param->cfg          ); //SIPP_MIPI_TX0_CFG_ADR

    SET_REG_WORD(paramBase + 0x0c,     param->lineCompare  ); //SIPP_MIPI_TX0_LINE_COMP_ADR
    SET_REG_WORD(paramBase + 0x14,     param->vCompare     ); //SIPP_MIPI_TX0_VCOMP_ADR

    SET_REG_WORD(paramBase + 0x18,     param->hSyncWidth   ); //SIPP_MIPI_TX0_HSYNC_WIDTH_ADR
    SET_REG_WORD(paramBase + 0x1c,     param->hBackPorch   ); //SIPP_MIPI_TX0_H_BACKPORCH_ADR
    SET_REG_WORD(paramBase + 0x20,     param->hActiveWidth ); //SIPP_MIPI_TX0_H_ACTIVEWIDTH_ADR
    SET_REG_WORD(paramBase + 0x24,     param->hFrontPorch  ); //SIPP_MIPI_TX0_H_FRONTPORCH_ADR

    SET_REG_WORD(paramBase + 0x28,     param->vSyncWidth   ); //SIPP_MIPI_TX0_VSYNC_WIDTH_ADR
    SET_REG_WORD(paramBase + 0x2c,     param->vBackPorch   ); //SIPP_MIPI_TX0_V_BACKPORCH_ADR
    SET_REG_WORD(paramBase + 0x30,     param->vActiveHeight); //SIPP_MIPI_TX0_V_ACTIVEHEIGHT_ADR
    SET_REG_WORD(paramBase + 0x34,     param->vFrontPorch  ); //SIPP_MIPI_TX0_V_FRONTPORCH_ADR
    SET_REG_WORD(paramBase + 0x38,     param->vSyncStartOff); //SIPP_MIPI_TX0_VSYNC_START_OFFSET_ADR
    SET_REG_WORD(paramBase + 0x3c,     param->vSyncEndOff  ); //SIPP_MIPI_TX0_VSYNC_END_OFFSET_ADR
}

void sippSetBufLatenciesMipiTx (SippFilter * fptr)
{
    // Fill in the latencies of each input and output buffer
    if (fptr->iBufCtx[0]) fptr->iBufCtx[0]->consumptionLatency = 0;
}

u32 sippGetOBufIdsMipiTx0 (SippFilter *fptr, u32 oBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (oBufIdx); //hush the compiler warning

    return (0x1 << SIPP_MIPI_TX0_ID);
}

u32 sippGetOBufIdsMipiTx1 (SippFilter *fptr, u32 oBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (oBufIdx); //hush the compiler warning

    return (0x1 << SIPP_MIPI_TX1_ID);
}

void sippSetOBufLevelsMipiTx0 (SippFilter *fptr, eSippObufControl eOBufSetting)
{
    UNUSED (fptr);         //hush the compiler warning
    UNUSED (eOBufSetting); //hush the compiler warning
}

void sippSetOBufLevelsMipiTx1 (SippFilter *fptr, eSippObufControl eOBufSetting)
{
    UNUSED (fptr);         //hush the compiler warning
    UNUSED (eOBufSetting); //hush the compiler warning
}
