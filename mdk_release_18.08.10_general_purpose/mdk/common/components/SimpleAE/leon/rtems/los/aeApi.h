///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

#ifndef AE_API_H__
#define AE_API_H__

#include <FramePump.h>
#include "aeCommStructs.h"

#ifdef __cplusplus
extern "C" {
#endif

    int         simpleAEinit();
    int         simpleAEpostInit();

// params:
//     fd - file descriptor for one of the cameras
//     id - id of th camera in stereo pair (0 or 1)
//     name - name of camera device
    int         simpleAEstart(const int fd, const int id, const char * name);

    int         simpleAEprocessFrame(const int id, const FramePumpBuffer *frameIn);

    aeMessage_t simpleAEget();

#ifdef __cplusplus
}
#endif

#endif
