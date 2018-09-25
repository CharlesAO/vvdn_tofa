///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Frame Sync test (Sync receive 2 frames and propagate on output
///            a single message containing both frame refs).
///

#include <Flic.h>
#include <Pool.h>
#include <MemAllocator.h>
#include <VcsHooksApi.h>
#include <UnitTestApi.h>

#include <Pool.h>
#include <ImgFrame.h>
#include "myTypes.h"
#include "Source.h"
#include "Sync.h"
#include "Output.h"

PlgPool<CocoFrame> plgPoolCoco;
PlgPool<BoboFrame> plgPoolBobo;
PlgSrc <CocoFrame> plgSrcCoco;
PlgSrc <BoboFrame> plgSrcBobo;
PlgOut <MegaMsg  > plgOut;

PlgSync<CocoFrame, BoboFrame, MegaMsg> plgSync;

Pipeline         p;

//############################################
extern "C" void *POSIX_Init (void *)
{
    unitTestInit();
    RgnAlloc.Create(RgnBuff, DEF_POOL_SZ);

  //dummy pool, params don't really matter as this is not a real case yet
    plgPoolCoco.Create(&RgnAlloc, 4, 128);
    plgPoolBobo.Create(&RgnAlloc, 5,  64);
    plgSrcCoco .Create(VAL_COCO);
    plgSrcBobo .Create(VAL_BOBO);
    plgSync    .Create();
    plgOut     .Create();

    p.Add(&plgPoolCoco);
    p.Add(&plgPoolBobo);
    p.Add(&plgSrcCoco);
    p.Add(&plgSrcBobo);
    p.Add(&plgSync);
    p.Add(&plgOut );

    plgPoolCoco.out.Link(&plgSrcCoco.inO);
    plgPoolBobo.out.Link(&plgSrcBobo.inO);
    plgSrcCoco .out.Link(&plgSync.inA);
    plgSrcBobo .out.Link(&plgSync.inB);
    plgSync    .out.Link(&plgOut.in);

    p.Start();
    sleep(15);
    p.Stop();
    p.Wait();
    p.Delete();

    unitTestFinalReport();
    exit(0);
}
