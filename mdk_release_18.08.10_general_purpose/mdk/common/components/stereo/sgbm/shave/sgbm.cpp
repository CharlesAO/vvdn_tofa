///
/// @file SGBM.cpp
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief Apply cost aggregation over a census cost (inspired by Semi-Global block Matching); SGBM Source File
///


//---------------------------------------------------------------------------//
//------------------------------- INCLUDES ----------------------------------//
//---------------------------------------------------------------------------//

// C++ includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Local Modules
#include "disparityMapApiDefines.h"
#include "kernelsCommonDefines.h"
#include "sgbm.h"
#include "helpers.h"

#include <svuCommonShave.h>

extern u32 functionTableIdx;
u32 upP2[MAX_WIDTH];
u32 upP1[MAX_WIDTH];
//--------------------------------------------------------------------//
// SGBMLineBased - compute aggregated cost for one path over one line //
//--------------------------------------------------------------------//
void SGBMLineBased(u8* lineCurrent, u8* linePath, u8* matchingCostCurrentPixel, u8* matchingCostPath, u8* aggregatedCost, u32 width, u32 disparities, u32* penaltyLUTP1, u32* penaltyLUTP2)
{
    u8 min2;
    u32 updatedP1 = ref_P1;
    u32 updatedP2 = 0;
    u32 cc = 0;

    for (u32 column = 0; column < width; column++)
    {
        u32 intensityDiff = (u32)abs(lineCurrent[column] - linePath[column]);
        updatedP2 = penaltyLUTP2[intensityDiff];
        updatedP1 = penaltyLUTP1[intensityDiff];

        cc                          = column * disparities;
        u8* matchingCostPathPtr     = &matchingCostPath[cc];
        u8* matchingCostCurrentPtr  = &matchingCostCurrentPixel[cc];
        u8* aggregatedCostPtr       = &aggregatedCost[cc];

        // compute min2
        mvcvMinKernel64(matchingCostPathPtr, &min2);

        // compute min1 and aggregated cost
        mvcvAggregateCostSGBM64_clamp(matchingCostPathPtr, matchingCostCurrentPtr, aggregatedCostPtr, updatedP1, updatedP2, min2, 1);
    }
}

void SGBMLineBasedRL(u8* lineCurrent, u8* linePath, u8* matchingCostCurrentPixel, u8* matchingCostPath, u8* aggregatedCost, u32 width, u32 disparities, u32* penaltyLUTP1, u32* penaltyLUTP2)
{
    u8 min2;
    u32 updatedP1 = ref_P1;
    u32 updatedP2 = 0;
    u32 cc = 0;

    for (s32 column = (width-1); column >= 0; column--)
    {
        u32 intensityDiff = (u32)abs(lineCurrent[column] - linePath[column]);
        updatedP2 = penaltyLUTP2[intensityDiff];
        updatedP1 = penaltyLUTP1[intensityDiff];

        cc                          = column * disparities;
        u8* matchingCostPathPtr     = &matchingCostPath[cc];
        u8* matchingCostCurrentPtr  = &matchingCostCurrentPixel[cc];
        u8* aggregatedCostPtr       = &aggregatedCost[cc];

        // compute min2
        mvcvMinKernel64(matchingCostPathPtr, &min2);

        // compute min1 and aggregated cost
        mvcvAggregateCostSGBM64_clamp(matchingCostPathPtr, matchingCostCurrentPtr, aggregatedCostPtr, updatedP1, updatedP2, min2, 1);
    }
}

//--------------------------------------------------------------------//
// semiGlobalBlockMatching - Compute aggregated cost using 5 paths    //
//--------------------------------------------------------------------//
void semiGlobalBlockMatching(u32 svuIdx,
        stereoUserConfig_t* userCfg,
        stereoSGBMConfig_t* sgbmConfig,
        u8* imgLine[2],
        u8* cost[2],
        u8* aggCostPath[2],
        u8* aggCostAllPaths,
        u32 line, u8* test[2][5])
{
    u32* penaltyTableP1 = sgbmConfig->penaltyTableP1;
    u32* penaltyTableP2 = sgbmConfig->penaltyTableP2;

    const u32 width     = sgbmConfig->width;
    const u32 disp      = sgbmConfig->disparities;
    const u32 prev      = (line - 1) % 2;
    const u32 curr      = line % 2;
    const u32 prevLine  = (line - 1) % 3;
    const u32 currLine  = line % 3;

#ifdef __PC__
    FILE* i = fopen("img.bin", "ab");
    fwrite(userCfg->aggCostPaths[prev][0], width * 64, 1, i);
    fclose(i);
#endif
    // following statement may seem weird but is correct: disp = 64 at L0 means disp = 128 in the ftable (DISP_F_TABLE_IDX_D128)
    u32 fIdx = ((disp == 64) ? AGG5_F_TABLE_IDX_D128 : AGG5_F_TABLE_IDX_D64);

    paddLine(imgLine[currLine] - 1, imgLine[currLine]);
    paddLine(imgLine[currLine] + width, imgLine[currLine] + width - 1);
    paddLine(imgLine[prevLine] - 1, imgLine[prevLine]);
    paddLine(imgLine[prevLine] + width, imgLine[prevLine] + width - 1);

#ifdef __MOVICOMPILE__
    UNUSED(test);
    // Compute aggregated cost: 1 Shave => 1 path
    switch (svuIdx)
    {
    case 0: // Left
        paddCost(aggCostPath[curr]-disp, cost[curr], disp);
        SGBMLineBased(imgLine[currLine], imgLine[currLine] - 1,
                cost[curr], aggCostPath[curr] - disp, aggCostPath[curr],
                width, disp, penaltyTableP1, penaltyTableP2);
        break;

    case 1: // Top-Left
        SGBMLineBased(imgLine[currLine], imgLine[prevLine] - 1,
                cost[curr], aggCostPath[prev] - disp, aggCostPath[curr],
                width, disp, penaltyTableP1, penaltyTableP2);
        paddCost(aggCostPath[prev] - disp, aggCostPath[curr], disp);
        break;

    case 2: // Top
        SGBMLineBased(imgLine[currLine], imgLine[prevLine],
                cost[curr], aggCostPath[prev], aggCostPath[curr],
                width, disp, penaltyTableP1, penaltyTableP2);
        break;

    case 3: // Top-Right
        SGBMLineBasedRL(imgLine[currLine], imgLine[prevLine] + 1,
                cost[curr], aggCostPath[prev] + disp, aggCostPath[curr],
                width, disp, penaltyTableP1, penaltyTableP2);
        paddCost(aggCostPath[curr] + (width) * disp,
                aggCostPath[curr] + (width - 1) * disp, disp);
        break;

    case 4: // Right
        paddCost(aggCostPath[curr] + (width) * disp,
                cost[curr] + (width - 1) * disp, disp);
        SGBMLineBasedRL(imgLine[currLine], imgLine[currLine] + 1,
                cost[curr], aggCostPath[curr] + disp, aggCostPath[curr],
                width, disp, penaltyTableP1, penaltyTableP2);
        break;

    default:
        assert(1 && "Invalid Shave index in SGBM. Valid range is [0 - 4]");
    }

    // Wait for each Shave to finish the aggregation of it's own path because
    // each Shave will need all other aggregated paths.
    barrier(svuIdx, userCfg->list_of_shaves_thread_1,
            COUNT_OF(userCfg->list_of_shaves_thread_1));

    // Aggregate the 5 paths by computing the arithmetic average.
    // Place the result in the current buffer which will become the previous.
    // This operation is executed redundantly, over the whole line, by each
    // Shave because it's faster than memcpy-ing in and out of the slices.
    ((aggregateFivePaths)fTable[fIdx])(
            userCfg->aggCostPaths[curr][0],
            userCfg->aggCostPaths[curr][1],
            userCfg->aggCostPaths[curr][2],
            userCfg->aggCostPaths[curr][3],
            userCfg->aggCostPaths[curr][4],
            aggCostAllPaths, width);

#else

    // Left
    paddCost(test[curr][0] - disp, cost[curr], disp);
    SGBMLineBased(imgLine[currLine], imgLine[currLine] - 1,
                cost[curr], test[curr][0] - disp, test[curr][0],
                width, disp, penaltyTableP1, penaltyTableP2);

     // Top-Left
    SGBMLineBased(imgLine[currLine], imgLine[prevLine] - 1,
                cost[curr], test[prev][1] - disp, test[curr][1],
                width, disp, penaltyTableP1, penaltyTableP2);
    paddCost(test[prev][1] - disp, test[curr][1], disp);

    // Top
    SGBMLineBased(imgLine[currLine], imgLine[prevLine],
                cost[curr], test[prev][2], test[curr][2],
                width, disp, penaltyTableP1, penaltyTableP2);

    // Top-Right
    SGBMLineBasedRL(imgLine[currLine], imgLine[prevLine] + 1,
                cost[curr], test[prev][3] + disp, test[curr][3],
                width, disp, penaltyTableP1, penaltyTableP2);
    paddCost(test[curr][3] + (width) * disp,
            test[curr][3] + (width - 1) * disp, disp);

    // Right
    paddCost(test[curr][4] + (width) * disp,
                   cost[curr] + (width - 1) * disp, disp);
    SGBMLineBasedRL(imgLine[currLine], imgLine[currLine] + 1,
                cost[curr], test[curr][4] + disp, test[curr][4],
                width, disp, penaltyTableP1, penaltyTableP2);

    // Aggregate the 5 paths by computing the arithmetic average.
    ((aggregateFivePaths)fTable[fIdx])(
            test[curr][0],
            test[curr][1],
            test[curr][2],
            test[curr][3],
            test[curr][4],
            aggCostAllPaths, width);
#endif
}
