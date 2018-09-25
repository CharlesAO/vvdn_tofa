///
/// @file DisparityMapConfig.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief DisparityMapConfig structures
///

#ifndef DISPARITY_MAP_CONFIG_H
#define DISPARITY_MAP_CONFIG_H

#include <mv_types.h>
#include <swcFrameTypes.h>

// Algorithm parameters
#define MAX_DISPARITIES     64
#define CENSUS_KERNEL_SIZE  5
#define MEDIAN_KERNEL_SIZE  5
#define LINE_PADDING        2
#define MEDIAN_PADDING      2

#ifdef __MOVICOMPILE__
const u32 paddingForDmaTransfer = MEDIAN_PADDING; // needed in case of using HW median filter
#endif

// DisparityConfig structure holds the needed buffers for input and output images
typedef struct
{
    frameBuffer		leftImage;          /* input left image */
    frameBuffer		rightImage;         /* input right image */
    u8*				disparityMap;       /* output disparity map */

} DisparityConfig;

// AlgorithmConfig structure holds the configuration
typedef struct
{
    u32 cfgCensusKernelSize;    /* census kernel size; default value = CENSUS_KERNEL_SIZE */
    u32 cfgMedianKernelSize;    /* median kernel size; default value = MEDIAN_KERNEL_SIZE */
    u32 cfgLinePadding;         /* size of line padding  (half of census kernel); default value = LINE_PADDING */
    u32 cfgMedianPadding;       /* size of line padding for median filtering (half of median kernel); default value = MEDIAN_PADDING */
    u32 cfgMaxDisparities;      /* maximum disparity; default value = MAX_DISPARITIES */

} AlgorithmConfig;

#endif // DISPARITY_MAP_CONFIG_H
