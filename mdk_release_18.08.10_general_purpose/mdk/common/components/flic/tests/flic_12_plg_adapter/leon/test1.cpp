///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Adapter test (basic float->int)
///

#include <RecAdpt.h>
#include <UnitTestApi.h>

#define T1_SEED 123

//#########################################################
class PlgSrc : public ThreadedPlugin{
  public:
   MSender  <float> out;
   void Create(){ Add(&out);  }
   virtual void *threadFunc(void *)
   {
     float fVal = (float)T1_SEED;
     while(Alive())
     {
        rtems_task_wake_after(100);
        out.Send(&fVal);
        fVal++;
     }
     return NULL;
   }
};

//#########################################################
class PlgSnk : public ThreadedPlugin{
  public:
   SReceiver<int> in;
   void Create(){in.Create(2); Add(&in);}
   void *threadFunc(void *)
   {
       int exp = T1_SEED;
       while(Alive())
       {
           int iVal;
           if(OK == in.Receive(&iVal)){
              printf("  %d\n", iVal);
              unitTestAssert(iVal == exp);
              exp++;
           }
       }
       unitTestAssert(exp > T1_SEED);
       return NULL;
   }
};

//#########################################################
class MyAdapter : public PlgAdapter<float, int>
{
   virtual void Convert(const float *dIn, int *dOut){
     *dOut = (int)*dIn;
   };
};

//#########################################################
void test1(int n)
{
  for(int i=0; i<n; i++)
  {
    printf("test1 - run %d\n", i);

    Pipeline  p;
    PlgSrc    plgSrc;
    PlgSnk    plgSnk;
    MyAdapter plgAdpt;

    plgSrc .Create();
    plgSnk .Create();

    p.Add(&plgSrc);
    p.Add(&plgSnk);
    p.Add(&plgAdpt);

    plgSrc .out.Link(&plgAdpt.in);
    plgAdpt.out.Link(&plgSnk .in);

    p.Start();
    sleep(1);
    p.Stop();
    p.Wait();
    p.Delete();
  }
}