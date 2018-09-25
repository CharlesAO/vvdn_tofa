///
/// @file DrvSipp.c
/// 
/// 
/// @date Jul 22, 2014
/// @ingroup DrvSipp
/// @{
///


#include "DrvSipp.h"
#include <registersMyriad.h>
#include <DrvRegUtils.h>
#include <DrvCpr.h>
#include <DrvTimer.h>


//======================================================================================
//==========  THIS IS A DRIVER FOR SIPP MIPI ONLY, NOT FOR GENERAL SIPP USAGE! =========
//======================================================================================

void DrvSippInit(u32 device)
{
    DrvCprSysDeviceAction(SIPP_DOMAIN, ASSERT_RESET,  device); //this is a soft reset only
    DrvCprSysDeviceAction(SIPP_DOMAIN, ENABLE_CLKS, DEV_MSS_SIPP);
    DrvCprSysDeviceAction(SIPP_DOMAIN, DEASSERT_RESET,  device);

    return;
}

void DrvSippReset(u32 device)
{
   DrvCprSysDeviceAction(SIPP_DOMAIN, PULSE_RESET,  device); //this is a soft reset only

   u32 data  = GET_REG_WORD_VAL(SIPP_CONTROL_ADR);
   data &= ~device;
   SET_REG_WORD(SIPP_CONTROL_ADR, data);

   return;
}

void DrvSippEnableRxFilter(u32 filterNr)
{
    u32 data  = GET_REG_WORD_VAL(SIPP_CONTROL_ADR);

    data |= 1 << (filterNr + SIPP_MIPI_RX0_ID);
    SET_REG_WORD(SIPP_CONTROL_ADR, data);

   return;
}

void DrvSippDisableRxFilter(u32 filterNr)
{
   u32 data  = GET_REG_WORD_VAL(SIPP_CONTROL_ADR);
   data &= ~(1 << (filterNr + SIPP_MIPI_RX0_ID));
   SET_REG_WORD(SIPP_CONTROL_ADR, data);
}

void DrvSippEnableTxFilter(u32 filterNr)
{
    u32 data  = GET_REG_WORD_VAL(SIPP_CONTROL_ADR);

    data |= 1 << (filterNr + SIPP_MIPI_TX0_ID);
    SET_REG_WORD(SIPP_CONTROL_ADR, data);

   return;
}

void DrvSippEnableRxFilterInterrupt2(u32 filterNr)
{
    u32 data  = GET_REG_WORD_VAL(SIPP_INT2_ENABLE_ADR);

    data |= 1 << (filterNr + SIPP_MIPI_RX0_ID);
    SET_REG_WORD(SIPP_INT2_ENABLE_ADR, data);

   return;
}

void DrvSippEnableTxFilterInterrupt2(u32 filterNr)
{
    u32 data  = GET_REG_WORD_VAL(SIPP_INT2_ENABLE_ADR);

    data |= 1 << (filterNr + SIPP_MIPI_TX0_ID);
    SET_REG_WORD(SIPP_INT2_ENABLE_ADR, data);

   return;
}

void DrvSippCfgTxFilterInterrupt2(u32 filterNr)
{
    u32 data  = GET_REG_WORD_VAL(SIPP_INT2_CFG_ADR);

    data |= 1 << (filterNr + SIPP_MIPI_TX0_ID);
    SET_REG_WORD(SIPP_INT2_CFG_ADR, data);

    return;
}

void DrvSippConfigureMipiRxBuffers(u32 filterNr, sipp_buf_t* settings)
{
    sipp_buf_t* outBuff;

    switch(filterNr)
    {
    case 0: outBuff = (sipp_buf_t *)SIPP_MIPI_RX0_BUF_BASE_ADR;
            break;
    case 1: outBuff = (sipp_buf_t *)SIPP_MIPI_RX1_BUF_BASE_ADR;
            break;
    case 2: outBuff = (sipp_buf_t *)SIPP_MIPI_RX2_BUF_BASE_ADR;
            break;
    case 3: outBuff = (sipp_buf_t *)SIPP_MIPI_RX3_BUF_BASE_ADR;
            break;
    default:
            return;
    }

    outBuff->base         = settings->base;
    outBuff->cfg          = settings->cfg;
    outBuff->context      = settings->context;
    outBuff->fill_control = settings->fill_control;
    outBuff->lcs          = settings->lcs;
    outBuff->line_stride  = settings->line_stride;
    outBuff->plane_stride = settings->plane_stride;

   return;
}

void DrvSippConfigureMipiTxBuffers(u32 filterNr, sipp_buf_t* settings)
{
    sipp_buf_t* inBuff;

    switch(filterNr)
    {
    case 0: inBuff = (sipp_buf_t *)SIPP_MIPI_TX0_BUF_BASE_ADR;
            break;
    case 1: inBuff = (sipp_buf_t *)SIPP_MIPI_TX1_BUF_BASE_ADR;
            break;
    default:return;
    }

    inBuff->base         = settings->base;
    inBuff->cfg          = settings->cfg;
    inBuff->context      = settings->context;
    inBuff->fill_control = settings->fill_control;
    inBuff->lcs          = settings->lcs;
    inBuff->line_stride  = settings->line_stride;
    inBuff->plane_stride = settings->plane_stride;

   return;
}

void DrvSippConfigureMipiRx(u32 filterNr, sipp_rx_t* settings)
{
    sipp_rx_t *rxConf;

    switch(filterNr)
    {
    case 0: rxConf = (sipp_rx_t *)SIPP_MIPI_RX0_FRM_DIM_ADR;
            break;
    case 1: rxConf = (sipp_rx_t *)SIPP_MIPI_RX1_FRM_DIM_ADR;
            break;
    case 2: rxConf = (sipp_rx_t *)SIPP_MIPI_RX2_FRM_DIM_ADR;
            break;
    case 3: rxConf = (sipp_rx_t *)SIPP_MIPI_RX3_FRM_DIM_ADR;
            break;
    default:
            return;
    }

    rxConf->cfg     = settings->cfg;
    rxConf->frm_dim = settings->frm_dim;
    rxConf->black01 = settings->black01;
    rxConf->black23 = settings->black23;
    rxConf->mask0   = settings->mask0;
    rxConf->mask1   = settings->mask1;
    rxConf->mask2   = settings->mask2;
    rxConf->mask3   = settings->mask3;
    rxConf->sel01   = settings->sel01;
    rxConf->sel23   = settings->sel23;
    rxConf->vbp     = settings->vbp;
    rxConf->x0      = settings->x0;
    rxConf->x1      = settings->x1;
    rxConf->x2      = settings->x2;
    rxConf->x3      = settings->x3;
    rxConf->y0      = settings->y0;
    rxConf->y1      = settings->y1;
    rxConf->y2      = settings->y2;
    rxConf->y3      = settings->y3;

   return;
}

void DrvSippConfigureMipiTx(u32 filterNr, sipp_tx_t* settings)
{
    sipp_tx_t *txConf;

    switch(filterNr)
    {
    case 0: txConf = (sipp_tx_t *)SIPP_MIPI_TX0_FRM_DIM_ADR;
            break;
    case 1: txConf = (sipp_tx_t *)SIPP_MIPI_TX1_FRM_DIM_ADR;
            break;
    default:return;
    }

    txConf->cfg                     = settings->cfg;
    txConf->frm_dim                 = settings->frm_dim;
    txConf->h_activewidth           = settings->h_activewidth;
    txConf->h_backporch             = settings->h_backporch;
    txConf->h_frontporch            = settings->h_frontporch;
    txConf->hsync_width             = settings->hsync_width;
    txConf->line_comp               = settings->line_comp;
    txConf->line_count              = settings->line_count;
    txConf->v_activeheight          = settings->v_activeheight;
    txConf->v_activeheight_even     = settings->v_activeheight_even;
    txConf->v_backporch             = settings->v_backporch;
    txConf->v_backporch_even        = settings->v_backporch_even;
    txConf->v_frontporch            = settings->v_frontporch;
    txConf->v_frontporch_even       = settings->v_frontporch_even;
    txConf->vcomp                   = settings->vcomp;
    txConf->vstatus                 = settings->vstatus;
    txConf->vsync_end_offset        = settings->vsync_end_offset;
    txConf->vsync_end_offset_even   = settings->vsync_end_offset_even;
    txConf->vsync_start_offset      = settings->vsync_start_offset;
    txConf->vsync_start_offset_even = settings->vsync_start_offset_even;
    txConf->vsync_width             = settings->vsync_width;
    txConf->vsync_width_even        = settings->vsync_width_even;

   return;
}

void DrvSippStartTx(u32 filterNr)
{
    sipp_tx_t *txConf;

    switch(filterNr)
    {
    case 0: txConf = (sipp_tx_t *)SIPP_MIPI_TX0_FRM_DIM_ADR;
            break;
    case 1: txConf = (sipp_tx_t *)SIPP_MIPI_TX1_FRM_DIM_ADR;
            break;
    default:return;
    }

   txConf->timing_gen_trig = 0x00000001;

   return;
}

///@}

