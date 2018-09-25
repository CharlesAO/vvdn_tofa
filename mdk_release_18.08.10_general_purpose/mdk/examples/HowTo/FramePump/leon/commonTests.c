///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///


// 1: Includes
// ----------------------------------------------------------------------------
#include "commonTests.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <swcCrc.h>
#include <swcLeonUtils.h>
#include <mvMacros.h>
#include "brdRtems.h"
#include <pthread.h>
#include <OsDrvTimer.h>
#include <sched.h>
#include "mvLog.h"

struct taskContext cams[] = {
    {
        .name         = "/dev/video_streaming",
        .fps          =  30,
    },
    {
        .name         = "/dev/video_tracking_left",
        .fps          =  30,
    },
    {
        .name         = "/dev/video_tracking_right",
        .fps          =  30,
    },
};

pthread_attr_t normalPrioThreadAttr;

void initStartTestThreads(void)
{
    unsigned i = 0;
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    // Initialize an empty thread attribute set with the default attributes
    if (pthread_attr_init(&normalPrioThreadAttr) !=0) {
        mvLog(MVLOG_WARN, "pthread_attr_init error");
    }
    // Change inheritsched attribute to PTHREAD_EXPLICIT_SCHED to indicate the the scheduling policy and parameters are to be set
    // from the corresponding values in the attribute object
    if (pthread_attr_setinheritsched(&normalPrioThreadAttr, PTHREAD_EXPLICIT_SCHED) != 0) {
        mvLog(MVLOG_WARN, "pthread_attr_setinheritsched error");
    }
    // Set the scheduling policy to round robin
    if (pthread_attr_setschedpolicy(&normalPrioThreadAttr, SCHED_RR) != 0) {
        mvLog(MVLOG_WARN, "pthread_attr_setschedpolicy error");
    }

    // 'cams' = cam task contexts
    for (i = 0; i < COUNT_OF(cams); i++) {
        cams[i].cnt = i;
        // Create cam threads with attributes specified in normalPrioThreadAttr
        // Thread starting point is 'testThreadFunc' with arg = (void*)&cams[i]
        sc = pthread_create( &cams[i].id, &normalPrioThreadAttr, testThreadFunc, (void*)&cams[i]);
        if (sc != RTEMS_SUCCESSFUL) {
            perror("Thread creation failed");
        }
    }

    for (i = 0; i < COUNT_OF(cams); i++) {
        void *value;
        // Wait here for thread to terminate
        // cams[i].id = (type)pthread_t
        sc = pthread_join( cams[i].id, &value);
        if (sc != RTEMS_SUCCESSFUL) {
            perror("Thread join failed");
        }
    }
}

int32_t checkTearingMarks(struct FramePumpBuffer *fpb, struct taskContext * tc)
{
    int32_t retCode = 0;
    uint32_t topMark = 0;
    uint32_t btmMark = 0;
    uint32_t framebytesize = 0;

    framebytesize = fpb->fb.spec.height * fpb->fb.spec.stride;

    topMark = *(uint32_t*)(fpb->fb.p1);
    btmMark = *(uint32_t*)(fpb->fb.p1 + framebytesize - sizeof(uint32_t));

    if (fpb->frameCount != topMark || fpb->frameCount != btmMark) {
        // Frame was torn (overwritten) a.k.a no frametearing mark
        retCode += -2;
        mvLog(MVLOG_WARN, " > Frame tearing detected !!  fc: %10lu  tm: %10lu bm: %10lu !!",
                fpb->frameCount, topMark, btmMark);
    }

    if (fpb->frameCount != tc->previousFrameMark + 1) {
        // Frame was skipped
        retCode += -1;
        mvLog(MVLOG_WARN, " > Frame lost here, got %10lu expected %10lu !!",
                fpb->frameCount, tc->previousFrameMark + 1);
    }

    tc->previousFrameMark = fpb->frameCount;

    return retCode;
}

void failTest(const char* failedFunc, int failedLine)
{
    mvLog(MVLOG_WARN, "Failed in %s, line %d", failedFunc, failedLine);

    exit(1);
}


void runApp(void)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    sc = brdInitializeBoard();
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_WARN, "Failure to initialize board driver %d!", sc);
        failTest( __func__, __LINE__ );
    }

    initStartTestThreads();

    mvLog(MVLOG_INFO, "Test finished");


    exit(0);
}
