///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Lcd Functions.
///
/// This is the implementation of Lcd library.
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <assert.h>

#include "LcdApi.h"
#include "DrvLcd.h"
#include "DrvLcdDefines.h"
#include "DrvSvu.h"
#include "DrvIcbDefines.h"
#include "DrvIcb.h"
#include <DrvCpr.h>
#include <DrvRegUtils.h>

#ifdef LCD_BLEN8
#define LCD_BLEN_BURST  D_LCD_DMA_LAYER_AXI_BURST_8
#else
#define LCD_BLEN_BURST  D_LCD_DMA_LAYER_AXI_BURST_16
#endif

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
typedef struct
{
    unsigned int MXI_DISPX_BASE_ADR;
    unsigned int IRQ_LCD_X;
    unsigned int LCDX_INT_CLEAR_ADR;
} LCDRegisters;

// all yuv formats have bit 3 low
#define IS_LCD_OUTF_RGB(x) (((x)& D_LCD_OUTF_FORMAT_YCBCR420_8B_LEGACY)== 0)
// all yuv formats have bit 3 high
#define IS_LCD_OUTF_YUV(x) ((x)& D_LCD_OUTF_FORMAT_YCBCR420_8B_LEGACY)

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
//Conversion (yuv->rgb) matrix, see formula to understand
static const u32 csc_coef_lcd[N_COEFS] =
{
        1024, 0, 1436,
        1024, -352, -731,
        1024, 1814, 0,
        -179, 125, -226
};

static LCDHandle *LCDHndl[1] = { NULL};

static const LCDRegisters lcds[1] =
{
    {
        .MXI_DISPX_BASE_ADR = LCD1_BASE_ADR,
        #ifdef Use_LcdGeneric_on_LRT
        .IRQ_LCD_X          = IRQ_LCD,
        #else
        .IRQ_LCD_X          = ROUTED_IRQ_LCD,
        #endif
        .LCDX_INT_CLEAR_ADR = LCD1_INT_CLEAR_ADR
    }
};

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

// Function to assign callbacks
void LCDSetupCallbacks(LCDHandle *hndl,
        allocateLcdFn * assignFrame, frameReadyLcdFn *frameReady,
        freqLcdFn *highres, freqLcdFn *lowres)
{
    hndl->cbGetFrame = assignFrame;
    hndl->cbFrameReady = frameReady;
    hndl->callBackFctHighRes = highres;
    hndl->callBackFctLowRes = lowres;
}

static inline void LCDSetLayerBuffersNormalRegisters(LCDHandle *hndl,
        int layer)
{
    //Layer 0
    if (layer == VL1 && hndl->currentFrameVL1 != NULL)
    {
        if (hndl->currentFrameVL1->p1 != NULL)
            SET_REG_WORD(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER0_DMA_START_ADR_OFFSET,
                (unsigned int )hndl->currentFrameVL1->p1);
        if (hndl->currentFrameVL1->p2 != NULL)
            SET_REG_WORD(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER0_DMA_START_CB_ADR_OFFSET,
                (unsigned int )hndl->currentFrameVL1->p2);
        if (hndl->currentFrameVL1->p3 != NULL)
            SET_REG_WORD(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER0_DMA_START_CR_ADR_OFFSET,
                (unsigned int )hndl->currentFrameVL1->p3);
    }
    //Layer 1
    if (layer == VL2 && hndl->currentFrameVL2 != NULL)
    {
        if (hndl->currentFrameVL2->p1 != NULL)
            SET_REG_WORD(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER1_DMA_START_ADR_OFFSET,
                (unsigned int )hndl->currentFrameVL2->p1);
        if (hndl->currentFrameVL2->p2 != NULL)
            SET_REG_WORD(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER1_DMA_START_CB_ADR_OFFSET,
                (unsigned int )hndl->currentFrameVL2->p2);
        if (hndl->currentFrameVL2->p3 != NULL)
            SET_REG_WORD(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER1_DMA_START_CR_ADR_OFFSET,
                (unsigned int )hndl->currentFrameVL2->p3);
    }
    //Layer 2 GL1
    if (layer == GL1 && hndl->currentFrameGL1 != NULL)
        if (hndl->currentFrameGL1->p1 != NULL)
            SET_REG_WORD(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER2_DMA_START_ADR_OFFSET,
                (unsigned int )hndl->currentFrameGL1->p1);

    //Layer 3 GL2
    if (layer == GL2 && hndl->currentFrameGL2 != NULL)
        if (hndl->currentFrameGL2->p1 != NULL)
            SET_REG_WORD(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER3_DMA_START_ADR_OFFSET,
                (unsigned int )hndl->currentFrameGL2->p1);
}

static inline void LCDSetLayerBuffersShadowRegisters(
    LCDHandle *hndl, int layer)
{
    //Layer 0
    if (layer == VL1 && hndl->currentFrameVL1 != NULL)
    {
        if (hndl->currentFrameVL1->p1 != NULL)
            SET_REG_WORD(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER0_DMA_START_SHADOW_OFFSET,
                (unsigned int )hndl->currentFrameVL1->p1);
        if (hndl->currentFrameVL1->p2 != NULL)
            SET_REG_WORD(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER0_DMA_START_CB_SHADOW_OFFSET,
                (unsigned int )hndl->currentFrameVL1->p2);
        if (hndl->currentFrameVL1->p3 != NULL)
            SET_REG_WORD(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER0_DMA_START_CR_SHADOW_OFFSET,
                (unsigned int )hndl->currentFrameVL1->p3);
    }
    //Layer 1
    if (layer == VL2 && hndl->currentFrameVL2 != NULL)
    {
        if (hndl->currentFrameVL2->p1 != NULL)
            SET_REG_WORD(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER1_DMA_START_SHADOW_OFFSET,
                (unsigned int )hndl->currentFrameVL2->p1);
        if (hndl->currentFrameVL2->p2 != NULL)
            SET_REG_WORD(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER1_DMA_START_CB_SHADOW_OFFSET,
                (unsigned int )hndl->currentFrameVL2->p2);
        if (hndl->currentFrameVL2->p3 != NULL)
            SET_REG_WORD(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER1_DMA_START_CR_SHADOW_OFFSET,
                (unsigned int )hndl->currentFrameVL2->p3);
    }
    //Layer 2 GL1
    if (layer == GL1 && hndl->currentFrameGL1 != NULL)
        if (hndl->currentFrameGL1->p1 != NULL)
            SET_REG_WORD(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER2_DMA_START_SHADOW_OFFSET,
                (unsigned int )hndl->currentFrameGL1->p1);
    //Layer 3 GL2
    if (layer == GL2 && hndl->currentFrameGL2 != NULL)
        if (hndl->currentFrameGL2->p1 != NULL)
            SET_REG_WORD(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER3_DMA_START_SHADOW_OFFSET,
                (unsigned int )hndl->currentFrameGL2->p1);
}

static inline void LCDSetLayerBuffersNormalAndShadowRegisters(
    LCDHandle *hndl, int layer)
{
    LCDSetLayerBuffersNormalRegisters(hndl, layer);
    LCDSetLayerBuffersShadowRegisters(hndl, layer);
}

void DrvLcdCtrlNoTrig(u32 lcdBase, u32 background, u32 outFormat, u32 ctrl)
{
    SET_REG_WORD(lcdBase + LCD_BG_COLOUR_LS_OFFSET,      background);           //Background color:
    SET_REG_WORD(lcdBase + LCD_BG_COLOUR_MS_OFFSET,      0);                    // TBD need to update this
    SET_REG_WORD(lcdBase + LCD_OUT_FORMAT_CFG_OFFSET, outFormat);
    SET_REG_WORD(lcdBase + LCD_CONTROL_OFFSET             , ctrl);           //interlaced, RGB-out, enable Layer2
}

void LCDConfigureLayers(LCDHandle *hndl,
                        const LCDDisplayCfg *lcdsp, u32 dmaUpdateType, u32 lcdControlEdited)
{
    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int lutWidthCorrection = 1;

    unsigned int VL1Cfg = D_LCD_LAYER_FIFO_00;
    unsigned int VL2Cfg = D_LCD_LAYER_FIFO_00;

    unsigned int GL1Cfg = D_LCD_LAYER_ALPHA_EMBED | D_LCD_LAYER_8BPP
                          | D_LCD_LAYER_TRANSPARENT_EN | D_LCD_LAYER_FIFO_50;
    unsigned int GL2Cfg = D_LCD_LAYER_FIFO_50 | D_LCD_LAYER_8BPP
                          | D_LCD_LAYER_TRANSPARENT_EN;


    unsigned int LCtrl = 0/*D_LCD_DMA_LAYER_ENABLE*/| dmaUpdateType
                         | LCD_BLEN_BURST | D_LCD_DMA_LAYER_V_STRIDE_EN;

    static frameBuffer nullFrameBuffer = {{0,0,0,0,0}, NULL, NULL,NULL};
    lcdControlEdited |= D_LCD_CTRL_PIPELINE_DMA;
    u32 prevVal = GET_REG_WORD_VAL(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_CONTROL_OFFSET);
    DrvLcdCtrlNoTrig(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, 0x808000,
               lcdsp->outputFormat, prevVal | lcdControlEdited);
    // Never, ever disable pipelined (outstanding) DMA reads (if possible)
    if(hndl->currentFrameVL1 == NULL)
      hndl->currentFrameVL1 = &nullFrameBuffer;
    if(hndl->currentFrameVL2 == NULL)
      hndl->currentFrameVL2 = &nullFrameBuffer;
    if(hndl->currentFrameGL1 == NULL)
      hndl->currentFrameGL1 = &nullFrameBuffer;
    if(hndl->currentFrameGL2 == NULL)
      hndl->currentFrameGL2 = &nullFrameBuffer;

    if(hndl->lcdFrameSpec != NULL)
    {

        switch (hndl->lcdFrameSpec->type)
        {
        case YUV420p:
            VL1Cfg = VL1Cfg | D_LCD_LAYER_8BPP | D_LCD_LAYER_CRCB_ORDER
                     | D_LCD_LAYER_FORMAT_YCBCR420PLAN | D_LCD_LAYER_PLANAR_STORAGE;
            if(IS_LCD_OUTF_RGB(hndl->lcdSpec->outputFormat))
            {   // input format yuv, output format rgb, we need to enable color space conversion
            	VL1Cfg |= D_LCD_LAYER_CSC_EN;
            }
            LCtrl = LCtrl | D_LCD_DMA_LAYER_V_STRIDE_EN;
            break;
        case YUV422p:
            VL1Cfg = VL1Cfg | D_LCD_LAYER_8BPP | D_LCD_LAYER_CRCB_ORDER
                     | D_LCD_LAYER_FORMAT_YCBCR422PLAN | D_LCD_LAYER_PLANAR_STORAGE;
            if(IS_LCD_OUTF_RGB(hndl->lcdSpec->outputFormat))
            {   // input format yuv, output format rgb, we need to enable color space conversion
            	VL1Cfg |= D_LCD_LAYER_CSC_EN;
            }
            LCtrl = LCtrl | D_LCD_DMA_LAYER_V_STRIDE_EN;
            break;
        case YUV422i:
            VL1Cfg = VL1Cfg | D_LCD_LAYER_16BPP | D_LCD_LAYER_FORMAT_YCBCR422LIN;
            if(IS_LCD_OUTF_RGB(hndl->lcdSpec->outputFormat))
            {   // input format yuv, output format rgb, we need to enable color space conversion
            	VL1Cfg |= D_LCD_LAYER_CSC_EN;
            }
            break;
        case RGB888:
            VL1Cfg = VL1Cfg | D_LCD_LAYER_24BPP | D_LCD_LAYER_FORMAT_RGB888;
            LCtrl = LCtrl | D_LCD_CTRL_OUTPUT_RGB;
            break;
        case RAW16:
            VL1Cfg = VL1Cfg | D_LCD_LAYER_16BPP | D_LCD_LAYER_FORMAT_RGB565;
            LCtrl = LCtrl | D_LCD_CTRL_OUTPUT_RGB;
            break;
        case RGBA8888:
                    VL1Cfg = VL1Cfg | D_LCD_LAYER_32BPP | D_LCD_LAYER_FORMAT_RGBA8888;
                    LCtrl = LCtrl | D_LCD_CTRL_OUTPUT_RGB;
                    break;
        default:
            break;
        }

        ////////////////////////////////////////////////////////////////////////////////
        ///////////  VL1
        ////////////////////////////////////////////////////////////////////////////////
        if (hndl->layerEnabled & VL2_ENABLED)
        {
            DrvLcdLayer0(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR,
                         hndl->lcdFrameSpec->width / 2, hndl->lcdFrameSpec->height, 0, 0,
                         VL1Cfg);
        }
        else
        {
            if (hndl->lcdSpec->width != hndl->lcdFrameSpec->width)
            {
                x = (hndl->lcdSpec->width - hndl->lcdFrameSpec->width) / 2;
            }

            if (hndl->lcdSpec->height != hndl->lcdFrameSpec->height)
            {
                y = (hndl->lcdSpec->height - hndl->lcdFrameSpec->height) / 2;
            }
            DrvLcdLayer0(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR,
                         hndl->lcdFrameSpec->width, hndl->lcdFrameSpec->height, x, y, VL1Cfg);
        }

        //Different settings if multiple video layers enabled (screen split in half)

        if (hndl->layerEnabled & VL2_ENABLED)
        {
            //Layer 0
            if (hndl->lcdFrameSpec->type == YUV420p)
            {
                DrvLcdCbPlanar(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, VL1,
                               (unsigned int) hndl->currentFrameVL1->p2,
                               hndl->lcdFrameSpec->width / 4, hndl->lcdFrameSpec->stride / 2);
                DrvLcdCrPlanar(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, VL1,
                               (unsigned int) hndl->currentFrameVL1->p3,
                               hndl->lcdFrameSpec->width / 4, hndl->lcdFrameSpec->stride / 2);
            }
            if (hndl->lcdFrameSpec->type == YUV422p)
            {
                DrvLcdCbPlanar(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, VL1,
                               (unsigned int) hndl->currentFrameVL1->p2,
                               hndl->lcdFrameSpec->width / 2, hndl->lcdFrameSpec->stride);
                DrvLcdCrPlanar(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, VL1,
                               (unsigned int) hndl->currentFrameVL1->p3,
                               hndl->lcdFrameSpec->width / 2, hndl->lcdFrameSpec->stride);
            }
            DrvLcdDmaLayer0(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR,
                            (unsigned int) hndl->currentFrameVL1->p1,
                            hndl->lcdFrameSpec->width / 2, hndl->lcdFrameSpec->height,
                            hndl->lcdFrameSpec->bytesPP, hndl->lcdFrameSpec->stride, LCtrl);
        }
        else
        {
            //Layer 0
            if (hndl->lcdFrameSpec->type == YUV420p)
            {
                DrvLcdCbPlanar(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, VL1,
                               (unsigned int) hndl->currentFrameVL1->p2,
                               hndl->lcdFrameSpec->width / 2, hndl->lcdFrameSpec->stride / 2);
                DrvLcdCrPlanar(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, VL1,
                               (unsigned int) hndl->currentFrameVL1->p3,
                               hndl->lcdFrameSpec->width / 2, hndl->lcdFrameSpec->stride / 2);
            }
            if (hndl->lcdFrameSpec->type == YUV422p)
            {
                DrvLcdCbPlanar(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, VL1,
                               (unsigned int) hndl->currentFrameVL1->p2,
                               hndl->lcdFrameSpec->width / 2, hndl->lcdFrameSpec->stride);
                DrvLcdCrPlanar(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, VL1,
                               (unsigned int) hndl->currentFrameVL1->p3,
                               hndl->lcdFrameSpec->width / 2, hndl->lcdFrameSpec->stride);
            }

            DrvLcdDmaLayer0(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR,
                            (unsigned int) hndl->currentFrameVL1->p1,
                            hndl->lcdFrameSpec->width, hndl->lcdFrameSpec->height,
                            hndl->lcdFrameSpec->bytesPP, hndl->lcdFrameSpec->stride, LCtrl);
        }

        ////////////////////////////////////////////////////////////////////////////////
        ///////////  VL2
        ////////////////////////////////////////////////////////////////////////////////
        if (hndl->layerEnabled & VL2_ENABLED)
        {
            DrvLcdLayer1(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR,
                         hndl->lcdFrameSpec->width / 2, hndl->lcdFrameSpec->height,
                         hndl->lcdFrameSpec->width / 2, 0, VL1Cfg);

            if (hndl->lcdFrameSpec->type == YUV420p)
            {
                DrvLcdCbPlanar(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, VL2,
                               (unsigned int) hndl->currentFrameVL2->p2,
                               hndl->lcdFrameSpec->width / 4, hndl->lcdFrameSpec->stride / 2);
                DrvLcdCrPlanar(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, VL2,
                               (unsigned int) hndl->currentFrameVL2->p3,
                               hndl->lcdFrameSpec->width / 4, hndl->lcdFrameSpec->stride / 2);
            }
            if (hndl->lcdFrameSpec->type == YUV422p)
            {
                DrvLcdCbPlanar(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, VL2,
                               (unsigned int) hndl->currentFrameVL2->p2,
                               hndl->lcdFrameSpec->width / 2, hndl->lcdFrameSpec->stride);
                DrvLcdCrPlanar(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, VL2,
                               (unsigned int) hndl->currentFrameVL2->p3,
                               hndl->lcdFrameSpec->width / 2, hndl->lcdFrameSpec->stride);
            }
            DrvLcdDmaLayer1(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR,
                            (unsigned int) hndl->currentFrameVL2->p1,
                            hndl->lcdFrameSpec->width / 2, hndl->lcdFrameSpec->height,
                            hndl->lcdFrameSpec->bytesPP, hndl->lcdFrameSpec->stride, LCtrl);
        }
    }
    else // if(hndl->lcdFrameSpec == NULL)
    {
        ////////////////////////////////////////////////////////////////////////////////
        ///////////  VL1
        ////////////////////////////////////////////////////////////////////////////////
        if (hndl->layerEnabled & VL1_ENABLED)
        {
            unsigned int VL1LCtrl = LCtrl;
            lcdControlEdited |= D_LCD_CTRL_VL1_ENABLE
                            | D_LCD_CTRL_ALPHA_TOP_VL1 | D_LCD_CTRL_ALPHA_BLEND_VL1;
            prevVal = GET_REG_WORD_VAL(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_CONTROL_OFFSET);
            DrvLcdCtrlNoTrig(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, 0x808000,
                       lcdsp->outputFormat, prevVal | lcdControlEdited);
            switch (hndl->VL1frameSpec->type)
            {
            case YUV420p:
                VL1Cfg = VL1Cfg | D_LCD_LAYER_8BPP | D_LCD_LAYER_CRCB_ORDER
                         | D_LCD_LAYER_FORMAT_YCBCR420PLAN
                         | D_LCD_LAYER_PLANAR_STORAGE;
                if(IS_LCD_OUTF_RGB(hndl->lcdSpec->outputFormat))
                {   // input format yuv, output format rgb, we need to enable color space conversion
                	VL1Cfg |= D_LCD_LAYER_CSC_EN;
                }
                VL1LCtrl = LCtrl | D_LCD_DMA_LAYER_V_STRIDE_EN;
                break;
            case YUV422p:
                VL1Cfg = VL1Cfg | D_LCD_LAYER_8BPP | D_LCD_LAYER_CRCB_ORDER
                         | D_LCD_LAYER_FORMAT_YCBCR422PLAN
                         | D_LCD_LAYER_PLANAR_STORAGE;
                if(IS_LCD_OUTF_RGB(hndl->lcdSpec->outputFormat))
                {   // input format yuv, output format rgb, we need to enable color space conversion
                	VL1Cfg |= D_LCD_LAYER_CSC_EN;
                }
                VL1LCtrl = LCtrl | D_LCD_DMA_LAYER_V_STRIDE_EN;
                break;
            case NV12:
                VL1Cfg = VL1Cfg | D_LCD_LAYER_8BPP | D_LCD_LAYER_CRCB_ORDER
                         | D_LCD_LAYER_FORMAT_NV12
                         | D_LCD_LAYER_PLANAR_STORAGE;
                if(IS_LCD_OUTF_RGB(hndl->lcdSpec->outputFormat))
                {   // input format yuv, output format rgb, we need to enable color space conversion
                	VL1Cfg |= D_LCD_LAYER_CSC_EN;
                }
                VL1LCtrl = LCtrl | D_LCD_DMA_LAYER_V_STRIDE_EN;
                break;
            case YUV422i:
                VL1Cfg = VL1Cfg | D_LCD_LAYER_16BPP | D_LCD_LAYER_FORMAT_YCBCR422LIN;
                if(IS_LCD_OUTF_RGB(hndl->lcdSpec->outputFormat))
                {   // input format yuv, output format rgb, we need to enable color space conversion
                	VL1Cfg |= D_LCD_LAYER_CSC_EN;
                }
                break;
            case RGB888:
                VL1Cfg = VL1Cfg | D_LCD_LAYER_24BPP | D_LCD_LAYER_FORMAT_RGB888;
                VL1LCtrl = LCtrl | D_LCD_CTRL_OUTPUT_RGB;
                break;
            case RAW16:
                VL1Cfg = VL1Cfg | D_LCD_LAYER_16BPP | D_LCD_LAYER_BGR_ORDER | D_LCD_LAYER_FORMAT_RGB565;
                VL1LCtrl = LCtrl | D_LCD_CTRL_OUTPUT_RGB;
                break;
            default:
                break;
            }

            DrvLcdLayer0(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR,
                         hndl->VL1frameSpec->width,
                         hndl->VL1frameSpec->height,
                         hndl->VL1offset.x,
                         hndl->VL1offset.y,
                         VL1Cfg);
            if(VL1Cfg & D_LCD_LAYER_CSC_EN)
            {
                DrvLcdCsc(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, VL1,
                          (u32 *) csc_coef_lcd);
            }

            if (hndl->VL1frameSpec->type == YUV420p)
            {
                DrvLcdCbPlanar(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, VL1,
                               (unsigned int) hndl->currentFrameVL1->p2,
                               hndl->VL1frameSpec->width / 2,
                               hndl->VL1frameSpec->stride / 2);
                DrvLcdCrPlanar(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, VL1,
                               (unsigned int) hndl->currentFrameVL1->p3,
                               hndl->VL1frameSpec->width / 2,
                               hndl->VL1frameSpec->stride / 2);
            }
            if (hndl->VL1frameSpec->type == YUV422p)
            {
                DrvLcdCbPlanar(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, VL1,
                               (unsigned int) hndl->currentFrameVL1->p2,
                               hndl->VL1frameSpec->width,
                               hndl->VL1frameSpec->stride);
                DrvLcdCrPlanar(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, VL1,
                               (unsigned int) hndl->currentFrameVL1->p3,
                               hndl->VL1frameSpec->width,
                               hndl->VL1frameSpec->stride);
            }
            if(hndl->VL1frameSpec->type == NV12)
            {
                DrvLcdCbPlanar(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, VL1,
                               (unsigned int) hndl->currentFrameVL1->p2,
                               hndl->VL1frameSpec->width,
                               hndl->VL1frameSpec->stride);
            }
            DrvLcdDmaLayer0(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR,
                            (unsigned int) hndl->currentFrameVL1->p1,
                            hndl->VL1frameSpec->width,
                            hndl->VL1frameSpec->height,
                            hndl->VL1frameSpec->bytesPP,
                            hndl->VL1frameSpec->stride,
                            VL1LCtrl);
        }

        ////////////////////////////////////////////////////////////////////////////////
        ///////////  VL2
        ////////////////////////////////////////////////////////////////////////////////
        if (hndl->layerEnabled & VL2_ENABLED)
        {
            unsigned int VL2LCtrl = LCtrl;
            lcdControlEdited |= D_LCD_CTRL_VL2_ENABLE
                            | D_LCD_CTRL_ALPHA_BOTTOM_VL2 | D_LCD_CTRL_ALPHA_BLEND_VL1;
            prevVal = GET_REG_WORD_VAL(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_CONTROL_OFFSET);
            DrvLcdCtrlNoTrig(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, 0x808000,
                       lcdsp->outputFormat, prevVal | lcdControlEdited);
            switch (hndl->VL2frameSpec->type)
            {
            case YUV420p:

                VL2Cfg = VL2Cfg | D_LCD_LAYER_8BPP | D_LCD_LAYER_CRCB_ORDER
                         | D_LCD_LAYER_FORMAT_YCBCR420PLAN
                         | D_LCD_LAYER_PLANAR_STORAGE ;
                if(IS_LCD_OUTF_RGB(hndl->lcdSpec->outputFormat))
                {   // input format yuv, output format rgb, we need to enable color space conversion
                	VL2Cfg |= D_LCD_LAYER_CSC_EN;
                }
                VL2LCtrl = LCtrl | D_LCD_DMA_LAYER_V_STRIDE_EN;
                break;
            case YUV422p:
                VL2Cfg = VL2Cfg | D_LCD_LAYER_8BPP | D_LCD_LAYER_CRCB_ORDER
                         | D_LCD_LAYER_FORMAT_YCBCR422PLAN
                         | D_LCD_LAYER_PLANAR_STORAGE ;
                if(IS_LCD_OUTF_RGB(hndl->lcdSpec->outputFormat))
                {   // input format yuv, output format rgb, we need to enable color space conversion
                	VL2Cfg |= D_LCD_LAYER_CSC_EN;
                }
                VL2LCtrl = LCtrl | D_LCD_DMA_LAYER_V_STRIDE_EN;
                break;
            case YUV422i:
                VL2Cfg = VL2Cfg | D_LCD_LAYER_16BPP | D_LCD_LAYER_FORMAT_YCBCR422LIN;
                if(IS_LCD_OUTF_RGB(hndl->lcdSpec->outputFormat))
                {   // input format yuv, output format rgb, we need to enable color space conversion
                	VL2Cfg |= D_LCD_LAYER_CSC_EN;
                }
                break;
            case RGB888:
                VL2Cfg = VL2Cfg | D_LCD_LAYER_24BPP | D_LCD_LAYER_FORMAT_RGB888;
                VL2LCtrl = LCtrl | D_LCD_CTRL_OUTPUT_RGB;
                break;
            case RAW16:
                VL2Cfg = VL2Cfg | D_LCD_LAYER_16BPP | D_LCD_LAYER_FORMAT_RGB888;
                VL2LCtrl = LCtrl | D_LCD_CTRL_OUTPUT_RGB;
                break;
            default:
                break;
            }

            DrvLcdLayer1(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR,
                         hndl->VL2frameSpec->width,
                         hndl->VL2frameSpec->height,
                         hndl->VL2offset.x,
                         hndl->VL2offset.y,
                         VL2Cfg);
            if(VL2Cfg & D_LCD_LAYER_CSC_EN)
            {
                DrvLcdCsc(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, VL2,
                          (u32 *) csc_coef_lcd);
            }

            if (hndl->VL2frameSpec->type == YUV420p)
            {
                DrvLcdCbPlanar(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, VL2,
                               (unsigned int) hndl->currentFrameVL2->p2,
                               hndl->VL2frameSpec->width / 2,
                               hndl->VL2frameSpec->stride / 2);
                DrvLcdCrPlanar(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, VL2,
                               (unsigned int) hndl->currentFrameVL2->p3,
                               hndl->VL2frameSpec->width / 2,
                               hndl->VL2frameSpec->stride / 2);
            }
            if (hndl->VL2frameSpec->type == YUV422p)
            {
                DrvLcdCbPlanar(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, VL2,
                               (unsigned int) hndl->currentFrameVL2->p2,
                               hndl->VL2frameSpec->width,
                               hndl->VL2frameSpec->stride);
                DrvLcdCrPlanar(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, VL2,
                               (unsigned int) hndl->currentFrameVL2->p3,
                               hndl->VL2frameSpec->width,
                               hndl->VL2frameSpec->stride);
            }
            DrvLcdDmaLayer1(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR,
                            (unsigned int) hndl->currentFrameVL2->p1,
                            hndl->VL2frameSpec->width,
                            hndl->VL2frameSpec->height,
                            hndl->VL2frameSpec->bytesPP,
                            hndl->VL2frameSpec->stride,
                            VL2LCtrl);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    ///////////  GL1
    ////////////////////////////////////////////////////////////////////////////////
    if (hndl->layerEnabled & GL1_ENABLED)
    {
        lcdControlEdited |=  D_LCD_CTRL_GL1_ENABLE | D_LCD_CTRL_ALPHA_TOP_GL1;
        prevVal = GET_REG_WORD_VAL(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_CONTROL_OFFSET);
        DrvLcdCtrlNoTrig(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, 0x808000,
                   lcdsp->outputFormat, prevVal | lcdControlEdited);
        switch (hndl->GL1FrameSpec->type)
        {
        case LUT2:
            GL1Cfg |= D_LCD_LAYER_FORMAT_CLUT | D_LCD_LAYER_LUT_2ENT;
            lutWidthCorrection = 8;
            break;
        case LUT4:
            GL1Cfg |= D_LCD_LAYER_FORMAT_CLUT | D_LCD_LAYER_LUT_4ENT;
            lutWidthCorrection = 4;
            break;
        case LUT16:
            GL1Cfg |= D_LCD_LAYER_FORMAT_CLUT | D_LCD_LAYER_LUT_16ENT;
            lutWidthCorrection = 2;
            break;
        case YUV422i:
            GL1Cfg = D_LCD_LAYER_FIFO_00 | D_LCD_LAYER_16BPP
                     | D_LCD_LAYER_FORMAT_YCBCR422LIN;
            if(IS_LCD_OUTF_RGB(hndl->lcdSpec->outputFormat))
            {   // input format yuv, output format rgb, we need to enable color space conversion
            	GL1Cfg |= D_LCD_LAYER_CSC_EN;
            }
            break;
        default:
            break;
        }
        // Configure LCD Layer2 (Graphic Layer 1)
        DrvLcdLayer2(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR,
                     hndl->GL1FrameSpec->width,                             // height
                     hndl->GL1FrameSpec->height,                             // width
                     hndl->GL1offset.x,                              // x start point
                     hndl->GL1offset.y,                              // y start point
                     GL1Cfg);

        switch (hndl->GL1FrameSpec->type)
        {
        case LUT2:
        case LUT4:
        case LUT16:
            // Configure LCD DMA for this layer
            DrvLcdDmaLayer2(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR,

                            ((unsigned int) hndl->currentFrameGL1->p1 & 0x00FFFFFF)
                            | (0x10000000),
                            hndl->GL1FrameSpec->width / lutWidthCorrection,     // width
                            hndl->GL1FrameSpec->height,                        // height
                            hndl->GL1FrameSpec->bytesPP,                          // bbp
                            hndl->GL1FrameSpec->width / lutWidthCorrection,    // stride
                            LCtrl | D_LCD_DMA_LAYER_V_STRIDE_EN);
            break;
        case YUV422i:
            DrvLcdDmaLayer2(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR,
                            (unsigned int) hndl->currentFrameGL1->p1,
                            hndl->GL1FrameSpec->width, hndl->GL1FrameSpec->height,
                            hndl->GL1FrameSpec->bytesPP, hndl->GL1FrameSpec->stride,
                            LCtrl);

            break;
        default:
            break;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    ///////////  GL2
    ////////////////////////////////////////////////////////////////////////////////
    if (hndl->layerEnabled & GL2_ENABLED)
    {
        lcdControlEdited |= D_LCD_CTRL_GL2_ENABLE | D_LCD_CTRL_ALPHA_BLEND_GL2;
        prevVal = GET_REG_WORD_VAL(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_CONTROL_OFFSET);
        DrvLcdCtrlNoTrig(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, 0x808000,
                   lcdsp->outputFormat, prevVal | lcdControlEdited);

        switch (hndl->GL2FrameSpec->type)
        {
        case LUT2:
            GL2Cfg |= D_LCD_LAYER_FORMAT_CLUT | D_LCD_LAYER_LUT_2ENT;
            lutWidthCorrection = 8;
            break;
        case LUT4:
            GL2Cfg |= D_LCD_LAYER_FORMAT_CLUT | D_LCD_LAYER_LUT_4ENT;
            lutWidthCorrection = 4;
            break;
        case LUT16:
            GL2Cfg |= D_LCD_LAYER_FORMAT_CLUT | D_LCD_LAYER_LUT_16ENT;
            lutWidthCorrection = 2;
            break;
        case YUV422i:
            GL2Cfg = D_LCD_LAYER_FIFO_00 | D_LCD_LAYER_16BPP
                     | D_LCD_LAYER_FORMAT_YCBCR422LIN;
            if(IS_LCD_OUTF_RGB(hndl->lcdSpec->outputFormat))
            {   // input format yuv, output format rgb, we need to enable color space conversion
            	GL2Cfg |= D_LCD_LAYER_CSC_EN;
            }
            break;
        default:
            break;
        }
        DrvLcdLayer3(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR,
                     hndl->GL2FrameSpec->width,
                     hndl->GL2FrameSpec->height,
                     hndl->GL2offset.x,
                     hndl->GL2offset.y,
                     GL2Cfg);

        switch (hndl->GL2FrameSpec->type)
        {
        case LUT2:
        case LUT4:
        case LUT16:
            // Configure LCD DMA for this layer
            DrvLcdDmaLayer3(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR,
                            (u32) hndl->currentFrameGL2->p1,
                            hndl->GL2FrameSpec->width / lutWidthCorrection,
                            hndl->GL2FrameSpec->height, hndl->GL2FrameSpec->bytesPP,
                            hndl->GL2FrameSpec->stride,
                            D_LCD_DMA_LAYER_ENABLE |
                            D_LCD_DMA_LAYER_CONT_UPDATE |
                            D_LCD_DMA_LAYER_AXI_BURST_16);
            break;
        case YUV422i:
            DrvLcdDmaLayer3(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR,
                            (unsigned int) hndl->currentFrameGL2->p1,
                            hndl->GL2FrameSpec->width, hndl->GL2FrameSpec->height,
                            hndl->GL2FrameSpec->bytesPP, hndl->GL2FrameSpec->stride,
                            LCtrl);

            break;
        default:
            break;
        }
    }

    if(hndl->lcdFrameSpec != NULL)
    {
        DrvLcdCtrlNoTrig(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, 0x808000,
                   lcdsp->outputFormat, lcdControlEdited | D_LCD_CTRL_VL1_ENABLE);
    }
    else
    {
        DrvLcdCtrlNoTrig(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, 0x808000,
                   lcdsp->outputFormat, lcdControlEdited);
    }

    return;
}

static void LCDEnableInterface (LCDHandle *handle)
{
    u32 controlRegisterAddress = lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR
                                 + LCD_CONTROL_OFFSET;
    u32 previousValue = GET_REG_WORD_VAL(controlRegisterAddress);

    previousValue |= D_LCD_CTRL_TIM_GEN_ENABLE | D_LCD_CTRL_ENABLE;

    /// Add datapath dependent configs
    switch(handle->outputDataPath)
    {
        case LCD_TO_PARALLEL:
            /// Mandatory for HDMI
            previousValue |= D_LCD_CTRL_BPORCH_ENABLE | D_LCD_CTRL_FPORCH_ENABLE;
            break;
        case LCD_TO_MIPI:
            break;
        default:
            assert(0);
            break;
    }

    SET_REG_WORD(controlRegisterAddress, previousValue);
}

void LCDStop (LCDHandle *hndl)
{
    hndl->LCDFrameCount = 0;

    if (hndl->lcdNumber == LCD1)
    {
        // Clock the Lcd interface
        DrvCprSysDeviceAction(MSS_DOMAIN, ENABLE_CLKS, DEV_MSS_LCD);
    }
    else
    {
        // Clock the Lcd interface
        // DrvCprSysDeviceAction(ENABLE_CLKS, DEV_LCD2);
    }

    // Disable Lcd Interrupts
    DrvIcbDisableIrq(lcds[hndl->lcdNumber].IRQ_LCD_X);

    // Disable Layers DMA's
    SET_REG_WORD(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_CONTROL_OFFSET,
                 0);
    SET_REG_WORD(
        lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER0_DMA_CFG_OFFSET,
        0);
    SET_REG_WORD(
        lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER1_DMA_CFG_OFFSET,
        0);
    SET_REG_WORD(
        lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER2_DMA_CFG_OFFSET,
        0);
    SET_REG_WORD(
        lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER3_DMA_CFG_OFFSET,
        0);

    //Wait for DMA's to be inactive
    while (((GET_REG_WORD_VAL(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER0_DMA_CFG_OFFSET))
            & D_LCD_DMA_LAYER_STATUS))
        ;
    while (((GET_REG_WORD_VAL(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER1_DMA_CFG_OFFSET))
            & D_LCD_DMA_LAYER_STATUS))
        ;
    while (((GET_REG_WORD_VAL(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER2_DMA_CFG_OFFSET))
            & D_LCD_DMA_LAYER_STATUS))
        ;
    while (((GET_REG_WORD_VAL(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER3_DMA_CFG_OFFSET))
            & D_LCD_DMA_LAYER_STATUS))
        ;

    //TODO: Create a timer to avoid indefinite timeouts

    if (hndl->lcdNumber == LCD1)
    {
        // Reset the Lcd interface
        DrvCprSysDeviceAction(MSS_DOMAIN, ASSERT_RESET, DEV_MSS_LCD);
    }
    else
    {
        // Reset the Lcd interface
        // DrvCprSysDeviceAction(MSS_DOMAIN, ASSERT_RESET, DEV_LCD2);
    }
}

static void LCDSaveCurrentFramesAsPreviousFrames (LCDHandle *handle,
        frameBuffer **previousLCDBuffers)
{
    previousLCDBuffers[0] = handle->currentFrameVL1;
    previousLCDBuffers[1] = handle->currentFrameVL2;
    previousLCDBuffers[2] = handle->currentFrameGL1;
    previousLCDBuffers[3] = handle->currentFrameGL2;
}

static void LCDGiveBackPreviousBuffers (LCDHandle *handle,
                                       frameBuffer **previousLCDBuffers)
{
    if (handle->cbFrameReady != NULL)
        handle->cbFrameReady(previousLCDBuffers[0], previousLCDBuffers[1],
                             previousLCDBuffers[2], previousLCDBuffers[3]);
}

static void LCDCollectNewLayerBuffersUsingGetFrameCallback (LCDHandle *handle)
{
    if (handle->cbGetFrame != NULL)
    {
        handle->LCDFrameCount++;

        handle->currentFrameVL1 = handle->cbGetFrame(VL1);
        LCDSetLayerBuffersNormalAndShadowRegisters(handle, VL1);

        if (handle->layerEnabled & VL2_ENABLED)
        {
            handle->currentFrameVL2 = handle->cbGetFrame(VL2);
            LCDSetLayerBuffersNormalAndShadowRegisters(handle, VL2);
        }

        if (handle->layerEnabled & GL1_ENABLED)
        {
            handle->currentFrameGL1 = handle->cbGetFrame(GL1);
            LCDSetLayerBuffersNormalAndShadowRegisters(handle, GL1);
        }

        if (handle->layerEnabled & GL2_ENABLED)
        {
            handle->currentFrameGL2 = handle->cbGetFrame(GL2);
            LCDSetLayerBuffersNormalAndShadowRegisters(handle, GL2);
        }
    }
}

void LCDHandler(u32 source)
{
    #ifdef Use_LcdGeneric_on_LRT
    u32 lcdNum = (source == IRQ_LCD) ? 0 : 1;
    #else
    u32 lcdNum = (source == ROUTED_IRQ_LCD) ? 0 : 1;
    #endif

    frameBuffer *previousLCDBuffers[4] =
    { NULL, NULL, NULL, NULL };

    if (LCDHndl[lcdNum] != NULL)
    {
        LCDSaveCurrentFramesAsPreviousFrames(LCDHndl[lcdNum],
                                             previousLCDBuffers);
        LCDCollectNewLayerBuffersUsingGetFrameCallback(LCDHndl[lcdNum]);
    }

    // Clear the interrupt that this handler cares about
    // Clear the interrupt flag in the peripheral
    SET_REG_WORD(lcds[lcdNum].LCDX_INT_CLEAR_ADR, D_LCD_INT_LINE_CMP);
    // Clear the interrupt flag in the ICB
    DrvIcbIrqClear(lcds[lcdNum].IRQ_LCD_X);

    if (LCDHndl[lcdNum] != NULL)
    {
        LCDGiveBackPreviousBuffers(LCDHndl[lcdNum], previousLCDBuffers);
    }
}

void LCDInitVL2Enable(LCDHandle *hndl)
{
    hndl->layerEnabled |= VL2_ENABLED;
}

void LCDInitVL2Disable(LCDHandle *hndl)
{
    hndl->layerEnabled &= ~VL2_ENABLED;
}

void LCDSetColorTable(LCDHandle *hndl, int layer, unsigned int *colorTable,
                      int length)
{
    if (layer == 1)
    {
        DrvLcdTransparency(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, GL1,
                           colorTable[0], 255);
        DrvLcdLut(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, GL1,
                  (u32*) colorTable, length);
    }
    else if (layer == 2)
    {
        DrvLcdTransparency(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, GL2,
                           colorTable[0], 255);
        DrvLcdLut(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, GL2,
                  (u32*) colorTable, length);
    }
}

void LCDInitGLEnable(LCDHandle *hndl, int layer,
                                       const frameSpec *fr)
{
    if (layer == 1)
    {
        hndl->layerEnabled |= GL1_ENABLED;
        hndl->GL1FrameSpec = fr;
    }
    if (layer == 2)
    {
        hndl->layerEnabled |= GL2_ENABLED;
        hndl->GL2FrameSpec = fr;
    }
}

void LCDInitGLDisable(LCDHandle *hndl, int layer)
{
    if (layer == 1)
        hndl->layerEnabled &= ~GL1_ENABLED;
    if (layer == 2)
        hndl->layerEnabled &= ~GL2_ENABLED;
}

void LCDInitLayer(LCDHandle *hndl, int layer, frameSpec *fsp, LCDLayerOffset position)
{
    switch(layer)
    {
    case VL1:
        hndl->layerEnabled |= VL1_ENABLED;
        hndl->VL1frameSpec = fsp;
        hndl->VL1offset = position;
        break;
    case VL2:
        hndl->layerEnabled |= VL2_ENABLED;
        hndl->VL2frameSpec = fsp;
        hndl->VL2offset = position;
        break;
    case GL1:
        hndl->layerEnabled |= GL1_ENABLED;
        hndl->GL1FrameSpec = fsp;
        hndl->GL1offset = position;
        break;
    case GL2:
        hndl->layerEnabled |= GL2_ENABLED;
        hndl->GL2FrameSpec = fsp;
        hndl->GL2offset = position;
        break;
    }
}

void LCDInit(LCDHandle *hndl, const LCDDisplayCfg *lcdSpec,
                              const frameSpec *fsp, unsigned int lcdNum)
{
    u32 controlRegisterAddress, previousValue;

    hndl->lcdFrameSpec = fsp;
    hndl->lcdSpec = lcdSpec;
    hndl->lcdNumber = lcdNum;
    hndl->outputDataPath = LCD_TO_PARALLEL;     /// Default output to Parallel port
    if(hndl->lcdFrameSpec != NULL)
        hndl->layerEnabled = VL1_ENABLED;
    else
        hndl->layerEnabled = 0;
    LCDHndl[lcdNum] = hndl;
#ifndef Use_LcdGeneric_on_LRT
    DrvIcbDynamicIrqConfig(LRT_TO_LOS /* LeonRT ICB base Address */,
            IRQ_LCD /* source IRQ */ , ROUTED_IRQ_LCD /* target IRQ */, 0x01 /* Enable Bit */);
#endif

    // Always, always enable pipelined (outstanding) DMA reads (if possible)
    controlRegisterAddress = lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR
                             + LCD_CONTROL_OFFSET;
    previousValue  = 0;
    previousValue |= D_LCD_CTRL_PIPELINE_DMA;

    SET_REG_WORD(controlRegisterAddress, previousValue);
}

static inline void LCDSetTimingFromDisplayCfg(LCDHandle *hndl,
                                       const LCDDisplayCfg *lcdSpec)
{
    // LCD timing
    DrvLcdTiming(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR, lcdSpec->width,
                 lcdSpec->hPulseWidth, lcdSpec->hBackP, lcdSpec->hFrontP,
                 lcdSpec->height, lcdSpec->vPulseWidth, lcdSpec->vBackP,
                 lcdSpec->vFrontP);
}

static void LCDCallbackFrequencySetter(LCDHandle *hndl)
{
    if ((hndl->lcdSpec->pixelClockkHz == 148500)
            && (hndl->callBackFctHighRes != NULL))
    {
        hndl->callBackFctHighRes();
    }
    else if (hndl->callBackFctLowRes != NULL)
    {
        hndl->callBackFctLowRes();
    }
}

void LCDEnYuv422i(){
	u32 data  = 0;
	data |= (0x2   << 9);     // Layer format
	data |= (0x1     << 14);  // Planar storage
	data |= ((0x1-1) << 15);  // Bytes per pixel
	SET_REG_WORD(LCD1_LAYER0_CFG_ADR,        data);
}

void LCDStart(LCDHandle *hndl)
{
    LCDCallbackFrequencySetter(hndl);

    LCDSetTimingFromDisplayCfg(hndl, hndl->lcdSpec);

    LCDCollectNewLayerBuffersUsingGetFrameCallback(hndl);

    LCDConfigureLayers(hndl, hndl->lcdSpec,
                       D_LCD_DMA_LAYER_CONT_UPDATE | D_LCD_DMA_LAYER_ENABLE,
                       hndl->lcdSpec->control & ~D_LCD_CTRL_DISPLAY_MODE_ONE_SHOT);

    SET_REG_WORD(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_TIMING_GEN_TRIG_OFFSET, 1);

    // Disable ICB (Interrupt Control Block) while setting new interrupt
    DrvIcbDisableIrq(lcds[hndl->lcdNumber].IRQ_LCD_X);
    DrvIcbIrqClear(lcds[hndl->lcdNumber].IRQ_LCD_X);

    DrvIcbSetIrqHandler(lcds[hndl->lcdNumber].IRQ_LCD_X, LCDHandler);

    // Configure interrupts
    DrvIcbConfigureIrq(lcds[hndl->lcdNumber].IRQ_LCD_X,
                       LCDGENERIC_INTERRUPT_LEVEL, POS_EDGE_INT);

    // Trigger the interrupts
    DrvIcbEnableIrq(lcds[hndl->lcdNumber].IRQ_LCD_X);

    if(hndl->lcdFrameSpec != NULL)
    {
        SET_REG_WORD(
            lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LINE_COMPARE_OFFSET,
            hndl->lcdFrameSpec->height - 5);
    }
    else
    {
        unsigned int lowestLine = 0;

        if(hndl->layerEnabled & VL1_ENABLED )
            if(hndl->VL1offset.y + hndl->VL1frameSpec->height > lowestLine)
                lowestLine = hndl->VL1offset.y + hndl->VL1frameSpec->height;

        if(hndl->layerEnabled & VL2_ENABLED )
            if(hndl->VL2offset.y + hndl->VL2frameSpec->height > lowestLine)
                lowestLine = hndl->VL2offset.y + hndl->VL2frameSpec->height;

        if(hndl->layerEnabled & GL1_ENABLED )
            if(hndl->GL1offset.y + hndl->GL1FrameSpec->height > lowestLine)
                lowestLine = hndl->GL1offset.y + hndl->GL1FrameSpec->height;

        if(hndl->layerEnabled & GL2_ENABLED )
            if(hndl->GL2offset.y + hndl->GL2FrameSpec->height > lowestLine)
                lowestLine = hndl->GL2offset.y + hndl->GL2FrameSpec->height;

        SET_REG_WORD(
            lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LINE_COMPARE_OFFSET,
            lowestLine - 5);
    }
    SET_REG_WORD(
        lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_INT_CLEAR_OFFSET,
        D_LCD_INT_LINE_CMP);
    SET_REG_WORD(
        lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_INT_ENABLE_OFFSET,
        D_LCD_INT_LINE_CMP);

    LCDEnableInterface(hndl);
}


void LCDHandlerOneShot(u32 source)
{
    #ifdef Use_LcdGeneric_on_LRT
    u32 lcdNum = (source == IRQ_LCD) ? 0 : 1;
    #else
    u32 lcdNum = (source == ROUTED_IRQ_LCD) ? 0 : 1;
    #endif
    u32 intstatus;

    intstatus = GET_REG_WORD_VAL(lcds[lcdNum].MXI_DISPX_BASE_ADR + LCD_INT_STATUS_OFFSET);

    // Clear the interrupt that this handler cares about
    // Clear the interrupt flag in the peripheral
    SET_REG_WORD(lcds[lcdNum].LCDX_INT_CLEAR_ADR, D_LCD_INT_EOF);
    // Clear the interrupt flag in the ICB
    DrvIcbIrqClear(lcds[lcdNum].IRQ_LCD_X);

    if ((intstatus & D_LCD_INT_EOF) &&
        LCDHndl[lcdNum] != NULL)
    {
        LCDGiveBackPreviousBuffers(LCDHndl[lcdNum],
                                   LCDHndl[lcdNum]->queuedFrames[LCDHndl[lcdNum]->qfront]);
        LCDHndl[lcdNum]->qfront = (LCDHndl[lcdNum]->qfront + 1) % 3;
    }
}

void LCDStartOneShot(LCDHandle *hndl)
{
    LCDCallbackFrequencySetter(hndl);

    LCDSetTimingFromDisplayCfg(hndl, hndl->lcdSpec);

    LCDConfigureLayers(hndl, hndl->lcdSpec, 0 /* 0: no auto restart */,
                       hndl->lcdSpec->control | D_LCD_CTRL_DISPLAY_MODE_ONE_SHOT);

    SET_REG_WORD(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_TIMING_GEN_TRIG_OFFSET, 0);

    hndl->firstFrameQueued = 0;
    hndl->LCDFrameCount = 0;

    if (hndl->cbFrameReady != NULL)
    {
        hndl->qfront = 0;
        hndl->qback = 2;

        // Clear the interrupt flag in the ICB
        DrvIcbIrqClear(lcds[hndl->lcdNumber].IRQ_LCD_X);

        DrvIcbSetupIrq(lcds[hndl->lcdNumber].IRQ_LCD_X,
                       LCDGENERIC_INTERRUPT_LEVEL,
                       POS_EDGE_INT, LCDHandlerOneShot);

        SET_REG_WORD(
            lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_INT_CLEAR_OFFSET,
            D_LCD_INT_EOF);
        SET_REG_WORD(
            lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_INT_ENABLE_OFFSET,
            D_LCD_INT_EOF);
    }

    LCDEnableInterface(hndl);
}

int LCDQueueFrame(LCDHandle *handle, frameBuffer *VL1Buffer,
                  frameBuffer *VL2Buffer, frameBuffer *GL1Buffer, frameBuffer *GL2Buffer)
{
    if (!LCDCanQueueFrame(handle))
        return -1;

    handle->qback = (handle->qback + 1) % 3;

    handle->queuedFrames[handle->qback][0] = handle->currentFrameVL1 =
                VL1Buffer;
    handle->queuedFrames[handle->qback][1] = handle->currentFrameVL2 =
                VL2Buffer;
    handle->queuedFrames[handle->qback][2] = handle->currentFrameGL1 =
                GL1Buffer;
    handle->queuedFrames[handle->qback][3] = handle->currentFrameGL2 =
                GL2Buffer;

    if (!handle->firstFrameQueued)
    {
        handle->firstFrameQueued = 1;
        handle->LCDFrameCount = 1;

        if (handle->layerEnabled & VL1_ENABLED)
        {
            LCDSetLayerBuffersNormalAndShadowRegisters(handle, VL1);
            SET_REG_WORD(
                lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER0_DMA_CFG_OFFSET,
                GET_REG_WORD_VAL(lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER0_DMA_CFG_OFFSET) | D_LCD_DMA_LAYER_ENABLE);
        }
        if (handle->layerEnabled & VL2_ENABLED)
        {
            LCDSetLayerBuffersNormalAndShadowRegisters(handle, VL2);
            SET_REG_WORD(
                lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER1_DMA_CFG_OFFSET,
                GET_REG_WORD_VAL(lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER1_DMA_CFG_OFFSET) | D_LCD_DMA_LAYER_ENABLE);
        }
        if (handle->layerEnabled & GL1_ENABLED)
        {
            LCDSetLayerBuffersNormalAndShadowRegisters(handle, GL1);
            SET_REG_WORD(
                lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER2_DMA_CFG_OFFSET,
                GET_REG_WORD_VAL(lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER2_DMA_CFG_OFFSET) | D_LCD_DMA_LAYER_ENABLE);
        }
        if (handle->layerEnabled & GL2_ENABLED)
        {
            LCDSetLayerBuffersNormalAndShadowRegisters(handle, GL2);
            SET_REG_WORD(
                lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER3_DMA_CFG_OFFSET,
                GET_REG_WORD_VAL(lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER3_DMA_CFG_OFFSET) | D_LCD_DMA_LAYER_ENABLE);
        }

    }
    else
    {
        handle->LCDFrameCount++;

        if (handle->layerEnabled & VL1_ENABLED)
        {
            LCDSetLayerBuffersShadowRegisters(handle, VL1);
            SET_REG_WORD(
                lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER0_DMA_CFG_OFFSET,
                GET_REG_WORD_VAL(lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER0_DMA_CFG_OFFSET) | D_LCD_DMA_LAYER_AUTO_UPDATE);
        }
        if (handle->layerEnabled & VL2_ENABLED)
        {
            LCDSetLayerBuffersShadowRegisters(handle, VL2);
            SET_REG_WORD(
                lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER1_DMA_CFG_OFFSET,
                GET_REG_WORD_VAL(lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER1_DMA_CFG_OFFSET) | D_LCD_DMA_LAYER_AUTO_UPDATE);
        }
        if (handle->layerEnabled & GL1_ENABLED)
        {
            LCDSetLayerBuffersShadowRegisters(handle, GL1);
            SET_REG_WORD(
                lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER2_DMA_CFG_OFFSET,
                GET_REG_WORD_VAL(lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER2_DMA_CFG_OFFSET) | D_LCD_DMA_LAYER_AUTO_UPDATE);
        }
        if (handle->layerEnabled & GL2_ENABLED)
        {
            LCDSetLayerBuffersShadowRegisters(handle, GL2);
            SET_REG_WORD(
                lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER3_DMA_CFG_OFFSET,
                GET_REG_WORD_VAL(lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER3_DMA_CFG_OFFSET) | D_LCD_DMA_LAYER_AUTO_UPDATE);
        }
    }

    // Trigger sending a frame
    SET_REG_WORD(lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_TIMING_GEN_TRIG_OFFSET, 1);

    return 0;
}


int LCDCanQueueFrame(LCDHandle *handle)
{
    /// Check if we triggered another frame already.
    /// Separate checks for each DMA are unnecessary because DMA's SHOULD finish faster than LCD (underflow if not)
    if (GET_REG_WORD_VAL(lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_TIMING_GEN_TRIG_OFFSET))
        return 0;
    else
        return 1;
}

void LCDSetOutput(LCDHandle *hndl, lcdDatapath_t dataPath)
{
    /// Sanity check
    if( dataPath >= LCD_INVALID_OUTPUT )
    {
        assert(!"Invalid LCD output path");
    }
    else
    {
        hndl->outputDataPath = dataPath;
    }
}
