#include <stdio.h>
#include <DrvTimer.h>
#include "DrvShaveL2Cache.h"
#include "DrvLeonL2C.h"
#include "swcLeonUtils.h"
#include "ipipe.h"
#include "IpipeServerApi.h"
#include "ipipeMsgQueue.h"
#include "appIspFromRx.h"

int main(void)
{
    DrvLL2CDisable(LL2C_OPERATION_INVALIDATE);
    DrvLL2CInitWriteThrough();

    gServerInfo.cbIcSetup       = appRxIspCbIcSetup;
    gServerInfo.cbIcTearDown    = appRxIspCbIcTearDown;
    gServerInfo.cbSourcesCommit = appRxIspSrcComit;

  #if 1
    setupIpipeServer();
    while (1) {
        appRxIspMain();
    }
  #else
    void getLeonLoad();
    getLeonLoad();  // Profiling
  #endif

  return 0;
}
