///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     User custom logging
///

#include <Flic.h>
#include <VcsHooksApi.h>
#include <UnitTestApi.h>
#include "PlgEcho.h"
#include "MyLogA.h"
#include "MyLogB.h"
#include "Data.h"

PlgSend  plgSend;
PlgRecv  plgRecv;
Pipeline p;

MyLogA logA;
MyLogB logB;

//##################################################
void test(IBuffLog<MyMsg> *log)
{
    plgSend.Create();
    plgRecv.Create();

    p.name = Flic::Name(".myPipe");
    p.log  = log;
    p.Add(&plgSend, ".Send");
    p.Add(&plgRecv, ".Recv");

    plgSend.out.Link(&plgRecv.in);

    plgRecv.in.mq.SetLog(log);

    log->Print("[THE START]\n");
    p.Start();
    p.Wait();
    log->Print("[THE END]\n");
    p.Delete();
    printf("\n");
}

//##################################################
extern "C" void *POSIX_Init (void *)
{
    unitTestInit();

    test(&logA);
    test(&logB);

    unitTestFinalReport();
    exit(0);
}
