///
/// @file computeCost.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief Apply cost aggregation over a census cost (inspired by Semi-Global block Matching); SGBM Interface
///


#ifndef COMPUTE_COST_H
#define COMPUTE_COST_H

#include <mv_types.h>
#include <disparityMapApiDefines.h>

//---------------------------------------------------------------------------//
//-------------------------- Exported Functions -----------------------------//
//---------------------------------------------------------------------------//

/******************************************************************************
 @brief     : PC function for computing cost aggregation by adding the 4 paths costs

 @param     : path0             : cost for path0
 @param     : path1             : cost for path1
 @param     : path2             : cost for path2
 @param     : path3             : cost for path3
 @param     : out               : aggregated cost
 @param     : width             : line size
 @param     : disparities       : number of disparities
 @return    : void
******************************************************************************/
extern "C" void computeCost(stereoUserConfig_t* algoCfg,
                        stereoMatImage_t *leftImage,
                        stereoMatImage_t *rightImage,
                        u8* outputCost);

#endif // COMPUTE_COST_H
