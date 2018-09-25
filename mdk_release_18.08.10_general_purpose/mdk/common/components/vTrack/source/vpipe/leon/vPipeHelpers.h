///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     helper functions for vPipe component
///

#ifndef _VPIPE_HELPERS_H_
#define _VPIPE_HELPERS_H_



#include "swcFrameTypes.h"

#ifndef VTRACK_CODE_SECTION
#define VTRACK_CODE_SECTION(x) x
#endif
#ifndef VTRACK_DATA_SECTION
#define VTRACK_DATA_SECTION(x) x
#endif

#include "vTrack.h"
#include "global_constants.h"
#include "featureMaintenanceApi.h"

// Makes ring buffer access more readable
#define CURR(ring_buffer) ring_buffer[(vpipe_frames + 0) % 2]
#define NEXT(ring_buffer) ring_buffer[(vpipe_frames + 1) % 2]
#define TEMP(ring_buffer) ring_buffer[2]

#define N_PYRS 2
#define CMX_PYR_LEVELS 2



#endif
