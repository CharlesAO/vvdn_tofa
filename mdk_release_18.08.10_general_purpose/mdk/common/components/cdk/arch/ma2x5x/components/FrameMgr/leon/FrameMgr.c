/**************************************************************************************************

 @File         : FrameMgr.c
 @Author       : MT
 @Brief        : Contain Frame Manager functionality
 Date          : 01 - March - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : � Movidius Srl 2014, � Movidius Ltd 2015

 Description :


 **************************************************************************************************/


/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
 **************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "FrameMgrApi.h"
#include "ipipeDbg.h"

#if defined(__sparc)
#include "swcLeonUtils.h"
#define StartCriticalSection()            swcLeonSetPIL(15)
#define EndCriticalSection(_x)            swcLeonSetPIL(_x)
#else
#define StartCriticalSection() (1)
#define EndCriticalSection(_x) (_x++)
#endif


/**************************************************************************************************
 ~~~  Exported Functions Implementation
 **************************************************************************************************/

//
int FrameMgrCreatePool(FramePool *pool, FrameT *frames, FrameProducedCB *callbacks, int32_t nCallbacks) {
    uint32_t i = 0;
    FrameT *locFrm;
    uint32_t old_value = StartCriticalSection();
    locFrm = frames;
    pool->frames = frames;
    pool->nextFree = frames;
    if(locFrm) {
        while(locFrm) {
            locFrm->framePool   = pool;
            locFrm->timestampNr = 0;
            locFrm->refcnt      = 0;
            locFrm->status      = FRAMEMGR_EMPTY;
            locFrm              = locFrm->next;
            i++;
        }
        pool->nFrames = i;
    }
    pool->seqNo      = 0;
    pool->nCallbacks = nCallbacks;
    pool->callbacks  = callbacks;
    EndCriticalSection(old_value);
    return 0;
}

FrameT * FrameMgrAcquireFrame(FramePool *pool) {
    uint32_t i = 0;
    FrameT *locFrm;
    uint32_t old_value = StartCriticalSection();
    locFrm  = pool->nextFree;
    for (i = 0; i < pool->nFrames; i++) {
        if (0 == locFrm) {
            locFrm = pool->frames;
        }
        if((FRAMEMGR_EMPTY ==locFrm->status) || (FRAMEMGR_DONE == locFrm->status)) {
            pool->nextFree = locFrm->next;
            locFrm->status = FRAMEMGR_ACQUIRED;
            locFrm->seqNo   = pool->seqNo;
            pool->seqNo++;
            locFrm->timestampNr = 0; // no timestamps informations
            EndCriticalSection(old_value);
            return(locFrm);
        }
        locFrm = locFrm->next;
    }
    //assert(1 == 0); // TODO: delete debug code
    EndCriticalSection(old_value);
    return (FrameT *)0;
}

//
void FrameMgrProduceFrame(FrameT *frame) {
    uint32_t old_value = StartCriticalSection();
    FramePool *pool = frame->framePool;
    uint32_t i = 0;
    frame->status = FRAMEMGR_PRODUCED;
    frame->refcnt += pool->nCallbacks;
    EndCriticalSection(old_value);
    for (i = 0; i < pool->nCallbacks; i++) {
        assert(NULL != pool->callbacks[i].callback);
        (pool->callbacks[i].callback)(frame, pool->callbacks[i].pluginObj);
    }
}

//
void FrameMgrReleaseFrame(FrameT *frame) {
    assert(frame);
    uint32_t old_value = StartCriticalSection();
    assert(frame->refcnt > 0);
    frame->refcnt--;
    if(0 == frame->refcnt) {
        frame->status = FRAMEMGR_DONE;
    }
    EndCriticalSection(old_value);
}

// useful for the fifo scenario and for Lock Zsl scenario
void FrameMgrIncreaseNrOfConsumer(FrameT *frame, uint32_t addNr) {
    uint32_t old_value = StartCriticalSection();
    // frame need to be produced, in order to change this
    assert(frame->status & (FRAMEMGR_DONE|FRAMEMGR_PRODUCED|FRAMEMGR_ACQUIRED));
    frame->refcnt += addNr;
    EndCriticalSection(old_value);
}

// time stamp adding
void FrameMgrAndAddTimeStamp(FrameT *oFrame, icTimestamp timeStamp) {
    assert(oFrame->timestampNr < MAX_TIMESTAMPS);
    oFrame->timestamp[oFrame->timestampNr] = timeStamp;
    oFrame->timestampNr++;
}

// add time stamps from other frames, output history time-line
void FrameMgrAddTimeStampHist(FrameT *oFrame, FrameT *iFrame) {
    uint32_t i;
    assert((oFrame->timestampNr + iFrame->timestampNr) <= MAX_TIMESTAMPS);
    for (i = 0; i < (iFrame->timestampNr); i++) {
        oFrame->timestamp[i + oFrame->timestampNr] = iFrame->timestamp[i];
    }
    oFrame->timestampNr += iFrame->timestampNr;
}


//
FrameT * FrameMgrLockFrame(FramePool *pool, uint64_t frameSel, int32_t tsRel) {
    assert(pool); // un initialized source. Not possible to lock zsl from that output
    FrameT      *locFrm = pool->frames;
    FrameT      *chosedFrm = NULL;
    uint32_t    old_value = StartCriticalSection();

    if(tsRel) { // selection base on relative time
        int64_t  whatedTimeStamp = (int64_t)frameSel;
        int64_t  bestDif         = INT64_MAX;
        while(locFrm) {
            if(locFrm->status & (FRAMEMGR_DONE|FRAMEMGR_PRODUCED)) { // frame is produced
                // frame need to have timestamp in order to allow selection base on this
                assert(locFrm->timestampNr>0);
                int64_t frameTimeStamp = (int64_t)locFrm->timestamp[locFrm->timestampNr-1];
                int64_t diff = frameTimeStamp - whatedTimeStamp;
                if(diff < 0) diff = -diff;
                if(diff < bestDif) {
                    bestDif = diff;
                    chosedFrm = locFrm;
                }
            }
            locFrm = locFrm->next;
        }
    }
    else {
        int32_t     whatedSeq = (int32_t)frameSel;
        int32_t    bestDif    = INT32_MAX;
        while(locFrm) {
            if(locFrm->status & (FRAMEMGR_DONE|FRAMEMGR_PRODUCED)) { // frame is produced
                int32_t diff = locFrm->seqNo - whatedSeq;
                if(diff < 0) diff = -diff;
                if(diff < bestDif) {
                    bestDif = diff;
                    chosedFrm = locFrm;
                }
            }
            locFrm = locFrm->next;
        }
    }
    if (chosedFrm) {
        FrameMgrIncreaseNrOfConsumer(chosedFrm, 1);
        // step it back in lock area if was available for reusing
        if(chosedFrm->status & FRAMEMGR_DONE) {
            chosedFrm->status = FRAMEMGR_PRODUCED;
        }
    }
    EndCriticalSection(old_value);
    return chosedFrm;
}

//
void FrameMgrUnlockFrame(FrameT *frame) {
    // look a frame involve increasing frame->refcnt by 1. As the consumers are smaller, the frame will keep
    // look, until at unlock command. Supported multiple look on same frame.
    FrameMgrReleaseFrame(frame);
}
void FrameMgrAddFrameToPool(FramePool *pool, FrameT *frame) {
    FrameT *locFrm, *newFrame;
    uint32_t i, planeNo;;
    uint32_t old_value = StartCriticalSection();
    newFrame  = frame;
    locFrm = pool->frames;
    for (i = 0; i < pool->nFrames; i++) {
        if (locFrm->next == NULL) {
            locFrm->next = newFrame;
            newFrame->framePool   = pool;
            newFrame->timestampNr = 0;
            newFrame->refcnt      = 0;
            newFrame->status     = FRAMEMGR_EMPTY;
            newFrame->next       = NULL;
            newFrame->type  = locFrm->type;
            newFrame->nPlanes  = locFrm->nPlanes;
            newFrame->seqNo  = 0;
            for(planeNo=0;planeNo<IC_TYPES_FRM_MAX_NR_PLANES;planeNo++){
                newFrame->stride[planeNo]  = locFrm->stride[planeNo];
                newFrame->height[planeNo]  = locFrm->height[planeNo];
                newFrame->fbPtr[planeNo]  = locFrm->fbPtr[planeNo];
            }
        }
        else{
            locFrm = locFrm->next;
        }
    }
    pool->nFrames++;
    EndCriticalSection(old_value);
}

