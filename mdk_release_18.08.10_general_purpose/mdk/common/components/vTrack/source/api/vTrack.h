///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
/// For License Warranty see: common/license.txt
///
/// @brief common fundamental structures for vTrack application
///

#ifndef __VTRACK_H__
#define __VTRACK_H__

#include <stdint.h>
#include "vTrackOutput.h"

#define SF_LINE_MAX 508
#define END_OF_FEATURES (9999999.99)

struct frameElements;
typedef struct frameElements frameBuffer;

// Pyramid of frame buffers
typedef struct tvPyramidBuffer
{
    uint32_t numPyramidLevels; //   Number of pyramid levels
    frameBuffer *pyrLevel; //   Pointer to array of frame buffers
}tvPyramidBuffer;

typedef enum cornerAlgoType {
    CORNER_OFF,
    HARRIS,
    SHI_TOMASI
} cornerAlgoType_t;

typedef struct cornerConfig {
    cornerAlgoType_t alg;
    uint8_t useSobel;
    bool insertAndSort;
} cornerConfig_t;

typedef enum pyramidAlgoType {
    PYRAMID_OFF,
    PYRAMID_ON
} pyramidAlgoType_t;

typedef enum ofAlgoType {
    OF,
    MEST
} ofAlgoType_t;
#endif
