///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     pixelPipeline for vtrack project
///

#include <string.h>
#include <stdio.h>

#include <stdlib.h>
#include <assert.h>
#include "swcFrameTypes.h"

#include "mv_types.h"
#include "pixelPipeHelper.h"
#include "pixelPipeGaussShaveHelper.h"
#include "vTrack.h"
#include "gaussHx2.h"
#include "gaussVx2.h"
#include "thresholdFilter.h"
#include "nonMax3x3_fp32.h"
#include "fifoCommApiDefines.h"
#include "global_constants.h"

#if defined(MA2150) || defined(MA2450)
#include "scCmxDma.h"
#include "scCmxDmaDefines.h"
#define ShDrvCmxDmaInitialize ScCmxDmaInitialize
#define ShDrvCmxDmaTransactionHnd ScCmxDmaTransactionHnd
#define ShDrvCmxDmaTransaction ScCmxDmaTransaction
#define ShDrvCmxDmaCreateTransaction ScCmxDmaCreateTransaction
#define ShDrvCmxDmaStartTransfer ScCmxDmaStartTransfer
#define ShDrvCmxDmaWaitTransaction ScCmxDmaWaitTransaction
#else
#include "ShDrvCmxDma.h"
#endif

//#define mvcvThresholdFilter_asm mvcvThresholdFilter
//#define mvcvGaussVx2_asm mvcvGaussVx2
//#define mvcvGaussHx2_asm mvcvGaussHx2
//#define mvcvNonMax3x3_fp32_asm mvcvNonMax3x3_fp32
#include "svuCommonShave.h"

struct tvFeatureCell;

// given line 10 pixels wide in a 14 byte buffer (padded line)
// create a padded line with replications
//  Example input:  --abcdefghjk--
//  Example output: aaabcdefghjkkk
//
void padLine(u8* line, u32 width)
{
    u32 i;
    /**(line -1)          = *line;
     *(line -2)          = *line;
     *(line -3)          = *line;
     *(line + width + 0) = *(line + width - 1);
     *(line + width + 1) = *(line + width - 1);
     *(line + width + 2) = *(line + width - 1);*/
    u8 pixelValueFirst = *(line);
    u8 pixelValueLast = *(line + width - 1);
    for (i = 0; i < PADDING_H; i++)
    {
        *(line - i - 1) = pixelValueFirst;
        *(line + width + i) = pixelValueLast;
    }
}

extern "C"
void gaussInit()
{
    ShDrvCmxDmaInitialize(NULL);
}

// pyramid levels are calculated iteratively.
// this function performs separable Gaussian filtering.  Note that
//    -- horizontal filtering is required on each input line
//    -- vertical filtering is only required for every second input line
void PyrLevelRec(u32 pyrN, u8* line_buff, u32 line_cnt, u32 pyr_width,
                 tvPyramidBuffer* pyr_out, const u32 num_pyr_levels, u8** gaussBuffers, void* list)
{
    ShDrvCmxDmaTransactionHnd hnd;
    u8** vbuffs = &gaussBuffers[pyrN * GAUSS_V_SPAN];
    if (pyrN < num_pyr_levels - 1)
    {
        frameBuffer* pyrLevel = &pyr_out->pyrLevel[pyrN];

        if(line_cnt > pyrLevel->spec.height - 1) {
            memcpy(vbuffs[4] - PADDING_H, vbuffs[3] - PADDING_H,
                   pyr_width + 2 * PADDING_H);
        } else {
            mvcvGaussHx2_asm((u8*) line_buff, (u8*) (vbuffs[4]), pyr_width);
        }
        if (line_cnt == 0)
        {
            memcpy(vbuffs[2] - PADDING_H, vbuffs[4] - PADDING_H,
                   pyr_width + 2 * PADDING_H);
            memcpy(vbuffs[3] - PADDING_H, vbuffs[4] - PADDING_H,
                   pyr_width + 2 * PADDING_H);
        }
        if ((line_cnt >= 2) && ((line_cnt & 1) == 0))
        {
            // Use the first line of the input buffer as the output buffer also as we won't need it afterwards
            mvcvGaussVx2_asm((u8**) &(vbuffs[0]),
                             (u8*) vbuffs[0],
                             pyr_width);
            padLine(vbuffs[0], pyr_width);
            PyrLevelRec(pyrN + 1, vbuffs[0], (line_cnt - 1) >> 1,
                        pyr_width >> 1,
                        pyr_out, num_pyr_levels, gaussBuffers, list);
            frameBuffer* nextPyrLevel = &pyr_out->pyrLevel[pyrN+1];
            ShDrvCmxDmaCreateTransaction(&hnd, (ShDrvCmxDmaTransaction*)list,
                                 (u8*) vbuffs[0] - PADDING_H,
                                 (u8*) (nextPyrLevel->p1 + ((line_cnt - 1) >> 1) *
                                        nextPyrLevel->spec.stride),
                                        nextPyrLevel->spec.stride);
            ShDrvCmxDmaStartTransfer(&hnd);
            ShDrvCmxDmaWaitTransaction(&hnd);

        }
        ROTATE_BUFFER(vbuffs, GAUSS_V_SPAN);
    }

}

void PyrLevel(void* params)
{
    gaussParameters_t* gaussParameters =  (gaussParameters_t*) params;

    ShDrvCmxDmaTransactionHnd hnd;
    ShDrvCmxDmaTransaction* list = (ShDrvCmxDmaTransaction*)gaussParameters->gaussDmaTrans;
    u32 line_cnt = gaussParameters->line_cnt;
    u32 pyr_width = gaussParameters->pyr_width;
    u32 pyr_height = gaussParameters->pyr_height;
    u8* line_buff = gaussParameters->line_buff;
    tvPyramidBuffer* pyr_out = gaussParameters->pyr_out;

    u32 num_pyr_levels = pyr_out->numPyramidLevels;
    frameBuffer* pyrLevel = &pyr_out->pyrLevel[0];
    if(line_cnt < pyr_height){
    ShDrvCmxDmaCreateTransaction(&hnd, list,
            (u8*) line_buff - PADDING_H,
            (u8*) (pyrLevel->p1 + (line_cnt) *
                pyrLevel->spec.stride),
            pyrLevel->spec.stride);
    ShDrvCmxDmaStartTransfer(&hnd);
    ShDrvCmxDmaWaitTransaction(&hnd);
    }
    PyrLevelRec(0, line_buff, line_cnt, pyr_width >> 1, pyr_out, num_pyr_levels,
            gaussParameters->gauss_buffers, gaussParameters->gaussDmaTrans);
}


