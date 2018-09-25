///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

#ifndef __OPTICAL_FLOW_TYPES_H__
#define __OPTICAL_FLOW_TYPES_H__

#include "mv_types.h"
#include "global_constants.h"

struct tvPyramidBuffer;
typedef struct tvPyramidBuffer tvPyramidBuffer;

// Used for patch/region sizing
    typedef struct {
        u32 width;
        u32 height;
    } tvSize;

    typedef struct {
        // Maximum number of iterations to estimate the position of a point at one level
        u32 max_iter;
        // Minimum threshold a point needs to move between iterations
        fp32 epsilon;
    } tvTermCriteria;

    typedef enum {
        OF_OPENCV,
        OF_MOVI,
		OF_MEST
    }OfAlgorithmType;

    typedef struct
    {
        tvTermCriteria termCriteria;
        tvSize winSize;
        u32 pyrLevels;
        OfAlgorithmType algoType;
        uint32_t maxNumFeatures;
        u8 subpixelDetection; //Enable/disable subpixel detection (0: disable, 1: enable)
        //currently only supported by PC version
    } tvOpticalFlowCfg;
#endif


