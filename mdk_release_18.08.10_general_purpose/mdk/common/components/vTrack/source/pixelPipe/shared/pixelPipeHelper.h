///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

#ifndef __PIXEL_PIPE_HELPER_H__
#define __PIXEL_PIPE_HELPER_H__

#include <stdbool.h>
#include "mv_types.h"
#include "global_constants.h"
#include "vTrack.h"

#define NO_CACHE(x) (void*)((u32)x | 0x08000000)

#if defined(MA2150) || defined(MA2450)
#define HARRIS_OUTPUT_TYPE fp32
#else
#define HARRIS_OUTPUT_TYPE fp16
#endif

#define SCALING_FOR_HARRIS_FP16 24
#define HARRIS_KERNEL_SIZE 5
#define HARRIS_PADDING 0


#define GAUSS_V_SPAN 5

#define N_HARRIS_LINE_BUFFS (HARRIS_KERNEL_SIZE + 1)
#define N_CORNER_LINE_BUFFS 4

#define GAUSS_TASK_TYPE 0
#define CORNER_TASK_TYPE 1


#define ROTATE_BUFFER(buffer, size) \
{ \
    typeof(*buffer) tmp = buffer[0]; \
    u32 i; \
    for (i = 0; i < size - 1; i++) \
    { \
        buffer[i] = buffer[i + 1]; \
    } \
    buffer[size - 1] = tmp; \
}

struct fifoCommTask_t;
typedef struct fifoCommTask_t fifoCommTask_t;

struct tvFeatureCell;

inline fp32 upscaleFloat(const fp32& val, uint32_t subtrahend) {
    u32* val_u32_ptr = (u32*) &val;
    u32 expVal = (*val_u32_ptr>>23)&0xFF;
    expVal = expVal + subtrahend;
    u32 res_u32 = ((*val_u32_ptr&0x807FFFFF)|(expVal << 23));
    u32* res_u32_ptr = &res_u32;
    fp32* res_ptr = (fp32*) res_u32_ptr;
    return *res_ptr;
}

inline fp32 downscaleFloat(const fp32& val, uint32_t subtrahend) {
    u32* val_u32_ptr = (u32*) &val;
    u32 expVal = (*val_u32_ptr>>23)&0xFF;
    expVal = expVal - subtrahend;
    u32 res_u32 = ((*val_u32_ptr&0x807FFFFF)|(expVal << 23));
    u32* res_u32_ptr = &res_u32;
    fp32* res_ptr = (fp32*) res_u32_ptr;
    return *res_ptr;
}

typedef struct gaussParameters_t
{
    u8* line_buff;
    u8* next_line_buff;
    u32 line_cnt;
    u32 pyr_width;
    u32 pyr_height;
    tvPyramidBuffer* pyr_out;
    u32 num_pyr_levels;
    u32 num_pyrs;
    u8** gauss_buffers;
    void* gaussDmaTrans;
} gaussParameters_t;
typedef struct cornerParameters_t
{
    HARRIS_OUTPUT_TYPE* corners1;
    HARRIS_OUTPUT_TYPE* corners2;
    HARRIS_OUTPUT_TYPE* corners3;
    fp32 thresholds[MAX_NUM_CELLS];
    u32 lineNo;
    u32 width;
    u32 height;
    tvFeatureCell* outCellData;
    u32 cellGridDimension;
    u32 maxNumFeatures;
    bool insertAndSort;
} cornerParameters_t;

typedef struct pixelPipeParams_t
{
    frameBuffer* in_img;
    tvFeatureCell** feature_cells;
    tvPyramidBuffer* pyramidBuffer;
    fp32* thresholds;
    u32 num_pyr_levels;
    u32 num_pyrs;
    u32 cellGridDimension;
    u32 maxNumFeatures;
    u8* svuBuff;
    u32 svuBuffSize;
    void* dmaBuff;
    u32 dmaBuffSize;
    fifoCommTask_t* gaussTask;
    fifoCommTask_t* cornerTask;
    bool isInitialised;
    pyramidAlgoType_t pyrAlg;
    cornerConfig_t corCfg;
}pixelPipeParams_t;
#endif


