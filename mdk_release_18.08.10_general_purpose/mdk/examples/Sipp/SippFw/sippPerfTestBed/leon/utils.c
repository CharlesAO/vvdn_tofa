///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Utility functions for sipp performance testbed application

#include <sipp.h>
#include <sippTestCommon.h>
#include <filters/conv5x5/conv5x5.h>

#include "perfTestbed.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

#ifndef SIPP_PC
#include <VcsHooksApi.h>
#endif

extern u16 convMatrix[5 * 5];
extern u16 conv5x50Cfg_cMat[][5 * 5];

static char * gFileDumpBuffer;
static char * gFileDumpBufferStart;
static u32    gu32Size = 0;

////////////////////////////////////////////////////////////////////////////
// Generic debug functions

void perfTestbedSetFileBuffer (u8 * pFileBuffer)
{
    gFileDumpBuffer = gFileDumpBufferStart = (char *)pFileBuffer;
}

#ifdef SIPP_VCS
void perfTestbedAddToFile (u32 * pu32NbChars, char * pLineBuffer)
{
    assert(*pu32NbChars < MAX_LINE_LENGTH);
    assert((gu32Size + *pu32NbChars) < FILE_DUMP_BUFFER_MAX_SIZE);
    memmove(gFileDumpBuffer, pLineBuffer, *pu32NbChars);
    gFileDumpBuffer += *pu32NbChars;
    gu32Size += *pu32NbChars;
}

void perfTestbedDumpFile (char * fileDumpName)
{
    saveMemoryToFile((u32)gFileDumpBufferStart, gu32Size, fileDumpName);
}
#endif

////////////////////////////////////////////////////////////////////////////
// Specific APP functions

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


