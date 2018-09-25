///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test plugins
///

#ifndef __TEST_PLG_ECHO_H__
#define __TEST_PLG_ECHO_H__

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include <ImgFrame.h>

class PlgEchoSend : public ThreadedPlugin{
  public:
   MSender  <ImgFramePtr> out;
   MReceiver<ImgFramePtr> in; //from frame pool
   void *threadFunc(void *);
   void Create();
};

class PlgEchoRecv : public ThreadedPlugin{
  public:
   SReceiver<ImgFramePtr> in;
   void *threadFunc(void *);
   void Create();
};

#endif