///
/// @file mixMedian
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     mix median - Mix two 8-bit planes according to a third 8-bit reference plane
///

#ifndef __MIXMEDIAN_MVCV_H__
#define __MIXMEDIAN_MVCV_H__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// mix median - Mix two 8-bit planes according to a third 8-bit reference plane
/// @param[out]        - first unsigned int contain number of valid points in the line, after score for every point
/// @param[in] in0       - array of pointers to input line of the image, 3 planes
/// @param[in] in1       - array of pointers to input line of the image, 3 planes
/// @param[in] ref       - array of pointers to input line of the image, 1 planes, reference
/// @param[in] offset    - offset
/// @param[in] slope     - slope
/// @param[in] width     - width of the input lines
/// @return    Nothing

MVCV_FUNC(void, mvispMixMedian, u8 *out[3], u8 *in0[3], u8 *in1[3], u8 *ref, float offset, float slope, unsigned int width)
//!@}

#endif //__MIXMEDIAN_MVCV_H__
