///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief FramePump CIF back-end implementation
///

#include "FramePumpPrivate.h"
#include "FramePumpDefines.h"
#include "FramePumpCif.h"
#include "FramePumpMipi.h"
#include "FramePumpCommon.h"

#include "registersMyriad.h"
#include <malloc.h>
#include <DrvCif.h>
#include <DrvIcb.h>
#include <DrvCpr.h>
#include <DrvMss.h>
#include <stdio.h>
#include <OsDrvTimer.h>
#include <rtems.h>
#include <bsp.h>

#define MVLOG_UNIT_NAME FramePumpCif
#include <mvLog.h>

#ifndef ROUTED_IRQ_CIF0
#define ROUTED_IRQ_CIF0            IRQ_DYNAMIC_0 //all of the CIF0 isr are presented on the same line
#endif

#ifndef ROUTED_IRQ_CIF1
#define ROUTED_IRQ_CIF1            IRQ_DYNAMIC_1 //all of the CIF1 isr are presented on the same line
#endif

#ifndef CONFIG_TEARING_CHECK
#define CONFIG_TEARING_CHECK    (0)
#endif

#define CIF_DMA_MODE D_CIF_DMA_AUTO_RESTART_CONTINUOUS

// For naming the ISR
#define ISR_NAME_STEM "ISR_CIF_"
#define MAX_ISR_DIGITS (5)

struct CifPwmRegisters {
    volatile unsigned int PWM_CTRL;
    volatile unsigned int PWM_RPT_LEADIN;
    volatile unsigned int PWM_HIGH_LOW;
};

struct CifRegisters {
    // 0x000
    volatile unsigned int CONTROL;
    volatile unsigned int INT_STATUS;
    volatile unsigned int INT_ENABLE;
    volatile unsigned int INT_CLEAR;
    // 0x010
    volatile unsigned int INPUT_IF_CFG;
    volatile unsigned int INPUT_FORMAT;
    volatile unsigned int FRAME_WIDTH;
    volatile unsigned int FRAME_HEIGHT;
    volatile unsigned int FRAME_COUNT;
    volatile unsigned int FRAME_RATE;
    volatile unsigned int LINE_COUNT;
    // 0x02C
    volatile unsigned int WIN_ROW_START;
    volatile unsigned int WIN_COL_START;
    volatile unsigned int WIN_WIDTH;
    volatile unsigned int WIN_HEIGHT;
    volatile unsigned int SUBSAMPLE_CFG;
    volatile unsigned int rsrvd0;
    // 0x044
    volatile unsigned int R_ACC_VAL;
    volatile unsigned int G_ACC_VAL;
    volatile unsigned int B_ACC_VAL;
    volatile unsigned int R_MAX_VAL;
    volatile unsigned int G_MAX_VAL;
    volatile unsigned int B_MAX_VAL;
    // 0x05C
    volatile unsigned int CCR_COEFF11;
    volatile unsigned int CCR_COEFF12;
    volatile unsigned int CCR_COEFF13;
    volatile unsigned int CCR_COEFF21;
    volatile unsigned int CCR_COEFF22;
    volatile unsigned int CCR_COEFF23;
    volatile unsigned int CCR_COEFF31;
    volatile unsigned int rsrvd1[2];
    // 0x088
    volatile unsigned int CCR_COEFF32;
    volatile unsigned int CCR_COEFF33;
    volatile unsigned int CCR_OFF1;
    volatile unsigned int CCR_OFF2;
    volatile unsigned int CCR_OFF3;
    // 0x094
    volatile unsigned int CSC_COEFF11;
    volatile unsigned int CSC_COEFF12;
    volatile unsigned int CSC_COEFF13;
    volatile unsigned int CSC_COEFF21;
    volatile unsigned int CSC_COEFF22;
    volatile unsigned int CSC_COEFF23;
    volatile unsigned int CSC_COEFF31;
    volatile unsigned int CSC_COEFF32;
    volatile unsigned int CSC_COEFF33;
    volatile unsigned int CSC_COEFF41;
    volatile unsigned int CSC_COEFF42;
    volatile unsigned int CSC_COEFF43;
    volatile unsigned int CSC_OFF1;
    volatile unsigned int CSC_OFF2;
    volatile unsigned int CSC_OFF3;
    volatile unsigned int CSC_OFF4;
    // 0x0D4
    volatile unsigned int OUTPUT_CFG;
    volatile unsigned int PREVIEW_CFG;
    volatile unsigned int FIFO_FLUSH;
    // 0x0E0
    volatile unsigned int DMA0_CFG;
    volatile unsigned int DMA0_START;
    volatile unsigned int DMA0_LINE_OFF;
    volatile unsigned int DMA0_LINE_WIDTH;
    volatile unsigned int DMA0_START_SHADOW;
    volatile unsigned int rsrvd2[3];
    volatile unsigned int DMA0_LEN;
    volatile unsigned int DMA0_LEN_SHADOW;
    volatile unsigned int DMA0_STATUS;

    volatile unsigned int DMA1_CFG;
    volatile unsigned int DMA1_START;
    volatile unsigned int DMA1_LINE_OFF;
    volatile unsigned int DMA1_LINE_WIDTH;
    volatile unsigned int rsrvd3[1];
    volatile unsigned int DMA1_START_SHADOW;
    volatile unsigned int DMA1_LEN;
    volatile unsigned int DMA1_LEN_SHADOW;
    volatile unsigned int DMA1_STATUS;

    volatile unsigned int DMA2_CFG;
    volatile unsigned int DMA2_START;
    volatile unsigned int DMA2_LINE_OFF;
    volatile unsigned int DMA2_LINE_WIDTH;
    volatile unsigned int DMA2_START_SHADOW;
    volatile unsigned int DMA2_LEN;
    volatile unsigned int DMA2_LEN_SHADOW;
    volatile unsigned int DMA2_STATUS;
    volatile unsigned int rsrvd4[8];
    // 0x170
    volatile unsigned int HSYNC_WIDTH;
    volatile unsigned int H_BACKPORCH;
    volatile unsigned int H_ACTIVEWIDTH;
    volatile unsigned int H_FRONTPORCH;
    volatile unsigned int VSYNC_WIDTH;
    volatile unsigned int V_BACKPORCH;
    volatile unsigned int V_ACTIVEHEIGHT;
    volatile unsigned int V_FRONTPORCH;
    volatile unsigned int VSYNC_START_OFFSET;
    volatile unsigned int VSYNC_END_OFFSET;
    // 0x198
    volatile struct CifPwmRegisters PWM[3];
    volatile unsigned int rsrvd8;
    // 0x1C0
    volatile unsigned int RAM_CTRL;
    volatile unsigned int rsrvd5[399];
    // 0x800
    volatile unsigned char LUT0[256];
    volatile unsigned char LUT1[256];
    volatile unsigned char LUT2[256];
};

struct FramePumpHwContextCif {
    struct FramePumpHwContext ctx;
    struct CifRegisters *baseAddr;
    struct FramePumpBuffer currentWriteBuffer;
    struct FramePumpBuffer nextWriteBuffer;
    int device;
    uint64_t lastTimestampNs;
    uint32_t lastIntStatus;
    rtems_id taskId;
};

struct cifConf_t {
    uint32_t input;
    uint32_t output;
    uint32_t preview;
    uint32_t control;
};

struct cifDmaConf_t {
    uint8_t widthDivider;
    uint8_t heightDivider;
    uint8_t bpp;
    uint16_t burstSize;
};

// Please make sure that this array has the same order as the swcFrameType
static const struct cifConf_t inOutPrevCtrlMatrix[] =
{
    [YUV422i] = {
        D_CIF_INFORM_FORMAT_YUV422 | D_CIF_INFORM_DAT_SIZE_8,
        D_CIF_OUTF_FORMAT_422,
        0,
        0
    },
    [YUV420p] = { // YUV420p
        D_CIF_INFORM_FORMAT_YUV422 | D_CIF_INFORM_DAT_SIZE_8,
        D_CIF_OUTF_FORMAT_420 | D_CIF_OUTF_CHROMA_SUB_CO_SITE_CENTER |
        D_CIF_OUTF_STORAGE_PLANAR | D_CIF_OUTF_Y_AFTER_CBCR,
        0,
        0
    },
    [YUV422p] = {0, 0, 0, 0},//YUV422p,
    [YUV400p] = {0, 0, 0, 0},//YUV400p,
    [RGBA8888]= {0, 0, 0, 0},//RGBA8888,
    [RGB888]  = {0, 0, 0, 0},//RGB888,
    [LUT2]    = {0, 0, 0, 0},//LUT2,
    [LUT4]    = {0, 0, 0, 0},//LUT4,
    [LUT16]   = {0, 0, 0, 0},//LUT16,
    [RAW16]   = {D_CIF_INFORM_FORMAT_RGB_BAYER | D_CIF_INFORM_DAT_SIZE_16,
                 0,
                 D_CIF_PREV_OUTF_RGB_BY | D_CIF_PREV_RAW_BPP_2,
                 D_CIF_CTRL_RGB_OUT_EN },
    [RAW8]    = {D_CIF_INFORM_FORMAT_RGB_BAYER | D_CIF_INFORM_DAT_SIZE_8,
                 0,
                 D_CIF_PREV_OUTF_RGB_BY | D_CIF_PREV_RAW_BPP_1,
                 D_CIF_CTRL_RGB_OUT_EN },
    [YUV444i] = {0, 0, 0, 0},//YUV444i,
    [NV12]    = {0, 0, 0, 0},//NV12,
};

static const struct cifDmaConf_t dmaConfMatrix[][3] =
{
    [YUV422i] = {{0},{0},{0}}, //YUV422i,
    [YUV420p] = {{0},{0},{0}}, //YUV420p,
    [YUV422p] = {{0},{0},{0}}, //YUV422p,
    [YUV400p] = {{0},{0},{0}}, //YUV400p,
    [RGBA8888]= {{0},{0},{0}}, //RGBA8888,
    [RGB888]  = {{0},{0},{0}}, //RGB888,
    [LUT2]    = {{0},{0},{0}}, //LUT2,
    [LUT4]    = {{0},{0},{0}}, //LUT4,
    [LUT16]   = {{0},{0},{0}}, //LUT16,
    [RAW16]   = {{ 1, 1, 2, D_CIF_DMA_AXI_BURST_8 },
                 {0,0,0,0},
                 {0,0,0,0}},
    [RAW8]    = {{ 1, 1, 1, D_CIF_DMA_AXI_BURST_8 },
                 {0,0,0,0},
                 {0,0,0,0}},
    [YUV444i] = {{0},{0},{0}}, //YUV444i,
    [NV12]    = {{0},{0},{0}}, //NV12,
};

// Functions Implementation
// ----------------------------------------------------------------------------

rtems_status_code cifInit (
    struct FramePumpVideoContext *ctx,
    struct FramePumpVideoDevice *vd)
{
    struct FramePumpHwContextCif *hwCtx = NULL;
    struct FramePumpVideoDeviceSensor *sensorVD = NULL;
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    hwCtx = (struct FramePumpHwContextCif *)
        calloc(1, sizeof(struct FramePumpHwContextCif));
    if (hwCtx == NULL) {
        return RTEMS_NO_MEMORY;
    } else {
        ctx->hwCtx = (struct FramePumpHwContext *)hwCtx;
    }

    sensorVD = (struct FramePumpVideoDeviceSensor *) vd;

    if(sensorVD->spec == NULL) {
        sc = RTEMS_INVALID_ADDRESS;
        return sc;
    }
    ctx->spec = sensorVD->spec;

    mvLog(MVLOG_DEBUG, "%s: %s:%d\n", __FILE__ , __func__, __LINE__);

    hwCtx->ctx.ctrlNo = sensorVD->ctrlNo;
    switch (hwCtx->ctx.ctrlNo) {
    case MIPI_CTRL_0:
    case MIPI_CTRL_1:
        hwCtx->baseAddr = (struct CifRegisters *) CIF1_BASE_ADR;
        hwCtx->device = 0;
        hwCtx->ctx.irqVector = ROUTED_IRQ_CIF0;
        hwCtx->ctx.clk = DEV_MSS_CIF0;
        DrvIcbDynamicIrqConfig(LRT_TO_LOS,
                               IRQ_CIF0,
                               ctx->hwCtx->irqVector,
                               1);
        break;
    case MIPI_CTRL_3:
    case MIPI_CTRL_5:
        hwCtx->baseAddr = (struct CifRegisters *) CIF2_BASE_ADR;
        hwCtx->device = 1;
        hwCtx->ctx.irqVector = ROUTED_IRQ_CIF1;
        hwCtx->ctx.clk = DEV_MSS_CIF0;
        DrvIcbDynamicIrqConfig(LRT_TO_LOS,
                               IRQ_CIF1,
                               ctx->hwCtx->irqVector,
                               1);
        break;
    default:
        return RTEMS_INVALID_NUMBER;
    }

    mvLog(MVLOG_DEBUG, "cifInit %s: %s:%d  %p\n", __FILE__ , __func__, __LINE__, hwCtx->baseAddr);

    mvLog(MVLOG_DEBUG, "%s: %s:%d\n", __FILE__ , __func__, __LINE__);

    sc = DrvMssConnectMipiToDevice(hwCtx->ctx.ctrlNo, DRV_MSS_CIF);
    if (sc != 1) {
        return RTEMS_IO_ERROR;
    }

    mvLog(MVLOG_DEBUG, "%s: %s:%d\n", __FILE__ , __func__, __LINE__);

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code cifConfigure (
    struct CifRegisters *cifAddr,
    const struct FramePumpSensorDriver *spec,
    struct FramePumpOutSpec *outSpec)
{
    frameType pixelFormat = outSpec->spec.type;
    uint32_t hbp = 0;
    uint32_t hfp = 0;
    uint32_t vbp = 0;
    uint32_t vfp = 0;
    uint32_t hardPrgHeight = 0;

    hardPrgHeight = spec->height;
    if (outSpec->hasMetadata) {
        hardPrgHeight +=spec->metaROI.lines;
    }

    switch (spec->type) {
    case FP_PARALLEL:
        hbp = spec->spec.parallel.hBackPorch;
        hfp = spec->spec.parallel.hFrontPorch;
        vbp = spec->spec.parallel.vBackPorch;
        vfp = spec->spec.parallel.vFrontPorch;
        break;
    case FP_MIPI:
        hbp = hfp = vbp = vfp = 0;
        break;
    case FP_UNKNOWN:
    default:
        return RTEMS_INVALID_NUMBER ;
    }

    DrvCifTimingCfg ((uint32_t)cifAddr, spec->width, hardPrgHeight,
                     hbp, hfp, vbp, vfp);

    DrvCifInOutCfg ((uint32_t)cifAddr,
                         inOutPrevCtrlMatrix[pixelFormat].input,
                         D_CIF_IN_PIX_CLK_CPR,
                         inOutPrevCtrlMatrix[pixelFormat].output,
                         inOutPrevCtrlMatrix[pixelFormat].preview);

    DrvCifWindowing((uint32_t)cifAddr,
                         spec->windowRowStart,
                         spec->windowColumnStart,
                         spec->width,
                         hardPrgHeight);

    DrvCifCtrlCfg ((uint32_t)cifAddr, spec->width, hardPrgHeight,
                        inOutPrevCtrlMatrix[pixelFormat].control |
                        D_CIF_CTRL_WINDOW_EN | D_CIF_CTRL_STATISTICS_FULL);

    if (dmaConfMatrix[pixelFormat][0].widthDivider &&
                    dmaConfMatrix[pixelFormat][0].heightDivider) {
        DrvCifDma0CfgPP((uint32_t)cifAddr,
                        (unsigned int) NULL,
                        (unsigned int) NULL,
                        spec->width / dmaConfMatrix[pixelFormat][0].widthDivider,
                        hardPrgHeight / dmaConfMatrix[pixelFormat][0].heightDivider,
                        dmaConfMatrix[pixelFormat][0].bpp,
                        CIF_DMA_MODE | dmaConfMatrix[pixelFormat][0].burstSize,
                        0);

        mvLog(MVLOG_DEBUG, "%s: %s:%d\n", __FILE__ , __func__, __LINE__);

        if (dmaConfMatrix[pixelFormat][1].widthDivider &&
                        dmaConfMatrix[pixelFormat][1].heightDivider) {
            DrvCifDma1CfgPP((uint32_t)cifAddr,
                            (unsigned int) NULL,
                            (unsigned int) NULL,
                            spec->width / dmaConfMatrix[pixelFormat][1].widthDivider,
                            hardPrgHeight / dmaConfMatrix[pixelFormat][1].heightDivider,
                            dmaConfMatrix[pixelFormat][1].bpp,
                            CIF_DMA_MODE | dmaConfMatrix[pixelFormat][1].burstSize,
                            0);
            mvLog(MVLOG_DEBUG, "%s: %s:%d\n", __FILE__ , __func__, __LINE__);
        }

        if (dmaConfMatrix[pixelFormat][2].widthDivider &&
                        dmaConfMatrix[pixelFormat][2].heightDivider) {
            DrvCifDma2CfgPP((uint32_t)cifAddr,
                            (unsigned int) NULL,
                            (unsigned int) NULL,
                            spec->width / dmaConfMatrix[pixelFormat][2].widthDivider,
                            hardPrgHeight / dmaConfMatrix[pixelFormat][2].heightDivider,
                            dmaConfMatrix[pixelFormat][2].bpp,
                            CIF_DMA_MODE | dmaConfMatrix[pixelFormat][2].burstSize,
                            0);
            mvLog(MVLOG_DEBUG, "%s: %s:%d\n", __FILE__ , __func__, __LINE__);
        }
    } else {
        return RTEMS_INVALID_ID;
    }

    return RTEMS_SUCCESSFUL;
}

static void CifCamHandler (void * context)
{
    struct FramePumpVideoContext *ctx = (struct FramePumpVideoContext *) context;
    struct FramePumpHwContextCif *hwCtx =
                    (struct FramePumpHwContextCif *)ctx->hwCtx;

    // First thing first ! Get timestamp
    hwCtx->lastTimestampNs = getTimestampNs();

    rtems_event_send(hwCtx->taskId, EVENT_DMA0_DONE);

    hwCtx->lastIntStatus = hwCtx->baseAddr->INT_STATUS;
    hwCtx->baseAddr->INT_CLEAR = 0xFFFFFFFF;

    mvLog(MVLOG_DEBUG, "IRQ at %llu", hwCtx->lastTimestampNs);
}

rtems_status_code cifOpen (struct FramePumpVideoContext *ctx)
{
    struct FramePumpHwContextCif *hwCtx =
                    (struct FramePumpHwContextCif *)ctx->hwCtx;
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    char name[sizeof(ISR_NAME_STEM) + MAX_ISR_DIGITS];

    if (InitMIPI(ctx->hwCtx->ctrlNo, ctx->spec) != DRV_MIPI_CFG_DONE) {
        return RTEMS_IO_ERROR;
    }

    mvLog(MVLOG_DEBUG, "%s: %s:%d\n", __FILE__ , __func__, __LINE__);

    hwCtx->baseAddr->INT_CLEAR = 0xFFFFFFFF;
    hwCtx->baseAddr->INT_ENABLE = D_CIF_INT_DMA0_DONE;

    // very carefully clear pending interrupts before enabling them
    BSP_Clear_interrupt(ctx->hwCtx->irqVector);

    // Set Priority and level
    BSP_Set_interrupt_type_priority(ctx->hwCtx->irqVector,
                                    POS_EDGE_INT,
                                    14); // TODO: irq priority

    // Generate ISR name.
    snprintf(name, sizeof(name), ISR_NAME_STEM "%d", (int)hwCtx->device);

    /* Install interrupt handler and disable this vector */
    sc = rtems_interrupt_handler_install(ctx->hwCtx->irqVector,
                                         name,
                                         RTEMS_INTERRUPT_UNIQUE,
                                         CifCamHandler,
                                         (void*)ctx);
    return sc;
}

rtems_status_code cifStop (struct FramePumpVideoContext *ctx);

rtems_status_code cifClose (struct FramePumpVideoContext *ctx)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    struct FramePumpHwContextCif *hwCtx =
        (struct FramePumpHwContextCif *)ctx->hwCtx;

    // if not explicitly stopped before
    if (hwCtx->taskId) {
      cifStop(ctx);
    }

    return sc;
}

rtems_status_code cifStart (struct FramePumpVideoContext *ctx)
{
    struct FramePumpHwContextCif *hwCtx =
        (struct FramePumpHwContextCif *)ctx->hwCtx;
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    sc = rtems_task_create(rtems_build_name( 'C', 'I', 'F', '0' + hwCtx->device ),
                           2, // HIGHEST PRIORITY !!! hardware task
                           RTEMS_MINIMUM_STACK_SIZE,
                           RTEMS_NO_PREEMPT | RTEMS_NO_TIMESLICE,
                           RTEMS_DEFAULT_ATTRIBUTES,
                           &hwCtx->taskId );
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_FATAL, "Cannot create thread!!!\n");
        return sc;
    }

    sc = rtems_task_start( hwCtx->taskId, commonTask,
                           (rtems_task_argument)ctx );
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_DEBUG, "Cannot start thread!!!\n");
        return sc;
    }

    mvLog(MVLOG_DEBUG, "%s: %s:%d\n", __FILE__ , __func__, __LINE__);

    return RTEMS_SUCCESSFUL;
}

rtems_status_code cifStop (struct FramePumpVideoContext *ctx)
{
    rtems_event_set ev = 0;
    struct FramePumpHwContextCif *hwCtx =
            (struct FramePumpHwContextCif *)ctx->hwCtx;

    int taskId = hwCtx->taskId;
    hwCtx->taskId = rtems_task_self();

    // In case we have started streaming
    // make sure the task stops before closing device
    if (taskId) {
        // Trigger the stop
        rtems_event_send(taskId, EVENT_STOP);

        rtems_event_receive(EVENT_STOP,
                            RTEMS_EVENT_ALL,
                            0,
                            &ev);
    }

    hwCtx->taskId = 0;
    hwCtx->ctx.frameCount = 0;
    memset(&hwCtx->currentWriteBuffer, 0, sizeof(struct FramePumpBuffer));
    memset(&hwCtx->nextWriteBuffer, 0, sizeof(struct FramePumpBuffer));

    return RTEMS_SUCCESSFUL;
}

rtems_status_code cifEof (struct FramePumpVideoContext *ctx)
{
    struct FramePumpHwContextCif *hwCtx =
        (struct FramePumpHwContextCif *)ctx->hwCtx;
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    struct FramePumpBuffer fpb = {0};
    rtems_event_set ev = 0;
    int level = 0;

    mvLog(MVLOG_DEBUG, "EOF at %llu", hwCtx->lastTimestampNs);

    // Put the newly wrote buffer in the output queue
    if (hwCtx->currentWriteBuffer.fbMem.p1 &&
        hwCtx->currentWriteBuffer.fbMem.p1 != hwCtx->nextWriteBuffer.fbMem.p1) {

        hwCtx->currentWriteBuffer.timestampNs = hwCtx->lastTimestampNs;
        hwCtx->currentWriteBuffer.frameCount = hwCtx->ctx.frameCount++;

        hwCtx->currentWriteBuffer.fb.spec.width  = ctx->spec->width;
        hwCtx->currentWriteBuffer.fb.spec.height = ctx->spec->height;

        if (ctx->spec->ctlFnc->frameCb) {
            ctx->spec->ctlFnc->frameCb(ctx, &hwCtx->currentWriteBuffer);
        }

        if (CONFIG_TEARING_CHECK) {
            insertTearingMarkers(&hwCtx->currentWriteBuffer);
        }

        sendFilledFrame(ctx, hwCtx->currentWriteBuffer);
    }

    hwCtx->currentWriteBuffer = hwCtx->nextWriteBuffer;

    // Try to grab new available frame
    fpb = getEmptyFrame (ctx);

    if (fpb.fbMem.p1 == NULL) {
        sc = -1;
    } else {
        mvLog(MVLOG_DEBUG, "Got frame CIF %p !!!", fpb.fbMem.p1);

        rtems_interrupt_disable(level);

        hwCtx->nextWriteBuffer = fpb;

        rtems_event_receive(EVENT_DMA0_DONE,
                            RTEMS_NO_WAIT | RTEMS_EVENT_ANY,
                            0,
                            &ev);
        rtems_interrupt_enable(level);
        sc = RTEMS_SUCCESSFUL;
    }

    DrvCifDma0StartPP((uint32_t)hwCtx->baseAddr,
                      (uint32_t)hwCtx->nextWriteBuffer.fbMem.p1,
                      (uint32_t)hwCtx->nextWriteBuffer.fbMem.p1);  //address update + DMA restart
    hwCtx->baseAddr->CONTROL |= D_CIF_CTRL_ENABLE;

    mvLog(MVLOG_DEBUG, "EOF done at %llu", hwCtx->lastTimestampNs);

    return sc;
}

static rtems_status_code cifEol (struct FramePumpVideoContext *ctx)
{
    (void)ctx;
    return RTEMS_SUCCESSFUL;
}

static rtems_status_code cifStopHandler (struct FramePumpVideoContext *ctx)
{
    struct FramePumpHwContextCif *hwCtx =
        (struct FramePumpHwContextCif *)ctx->hwCtx;
    rtems_event_set ev = 0;
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    // Wait for DMA DONE (otherwise JTAG FLOW CONTROL ERROR !!!)
    sc = rtems_event_receive(EVENT_DMA0_DONE,
                             RTEMS_EVENT_ANY,
                             50 * rtems_clock_get_ticks_per_second() / 1000,
                             &ev);

    // stop CIF and clear FIFOs
    hwCtx->baseAddr->FIFO_FLUSH = 1;

    // TODO: Find way to eliminate hard-reset below
    DrvCifReset(hwCtx->device);

    sc = rtems_interrupt_handler_remove(ctx->hwCtx->irqVector, CifCamHandler, (void*)ctx);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_DEBUG, "Unable to unregister\n");
    }

    // suicide
    rtems_event_send(hwCtx->taskId, EVENT_STOP);
    rtems_task_delete(rtems_task_self());

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code cifStartHandler (struct FramePumpVideoContext *ctx)
{
    struct FramePumpHwContextCif *hwCtx =
        (struct FramePumpHwContextCif *)ctx->hwCtx;
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    sc = cifConfigure(hwCtx->baseAddr, ctx->spec, &ctx->outSpec);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_FATAL, "Cannot configure CIF");
        return sc;
    }

    // Fake a EOF for frame sequence corectness
    rtems_event_send(hwCtx->taskId, EVENT_DMA0_DONE);

    return RTEMS_SUCCESSFUL;
}

struct FramePumpHwFunc cifFunc =
{
    .init = cifInit,
    .open = cifOpen,
    .close = cifClose,

    .start = cifStart,
    .stop = cifStop,

    .eof = cifEof,
    .eol = cifEol,
    .stopHandler = cifStopHandler,
    .startHandler = cifStartHandler
};
