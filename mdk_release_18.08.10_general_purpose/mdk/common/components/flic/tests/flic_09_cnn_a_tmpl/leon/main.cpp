///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Dummy sequential CNN Age + Gender pipe
///

#include <Flic.h>

#include <MemAllocator.h>
#include <VcsHooksApi.h>
#include <UnitTestApi.h>

#include "test.h"
#include "Pool.h"
#include "Source.h"
#include "CnnFrame.h"
#include "DetAge.h"
#include "DetGen.h"
#include "Output.h"

PlgPool<CnnFrame> plgPool;
PlgSrc <CnnFrame> plgSrc;
PlgAge <CnnFrame> plgAge;
PlgGen <CnnFrame> plgGen;
PlgOut <CnnFrame> plgOut;

Pipeline  p;

//############################################
extern "C" void *POSIX_Init (void *)
{
    unitTestInit();
    RgnAlloc.Create(RgnBuff, DEF_POOL_SZ);

  //dummy pool, params don't really matter as this is not a real case yet
    plgPool.Create(&RgnAlloc, 6, 22*22);
    plgSrc .Create();
    plgAge .Create(/*ageTrainingData*/);
    plgGen .Create(/*genderTrainingData*/);
    plgOut .Create();

  //Say the source provides 224x224 regions
    plgSrc.roiW = plgSrc.roiH = 22;

    p.Add(&plgPool);
    p.Add(&plgSrc );
    p.Add(&plgAge );
    p.Add(&plgGen );
    p.Add(&plgOut );

    plgPool.out.Link(&plgSrc.inO);
    plgSrc .out.Link(&plgAge.in);
    plgAge .out.Link(&plgGen.in);
    plgGen .out.Link(&plgOut.in);

    p.Start();
    sleep(10);
    p.Stop();
    p.Wait();
    p.Delete();

    unitTestFinalReport();
    exit(0);
}
