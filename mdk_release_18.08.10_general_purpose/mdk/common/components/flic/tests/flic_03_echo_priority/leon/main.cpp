///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Input grouping (::TryReceive on inputs)
///

#include <Flic.h>
#include "PlgEcho.h"
#include <UnitTestApi.h>

PlgEchoSend  plgSendS;
PlgEchoSend  plgSendV;
PlgEchoRecv  plgRecv;
Pipeline     p;

//############################################
extern "C" void *POSIX_Init (void *)
{
    unitTestInit();
                                             //Roles:
    plgSendS.Create();  plgSendS.what = 333; //Still - src
    plgSendV.Create();  plgSendV.what = 555; //Video - src
    plgRecv .Create();                       //ISP

    p.Add(&plgSendV);
    p.Add(&plgSendS);
    p.Add(&plgRecv);

    plgSendV.out.Link(&plgRecv.inV);
    plgSendS.out.Link(&plgRecv.inS);

    p.Start();
    sleep(10);

    p.Stop();
    p.Wait();
    p.Delete();

    unitTestFinalReport();
    exit(0);
}
