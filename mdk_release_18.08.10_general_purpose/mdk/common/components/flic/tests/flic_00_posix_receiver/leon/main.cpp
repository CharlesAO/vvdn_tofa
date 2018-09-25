///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Echo test ("infinite" loop) using POSIX mq receiver
///

#include <Flic.h>
#include <UnitTestApi.h>
#include "PlgEcho.h"

PlgEchoSend  plgSend;
PlgEchoRecv  plgRecv;
Pipeline     p;

void MqRecTest1();
void MqRecTest2();
void MqRecTest3();

// TBD:
// 1) review return codes (put on same page with SReceiver)
// 2) check what can be factored out (thin templating)
// 3) standardize !!!

//############################################
extern "C" void *POSIX_Init (void *)
{
    unitTestInit();

    MqRecTest1();
    MqRecTest2();
    MqRecTest3();

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
