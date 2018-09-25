/// =====================================================================================
///
///        @file:      PipeCommon.h
///        @brief:
///        @author:    csoka, attila.csok@movidius.com
///        @copyright: All code copyright Movidius Ltd 2017, all rights reserved.
///                  For License Warranty see: common/license.txt
/// =====================================================================================
///

#ifndef MONOPIPECOMMON_H__
#define MONOPIPECOMMON_H__

/// System Includes
/// -------------------------------------------------------------------------------------
#include "stdint.h"
#include <Flic.h>
#include <MemAllocator.h>
#include <sipp_isp_types.h>
#include "GrupsTypes.h"
#include "PlgStereoProc.h"

/// Application Includes
/// -------------------------------------------------------------------------------------

/// Source Specific #defines and types (typedef,enum,struct)
/// -------------------------------------------------------------------------------------
#define N_MONO_CAMS 2

/// Global Data (Only if absolutely necessary)
/// -------------------------------------------------------------------------------------

/// Static Local Data
/// -------------------------------------------------------------------------------------

/// Static Function Prototypes
/// -------------------------------------------------------------------------------------

/// Functions Implementation
/// -------------------------------------------------------------------------------------
typedef struct{
    stereo_resources res;
    stereo_cfg cfg;
    uint32_t maxImgSz;
    uint32_t nrFrms;
}MonoPipeStereoConfig;


typedef struct {
    uint32_t        camId[N_MONO_CAMS];
    SpSrcCfg        spSrcCfg[N_MONO_CAMS];
    SpIspCfg        spIspVdoCfg[N_MONO_CAMS];

    const char*     mesageQueueEventName[N_MONO_CAMS];
    IAllocator      *rgnAlloc;
    uint32_t        ipcThreadPriority[N_MONO_CAMS];

}MonoPipeConfig;

#endif /// MONOPIPECOMMON_H__
