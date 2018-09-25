///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Templated plugin example
///

#include <Flic.h>
#include "PlgEcho.h"

class AlaMsg {
  public:
    uint32_t data;
    virtual const char * Info(){
        static const char *nfo = "AlaMsg";
        return nfo;
    }
};

class BalaMsg : public AlaMsg {
  public:
    uint32_t coco;
    const char * Info(){
        static const char *nfo = "BalaMsg";
        return nfo;
    }
};

PlgEchoSend<AlaMsg> plgSendA;
PlgEchoRecv<AlaMsg> plgRecvA;
Pipeline                  pA;

PlgEchoSend<BalaMsg> plgSendB;
PlgEchoRecv<BalaMsg> plgRecvB;
Pipeline                   pB;

//############################################
extern "C" void *POSIX_Init (void *)
{
    unitTestInit();

  //=================================
    printf("\n using AlaMsg ...\n");
    plgSendA.Create();
    plgRecvA.Create();
    pA.Add(&plgSendA);
    pA.Add(&plgRecvA);
    plgSendA.out.Link(&plgRecvA.in);
    pA.Start();
    pA.Wait();
    pA.Stop();//clears res
    pA.Delete();

  //=================================
    printf("\n using BalaMsg ...\n");
    plgSendB.Create();
    plgRecvB.Create();
    pB.Add(&plgSendB);
    pB.Add(&plgRecvB);
    plgSendB.out.Link(&plgRecvB.in);
    pB.Start();
    pB.Wait();
    pB.Stop();//clears res
    pB.Delete();

    unitTestFinalReport();
    exit(0);
}
