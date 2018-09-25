/**************************************************************************************************

 @File         : FrameMgrApi.h
 @Author       : MT
 @Brief        : Contain Frame Manager Api interface
 Date          : 01 - March - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : � Movidius Srl 2014, � Movidius Ltd 2015

 Description :


 **************************************************************************************************/

#ifndef __FRAME_MGR_API_H__
#define __FRAME_MGR_API_H__


#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
 **************************************************************************************************/
#include "IcTypes.h"

/**************************************************************************************************
 ~~~  Specific #defines
 **************************************************************************************************/


/**************************************************************************************************
 ~~~  Basic typdefs
 **************************************************************************************************/


/**************************************************************************************************
 ~~~  Exported Functions
 **************************************************************************************************/
//
int FrameMgrCreatePool(FramePool *pool, FrameT *frames,
        FrameProducedCB *callbacks, int32_t nCallbacks);

//
FrameT * FrameMgrAcquireFrame(FramePool *pool);

//
void FrameMgrProduceFrame(FrameT *frame);

//
void FrameMgrReleaseFrame(FrameT *frame);

//
FrameT * FrameMgrLockFrame(FramePool *pool, uint64_t frameSel, int32_t tsRel);

//
void FrameMgrUnlockFrame(FrameT *frame);

//
void FrameMgrIncreaseNrOfConsumer(FrameT *frame, uint32_t addNr);

// time stamp adding
void FrameMgrAndAddTimeStamp(FrameT *oFrame, icTimestamp timeStamp);

// add time stamps from other frames, output history time-line
void FrameMgrAddTimeStampHist(FrameT *oFrame, FrameT *iFrame);
// add a new frame at the end of an already existing frame list
void FrameMgrAddFrameToPool(FramePool *pool, FrameT *frame);

#ifdef __cplusplus
}
#endif

#endif //__FRAME_MGR_API_H__
