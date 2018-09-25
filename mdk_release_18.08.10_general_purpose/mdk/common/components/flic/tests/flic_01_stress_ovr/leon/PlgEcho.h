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

//Test params:
#define N_HIST       32 //num elems in histogram = num Sender threads
#define NUM_RECS   8000

struct EchoMsg {
    uint32_t data;
};

class PlgEchoSend : public ThreadedPlugin{
  public:
   MSender<EchoMsg> out;
   void *threadFunc(void *);
   void Create(int data);
   uint32_t sent;

  private:
   uint32_t what;
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
