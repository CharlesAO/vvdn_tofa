///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains basic declarations for the SIPP concurrency application
///            app layer types
///

#ifndef __CONCURRENCY_APP_TYPES_H__
#define __CONCURRENCY_APP_TYPES_H__

/////////////////////////////////////////////////////////////////////////////////
//  Macros
/////////////////////////////////////////////////////////////////////////////////
#define CC_APP_EVENT_HISTORY_SIZE 0x8

/////////////////////////////////////////////////////////////////////////////////
//  Types
/////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// Pipeline state enum

typedef enum
{
    CC_APP_PIPE_STATE_UNINIT            = 0x00,
    CC_APP_PIPE_STATE_IDLE              = 0x01,
    CC_APP_PIPE_STATE_RUNNING           = 0x02,
    CC_APP_PIPE_STATE_COMPLETE          = 0x03

} CC_APP_PIPE_STATE;

//////////////////////////////////////////////////////////////
// Pipeline state enum

typedef enum
{
    CC_APP_PIPE_EVENT_NONE              = 0x00,
    CC_APP_PIPE_EVENT_FRAME_DONE        = 0x01

} CC_APP_PIPE_EVENT;


//////////////////////////////////////////////////////////////
// CC App pipeline definition

typedef struct
{
    CC_APP_PIPE_STATE eState;
    SippPipeline *    sippFwHandle;

    SippFilter * dmaIn;
    SippFilter * dmaOut;

    SippFilter *lscMa2x5x0;
    SippFilter *rawMa2x5x0;
    SippFilter *debayerMa2x5x0;
    SippFilter *genLuma0;
    SippFilter *genChroma0;
    SippFilter *lumaBlur0;
    SippFilter *sharpenMa2x5x1;
    SippFilter *ydnsMa2x5x1;
    SippFilter *polyphasefirMa2x5x1;
    SippFilter *medianMa2x5x1;
    SippFilter *chromadnsMa2x5x1;
    SippFilter *colorcombMa2x5x1;
    SippFilter *lutMa2x5x1;

    SippFilter *sobel0;
    SippFilter *erode3x30;
    SippFilter *dilate3x30;
    SippFilter *lut0;

    SippFilter * conv5x5;
    SippFilter * box5x5;
    SippFilter * lap5x5;

    u32               framesComplete;

    CC_APP_PIPE_EVENT eventList[CC_APP_EVENT_HISTORY_SIZE];
    u32               eventListWrPtr;
    u32               eventListRdPtr;

    u32               runAsync;

} ConcurrencyAppPipe, * pConcurrencyAppPipe;

#endif // __CONCURRENCY_APP_TYPES_H__
