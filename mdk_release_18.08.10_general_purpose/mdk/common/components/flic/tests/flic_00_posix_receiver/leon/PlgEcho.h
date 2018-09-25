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
#include <PmqRec.h>

struct EchoMsg {
    uint32_t data;
    uint32_t m1, m2;
};

class PlgEchoSend : public ThreadedPlugin{
  public:
   MSender<EchoMsg> out;
   void *threadFunc(void *);
   void Create();
};

class PlgEchoRecv : public ThreadedPlugin{
  public:

  #if 0 //either implementation should work
   SReceiver<EchoMsg> in;
  #else
   PmqRec   <EchoMsg> in;
  #endif

   void *threadFunc(void *);
   void Create();
};

#endif