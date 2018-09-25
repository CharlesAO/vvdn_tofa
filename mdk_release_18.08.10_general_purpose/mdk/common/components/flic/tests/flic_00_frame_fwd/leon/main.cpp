///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Frame propagation from Source to End-consumer
///

#include <Flic.h>
#include <MemAllocator.h>
#include <UnitTestApi.h>
#include "PlgRecv.h"
#include "PlgSrc.h"
#include "PlgBuf.h"
#include "cfg.h"

#define N_FRAMES 3
#define FRAME_SZ 128

PlgSrc            plgSrc;
PlgPool<ImgFrame> plgPool;
PlgBuf            plgBuff[4];
PlgRecv           plgRecv;
Pipeline          p;

//##################################################
extern "C" void *POSIX_Init (void *)
{
    ImgFramePtr fMsg;
    unitTestInit();

 //Create structs
    RgnAlloc  .Create(RgnBuff, DEF_POOL_SZ);
    plgSrc    .Create();
    plgPool   .Create(&RgnAlloc, N_FRAMES, FRAME_SZ);
    plgBuff[0].Create();
    plgBuff[1].Create();
    plgBuff[2].Create();
    plgBuff[3].Create();
    plgRecv   .Create();

  //Add
    p.Add(&plgSrc);
    p.Add(&plgPool);
    p.Add(&plgRecv);
    p.Add(&plgBuff[0]);
    p.Add(&plgBuff[1]);
    p.Add(&plgBuff[2]);
    p.Add(&plgBuff[3]);

  //Link
       plgPool.out.Link(&plgSrc    .inO);
        plgSrc.out.Link(&plgBuff[0].in);
    plgBuff[0].out.Link(&plgBuff[1].in);
    plgBuff[1].out.Link(&plgBuff[2].in);
    plgBuff[2].out.Link(&plgBuff[3].in);
    plgBuff[3].out.Link(&plgRecv   .in);

  //Test: src to fire all frames
    p.Start();
    p.Wait();
    p.Delete();

    unitTestAssert(plgRecv.no == T_FRM);
    unitTestFinalReport();
    exit(0);
}
