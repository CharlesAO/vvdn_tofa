///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Dummy buffer plugin
///

#ifndef __TEST_PLG_BUF_H__
#define __TEST_PLG_BUF_H__

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include <ImgFrame.h>

class PlgBuf : public ThreadedPlugin{
  public:
   SReceiver<ImgFramePtr> in;
   MSender  <ImgFramePtr> out;
   virtual void *threadFunc(void *);
   virtual void Create();
};

#endif