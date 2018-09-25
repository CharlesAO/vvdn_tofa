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
#include <rtems.h>
#include <OsDrvDynIrq.h>

#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <MestUse.h>

#define MSS_CV_MOTEST_ID 2
#define MOTEST_SEM_NAME "/motest_sem"
#define MEST_AUX_SEM_NAME "/mest_aux_sem"
#define MEST_RUN_SEM_NAME "/mest_run_sem"

static frameBuffer *srcFrame, *refFrame;
static rtems_vector_number motestIrq = 0;
static sem_t *motestSem = SEM_FAILED;

void mestPassFramesToModel(frameBuffer *prevFrame, frameBuffer *currFrame)
{
    srcFrame = prevFrame;
    refFrame = currFrame;
}

void motestIntr(uint32_t source)
{
    UNUSED(source);
    sem_post(motestSem);
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

//    assert(refFrameAddr == ALIGN_UP(refFrameAddr, 128));
//    assert(srcFrameAddr == ALIGN_UP(srcFrameAddr, 128));
//    assert(coordsAddr   == ALIGN_UP(coordsAddr, 128));
//    assert(resultsAddr  == ALIGN_UP(resultsAddr, 128));

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
    SET_REG_WORD(MOTEST0_INT_MASK_ADR, 1);
    SET_REG_BIT_NUM(MOTEST0_CFG_MEM_CTRL_ADR, 28);

    // Flush data cache to avoid misreading from MotEst HW
    rtems_cache_flush_multiple_data_lines(refFrameAddr, roiStride * (roiHeight - 1) + roiWidth);
    rtems_cache_flush_multiple_data_lines(srcFrameAddr, roiStride * (roiHeight - 1) + roiWidth);
    if(roiCrtPtr->points)
        rtems_cache_flush_multiple_data_lines(coordsAddr, roiCrtPtr->pointsCnt * 4);

    SET_REG_BIT_NUM(MOTEST0_CFG_ADR, 31); // block start
}

mestError_t motestSetPostCallback(mestHandler_t *hnd,
        void *(*callbackRoutine)(void *arg),
        void *callbackArgs)
{
    mestHandlerPrivate_t *_hnd = hnd->privateData;
    _hnd->motestDoneHandler = callbackRoutine;
    _hnd->motestDoneHandlerArgs = callbackArgs;

    sem_t *sem = sem_open(MEST_AUX_SEM_NAME, O_RDWR);
    sem_wait(sem);
    if(!motestIrq)
    {
        if(OsDrvDynIrqRequest(&motestIrq) != RTEMS_SUCCESSFUL)
            return MEST_ERROR;
    }
    if((motestSem = sem_open(MOTEST_SEM_NAME, O_RDWR)) == SEM_FAILED)
    {
        motestSem = sem_open(MOTEST_SEM_NAME, O_RDWR | O_CREAT, 0, 0);
        assert(motestSem != SEM_FAILED);
    }

    if (swcWhoAmI() == PROCESS_LEON_OS)
    {
        DrvIcbIrqClear(motestIrq);
        DrvIcbDynamicIrqConfig(LRT_TO_LOS, LEON_MSS_IRQ_MEST, motestIrq, 0x1);
        DrvIcbSetupIrq(motestIrq, 5, POS_EDGE_INT, motestIntr);
    }
    else
    {
        DrvIcbIrqClear(LEON_MSS_IRQ_MEST);
        DrvIcbSetupIrq(LEON_MSS_IRQ_MEST, 5, POS_EDGE_INT, motestIntr);
    }
    sem_post(sem);

    return MEST_SUCCESS;
}

static void motestRemoveIrq()
{
    sem_t *sem = sem_open(MEST_AUX_SEM_NAME, O_RDWR);
    sem_wait(sem);
    if(motestIrq)
        OsDrvDynIrqRelease(motestIrq);
    motestIrq = 0;
    sem_post(sem);
}

void waitForEof()
{
    SET_REG_WORD(MOTEST0_INT_CLEAR_ADR, 1 << 0);
    if(swcWhoAmI() == PROCESS_LEON_OS)
        DrvIcbIrqClear(motestIrq);
    else
        DrvIcbIrqClear(LEON_MSS_IRQ_MEST);
}

mestError_t mestWait(mestHandler_t *hnd)
{
    mestHandlerPrivate_t *_hnd = hnd->privateData;
    while(mestRunning(hnd))
    {
        sem_wait(motestSem);
        waitForEof();
        rtems_cache_invalidate_multiple_data_lines(_hnd->roiCrtPtr->results,
                _hnd->roiCrtPtr->pointsCnt * sizeof(_hnd->roiCrtPtr->results[0]));
        _hnd->motestDoneHandler(_hnd->motestDoneHandlerArgs);
    }

    return MEST_SUCCESS;
}

mestError_t mestMutexInit()
{
    sem_t *sem;
    do
    {
        sem = sem_open(MEST_RUN_SEM_NAME, O_RDWR | O_CREAT, 0, 1);
        if(sem == SEM_FAILED)
        {
            if(errno == EEXIST)
                sem = sem_open(MEST_RUN_SEM_NAME, O_RDWR);
        }
    } while(sem == SEM_FAILED);
    do
    {
        sem = sem_open(MEST_AUX_SEM_NAME, O_RDWR | O_CREAT, 0, 1);
        if(sem == SEM_FAILED)
        {
            if(errno == EEXIST)
                sem = sem_open(MEST_AUX_SEM_NAME, O_RDWR);
        }
    } while(sem == SEM_FAILED);
    return MEST_SUCCESS;
}

mestError_t mestMutexDeinit()
{
    sem_t *sem;

    motestRemoveIrq();

    sem = sem_open(MEST_RUN_SEM_NAME, O_RDWR);
    if(sem == SEM_FAILED)
        return MEST_MUTEX_ERROR;
    sem_close(sem);
    sem_unlink(MEST_RUN_SEM_NAME);

    sem = sem_open(MEST_AUX_SEM_NAME, O_RDWR);
    if(sem == SEM_FAILED)
        return MEST_MUTEX_ERROR;
    sem_close(sem);
    sem_unlink(MEST_AUX_SEM_NAME);

    return MEST_SUCCESS;
}

mestError_t lockMest()
{
    sem_t *sem;
    if((sem = sem_open(MEST_RUN_SEM_NAME, O_RDWR)) == SEM_FAILED)
        return MEST_NOT_STARTED;
    sem_wait(sem);
    return MEST_SUCCESS;
}

mestError_t unlockMest()
{
    sem_t *sem;

    if((sem = sem_open(MEST_RUN_SEM_NAME, O_RDWR)) == SEM_FAILED)
        return MEST_RUNTIME_ERROR;
    sem_post(sem);
    return MEST_SUCCESS;
}

int checkBufferCacheAlignment(void *ptr, int size)
{
    const int __CACHE_LINE_SIZE = rtems_cache_get_data_line_size();
    if(ptr != ALIGN_UP(ptr, __CACHE_LINE_SIZE) ||
       size != ALIGN_UP(size, __CACHE_LINE_SIZE))
    {
        return __CACHE_LINE_SIZE;
    }
    return 0;
}
