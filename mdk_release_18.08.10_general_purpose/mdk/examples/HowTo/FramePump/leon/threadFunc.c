///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///


// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <swcCrc.h>
#include <swcLeonUtils.h>
#include <FramePump.h>
#include <mvMacros.h>
#include <pthread.h>
#include "mvLog.h"

#include "brdRtems.h"
#include "commonTests.h"


// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define NB_OF_FRAMES        (100)

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

//simplified buffer allocation with the size of the biggest frame (imx214, if present)
volatile uint64_t ts[10];

// 4: Static Local Data
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------
void *testThreadFunc(void *ctx)
{
    struct taskContext *tc = (struct taskContext *)ctx;
    struct FramePumpBuffer fpb = {0};
    int fd = 0;
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    int i = 0;
    frameSpec camFrameSpec = {0};

    mvLog(MVLOG_INFO, "Openning %s camera...", tc->name);
    fd = open(tc->name, O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);
    if (fd < 0) {
        mvLog(MVLOG_WARN, "Failed to open camera %s : %s", tc->name, strerror(errno));
        return NULL;
    }

    sc = ioctl(fd, FRAME_PUMP_GET_FRAME_SPEC, &camFrameSpec);
    if (sc != RTEMS_SUCCESSFUL) {
        perror("Failed to get camFrameSpec!!!");
        failTest( __func__, __LINE__);
    }

    mvLog(MVLOG_INFO, "Allocating FramePump buffers", tc->name);
    for (i = 0; i < IN_FRAMES_QUEUE_SIZE; i++) {
        // Get frame size, allocate it, add it to FramePump's queue
        void * frameData = NULL;
        size_t frameSize = camFrameSpec.width * camFrameSpec.height * camFrameSpec.bytesPP;

        memset(&fpb, 0, sizeof(struct FramePumpBuffer));

        sc = posix_memalign(&frameData, 64, frameSize);
        if (sc != RTEMS_SUCCESSFUL) {
            mvLog(MVLOG_WARN, "No more space");
            break;
        }
        fpb.fbMem.p1      = frameData;

        memset(fpb.fbMem.p1, 0, camFrameSpec.width * camFrameSpec.height * camFrameSpec.bytesPP);

        sc = write(fd, &fpb, sizeof(struct FramePumpBuffer));
        if (sc != sizeof(struct FramePumpBuffer)) {
            perror("Failed to write!!!");
            failTest( __func__, __LINE__);
        } else {
            mvLog(MVLOG_INFO, "Initial push to FPump: 0x%lx", (uint32_t)fpb.fbMem.p1);
        }
    }

    mvLog(MVLOG_INFO, "Starting FramePump", tc->name);
    if (-1 == ioctl(fd, FP_SHARED_CTL(FRAME_PUMP_START))) {
        perror("Failed to send start!!!");
        failTest( __func__, __LINE__);
    }

    tc->previousFrameMark = -1;
    uint32_t localCount = 0;

    mvLog(MVLOG_INFO, "Reading frames", tc->name);
    while (localCount < NB_OF_FRAMES) {
        // Get frame, uses 1 frame pump buffer from the queue
        sc = read(fd, &fpb, sizeof(fpb));
        if (sc != sizeof(struct FramePumpBuffer)) {
            mvLog(MVLOG_WARN, "Failed to read from camera %s : %s", tc->name, strerror(errno));
            failTest( __func__, __LINE__);
        }

        localCount++;

        if (-1 == checkTearingMarks(&fpb, tc)) {
            mvLog(MVLOG_WARN, "Mark tearing found");
        }

#if 1 // LOG FRAMES
        mvLog(MVLOG_INFO, "New frame (%d, 0x%x, delta %llums) from %s", (int)fpb.frameCount, (int)fpb.fb.p1, (fpb.timestampNs - ts[tc->cnt])/1000000, tc->name);
        ts[tc->cnt] = fpb.timestampNs;
#endif
        // Release the frame pump buffer back into queue
        sc = write(fd, &fpb, sizeof(fpb));
        if (sc != sizeof(struct FramePumpBuffer)) {
            perror("Failed to write!!!");
            failTest( __func__, __LINE__);
        }
    } // While

    sc = close(fd);
    if (sc < 0) {
        perror("Failed to close camera");
        failTest( __func__, __LINE__);
    }

    return NULL;
}
