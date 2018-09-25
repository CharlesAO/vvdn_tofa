///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Multiple pipes test
///

#include <Flic.h>
#include <UnitTestApi.h>
#include "PlgEcho.h"

#define N_PIPES 3

PlgEchoSend  plgSend[N_PIPES];
PlgEchoRecv  plgRecv[N_PIPES];
Pipeline     p      [N_PIPES];

//############################################
extern "C" void *POSIX_Init (void *)
{
    uint32_t i;
    unitTestInit();

    for(i=0; i<N_PIPES; i++){
      plgSend[i].Create();
      plgRecv[i].Create();
      p[i].Add(&plgSend[i]);
      p[i].Add(&plgRecv[i]);
      plgSend[i].out.Link(&plgRecv[i].in);
    }

    for(i=0; i<N_PIPES; i++)
      p[i].Start();

    sleep(4);

    //delayed stops
    for(i=0; i<N_PIPES; i++){
      p[i].Stop();
      rtems_task_wake_after(1500);
    }

    for(i=0; i<N_PIPES; i++){
      p[i].Wait();
      p[i].Delete();
    }

    unitTestFinalReport();
    exit(0);
}
