///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     MEST API private types.
///
/// MEST private structure types.
///

#ifndef MEST_PRIVATE_TYPES_H
#define MEST_PRIVATE_TYPES_H

#include <MestPublicTypes.h>

typedef struct mestRoiQueueElList
{
    mestROI_t roi;
    mestROIId_t id;
    mestFeatureSelPolicy_t policy;
    uint32_t rasterStep;
    uint32_t *points;
    uint32_t pointsCnt;
    uint32_t *results;
    uint32_t startX, startY, endX, endY;
    uint32_t roiCfg;

    struct mestRoiQueueElList *next;
} mestRoiQueueElList_t;

typedef struct
{
    mestRoiQueueElList_t *head, *tail;
    int32_t size;
} mestRoiQueueList_t;

typedef struct {
    mestRoiQueueList_t roiQueue;
    mestRoiQueueElList_t *roiCrtPtr;
    uint64_t totalROIs;
    uint32_t *coordsBuf, *resBuf, MAX_COORDINATES, MAX_RESULTS;
    uint32_t coordsBufCnt, totalCoordinatesCnt;
    uint32_t minStep;
    mestOutput_t *outputStr;
    float avgCost, avgMVx, avgMVy;
    uint32_t avgCnt;
    uint32_t staticConfigBits;
    uint32_t running;
    void *(*motestDoneHandler)(void*);
    void *motestDoneHandlerArgs;
    mestParamConfig_t staticConfig;
    mestRuntimeConfig_t runtimeConfig;
} mestHandlerPrivate_t;

#endif // MEST_PRIVATE_TYPES_H

