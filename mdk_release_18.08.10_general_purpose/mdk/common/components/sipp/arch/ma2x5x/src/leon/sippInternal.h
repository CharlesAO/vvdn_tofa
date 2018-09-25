///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP core : internal definitions
///

#ifndef __SIPP_INTERNAL_H__
#define __SIPP_INTERNAL_H__

#include <sippDefines.h>
#include <sippTypesPrivate.h>
#include <sippSessionControl.h>
#include <sippPal.h>

#ifdef SIPP_RUNTIME_PROFILING
#include <DrvTimer.h>
#endif

//////////////////////////////////////////////////////////////////////////
// SIPP API helper functions
void sippHWInit (void);
bool sippSWInit (void);
void sippInitPipeline (ptSippPipelineSuper ptSPipe, u32 sliceFirst, u32 sliceLast, u32 sliceSize, u8 * mbinImg);
void sippAddFilterToPipe (pSippPipeline pPipe, pSippFilter pFilter, u32 flags, u32 out_W, u32 out_H, u32 num_pl, u32 bpp, FnSvuRun funcSvuRun, const char * name);
eSIPP_STATUS sippElaboratePipeline (pSippPipeline pPipe);
eSIPP_STATUS sippTermInternal (void);
eSIPP_STATUS sippFreePipeResource (pSippPipeline pPipe);
eSIPP_STATUS sippRescheduleRequest (pSippPipeline pPipe);
eSIPP_STATUS sippRunItersRequest (pSippPipeline pPipe, u32 uNumIters);
eSIPP_STATUS sippResetFilterVariables (pSippPipeline pPipe);

//////////////////////////////////////////////////////////////////////////
// Framework manager APIs
void sippCallbackInit (void);
eSIPP_STATUS sippIssueCommand (pSippPipeline pPipe, eSippCommand eSippCmd, SippCommandData pCmdData);
void sippEventNotify (pSippPipeline pPipe, eSIPP_PIPELINE_EVENT eEvent, SIPP_PIPELINE_EVENT_DATA * pData);

//////////////////////////////////////////////////////////////////////////
// Framework access scheduler APIs
#include <sippAccessScheduler.h>

//////////////////////////////////////////////////////////////////////////
// Framework session control APIs
void sippHWSessionInit (void);
void sippHWSessionCommand (pSippPipeline pPipe, u32 uHWPipeID, eSIPP_ACCESS_SCHEDULER_EVENT eEvent, SIPP_ACCESS_SCHEDULER_EVENT_DATA pData);
void sippHWSessionRemoveActiveLists (pSippPipeline pPipe, u32 uHWPipeID);
void sippHWSessionRemoveLoadedPipe (pSippPipeline pPipe);
void sippPipeSessionControlInit (void);
void sippPipeSessionControl (eSIPP_ACCESS_SCHEDULER_EVENT eEvent, SIPP_ACCESS_SCHEDULER_EVENT_DATA pData, u32 uPipeIdx, u32 uHWPipeIdx);
#ifdef RT_DBG
u32  sippHWGetPipeIdx (pSippPipeline pPipe);
#endif

//////////////////////////////////////////////////////////////////////////
// Base firmware APIs
eSIPP_STATUS sippCoreFinalisePipeline (pSippPipeline pPipe);
eSIPP_STATUS sippCoreResourceInit (void);
eSIPP_STATUS sippCoreReschedulePipeline (pSippPipeline pPipe);

//////////////////////////////////////////////////////////////////////////
// Pipeline validation and analysis
bool sippValidatePipe (pSippPipeline pPipe);
#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
void sippAnalysePipe2x5x (pSippPipeline pPipe);

u32 sippCheckOPipeConnectionSigma (SippFilter * ptrFilt, SippFilter * parentFilt, u32 * parOBufIdx);
u32 sippCheckOPipeConnectionLsc (SippFilter * ptrFilt, SippFilter * parentFilt, u32 * parOBufIdx);
u32 sippCheckOPipeConnectionRaw (SippFilter * ptrFilt, SippFilter * parentFilt, u32 * parOBufIdx);
u32 sippCheckOPipeConnectionDbyr (SippFilter * ptrFilt, SippFilter * parentFilt, u32 * parOBufIdx);
u32 sippCheckOPipeConnectionDoGLTM (SippFilter * ptrFilt, SippFilter * parentFilt, u32 * parOBufIdx);
u32 sippCheckOPipeConnectionLuma (SippFilter * ptrFilt, SippFilter * parentFilt, u32 * parOBufIdx);
u32 sippCheckOPipeConnectionSharpen (SippFilter * ptrFilt, SippFilter * parentFilt, u32 * parOBufIdx);
u32 sippCheckOPipeConnectionGenChroma (SippFilter * ptrFilt, SippFilter * parentFilt, u32 * parOBufIdx);
u32 sippCheckOPipeConnectionMedian (SippFilter * ptrFilt, SippFilter * parentFilt, u32 * parOBufIdx);
u32 sippCheckOPipeConnectionChrDns (SippFilter * ptrFilt, SippFilter * parentFilt, u32 * parOBufIdx);
u32 sippCheckOPipeConnectionColourComb (SippFilter * ptrFilt, SippFilter * parentFilt, u32 * parOBufIdx);
u32 sippCheckOPipeConnectionLut (SippFilter * ptrFilt, SippFilter * parentFilt, u32 * parOBufIdx);
u32 sippCheckOPipeConnectionPoly (SippFilter * ptrFilt, SippFilter * parentFilt, u32 * parOBufIdx);
u32 sippCheckOPipeConnectionDefault (SippFilter * ptrFilt, SippFilter * parentFilt, u32 * parOBufIdx);

s32 sippIdentifyOPipeSchedulingEntity (pSippPipeline pPipe);

#endif

//////////////////////////////////////////////////////////////////////////
// SIPP Core utility functions

void sippCoreHwInitialLoad (pSippPipeline pPipe);
void sippCoreHwInitialSave (pSippPipeline pPipe);
void sippComputeSliceLayout (pSippPipeline pPipe);
int  sippUsingPrecompSched (pSippPipeline pPipe);
void sippGetFirstHwFiltIdx (pSippPipeline pPipe);
u32  sippIniHwFilters (pSippPipeline pPipe);
u32  sippBuildLnBuffs (pSippPipeline pPipe);
void sippComputePaddingOffsets (pSippPipeline pPipe);
void sippAsmOptSetup (pSippPipeline pPipe);
void sippComputeSwOutCt (pSippPipeline pPipe);
void sippIncrementOutBuffs (pSippPipeline pPipe);
void sippComputeBufferProps (pSippPipeline pPipe);
void sippInitBufferLnPointers (pSippPipeline pPipe);
void sippUtilOrderPixels (u32 bayerPattern, u32 inGr, u32 inR, u32 inB, u32 inGb, u32 *out);
void sippListSort (s32 * pnList, s32 * pnSortIndices, u32 uSize, u8   descending);
s32  sippFindInList (void * pElement, void ** ppList, u32 numList);
void sippCQInit (SippPipeline * pPipe);
#ifdef SIPP_SCRT_ENABLE_OPIPE
void sippOPipeSetBuffLineStrides (pSippPipeline pPipe);
u32  sippBuildLnBuffsOPipe (pSippPipeline pPipe);
#endif

//////////////////////////////////////////////////////////////////////////
// Debug
void sippDbgPrintNumPar (SippFilter *filters[], u32 nFilters);
void sippDbgShowBuffPtr (SippFilter* fptr, const char *msg);
void sippDbgDumpRunMask (u32 mask,    int iteration, int dbgDump);
void sippDbgFrameCheck (SippPipeline *pl);
void sippDbgDumpSchedForVcs (SippPipeline *pl);
void sippDbgDumpSchedForVcsCArr (SippPipeline *pl);
void sippDbgDumpGraph (SippPipeline *pl, const char *fname);
void sippDbgCreateDumpFiles (SippPipeline *pl);
void sippDbgDumpFilterOuts (SippPipeline *pl);
void sippDbgDumpAsmOffsets (SippPipeline *pl);
void sippDumpHtmlMap (SippPipeline * pl);
void sippPrintSliceWidth (SippPipeline *pl);

void sippErrorInit (void);
void sippAssert (u32 condition, u32 errCode);

//////////////////////////////////////////////////////////////////////////
// Schedule calculation
u32  sippGenericSchedule (SippPipeline * pl, bool allocMem, bool reschedPipe);
void sippGenericScheduleSetBufConsModels (pSippPipeline pipeLine);

#ifdef SIPP_SCRT_ENABLE_MLPI
u32  sippMLPISchedule (SippPipeline * pl, bool allocMem, bool reschedPipe);
void sippMLPIScheduleSetBufConsModels (pSippPipeline pipeLine);
#endif

#ifdef SIPP_SCRT_ENABLE_OPIPE
u32  sippOPipeSchedule (SippPipeline * pl, bool allocMem, bool reschedPipe);
void sippOPipeScheduleSetBufConsModels (pSippPipeline pipeLine);
#endif

float sippGetCoord2 (s32 in, float factor, float centreIn, float centreOut);

//////////////////////////////////////////////////////////////////////////
//Schedule helpers
#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
u8 askRegular (SippSchEnt * pSE, u32 iBufId, s32 lineRunNum, u32 linesPerIter);
u8 askResizer (SippSchEnt * pSE, u32 iBufId, s32 lineRunNum, u32 linesPerIter);
u8 askPolyFirResizer (SippSchEnt * pSE, u32 iBufId, s32 lineRunNum, u32 linesPerIter);
u8 askCrop (SippSchEnt * pSE, u32 iBufId, s32 lineRunNum, u32 linesPerIter);
u8 askHwDebayer (SippSchEnt * pSE, u32 iBufId, s32 lineRunNum, u32 linesPerIter);
u8 askHwColorCombChroma (SippSchEnt * pSE, u32 iBufId, s32 lineRunNum, u32 linesPerIter);
u8 askChromaGenDownsizer (SippSchEnt * pSE, u32 iBufId, s32 lineRunNum, u32 linesPerIter);

u8 askRegularLatency (SippSchEnt * pSE, u32 iBufId, s32 lineRunNum, u32 linesPerIter);
u8 askCropLatency (SippSchEnt * pSE, u32 iBufId, s32 lineRunNum, u32 linesPerIter);
u8 askHwDebayerLatency (SippSchEnt * pSE, u32 iBufId, s32 lineRunNum, u32 linesPerIter);
u8 askHwMedLumaLatency (SippSchEnt * pSE, u32 iBufId, s32 lineRunNum, u32 linesPerIter);

void sippGenericColorCombChromaCheck (void * pPipe, void * pSE, void * pParam);
void sippGenericDMACheck (void * pPipe, void * pSE, void * pParam);

#ifdef SIPP_SCRT_ENABLE_MLPI
u8 mlpiAskRegular (SippSchEnt * pSE, u32 iBufId, s32 lineRunNum, u32 linesPerIter);
u8 mlpiAskCrop (SippSchEnt * pSE, u32 iBufId, s32 lineRunNum, u32 linesPerIter);
u8 mlpiAskHwColorCombChroma (SippSchEnt * pSE, u32 iBufId, s32 lineRunNum, u32 linesPerIter);
u8 mlpiAskResizer (SippSchEnt * pSE, u32 iBufId, s32 lineRunNum, u32 linesPerIter);
u8 mlpiAskPolyFirResizer (SippSchEnt * pSE, u32 iBufId, s32 lineRunNum, u32 linesPerIter);
u8 mlpiAskChromaGenDownsizer (SippSchEnt * pSE, u32 iBufId, s32 lineRunNum, u32 linesPerIter);

u8 mlpiAskRegularLatency (SippSchEnt * pSE, u32 iBufId, s32 lineRunNum, u32 linesPerIter);
u8 mlpiAskCropLatency (SippSchEnt * pSE, u32 iBufId, s32 lineRunNum, u32 linesPerIter);
u8 mlpiAskHwMedLumaLatency (SippSchEnt * pSE, u32 iBufId, s32 lineRunNum, u32 linesPerIter);

void sippMLPIDMACheck (void * pPipe, void * pSE, void * pParam);
#endif
#endif

//////////////////////////////////////////////////////////////////////////
//Schedule RD/WR routines (Only update & test in pair!)
void sippGenericSchedWr (SippPipeline * pl, u32 iteration);
#ifdef SIPP_SCRT_ENABLE_MLPI
void sippMLPISchedWr (SippPipeline * pl, u32 iteration);
#endif

//////////////////////////////////////////////////////////////////////////
// Schedule specific Debug
void sippGenericDbgPrintRunnable (SippSchEnt *SEs[], u32 nSE, u32 iteration);
void sippGenericDbgShowBufferReq (SippSchEnt *SEs[], u32 nSE);
void sippGenericDbgDumpBuffState (SippFilter *filters[], u32 nFilters, u32 iteration);
#ifdef SIPP_SCRT_ENABLE_MLPI
void sippMLPIDbgPrintRunnable (SippSchEnt *SEs[], u32 nSE, u32 iteration);
void sippMLPIDbgShowBufferReq (SippSchEnt *SEs[], u32 nSE);
void sippMLPIDbgDumpBuffState (SippFilter *filters[], u32 nFilters, u32 iteration);
#endif

//////////////////////////////////////////////////////////////////////////
//Context switch helpers
void ctxSwitchOnePar (SippFilter * fptr, bool bSave, u32 unitID);
void ctxSwitchTwoPar (SippFilter * fptr, bool bSave, u32 unitID);
void ctxSwitchChromaDns (SippFilter * fptr, bool bSave, u32 unitID);
void ctxSwitchLut (SippFilter * newF, SippFilter * oldF, u32 unitID);
void ctxSwitchPoly (SippFilter * fptr, bool bSave, u32 unitID);
void ctxSwitchColComb (SippFilter * newF, SippFilter * oldF, u32 unitID);
void ctxSwitchMipiRx (SippFilter * newF, SippFilter * oldF, u32 unitID);
void ctxSwitchMipiTx (SippFilter * fptr, bool bSave, u32 unitID);

void sippConfigSvus (SippPipeline * pl);
void sippDataSectAction (SippPipeline * pl, u64 action);
void sippChainDmaDesc (SippPipeline * pl);
void sippSetupSvus (SippPipeline * pl);
void sippInitSyncMutexes (SippPipeline * pl);
void sippKickSvus (SippPipeline * pl);

void sippGetCtxOrder (SippPipeline *pl);
void sippComputeHwCtxChg (SippPipeline *pl);
void sippHandleCtxSwitch (SippPipeline *pl, int doLoop);

//////////////////////////////////////////////////////////////////////////
// SIPP Runtime hook APIs

void sippGenericRuntimeFrameReset (pSippPipeline pPipe);
void sippGenericRuntimeClaimHWResource (pSippPipeline pPipe);
void sippGenericRuntime (pSippPipeline pPipe, eSIPP_ACCESS_SCHEDULER_EVENT eEvent, SIPP_ACCESS_SCHEDULER_EVENT_DATA pData);
void sippGenericRuntimeHWProcessIters (pSippPipeline pPipe, u32 numIters);
u32  sippGenericRunIterDone (pSippPipeline pPipe);
void sippGenericRunNextIter (pSippPipeline pPipe);
void sippGenericUpdateHWUnits2x5x (SippPipeline * pPipe);
void sippGenericStartUnits (SippPipeline *pl);
void sippGenericLinePrepare (SippPipeline *pl, int iteration);
void sippGenericUpdateExecNums (SippPipeline *pl);

#ifdef SIPP_SCRT_ENABLE_MLPI
void sippMLPIRuntimeFrameReset (pSippPipeline pPipe);
void sippMLPIRuntimeClaimHWResource (pSippPipeline pPipe);
void sippMLPIRuntime (pSippPipeline pPipe, eSIPP_ACCESS_SCHEDULER_EVENT eEvent, SIPP_ACCESS_SCHEDULER_EVENT_DATA pData);
void sippMLPIRuntimeHWProcessIters (pSippPipeline pPipe, u32 numIters);
u32  sippMLPIRunIterDone (pSippPipeline pPipe);
void sippMLPIRunNextIter (pSippPipeline pPipe);
void sippMLPIUpdateHWUnits2x5x (SippPipeline * pPipe);
void sippMLPIStartUnits (SippPipeline *pl);
void sippMLPILinePrepare (SippPipeline *pl, int iteration);
void sippMLPIUpdateExecNums (SippPipeline *pl);
void sippMLPIStartHWUnits2x5x (SippPipeline * pPipe);
void sippMLPIRunUpdateCQ (SippPipeline * pPipe);
void sippMLPIUpdateHWUnits2x5xCQ (SippPipeline * pPipe);
void sippMLPIStartHWUnits2x5xCQ (SippPipeline * pPipe, u32 iteration);
void sippMLPIRunStartCQ (SippPipeline * pPipe);
#endif

#ifdef SIPP_SCRT_ENABLE_OPIPE
void sippOPipeRuntimeFrameReset (pSippPipeline pPipe);
void sippOPipeRuntimeClaimHWResource (pSippPipeline pPipe);
void sippOPipeRuntime (pSippPipeline pPipe, eSIPP_ACCESS_SCHEDULER_EVENT eEvent, SIPP_ACCESS_SCHEDULER_EVENT_DATA pData);
u32  sippOPipeRunIterDone (pSippPipeline pPipe);
void sippOPipeRunNextIter (pSippPipeline pPipe);
#endif

//////////////////////////////////////////////////////////////////////////
// SIPP ISR

void sippIbflDecHandler (u32 irqSource);
void sippObflIncHandler (u32 irqSource);
void sippSvuDoneIrqHandler (u32 irqSource);
#ifdef USE_CMX_DMA_NEW_DRIVER
void sippCmxDmaDoneIrqHandler (bool bClear);
#else
void sippCmxDmaDoneIrqHandler (void);
#endif
#ifdef BUGZILLA_22709
void sippFrameDoneIrqHandler (u32 irqSource);
#endif

void sippCheckIterComplete (SippPipeline * pPipe, u32 Flag);
void sippIsrSetup (void);
void sippIntBarrierSetup (u32 useIntBar);

#ifdef SIPP_SCRT_ENABLE_OPIPE
u32  sippOpipeHandleIbflDec (SippPipeline * pPipe, u32 status);
u32  sippOpipeHandleObflInc (SippPipeline * pPipe, u32 status, u32 eof);
void sippOPipeEofIsr (SippPipeline * pPipe, u32 status);
#endif

//////////////////////////////////////////////////////////////////////////
// SIPP ISR

#if SIPP_RTOS != SIPP_NO_RTOS
bool sippThreadCreate (void);
#endif

#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)

//////////////////////////////////////////////////////////////////
//  2x5x SPECIFIC

//////////////////////////////////////////
//HW Filter Inits
u32 sippInitSigma (SippFilter *fptr);
u32 sippInitLsc (SippFilter *fptr);
u32 sippInitRaw (SippFilter *fptr);
u32 sippInitDbyr (SippFilter *fptr);
u32 sippInitDogLtm (SippFilter *fptr);
u32 sippInitLumaDns (SippFilter *fptr);
u32 sippInitSharpen (SippFilter *fptr);
u32 sippInitGenChr (SippFilter *fptr);
u32 sippInitMed (SippFilter *fptr);
u32 sippInitChrDns (SippFilter *fptr);
u32 sippInitColComb (SippFilter *fptr);
u32 sippInitLut (SippFilter *fptr);
u32 sippInitEdgeOp (SippFilter *fptr);
u32 sippInitConv (SippFilter *fptr);
u32 sippInitHarris (SippFilter *fptr);
u32 sippInitPolyFir (SippFilter *fptr);
u32 sippInitMipiTx (SippFilter *fptr);
u32 sippInitMipiRx (SippFilter *fptr);

//////////////////////////////////////////
//HW Filter loads
void sippLoadSigma (SippFilter *fptr);
void sippLoadLsc (SippFilter *fptr);
void sippLoadRaw (SippFilter *fptr);
void sippLoadDbyr (SippFilter *fptr);
void sippLoadDogLtm (SippFilter *fptr);
void sippLoadLumaDns (SippFilter *fptr);
void sippLoadSharpen (SippFilter *fptr);
void sippLoadGenChr (SippFilter *fptr);
void sippLoadMed (SippFilter *fptr);
void sippLoadChrDns (SippFilter *fptr);
void sippLoadColComb (SippFilter *fptr);
void sippLoadLut (SippFilter *fptr);
void sippLoadEdgeOp (SippFilter *fptr);
void sippLoadConv (SippFilter *fptr);
void sippLoadHarris (SippFilter *fptr);
void sippLoadPolyFir0 (SippFilter *fptr);
void sippLoadPolyFir1 (SippFilter *fptr);
void sippLoadPolyFir2 (SippFilter *fptr);
void sippLoadMipiTx (SippFilter *fptr);
void sippLoadMipiRx (SippFilter *fptr);

//////////////////////////////////////////
// Input Ctx reg access
u32 sippGetIBufCtxSigma (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufCtxLsc (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufCtxRaw (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufCtxChrDns (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufCtxLumDns (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufCtxPolyFir0 (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufCtxPolyFir1 (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufCtxPolyFir2 (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufCtxConv (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufCtxHarris (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufCtxEdgeOp (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufCtxDbyr (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufCtxMed (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufCtxLut (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufCtxGenChroma (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufCtxDoGLTM (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufCtxSharpen (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufCtxColComb (SippFilter *fptr, u32 iBufIdx);

//////////////////////////////////////////
// Buffer id retrieval
u32 sippGetIBufIdsSigma (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufIdsLsc (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufIdsRaw (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufIdsChrDns (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufIdsLumDns (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufIdsPolyFir0 (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufIdsPolyFir1 (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufIdsPolyFir2 (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufIdsConv (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufIdsHarris (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufIdsEdgeOp (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufIdsDbyr (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufIdsMed (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufIdsLut (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufIdsGenChroma (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufIdsDoGLTM (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufIdsSharpen (SippFilter *fptr, u32 iBufIdx);
u32 sippGetIBufIdsColComb (SippFilter *fptr, u32 iBufIdx);

u32 sippGetOBufIdsSigma (SippFilter *fptr, u32 oBufIdx);
u32 sippGetOBufIdsLsc (SippFilter *fptr, u32 oBufIdx);
u32 sippGetOBufIdsRaw (SippFilter *fptr, u32 oBufIdx);
u32 sippGetOBufIdsChrDns (SippFilter *fptr, u32 oBufIdx);
u32 sippGetOBufIdsLumDns (SippFilter *fptr, u32 oBufIdx);
u32 sippGetOBufIdsPolyFir0 (SippFilter *fptr, u32 oBufIdx);
u32 sippGetOBufIdsPolyFir1 (SippFilter *fptr, u32 oBufIdx);
u32 sippGetOBufIdsPolyFir2 (SippFilter *fptr, u32 oBufIdx);
u32 sippGetOBufIdsConv (SippFilter *fptr, u32 oBufIdx);
u32 sippGetOBufIdsHarris (SippFilter *fptr, u32 oBufIdx);
u32 sippGetOBufIdsEdgeOp (SippFilter *fptr, u32 oBufIdx);
u32 sippGetOBufIdsDbyr (SippFilter *fptr, u32 oBufIdx);
u32 sippGetOBufIdsMed (SippFilter *fptr, u32 oBufIdx);
u32 sippGetOBufIdsLut (SippFilter *fptr, u32 oBufIdx);
u32 sippGetOBufIdsGenChroma (SippFilter *fptr, u32 oBufIdx);
u32 sippGetOBufIdsDoGLTM (SippFilter *fptr, u32 oBufIdx);
u32 sippGetOBufIdsSharpen (SippFilter *fptr, u32 oBufIdx);
u32 sippGetOBufIdsColComb (SippFilter *fptr, u32 oBufIdx);
u32 sippGetOBufIdsMipiTx0 (SippFilter *fptr, u32 oBufIdx);
u32 sippGetOBufIdsMipiTx1 (SippFilter *fptr, u32 oBufIdx);
u32 sippGetOBufIdsMipiRx0 (SippFilter *fptr, u32 oBufIdx);
u32 sippGetOBufIdsMipiRx1 (SippFilter *fptr, u32 oBufIdx);
u32 sippGetOBufIdsMipiRx2 (SippFilter *fptr, u32 oBufIdx);
u32 sippGetOBufIdsMipiRx3 (SippFilter *fptr, u32 oBufIdx);

//////////////////////////////////////////
// Buffer latencies setup
void sippSetBufLatenciesSigma (SippFilter *fptr);
void sippSetBufLatenciesLsc (SippFilter *fptr);
void sippSetBufLatenciesRaw (SippFilter *fptr);
void sippSetBufLatenciesDbyr (SippFilter *fptr);
void sippSetBufLatenciesDoGLTM (SippFilter *fptr);
void sippSetBufLatenciesLuma (SippFilter *fptr);
void sippSetBufLatenciesSharpen (SippFilter *fptr);
void sippSetBufLatenciesGenChroma (SippFilter *fptr);
void sippSetBufLatenciesMedian (SippFilter *fptr);
void sippSetBufLatenciesChrDns (SippFilter *fptr);
void sippSetBufLatenciesColourComb (SippFilter *fptr);
void sippSetBufLatenciesLut (SippFilter *fptr);
void sippSetBufLatenciesPoly (SippFilter *fptr);
void sippSetBufLatenciesConv (SippFilter *fptr);
void sippSetBufLatenciesHarris (SippFilter *fptr);
void sippSetBufLatenciesEdgeOp (SippFilter *fptr);
void sippSetBufLatenciesMipiRx (SippFilter *fptr);
void sippSetBufLatenciesMipiTx (SippFilter *fptr);


//////////////////////////////////////////
// Buffer fill levels setup
void sippSetOBufLevelsSigma (SippFilter *fptr, eSippObufControl eOBufSetting);
void sippSetOBufLevelsLsc (SippFilter *fptr, eSippObufControl eOBufSetting);
void sippSetOBufLevelsRaw (SippFilter *fptr, eSippObufControl eOBufSetting);
void sippSetOBufLevelsDbyr (SippFilter *fptr, eSippObufControl eOBufSetting);
void sippSetOBufLevelsDoGLTM (SippFilter *fptr, eSippObufControl eOBufSetting);
void sippSetOBufLevelsLuma (SippFilter *fptr, eSippObufControl eOBufSetting);
void sippSetOBufLevelsSharpen (SippFilter *fptr, eSippObufControl eOBufSetting);
void sippSetOBufLevelsGenChroma (SippFilter *fptr, eSippObufControl eOBufSetting);
void sippSetOBufLevelsMedian (SippFilter *fptr, eSippObufControl eOBufSetting);
void sippSetOBufLevelsChrDns (SippFilter *fptr, eSippObufControl eOBufSetting);
void sippSetOBufLevelsColourComb (SippFilter *fptr, eSippObufControl eOBufSetting);
void sippSetOBufLevelsLut (SippFilter *fptr, eSippObufControl eOBufSetting);
void sippSetOBufLevelsConv (SippFilter *fptr, eSippObufControl eOBufSetting);
void sippSetOBufLevelsHarris (SippFilter *fptr, eSippObufControl eOBufSetting);
void sippSetOBufLevelsEdgeOp (SippFilter *fptr, eSippObufControl eOBufSetting);
void sippSetOBufLevelsPoly0 (SippFilter *fptr, eSippObufControl eOBufSetting);
void sippSetOBufLevelsPoly1 (SippFilter *fptr, eSippObufControl eOBufSetting);
void sippSetOBufLevelsPoly2 (SippFilter *fptr, eSippObufControl eOBufSetting);
void sippSetOBufLevelsMipiTx0 (SippFilter *fptr, eSippObufControl eOBufSetting);
void sippSetOBufLevelsMipiTx1 (SippFilter *fptr, eSippObufControl eOBufSetting);
void sippSetOBufLevelsMipiRx0 (SippFilter *fptr, eSippObufControl eOBufSetting);
void sippSetOBufLevelsMipiRx1 (SippFilter *fptr, eSippObufControl eOBufSetting);
void sippSetOBufLevelsMipiRx2 (SippFilter *fptr, eSippObufControl eOBufSetting);
void sippSetOBufLevelsMipiRx3 (SippFilter *fptr, eSippObufControl eOBufSetting);

//////////////////////////////////////////
// Filter setup updates
void sippSetupUpdateMedian (SippFilter *fptr);
void sippSetupUpdateDoGLTM (SippFilter *fptr);

// End of 2x5x SPECIFIC
////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////////////////////
// Memory allocation
void * sippMemAlloc (ptSippMCB pSippMCB, SippVirtualPool vPool, s32 n_bytes);
void   sippInitLnMemPool (ptSippMCB ptMCB, u8 * start);
void   sippInitPhysicalPoolGlobal (void);
void   sippMemInitVirtPhysMaps (void);
s32    sippAllocCmxLineBuffers (SippPipeline * pipe);
s32    sippAllocCmxLineBuffersOPipe (SippPipeline * pipe);
u8     sippMemLBMatchRegionsToChunks (pSippCmxBufferMap pCmxMap, u32 chunkStride, u32 numChunks);
void   sippMemLBConsolidateRegions (pSippCmxBufferMap pCmxMap);
void   sippInitSchedPool (ptSippMCB ptMCB, u32 sliceFirst, u32 sliceLast);
void   sippMemFreeList (ptSippMCB pSippMCB, SippVirtualPool vPool);
void   sippMemFree (ptSippMCB pSippMCB, SippVirtualPool vPool, void * pPtr);
u32    sippInitPhysicalPoolPipe (ptSippMCB ptMCB);
u32    sippInitLnMemPoolSlices (ptSippMCB ptMCB, u32 firstSlice, u32 lastSlice);
SippVirtualPool sippMemFindMaxLnMemPoolFree (ptSippMCB pSippMCB);

//////////////////////////////////////////////////////////////////////////////////////////
// HW buffer control
u32  sippIbufSetup (SippFilter * fptr, u32 parNo);
u32  sippObufSetup (SippFilter * fptr, u32 oBufIdx);
u32  sippBufSetupIrqRate (SippHwBuf * buf, u32 oBuf, u32 bufId, u32 numLinesPerIrq);
u32  sippBufSetupIrqRateCQ (SippHwBuf * buf, u32 oBuf, u32 bufId, u32 numLinesPerIrq, psSippCMDQu pQu);
u32  sippBufGetObufCtx (SippFilter * fptr, u32 oBufIdx);

//////////////////////////////////////////////////////////////////////////////////////////
// Cmx Dma usage
void sippCmxDmaInit (void);
u32  sippInitDma (SippFilter *fptr);
void sippKickDma (SippPipeline *pl);
u32  sippWaitDma (void);
void sippKickDmaCQ (SippPipeline * pl, u32 iteration);
void sippRunDmaCQ (SippPipeline * pl);
u32  sippDmaCQInit (SippPipeline * pPipe);
void topLevelCmxDmaIrqHandler (u32 irqSource);
void sippCmxDmaInitAsync (void);

#if defined(MYRIAD2) || defined(SIPP_PC)
void dmaKickSequence (SippPipeline * pl);
#endif
void dmaKickSequenceConcurrent (SippPipeline * pl);

//////////////////////////////////////////////////////////////////////////////////////////
// Cmx Dma Driver usage
#ifdef SIPP_USE_CMXDMA_DRIVER
void sippDrvCmxDmaInit (void);
void sippDrvKickDma (SippPipeline *pl);
u32 sippInitDrvDesc (SippPipeline * pl);

#if defined(MYRIAD2) || defined(SIPP_PC)
void dmaDrvKickSequence (SippPipeline * pl);
#endif
void dmaDrvKickSequenceConcurrent (SippPipeline * pl);
#endif

//////////////////////////////////////////////////////////////////////////////////////////
// Shave usage
void sippKickShaveM1PC (SippPipeline *pl);
void sippWaitShave (SippPipeline *pl);
void sippStopSvus     (SippPipeline *pl);

extern SippPipeline * SVU_SYM(sipp_pl);

//////////////////////////////////////////////////////////////////////////////////////////
// SIPP Heap management
tsSippHeap * sippHeapCreate (u8 * sippHeap_start, u32 sippHeap_size);
void * sippAlloc (tsSippHeap * pSippHeap, void * pPrev, u32 size);
void sippFreeList (tsSippHeap * pSippHeap, void * pStartPtr);
void sippFree (tsSippHeap * pSippHeap, void * pPtr);
u32  sippHeapCheck (tsSippHeap * pSippHeap);

#ifdef RT_DBG
#define SIPP_DEBUG_HISTORY_INDEX  1024
extern u32 sippDebugHistory[SIPP_DEBUG_HISTORY_INDEX];
extern u32 sippDebugHistoryIdx;
extern u32 sippPipeHistory0[SIPP_DEBUG_HISTORY_INDEX];
extern u32 sippPipeHistory1[SIPP_DEBUG_HISTORY_INDEX];
extern u32 sippPipeHistory2[SIPP_DEBUG_HISTORY_INDEX];
extern u32 sippPipeHistory3[SIPP_DEBUG_HISTORY_INDEX];
extern u32 sippPipeHistoryIdx[SIPP_MAX_SUPPORTED_PIPELINES];

#define SIPP_HISTORY_LOG(x) {\
                                sippDebugHistory[sippDebugHistoryIdx++] = x;\
                                if (sippDebugHistoryIdx == SIPP_DEBUG_HISTORY_INDEX) sippDebugHistoryIdx = 0;\
                            }
#define SIPP_PIPE_LOG(x,pl) {\
                                u32 idx = sippHWGetPipeIdx (pl);\
                                switch (idx)\
                                {\
                                    case 0 : { \
                                                  sippPipeHistory0[sippPipeHistoryIdx[idx]++] = x;\
                                                  if (sippPipeHistoryIdx[idx] == SIPP_DEBUG_HISTORY_INDEX) sippPipeHistoryIdx[idx] = 0;\
                                             }; break; \
                                    case 1 : { \
                                                  sippPipeHistory1[sippPipeHistoryIdx[idx]++] = x;\
                                                  if (sippPipeHistoryIdx[idx] == SIPP_DEBUG_HISTORY_INDEX) sippPipeHistoryIdx[idx] = 0;\
                                             }; break; \
                                    case 2 : { \
                                                  sippPipeHistory2[sippPipeHistoryIdx[idx]++] = x;\
                                                  if (sippPipeHistoryIdx[idx] == SIPP_DEBUG_HISTORY_INDEX) sippPipeHistoryIdx[idx] = 0;\
                                             }; break; \
                                    case 3 : { \
                                                  sippPipeHistory3[sippPipeHistoryIdx[idx]++] = x;\
                                                  if (sippPipeHistoryIdx[idx] == SIPP_DEBUG_HISTORY_INDEX) sippPipeHistoryIdx[idx] = 0;\
                                             }; break; \
                                }\
                            }

#endif // RT_DBG
#endif

// End of file
