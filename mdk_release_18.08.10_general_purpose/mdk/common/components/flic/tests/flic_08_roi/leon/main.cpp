///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Multiple ROI per frame detect & forward e.g.
///            (see dumped log.txt)
///

#include <Flic.h>
#include <MemAllocator.h>
#include <VcsHooksApi.h>
#include <UnitTestApi.h>
#include "Plugs.h"

PlgPool<RoiFrame> plgPool;
PlgIsp            plgIsp;
PlgFaceDet        plgDet;
PlgOut            plgOut;
Pipeline          p;

//uint8_t  logBuf[8*1024] SECTION(".shv0.S.init") ALIGNED(8);
//LogDef   log(logBuf, sizeof(logBuf));

//############################################
extern "C" void *POSIX_Init (void *)
{
    unitTestInit();
    RgnAlloc.Create(RgnBuff, DEF_POOL_SZ);

  //dummy pool, params don't really matter as this is not a real case yet
    plgPool.Create(&RgnAlloc, 4, 32*24);
    plgIsp .Create();
    plgDet .Create();
    plgOut .Create();

    p.name = Flic::Name(".myPipe");

    p.Add(&plgPool,".pool");
    p.Add(&plgIsp, ".isp" );
    p.Add(&plgDet, ".face");
    p.Add(&plgOut, ".out" );

    plgPool.out.Link(&plgIsp.inO);
    plgIsp .out.Link(&plgDet.in);
    plgDet .out.Link(&plgOut.in);

   //Default logging
  //p.SetLog(&log);

    p.Start();
    sleep(2);
    p.Stop();
    p.Wait();
  //saveMemoryToFile((u32)log.base, (u32)log.pos, "log.txt");

    unitTestFinalReport();
    exit(0);
}
