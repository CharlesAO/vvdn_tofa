///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     FLIC stress test: 128 Senders push to a single Receiver
///            (OvrOldest mode). Checking that received cnt <= sent cnt
///

#include <Flic.h>
#include <rtems/score/heap.h>
#include <rtems/libcsupport.h>
#include <UnitTestApi.h>
#include <DrvCpr.h>
#include "PlgEcho.h"

uint32_t histOut[N_HIST] SECTION(".cmx_direct.bss");

PlgEchoSend  plgSend[N_HIST];
PlgEchoRecv  plgRecv;
Pipeline     p;

//############################################
extern "C" void *POSIX_Init (void *)
{
    int i, totSent;
    unitTestInit();

    DrvCprInit();

    memset(histOut, 0, sizeof(histOut));

    plgRecv.Create(NUM_RECS, histOut);
    for(i=0; i<N_HIST; i++)
      plgSend[i].Create(i);

    p.Add(&plgRecv);
    for(i=0; i<N_HIST; i++)
       p.Add(&plgSend[i]);

    for(i=0; i<N_HIST; i++)
       plgSend[i].out.Link(&plgRecv.in);

    printf("Started ...\n");
    p.Start();

    plgRecv.Join(); //waiting for receiver to get enough messages
    p.Stop();
    p.Wait();

   //Send success rate:
    totSent = 0;
    for(i=0; i<N_HIST; i++)
      totSent += plgSend[i].sent;
    printf("Drop rate = %.2f%%\n\n", ((float)totSent-NUM_RECS)/totSent*100.0f);

   //Received histogram should be <= input histogram (per index)
    for(i=0; i<N_HIST; i++){
      printf("%3d sent = %5ld, recv=%5ld\n", i, plgSend[i].sent, histOut[i]);
      unitTestAssert(plgSend[i].sent >= histOut[i]);
    }

    unitTestFinalReport();
    exit(0);
}
