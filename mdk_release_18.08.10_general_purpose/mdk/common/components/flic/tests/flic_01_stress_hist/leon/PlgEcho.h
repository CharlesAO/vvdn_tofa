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
   MSender<EchoMsg> out;
   void *threadFunc(void *);
   void Create(int data, int howMany);

  private:
   uint32_t what;
   uint32_t num;
};

class PlgEchoRecv : public ThreadedPlugin{
  public:
   SReceiver<EchoMsg> in;
   void *threadFunc(void *);
   void Create(int num, uint32_t *out);

  private:
   uint32_t  nHist;
   uint32_t *hist;
};

#endif