///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     MvTensorDefines header
///

#ifndef __MV_TENSOR_DEFINES_H__
#define __MV_TENSOR_DEFINES_H__

// Includes
// ----------------------------------------------------------------------------
#include "mv_types.h"

// Defines and typedefs
// ----------------------------------------------------------------------------
#define SIZE_NO 5
#define STRIDE_NO 5
#define TYPE_NO 3
#define MAX_CHANNELS 256
#define MAX_OUTPUT_MAPS 200
#define TOTAL_SHV_LOCAL_BUFF_SIZE (35*1024)
#define SHAVE_MAX_NO 12
#define PADDING 16

enum convDataTypeEnum
{
    xhhx,
    hhhh,
    hhhx
};

enum convSizeEnum
{
    k3x3,
    k5x5,
    k7x7,
    k9x9,
    k11x11
};

enum convStrideEnum
{
    stride1,
    stride2,
    stride3,
    stride4,
    stride8
};

typedef struct
{
    u8 *input[MAX_CHANNELS];
    u8 *output[MAX_OUTPUT_MAPS];
    u8 *convTaps;
    u8 inputBPP;
    u8 outputBPP;
    u8 kernelBPP;
    u8 channelsNo;
    u8 outputMapsNo;
    u32 inputWidth;
    u32 inputHeight;
    u32 inputStride;
    u32 kernelSize;
    enum convSizeEnum convSize;
    enum convStrideEnum convStride;
    enum convDataTypeEnum convDataType;
} mvTensorInfo;

typedef int (*fp)(mvTensorInfo*, u32);

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------
void conv4d(mvTensorInfo *infoPtr, u32 firstShv, u32 lastShv);

#endif//__MV_TENSOR_DEFINES_H__
