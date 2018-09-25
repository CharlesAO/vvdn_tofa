///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///

#include <string.h>

#include "FramePumpCommon.h"

#define MVLOG_UNIT_NAME FramePumpCommon
#include <mvLog.h>

/**
 * getEmptyFrame
 *
 * @brief   Receives an empty FramePumpBuffer from the 'inFifo' message queue.
 *
 * @param[in]   *ctx - pointer to FramePumpVideoContext structure
 *
 * @return  [FramePumpBuffer]
 */
struct FramePumpBuffer getEmptyFrame (struct FramePumpVideoContext *ctx)
{
    struct FramePumpBuffer nf = {0};
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    size_t size = 0;

    sc = rtems_message_queue_receive(ctx->inFifo,
                                     (void*)&nf,
                                     &size,
                                     RTEMS_DEFAULT_ATTRIBUTES,
                                     1 * rtems_clock_get_ticks_per_second() / 1000);

    if (sc || size != sizeof(struct FramePumpBuffer)) {
        memset(&nf, 0, sizeof(struct FramePumpBuffer));
        mvLog(MVLOG_WARN, "No empty frame available for %s!!!", ctx->ctlName);
    }
    return nf;
}

/**
 * sendFilledFrame
 *
 * @brief   Sends a filled FramePumpBuffer structure to the 'outFifo' message queue.
 *
 * @param[in]   *ctx - pointer to FramePumpVideoContext structure
 * @param[in]   fpb - FramePumpBuffer structure
 *
 * @return  [rtems_status_code]
 */
rtems_status_code sendFilledFrame (struct FramePumpVideoContext *ctx, struct FramePumpBuffer fpb)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    sc = rtems_message_queue_send(ctx->outFifo,
                                  (void*)&fpb,
                                  sizeof(struct FramePumpBuffer));

    return sc;
}

/**
 * inspectQueues
 *
 * @brief   Checks for pending messages in a devices 'inFifo' and 'outFifo' queues.
 *
 * @param[in]   *ctx - pointer to FramePumpVideoContext structure
 *
 * @return  nothing
 */
void inspectQueues (struct FramePumpVideoContext *ctx)
{
    uint32_t countin = 0;
    uint32_t countout = 0;
    mvLog_t lt = MVLOG_DEBUG;

    rtems_message_queue_get_number_pending(ctx->inFifo, &countin);
    rtems_message_queue_get_number_pending(ctx->outFifo, &countout);

    if (countin || countout) {
        lt = MVLOG_WARN;
    }
    mvLog(lt, "%s: %s:%d   -----     %lu %lu \n", __FILE__ , __func__, __LINE__, countin, countout);
}

/**
 * commonTask
 *
 * @brief   Calls 'startHandler' function for underlying hardware (SIPP or CIF).
 *          Waits indefinitly and repeatedly for 'DMA0_DONE, 'STOP', or 'EOL_BLOCK'
 *          events, calling corresponding hardware functions in each case.
 *
 * @param[in]   context - RTEMS task argument
 *
 * @return  [rtems_task]
 */
rtems_task commonTask (rtems_task_argument context)
{
    struct FramePumpVideoContext *ctx = (struct FramePumpVideoContext *) context;
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    rtems_event_set ev = 0;

    ctx->hwFnc->startHandler(ctx);

    while(1) {
        ev = 0;
        sc = rtems_event_receive(EVENT_DMA0_DONE    |
                                 EVENT_STOP         |
                                 EVENT_EOL_BLOCK,
                                 RTEMS_EVENT_ANY,
                                 1000 * rtems_clock_get_ticks_per_second() / 1000,
                                 &ev);
        if (sc != RTEMS_SUCCESSFUL) {
            mvLog(MVLOG_WARN, "Time out for camera events: %s", ctx->ctlName);
            continue;
        }

        if (ev & EVENT_STOP) {
            ctx->hwFnc->stopHandler(ctx);
        }

        if (ev & EVENT_EOL_BLOCK) {
            ctx->hwFnc->eol(ctx);
        }

        if (ev & EVENT_DMA0_DONE) {
            ctx->hwFnc->eof(ctx);
        }
    }
}

/**
 * bitsFromMipiPixelFormat
 *
 * @brief   Converts eDrvMipiDataType enum to uint32_t representing the number of
 *          bits in the MIPI pixel format.
 *
 * @param[in]   pixelFormat - MIPI pixel format enum
 *
 * @return  [uint32_t]
 *          bits
 */
#ifndef MA2X8X
uint32_t bitsFromMipiPixelFormat (const eDrvMipiDataType pixelFormat)
{
    uint32_t nbBits = 0;

    switch (pixelFormat) {
    case CSI_RAW_6:
        nbBits = 6;
        break;
    case CSI_RAW_7:
        nbBits = 7;
        break;
    case CSI_RAW_8:
        nbBits = 8;
        break;
    case CSI_RAW_10:
        nbBits = 10;
        break;
    case CSI_RAW_12:
        nbBits = 12;
        break;
    case CSI_RAW_14:
        nbBits = 14;
        break;
    case CSI_RGB_565:
    case CSI_YUV_422_B8:
        nbBits = 16;
        break;
    case CSI_RGB_888:
        nbBits = 24;
        break;
    default:
        nbBits = 0;
    }

    return nbBits;
}
#else
uint32_t bitsFromMipiPixelFormat (const OsDrvMipiCsi2DataType pixelFormat)
{
    uint32_t nbBits = 0;

    switch (pixelFormat) {
    case CSI_LP_DT_RAW6:
        nbBits = 6;
        break;
    case CSI_LP_DT_RAW7:
        nbBits = 7;
        break;
    case CSI_LP_DT_RAW8:
        nbBits = 8;
        break;
    case CSI_LP_DT_RAW10:
        nbBits = 10;
        break;
    case CSI_LP_DT_RAW12:
        nbBits = 12;
        break;
    case CSI_LP_DT_RAW14:
        nbBits = 14;
        break;
    case CSI_LP_DT_RGB565:
    case CSI_LP_DT_YUV422_8B:
        nbBits = 16;
        break;
    case CSI_LP_DT_RGB888:
        nbBits = 24;
        break;
    default:
        nbBits = 0;
    }

    return nbBits;
}
#endif

/**
 * bytesFromFrameType
 *
 * @brief   Converts frameTypes enum to uint32_t representing the number of
 *          bytes in the frame type.
 *
 * @param[in]   frameType - frame type enum
 *
 * @return  [uint32_t]
 *          bytes
 */
uint32_t bytesFromFrameType (const enum frameTypes frameType)
{
    uint32_t nbBytes = 0;

    switch (frameType) {
    case YUV420p:
    case YUV422p:
    case YUV400p:
    case LUT2:
    case LUT4:
    case RAW8:
    case NV12:
        nbBytes = 1;
        break;
    case YUV422i:
    case LUT16:
    case RAW16:
        nbBytes = 2;
        break;
    case RGB888:
    case YUV444i:
        nbBytes = 3;
        break;
    case RGBA8888:
        nbBytes = 4;
        break;
    default:
        nbBytes = 0;
    }

    return nbBytes;
}

/**
 * getTimestampNs
 *
 * @brief   Returns timestamp in nanoseconds
 *
 * @return  [uint64_t]
 *          timestamp in ns
 */
uint64_t getTimestampNs (void)
{
    uint64_t timestamp = 0;
    timestamp = rtems_clock_get_uptime_nanoseconds ();
    return timestamp;
}

void insertTearingMarkers (struct FramePumpBuffer *fpb)
{
    uint32_t *topmarker = NULL;
    uint32_t *btmmarker = NULL;
    uint32_t framebytesize = 0;

    framebytesize = fpb->fb.spec.height * fpb->fb.spec.stride;

    topmarker = (uint32_t*)fpb->fb.p1;
    btmmarker = (uint32_t*)(fpb->fb.p1 + framebytesize - sizeof(uint32_t));

    *topmarker = *btmmarker = fpb->frameCount;
}
