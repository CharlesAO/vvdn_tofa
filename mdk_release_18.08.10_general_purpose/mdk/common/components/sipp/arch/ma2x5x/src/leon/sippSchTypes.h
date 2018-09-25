// -----------------------------------------------------------------------------
// Copyright (C) 2015 Movidius Ltd. All rights reserved
//
// Company          : Movidius
// Description      : Private header file for Sched types
//
// -----------------------------------------------------------------------------

#ifndef _SIPP_SCH_TYPES_H_
#define _SIPP_SCH_TYPES_H_

// This deliberately does not use other sipp types (eg sippFilter) -
// Schedulers need not be tied to scheduling anything in particular - they must be capable of scheduling any
// entity which may be described in terms of how/when it consumes data from input buffers and how/when
// it writes the data to its output buffers

//////////////////////////////////////////////////////////
// Header Files
/////////////////////////////////////////////////////////

#include <sippCfg.h>
#include <sippTypes.h>

///////////////////////////////////////////////////////////////////////////////
// RunStatus
//
// Scheduling status enum

typedef enum
{
    RS_DONT_KNOW = 0,
    RS_CAN_RUN   = 1,
    RS_CANNOT    = 2

} RunStatus;

///////////////////////////////////////////////////////////////////////////////
// sSchBuffer
//
// Buffer representation

struct sSchLineBufferS
{
    u8  latency;                        // The production latency - when was this supposed to be filled in?
                                        // it needs to be left until the scheduling entities have been created
                                        //
    u16 internalFillLevel;
    s32 mostRecentLine;
    u32 filtOutHeight;
    u32 bottomLineReplication;
    u32 numLines;
    u8  allocReq;
    u8  manageReq;                       // Managing is not needed in oPipe direct streaming
    u8  numSWConsumers;
    u8  hPadding;
    u32 hwOutputBufId;                   // These are really more runtime elements
    u32 hwInputBufId;                    // However this is a convenient place to store them for now

    #ifdef SIPP_SCRT_ENABLE_OPIPE
    pSippOPipeBuf pManagedBuf;
    #endif
};

typedef void (*SchedAddCheck) (void * pPipe, void * pSE, void * pParam);

///////////////////////////////////////////////////////////////////////////////
// SippSchEntS
//
// Additional data structure required ONLY for scheduling & full-frame test
// for a scheduling entity

struct SippSchEntS
{
    psSchLineBuffer      oBufs[SIPP_SE_MAX_OBUFS];
    psSchLineBuffer      iBufs[SIPP_SE_MAX_IBUFS];
    SchedIBufUsageInfo * iBufCtx[SIPP_SE_MAX_IBUFS];
    u32                  numOBufs;
    u32                  numIBufs;

    SippSchEnt *         cons[SIPP_SE_MAX_CONSUMERS];
    u32                  consIbufIdx[SIPP_SE_MAX_CONSUMERS];
    u32                  numConsumers;

    u32                  maxLineRequired[SIPP_SE_MAX_IBUFS];
    u32                  minLineRequired[SIPP_SE_MAX_IBUFS];
    u8                   runPadCheck[SIPP_SE_MAX_IBUFS];

    s32                  parentKS[SIPP_SE_MAX_IBUFS];
    u32 *                ptrFiltKS[SIPP_SE_MAX_IBUFS];
    s32                  parentKSMin[SIPP_SE_MAX_IBUFS];

    RunStatus            canRunP;      // Can run due to Parent conditions
    RunStatus            canRunC;      // Can run due to Consumer conditions
    RunStatus            canConsume;   // Should consume the input while producing no output (resizers)
    s32                  dbgJustRoll;
    s32                  numLineRuns;  // This is the current iteration number within the scheduler - NOT necessarily the output line number if any OBufs of the SE have latency
    u32                  maxOutputH;
    u32                  linesPerIter; // Number of lines of processing to be carried out on every schedule iteration

    u8                   recordParentKS[SIPP_SE_MAX_IBUFS];

    s16 *                currKSOffset[SIPP_SE_MAX_IBUFS]; // Used with resizers and multiple lines per iteration
    u16 *                KSIterList[SIPP_SE_MAX_IBUFS];   // Used with resizers and multiple lines per iteration - assigned from the filter
                                                          // Should be sized as the next integer multiple of lines per iteration greater than or equal to pSchEnt->maxOutputH
                                                          // so should allocate once we know this value
    SchedAddCheck        pfRunAdditionalCheck;
    void *               pRunAdditionalParam;

    u8                   pipeSEId;
};

///////////////////////////////////////////////////////////////////////////////
// SippFilterSchedInfo
//
// Additional data structure required ONLY for scheduling & full-frame test
// for a scheduling entity

struct SippFilterSchedInfoS
{
    bool parentLatenciesKnown;
    bool latenciesKnown;
    u8   subSampleScale;
    u8   nlinesUsedParent[SIPP_SE_MAX_IBUFS];
    u8   oBufLatency[SIPP_FILTER_MAX_OBUFS];
    bool hasBuffers;
};

//////////////////////////////////////////////////////////////////////////////
// SippManagedBufSchedInfo
//
// Additional data structure required ONLY for scheduling & full-frame test
// for a scheduling entity

typedef struct
{
    u32 placeHolder;

} SippManagedBufSchedInfo, *pSippManagedBufSchedInfo;

#endif // _SIPP_SCH_TYPES_H_

