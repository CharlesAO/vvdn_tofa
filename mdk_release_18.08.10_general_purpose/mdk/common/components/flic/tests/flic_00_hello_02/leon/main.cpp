///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     FramePool Receive + Forward
///

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include <MemAllocator.h>
#include <Pool.h>
#include <PoolObj.h>
#include <UnitTestApi.h>

#define FRM_NUM   8 //number of frames
#define FRM_SZ  128 //frame size

//##################################################
class PlgA : public ThreadedPlugin{
  public:
   MReceiver<PoPtr<PoBuf>> inO;
   MSender  <PoPtr<PoBuf>> out;

   void Create(){
     Add(&inO, ".inO");
     Add(&out, ".out");
   }

   virtual void *threadFunc(void *)
   {
     for(int i=0; i<FRM_NUM; i++)
     {
        PoPtr<PoBuf> fMsg;
        if(OK == inO.Receive(&fMsg)){           //a) get a frame to write
          memset((void*)fMsg->base, i, FRM_SZ); //b) write it
          out.Send(&fMsg);                      //c) give it to consumers
        }
     //---------------------------------------> //d)'fMsg' gets destroyed here
     }
     return NULL;
   }
};

//##################################################
class PlgB : public ThreadedPlugin{
  public:
   SReceiver<PoPtr<PoBuf>> in;
   void Create(){
      in.Create(2);
      Add(&in, ".in");
   }
   void *threadFunc(void *)
   {
       for(int i=0; i<FRM_NUM; i++)
       {
           PoPtr<PoBuf> fMsg;
           if(OK == in.Receive(&fMsg)){              //a) get a frame to read
                                                     //b) use input frame
                uint8_t data = ((uint8_t*)fMsg->base)[0];
                printf("Got a frame of %d\n", data);
                unitTestAssert(data == i);
           }
           //--------------------------------------> //c)'fMsg' gets destroyed here
       }
      return NULL;
   }
};

PlgPool<PoBuf> plgP;
PlgA    plgA;
PlgB    plgB;
Pipeline   p;

//##################################################
extern "C" void *POSIX_Init (void *)
{
    unitTestInit();

    RgnAlloc.Create( RgnBuff, DEF_POOL_SZ);
    plgP    .Create(&RgnAlloc, 2,  FRM_SZ);
    plgA    .Create();
    plgB    .Create();

    p.Add(&plgA);
    p.Add(&plgP);
    p.Add(&plgB);

    plgP.out.Link(&plgA.inO);
    plgA.out.Link(&plgB.in);

    p.Start();
    p.Wait();

    unitTestFinalReport();
    exit(0);
}
