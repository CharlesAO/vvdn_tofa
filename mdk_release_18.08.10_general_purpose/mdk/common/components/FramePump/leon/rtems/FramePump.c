///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief FramePump main API implementation
///

#include <FramePump.h>
#include <FramePumpCommon.h>
#include <FramePumpPrivate.h>
#include <malloc.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>

#include <bsp.h>
#include <rtems.h>
#include <rtems/libio.h>

#include <DrvLeonL2C.h>
#include <swcLeonUtils.h>

#define MVLOG_UNIT_NAME FramePump
#include <mvLog.h>

#ifndef MAX_FRAME_PUMP_CAMS
#define MAX_FRAME_PUMP_CAMS (8)
#endif

#ifndef FP_QUEUED_FRAMES
#define FP_QUEUED_FRAMES    (100)
#endif

#define __CACHE_LINE_SIZE     (64)

static struct FramePumpVideoContext *fpContext[MAX_FRAME_PUMP_CAMS];
static int lastDevice = 0;

static rtems_status_code framePumpGetFrameSpec (
    frameSpec* pFrameSpec,
    struct FramePumpVideoContext *ctx);
static rtems_status_code framePumpGetMemoryFrameSpec (
    frameSpec* pFrameSpec,
    struct FramePumpVideoContext *ctx);
static rtems_status_code initializeOutSpec (
    struct FramePumpVideoContext *ctx);

/**
 * FramePumpInitialize
 *
 * @brief   FramePump componant initialization function. First call from RTEMS after
 *          successful device driver registration contains no argument and returns
 *          RTEMS_SUCCESSFUL. Subsequent calls to this function initialize FramePump
 *          devices and create message queues for in and out transactions. It also
 *          initializes the underlying hardware (SIPP or CIF) and assigns a device
 *          name to the provided major/minor number pair.
 *
 * @param[in]   major - RTEMS device major number
 * @param[in]   minor - RTEMS device minor number
 * @param[in]   *e - pointer to argument
 *
 * @return  [rtems_device_driver]
 */
rtems_device_driver FramePumpInitialize(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    (void)minor;

    if(e == NULL) {
        // Register driver
        // Function is called by RTEMS after successful driver registration with NULL arg
        return sc;
    } else {
        // Register device
        struct FramePumpVideoDeviceSensor * fpvd =
            (struct FramePumpVideoDeviceSensor*)e;
        struct FramePumpVideoContext * ctx = (struct FramePumpVideoContext *)
            calloc(1, sizeof(struct FramePumpVideoContext));
        uint32_t status = 0;

        if (fpvd == NULL) {
            mvLog(MVLOG_ERROR, "NULL initialize structure for FramePump");
            sc = RTEMS_INVALID_ADDRESS;
            goto cleanUp;
        }

        if (fpvd->vd.hwFnc == NULL) {
            mvLog(MVLOG_ERROR, "NULL hwFnc for FramePump device");
            sc = RTEMS_INVALID_ADDRESS;
            goto cleanUp;
        }

        ctx->hwFnc = fpvd->vd.hwFnc;

        if (fpvd->spec->ctlFnc == NULL) {
            mvLog(MVLOG_ERROR, "NULL ctlFnc for FramePump device");
            sc = RTEMS_INVALID_ADDRESS;
            goto cleanUp;
        }

        ctx->ctlFnc = fpvd->spec->ctlFnc;

        if (fpvd->vd.controlDevice == NULL) {
            mvLog(MVLOG_ERROR, "NULL controlDevice for FramePump device");

            sc = RTEMS_INVALID_ADDRESS;
            goto cleanUp;
        }

        if ((fpvd->vd.controlDevice && stat(fpvd->vd.controlDevice, NULL)) ||
           (fpvd->vd.sharedControlDevice && stat(fpvd->vd.sharedControlDevice, NULL))) {
            mvLog(MVLOG_WARN, "Control devices not found for FramePump %s device", fpvd->vd.controlDevice);
            sc = RTEMS_NOT_DEFINED;
            goto cleanUp;
        }

        ctx->ctlName = fpvd->vd.controlDevice;
        ctx->ctlNameShared = fpvd->vd.sharedControlDevice;

        if(ctx->hwFnc->init) {
            sc = ctx->hwFnc->init(ctx, &fpvd->vd);
            if (sc != RTEMS_SUCCESSFUL) {
                mvLog(MVLOG_ERROR, "Could not initialize hardware for FramePump : %d", sc);
                goto cleanUp;
            }
        } else {
            mvLog(MVLOG_INFO, "Initialize hardware function NULL");
        }

        // Create message queue with FP_QUEUED_FRAMES entries of size (FramePumpBuffer)
        // Default attributes - waiting tasks are serviced in FIFO order
        // rtems ID of message queue is returned in ctx->inFifo
        sc = rtems_message_queue_create(rtems_build_name( 'F', 'P', 'I', 'N' ),
                                        FP_QUEUED_FRAMES,
                                        sizeof(struct FramePumpBuffer),
                                        RTEMS_DEFAULT_ATTRIBUTES,
                                        &ctx->inFifo);
        if (sc != RTEMS_SUCCESSFUL) {
            mvLog(MVLOG_ERROR, "Could not create message queue : %d", sc);
            goto cleanUp;
        }

        sc = rtems_message_queue_create(rtems_build_name( 'F', 'P', 'O', 'T' ),
                                        FP_QUEUED_FRAMES,
                                        sizeof(struct FramePumpBuffer),
                                        RTEMS_DEFAULT_ATTRIBUTES,
                                        &ctx->outFifo);
        if (sc != RTEMS_SUCCESSFUL) {
            mvLog(MVLOG_ERROR, "Could not create message queue : %d", sc);
            goto cleanUp;
        }

        // register with the minor number from virtual channel
        // 'lastDevice' is minor number
        sc = rtems_io_register_name(fpvd->vd.name, major, lastDevice);
        if (sc != RTEMS_SUCCESSFUL)
        {
            mvLog(MVLOG_ERROR, "Could not register name %s : %d", fpvd->vd.name , sc);
            goto cleanUp;
        }

        sc = initializeOutSpec(ctx);
        if (sc != RTEMS_SUCCESSFUL) {
            mvLog(MVLOG_ERROR, "Could not initialize OutSpec %s : %d", fpvd->vd.name , sc);
            goto cleanUp;
        }

        rtems_interrupt_disable(status);
        if (lastDevice < MAX_FRAME_PUMP_CAMS) {
            fpContext[lastDevice++] = ctx;
        } else {
            sc = RTEMS_TOO_MANY;
        }
        rtems_interrupt_enable(status);

        return RTEMS_SUCCESSFUL;

    cleanUp:
        free(ctx);
        return sc;
    }
}

/**
 * FramePumpOpen
 *
 * @brief   FramePump componant open function. Flushes in/out message queues and calls
 *          'open' functions for both underlying hardware (SIPP or CIF) and the assigned
 *          sensor control device. Sends 'STOP' and 'SEND_CONFIG' commands to the sensor
 *          control device through the 'config' function.
 *
 * @param[in]   major - RTEMS device major number
 * @param[in]   minor - RTEMS device minor number
 * @param[in]   *e - pointer to argument
 *
 * @return  [rtems_device_driver]
 */
rtems_device_driver FramePumpOpen(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    uint32_t count = 0;
    rtems_status_code sc = RTEMS_NOT_DEFINED;
    struct FramePumpControlConfig cfg = {0};

    struct FramePumpVideoContext * ctx = fpContext[(int)minor];
    if (ctx == NULL) {
        return sc;
    }

    (void)major;
    (void)e;

    // Make sure the queues are flushed at start-up
    sc = rtems_message_queue_flush(ctx->inFifo, &count);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_ERROR, "Could not flush inFifo");
        return sc;
    }

    sc = rtems_message_queue_flush(ctx->outFifo, &count);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_ERROR, "Could not flush outFifo");
        return sc;
    }

    sc = ctx->ctlFnc->open(ctx);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_ERROR, "Failed to open control device");
        return sc;
    }

    sc = ctx->hwFnc->open(ctx);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_ERROR, "Failed to start hardware %d", sc);
        return sc;
    }

    cfg.cfgType = FRAME_PUMP_CONTROL_STOP;
    sc = ctx->ctlFnc->config(ctx, &cfg);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_ERROR, "Failed to send control stop command");
        return sc;
    }

    cfg.cfgType = FRAME_PUMP_CONFIG_SEND_CONFIG;
    sc = ctx->ctlFnc->config(ctx, &cfg);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_ERROR, "Failed to send control configuration");
        return sc;
    }

    return RTEMS_SUCCESSFUL;
}

/**
 * FramePumpClose
 *
 * @brief   FramePump componant close function. Sends 'STOP' command to sensor control
 *          device through the 'config' function. Calls 'stop' function for the underlying
 *          hardware (SIPP or CIF) and calls 'close' function for the sensor control
 *          device.
 *
 * @param[in]   major - RTEMS device major number
 * @param[in]   minor - RTEMS device minor number
 * @param[in]   *e - pointer to argument
 *
 * @return  [rtems_device_driver]
 */
rtems_device_driver FramePumpClose(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    rtems_status_code sc = RTEMS_NOT_DEFINED;
    struct FramePumpControlConfig cfg = {0};
    struct FramePumpVideoContext * ctx = fpContext[(int)minor];
    if (ctx == NULL) {
         return sc;
    }

    (void)major;
    (void)e;

    sc = ctx->hwFnc->close(ctx);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_ERROR, "Failed to send stop hardware");
        return sc;
    }

    cfg.cfgType = FRAME_PUMP_CONTROL_STOP;
    sc = ctx->ctlFnc->config(ctx, &cfg);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_ERROR, "Failed to send stop command");
        return sc;
    }

    sc = ctx->ctlFnc->close(ctx);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_ERROR, "Failed to close control device %d", sc);
        return sc;
    }

    return RTEMS_SUCCESSFUL;
}

/**
 * frameCacheInvalidate
 *
 * @brief   Invalidates cache data range corrsponding to contents of FramePumpBuffer structure.
 *
 * @param[in]   *fpb - pointer to FramePumpBuffer structure
 *
 * @return  nothing
 */
static void frameCacheInvalidate(struct FramePumpBuffer *fpb)
{
    const int linesize = fpb->fb.spec.width * fpb->fb.spec.bytesPP;
    uint32_t i = 0;

    if (fpb->fb.spec.width * fpb->fb.spec.bytesPP == fpb->fb.spec.stride) {
        const void *from = ALIGN_DOWN(fpb->fb.p1, __CACHE_LINE_SIZE);
        const void *to   = ALIGN_UP(fpb->fb.p1 +
                                    fpb->fb.spec.height * linesize,
                                    __CACHE_LINE_SIZE);
        rtems_cache_invalidate_data_range(from, to - from);
    } else {
        for (i = 0; i < fpb->fb.spec.height ; i++) {
            const void *from = ALIGN_DOWN(fpb->fb.p1 + i * linesize, __CACHE_LINE_SIZE);
            const void *to   = ALIGN_UP(fpb->fb.p1 + (i+1) * linesize, __CACHE_LINE_SIZE);

            rtems_cache_invalidate_data_range(from, to - from);
        }
    }
}

static rtems_status_code frameValidateAndCheck(
    struct FramePumpVideoContext *ctx,
    struct FramePumpBuffer *fpb)
{
    (void)ctx;

    // Check we have some memory at least
    if (fpb->fb.p1 == NULL && fpb->fbMem.p1 == NULL) {
        return RTEMS_INVALID_ADDRESS;
    }

    // Memory descripting buffer takes precedence
    if (fpb->fbMem.p1) {
        fpb->fb.p1 = fpb->fbMem.p1;
    }
    // If not, fallback
    else {
        fpb->fbMem.p1 = fpb->fb.p1;
    }

    fpb->fb.spec    = ctx->outSpec.spec;
    fpb->fbMem.spec = ctx->outSpec.spec;

    if (ctx->outSpec.hasMetadata) {
        if (ctx->spec->metaROI.x0 == 0) {
            fpb->fb.p1 += ctx->spec->metaROI.lines * fpb->fb.spec.stride;
        } else if (ctx->spec->metaROI.x0 == ctx->spec->height) {
            fpb->fb.spec.height -= ctx->spec->metaROI.lines;
        }
    }

    return RTEMS_SUCCESSFUL;
}

/**
 * FramePumpRead
 *
 * @brief   FramePump componant read function. Sends message to 'out' message queue requesting
 *          a FramePumpBuffer structure. Corresponding data range is invalidated in the cache
 *          and the resulting buffer is assigned to the provided argument structure.
 *
 * @param[in]   major - RTEMS device major number
 * @param[in]   minor - RTEMS device minor number
 * @param[in]   *e - pointer to argument
 *
 * @return  [rtems_device_driver]
 */
rtems_device_driver FramePumpRead(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    int32_t sc = RTEMS_SUCCESSFUL;

    rtems_libio_rw_args_t *rw_args = (rtems_libio_rw_args_t*) e;
    if (rw_args == NULL) {
        return RTEMS_INVALID_ADDRESS;
    }

    struct FramePumpVideoContext * ctx = fpContext[(int)minor];
    if (ctx == NULL) {
        return RTEMS_NOT_DEFINED;
    }

    (void)major;

    if (rw_args->count != sizeof(struct FramePumpBuffer)) {
        return RTEMS_INVALID_NUMBER;
    }

    struct FramePumpBuffer fpb;
    size_t size;

    sc = rtems_message_queue_receive(ctx->outFifo,
                                     (void*)&fpb,
                                     &size,
                                     RTEMS_DEFAULT_ATTRIBUTES,
                                     200 * rtems_clock_get_ticks_per_second() / 1000);
    if (sc != RTEMS_SUCCESSFUL) {
        rw_args->bytes_moved = 0;
        return RTEMS_TIMEOUT;
    }

    if (size != sizeof(struct FramePumpBuffer)) {
        return RTEMS_IO_ERROR;
    }

    frameCacheInvalidate(&fpb);

    *(struct FramePumpBuffer*)rw_args->buffer = fpb;
    rw_args->bytes_moved = rw_args->count;

    mvLog(MVLOG_DEBUG, "Received 0x%p from %s", fpb.fb.p1, ctx->ctlName);

    return RTEMS_SUCCESSFUL;
}

/**
 * FramePumpWrite
 *
 * @brief   FramePump componant write function. Corresponding data range is invalidated in the
 *          cache. Sends message to 'in' message queue containing the FramePumpBuffer structure
 *          to be sent.
 *
 * @param[in]   major - RTEMS device major number
 * @param[in]   minor - RTEMS device minor number
 * @param[in]   *e - pointer to argument
 *
 * @return  [rtems_device_driver]
 */
rtems_device_driver FramePumpWrite(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    int32_t sc = RTEMS_SUCCESSFUL;

    rtems_libio_rw_args_t *rw_args = (rtems_libio_rw_args_t*) e;
    if (rw_args == NULL) {
        return RTEMS_INVALID_ADDRESS;
    }

    struct FramePumpVideoContext * ctx = fpContext[(int)minor];
    if (ctx == NULL) {
        return RTEMS_NOT_DEFINED;
    }

    (void)major;

    if (rw_args->count != sizeof(struct FramePumpBuffer)) {
        return RTEMS_INVALID_NUMBER;
    }

    struct FramePumpBuffer * fpb = (struct FramePumpBuffer*)rw_args->buffer;

    frameCacheInvalidate(fpb);
    frameValidateAndCheck(ctx, fpb);

    sc = rtems_message_queue_send(ctx->inFifo,
                                  (void*)fpb,
                                  sizeof(struct FramePumpBuffer));
    if (sc != RTEMS_SUCCESSFUL) {
        rw_args->bytes_moved = 0;
        return RTEMS_IO_ERROR;
    }

    rw_args->bytes_moved = rw_args->count;

    mvLog(MVLOG_DEBUG, "Prepared 0x%p for %s", fpb->fb.p1, ctx->ctlName);

    return RTEMS_SUCCESSFUL;
}

/**
 * FramePumpControl
 *
 * @brief   FramePump componant control function. Multiple services are offered by this funtion.
 *          A check is first performed to see if this call is related to a sensor control device.
 *          If not, the FramePump services offered correspond to the following parameters:
 *
 *              FRAME_PUMP_START    -   calls 'start' function for underlying hardware (SIPP or CIF)
 *                                  -   sends 'FRAME_PUMP_START' condition to sensor control device
 *                                      through the 'config' function
 *              FRAME_PUMP_STOP     -   sends 'FRAME_PUMP_STOP' condition to sensor control device
 *                                      through the 'config' function
 *              FRAME_PUMP_GET_MEMORY_FRAME_SPEC    -   calls framePumpGetMemoryFrameSpec function
 *              FRAME_PUMP_GET_FRAME_SPEC   -   calls framePumpGetFrameSpec function
 *
 * @param[in]   major - RTEMS device major number
 * @param[in]   minor - RTEMS device minor number
 * @param[in]   *e - pointer to argument
 *
 * @return  [rtems_device_driver]
 */
rtems_device_driver FramePumpControl(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    struct FramePumpControlConfig cfg = {0};

    rtems_libio_ioctl_args_t * ctl = (rtems_libio_ioctl_args_t *) e;
    if (ctl == NULL) {
        return RTEMS_INVALID_ADDRESS;
    }

    struct FramePumpVideoContext * ctx = fpContext[(int)minor];
    if (ctx == NULL) {
        return RTEMS_NOT_DEFINED;
    }

    (void)major;

    mvLog(MVLOG_DEBUG, "Control %X with ctx 0x%p sent to %s",
          (unsigned int)ctl->command, ctl->buffer, ctx->ctlName);

    // Try to check if it's a sensor driver control command
    cfg.cfgType = ctl->command;
    cfg.buffer = ctl->buffer;
    sc = ctx->ctlFnc->config(ctx, &cfg);
    if (sc != RTEMS_INVALID_NUMBER) {
        ctl->ioctl_return = 0;
        return sc;
    }

    switch (FP_STRIP_CTL(ctl->command)) {
    case FRAME_PUMP_START:
        mvLog(MVLOG_DEBUG, "START %s: %s:%d", __FILE__ , __func__, __LINE__);
        sc = ctx->hwFnc->start(ctx);
        if (sc != RTEMS_SUCCESSFUL) {
            mvLog(MVLOG_ERROR, "Failed to open control device");
            return sc;
        }

        if (FP_IS_SHARED_CTL(ctl->command)) {
            cfg.cfgType = FP_SHARED_CTL(FRAME_PUMP_CONTROL_START);
        } else {
            cfg.cfgType = FRAME_PUMP_CONTROL_START;
        }
        sc = ctx->ctlFnc->config(ctx, &cfg);
        break;
    case FRAME_PUMP_STOP:
        mvLog(MVLOG_INFO, "STOP %s: %s:%d", __FILE__ , __func__, __LINE__);
        if (FP_IS_SHARED_CTL(ctl->command)) {
            cfg.cfgType = FP_SHARED_CTL(FRAME_PUMP_CONTROL_STOP);
        } else {
            cfg.cfgType = FRAME_PUMP_CONTROL_STOP;
        }
        sc = ctx->ctlFnc->config(ctx, &cfg);
        break;
    case FRAME_PUMP_GET_MEMORY_FRAME_SPEC:
        sc = framePumpGetMemoryFrameSpec(ctl->buffer, ctx);
        break;
    case FRAME_PUMP_GET_FRAME_SPEC:
        sc = framePumpGetFrameSpec(ctl->buffer, ctx);
        break;
    default:
        sc = RTEMS_INVALID_NUMBER;
        break;
    }

    ctl->ioctl_return = (int)sc;
    return sc;
}

/**
 * pfFromMipiPf
 *
 * @brief   Derives pixel format from MIPI data type
 *
 * @param[in]   pixelFormat - MIPI data type
 *
 * @return  [frameType]
 *          RAW8,
 *          RAW16,
 *          -1
 */

#ifndef MA2X8X
frameType pfFromMipiPf(eDrvMipiDataType pixelFormat)
{
    switch (pixelFormat)
    {
    case CSI_RAW_6:
    case CSI_RAW_7:
    case CSI_RAW_8:
        return RAW8;
    case CSI_RAW_10:
    case CSI_RAW_12:
    case CSI_RAW_14:
        return RAW16;
    case CSI_RGB_565:
    case CSI_RGB_888:
    case CSI_YUV_422_B8:
    default:
        return RTEMS_INVALID_NUMBER ;
    }
}
#else
frameType pfFromMipiPf(OsDrvMipiCsi2DataType pixelFormat)
{
    switch (pixelFormat)
    {
    case CSI_LP_DT_RAW6:
    case CSI_LP_DT_RAW7:
    case CSI_LP_DT_RAW8:
        return RAW8;
    case CSI_LP_DT_RAW10:
    case CSI_LP_DT_RAW12:
    case CSI_LP_DT_RAW14:
        return RAW16;
    case CSI_LP_DT_RGB565:
    case CSI_LP_DT_RGB888:
    case CSI_LP_DT_YUV422_8B:
    default:
        return RTEMS_INVALID_NUMBER ;
    }
}
#endif
/**
 * initializeOutSpec
 *
 * @brief   Assigns outSpec structure fields from FramePumpVideoContext specification
 *
 * @param[in/out]   *ctx - pointer to FramePumpVideoContext struture
 *
 * @return  [int]
 *          0 if successful,
 *          -2 otherwise
 */
static rtems_status_code initializeOutSpec (struct FramePumpVideoContext *ctx)
{
    ctx->outSpec.spec.width   = ctx->spec->width;
    ctx->outSpec.spec.height  = ctx->spec->height;

    switch (ctx->spec->type)
    {
    case FP_MIPI:
        ctx->outSpec.spec.type =
            pfFromMipiPf(ctx->spec->spec.mipi.pixelFormat);
        break;
    case FP_PARALLEL:
        ctx->outSpec.spec.type = ctx->spec->spec.parallel.pixelFormat;
        break;
    case FP_OTHER:
        break;
    default:
        return RTEMS_INVALID_NUMBER ;
    }

    ctx->outSpec.spec.bytesPP = bytesFromFrameType(ctx->outSpec.spec.type);
    ctx->outSpec.spec.stride   = ctx->spec->width * ctx->outSpec.spec.bytesPP;

    return RTEMS_SUCCESSFUL;
}

/**
 * framePumpGetFrameSpec
 *
 * @brief   Assigns frame specification to supplied structure from specified context.
 *
 * @param[out]  *pFrameSpec  - pointer to frameSpec structure
 * @param[in]   *ctx         - pointer to FramePumpVideoContext structure
 *
 * @return  [int]
 *          RTEMS_INVALID_ADRESS if frameSpec pointer is NULL
 *          0 otherwise
 */
static rtems_status_code framePumpGetFrameSpec (
    frameSpec* pFrameSpec,
    struct FramePumpVideoContext *ctx)
{
    if (!pFrameSpec) {
        return RTEMS_INVALID_ADDRESS;
    }

    pFrameSpec->width   = ctx->outSpec.spec.width;
    pFrameSpec->height  = ctx->outSpec.spec.height;
    pFrameSpec->bytesPP = ctx->outSpec.spec.bytesPP;
    pFrameSpec->stride  = ctx->outSpec.spec.stride;
    pFrameSpec->type    = ctx->outSpec.spec.type;

    return RTEMS_SUCCESSFUL;
}

/**
 * framePumpGetMemoryFrameSpec
 *
 * @brief   Assigns frame specification to supplied structure from specified context.
 *          Adds metadata information if present.
 *
 * @param[out]  *pFrameSpec  - pointer to frameSpec structure
 * @param[in]   *ctx         - pointer to FramePumpVideoContext structure
 *
 * @return  [int]
 *          RTEMS_INVALID_ADRESS if frameSpec pointer is NULL
 *          0 otherwise
 */
static rtems_status_code framePumpGetMemoryFrameSpec (
    frameSpec* pFrameSpec,
    struct FramePumpVideoContext *ctx)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    sc = framePumpGetFrameSpec(pFrameSpec, ctx);
    if (sc != RTEMS_SUCCESSFUL) {
        return RTEMS_INVALID_ADDRESS;
    }

    if (ctx->outSpec.hasMetadata) {
        pFrameSpec->width   += ctx->spec->metaROI.columns;
        pFrameSpec->height  += ctx->spec->metaROI.lines;
    }

    return RTEMS_SUCCESSFUL;
}

/**
 * RTEMS Device Driver function table for FramePump componant
 */
rtems_driver_address_table FramePumpDrvTblName = {
    .initialization_entry = FramePumpInitialize,
    .open_entry           = FramePumpOpen,
    .close_entry          = FramePumpClose,
    .read_entry           = FramePumpRead,
    .write_entry          = FramePumpWrite,
    .control_entry        = FramePumpControl
};
