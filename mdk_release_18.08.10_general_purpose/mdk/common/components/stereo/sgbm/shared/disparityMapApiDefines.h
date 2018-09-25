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
#include <Defines.h>

// MatImage structure that simulates the behaviour of OpenCV Mat class
typedef struct
{
    u8*     data;
    u32     w;
    u32     h;
    u32     step;
} stereoMatImage_t;


// TileConfig structure holds the tile configuration (input and output addresses)
typedef struct
{
    frameBuffer     leftImage;          /* input: tile from left image for specific level */
    frameBuffer     rightImage;         /* input: tile from right image for specific level */
    frameBuffer     disparityMap;       /* output: tile disparity map for specific level */
    frameBuffer     confidenceMap;      /* output: confidence map */

    u32             censusKernelSize;   /* census kernel size; default value = CENSUS_KERNEL_SIZE */
    u32             disparities;        /* number of disparities at this level */

} stereoTileConfig_t;

// SGBMConfig structure holds the SGBM parameters and cost addresses
typedef struct
{
    u8*             censusCost;         /* disparity cost */
    u8*             aggregatedCost;     /* aggregated cost */
    u32*            penaltyTableP1;     /* penalty for cost aggregation when the disparity changes a little bit */
    u32*            penaltyTableP2;     /* penalty for cost aggregation for larger disparity changes */
    u32             disparities;        /* number of disparities to compute */
    u32             width;              /* width of the tile */
} stereoSGBMConfig_t;

// AlgorithmConfig structure holds the algorithm configuration with respect to SHAVEs used, pyramid levels, etc.
typedef struct
{
	frameSpec* frame;
    u32 maxDisparities;      /* maximum disparity; default value = MAX_DISPARITIES_LEVEL1 */
    u32 confidenceThreshold;    /* confidence threshold; default value = DEFAULT_CONFIDENCE_VAL */

    u32 list_of_shaves_thread_1[MAX_SHAVES];   /* SHAVEs used for stereo thread */

    u8* aggCostPaths[2][SGBM_SHAVES_USED];
} stereoUserConfig_t;

#endif // DISPARITY_MAP_CONFIG_H
