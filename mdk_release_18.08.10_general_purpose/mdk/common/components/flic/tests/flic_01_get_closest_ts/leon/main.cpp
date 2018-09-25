///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     "Get closest TS" ::Receive example
///

#include <Flic.h>
#include "PlgEcho.h"
#include <UnitTestApi.h>

PlgEchoSend  plgSend;
PlgEchoRecv  plgRecv;
Pipeline     p;

//############################################
extern "C" void *POSIX_Init (void *)
{
    unitTestInit();

    plgSend.Create();
    plgRecv.Create();

    p.Add(&plgSend);
    p.Add(&plgRecv);

    plgSend.out.Link(&plgRecv.in);

    p.Start();
    p.Wait();

    unitTestFinalReport();
    exit(0);
}
