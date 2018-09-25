///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     bicubicWarp API
///

#include <stdio.h>
#include <math.h>
#include <mv_types.h>
#include <registersMyriad.h>
#include <VcsHooksApi.h>
#include <DrvLeonL2C.h>

#include "bicubicWarpApi.h"
#include "bicubicWarpApiDefines.h"

#ifdef __RTEMS__
#include <rtems.h>
#include <bsp.h>
#include <bsp/irq-generic.h>
#endif

// Initialize function
int bicubicWarpInit(bicubicWarpContext* ctx)
{
    u32 irqNumber = ctx->irqNumber;
    irq_handler bicubicIrqHandler = ctx->hndl;

#ifdef __RTEMS__
    BSP_Set_interrupt_type_priority(irqNumber, POS_EDGE_INT, IRQ_PRIORITY_BICUBIC);
    BSP_interrupt_register(irqNumber, NULL, (rtems_interrupt_handler)bicubicIrqHandler, NULL);
#else
    DrvIcbSetupIrq(irqNumber, IRQ_PRIORITY_BICUBIC, POS_EDGE_INT, bicubicIrqHandler);
    DrvIcbEnableIrq(irqNumber);
#endif

    return 0;
}

// Generate mesh for rotation and translation relative to the center of the image
void bicubicWarpGenerateMeshRT(bicubicWarpContext* ctx)
{
    fp32 *xyRectifiedBuffer = ctx->xyRectifiedBuffer;
    fp32 *matrixRT = ctx->homography;
    u32 width = ctx->width;
    u32 height = ctx->height;

    u32 y, x;
    fp32 coord;
    fp32 x0 = 0.5f * width;
    fp32 y0 = 0.5f * height;

    fp32* xyRectified = &xyRectifiedBuffer[0];

    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            coord = x0 + (x-x0) * matrixRT[0] + (y-y0) * matrixRT[1] + matrixRT[2];
            *xyRectified = coord;
            xyRectified++;
            coord = y0 + (x-x0) * matrixRT[3] + (y-y0) * matrixRT[4] + matrixRT[5];
            *xyRectified = coord;
            xyRectified++;
        }
    }
    // Enforce cache coherency: make sure the newly written data (xyRectified buffer)
    // gets to the memory so that the bicubic hw block reads updated values.
    swcLeonDataCacheFlush();
    DrvLL2CFlushOpOnAddrRange(LL2C_OPERATION_WRITE_BACK, 0, (u32)xyRectifiedBuffer, (u32)xyRectified);
}

// Generate mesh for rotation, translation and projection relative to the top-left corner
void bicubicWarpGenerateMeshHomographyRTP(bicubicWarpContext* ctx)
{
    u32 y, x;
    fp32 coord;
    fp32 w;

    fp32 *xyRectifiedBuffer = ctx->xyRectifiedBuffer;
    fp32 *matrixRTP = ctx->homography;
    u32 width = ctx->width;
    u32 height = ctx->height;
    fp32* xyRectified = &xyRectifiedBuffer[0];

    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            w = x * matrixRTP[6] + y * matrixRTP[7] + matrixRTP[8];
            coord = x * matrixRTP[0] + y * matrixRTP[1] + matrixRTP[2];
            *xyRectified = coord / w;
            xyRectified++;
            w = x * matrixRTP[6] + y * matrixRTP[7] + matrixRTP[8];
            coord = x * matrixRTP[3] + y * matrixRTP[4] + matrixRTP[5];
            *xyRectified = coord / w;
            xyRectified++;
        }
    }
    // Enforce cache coherency: make sure the newly written data (xyRectified buffer)
    // gets to the memory so that the bicubic hw block reads updated values.
    swcLeonDataCacheFlush();
    DrvLL2CFlushOpOnAddrRange(LL2C_OPERATION_WRITE_BACK, 0, (u32)xyRectifiedBuffer, (u32)xyRectified);
}

// Generate rectified coordinates based on LUT maps
void bicubicWarpGenerateMeshFromLUTMaps(bicubicWarpContext* ctx)
{
    u32 y, x;
    u32 cnt = 0;
    fp32 coord;

    fp32 *xyRectifiedBuffer = ctx->xyRectifiedBuffer;
    fp32 *map_x = ctx->mapx;
    fp32 *map_y = ctx->mapy;
    u32 width = ctx->width;
    u32 height = ctx->height;
    fp32* xyRectified = &xyRectifiedBuffer[0];

    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            coord = map_x[cnt];
            *xyRectified = coord;
            xyRectified++;
            coord = map_y[cnt];
            *xyRectified = coord;
            xyRectified++;
            cnt++;
        }
    }
    // Enforce cache coherency: make sure the newly written data (xyRectified buffer)
    // gets to the memory so that the bicubic hw block reads updated values.
    swcLeonDataCacheFlush();
    DrvLL2CFlushOpOnAddrRange(LL2C_OPERATION_WRITE_BACK, 0, (u32)xyRectifiedBuffer, (u32)xyRectified);
}

// Apply bicubic filter on frame
int bicubicWarpProcessFrame(bicubicWarpContext* bicubicCtx)
{
    u8* input = bicubicCtx->inputImageBuffer;
    u8* output = bicubicCtx->outputImageBuffer;
    u32 width = bicubicCtx->width;
    u32 height = bicubicCtx->height;
    u32 bpp = bicubicCtx->bpp;

    // Configure Bicubic block
    SET_REG_WORD(BIC_ID_ADR, 0xa0);
    SET_REG_WORD(BIC_LINE_LENGTH_ADR, width * height);
    SET_REG_WORD(BIC_INPUT_HEIGHT_ADR, height);
    SET_REG_WORD(BIC_XY_PIXEL_LIST_BASE_ADR,  (u32)(bicubicCtx->xyRectifiedBuffer));

    SET_REG_WORD(BIC_INPUT_BASE_ADR, (u32)(&input[0]));
    SET_REG_WORD(BIC_INPUT_TOP_ADR, (u32)(&input[0] + width * height * bpp));
    SET_REG_WORD(BIC_INPUT_CHUNK_STRIDE_ADR, width * bpp);
    SET_REG_WORD(BIC_INPUT_CHUNK_WIDTH_ADR, width);
    SET_REG_WORD(BIC_INPUT_LINE_STRIDE_ADR, width * bpp);
    SET_REG_WORD(BIC_INPUT_LINE_WIDTH_ADR, width);

    SET_REG_WORD(BIC_OUTPUT_BASE_ADR, (u32)&output[0]);
    SET_REG_WORD(BIC_OUTPUT_CHUNK_STRIDE_ADR, width * bpp);
    SET_REG_WORD(BIC_OUTPUT_CHUNK_WIDTH_ADR, width);

    SET_REG_WORD(BIC_CLAMP_MAX_ADR, CLAMP_MAX);
    SET_REG_WORD(BIC_CLAMP_MIN_ADR, CLAMP_MIN);

    u64 bicubicCtrl = 0;
    bicubicCtrl |= (bicubicCtx->pixelFormat << BICUBIC_PIX_FORMAT_SHIFT) & BICUBIC_PIX_FORMAT_MASK;
    bicubicCtrl |= (1 << BICUBIC_IRQ_SHIFT) & BICUBIC_IRQ_MASK;
    bicubicCtrl |= (1 << BICUBIC_RUN_SHIFT) & BICUBIC_RUN_MASK;
    bicubicCtrl |= (1 << BICUBIC_BYPASS_SHIFT) & BICUBIC_BYPASS_MASK;
    bicubicCtrl |= (1 << BICUBIC_BORDER_MODE_SHIFT) & BICUBIC_BORDER_MODE_MASK;
    bicubicCtrl |= (1 << BICUBIC_CLAMP_SHIFT) & BICUBIC_CLAMP_MASK;
    bicubicCtrl |= (1 << BICUBIC_BILINEAR_SHIFT) & BICUBIC_BILINEAR_MASK;

    // Start bicubic filter
    SET_REG_WORD(BIC_CTRL_ADR, bicubicCtrl);

    return 0;
}
