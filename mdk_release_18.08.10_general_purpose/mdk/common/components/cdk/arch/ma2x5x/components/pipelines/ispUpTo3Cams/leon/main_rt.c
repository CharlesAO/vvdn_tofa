
/**************************************************************************************************

 @File         : main_rt.c
 @Author       : MT
 @Brief        : Contains Lrt code starting point
 Date          : 01 - March - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : � Movidius Srl 2014, � Movidius Ltd 2015
 Description   :


 **************************************************************************************************/


/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
 **************************************************************************************************/
#include <stdio.h>
#include "DrvShaveL2Cache.h"
#include "DrvLeonL2C.h"
#include "swcLeonUtils.h"
#include "ipipe.h"
#include "IpipeServerApi.h"
#include "ipipeMsgQueue.h"
#include "app3CamIsp.h"


/**************************************************************************************************
 ~~~  Specific #defines
 **************************************************************************************************/


/**************************************************************************************************
 ~~~ Local File function declarations
 **************************************************************************************************/


/**************************************************************************************************
 ~~~  Exported Functions
 **************************************************************************************************/
int main(void)
{
    DrvLL2CDisable(LL2C_OPERATION_INVALIDATE);
    DrvLL2CInitWriteThrough();

    gServerInfo.cbIcSetup       = app3CamIspCbIcSetup;
    gServerInfo.cbIcTearDown    = app3CamIspCbIcTearDown;
    gServerInfo.cbSourcesCommit = app3CamIspSrcComit;

    setupIpipeServer();

    while (1) {
        app3CamIspMain();
    }
    return 0;
}

/**************************************************************************************************
 ~~~ Local Functions Implementation
 **************************************************************************************************/
