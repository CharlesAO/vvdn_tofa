///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief	 Applies a tone mapping function to the Luma channel.  
///          The mapping function is based on both the original Luma value, 
///          plus a second local brightness input image 
///          Local Tone Mapping increases contrast locally, applying a contrast 
///          curve that is adapted to the local image intensity
///

#ifndef __LOCALTM_H__
#define __LOCALTM_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// Local Tone Mappibg  - Tone mapping is a technique used in image processing and computer graphics to map one set of colors to another in order to approximate the appearance of high dynamic range images in a medium that has a more limited dynamic range.
/// @param[in] luma_in     - array of pointer to input line in fp16
/// @param[in] bg8         - array of pointers to input lines in U8
/// @param[out] output     - array of pointer  to output line
/// @param[in] width       - width of input line
/// @param[in] run_no      - run number used in sipp for coefficients selection

MVCV_FUNC(void, mvispLocalTM, half** luma_in, u8** bg8, half** output, half *curves, u32 width, u32 run_no)
//!@}

#endif //__LOCALTM_H__