///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Typical plugin with input and output (see plgB)
///
#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include <Pool.h>
#include <PoolObj.h>
#include <MemAllocator.h>
#include <UnitTestApi.h>

#define FRM_NUM   8 //number of frames
#define FRM_SZ  128 //frame size
#define OUT_FN(x) x+1

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
        if(OK == inO.Receive(&fMsg)){            //a) get a frame to write
           memset((void*)fMsg->base, i, FRM_SZ); //b) write it
           out.Send(&fMsg);                      //c) give it to consumers
        }
        //---------------------------------------//d) 'fMsg' gets destroyed here
     }
     return NULL;
   }
};
//##################################################
class PlgB : public ThreadedPlugin{
  public:
   SReceiver<PoPtr<PoBuf>> in;
   MReceiver<PoPtr<PoBuf>> inO;
   MSender  <PoPtr<PoBuf>> out;

   void Create(){
      in.Create(2);
      Add(&in , ".in");
      Add(&inO, ".inO");
      Add(&out, ".out");
   }
   void genOutput(PoPtr<PoBuf> fIn, PoPtr<PoBuf> fOut)
   {
     //make out = in + 1
       uint8_t *in  = (uint8_t*)fIn ->base;
       uint8_t *out = (uint8_t*)fOut->base;
       for(int i=0; i<FRM_SZ; i++)
           out[i] = OUT_FN(in[i]);
   }
   void *threadFunc(void *)
   {
       for(int i=0; i<FRM_NUM; i++)
       {
           PoPtr<PoBuf> msgIn;
           PoPtr<PoBuf> msgOut;
           int err = 0;

           err += in .Receive(&msgIn);  //a) get a frame to read
           err += inO.Receive(&msgOut); //b) get an output buffer to write

           if(!err){                    //c) generate output-frame based on input-frame
             genOutput(msgIn, msgOut);
             out.Send(&msgOut);         //d) fwd to consumers
           }
           //-------------------------- //e) msgIn, msgOut destroyed here...
       }
      return NULL;
   }
};

//##################################################
class PlgC : public ThreadedPlugin{
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
           if(OK == in.Receive(&fMsg)){            //a) get a frame to read
              uint8_t data = ((uint8_t*)fMsg->base)[0];
              printf("Got a frame of %d\n", data); //b) use input frame
              unitTestAssert(data == OUT_FN(i));
           }
           //--------------------------------------//c) fMsg destroyed here
       }
      return NULL;
   }
};

PlgPool<PoBuf> plgPa;
PlgPool<PoBuf> plgPb;
PlgA    plgA;
PlgB    plgB;
PlgC    plgC;
Pipeline   p;

//##################################################
extern "C" void *POSIX_Init (void *)
{
    unitTestInit();
    RgnAlloc.Create(RgnBuff, DEF_POOL_SZ);

    plgPa.Create(&RgnAlloc, 2, FRM_SZ);
    plgPb.Create(&RgnAlloc, 2, FRM_SZ);
    plgA .Create();
    plgB .Create();
    plgC .Create();

    p.Add(&plgA);
    p.Add(&plgB);
    p.Add(&plgC);
    p.Add(&plgPa);
    p.Add(&plgPb);

    plgPa.out.Link(&plgA.inO);
    plgPb.out.Link(&plgB.inO);
    plgA .out.Link(&plgB.in);
    plgB .out.Link(&plgC.in);

    p.Start();
    p.Wait();

    unitTestFinalReport();
    exit(0);
}
