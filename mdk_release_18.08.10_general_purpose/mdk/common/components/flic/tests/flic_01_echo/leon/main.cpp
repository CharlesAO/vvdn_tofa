///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Echo test ("infinite" loop)
///

#include <Flic.h>
#include <UnitTestApi.h>
#include "PlgEcho.h"

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
    sleep(5);
    p.Stop();

    p.Wait();
    p.Delete();

    unitTestFinalReport();
    exit(0);
}
