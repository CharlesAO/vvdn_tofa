///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Dummy sink plugin
///

#ifndef __TEST_PLG_ECHO_H__
#define __TEST_PLG_ECHO_H__

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include <ImgFrame.h>

class PlgRecv : public ThreadedPlugin{
  public:
   SReceiver<ImgFramePtr> in;
   int       no;
   virtual void *threadFunc(void *);
   virtual void Create();
};

#endif