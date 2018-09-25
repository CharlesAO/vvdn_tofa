///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Hello World : pass 4 messages from Sender to Receiver
///

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include <UnitTestApi.h>

class MyMsg{
  public:
    uint32_t data;
};

//##################################################
class PlgA : public ThreadedPlugin{
  public:
   MSender<MyMsg> out;
   void Create(){
      Add(&out, ".out");
   }
   virtual void *threadFunc(void *)
   {
       MyMsg m;
       m.data = 100; out.Send(&m);
       m.data = 200; out.Send(&m);
       m.data = 300; out.Send(&m);
       m.data = 400; out.Send(&m);
       return NULL;
   }
};

//##################################################
class PlgB : public ThreadedPlugin{
  public:
   SReceiver<MyMsg> in;
   void Create(){
      in.Create(2);
      Add(&in, ".in");
   }
   void *threadFunc(void *){
      MyMsg m;
      unitTestAssert(OK == in.Receive(&m)); unitTestAssert(m.data == 100);
      unitTestAssert(OK == in.Receive(&m)); unitTestAssert(m.data == 200);
      unitTestAssert(OK == in.Receive(&m)); unitTestAssert(m.data == 300);
      unitTestAssert(OK == in.Receive(&m)); unitTestAssert(m.data == 400);
      return NULL;
   }
};

PlgA  plgA;
PlgB  plgB;
Pipeline p;

//##################################################
extern "C" void *POSIX_Init (void *)
{
    unitTestInit();

    plgA.Create();
    plgB.Create();

    p.Add(&plgA);
    p.Add(&plgB);

    plgA.out.Link(&plgB.in);

    p.Start();
    p.Wait();
    unitTestFinalReport();
    exit(0);
}
