///
/// @file DisparityMap.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief Module used to compute dense disparity map; Disparity Map Interface
///


#ifndef DISP_MAP_H
#define DISP_MAP_H


#include <DisparityMapApiDefines.h>

//---------------------------------------------------------------------------//
//-------------------------- Exported Functions -----------------------------//
//---------------------------------------------------------------------------//


/******************************************************************************

 @brief     : Init Function used to allocate all internal buffers

 @param     : algoCfg               : pointer to AlgorithmConfig
 @param     : width					: image width

 @return    : void

******************************************************************************/
extern "C" void disparityMapInit(AlgorithmConfig* algoCfg,
                                 u32 width);



/******************************************************************************

 @brief     : Compute dense disparity map

 @param     : algoCfg               : pointer to AlgorithmConfig
 @param     : leftImage             : left image pointer
 @param     : rightImage            : right image pointer
 @param     : disparity             : disparity map
 @param     : width                 : image width
 @param     : height                : image height

 @return    : void

******************************************************************************/
extern "C" void disparityMapBase(AlgorithmConfig* algoCfg,
                                 u8 *leftImage,
                                 u8 *rightImage,
                                 u8 *disparity,
                                 u32 width,
                                 u32 height);


/******************************************************************************

 @brief     : Function that runs disparity map per patches

 @param     : algoCfg               : pointer to AlgorithmConfig
 @param     : dispCfg               : pointer to DisparityConfig
 @param     : width                 : image width
 @param     : height                : image height

 @return    : void

******************************************************************************/
extern "C" void disparityMapRunPatches(AlgorithmConfig* algoCfg,
                                        DisparityConfig* dispCfg,
                                        u32 width,
                                        u32 height);

void medianFiltering(AlgorithmConfig* algoCfg,
					u8 *disparity,
					u8* disparityMedian,
					u32 width,
					u32 height);

#endif // DISP_MAP_H
