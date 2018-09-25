#ifndef __COMMON_SHAVE_DEFINES__
#define __COMMON_SHAVE_DEFINES__

// MvCV
#include <censusMatching64.h>
#include <censusMatching32.h>
#include <censusMatchingPyrOnePos.h>
#include <censusMatchingPyrOnePosWindow.h>
#include <censusMinConfidence64.h>
#include <censusMinConfidence32.h>
#include <computeCombinedCost64.h>
#include <computeCombinedCost32.h>
#include <computeCombinedCost3.h>
#include <computeCombinedCost3Window.h>
#include <computeADPyrOnePos.h>
#include <computeADPyrOnePosWindow.h>
#include <computeAD64.h>
#include <computeAD32.h>
#include <minKernel64.h>
#include <minKernel32.h>
#include <aggregateCostSGBM64_clamp.h>
#include <aggregateCostSGBM32_clamp.h>
#include <aggregateFivePaths64.h>
#include <aggregateFivePaths32.h>
#include <censusTransformAverageRefMask7x7.h>
#include <censusMinSubpixel3.h>
#include <censusMinSubpixel3Window.h>
#include <censusMinConfidence64Subpixel.h>
#include <thresholdDisparityFp16.h>
#include <convertFp16ToQ4.h>
#include <minAggregate_line.h>
#include <convert_fp16_u8.h>

// Generic pointer function type
typedef void (*fType)();

// Specific pointer function types
typedef void (*censusMatching)(u32 *in1, u32 *in2, u8 *out, u32 width, u32 flag);
typedef void (*censusMin)(u8 *in, u8 *out, u8* outm, u32 width);
typedef void (*censusMinConfidence)(u8 *in, u8 *out, u8* outm, u32 width);
typedef void (*computeCombinedCost)(u8* disparityCost, u8* adCost, u32 alpha, u32 beta, u32 width, u32 normFactor);
typedef void (*computeAD)(u8* inputLeft, u8* inputRight, u8* output, u32 width, u32 flag);
typedef void (*minKernel)(u8* in, u8* minVal);
typedef void (*aggregateCost)(u8* input1, u8* input2, u8* output, u16 penaltyP1, u16 penaltyP2, u8 min2, u8 activateClamp);
typedef void (*aggregateThreePaths)(u8* path0, u8* path1, u8* path2, u8* out, u32 width);
typedef void (*aggregateFivePaths)(u8* path0, u8* path1, u8* path2, u8* path3, u8* path4, u8* out, u32 width);
//#define USE_C_KERNELS
// MvCV kernels
#ifdef __PC__

#define censusTransform                 mvcvCensusTransformAverageRefMask7x7
#define mvcvCensusMatchingPyrOnePos     mvcvCensusMatchingPyrOnePos
#define mvcvCensusMatchingPyrOnePosWindow mvcvCensusMatchingPyrOnePosWindow
#define mvcvCensusMinSubpixel3          mvcvCensusMinSubpixel3
#define mvcvCensusMin3SubpixelWindow    mvcvCensusMinSubpixel3Window
#define mvcvComputeCombinedCost3        mvcvComputeCombinedCost3
#define mvcvComputeCombinedCost3Window  mvcvComputeCombinedCost3Window
#define mvcvComputeADPyrOnePos          mvcvComputeADPyrOnePos
#define mvcvComputeADPyrOnePosWindow    mvcvComputeADPyrOnePosWindow
#define mvcvThresholdDisparity          mvcvThresholdDisparityFp16
#define mvcvCensusMatching64            mvcvCensusMatching64
#define mvcvComputeAD64                 mvcvComputeAD64
#define mvcvComputeCombinedCost64       mvcvComputeCombinedCost64
#define mvcvCensusMinConfidence64       mvcvCensusMinConfidence64
#define mvcvMinKernel64                 mvcvMinKernel64
#define mvcvAggregateCostSGBM64_clamp   mvcvAggregateCostSGBM64_clamp
#define mvcvAggregateFivePaths64        mvcvAggregateFivePaths64
#define mvcvCensusMatching32            mvcvCensusMatching32
#define mvcvComputeAD32                 mvcvComputeAD32
#define mvcvComputeCombinedCost32       mvcvComputeCombinedCost32
#define mvcvCensusMinConfidence32       mvcvCensusMinConfidence32
#define mvcvMinKernel32                 mvcvMinKernel32
#define mvcvAggregateCostSGBM32_clamp   mvcvAggregateCostSGBM32_clamp
#define mvcvAggregateFivePaths32        mvcvAggregateFivePaths32
#define mvcvConvertFp16ToQ4             mvcvConvertFp16ToQ4
#define mvcvMinAggregate_line           mvcvMinAggregate_line
#define mvcvConvert_fp16_u8             mvcvConvert_fp16_u8
#define mvcvCensusMinConfidence64Subpixel mvcvCensusMinConfidence64Subpixel
#else

#ifndef USE_C_KERNELS
#define censusTransform                 mvcvCensusTransformAverageRefMask7x7_asm
#define mvcvCensusMatchingPyrOnePos     mvcvCensusMatchingPyrOnePos_asm
#define mvcvCensusMatchingPyrOnePosWindow mvcvCensusMatchingPyrOnePosWindow_asm
#define mvcvCensusMinSubpixel3          mvcvCensusMinSubpixel3_asm
#define mvcvCensusMin3SubpixelWindow    mvcvCensusMinSubpixel3Window_asm
#define mvcvComputeCombinedCost3        mvcvComputeCombinedCost3_asm
#define mvcvComputeCombinedCost3Window  mvcvComputeCombinedCost3Window_asm
#define mvcvComputeADPyrOnePos          mvcvComputeADPyrOnePos_asm
#define mvcvComputeADPyrOnePosWindow    mvcvComputeADPyrOnePosWindow_asm
#define mvcvThresholdDisparity          mvcvThresholdDisparityFp16_asm
#define mvcvCensusMatching64            mvcvCensusMatching64_asm
#define mvcvComputeAD64                 mvcvComputeAD64_asm
#define mvcvComputeCombinedCost64       mvcvComputeCombinedCost64_asm
#define mvcvCensusMinConfidence64       mvcvCensusMinConfidence64_asm
#define mvcvMinKernel64                 mvcvMinKernel64_asm
#define mvcvAggregateCostSGBM64_clamp   mvcvAggregateCostSGBM64_clamp_asm
#define mvcvCensusMatching32            mvcvCensusMatching32_asm
#define mvcvComputeAD32                 mvcvComputeAD32_asm
#define mvcvComputeCombinedCost32       mvcvComputeCombinedCost32_asm
#define mvcvCensusMinConfidence32       mvcvCensusMinConfidence32_asm
#define mvcvMinKernel32                 mvcvMinKernel32_asm
#define mvcvAggregateCostSGBM32_clamp   mvcvAggregateCostSGBM32_clamp_asm
#define mvcvAggregateFivePaths32        mvcvAggregateFivePaths32_asm
#define mvcvAggregateFivePaths64        mvcvAggregateFivePaths64_asm
#define mvcvConvertFp16ToQ4             mvcvConvertFp16ToQ4_asm
#define mvcvMinAggregate_line           mvcvMinAggregate_line_asm
#define mvcvConvert_fp16_u8             mvcvConvert_fp16_u8_asm
#define mvcvCensusMinConfidence64Subpixel mvcvCensusMinConfidence64Subpixel_asm
#else
#define censusTransform                 mvcvCensusTransformAverageRefMask7x7
#define mvcvCensusMatchingPyrOnePos     mvcvCensusMatchingPyrOnePos
#define mvcvCensusMatchingPyrOnePosWindow mvcvCensusMatchingPyrOnePosWindow
#define mvcvCensusMinSubpixel3          mvcvCensusMinSubpixel3
#define mvcvCensusMin3SubpixelWindow    mvcvCensusMinSubpixel3Window
#define mvcvComputeCombinedCost3        mvcvComputeCombinedCost3
#define mvcvComputeCombinedCost3Window  mvcvComputeCombinedCost3Window
#define mvcvComputeADPyrOnePosWindow    mvcvComputeADPyrOnePosWindow
#define mvcvComputeADPyrOnePos          mvcvComputeADPyrOnePos
#define mvcvThresholdDisparity          mvcvThresholdDisparityFp16
#define mvcvCensusMatching64            mvcvCensusMatching64
#define mvcvComputeAD64                 mvcvComputeAD64
#define mvcvComputeCombinedCost64       mvcvComputeCombinedCost64
#define mvcvCensusMinConfidence64       mvcvCensusMinConfidence64
#define mvcvMinKernel64                 mvcvMinKernel64
#define mvcvAggregateCostSGBM64_clamp   mvcvAggregateCostSGBM64_clamp
#define mvcvCensusMatching32            mvcvCensusMatching32
#define mvcvComputeAD32                 mvcvComputeAD32
#define mvcvComputeCombinedCost32       mvcvComputeCombinedCost32
#define mvcvCensusMinConfidence32       mvcvCensusMinConfidence32
#define mvcvMinKernel32                 mvcvMinKernel32
#define mvcvAggregateCostSGBM32_clamp   mvcvAggregateCostSGBM32_clamp
#define mvcvAggregateFivePaths32        mvcvAggregateFivePaths32
#define mvcvAggregateFivePaths64        mvcvAggregateFivePaths64
#define mvcvConvertFp16ToQ4             mvcvConvertFp16ToQ4
#define mvcvMinAggregate_line           mvcvMinAggregate_line
#define mvcvConvert_fp16_u8             mvcvConvert_fp16_u8
#define mvcvCensusMinConfidence64Subpixel mvcvCensusMinConfidence64Subpixel
#endif

#endif

static const fType fTable[14] =
{
    // D_128_F_TABLE_IDX
    (fType)mvcvCensusMatching64, (fType)mvcvComputeAD64, (fType)mvcvComputeCombinedCost64, (fType)mvcvCensusMinConfidence64,
    // SGBM_F_TABLE_IDX
    (fType)mvcvMinKernel64, (fType)mvcvAggregateCostSGBM64_clamp, (fType)mvcvAggregateFivePaths64,
    // D_64_F_TABLE_IDX
    (fType)mvcvCensusMatching32, (fType)mvcvComputeAD32, (fType)mvcvComputeCombinedCost32, (fType)mvcvCensusMinConfidence32,
    // SGBM_F_TABLE_IDX
    (fType)mvcvMinKernel32, (fType)mvcvAggregateCostSGBM32_clamp, (fType)mvcvAggregateFivePaths32
};

// Function table indexes
#define F_TABLE_IDX_SHIFT       7
#define COST_F_TABLE_IDX_D128   0
#define COST_F_TABLE_IDX_D64    (COST_F_TABLE_IDX_D128 + F_TABLE_IDX_SHIFT)
#define DISP_F_TABLE_IDX_D128   3
#define DISP_F_TABLE_IDX_D64    (DISP_F_TABLE_IDX_D128 + F_TABLE_IDX_SHIFT)
#define SGBM_F_TABLE_IDX_D128   4
#define SGBM_F_TABLE_IDX_D64    (SGBM_F_TABLE_IDX_D128 + F_TABLE_IDX_SHIFT)
#define AGG5_F_TABLE_IDX_D128   6
#define AGG5_F_TABLE_IDX_D64    (AGG5_F_TABLE_IDX_D128 + F_TABLE_IDX_SHIFT)


#endif //__COMMON_SHAVE_DEFINES__
