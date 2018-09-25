///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main mvTensor file
///
// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <swcShaveLoader.h>
#include <DrvShaveL2Cache.h>
#include <DrvTimer.h>
#include <DrvLeonL2C.h>


#include "mvTensorApi.h"

extern u32 small4Dconv0_start;
extern u32 small4Dconv1_start;
extern u32 small4Dconv2_start;
extern u32 small4Dconv3_start;
extern u32 small4Dconv4_start;
extern u32 small4Dconv5_start;
extern u32 small4Dconv6_start;
extern u32 small4Dconv7_start;
extern u32 small4Dconv8_start;
extern u32 small4Dconv9_start;
extern u32 small4Dconv10_start;
extern u32 small4Dconv11_start;

fp startShave[SHAVE_MAX_NO] =
{
    (fp)&small4Dconv0_start,
    (fp)&small4Dconv1_start,
    (fp)&small4Dconv2_start,
    (fp)&small4Dconv3_start,
    (fp)&small4Dconv4_start,
    (fp)&small4Dconv5_start,
    (fp)&small4Dconv6_start,
    (fp)&small4Dconv7_start,
    (fp)&small4Dconv8_start,
    (fp)&small4Dconv9_start,
    (fp)&small4Dconv10_start,
    (fp)&small4Dconv11_start
};

void conv4d(mvTensorInfo *infoPtr, u32 firstShv, u32 lastShv)
{
    u32 shaveNo = lastShv - firstShv + 1;
    u32 mapsPerShave = infoPtr->outputMapsNo / shaveNo;
    u32 remainedMaps = infoPtr->outputMapsNo % shaveNo;

    u32 firstMap = 0;
    u32 lastMap  = 0;

    u32 startedShv = 0;
    for(u32 shvIndex = 0; shvIndex < shaveNo; shvIndex++)
    {
        lastMap += mapsPerShave;
        if(remainedMaps > 0)
        {
            lastMap++;
            remainedMaps--;
        }

        swcResetShave(firstShv + shvIndex);
        swcSetAbsoluteDefaultStack(firstShv + shvIndex);
        
        swcStartShaveCC(firstShv + shvIndex, (u32) startShave[shvIndex + firstShv],
                "iiiii", infoPtr, firstMap, lastMap, 0, infoPtr->inputHeight);

        firstMap = lastMap;
        startedShv++;
        if(lastMap > infoPtr->outputMapsNo)
            break;
    }

    // Wait for all shaves to finish
    for (u32 shvIndex = 0; shvIndex < startedShv; shvIndex++)
    {
        swcWaitShave(firstShv + shvIndex);
        DrvShaveL2CachePartitionFlush( (firstShv + shvIndex) % MAX_SHAVE_L2C_PARTITIONS); //wrap around
    }

}
