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

struct EchoMsg {
    uint32_t data;
};

class PlgEchoSend : public ThreadedPlugin{
  public:
   MSender<EchoMsg>  out;
   uint32_t what;
   void *threadFunc(void *);
   void Create();
};

class PlgEchoRecv : public ThreadedPlugin{
  public:
   SemSync            sync;
   SReceiver<EchoMsg> inS;
   SReceiver<EchoMsg> inV;
   void *threadFunc(void *);
   void Create();
   void Stop  ();
};

#endif