///
/// @file depth.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief Header file for for computing disparity map on image tiles module
///
#ifndef _START_DISPARITY_MAP_H_
#define _START_DISPARITY_MAP_H_

#include <Defines.h>
#include <disparityMapApiDefines.h>

/******************************************************************************
 @brief     : Run disparity map SGBM algorithm

 @param     : algoConfig            : pointer to AlgorithmConfig
 @param     : leftImage             : pointer to TileConfig
 @param     : rightImage            : pointer to leftImage
 @param     : downsampledLeftImage  : pointer to rightImage
 @param     : downsampledRightImage : pointer to depthMapMedian
 @param     : depthMapMedian        : line width

 @return    : void
******************************************************************************/
extern "C" void runDisparityMap(stereoUserConfig_t* userCfg, int* dataL2Cpartitions,
                    u8* leftImage, u8* rightImage,
                    u8* downsampledLeftImage, u8* downsampledRightImage,
                    u8* combinedCostCube,
                    outputType* predictedDepthMapLR,
                    outputType* depthMapLR,
                    u8* confidence,
                    u8* disparityMedianIntegerBuffArea, outputType* disparityMedianSubpixelBuffArea,
                    u32 flag, u32 threadNo);

#endif // _START_DISPARITY_MAP_H_
