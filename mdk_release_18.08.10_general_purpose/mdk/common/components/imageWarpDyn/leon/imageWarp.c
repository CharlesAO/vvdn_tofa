///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Image Warp Function.
///
/// This is the implementation of a simple image warp.
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <mv_types.h>
#include <DrvShaveL2Cache.h>
#include <swcShaveLoader.h>
#include <swcFrameTypes.h>
#include <DrvCmxDma.h>

#include "imageWarpDefines.h"
#include "imageWarp.h"

#include "theDynContext.h"

extern DynamicContext_t MODULE_DATA(imageWarpDynlib);

unsigned int ready=0;

u32 IMGWARP_start(swcShaveUnit_t svu, meshStruct* mesh, tileList* tileNodes, frameBuffer *inputFb, frameBuffer *outputFb, unsigned short padding)
{

    swcShaveUnit_t svuList[1];
    svuList[0]=svu;
    int status;
    if(!ready){
      swcSetupDynShaveApps(&MODULE_DATA(imageWarpDynlib), svuList, 1);
      ready=1;
    }

    status=swcRunShaveAlgoCC(&MODULE_DATA(imageWarpDynlib), (int*) &svu, "iiiii", mesh, inputFb, outputFb, tileNodes, padding );
    if (status != MYR_DYN_INFR_SUCCESS)
        MYR_DYN_INFR_CHECK_CODE(status);
    swcWaitShave(svu);

    return 0;
}

void IMGWARP_cleanup(void)
{
    ready=0;
    swcCleanupDynShaveApps(&MODULE_DATA(imageWarpDynlib));
}

