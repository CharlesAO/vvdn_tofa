///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief FramePump SIPP RX backend implementation
///

#include "FramePumpSipp.h"
#include "FramePumpMipi.h"
#include "FramePumpCommon.h"
#include "registersMyriad.h"
#include <malloc.h>
#include "DrvSipp.h"
#include <DrvIcb.h>
#include <DrvCpr.h>
#include <DrvMss.h>
#include <rtems.h>
#include <bsp.h>

#include <OsDrvTimer.h>
#include <OsDrvCmxDma.h>

#define MVLOG_UNIT_NAME FramePumpSipp
#include <mvLog.h>

#ifndef FRAMEPUMP_SIPPCMX_IRQ_RATE
#define FRAMEPUMP_SIPPCMX_IRQ_RATE (3)
#endif

#ifndef FRAMEPUMP_SIPPCMX_BUFFER_COUNT
#define FRAMEPUMP_SIPPCMX_BUFFER_COUNT (4)
#endif

#ifndef FRAMEPUMP_SIPPCMX_MEMORY_POOL_SIZE_BYTES
#define FRAMEPUMP_SIPPCMX_MEMORY_POOL_SIZE_BYTES (0 * 1024)
#endif

#ifndef FRAMEPUMP_SIPPCMX_MEMORY_POOL_SECTION
#define FRAMEPUMP_SIPPCMX_MEMORY_POOL_SECTION ".cmx.bss"
#endif

#ifndef CONFIG_TEARING_CHECK
#define CONFIG_TEARING_CHECK (0)
#endif

#define CB_LINES(IRQ_RATE, BUFF_NUM) ((1<<IRQ_RATE) * BUFF_NUM)

#ifndef ROUTED_IRQ_SIPP_LINE_ALL
#define ROUTED_IRQ_SIPP_LINE_ALL   IRQ_DYNAMIC_2 // all 3 SIPP line isr are presented on the same line
#endif

#ifndef ROUTED_IRQ_SIPP_FRAME_ALL
#define ROUTED_IRQ_SIPP_FRAME_ALL  IRQ_DYNAMIC_4 // all 3 SIPP frame isr are presented on the same line
#endif

#define SIPP_BASE_ADR SIPP_BASE2_ADR

// For naming the ISR
#define ISR_NAME_STEM "ISR_SIPP_"
#define MAX_ISR_DIGITS (5)

struct sippRegisters {
    volatile uint32_t SOFT_RESET;
    volatile uint32_t CONTROL;
    volatile uint32_t START;
    volatile uint32_t IBFL_INC;
    volatile uint32_t OBFL_DEC;
    volatile uint32_t OPIPE_CFG;
    volatile uint32_t STATUS;
    volatile uint32_t INT0_STATUS;
    volatile uint32_t INT0_ENABLE;
    volatile uint32_t INT0_CLEAR;
    volatile uint32_t INT1_STATUS;
    volatile uint32_t INT1_ENABLE;
    volatile uint32_t INT1_CLEAR;
    volatile uint32_t INT2_STATUS;
    volatile uint32_t INT2_ENABLE;
    volatile uint32_t INT2_CLEAR;

    volatile uint32_t INT0_BARRIER0;
    volatile uint32_t INT0_BARRIER1;
    volatile uint32_t INT0_BARRIER2;
    volatile uint32_t INT0_BARRIER3;
    volatile uint32_t INT1_BARRIER0;
    volatile uint32_t INT1_BARRIER1;
    volatile uint32_t INT1_BARRIER2;
    volatile uint32_t INT1_BARRIER3;
    volatile uint32_t INT2_BARRIER0;
    volatile uint32_t INT2_BARRIER1;
    volatile uint32_t INT2_BARRIER2;
    volatile uint32_t INT2_BARRIER3;
    volatile uint32_t DISAB_EOF;
    volatile uint32_t INT2_CFG;
    volatile uint32_t SLC_SIZE;
    volatile uint32_t SHADOW_SELECT;
    volatile uint32_t UPFIRDN0_VPHASE;
    volatile uint32_t UPFIRDN1_VPHASE;
    volatile uint32_t UPFIRDN2_VPHASE;
    volatile uint32_t IBUF_SHADOW_OFF;
    volatile uint32_t OBUF_SHADOW_OFF;
};

struct FramePumpSippDebug
{
    volatile uint32_t sipplineCounter;
    volatile uint32_t sippframeCounter;

    tyTimeStamp irqstamp;
    uint64_t irqticks;
};

struct FramePumpHwContextSipp
{
    struct FramePumpHwContext ctx;
    struct sippRegisters *baseAddr;
    struct sipp_buf      *outputAddr;
    struct FramePumpBuffer currentWriteBuffer;
    struct FramePumpBuffer nextWriteBuffer;
    int device;
    uint32_t idMask;
    uint64_t lastTimestampNs;
    rtems_id taskId;

    uint64_t frameIrqTimestamp;
    uint64_t lineIrqTimestamp;
};

struct FramePumpHwContextSippCmx
{
    struct FramePumpHwContextSipp sippCommon;

    int lastReadLineCB;
    int lastWrittenLineOutput;
    void * cmxBuffer;
    int lastOCBL;
    int irqRate;
    int cbLines;
    int nBuffers;
    dmaRequesterId reqId;
};

__attribute__((section(".cmx.cdmaDescriptors")))
dmaTransactionList_t sippRxTasks[4];

__attribute__((section(FRAMEPUMP_SIPPCMX_MEMORY_POOL_SECTION)))
static uint8_t framePumpSippCmxLineBuffers[FRAMEPUMP_SIPPCMX_MEMORY_POOL_SIZE_BYTES];
static size_t framePumpSippCmxLineBuffersIndex;

// Functions Declarations
// ----------------------------------------------------------------------------
static rtems_status_code sippCommonInit (struct FramePumpVideoContext *ctx, struct FramePumpVideoDevice *vd);
static rtems_status_code sippEof (struct FramePumpVideoContext *ctx);
static rtems_status_code sippDirectConfigure (struct FramePumpHwContextSipp *hwCtx,
                               const struct FramePumpSensorDriver *spec,
                               struct FramePumpOutSpec *outSpec);
static rtems_status_code sippCommonOpen (struct FramePumpVideoContext *ctx);
static rtems_status_code sippCmxConfigure (struct FramePumpHwContextSippCmx *hwCtx,
                            const struct FramePumpSensorDriver *spec,
                            struct FramePumpOutSpec *outSpec);
static void SippLineHandler (void * context);
static void SippFrameHandler (void * context);
static rtems_status_code sippCommonConfigure (const struct FramePumpSensorDriver *spec,
                               struct FramePumpOutSpec *outSpec,
                               sipp_buf_t *sippMipiRxBuf,
                               sipp_rx_t *sippMipiRxCfg );
static rtems_status_code sippDirectEof (struct FramePumpVideoContext *ctx);
static rtems_status_code sippCmxEof (struct FramePumpVideoContext *ctx);
static rtems_status_code sippCommonStopHandler (struct FramePumpVideoContext *ctx);


// Functions Implementation
// ----------------------------------------------------------------------------

static rtems_status_code installISR (
    uint32_t id,
    uint32_t idMask,
    uint32_t vector,
    volatile uint32_t *clearReg,
    volatile uint32_t *enableReg,
    rtems_interrupt_handler handler,
    void *context)
{
    char name[sizeof(ISR_NAME_STEM) + MAX_ISR_DIGITS];
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    uint32_t status = 0;

    rtems_interrupt_disable(status);
    *clearReg   = idMask;
    *enableReg |= idMask;
    rtems_interrupt_enable(status);

    // Very carefully clear pending interrupts before enabling them.
    BSP_Clear_interrupt(vector);

    // Set priority and level.
    BSP_Set_interrupt_type_priority(vector, POS_EDGE_INT, 14);

    // Generate ISR name.
    snprintf(name, sizeof(name), ISR_NAME_STEM "%d", (int)id);

    sc = rtems_interrupt_handler_install(
        vector, name, RTEMS_INTERRUPT_SHARED, handler, context);
    return sc;
}

static rtems_status_code removeISR (
    uint32_t vector,
    uint32_t idMask,
    volatile uint32_t *enableReg,
    rtems_interrupt_handler handler,
    void *context)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    uint32_t status = 0;

    rtems_interrupt_disable(status);
    *enableReg &= ~idMask;
    rtems_interrupt_enable(status);

    sc = rtems_interrupt_handler_remove(vector, handler, context);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_DEBUG, "Unable to unregister");
    }
    return sc;
}

static void *allocateLineBuffers (uint32_t irqRate, uint32_t nBuffers, uint32_t lineWidthBytes)
{
    size_t requiredSize = 0;
    void *buffer = NULL;
    uint32_t status = 0;
    rtems_interrupt_disable(status);

    requiredSize = CB_LINES(irqRate, nBuffers) * lineWidthBytes;

    if (framePumpSippCmxLineBuffersIndex + requiredSize >
       sizeof(framePumpSippCmxLineBuffers)) {
        rtems_interrupt_enable(status);
        mvLog(MVLOG_FATAL, "Not enough memory for FramePump with SIPP CMX");
        mvLog(MVLOG_FATAL, "Please increase FRAMEPUMP_SIPPCMX_MEMORY_POOL_SIZE_BYTES define");
        mvLog(MVLOG_FATAL, "to accomodate %d bytes\n", requiredSize);
        return NULL;
    }

    buffer = &framePumpSippCmxLineBuffers[framePumpSippCmxLineBuffersIndex];
    framePumpSippCmxLineBuffersIndex += requiredSize;

    mvLog(MVLOG_DEBUG, "\nframePumpSippCmxLineBuffersIndex: %d; addr: 0x%x\n",
          framePumpSippCmxLineBuffersIndex, &framePumpSippCmxLineBuffers[framePumpSippCmxLineBuffersIndex]);

    rtems_interrupt_enable(status);
    return buffer;
}

static rtems_status_code sippDirectInit (struct FramePumpVideoContext *ctx, struct FramePumpVideoDevice *vd)
{
    struct FramePumpHwContextSipp *hwCtx = NULL;
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    hwCtx = (struct FramePumpHwContextSipp *)
        calloc(1, sizeof(struct FramePumpHwContextSipp));
    if (hwCtx == NULL) {
        return RTEMS_NO_MEMORY;
    } else {
        ctx->hwCtx = (struct FramePumpHwContext *)hwCtx;
    }

    sc = sippCommonInit(ctx, vd);

    return sc;
}

static rtems_status_code sippCmxInit (struct FramePumpVideoContext *ctx, struct FramePumpVideoDevice *vd)
{
    struct FramePumpHwContextSippCmx *hwCtx = NULL;
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    hwCtx = (struct FramePumpHwContextSippCmx *)
        calloc(1, sizeof(struct FramePumpHwContextSippCmx));
    if (hwCtx == NULL) {
        return RTEMS_NO_MEMORY;
    } else {
        ctx->hwCtx = (struct FramePumpHwContext *)hwCtx;
    }

    sc = sippCommonInit(ctx, vd);

    return sc;
}

static rtems_status_code sippCommonInit (struct FramePumpVideoContext *ctx, struct FramePumpVideoDevice *vd)
{
    struct FramePumpVideoDeviceSensor *sensorVD = NULL;
    struct FramePumpHwContextSipp *hwCtx = (struct FramePumpHwContextSipp *)ctx->hwCtx;
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    sensorVD = (struct FramePumpVideoDeviceSensor *) vd;

    if (sensorVD->spec == NULL) {
        sc = RTEMS_INVALID_ADDRESS;
        return sc;
    } else {
        ctx->spec = sensorVD->spec;
    }

    hwCtx->ctx.ctrlNo = sensorVD->ctrlNo;
    switch (hwCtx->ctx.ctrlNo) {
    case MIPI_CTRL_0:
    case MIPI_CTRL_1:
        hwCtx->device = 0;
        hwCtx->outputAddr = (struct sipp_buf *) SIPP_MIPI_RX0_BUF_BASE_ADR;
        hwCtx->idMask = SIPP_MIPI_RX0_ID_MASK;
        break;
    case MIPI_CTRL_2:
        hwCtx->device = 1;
        hwCtx->outputAddr = (struct sipp_buf *) SIPP_MIPI_RX1_BUF_BASE_ADR;
        hwCtx->idMask = SIPP_MIPI_RX1_ID_MASK;
        break;
    case MIPI_CTRL_3:
        hwCtx->device = 2;
        hwCtx->outputAddr = (struct sipp_buf *) SIPP_MIPI_RX2_BUF_BASE_ADR;
        hwCtx->idMask = SIPP_MIPI_RX2_ID_MASK;
        break;
    case MIPI_CTRL_4:
        hwCtx->device = 3;
        hwCtx->outputAddr = (struct sipp_buf *) SIPP_MIPI_RX3_BUF_BASE_ADR;
        hwCtx->idMask = SIPP_MIPI_RX3_ID_MASK;
        break;
    default:
        return RTEMS_INVALID_NUMBER;
    }
    hwCtx->baseAddr = (struct sippRegisters *) SIPP_BASE_ADR;

    hwCtx->ctx.clk = DEV_MSS_SIPP;
    hwCtx->ctx.irqVector = ROUTED_IRQ_SIPP_FRAME_ALL;
    hwCtx->ctx.lineIrqVector = ROUTED_IRQ_SIPP_LINE_ALL;

    DrvIcbDynamicIrqConfig(LRT_TO_LOS, IRQ_SIPP_2, ctx->hwCtx->irqVector, 1);
    DrvIcbDynamicIrqConfig(LRT_TO_LOS, IRQ_SIPP_1, ctx->hwCtx->lineIrqVector, 1);

    sc = DrvMssConnectMipiToDevice(hwCtx->ctx.ctrlNo, DRV_MSS_SIPP_RX);
    if (sc != 1) {
        return RTEMS_IO_ERROR;
    }

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code sippDirectOpen (struct FramePumpVideoContext *ctx)
{
    return sippCommonOpen(ctx);
}

static rtems_status_code sippDirectClose (struct FramePumpVideoContext *ctx)
{
    struct FramePumpHwContextSipp *hwCtx =
        (struct FramePumpHwContextSipp *)ctx->hwCtx;

    removeISR(ctx->hwCtx->irqVector, hwCtx->idMask,
              &hwCtx->baseAddr->INT2_ENABLE,
              SippFrameHandler,
              (void*)ctx);

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code sippCmxOpen(struct FramePumpVideoContext *ctx)
{
    struct FramePumpHwContextSipp *hwCtx =
        (struct FramePumpHwContextSipp *)ctx->hwCtx;
    struct FramePumpHwContextSippCmx *hwCtxCmx =
        (struct FramePumpHwContextSippCmx *)ctx->hwCtx;
    int errorStatus = MYR_DRV_SUCCESS;
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    // Make sure CMX-DMA was initialized
    errorStatus = OsDrvCmxDmaInitDefault();
    if (errorStatus) {
        mvLog(MVLOG_DEBUG, "CMX DMA already initialized or error : %d! ", errorStatus);
    }

    errorStatus = OsDrvCmxDmaInitRequester(3, &hwCtxCmx->reqId);
    if (errorStatus != MYR_DRV_SUCCESS) {
        mvLog(MVLOG_ERROR, "OsDrvCmxDmaInitRequester error %d", errorStatus);
    }

    sc = sippCommonOpen(ctx);
    if (sc != RTEMS_SUCCESSFUL) {
        return sc;
    }

    sc = installISR(hwCtx->device, hwCtx->idMask, ctx->hwCtx->lineIrqVector,
                    &hwCtx->baseAddr->INT1_CLEAR,
                    &hwCtx->baseAddr->INT1_ENABLE,
                    SippLineHandler, ctx);
    return sc;
}

static rtems_status_code sippCommonOpen (struct FramePumpVideoContext *ctx)
{
    struct FramePumpHwContextSipp *hwCtx =
        (struct FramePumpHwContextSipp *)ctx->hwCtx;
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    inspectQueues(ctx);

    if (InitMIPI(ctx->hwCtx->ctrlNo, ctx->spec) != DRV_MIPI_CFG_DONE) {
        return RTEMS_IO_ERROR;
    }

    sc = installISR(hwCtx->device, hwCtx->idMask, ctx->hwCtx->irqVector,
                    &hwCtx->baseAddr->INT2_CLEAR,
                    &hwCtx->baseAddr->INT2_ENABLE,
                    SippFrameHandler, ctx);
    return sc;
}

rtems_status_code sippStart (struct FramePumpVideoContext *ctx, rtems_name taskName)
{
    struct FramePumpHwContextSipp *hwCtx =
        (struct FramePumpHwContextSipp *)ctx->hwCtx;
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    sc = rtems_task_create(taskName,
                           1, // HIGHEST PRIORITY !!! hardware task
                           RTEMS_MINIMUM_STACK_SIZE,
                           RTEMS_NO_PREEMPT | RTEMS_NO_TIMESLICE,
                           RTEMS_DEFAULT_ATTRIBUTES,
                           &hwCtx->taskId );
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_DEBUG, "Cannot create thread!!!");
        return sc;
    }

    sc = rtems_task_start( hwCtx->taskId, commonTask,
                           (rtems_task_argument)ctx );
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_DEBUG, "Cannot start thread!!!");
        return sc;
    }

    return RTEMS_SUCCESSFUL;
}

rtems_status_code sippDirectStart (struct FramePumpVideoContext *ctx)
{
    struct FramePumpHwContextSipp *hwCtx =
        (struct FramePumpHwContextSipp *)ctx->hwCtx;
    rtems_name taskName = rtems_build_name( 'S', 'I', 'P', '0' + hwCtx->device );
    return sippStart(ctx, taskName);
}

rtems_status_code sippCmxStart (struct FramePumpVideoContext *ctx)
{
    struct FramePumpHwContextSipp *hwCtx =
        (struct FramePumpHwContextSipp *)ctx->hwCtx;
    rtems_name taskName = rtems_build_name( 'S', 'C', 'X', '0' + hwCtx->device );
    return sippStart(ctx, taskName);
}

static rtems_status_code sippDirectConfigure (
    struct FramePumpHwContextSipp *hwCtx,
    const struct FramePumpSensorDriver *spec,
    struct FramePumpOutSpec *outSpec)
{
    sipp_buf_t sippMipiRxBuf = {0};
    sipp_rx_t sippMipiRxCfg = {0};

    sippCommonConfigure(spec, outSpec, &sippMipiRxBuf, &sippMipiRxCfg);

    DrvSippConfigureMipiRxBuffers(hwCtx->device, &sippMipiRxBuf);
    DrvSippConfigureMipiRx(hwCtx->device, &sippMipiRxCfg);

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code sippCmxConfigure (
    struct FramePumpHwContextSippCmx *hwCtx,
    const struct FramePumpSensorDriver *spec,
    struct FramePumpOutSpec *outSpec)
{
    sipp_buf_t sippMipiRxBuf = {0};
    sipp_rx_t sippMipiRxCfg = {0};

    sippCommonConfigure(spec, outSpec, &sippMipiRxBuf, &sippMipiRxCfg);

    hwCtx->irqRate   = FRAMEPUMP_SIPPCMX_IRQ_RATE;
    hwCtx->nBuffers  = FRAMEPUMP_SIPPCMX_BUFFER_COUNT;
    hwCtx->cbLines   = CB_LINES(hwCtx->irqRate, hwCtx->nBuffers);

    if (hwCtx->cmxBuffer == NULL) {
        hwCtx->cmxBuffer = allocateLineBuffers(hwCtx->irqRate, hwCtx->nBuffers,
                                               outSpec->spec.stride);
        if (hwCtx->cmxBuffer == NULL) {
            return RTEMS_NO_MEMORY;
        }
    }

    sippMipiRxBuf.cfg |= hwCtx->cbLines;
    sippMipiRxBuf.lcs = hwCtx->irqRate;

    DrvSippConfigureMipiRxBuffers(hwCtx->sippCommon.device, &sippMipiRxBuf);
    DrvSippConfigureMipiRx(hwCtx->sippCommon.device, &sippMipiRxCfg);

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code sippCommonConfigure (
    const struct FramePumpSensorDriver *spec,
    struct FramePumpOutSpec *outSpec,
    sipp_buf_t *sippMipiRxBuf,
    sipp_rx_t *sippMipiRxCfg )
{
    uint32_t camDataMask = 0;
    uint32_t bitsPPcameraConfig = 0;
    uint32_t bytesPP = outSpec->spec.bytesPP;
    uint32_t bitsPPrequested = bytesPP * 8;
    uint32_t hardPrgHeight = 0;
    int bitsToShift = 0;

    camDataMask = (bitsPPrequested << 8) - 1;
    bitsPPcameraConfig = bitsFromMipiPixelFormat(spec->spec.mipi.pixelFormat);

    if (bitsPPcameraConfig == 0 || bitsPPrequested == 0) {
        return RTEMS_INVALID_SIZE ;
    }

    if (bitsPPcameraConfig > bitsPPrequested) {
        bitsToShift = bitsPPcameraConfig - bitsPPrequested;

        camDataMask <<= bitsToShift;
        sippMipiRxCfg->cfg |= (1 << 8) |  // enable format conversion
            (bitsToShift << 12);         // number of bits to shift
    }

    hardPrgHeight = spec->height;
    if (outSpec->hasMetadata) {
        hardPrgHeight += spec->metaROI.lines;
    }

    sippMipiRxBuf->base = (uint32_t)NULL;
    sippMipiRxBuf->plane_stride = 0;
    sippMipiRxBuf->line_stride = bytesPP * spec->width;
    sippMipiRxBuf->cfg |= bytesPP << 28;

    sippMipiRxCfg->sel01  |= 0x00000f00;
    sippMipiRxCfg->mask0   = camDataMask;
    sippMipiRxCfg->frm_dim = ( (hardPrgHeight << 16) | spec->width);
    sippMipiRxCfg->x0      = ( (spec->width   << 16) | spec->windowColumnStart);
    sippMipiRxCfg->y0      = ( (hardPrgHeight << 16) | spec->windowRowStart);

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code sippDirectStartHandler (struct FramePumpVideoContext *ctx)
{
    struct FramePumpHwContextSipp *hwCtx =
        (struct FramePumpHwContextSipp *)ctx->hwCtx;

    sippDirectConfigure(hwCtx, ctx->spec, &ctx->outSpec);

    sippDirectEof(ctx);

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code sippCmxStartHandler (struct FramePumpVideoContext *ctx)
{
    struct FramePumpHwContextSippCmx *hwCtx =
        (struct FramePumpHwContextSippCmx *)ctx->hwCtx;

    sippCmxConfigure((struct FramePumpHwContextSippCmx *)hwCtx,
                     ctx->spec, &ctx->outSpec);

    sippCmxEof(ctx);

    return RTEMS_SUCCESSFUL;
}

static void SippLineHandler (void * context)
{
    struct FramePumpVideoContext *ctx = (struct FramePumpVideoContext *) context;
    struct FramePumpHwContextSipp *hwCtx =
        (struct FramePumpHwContextSipp *)ctx->hwCtx;
    uint32_t sippCamFrameStatus = 0;
    volatile uint32_t reg = 0;

    sippCamFrameStatus = GET_REG_WORD_VAL(SIPP_INT1_STATUS_ADR);

    // Check if the context is good
    if ((sippCamFrameStatus & hwCtx->idMask) == 0) {
        return;
    }

    hwCtx->baseAddr->INT1_CLEAR = hwCtx->idMask;
    reg = hwCtx->baseAddr->INT1_CLEAR; // TODO: finish transaction on bus (not sure if needed)
    (void)reg;

    DrvTimerStartTicksCount(&hwCtx->lineIrqTimestamp);

    rtems_event_send(hwCtx->taskId, EVENT_EOL_BLOCK);
}

static rtems_status_code sippDirectEol (struct FramePumpVideoContext *ctx)
{
    (void)ctx;
    return RTEMS_SUCCESSFUL;
}

static rtems_status_code sippCmxEol (struct FramePumpVideoContext *ctx)
{
    struct FramePumpHwContextSippCmx *hwCtx =
        (struct FramePumpHwContextSippCmx *)ctx->hwCtx;
    int errorStatus = 0;
    uint64_t irqticks = 0;

    uint32_t context = hwCtx->sippCommon.outputAddr->context;
    int ocbl = (context >> 16) & 0x03FF;
    int lo   = (context >>  0) & 0xFFFF;
    int obfl = (hwCtx->cbLines + ocbl - hwCtx->lastOCBL) % (int)hwCtx->cbLines;
    uint32_t transferSize = 0;
    int status = 0;
    int i = 0;
    int lineStride = hwCtx->sippCommon.currentWriteBuffer.fb.spec.width *
        hwCtx->sippCommon.currentWriteBuffer.fb.spec.bytesPP;

    int firstLine = (hwCtx->cbLines + ocbl - obfl) % (int)hwCtx->cbLines;
    int linesToTransfer =
        (obfl + firstLine) < hwCtx->cbLines ?
        obfl : hwCtx->cbLines - firstLine;

    int firstOutputLine = lo - linesToTransfer - (obfl - linesToTransfer);

#ifdef MA2100
    firstOutputLine = hwCtx->lastWrittenLineOutput;
#endif

    void *srcPtr = NULL;
    void *dstPtr = NULL;

    hwCtx->lastOCBL = ocbl - (obfl - linesToTransfer);

    if (obfl != 0) {
        tyTimeStamp dmastamp;
        uint64_t dmaticks, dmastartticks;

        srcPtr = hwCtx->cmxBuffer + firstLine * lineStride;
        dstPtr = hwCtx->sippCommon.currentWriteBuffer.fbMem.p1 + firstOutputLine * lineStride;
        transferSize = linesToTransfer * lineStride;

        OsDrvTimerGetElapsedTicks(&hwCtx->sippCommon.lineIrqTimestamp,
                                  &irqticks);

        if (hwCtx->sippCommon.currentWriteBuffer.fbMem.p1) {
            errorStatus = OsDrvCmxDmaCreateTransaction(
                hwCtx->reqId, &sippRxTasks[hwCtx->sippCommon.device],
                (uint8_t*)srcPtr, (uint8_t*)dstPtr, transferSize);

            if (errorStatus != MYR_DRV_SUCCESS) {
                mvLog(MVLOG_ERROR, "OsDrvCmxDmaCreateTransaction error %d", errorStatus);
            }

            OsDrvTimerStartTicksCount(&dmastamp);

            errorStatus = OsDrvCmxDmaStartListTask(
                &sippRxTasks[hwCtx->sippCommon.device], &status);

            if (errorStatus != MYR_DRV_SUCCESS) {
                mvLog(MVLOG_ERROR, "OsDrvCmxDmaStartListTask error %d", errorStatus);
            }

            OsDrvTimerGetElapsedTicks(&dmastamp, &dmastartticks);

            errorStatus = OsDrvCmxDmaWaitTask(
                &sippRxTasks[hwCtx->sippCommon.device]);

            if (errorStatus != MYR_DRV_SUCCESS) {
                mvLog(MVLOG_ERROR, "OsDrvCmxDmaWaitTask error %d", errorStatus);
            }

        OsDrvTimerGetElapsedTicks(&dmastamp, &dmaticks);

#if 1
        if (dmastartticks > 50000) {
            mvLog(MVLOG_FATAL, "%s dma start time: %llu", ctx->ctlName , dmastartticks);
        }

        if (dmaticks > 50000) {
            mvLog(MVLOG_FATAL, "%s dma time: %llu", ctx->ctlName, dmaticks);
        }

        if (irqticks > 50000) {
            mvLog(MVLOG_FATAL, "%s irq time: %llu", ctx->ctlName, irqticks);
        }
#endif
        }

        for (i = 0; i < linesToTransfer ; i++) {
            hwCtx->sippCommon.outputAddr->fill_control = 1 << 30;
        }
    }

    if (hwCtx->lastReadLineCB != firstLine) {
        mvLog(MVLOG_FATAL, "%p -> %p : %ld : "
              "%5d %5d %5d %5d %5d %5d",
              srcPtr, dstPtr, transferSize,
              ocbl, lo, obfl, firstLine, firstOutputLine, linesToTransfer);
    }

    hwCtx->lastWrittenLineOutput += linesToTransfer;

    hwCtx->lastReadLineCB += linesToTransfer;
    hwCtx->lastReadLineCB = hwCtx->lastReadLineCB % hwCtx->cbLines;

    return RTEMS_SUCCESSFUL;
}

void SippFrameHandler (void *context)
{
    struct FramePumpVideoContext *ctx = (struct FramePumpVideoContext *) context;
    struct FramePumpHwContextSipp *hwCtx =
        (struct FramePumpHwContextSipp *)ctx->hwCtx;
    uint32_t sippCamFrameStatus = 0;
    volatile uint32_t reg = 0;

    // First thing first ! Get timestamp
    hwCtx->lastTimestampNs = getTimestampNs();

    sippCamFrameStatus = GET_REG_WORD_VAL(SIPP_INT2_STATUS_ADR);

    // Check if the context is good
    if ((sippCamFrameStatus & hwCtx->idMask) == 0) {
        return;
    }

    hwCtx->baseAddr->INT2_CLEAR = hwCtx->idMask;
    reg = hwCtx->baseAddr->INT2_CLEAR; // TODO: finish transaction on bus (not sure if needed)
    (void)reg;

    DrvTimerStartTicksCount(&hwCtx->frameIrqTimestamp);

    mvLog(MVLOG_DEBUG, "IRQ at %llu", hwCtx->lastTimestampNs);

    rtems_event_send(hwCtx->taskId, EVENT_DMA0_DONE);
}

static rtems_status_code sippDirectEof (struct FramePumpVideoContext *ctx)
{
    struct FramePumpHwContextSipp *hwCtx =
        (struct FramePumpHwContextSipp *)ctx->hwCtx;
    uint32_t status = 0;
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    sc = sippEof(ctx);

    rtems_interrupt_disable(status);
    DrvSippEngineRestart((uint8_t*)hwCtx->nextWriteBuffer.fbMem.p1, (uint32_t)hwCtx->outputAddr, hwCtx->device);
    rtems_interrupt_enable(status);

    return sc;
}

static rtems_status_code sippCmxEof (struct FramePumpVideoContext *ctx)
{
    struct FramePumpHwContextSippCmx *hwCtx =
        (struct FramePumpHwContextSippCmx *)ctx->hwCtx;
    uint32_t status = 0;
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    hwCtx->lastReadLineCB = 0;
    hwCtx->lastWrittenLineOutput = 0;
    hwCtx->lastOCBL = 0;

    // Make sure the context is ready to receive the new frame
    hwCtx->sippCommon.outputAddr->fill_control = 1 << 31;
    hwCtx->sippCommon.outputAddr->context = 1<<31;

    sc = sippEof(ctx);

    rtems_interrupt_disable(status);
    DrvSippEngineRestart((uint8_t*)hwCtx->cmxBuffer,
                         (uint32_t)hwCtx->sippCommon.outputAddr,
                         hwCtx->sippCommon.device );
    rtems_interrupt_enable(status);

    return sc;
}

static rtems_status_code sippEof (struct FramePumpVideoContext *ctx)
{
    struct FramePumpHwContextSipp *hwCtx =
        (struct FramePumpHwContextSipp *)ctx->hwCtx;
    rtems_status_code sc = RTEMS_SUCCESSFUL;
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
    struct FramePumpBuffer fpb = getEmptyFrame (ctx);
    if (fpb.fbMem.p1 == NULL) {
        sc = -1;
    } else {
        rtems_interrupt_disable(level);
        hwCtx->nextWriteBuffer = fpb;
        rtems_event_receive(EVENT_DMA0_DONE,
                RTEMS_NO_WAIT | RTEMS_EVENT_ANY,
                0,
                &ev);
        rtems_interrupt_enable(level);
        sc = RTEMS_SUCCESSFUL;
    }

    mvLog(MVLOG_DEBUG, "EOF done at %llu", hwCtx->lastTimestampNs);
    return sc;
}

rtems_status_code sippStop (struct FramePumpVideoContext *ctx)
{
    rtems_event_set ev = 0;
    struct FramePumpHwContextSipp *hwCtx =
        (struct FramePumpHwContextSipp *)ctx->hwCtx;

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

static rtems_status_code sippDirectStopHandler (struct FramePumpVideoContext *ctx)
{
    sippCommonStopHandler(ctx);
    return RTEMS_SUCCESSFUL;
}

static rtems_status_code sippCmxStopHandler (struct FramePumpVideoContext *ctx)
{
    sippCommonStopHandler(ctx);

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code sippCommonStopHandler (struct FramePumpVideoContext *ctx)
{
    struct FramePumpHwContextSipp *hwCtx =
        (struct FramePumpHwContextSipp *)ctx->hwCtx;
    uint32_t status = 0;

    rtems_interrupt_disable(status);
    DrvSippDisableRxFilter( hwCtx->device );
    rtems_interrupt_enable(status);

    // Suicide
    rtems_event_send(hwCtx->taskId, EVENT_STOP);
    rtems_task_delete(rtems_task_self());

    return RTEMS_SUCCESSFUL;
}

const struct FramePumpHwFunc sippFunc =
{
    .init = sippDirectInit,
    .open = sippDirectOpen,
    .close = sippDirectClose,
    .start = sippDirectStart,
    .stop = sippStop,

    .eof = sippDirectEof,
    .eol = sippDirectEol,
    .stopHandler = sippDirectStopHandler,
    .startHandler = sippDirectStartHandler
};

const struct FramePumpHwFunc sippCmxFunc =
{
    .init = sippCmxInit,
    .open = sippCmxOpen,
    .start = sippCmxStart,
    .stop = sippStop,

    .eof = sippCmxEof,
    .eol = sippCmxEol,
    .stopHandler = sippCmxStopHandler,
    .startHandler = sippCmxStartHandler
};
