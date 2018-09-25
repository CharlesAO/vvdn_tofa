///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Bridge between MEST API and HW or C Model.
///
/// This is the API of the bridge
///

#ifndef MEST_USE_H
#define MEST_USE_H

#include <MestApi.h>
#include <MestPrivateTypes.h>

/**
 *
 * @param [in] prevFrame - previous frame
 * @param [in] currFrame - current frame
 */
void mestPassFramesToModel(frameBuffer *prevFrame, frameBuffer *currFrame);

/**
 *
 * @param [in] roiCrtPtr - pointer to the ROI element in queue that needs to be processed
 */
void mestConfigureAndStart(const mestHandlerPrivate_t *hnd);

mestError_t mestWait(mestHandler_t *hnd);

void motestDestroy();

mestError_t motestSetPostCallback(mestHandler_t *hnd,
        void *(*callbackRoutine)(void *arg), void *callbackArgs);

mestError_t lockMest();

mestError_t unlockMest();

int checkBufferCacheAlignment(void *ptr, int size);

#endif //MEST_MYRIAD_USE_H
