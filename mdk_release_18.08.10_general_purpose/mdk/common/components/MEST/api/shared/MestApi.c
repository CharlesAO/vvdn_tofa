///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     MEST Component Functions API.
///
/// This is the implementation of MEST API.
///
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>

#include <MestUse.h>

#include <mvMacros.h>
#include <mvLog.h>

#ifdef __PC__
    #include "MotEst_wrapper.h"
#endif

#ifdef __PC__
    #define MEST_BUFFER_ALIGNMENT 1
#else
    #define MEST_BUFFER_ALIGNMENT 128
#endif

// Faster conversion, implies necessity of arithmetic shift
#define GET_COST(val) ((uint32_t)((val) & 0xFFFF0000) >> 16)
#define GET_X(val) (((int)(val) & 0xFF00) << 16 >> 24)
#define GET_Y(val) (((int)(val) & 0x00FF) << 24 >> 24)

//#define GET_COST(val) (*((uint16_t*)(&val) + 1))
//#define GET_X(val) (*((int8_t*)(&val) + 1))
//#define GET_Y(val) (*((int8_t*)(&val) + 0))

#ifndef UNUSED
    #define UNUSED(X) ((void)X)
#endif

const uint16_t MEST_WRONG_INLIER_COST = 0xFFFF;

static void *motestDoneHandler(void *args);

// MEST ROI

static inline int checkInBounds(const xyPos_t pt, int xLeft, int yTop, int xRight, int yBottom)
{
    return (pt.x >= xLeft && pt.x <= xRight && pt.y >= yTop && pt.y <= yBottom);
}

static inline int checkInBoundsRect(
        const xyPos_t pt,
        const xyPos_t topLeft,
        const xyPos_t bottomRight)
{
    return (pt.x <= bottomRight.x && pt.x >= topLeft.x &&
            pt.y <= bottomRight.y && pt.y >= topLeft.y);
}

static inline int compareSpecs(const frameSpec *a, const frameSpec *b)
{
    return (a->width == b->width && a->height == b->height && a->stride == b->stride
        && a->bytesPP == b->bytesPP && a->type == b->type);
}

static mestError_t motestConfigAndStartRoutine(mestHandlerPrivate_t *this)
{
    if(this->roiCrtPtr->pointsCnt == 0)
    {
        mvLog(MVLOG_WARN, "Empty ROI found. Skipping ROI.");
        return MEST_RUNTIME_ERROR;
    }

    int roiCfg = this->staticConfigBits;

    if(this->roiCrtPtr->points == NULL)
        roiCfg |= (1 << 28);
    else
        roiCfg &= ~(1 << 28);
    roiCfg &= ~0x3FFFF;
    roiCfg |= (this->roiCrtPtr->pointsCnt & 0x3FFFF);

    this->roiCrtPtr->roiCfg = roiCfg;
    this->roiCrtPtr->results = this->resBuf;

    mestConfigureAndStart(this);

    return MEST_SUCCESS;
}

static inline float filterCost(mestHandler_t *this, float cost, mestFilterPolicy_t policy)
{
    mestHandlerPrivate_t *_hnd = this->privateData;
    if(policy == MEST_MAXIMIZE_INLIER_COUNT)
    {
        if(cost < _hnd->runtimeConfig.minCostThreshold ||
           cost > _hnd->runtimeConfig.maxCostThreshold)
        {
            return MEST_WRONG_INLIER_COST;
        }
    }
    return cost;
}

static inline int copyMvCost(
        mestHandler_t *this,
        const uint32_t *res,
        uint16_t **costs,
        xyPos_t **mvs,
        const int allFlowsMode)
{
    mestHandlerPrivate_t *_hnd = this->privateData;
    int cost;
    float x, y;

    cost = GET_COST(*res);
    cost = filterCost(this, cost, _hnd->runtimeConfig.filteringPolicy);
    x = GET_X(*res) / 4.0f;
    y = GET_Y(*res) / 4.0f;

    // All MVs are written out
    if(allFlowsMode == 1)
    {
        **costs = cost;
        (*mvs)->x = x;
        (*mvs)->y = y;
        (*costs)++;
        (*mvs)++;
    }
    else // Only the average MV will be written out
    {
        if(cost == MEST_WRONG_INLIER_COST)
            return 1;
        _hnd->avgCost += cost;
        _hnd->avgMVx += x;
        _hnd->avgMVy += y;
    }

    return 0;
}

static void transferResults(mestHandler_t *this, mestRoiQueueElList_t *roiCrtPtr)
{
    mestHandlerPrivate_t *_hnd = this->privateData;
    int offsetX, offsetY;
    uint32_t i, resCnt, X, Y;

    resCnt = 0;
    uint16_t *costs = &_hnd->outputStr->meta.costs[_hnd->outputStr->header.outputFlowCnt];
    xyPos_t *mvs = &_hnd->outputStr->meta.outputFlows[_hnd->outputStr->header.outputFlowCnt];

    const int allFlowsMode = (_hnd->runtimeConfig.outputPolicy == MEST_ALL_FLOWS ? 1 : 0);

    // Directed mode, candidate list exists
    if(roiCrtPtr->points != NULL)
    {
        const uint32_t *res = roiCrtPtr->results;
        for(i = 0; i < roiCrtPtr->pointsCnt; i++, res++)
        {
            // Check if output is filtered based on cost threshold
            if(copyMvCost(this, res, &costs, &mvs, allFlowsMode))
                continue;
            resCnt++;
        }
        if(_hnd->runtimeConfig.outputPolicy == MEST_ALL_FLOWS)
            _hnd->outputStr->header.outputFlowCnt += resCnt;
    }
    else // Raster mode
    {
        const int step = roiCrtPtr->rasterStep;
        const int stride = (roiCrtPtr->endX - roiCrtPtr->startX) / step;

        offsetX = (uint32_t) (roiCrtPtr->roi.topLeft.x + step - 1)
                / step - roiCrtPtr->startX / MEST_BUFFER_ALIGNMENT;
        offsetY = (uint32_t) (roiCrtPtr->roi.topLeft.y + step - 1)
                / step - roiCrtPtr->startY / MEST_BUFFER_ALIGNMENT;

        mvLog(MVLOG_DEBUG, "Offsets: %d %d", offsetX, offsetY);
        mvLog(MVLOG_DEBUG, "Start points: %" PRIu32 " %" PRIu32 "",
                roiCrtPtr->startX, roiCrtPtr->startY);
        mvLog(MVLOG_DEBUG, "End points: %" PRIu32 " %" PRIu32 "",
                roiCrtPtr->endX, roiCrtPtr->endY);
        mvLog(MVLOG_DEBUG, "Offsets: %d %d", offsetX, offsetY);


        Y = ALIGN_UP((uint32_t) roiCrtPtr->roi.topLeft.y, step);
        mvLog(MVLOG_DEBUG, "Start offset: %" PRIu32 "", Y);
        for(; Y < roiCrtPtr->roi.bottomRight.y; Y += step)
        {
            const uint32_t *res = roiCrtPtr->results + offsetY * stride + offsetX;
            X = ALIGN_UP((uint32_t) roiCrtPtr->roi.topLeft.x, step);
            for(; X < roiCrtPtr->roi.bottomRight.x; X += step, res++)
            {
                // Check if output is filtered based on cost threshold
                if(copyMvCost(this, res, &costs, &mvs, allFlowsMode))
                    continue;
                resCnt++;
            }
            offsetY++;
        }
        mvLog(MVLOG_DEBUG, "Needed/computed points: %" PRIu32 " %" PRIu32 "",
                roiCrtPtr->pointsCnt, resCnt);
        if(_hnd->runtimeConfig.outputPolicy == MEST_ALL_FLOWS)
            _hnd->outputStr->header.outputFlowCnt += resCnt;
    }
    // The if-else is only for average flow policy
    if(_hnd->runtimeConfig.outputPolicy == MEST_AVERAGE_FLOW)
    {
        _hnd->outputStr->meta.costs[0] += _hnd->avgCost;
        _hnd->outputStr->meta.outputFlows[0].x += _hnd->avgMVx;
        _hnd->outputStr->meta.outputFlows[0].y += _hnd->avgMVy;
        _hnd->avgCnt += resCnt;
    }
    else if(_hnd->runtimeConfig.outputPolicy == MEST_AVERAGE_FLOW_PER_ROI)
    {
        _hnd->outputStr->meta.costs[_hnd->outputStr->header.outputFlowCnt] =
                _hnd->avgCost / resCnt;
        _hnd->outputStr->meta.outputFlows[_hnd->outputStr->header.outputFlowCnt].x =
                _hnd->avgMVx / resCnt;
        _hnd->outputStr->meta.outputFlows[_hnd->outputStr->header.outputFlowCnt].y =
                _hnd->avgMVy / resCnt;
        _hnd->outputStr->header.outputFlowCnt++;
    }
}

static void *motestDoneHandler(void *args)
{
    mestHandler_t *this = (mestHandler_t*)args;
    mestHandlerPrivate_t *_hnd = this->privateData;
    if(_hnd->running == 0)
        return (void*)0x2; // No run to wait for
    UNUSED(args);
    transferResults(this, _hnd->roiCrtPtr); // decode results from HW format to fp32 format
    _hnd->roiCrtPtr = _hnd->roiCrtPtr->next;

    // If there are ROIs left to process
    if(_hnd->roiCrtPtr != NULL)
    {
        if(motestConfigAndStartRoutine(_hnd) != MEST_SUCCESS)
        {
            mvLog(MVLOG_FATAL, "Could not configure run for ROI!");
            _hnd->running = 0;
            return (void*) 0x3; // Error occurred
        }
        _hnd->running = 1;
        return NULL;
    }

    // All ROIs done
    if(_hnd->runtimeConfig.outputPolicy == MEST_AVERAGE_FLOW)
    {
        _hnd->outputStr->meta.costs[0] = _hnd->avgCost / _hnd->avgCnt;
        _hnd->outputStr->meta.outputFlows[0].x = _hnd->avgMVx / _hnd->avgCnt;
        _hnd->outputStr->meta.outputFlows[0].y = _hnd->avgMVy / _hnd->avgCnt;
        _hnd->outputStr->header.outputFlowCnt = 1;
    }
    _hnd->running = 0;
    return (void*)0x1; // Signaling the last ROI
}

static uint32_t* try_alocate_coords(mestHandlerPrivate_t *this, int reqSize)
{
    mvLog(MVLOG_DEBUG, "Buffer: %p %" PRIu32 " %d %" PRIu32 "",
            this->coordsBuf,
            this->coordsBufCnt,
            reqSize,
            this->MAX_COORDINATES);
    if(ALIGN_UP(this->coordsBufCnt, MEST_BUFFER_ALIGNMENT) + reqSize <= this->MAX_COORDINATES)
    {
        this->coordsBufCnt = ALIGN_UP(this->coordsBufCnt, MEST_BUFFER_ALIGNMENT);
        return this->coordsBuf + this->coordsBufCnt;
    }
    int lastPos = 0;
    mestRoiQueueElList_t *it;
    for(it = this->roiQueue.head; it; it = it->next)
    {
        if(it->pointsCnt && it->points != NULL && it->points != this->coordsBuf + lastPos)
        {
            // shift coordinates to left
            uint32_t *ptr = this->coordsBuf + lastPos;
            memcpy(ptr, it->points, it->pointsCnt * sizeof(uint32_t));
            it->points = this->coordsBuf + lastPos;
        }
        lastPos += ALIGN_UP(it->pointsCnt, MEST_BUFFER_ALIGNMENT);
    }
    this->coordsBufCnt = lastPos;
    if(this->coordsBufCnt + reqSize <= this->MAX_COORDINATES)
        return this->coordsBuf + this->coordsBufCnt;

    mvLog(MVLOG_WARN, "Could not allocate buffer for coordinates");
    return NULL;
}

static uint32_t* mestRoiGenerateList(
    mestHandlerPrivate_t *this,
    const mestROI_t roi,
    mestFeatureSelPolicy_t policy,
    const xyPos_t *pts,
    uint32_t *ptsCnt,
    uint32_t *ptsStep
    )
{
    uint32_t cnt, step = 1;
    uint32_t *list = NULL;

    switch (policy)
    {
    case MEST_DENSE_POLICY_STEP_1:
        step = 1;
        break;
    case MEST_DENSE_POLICY_STEP_2:
        step = 2;
        break;
    case MEST_DENSE_POLICY_STEP_4:
        step = 4;
        break;
    case MEST_DENSE_POLICY_STEP_8:
        step = 8;
        break;
    case MEST_DENSE_POLICY_STEP_16:
        step = 16;
        break;
    case MEST_DENSE_POLICY_STEP_32:
        step = 32;
        break;
    case MEST_USER_DEFINED:
        break;
    default:
        *ptsCnt = 0;
        return NULL;
    }
    this->minStep = MIN(this->minStep, step);

    if(policy != MEST_USER_DEFINED)
    {
        *ptsStep = step;
        uint32_t startX, startY, endX, endY;
        int width, height;
        startX = ALIGN_UP((uint32_t) roi.topLeft.x, step);
        startY = ALIGN_UP((uint32_t) roi.topLeft.y, step);
        endX = ALIGN_DOWN((uint32_t) roi.bottomRight.x, step);
        endY = ALIGN_DOWN((uint32_t) roi.bottomRight.y, step);
        width =  (endX - startX) / step + 1;
        height = (endY - startY) / step + 1;
        if(width <= 0 || height <= 0) // not a valid ROI
        {
            *ptsCnt = 0;
        }
        else
        {
            *ptsCnt = width * height;
            this->totalCoordinatesCnt += *ptsCnt;
        }
        return NULL;
    }

    list = (this->coordsBuf + this->coordsBufCnt);
    cnt = 0;
    if(pts != NULL)
    {
        if((list = try_alocate_coords(this, *ptsCnt)) == NULL) // try to allocate space for coordinates
        {
            *ptsCnt = 0;
            return NULL;
        }
        mvLog(MVLOG_DEBUG, "List allocated: %p", list);
        cnt = *ptsCnt;
        for (uint32_t i = 0; i < cnt; i++, pts++)
        {
            if(!checkInBoundsRect(*pts, roi.topLeft, roi.bottomRight))
            {
                mvLog(MVLOG_ERROR, "Point (x,y) (%f, %f) is not in [%f,%f] - [%f, %f] range!",
                        pts->x, pts->y,
                        roi.topLeft.x, roi.topLeft.y,
                        roi.bottomRight.x, roi.bottomRight.y);
            }
            list[i] = (((uint32_t)pts->x) << 16 | ((uint32_t)pts->y));
        }
    }
    *ptsCnt = cnt;
    this->coordsBufCnt += cnt;
    this->totalCoordinatesCnt += cnt;

    return list;
}

mestError_t mestAddROI(
    mestHandler_t *hnd,
    const mestROI_t *roi,
    const xyPos_t *locations,
    const uint32_t locationsCnt,
    const mestFeatureSelPolicy_t policy,
    mestROIId_t id
    )
{
    mestHandlerPrivate_t *_hnd = hnd->privateData;
    if(id == -1)
        id = _hnd->roiQueue.size;
    if(id != _hnd->roiQueue.size)
        return MEST_ROI_ERROR;
    if (roi->bottomRight.x < roi->topLeft.x || roi->bottomRight.y < roi->topLeft.y)
        return MEST_ROI_INVALID_POINTS;
    if(!checkInBounds(roi->topLeft, 0, 0,
            _hnd->staticConfig.spec.width - 1,
            _hnd->staticConfig.spec.height - 1))
    {
        mvLog(MVLOG_ERROR, "ROI top-left corner is out of bounds");
        return MEST_ROI_INVALID_POINTS;
    }
    if(!checkInBounds(roi->bottomRight, 0, 0,
            _hnd->staticConfig.spec.width - 1,
            _hnd->staticConfig.spec.height - 1))
    {
        mvLog(MVLOG_ERROR, "ROI bottom-right corner is out of bounds");
        return MEST_ROI_INVALID_POINTS;
    }
    if(policy < MEST_DENSE_POLICY_STEP_1 || policy > MEST_USER_DEFINED)
    {
        mvLog(MVLOG_ERROR, "ROI invalid policy");
        return MEST_ROI_INVALID;
    }

    mestRoiQueueElList_t *q = malloc(sizeof(mestRoiQueueElList_t));
    q->policy = policy;
    q->roi = *roi;
    q->id = id;
    q->points = NULL;
    q->pointsCnt = locationsCnt;
    q->results = NULL;

    q->next = NULL;

    if (policy == MEST_USER_DEFINED)
    {
        q->startX = 0;
        q->startY = 0;
        q->endX = _hnd->staticConfig.spec.width - 1;
        q->endY = _hnd->staticConfig.spec.height - 1;
        q->points = mestRoiGenerateList(_hnd, q->roi, policy, locations, &q->pointsCnt, NULL);
        mvLog(MVLOG_DEBUG, "List generated: %p %" PRIu32 "", q->points, q->pointsCnt);
    }
    else
    {
        q->points = mestRoiGenerateList(_hnd, q->roi, policy, NULL, &q->pointsCnt, &q->rasterStep);
        int X, Y;
        const int step = q->rasterStep;
        X = ALIGN_UP((int) q->roi.topLeft.x, step);
        Y = ALIGN_UP((int) q->roi.topLeft.y, step);
        q->startX = ALIGN_DOWN(MAX(X - step - 16, 0), MEST_BUFFER_ALIGNMENT);
        q->startY = ALIGN_DOWN(MAX(Y - step - 16, 0), MEST_BUFFER_ALIGNMENT);
        X = ALIGN_DOWN(((int) q->roi.bottomRight.x), step);
        Y = ALIGN_DOWN(((int) q->roi.bottomRight.y), step);
        mvLog(MVLOG_DEBUG, "X and Y: %d %d", X, Y);
        q->endX = MIN((uint32_t) X + step + 16, _hnd->staticConfig.spec.width - 1);
        q->endY = MIN((uint32_t) Y + step + 16, _hnd->staticConfig.spec.height - 1);
        q->pointsCnt = ((q->endX - q->startX) / step + 1) * ((q->endY - q->startY) / step + 1);
    }
    if (q->pointsCnt == 0)
    {
        mvLog(MVLOG_ERROR, "Could not generate points for ROI");
        return MEST_ROI_INVALID_POINTS;
    }

    if(!_hnd->roiQueue.head)
    {
        _hnd->roiQueue.head = _hnd->roiQueue.tail = q;
    }
    else
    {
        _hnd->roiQueue.tail->next = q;
        _hnd->roiQueue.tail = q;
    }
    _hnd->roiQueue.size++;
    _hnd->totalROIs++;

    return MEST_ROI_SUCCESS;
}

mestError_t mestRemoveROI(mestHandler_t *hnd, const mestROIId_t id)
{
    mestHandlerPrivate_t *_hnd = hnd->privateData;
    mestRoiQueueElList_t *it, *aux;

    if(_hnd->roiQueue.head->id == id)
    {
        it = _hnd->roiQueue.head->next;
        free(_hnd->roiQueue.head);
        _hnd->roiQueue.head = it;
        _hnd->roiQueue.size--;
        return MEST_SUCCESS;
    }
    else
    {
        it = _hnd->roiQueue.head;
        while (it->next && it->next->id != id)
            it = it->next;

        if (it->next && it->next->id == id)
        {
            aux = it->next->next;
            _hnd->totalCoordinatesCnt -= it->next->pointsCnt;
            free(it->next);
            it->next = aux;
            _hnd->roiQueue.size--;
            return MEST_SUCCESS;
        }
    }
    return MEST_ROI_NOT_FOUND;
}

mestError_t mestRemoveAllROI(mestHandler_t *hnd)
{
    mestHandlerPrivate_t *_hnd = hnd->privateData;
    mestRoiQueueElList_t *it;
    while(_hnd->roiQueue.head)
    {
        it = _hnd->roiQueue.head->next;
        free(_hnd->roiQueue.head);
        _hnd->roiQueue.head = it;
    }
    _hnd->roiQueue.head = _hnd->roiQueue.tail = NULL;
    _hnd->roiQueue.size = 0;
    return MEST_ROI_SUCCESS;
}

mestError_t mestUpdateROI(
        mestHandler_t *hnd,
        const mestROIId_t id,
        const mestROI_t * newRoi)
{
    mestHandlerPrivate_t *_hnd = hnd->privateData;
    mestRoiQueueElList_t *it;

    if(!_hnd->roiQueue.tail)
    {
        mvLog(MVLOG_WARN, "Cannot update ROI, ROI queue is empty");
        return MEST_ROI_QUEUE_EMPTY;
    }
    it = _hnd->roiQueue.head;
    while (it && it->id != id)
        it = it->next;

    if (it && it->id == id)
    {
        it->roi = *newRoi;
        return MEST_SUCCESS;
    }
    return MEST_ROI_NOT_FOUND;
}

mestError_t mestUpdateROIFeatures(
        mestHandler_t *hnd,
        const mestROIId_t id,
        xyPos_t* locations,
        uint32_t locationsCnt)
{
    mestHandlerPrivate_t *_hnd = hnd->privateData;
    mestRoiQueueElList_t *it;
    if(!_hnd->roiQueue.tail)
    {
        mvLog(MVLOG_WARN, "Cannot update ROI feature list. "
                    "ROI queue is empty");
        return MEST_ROI_QUEUE_EMPTY;
    }
    it = _hnd->roiQueue.head;
    while (it && it->id != id)
        it = it->next;

    if (it && it->id == id)
    {
        uint32_t step, *buf = NULL;
        // Remove old points from list count
        _hnd->totalCoordinatesCnt -= it->pointsCnt;
        // Mark previous list as empty and unused
        it->pointsCnt = 0;
        buf = mestRoiGenerateList(_hnd, it->roi, it->policy, locations, &locationsCnt, &step);
        if (locationsCnt == 0)
        {
            mvLog(MVLOG_ERROR, "Cannot update ROI feature list. "
                        "Invalid points given to ROI");
            return MEST_ROI_INVALID_POINTS;
        }
        it->points = buf;
        it->pointsCnt = locationsCnt;
        it->rasterStep = step;
        return MEST_ROI_SUCCESS;
    }
    mvLog(MVLOG_WARN, "ROI was not found for feature list update");
    return MEST_ROI_NOT_FOUND;
}

mestROIId_t mestROIgetNextId(mestHandler_t *hnd)
{
    mestHandlerPrivate_t *_hnd = hnd->privateData;
    return _hnd->roiQueue.size;
}

// MEST API

mestError_t mestInitStaticConfigVGA(mestParamConfig_t* config)
{
    config->spec.width = 640;
    config->spec.height = 480;
    config->spec.stride = 640;
    config->spec.bytesPP = 1;
    config->spec.type = RAW8;

    return MEST_SUCCESS;
}

static mestError_t mestInitDefaultRuntimeConfig(mestRuntimeConfig_t* config)
{
    config->minCostThreshold = 0;
    config->maxCostThreshold = 64 * 256;
    config->precision = MEST_PRECISION_INTEGER;
    config->outputPolicy = MEST_ALL_FLOWS;
    config->filteringPolicy = MEST_RAW_FLOW_VECTORS;

    return MEST_SUCCESS;
}

mestRuntimeConfig_t mestGetRuntimeConfig(const mestHandler_t *hnd)
{
    const mestHandlerPrivate_t *_hnd = hnd->privateData;
    return _hnd->runtimeConfig;
}

mestError_t mestSetRuntimeConfig(mestHandler_t *hnd, const mestRuntimeConfig_t *config)
{
    mestHandlerPrivate_t *_hnd = hnd->privateData;
    memcpy(&_hnd->runtimeConfig, config, sizeof(_hnd->runtimeConfig));
    return MEST_SUCCESS;
}

static void clearPreviousData(mestHandler_t *hnd)
{
    if(!hnd->privateData)
        return;
    mestHandlerPrivate_t *_hnd = hnd->privateData;
    mestRoiQueueElList_t *p, *q;

    p = _hnd->roiQueue.head;
    while(p)
    {
        q = p->next;
        free(p);
        p = q;
    }
    free(hnd->privateData);
    hnd->privateData = 0;
}

mestError_t mestInit(
        mestHandler_t *hnd,
        const mestMode_t mode,
        const mestResourceConfig_t* memory,
        const mestParamConfig_t *config)
{
    if(memory == NULL)
    {
        mvLog(MVLOG_FATAL, "Resource pointer is NULL!");
        return MEST_NULL_BUFFER;
    }

    size_t startAddr;

    startAddr = (size_t) memory->coordsHeapStartAddress;
    if (startAddr != ALIGN_UP((size_t) startAddr, MEST_BUFFER_ALIGNMENT))
    {
        mvLog(MVLOG_FATAL, "Coordinates buffer must be aligned at %d bytes!",
                MEST_BUFFER_ALIGNMENT);
        return MEST_BUFFER_NOT_ALIGNED;
    }

    startAddr = (size_t) memory->resultsTmpHeapStartAddress;
    if (startAddr != ALIGN_UP((size_t) startAddr, MEST_BUFFER_ALIGNMENT))
    {
        mvLog(MVLOG_FATAL, "Aux result buffer must be aligned at %d bytes!",
                MEST_BUFFER_ALIGNMENT);
        return MEST_BUFFER_NOT_ALIGNED;
    }

    int cacheLine;
    if((cacheLine = checkBufferCacheAlignment(memory->coordsHeapStartAddress,
            memory->coordsHeapSize)))
    {
        mvLog(MVLOG_FATAL, "Buffer address or size not aligned at %d bytes!", cacheLine);
        return MEST_BUFFER_NOT_ALIGNED;
    }

    if((cacheLine = checkBufferCacheAlignment(memory->resultsTmpHeapStartAddress,
            memory->resultsTmpHeapSize)))
    {
        mvLog(MVLOG_FATAL, "Buffer address or size not aligned at %d bytes!", cacheLine);
        return MEST_BUFFER_NOT_ALIGNED;
    }

    hnd->privateData = calloc(1, sizeof(mestHandlerPrivate_t));
    mestHandlerPrivate_t *_hnd = hnd->privateData;
    (void) mode;
    _hnd->coordsBuf = memory->coordsHeapStartAddress;
    _hnd->MAX_COORDINATES = memory->coordsHeapSize / sizeof(uint32_t);
    _hnd->resBuf = memory->resultsTmpHeapStartAddress;
    _hnd->MAX_RESULTS = memory->resultsTmpHeapSize / sizeof(uint32_t);
    _hnd->minStep = (1 << 30);
    memcpy(&_hnd->staticConfig, config, sizeof(_hnd->staticConfig));
    mestInitDefaultRuntimeConfig(&_hnd->runtimeConfig);

    mestError_t mestStatus;
    if((mestStatus = motestSetPostCallback(hnd, motestDoneHandler, hnd)) != MEST_SUCCESS)
        return mestStatus;

    return MEST_SUCCESS;
}

static inline uint32_t mestGetFlowBytes(mestHandler_t *hnd)
{
    mestHandlerPrivate_t *_hnd = hnd->privateData;
    const mestMeta_t *dummyMeta = &((mestOutput_t*)(0))->meta;
    const mestOutputPolicy_t policy = _hnd->runtimeConfig.outputPolicy;
    uint32_t ret = 0;

    if(policy == MEST_ALL_FLOWS)
    {
        ret += ALIGN_UP(_hnd->totalCoordinatesCnt * sizeof(dummyMeta->outputFlows[0]), 8);
    }
    else if(policy == MEST_AVERAGE_FLOW)
    {
        ret += ALIGN_UP(sizeof(dummyMeta->outputFlows[0]), 8);
    }
    else if(policy == MEST_AVERAGE_FLOW_PER_ROI)
    {
        ret += ALIGN_UP(_hnd->roiQueue.size * sizeof(dummyMeta->outputFlows[0]), 8);
    }

    return ret;
}

uint32_t mestGetResultSize(mestHandler_t *hnd)
{
    mestHandlerPrivate_t *_hnd = hnd->privateData;
    const mestMeta_t *dummyMeta = &((mestOutput_t*)(0))->meta;
    const mestOutputPolicy_t policy = _hnd->runtimeConfig.outputPolicy;
    uint32_t ret = 0;

    ret += ALIGN_UP(sizeof(mestOutput_t), 8);
    if(policy == MEST_ALL_FLOWS)
    {
        ret += mestGetFlowBytes(hnd);
        ret += _hnd->totalCoordinatesCnt * sizeof(dummyMeta->costs[0]);
    }
    else if(policy == MEST_AVERAGE_FLOW)
    {
        ret += mestGetFlowBytes(hnd) + sizeof(dummyMeta->costs[0]);
    }
    else if(policy == MEST_AVERAGE_FLOW_PER_ROI)
    {
        ret += mestGetFlowBytes(hnd);
        ret += _hnd->roiQueue.size * sizeof(dummyMeta->costs[0]);
    }

    return ret;
}

static mestError_t mestResultInit(
        mestHandler_t *hnd,
        mestOutput_t *result,
        const void* memory,
        const uint32_t size)
{
    mestMeta_t *meta = &result->meta;

    if (memory == NULL)
    {
        mvLog(MVLOG_ERROR, "Resource pointer is NULL!");
        return MEST_NULL_BUFFER;
    }

    const size_t startAddr = (size_t) memory;

    const uint32_t algnFlowBytesReq = mestGetFlowBytes(hnd);
    const uint32_t bytesReq = mestGetResultSize(hnd) - ALIGN_UP(sizeof(mestOutput_t), 8);

    if (bytesReq > size)
    {
        mvLog(MVLOG_ERROR, "Result buffer too small, minimum required is %" PRIu32 " bytes!",
                bytesReq);
        return MEST_BUFFER_TOO_SMALL;
    }

    meta->outputFlows = (xyPos_t*) startAddr;
    meta->costs = (void*) meta->outputFlows + algnFlowBytesReq;

    result->header.outputFlowCnt = 0;

    return MEST_SUCCESS;
}

mestError_t mestDestroy(mestHandler_t *hnd)
{
    clearPreviousData(hnd);
    return MEST_SUCCESS;
}


mestError_t mestRun(
        mestHandler_t *hnd,
        frameBuffer prevFrame,
        frameBuffer currFrame,
        mestBulkOutput_t* resultMem)
{
    mestHandlerPrivate_t *_hnd = hnd->privateData;
    uint32_t cfg;
    const frameSpec spec = _hnd->staticConfig.spec;
    mestError_t err = MEST_SUCCESS;

    if(resultMem == NULL)
        return MEST_ERROR;
    mestOutput_t* result = (mestOutput_t*) (resultMem->address);

    const void *addr = ((void*) resultMem->address) + ALIGN_UP(sizeof(mestOutput_t), 8);
    const uint32_t size = resultMem->size - ALIGN_UP(sizeof(mestOutput_t), 8);
    if((err = mestResultInit(hnd, result, addr, size)) != MEST_SUCCESS)
    {
        mvLog(MVLOG_ERROR, "Insufficient memory for results!");
        return MEST_ERROR;
    }

    if(_hnd->roiQueue.size <= 0)
    {
        mvLog(MVLOG_ERROR, "ROI queue is empty!");
        return MEST_ROI_QUEUE_EMPTY;
    }
    if(!compareSpecs(&prevFrame.spec, &_hnd->staticConfig.spec))
    {
        mvLog(MVLOG_ERROR, "Frame specs mismatch!");
        return MEST_ERROR;
    }
    if(!compareSpecs(&currFrame.spec, &_hnd->staticConfig.spec))
    {
        mvLog(MVLOG_ERROR, "Frame specs mismatch!");
        return MEST_ERROR;
    }
    if (prevFrame.p1 == NULL || currFrame.p1 == NULL || result == NULL)
    {
        mvLog(MVLOG_FATAL, "NULL buffer detected!");
        return MEST_NULL_BUFFER;
    }
    const size_t prevBuff = (size_t)(prevFrame.p1);
    const size_t currBuff = (size_t)(currFrame.p1);
    if (prevBuff % MEST_BUFFER_ALIGNMENT || currBuff % MEST_BUFFER_ALIGNMENT)
    {
        mvLog(MVLOG_ERROR, "Input buffers are not aligned to %d!", MEST_BUFFER_ALIGNMENT);
        return MEST_BUFFER_NOT_ALIGNED;
    }

    mvLog(MVLOG_DEBUG, "Width: %d", spec.width);
    if (spec.width <= 0 || spec.width > IMG_MAX_WIDTH)
    {
        mvLog(MVLOG_ERROR, "Invalid frame width!");
        return MEST_ERROR;
    }

    mvLog(MVLOG_DEBUG, "Height: %d", spec.height);
    if (spec.height <= 0 || spec.height > IMG_MAX_HEIGHT)
    {
        mvLog(MVLOG_ERROR, "Invalid frame height!");
        return MEST_ERROR;
    }

    mvLog(MVLOG_DEBUG, "Stride: %d", spec.stride);
    if (spec.stride <= 0 || spec.stride > IMG_MAX_STRIDE ||
        spec.stride % MEST_BUFFER_ALIGNMENT != 0)
    {
        mvLog(MVLOG_ERROR, "Invalid frame stride!");
        return MEST_ERROR;
    }

    //error case, not enough space
    if(ALIGN_UP(_hnd->coordsBufCnt, MEST_BUFFER_ALIGNMENT) > _hnd->MAX_COORDINATES)
    {
        mvLog(MVLOG_ERROR, "Auxiliary coordinates buffer is too small, "
                "minimum required is %" PRIu32 "!",
                _hnd->MAX_COORDINATES);
        return MEST_HEAP_ERROR;
    }
    if(_hnd->totalCoordinatesCnt > _hnd->MAX_RESULTS)
    {
        mvLog(MVLOG_ERROR, "Auxiliary results buffer is too small, "
                "minimum required is %" PRIu32 "!",
                _hnd->MAX_RESULTS);
        return MEST_HEAP_ERROR;
    }

    mvLog(MVLOG_DEBUG, "BPP: %d, Type: %d", spec.bytesPP, spec.type);

    cfg = 0;
    cfg |= (0x88 << 20); // 8 burst count for read/write
    cfg |= (0x3 << 18); // cache enabled and reset every time
    if (_hnd->runtimeConfig.precision == MEST_PRECISION_INTEGER)
        cfg |= (0 << 29);
    else if (_hnd->runtimeConfig.precision == MEST_PRECISION_HALF)
        cfg |= (2 << 29);
    else if (_hnd->runtimeConfig.precision == MEST_PRECISION_QUARTER)
        cfg |= (3 << 29);
    cfg |= (_hnd->totalCoordinatesCnt & 0x3FFFF);
    _hnd->staticConfigBits = cfg;

    _hnd->roiCrtPtr = _hnd->roiQueue.head;
    _hnd->outputStr = result;
    result->header.outputFlowCnt = 0;

    mvLog(MVLOG_DEBUG, "Coordinates in buffer: %" PRIu32 "", _hnd->coordsBufCnt);
    mvLog(MVLOG_DEBUG, "Total coordinates: %" PRIu32 "", _hnd->totalCoordinatesCnt);

    if((err = lockMest()) != MEST_SUCCESS)
    {
        mvLog(MVLOG_ERROR, "Could not lock MEST mutex!");
        return err;
    }

    mestPassFramesToModel(&prevFrame, &currFrame);
    _hnd->running = 1;

    if((err = motestConfigAndStartRoutine(_hnd)) != MEST_SUCCESS)
    {
        _hnd->running = 0;
        if(unlockMest() != MEST_SUCCESS)
        {
            mvLog(MVLOG_ERROR, "Error on unlocking MEST mutex!");
            return MEST_RUNTIME_ERROR;
        }
        return err;
    }

    mestWait(hnd);
    _hnd->running = 0;
    if(unlockMest() != MEST_SUCCESS)
    {
        mvLog(MVLOG_ERROR, "Error on unlocking MEST mutex!");
        return MEST_RUNTIME_ERROR;
    }
    return MEST_SUCCESS;
}

int mestRunning(mestHandler_t *hnd)
{
    mestHandlerPrivate_t *_hnd = hnd->privateData;
    return (_hnd->running == 1);
}
