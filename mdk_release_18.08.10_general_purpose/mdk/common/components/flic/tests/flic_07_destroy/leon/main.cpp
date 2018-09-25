///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     App Start/Stop cycle
///

#include <Flic.h>
#include <UnitTestApi.h>
#include "PlgEcho.h"

PlgEchoSend  plgSend;
PlgEchoRecv  plgRecv;
Pipeline     p;

#define N_RUNS 10

//############################################
extern "C" void *POSIX_Init (void *)
{
    int i = 0;
    unitTestInit();

    while(i++ < N_RUNS)
    {
       printf("===== RUN (%d) =====\n", i);

       plgSend.Create();
       plgRecv.Create();

       p.Add(&plgSend);
       p.Add(&plgRecv);
       plgSend.out.Link(&plgRecv.in);

       p.Start();
       sleep(1);
       p.Stop();

       p.Wait();
       p.Delete();
    }

    unitTestFinalReport();
    exit(0);
}
