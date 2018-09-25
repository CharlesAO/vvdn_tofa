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
#include <swcCrc.h>
#include <swcLeonUtils.h>
#include <mvMacros.h>
#include "swcFrameTypes.h"
#include "FramePump.h"

struct taskContext {
    const char *name;
    int         cnt;
    pthread_t   id;
    uint32_t         previousFrameMark;
    uint64_t         previousTimeStampNs;
    uint32_t         fps;
};

extern struct taskContext cams[3];

// Exported by common
void initStartTestThreads(void);
int32_t checkTearingMarks(struct FramePumpBuffer *fpb, struct taskContext * tc);
void failTest(const char*, int);
void runApp(void);

// Called from common
void *testThreadFunc(void *ctx);
