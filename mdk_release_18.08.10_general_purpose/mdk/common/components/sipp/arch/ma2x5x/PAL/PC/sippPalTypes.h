// -----------------------------------------------------------------------------
// Copyright (C) 2015 Movidius Ltd. All rights reserved
//
// Filename         : sippPalTypes.h
// Company          : Movidius
// Description      : Header file to define types used in the
//                    SIPP Platform Abstraction Layer
//
// -----------------------------------------------------------------------------

#ifndef _SIPP_PAL_TYPES_H_
#define _SIPP_PAL_TYPES_H_

#include <sippBaseTypes.h>

typedef u32  SIPP_PAL_TIMER_ID;
typedef u32  SIPP_PAL_CRIT_STATE;

/* Flag indicating a fatal error - top bit of error_num */
#define SIPP_ERROR_FATAL              0x80000000
#define SIPP_ERROR_WARNING            0x00000000

/* Flags for trace levels */
#define SIPP_TRACE_MASK_MODULE        0x0FFFFFFF
#define SIPP_TRACE_MASK_LEVEL         0x70000000
#define SIPP_TRACE_MASK_TIMESTAMP     0x80000000

#define SIPP_TRACE_SHIFT_LEVEL        28
#define SIPP_TRACE_SHIFT_TIMESTAMP    31

#define SIPP_TRACE_LEVEL_NEVER        0x00000000
#define SIPP_TRACE_LEVEL_1            0x10000000
#define SIPP_TRACE_LEVEL_2            0x20000000
#define SIPP_TRACE_LEVEL_3            0x30000000
#define SIPP_TRACE_LEVEL_4            0x40000000
#define SIPP_TRACE_LEVEL_5            0x50000000
#define SIPP_TRACE_LEVEL_6            0x60000000
#define SIPP_TRACE_LEVEL_ALWAYS       0x70000000

/* Disable the timestamp */
#define SIPP_TRACE_NO_TIMESTAMP       0x80000000

/* Module identifiers */
#define SIPP_TRACE_API                0x00000001
#define SIPP_TRACE_FRAMEWORK_MANAGER  0x00000002
#define SIPP_TRACE_ACCESS_SCHEDULER   0x00000003
#define SIPP_TRACE_PAL                0x00000004
#define SIPP_TRACE_CORE               0x00000005
#define SIPP_TRACE_SESS_CONTROL       0x00000006
#define SIPP_TRACE_RUNTIME            0x00000007
#define SIPP_TRACE_HEAP               0x00000008

/* The upper supported value of a trace module identifier. Make sure that
   there are no trace IDs defined above this value since it is used to
   determine the amount of storage required for trace flags! */
#define SIPP_TRACE_MAX                0x0000001F

/* ID indicating that message should be SIPP_FW_MGRed regardless of the
   modules which are currently enabled. */
#define SIPP_TRACE_ANY                0x00000000

/* The number of 32bit words needed to store our trace flags */
#define SIPP_TRACE_FLAGS_WORDS ((SIPP_TRACE_MAX+31)/32)

/* Data structure used to store trace flags */
typedef struct
{

  u32 Flags[SIPP_TRACE_FLAGS_WORDS];

} SIPP_TRACE_FLAGS;


/////////////////////////////////////////////////////////////////////////////////
// Trace level definitions

#define SIPP_API_TL_CATASTROPHE  (SIPP_TRACE_LEVEL_ALWAYS | SIPP_TRACE_ANY)
#define SIPP_API_TL_SEVERE       (SIPP_TRACE_LEVEL_ALWAYS | SIPP_TRACE_API)
#define SIPP_API_TL_ERROR        (SIPP_TRACE_LEVEL_6      | SIPP_TRACE_API)
#define SIPP_API_TL_WARNING      (SIPP_TRACE_LEVEL_5      | SIPP_TRACE_API)
#define SIPP_API_TL_FUNC         (SIPP_TRACE_LEVEL_4      | SIPP_TRACE_API)
#define SIPP_API_TL_INFO         (SIPP_TRACE_LEVEL_3      | SIPP_TRACE_API)
#define SIPP_API_TL_VERBOSE      (SIPP_TRACE_LEVEL_2      | SIPP_TRACE_API)
#define SIPP_API_TL_DEBUG        (SIPP_TRACE_LEVEL_1      | SIPP_TRACE_API)

#define SIPP_FW_MGR_TL_CATASTROPHE  (SIPP_TRACE_LEVEL_ALWAYS | SIPP_TRACE_ANY)
#define SIPP_FW_MGR_TL_SEVERE       (SIPP_TRACE_LEVEL_ALWAYS | SIPP_TRACE_FRAMEWORK_MANAGER)
#define SIPP_FW_MGR_TL_ERROR        (SIPP_TRACE_LEVEL_6      | SIPP_TRACE_FRAMEWORK_MANAGER)
#define SIPP_FW_MGR_TL_WARNING      (SIPP_TRACE_LEVEL_5      | SIPP_TRACE_FRAMEWORK_MANAGER)
#define SIPP_FW_MGR_TL_FUNC         (SIPP_TRACE_LEVEL_4      | SIPP_TRACE_FRAMEWORK_MANAGER)
#define SIPP_FW_MGR_TL_INFO         (SIPP_TRACE_LEVEL_3      | SIPP_TRACE_FRAMEWORK_MANAGER)
#define SIPP_FW_MGR_TL_VERBOSE      (SIPP_TRACE_LEVEL_2      | SIPP_TRACE_FRAMEWORK_MANAGER)
#define SIPP_FW_MGR_TL_DEBUG        (SIPP_TRACE_LEVEL_1      | SIPP_TRACE_FRAMEWORK_MANAGER)

#define SIPP_ACC_SCH_TL_CATASTROPHE  (SIPP_TRACE_LEVEL_ALWAYS | SIPP_TRACE_ANY)
#define SIPP_ACC_SCH_TL_SEVERE       (SIPP_TRACE_LEVEL_ALWAYS | SIPP_TRACE_ACCESS_SCHEDULER)
#define SIPP_ACC_SCH_TL_ERROR        (SIPP_TRACE_LEVEL_6      | SIPP_TRACE_ACCESS_SCHEDULER)
#define SIPP_ACC_SCH_TL_WARNING      (SIPP_TRACE_LEVEL_5      | SIPP_TRACE_ACCESS_SCHEDULER)
#define SIPP_ACC_SCH_TL_FUNC         (SIPP_TRACE_LEVEL_4      | SIPP_TRACE_ACCESS_SCHEDULER)
#define SIPP_ACC_SCH_TL_INFO         (SIPP_TRACE_LEVEL_3      | SIPP_TRACE_ACCESS_SCHEDULER)
#define SIPP_ACC_SCH_TL_VERBOSE      (SIPP_TRACE_LEVEL_2      | SIPP_TRACE_ACCESS_SCHEDULER)
#define SIPP_ACC_SCH_TL_DEBUG        (SIPP_TRACE_LEVEL_1      | SIPP_TRACE_ACCESS_SCHEDULER)

#define SIPP_PAL_TL_CATASTROPHE   (SIPP_TRACE_LEVEL_ALWAYS | SIPP_TRACE_ANY)
#define SIPP_PAL_TL_SEVERE        (SIPP_TRACE_LEVEL_ALWAYS | SIPP_TRACE_PAL)
#define SIPP_PAL_TL_ERROR         (SIPP_TRACE_LEVEL_6      | SIPP_TRACE_PAL)
#define SIPP_PAL_TL_WARNING       (SIPP_TRACE_LEVEL_5      | SIPP_TRACE_PAL)
#define SIPP_PAL_TL_FUNC          (SIPP_TRACE_LEVEL_4      | SIPP_TRACE_PAL)
#define SIPP_PAL_TL_INFO          (SIPP_TRACE_LEVEL_3      | SIPP_TRACE_PAL)
#define SIPP_PAL_TL_VERBOSE       (SIPP_TRACE_LEVEL_2      | SIPP_TRACE_PAL)
#define SIPP_PAL_TL_DEBUG         (SIPP_TRACE_LEVEL_1      | SIPP_TRACE_PAL)

#define SIPP_CORE_TL_CATASTROPHE      (SIPP_TRACE_LEVEL_ALWAYS | SIPP_TRACE_ANY)
#define SIPP_CORE_TL_SEVERE           (SIPP_TRACE_LEVEL_ALWAYS | SIPP_TRACE_CORE)
#define SIPP_CORE_TL_ERROR            (SIPP_TRACE_LEVEL_6      | SIPP_TRACE_CORE)
#define SIPP_CORE_TL_WARNING          (SIPP_TRACE_LEVEL_5      | SIPP_TRACE_CORE)
#define SIPP_CORE_TL_FUNC             (SIPP_TRACE_LEVEL_4      | SIPP_TRACE_CORE)
#define SIPP_CORE_TL_INFO             (SIPP_TRACE_LEVEL_3      | SIPP_TRACE_CORE)
#define SIPP_CORE_TL_VERBOSE          (SIPP_TRACE_LEVEL_2      | SIPP_TRACE_CORE)
#define SIPP_CORE_TL_DEBUG            (SIPP_TRACE_LEVEL_1      | SIPP_TRACE_CORE)

#define SIPP_SES_CTRL_TL_CATASTROPHE      (SIPP_TRACE_LEVEL_ALWAYS | SIPP_TRACE_ANY)
#define SIPP_SES_CTRL_TL_SEVERE           (SIPP_TRACE_LEVEL_ALWAYS | SIPP_TRACE_SESS_CONTROL)
#define SIPP_SES_CTRL_TL_ERROR            (SIPP_TRACE_LEVEL_6      | SIPP_TRACE_SESS_CONTROL)
#define SIPP_SES_CTRL_TL_WARNING          (SIPP_TRACE_LEVEL_5      | SIPP_TRACE_SESS_CONTROL)
#define SIPP_SES_CTRL_TL_FUNC             (SIPP_TRACE_LEVEL_4      | SIPP_TRACE_SESS_CONTROL)
#define SIPP_SES_CTRL_TL_INFO             (SIPP_TRACE_LEVEL_3      | SIPP_TRACE_SESS_CONTROL)
#define SIPP_SES_CTRL_TL_VERBOSE          (SIPP_TRACE_LEVEL_2      | SIPP_TRACE_SESS_CONTROL)
#define SIPP_SES_CTRL_TL_DEBUG            (SIPP_TRACE_LEVEL_1      | SIPP_TRACE_SESS_CONTROL)

#define SIPP_RUNTIME_TL_CATASTROPHE      (SIPP_TRACE_LEVEL_ALWAYS | SIPP_TRACE_ANY)
#define SIPP_RUNTIME_TL_SEVERE           (SIPP_TRACE_LEVEL_ALWAYS | SIPP_TRACE_RUNTIME)
#define SIPP_RUNTIME_TL_ERROR            (SIPP_TRACE_LEVEL_6      | SIPP_TRACE_RUNTIME)
#define SIPP_RUNTIME_TL_WARNING          (SIPP_TRACE_LEVEL_5      | SIPP_TRACE_RUNTIME)
#define SIPP_RUNTIME_TL_FUNC             (SIPP_TRACE_LEVEL_4      | SIPP_TRACE_RUNTIME)
#define SIPP_RUNTIME_TL_INFO             (SIPP_TRACE_LEVEL_3      | SIPP_TRACE_RUNTIME)
#define SIPP_RUNTIME_TL_VERBOSE          (SIPP_TRACE_LEVEL_2      | SIPP_TRACE_RUNTIME)
#define SIPP_RUNTIME_TL_DEBUG            (SIPP_TRACE_LEVEL_1      | SIPP_TRACE_RUNTIME)

#define SIPP_HEAP_TL_CATASTROPHE      (SIPP_TRACE_LEVEL_ALWAYS | SIPP_TRACE_ANY)
#define SIPP_HEAP_TL_SEVERE           (SIPP_TRACE_LEVEL_ALWAYS | SIPP_TRACE_RUNTIME)
#define SIPP_HEAP_TL_ERROR            (SIPP_TRACE_LEVEL_6      | SIPP_TRACE_RUNTIME)
#define SIPP_HEAP_TL_WARNING          (SIPP_TRACE_LEVEL_5      | SIPP_TRACE_RUNTIME)
#define SIPP_HEAP_TL_FUNC             (SIPP_TRACE_LEVEL_4      | SIPP_TRACE_RUNTIME)
#define SIPP_HEAP_TL_INFO             (SIPP_TRACE_LEVEL_3      | SIPP_TRACE_RUNTIME)
#define SIPP_HEAP_TL_VERBOSE          (SIPP_TRACE_LEVEL_2      | SIPP_TRACE_RUNTIME)
#define SIPP_HEAP_TL_DEBUG            (SIPP_TRACE_LEVEL_1      | SIPP_TRACE_RUNTIME)

#endif /* _SIPP_PAL_TYPES_H_ */

