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
u32 sippInitMipiRx (SippFilter * fptr)
{
    MipiRxParam * param  = (MipiRxParam *)fptr->params;
    u32           retVal = 0;

    // MipiRX has no input buffer
    retVal = sippObufSetup (fptr, 0x0);

    // One time constants updates
    param->frmDim = (fptr->outputH << SIPP_IMGDIM_SIZE)|fptr->outputW;

    return retVal;
}

//#######################################################################################
// Clearly this is a HW session only function

void sippLoadMipiRx (SippFilter * fptr)
{
    SippHwBuf *   targetO;
    UInt32        targetParam;
    SippHwBuf *   srcO    = fptr->oBuf[0x0];
    MipiRxParam * param   = (MipiRxParam *)fptr->params;
    UInt32        curUnit = fptr->unit - SIPP_MIPI_RX0_ID;

    // Figure out Out-Buf and Config params addresses given that we have 4 distinct MIPI-RX filters
    targetO     = (SippHwBuf *)(O_BASE(SIPP_MIPI_RX0_ID) + 0x1C * curUnit);

    targetParam = SIPP_MIPI_RX0_BASE_ADR + 0x80 * curUnit;

    // Program Out buffer
    if (srcO)
    {
        PROG_IO_BUFF(targetO, srcO);
    }
    else
    {
        // Still need to set up planes for an oPipe setup
        SET_REG_WORD((u32)&targetO->cfg, ((fptr->nPlanes[0]-1) << SIPP_NP_OFFSET));
    }

    // MipiRX specific params
    SET_REG_WORD(targetParam + 0x00,    param->frmDim);     //SIPP_MIPI_RX0_FRM_DIM_ADR
    SET_REG_WORD(targetParam + 0x04,    param->cfg);        //SIPP_MIPI_RX0_CFG_ADR

    SET_REG_WORD(targetParam + 0x08,    param->winX[0]);    //SIPP_MIPI_RX0_X0_ADR
    SET_REG_WORD(targetParam + 0x0c,    param->winX[1]);    //SIPP_MIPI_RX0_X1_ADR
    SET_REG_WORD(targetParam + 0x10,    param->winX[2]);    //SIPP_MIPI_RX0_X2_ADR
    SET_REG_WORD(targetParam + 0x14,    param->winX[3]);    //SIPP_MIPI_RX0_X3_ADR
    SET_REG_WORD(targetParam + 0x18,    param->winY[0]);    //SIPP_MIPI_RX0_Y0_ADR
    SET_REG_WORD(targetParam + 0x1c,    param->winY[1]);    //SIPP_MIPI_RX0_Y1_ADR
    SET_REG_WORD(targetParam + 0x20,    param->winY[2]);    //SIPP_MIPI_RX0_Y2_ADR
    SET_REG_WORD(targetParam + 0x24,    param->winY[3]);    //SIPP_MIPI_RX0_Y3_ADR
    SET_REG_WORD(targetParam + 0x28,    param->sel01  );    //SIPP_MIPI_RX0_SEL01_ADR
    SET_REG_WORD(targetParam + 0x2c,    param->sel23  );    //SIPP_MIPI_RX0_SEL23_ADR
    SET_REG_WORD(targetParam + 0x30,    param->selMask[0]); //SIPP_MIPI_RX0_MASK0_ADR
    SET_REG_WORD(targetParam + 0x34,    param->selMask[1]); //SIPP_MIPI_RX0_MASK1_ADR
    SET_REG_WORD(targetParam + 0x38,    param->selMask[2]); //SIPP_MIPI_RX0_MASK2_ADR
    SET_REG_WORD(targetParam + 0x3c,    param->selMask[3]); //SIPP_MIPI_RX0_MASK3_ADR
    SET_REG_WORD(targetParam + 0x40,    param->black01);    //SIPP_MIPI_RX0_BLACK01_ADR
    SET_REG_WORD(targetParam + 0x44,    param->black23);    //SIPP_MIPI_RX0_BLACK23_ADR
    SET_REG_WORD(targetParam + 0x48,    param->vbp);        //SIPP_MIPI_RX0_VBP_ADR
}

void sippSetBufLatenciesMipiRx (SippFilter *fptr)
{
    // Fill in the latencies of each input and output buffer
    fptr->OBufProdLatency[0] = 0;
}

u32 sippGetOBufIdsMipiRx0 (SippFilter *fptr, u32 oBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (oBufIdx); //hush the compiler warning

    return (0x1 << SIPP_MIPI_RX0_ID);
}

u32 sippGetOBufIdsMipiRx1 (SippFilter *fptr, u32 oBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (oBufIdx); //hush the compiler warning

    return (0x1 << SIPP_MIPI_RX1_ID);
}

u32 sippGetOBufIdsMipiRx2 (SippFilter *fptr, u32 oBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (oBufIdx); //hush the compiler warning

    return (0x1 << SIPP_MIPI_RX2_ID);
}

u32 sippGetOBufIdsMipiRx3 (SippFilter *fptr, u32 oBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (oBufIdx); //hush the compiler warning

    return (0x1 << SIPP_MIPI_RX3_ID);
}

void sippSetOBufLevelsMipiRx0 (SippFilter *fptr, eSippObufControl eOBufSetting)
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

        SET_REG_WORD(O_FC(SIPP_MIPI_RX0_ID), (1<<31) | level);
    }
}

void sippSetOBufLevelsMipiRx1 (SippFilter *fptr, eSippObufControl eOBufSetting)
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

        SET_REG_WORD(O_FC(SIPP_MIPI_RX1_ID), (1<<31) | level);
    }
}

void sippSetOBufLevelsMipiRx2 (SippFilter *fptr, eSippObufControl eOBufSetting)
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

        SET_REG_WORD(O_FC(SIPP_MIPI_RX2_ID), (1<<31) | level);
    }
}

void sippSetOBufLevelsMipiRx3 (SippFilter *fptr, eSippObufControl eOBufSetting)
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

        SET_REG_WORD(O_FC(SIPP_MIPI_RX3_ID), (1<<31) | level);
    }
}

