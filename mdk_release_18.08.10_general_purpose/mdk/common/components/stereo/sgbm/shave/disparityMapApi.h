///
/// @file DisparityMap.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief Module used to compute dense disparity map; Disparity Map Interface
///


#ifndef DISP_MAP_H
#define DISP_MAP_H


#include <disparityMapApiDefines.h>

/******************************************************************************

 @brief     : Function that runs disparity map per patches

 @param     : algoCfg               : pointer to AlgorithmConfig
 @param     : dispCfg               : pointer to TileConfig
 @param     : patchWidthL1          : image width

 @return    : void

******************************************************************************/
extern "C" void sgbm(u8* combinedCostCube,
        stereoUserConfig_t* userCfg,
        stereoTileConfig_t* tileCfg,
        unsigned short* dispMedianSubpixel,
        u8* dispMedianInteger,
        u32 flag);

#endif // DISP_MAP_H
