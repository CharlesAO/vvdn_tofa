///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Bridge implementation between MEST API and MotEst hardware block.
///
/// This is the API implementation of the bridge for Leon processor.
///

#include <stdio.h>
#include <assert.h>

#include <registersMyriad.h>
#include <DrvRegUtilsDefines.h>
#include <swcLeonUtilsDefines.h> //for NOP
#include <DrvLeonL2C.h>
#include <swcLeonUtils.h>
#include <DrvIcb.h>
#include <mvMacros.h>

#include <unistd.h>
#include <time.h>

#include <MestUse.h>

#define MSS_CV_MOTEST_ID 2
#define NOPS(X) __asm__ __volatile__ ("NOP " # X)

static frameBuffer *srcFrame, *refFrame;

void mestPassFramesToModel(frameBuffer *prevFrame, frameBuffer *currFrame)
{
    srcFrame = prevFrame;
    refFrame = currFrame;
}

void mestConfigureAndStart(const mestHandlerPrivate_t *hnd)
{
    const mestRoiQueueElList_t *roiCrtPtr = hnd->roiCrtPtr;
    const int offset_prev = roiCrtPtr->startY * srcFrame->spec.stride
                          + roiCrtPtr->startX * srcFrame->spec.bytesPP;
    const int offset_curr = roiCrtPtr->startY * refFrame->spec.stride
                          + roiCrtPtr->startX * refFrame->spec.bytesPP;

    const uint8_t  *refFrameAddr = (refFrame->p1 + offset_curr);
    const uint8_t  *srcFrameAddr = (srcFrame->p1 + offset_prev);
    const uint32_t *coordsAddr   = roiCrtPtr->points;
    const uint32_t *resultsAddr  = roiCrtPtr->results;

    const int roiWidth  = (roiCrtPtr->endX - roiCrtPtr->startX + 1);
    const int roiHeight = (roiCrtPtr->endY - roiCrtPtr->startY + 1);
    const int roiStride = refFrame->spec.stride;

    CLR_REG_BIT_NUM(CCS_RSTN_CTRL_ADR, MSS_CV_MOTEST_ID);
    SET_REG_BIT_NUM(CCS_RSTN_CTRL_ADR, MSS_CV_MOTEST_ID);

    SET_REG_WORD(MOTEST0_REF_BASE_ADR, (uint32_t) refFrameAddr);
    SET_REG_WORD(MOTEST0_SRC_BASE_ADR, (uint32_t) srcFrameAddr);
    SET_REG_WORD(MOTEST0_COORDS_BASE_ADR, (uint32_t) coordsAddr);
    SET_REG_WORD(MOTEST0_RES_BASE_ADR, (uint32_t) resultsAddr);

    SET_REG_WORD(MOTEST0_IMAGE_SIZE_ADR, (roiHeight << 16) | roiWidth);
    SET_REG_WORD(MOTEST0_LINE_STRIDE_ADR, roiStride);
    SET_REG_WORD(MOTEST0_BLOCK_NUMBER_COORDS_ADR, roiCrtPtr->rasterStep << 24 |
                                                                      1 << 23 | // cost+mv
                                                                      0 << 18 | // MAX_READ_BLOCK
                                                    (roiCrtPtr->pointsCnt - 1));
    SET_REG_WORD(MOTEST0_CFG_ADR, roiCrtPtr->roiCfg);
    SET_REG_BIT_NUM(MOTEST0_CFG_MEM_CTRL_ADR, 28);

    SET_REG_BIT_NUM(MOTEST0_CFG_ADR, 31); // block start
}

void waitForEof()
{
    uint32_t status;
    do {
        status = GET_REG_WORD_VAL(MOTEST0_INT_STATUS_ADR);
        // do NOPs for a while
        NOPS(10);
    } while ((status & (1 << 0)) == 0);
    SET_REG_WORD(MOTEST0_INT_CLEAR_ADR, 1 << 0);
}

mestError_t motestSetPostCallback(mestHandler_t *hnd,
        void *(*callbackRoutine)(void *arg), void *callbackArgs)
{
    mestHandlerPrivate_t *_hnd = hnd->privateData;
    _hnd->motestDoneHandler = callbackRoutine;
    _hnd->motestDoneHandlerArgs = callbackArgs;

    return MEST_SUCCESS;
}

mestError_t mestWait(mestHandler_t *hnd)
{
    mestHandlerPrivate_t *_hnd = hnd->privateData;
    do
    {
        waitForEof();
        _hnd->motestDoneHandler(_hnd->motestDoneHandlerArgs);
    } while(mestRunning(hnd));

    return MEST_SUCCESS;
}

mestError_t mestMutexInit()
{
    return MEST_SUCCESS;
}

mestError_t mestMutexDeinit()
{
    return MEST_SUCCESS;
}

mestError_t lockMest()
{
    uint32_t status = GET_REG_WORD_VAL(MOTEST0_CFG_ADR);
    if((status & (1 << 31)) != 0)
        return MEST_ALREADY_STARTED;
    return MEST_SUCCESS;
}

mestError_t unlockMest()
{
    return MEST_SUCCESS;
}

int checkBufferCacheAlignment(void *ptr, int size)
{
    const int __CACHE_LINE_SIZE = 64;
    if(ptr != ALIGN_UP(ptr, __CACHE_LINE_SIZE) ||
       size != ALIGN_UP(size, __CACHE_LINE_SIZE))
    {
        return __CACHE_LINE_SIZE;
    }
    return 0;
}
