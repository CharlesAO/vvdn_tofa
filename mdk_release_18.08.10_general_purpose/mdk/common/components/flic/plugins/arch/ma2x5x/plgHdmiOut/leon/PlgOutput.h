///
/// @file      PlgOutput.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     PlgOutput header.
///

#ifndef __PLG_OUTPUT_H__
#define __PLG_OUTPUT_H__

#include <Flic.h>
#include <Receiver.h>
#include "PlgHdmiOut.h"

class PlgOutput : public ThreadedPlugin
{
  public:
    SReceiver<ImgFramePtr> in;

    void  *threadFunc(void *);
    void   Create();

    uint32_t frmCnt;
};

#endif
