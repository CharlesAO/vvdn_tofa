///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Dummy source plugin
///

#ifndef __PLG_SRC_H__
#define __PLG_SRC_H__

#include <Flic.h>
#include <Receiver.h>
#include <Sender.h>
#include <ImgFrame.h>

class PlgSrc: public ThreadedPlugin
{
  public:
    MReceiver<ImgFramePtr> inO;  //frame from frame-pool
    MSender  <ImgFramePtr> out;  //send frame to consumer

    void    *threadFunc(void *);
    void     Create();
};

#endif
