///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Image warp component API
///

#ifndef _IMAGE_WARP_H_
#define _IMAGE_WARP_H_


// 1: Includes
// ----------------------------------------------------------------------------
#include <mv_types.h>
#include "swcFrameTypes.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------


// 3: Static Local Data
// ----------------------------------------------------------------------------

// 4:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------
/// @param[in] mx                 - input mesh for x - coordinates related to input image coordinates
/// @param[in] my                 - input mesh for y- coordinates related to input image coordinates
/// @param[in] img                - input image address
/// @param[in] shaves_used        - number of shaves used to compute the entire algo
/// @param[in] out_img            - Input tile of data, (u16 format) it has variable width/height size
/// @param[in] out_width          - Input tile of data, (u16 format) it has variable width/height size
/// @param[out] tiles             - structure of information for each input tile
/// @param[out] tile_no           - number of tiles resulted from the computation
///
///
///
///
#ifdef __cplusplus
extern "C"
#endif
void Entry(meshStruct* mesh,
           frameBuffer *inputFb,
           frameBuffer *outputFb,
           tileList* tileList,
           unsigned short paddingvalue);

#endif

