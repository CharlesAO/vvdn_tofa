///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

#ifndef __SIPP_MA2x5x_H__
#define __SIPP_MA2x5x_H__

#include <sippBaseTypes.h>
#include <sippCfg.h>
#include <sippEvents.h>
#include "DrvSippDefines.h"
#include <sippHwIds.h>
#include <sippPlatform.h>
#include <sippHwBitfieldDefs.h>
#include <sippTypes.h>
#include <sippShaveSym.h>

////////////////////////////////////////////////////////////////
// Public API
////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////
// Pipeline creation

void           sippSetInitMask (u32 mask);
bool           sippInitialize (void);
void           sippTerm (void);
SippPipeline * sippCreatePipeline (u32 sliceFirst, u32 sliceLast, u8 * pmBinImg);
void           sippFinalizePipeline (SippPipeline * pl);
SippFilter *   sippCreateFilter (SippPipeline * pPipe, u32 flags, u32 out_W, u32 out_H, u32 num_pl, u32 bpp, u32 paramSz, FnSvuRun funcSvuRun, const char * name);
void           sippFilterSetBufBitsPP (pSippFilter pFilter, u32 oBufIdx, u32 bitsPerPixel);
void           sippPipeSetNumLinesPerBuf (pSippFilter pFilter, u32 oBufIdx, u32 numLines);
void           sippLinkFilter (SippFilter * f, SippFilter * par, u32 vKerSz, u32 hKerSz);
void           sippLinkFilterSetOBuf (SippFilter * pFilter, SippFilter * pParent, u32 parentOBufIdx);
void           sippFilterAddOBuf (pSippFilter pFilter, u32 numPlanes, u32 bpp);
void           sippDeletePipeline (SippPipeline * pPipe);
void           sippRegisterEventCallback (SippPipeline * pPipe, sippEventCallback_t pfCallback);

#if SIPP_RTOS == SIPP_NO_RTOS
void           sippFrameworkRun (void);
#endif

////////////////////////////////////////////////////////////////
// Memory allocation

s32    sippAllocCmxMemRegion (SippPipeline * pipe, SippMemRegion * memRegList);
void   sippMemStatus (void);
u32    sippMemCheck (ptSippMCB pSippMCB, SippVirtualPool vPool);
void   sippChooseMemPool (ptSippMCB pSippMCB, SippVirtualPool vPool, u32 physPoolIdx);

////////////////////////////////////////////////////////////////
// Sipp functions

#if SIPP_RTOS == SIPP_NO_RTOS
void sippProcessFrame (SippPipeline * pl);
#endif
void sippProcessFrameNB (SippPipeline * pl);
void sippProcessIters (SippPipeline * pl, u32 numIters);
void sippProcessItersNB (SippPipeline * pl, u32 numIters);

void sippReschedulePipeline (SippPipeline * pPipe);

#ifdef SIPP_PC
void sippGenericRuntimeWaitIsrThreadTerm (void);
#endif

void sippPipeSetLinesPerIter (pSippPipeline pPipe, u32 linesPerIter);

////////////////////////////////////////////////////////////////
// File utils

void sippRdFileU8 (u8 * buff, int count, const char * fName);
void sippWrFileU8 (u8 * buff, int count, const char * fName);
void sippRdFileU8toF16 (half * buff, int count, const char * fName);
void sippWrFileF16toU8 (half * buff, int count, const char * fName);
void sippTestCrcCheck (const void * pStart, u32 lengthBytes, u32 expectedCrc);

////////////////////////////////////////////////////////////////
// Error report

void sippErrorSetFatal (u32 errCode);
void sippError (u32 * errStatusMask, u32 errCode);
u32  sippGetLastError (void);
u32  sippGetErrorHistory (u32 * ptrErrList);
u32  sippPipeGetErrorStatus (SippPipeline * pPipe);

void sippDbgLevel (SippPipeline * pl, int level);

////////////////////////////////////////////////////////////////
// Data compare

void sippDbgCompareU8 (u8  * refA, u8 * refB, int len);
void sippDbgCompareU16 (u16 * refA, u16 * refB, int len);
void sippDbgCompareU32 (u32 * refA, u32 * refB, int len);
void sippDbgCompareDeltaU8 (u8 * refA, u8 * refB, int len, u8 delta);

////////////////////////////////////////////////////////////////
// RunTime Stat interpretation functions

#ifdef SIPP_RUNTIME_PROFILING
void sippDbgParseRTStats (SippPipeline * pPipe, u8 printCtrl);
#endif

void sippSetSliceSize (u32 size);

#include <sippHwDefs.h>

////////////////////////////////////////////////////////////////
// ISP utils
void sippUtilComputeFp16Lut (half (*formula)(half input), half *outLut, u32 lutSize);
void sippUtilComputeFp16LutChannelMode (half (*formula)(half input), half *outLut, u32 lutSize);
void sippUtilPrintFp16Lut (half *fp16Lut, u32 lutSize, const char *fName);
void sharpenSigmaToCoefficients (float sigma,    u16 *coeffs);
void lumaGenLut (float strength, u8 *lut, int *bitpos);

////////////////////////////////////////////////////////////////
// HW filter config helper functions
u32 sippHwChromaDnsCfgReg (u32 numHorizPass, u32 refInputEn, u32 diffLimit, u32 forceHorizWeight, u32 forceVertWeight, u32 threePlaneModeEn);
u32 sippHwConvolutionCfgReg (u32 kernelSz, u32 oputClamp, u32 oputAbsVal, u32 oputSquare, u32 enAccum, u32 disableFilter, u32 accumThresh);
u32 sippHwHarrisCornerCfgReg (u32 uKernelSize, u32 uOutputDeterminant);
u32 sippHwLutCfgReg (u32 interpMode, u32 channelMode, u32 integerWidth, u32 numLUTSPerChannel, u32 numActiveChannels, u32 enLUTLoad, u32 apbAccessEn);
u32 sippHwMedianCfgReg (s32 nThres, u32 uKernelSize, u32 uOSel);
u32 sippHwUnsharpCfgReg (u32 uThres, u32 uKernelSize, u32 uClampOutput, u32 uMode, u32 uOutputDeltas);

////////////////////////////////////////////////////////////////
// Shave filter utils
u32  getInPtr (SippFilter *fptr, u32 parent, u32 iterNo, u32 lineNo, u32 planeNo);
u32  getOutPtr (SippFilter *fptr, u32 iterNo, u32 planeNo);
u32  getPlaneIoPtrs (SippFilter *fptr, u32 parNo, u32 iterNo, u32 planeNo, void *inPtrs);
void getIn3PlanePtr (SippFilter *fptr, u32 parent, u32 iterNo, u32 lineNo, void *out3Ptr);

UInt32 sippFilterGetNumOutPlanes (SippFilter * fptr, UInt32 outBufferIdx);
UInt32 sippFilterGetOutputBpp (SippFilter * fptr);
UInt32 sippFilterGetParentInputLines (SippFilter * fptr, UInt32 parentIdx);
UInt32 sippFilterGetParentPlaneStride (SippFilter * fptr, UInt32 parentIdx);
UInt32 sippFilterGetPlaneStride (SippFilter * fptr, UInt32 outBufferIdx);
UInt32 sippFilterGetLinesThisIter (SippFilter * fptr);
UInt32 sippFilterGetLinesPerIter (SippFilter * fptr);
UInt32 sippFilterGetParentSliceWidth (SippFilter * fptr, UInt32 parentIdx);
UInt32 sippFilterGetParentOutputWidth (SippFilter * fptr, UInt32 parentIdx);
UInt32 sippFilterGetParentOutputHeight (SippFilter * fptr, UInt32 parentIdx);

////////////////////////////////////////////////////////////////
// M2 related stuff
#if defined(__myriad2__)
void sippUpShaveCmxPriority (void);         // Increase Shave-IDC CMX priority to reduce stalls
void sippGlobalHwClockCfg (u32 enable); // SIPP global clock gating control
void sippHwIndividualClkEn (u32 mask);  // Individual SIPP filters clock enable

#endif //__myriad2__

////////////////////////////////////////////////////////////////
// Direct shave control
void sippStopSvus (SippPipeline * pl);

////////////////////////////////////////////////////////////////
// Re-route irqs to Leon OS
void sippDynRouteIrq (u32 irqSipp0_dynamic, u32 irqSipp1_dynamic, u32 irqSipp2_dynamic);

#ifdef __cplusplus
}
#endif

// End of API declaration
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
// API Macros

////////////////////////////////////////////////////////////////
// Sipp Filter Flags (SippFilter.flags)
#define SIPP_REQ_SW_VIEW        (1<<2)
#define SIPP_FLAG_DO_H_PADDING  (1<<3)
#define SIPP_RESIZE             (1<<4)
#define SIPP_CROP               (1<<5)
#define SIPP_SHIFT_PLANES       (1<<6)
#define SIPP_PROVIDE_CHUNK_POS  (1<<7)

////////////////////////////////////////////////////////////////
// Pipeline flags
#define PLF_REQUIRES_SW_PADDING  (1<<0)
#define PLF_UNIQUE_SVU_CODE_SECT (1<<1)
#define PLF_IS_FINALIZED         (1<<2)
#define PLF_MAP_SVU_CODE_IN_DDR  (1<<3)
#define PLF_RUNS_ITER_GROUPS     (1<<4)           // So the client needs to set this in order to establish this
                                                  // as a pipeline which can run in iterations?
#define PLF_DISABLE_OPIPE_CONS   (1<<5)           // Set to make framework avoid oPipe connections though they may be available
#define PLF_PROVIDE_RT_STATS     (1<<6)           // Set to make runtime statistics available via pipe structure
#define PLF_ENABLE_SW_QU_USE     (1<<7)           // Enable SW command queue usage, potentially increasing performance
#define PLF_CONSIDER_OPIPE_RT    (1<<8)           // Consider the opipe runtime as an option for the pipe


////////////////////////////////////////////////////////////////
// Decorator macros to make calls to sippCreateFilter() more readable
#define N_PL(x)   (x) // number of planes
#define BPP(x)    (x) // Bits per pixel of the output buffer
#define  SZ(x)    sizeof(x)

////////////////////////////////////////////////////////
// Placeholder for don't care values

#define SIPP_AUTO (-1)


#endif // !__SIPP_MA2x5x_H__
