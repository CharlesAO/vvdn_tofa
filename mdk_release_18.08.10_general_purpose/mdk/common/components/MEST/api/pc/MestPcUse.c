///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Bridge implementation between MEST API and C Model.
///
/// This is the API implementation of the bridge for PC.
///

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <MotEst_wrapper.h>
#include <MestUse.h>

#include <mvLog.h>

ARGS args;

static frameBuffer *srcFrame, *refFrame;
static sem_t mestSemaphore;

void mestPassFramesToModel(frameBuffer *prevFrame, frameBuffer *currFrame)
{
    srcFrame = prevFrame;
    refFrame = currFrame;
}

void mestConfigureAndStart(const mestHandlerPrivate_t *hnd)
{
    const mestRoiQueueElList_t *roiCrtPtr = hnd->roiCrtPtr;
    FILE *fout_f1, *fout_f2;
    int roiHeight, roiWidth, roiStride;

    strcpy(args.ImgFname[0], "/tmp/mest_ref_frame.raw");
    strcpy(args.ImgFname[1], "/tmp/mest_src_frame.raw");

    fout_f1 = fopen(args.ImgFname[0], "wb");
    fout_f2 = fopen(args.ImgFname[1], "wb");
    if (roiCrtPtr->points == NULL)
    {
        roiWidth = (roiCrtPtr->endX - roiCrtPtr->startX + 1);
        roiHeight = (roiCrtPtr->endY - roiCrtPtr->startY + 1);
        roiStride = roiWidth * refFrame->spec.bytesPP;
        mvLog(MVLOG_DEBUG, "ROI real coords: %u %u %u %u\n",
                roiCrtPtr->startX, roiCrtPtr->startY,
                roiCrtPtr->endX, roiCrtPtr->endY);

        for (int i = roiCrtPtr->startY; i <= roiCrtPtr->endY; i++)
        {
            int offsetYRef, offsetXRef, offsetYSrc, offsetXSrc;
            offsetYRef = i * refFrame->spec.stride;
            offsetXRef = roiCrtPtr->startX * refFrame->spec.bytesPP;
            offsetYSrc = i * srcFrame->spec.stride;
            offsetXSrc = roiCrtPtr->startX * srcFrame->spec.bytesPP;

            fwrite(refFrame->p1 + offsetYRef + offsetXRef, 1, roiStride, fout_f1);
            fwrite(srcFrame->p1 + offsetYSrc + offsetXSrc, 1, roiStride, fout_f2);
        }
        args.Width = roiWidth;
        args.Height = roiHeight;
    }
    else
    {
        fwrite(refFrame->p1, refFrame->spec.height, refFrame->spec.stride, fout_f1);
        fwrite(srcFrame->p1, srcFrame->spec.height, srcFrame->spec.stride, fout_f2);
        args.Width = refFrame->spec.width;
        args.Height = refFrame->spec.height;
    }
    fclose(fout_f1);
    fclose(fout_f2);

    if (roiCrtPtr->points)
    {
        strcpy(args.ListFname, "/tmp/mest_coords.txt");
        FILE *fout = fopen(args.ListFname, "w");
        for (uint32_t i = 0, x, y; i < roiCrtPtr->pointsCnt; i++)
        {
            x = (roiCrtPtr->points[i] >> 16);
            y = (roiCrtPtr->points[i] & 0xFFFF);
            fprintf(fout, "%u %u\n", x, y);
        }
        fclose(fout);
        args.Width = refFrame->spec.width;
        args.Height = refFrame->spec.height;
    }
    args.RasterStep = (roiCrtPtr->rasterStep != 0 ? roiCrtPtr->rasterStep : 1);
    args.OutMode = 1;
    args.NumImg = 2;
    args.ImgMode = 0;
    args.SrchMode = (roiCrtPtr->points == NULL) ? 0 : 3;
    args.DisHpel = ((roiCrtPtr->roiCfg >> 29 & 0x1) == 0);
    args.DisQpel = ((roiCrtPtr->roiCfg >> 29 & 0x2) == 0);
    runMotestWrapper(args, roiCrtPtr->results, roiCrtPtr->pointsCnt);
}

mestError_t motestSetPostCallback(mestHandler_t *hnd,
        void *(*callbackRoutine)(void *arg),
        void *callbackArgs)
{
    mestHandlerPrivate_t *_hnd = hnd->privateData;
    _hnd->motestDoneHandler = callbackRoutine;
    _hnd->motestDoneHandlerArgs = callbackArgs;

    return MEST_SUCCESS;
}

inline mestError_t mestWait(mestHandler_t *hnd)
{
    mestHandlerPrivate_t *_hnd = hnd->privateData;
    while(mestRunning(hnd))
    {
        _hnd->motestDoneHandler(_hnd->motestDoneHandlerArgs);
        usleep(1);
    }

    return MEST_SUCCESS;
}

mestError_t mestMutexInit()
{
    if(sem_init(&mestSemaphore, 0, 1) < 0)
        return MEST_MUTEX_ERROR;
    return MEST_SUCCESS;
}

mestError_t mestMutexDeinit()
{
    if(sem_destroy(&mestSemaphore) < 0)
        return MEST_MUTEX_ERROR;
    return MEST_SUCCESS;
}

mestError_t lockMest()
{
    if(sem_wait(&mestSemaphore) < 0)
        return MEST_NOT_STARTED;
    return MEST_SUCCESS;
}

mestError_t unlockMest()
{
    if(sem_post(&mestSemaphore) < 0)
        return MEST_NOT_STARTED;
    return MEST_SUCCESS;
}

int checkBufferCacheAlignment(void *ptr, int size)
{
    // Just a dummy if
    if((size_t) ptr == (size & 0))
        return 0;
    return 0;
}
