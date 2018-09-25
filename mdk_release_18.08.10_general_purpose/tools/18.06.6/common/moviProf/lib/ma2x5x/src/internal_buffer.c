#include <DrvSvuDefines.h>

#include "ptypes.h"

#define FPROFILER_BUFFER_SIZE ( (TOTAL_NUM_SHAVES+2) * (sizeof(ProfileBufferMasterRecord_t) + (CONFIG_PROFILE_MAX_CHUNKS * FPROFILER_CHUNK_SIZE)))
/* #define FPROFILER_BUFFER_SIZE (18 * (sizeof(ProfileBufferMasterRecord_t) + (10 * FPROFILER_CHUNK_SIZE))) */

const int __buffer_size__ = FPROFILER_BUFFER_SIZE;

char __profile_internal_buffer__[FPROFILER_BUFFER_SIZE] FPROFILER_DATA_SECTION;

int _profile_last_error = 0;
int _profile_error_count = 0;

/* set this to 1 from the debugger to continue from p_assert() */
volatile int _profile_unhalt = 0;

#if defined(MA2X8X) && defined(__RTEMS__)
#include <OsDrvMutex.h>
OsDrvMutexHandler _p_hndl = {0};
#endif
