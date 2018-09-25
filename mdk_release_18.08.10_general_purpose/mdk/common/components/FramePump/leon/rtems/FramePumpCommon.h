///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///

#ifndef FRAMEPUMPCOMMON_H__
#define FRAMEPUMPCOMMON_H__

#include "FramePumpDefines.h"
#include "FramePumpPrivate.h"
#include "FramePump.h"
#include <swcFrameTypes.h>

// RTEMS event flags
#define EVENT_EOL_BLOCK    RTEMS_EVENT_22
#define EVENT_DMA0_DONE    RTEMS_EVENT_23
#define EVENT_STOP         RTEMS_EVENT_24

struct FramePumpBuffer getEmptyFrame (struct FramePumpVideoContext *ctx);
rtems_status_code sendFilledFrame (struct FramePumpVideoContext *ctx, struct FramePumpBuffer fpb);
void inspectQueues(struct FramePumpVideoContext *ctx);

rtems_task commonTask(rtems_task_argument context);

#ifndef MA2X8X
uint32_t bitsFromMipiPixelFormat(eDrvMipiDataType pixelFormat);
#else
uint32_t bitsFromMipiPixelFormat(OsDrvMipiCsi2DataType pixelFormat);
#endif

uint32_t bytesFromFrameType(enum frameTypes frameType);

uint64_t getTimestampNs(void);
void insertTearingMarkers(struct FramePumpBuffer *fpb);

#endif // FRAMEPUMPCOMMON_H__
