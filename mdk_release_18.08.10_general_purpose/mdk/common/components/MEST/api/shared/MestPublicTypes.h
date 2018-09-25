///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     MEST API public types.
///
/// MEST public structure types.
///

#ifndef MEST_PUBLIC_TYPES_H
#define MEST_PUBLIC_TYPES_H

#include <swcFrameTypes.h>

#include <MestDefines.h>
#include <MestOutput.h>

#include <stdint.h>

typedef enum mestError
{
    MEST_SUCCESS = 0,
    MEST_HEAP_ERROR,
    MEST_RUNTIME_ERROR,
    MEST_MUTEX_ERROR,
    MEST_BUFFER_NOT_ALIGNED,
    MEST_BUFFER_TOO_SMALL,
    MEST_ERROR,
    MEST_NULL_BUFFER,
    MEST_NOT_STARTED,
    MEST_ALREADY_STARTED,

    MEST_ROI_SUCCESS,
    MEST_ROI_INVALID,
    MEST_ROI_ERROR,
    MEST_ROI_NOT_FOUND,
    MEST_ROI_QUEUE_FULL,
    MEST_ROI_QUEUE_EMPTY,
    MEST_ROI_INVALID_POINTS,
    MEST_ROI_INVALID_CORNERS
} mestError_t;

typedef enum mestMode
{
    MEST_MODE_NORMAL,
} mestMode_t;

typedef enum mestFeatureSelPolicy
{
    MEST_DENSE_POLICY_STEP_1,
    MEST_DENSE_POLICY_STEP_2,
    MEST_DENSE_POLICY_STEP_4,
    MEST_DENSE_POLICY_STEP_8,
    MEST_DENSE_POLICY_STEP_16,
    MEST_DENSE_POLICY_STEP_32,
    MEST_USER_DEFINED,
} mestFeatureSelPolicy_t;

typedef enum mestFilterPolicy
{
//    MEST_MAXIMIZE_INLIER_RATIO = 0x1,
    MEST_MAXIMIZE_INLIER_COUNT = 0x2,
    /* TODO: construct histogram with MAXIMIZED_INLIER_RATIO
       from a more restraint set, then select list with vectors
       in the winner bin from the whole RAW set)*/
//    MEST_BINNED_INLIERS = 0x4,
    MEST_RAW_FLOW_VECTORS = 0x8,
} mestFilterPolicy_t;

typedef enum mestOutputPolicy
{
    MEST_ALL_FLOWS = 0x1,
    MEST_ALL_ROI_FLOWS = 0x2,
    MEST_AVERAGE_FLOW = 0x4,
    MEST_AVERAGE_FLOW_PER_ROI = 0x8,
} mestOutputPolicy_t;

typedef struct
{
    uint64_t timestampNs;
    uint64_t frameId;
    uint32_t outputFlowCnt;
} mestOutputHeader_t;

typedef struct
{
    uint32_t hwMestTime;
    uint32_t postProcTime;
} mestDebug_t;

typedef struct
{
    mestOutputHeader_t header;
    mestMeta_t meta;
    mestDebug_t debug;
} mestOutput_t;

typedef struct
{
// static parameters
    frameSpec spec;
} mestParamConfig_t;

typedef struct
{
    // memory config, cache partitions, shave numbers, any other resources
    uint32_t *coordsHeapStartAddress;
    uint32_t coordsHeapSize;
    uint32_t *resultsTmpHeapStartAddress;
    uint32_t resultsTmpHeapSize;
    uint32_t useMalloc; // for now just use malloc
} mestResourceConfig_t;

typedef enum mestPrecision
{
    MEST_PRECISION_INTEGER, MEST_PRECISION_HALF, MEST_PRECISION_QUARTER
} mestPrecision_t;

typedef struct
{
    // runtime configuration: output policy, filter policy
    mestOutputPolicy_t outputPolicy;
    mestFilterPolicy_t filteringPolicy;
    mestPrecision_t precision;
    float minCostThreshold, maxCostThreshold;
} mestRuntimeConfig_t;

typedef struct mestROI
{
    xyPos_t topLeft;
    xyPos_t bottomRight;
} mestROI_t;

typedef int32_t mestROIId_t;

#endif // MEST_PUBLIC_TYPES_H

