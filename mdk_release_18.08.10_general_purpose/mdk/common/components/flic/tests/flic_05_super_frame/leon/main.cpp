///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Super-Frame example:
///            a unique super-frame is split into floating sub-parts
///            which are fed to other plugs, then waited for (the super-frame join).
///            The final generated super-frame is forwarded to consumer

#include <Flic.h>
#include <MemAllocator.h>
#include "PlgEcho.h"
#include "PlgSuper.h"
#include <UnitTestApi.h>

PlgPool<ImgFrame> plgPool;  //Super frames
PlgEchoSend       plgSend1; //src1
PlgEchoSend       plgSend2; //src2
PlgSuper          plgSuper; //isp
PlgEchoRecv       plgRecv;  //out
Pipeline          p;

//############################################
extern "C" void *POSIX_Init (void *)
{
    unitTestInit();

    plgSend1.Create();   plgSend1.seed = 0x30;
    plgSend2.Create();   plgSend2.seed = 0xA0;
    plgSuper.Create();
    plgPool .Create(&HeapAlloc, 5, 128);
    plgRecv .Create();

   //expected data
    plgSuper.pSeed[0] = plgSend1.seed;
    plgSuper.pSeed[1] = plgSend2.seed;

    p.Add(&plgSend1);
    p.Add(&plgSend2);
    p.Add(&plgSuper);
    p.Add(&plgPool );
    p.Add(&plgRecv );

    plgPool .out     .Link(&plgSuper.iSuper);
    plgSuper.oSuper  .Link(&plgRecv.in );
    plgSuper.oPart[0].Link(&plgSend1.in);
    plgSuper.oPart[1].Link(&plgSend2.in);
    plgSend1.out     .Link(&plgSuper.iPart[0]);
    plgSend2.out     .Link(&plgSuper.iPart[1]);

    p.Start();
    sleep(10);
    p.Stop();
    p.Wait();
    p.Delete();

    unitTestFinalReport();
    exit(0);
}
