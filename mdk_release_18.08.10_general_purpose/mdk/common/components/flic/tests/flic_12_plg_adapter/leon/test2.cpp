///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Adapter test (Type1 to Type2)
///

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include <MemAllocator.h>
#include <Pool.h>
#include <PoolObj.h>
#include <RecAdpt.h>
#include <UnitTestApi.h>

#define FRM_NUM  16 //number of frames
#define FRM_SZ  128 //frame size
#define T2_SEED  53

typedef struct
{
  PoPtr<PoBuf> ptr;
  uint32_t     timeStamp; //[ms]
}Type1;

typedef struct
{
  PoPtr<PoBuf> frm;
  uint64_t     ts; //[us]
}Type2;

//User-defined type adapter
class MyTypeAdpt : public PlgAdapter<Type1, Type2>
{
   virtual void Convert(const Type1 *dIn, Type2 *dOut){
     dOut->frm = dIn->ptr;
     dOut->ts  = dIn->timeStamp * 1000;
   };
};

//#########################################################
class PlgA : public ThreadedPlugin{
  public:
   MReceiver<PoPtr<PoBuf>> inO;
   MSender  <Type1       > out;

   void Create(){
     Add(&inO, ".inO");
     Add(&out, ".out");
   }

   virtual void *threadFunc(void *)
   {
     for(int i=0; i<FRM_NUM; i++)
     {
        Type1 dOut;
        PoPtr<PoBuf> fMsg;
        if(OK == inO.Receive(&fMsg)){
          memset((void*)fMsg->base, i, FRM_SZ);
          dOut.ptr = fMsg;
          dOut.timeStamp = T2_SEED + i;
          out.Send(&dOut);
        }
     }
     return NULL;
   }
};

//#########################################################
class PlgB : public ThreadedPlugin{
  public:
   SReceiver<Type2> in;
   void Create(){
      in.Create(2);
      Add(&in, ".in");
   }
   void *threadFunc(void *)
   {
       for(int i=0; i<FRM_NUM; i++)
       {
           Type2 msg;
           if(OK == in.Receive(&msg)){
                uint8_t data = ((uint8_t*)msg.frm->base)[0];
                printf(" ... got a frame of %d\n", data);
                unitTestAssert(data   == i);
                unitTestAssert(msg.ts == (uint64_t)(T2_SEED+i)*1000);
           }
       }
      return NULL;
   }
};

//#########################################################
void test2(int n)
{
  for(int i=0; i<n; i++)
  {
    printf("test2 - run %d\n", i);

    MyTypeAdpt     plgX;
    PlgPool<PoBuf> plgP;
    PlgA    plgA;
    PlgB    plgB;
    Pipeline   p;

    RgnAlloc.Create( RgnBuff, DEF_POOL_SZ);
    plgP    .Create(&RgnAlloc, 2,  FRM_SZ);
    plgA    .Create();
    plgB    .Create();

    p.Add(&plgA);
    p.Add(&plgP);
    p.Add(&plgB);
    p.Add(&plgX);

    plgP.out.Link(&plgA.inO);
    plgA.out.Link(&plgX.in);
    plgX.out.Link(&plgB.in);

    p.Start();
    sleep(1);
    p.Stop();
    p.Wait();
    p.Delete();
    RgnAlloc.Delete();
  }
}