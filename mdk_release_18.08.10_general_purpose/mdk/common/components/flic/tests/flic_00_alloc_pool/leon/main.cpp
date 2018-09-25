///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Pool alloc/free cycle via Region/Heap allocators
///

#include <Flic.h>
#include <Pool.h>
#include <MemAllocator.h>
#include <UnitTestApi.h>
#include <PoolObj.h>
#if defined(MA2450)
#include <OsDrvTimer.h>
#endif

#define N_RUNS      (100*1000)
#define N_POOL_FRMS 3
#define FRM_SZ      128 //[bytes]

PlgPool<PoBuf> plgPool;

#if defined(MA2450)
tyTimeStamp  ticks;
u64          cycles;
#endif

//#########################################################
extern "C" void *POSIX_Init(void *)
{
    unitTestInit();

    #if defined(MA2450)
    OsDrvTimerInit();
    printf("Please wait ...\n");
    OsDrvTimerStartTicksCount(&ticks);
    #endif
  //=======================================================
  //Using Region manager and custom buffer
    RgnAlloc.Create(RgnBuff, DEF_POOL_SZ);
    for(int i=0; i<N_RUNS; i++)
    {
        plgPool.Create(&RgnAlloc, N_POOL_FRMS, FRM_SZ);
        plgPool.Stop  (); //destroy
        plgPool.Delete(); //dealloc
    }

   //=======================================================
   //Using default heap alloc
    for(int i=0; i<N_RUNS; i++)
    {
        plgPool.Create(&HeapAlloc, N_POOL_FRMS, FRM_SZ);
        plgPool.Stop  (); //destroy
        plgPool.Delete(); //dealloc
    }

    #if defined(MA2450)
    OsDrvTimerGetElapsedTicks(&ticks, &cycles);
    #endif
    unitTestAssert(1);//if it got here, we're good :)
    unitTestFinalReport();
    exit(0);
}
