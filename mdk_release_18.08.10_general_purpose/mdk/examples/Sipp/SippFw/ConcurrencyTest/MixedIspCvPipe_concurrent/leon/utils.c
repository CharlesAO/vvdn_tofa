///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application to test running SIPP pipelines concurrently
///            This test uses pipelines which have no conflicting SHAVE or
///            HW filter resource and therefore should be capable of running
///            concurrently

#include <sipp.h>
#include <sippTestCommon.h>
#include <filters/conv5x5/conv5x5.h>

extern u16 convMatrix[5 * 5];
extern u16 conv5x50Cfg_cMat[][5 * 5];

void initConvMat (SippFilter * conv5x5, u32 pipeId, u32 ks)
{
    u32 i;
    Conv5x5Param *convCfg = (Conv5x5Param*)conv5x5->params;

    convCfg->cMat = conv5x50Cfg_cMat[pipeId];

    for (i = 0; i < ks * ks; i++)
    {
        convCfg->cMat[i]   = convMatrix[i];
    }
}

