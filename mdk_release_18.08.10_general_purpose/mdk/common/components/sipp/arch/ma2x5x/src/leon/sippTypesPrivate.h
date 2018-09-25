// -----------------------------------------------------------------------------
// Copyright (C) 2015 Movidius Ltd. All rights reserved
//
// Company          : Movidius
// Description      : Private header file for internal SIPP Framework types
//
// -----------------------------------------------------------------------------

#ifndef _SIPP_TYPES_PRIVATE_H_
#define _SIPP_TYPES_PRIVATE_H_

//////////////////////////////////////////////////////////
// Header Files
/////////////////////////////////////////////////////////

#include <sippCfg.h>
#include <sippTypes.h>
#include <sippAccessSchedulerTypes.h>
#include <sippSchTypes.h>
#include "DrvSippDefines.h"

//////////////////////////////////////////////////////////
// Private types
/////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
// eSippPipeState

typedef enum
{
    eSIPP_PIPE_WAIT_INIT = 0x0,
    eSIPP_PIPE_WAIT_FINALISE,
    eSIPP_PIPE_ACTIVE,
    eSIPP_PIPE_END_SESSION

} eSippPipeState;

//////////////////////////////////////////////////////////
// eSippObufCOntrol

typedef enum
{
    eSIPP_SET_OBUF_SPACE_FULL = 0x0,
    eSIPP_SET_OBUF_SPACE_EMPTY,
    eSIPP_SET_OBUF_SPACE_ITER

} eSippObufControl;

//////////////////////////////////////////////////////////
// tSippPipelineSuper

typedef struct
{
    // Data
    SippPipeline   tPublicPipe;
    u32            uPipeIdx;          // gives its index in tSippFramework.pPipelines[] for convenience
    eSippPipeState eState;
    bool           bSVUOnly;
    u32            uHWPipeID;

} tSippPipelineSuper, *ptSippPipelineSuper;

//////////////////////////////////////////////////////////
// tSippFramework

typedef struct
{
    bool                bInit;
    ptSippPipelineSuper pPipelines[SIPP_MAX_SUPPORTED_PIPELINES];
    u32                 dynIrqSipp0;
    u32                 dynIrqSipp1;
    u32                 dynIrqSipp2;

} tSippFramework;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Context switch...
typedef u32  (*FnHwFltInit)(SippFilter *fptr);
typedef void (*FnHwFltLoad)(SippFilter *fptr);
typedef u32  (*FnGetIBufCtx)(SippFilter *fptr, u32 bufIdx);
typedef u32  (*FnGetIBufIds)(SippFilter *fptr, u32 bufIdx);
typedef u32  (*FnGetOBufIds)(SippFilter *fptr, u32 bufIdx);
typedef u32  (*FnCheckOpipeCons)(SippFilter * fptr, SippFilter *parfptr, u32 * parOBufIdx);
typedef void (*FnHwSetObufLatencies)(SippFilter *fptr);
typedef void (*FnSetOBufs)(SippFilter *fptr, eSippObufControl eOBufSetting);
typedef void (*FnHwSetupUpdate)(SippFilter *fptr);

////////////////////////////////////////////////////////
//Constants shared by all pipelines

//Typical HW-SIPP unit helpers
typedef struct
{
    FnHwFltInit hwFnInit;  //internal params packaging
    FnHwFltLoad hwFnLoad;  //load filter in certain context

    u32         paramSz;   //sw param size

    #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    FnSetOBufs           hwFnSetOBufs;  // dunno if we really need this as part of a structure...
    u32                  iFcAddr;
    FnGetIBufCtx         hwGetIbufCtx;
    FnGetIBufIds         hwGetIbufIds;
    FnGetOBufIds         hwGetObufIds;
    FnCheckOpipeCons     hwCheckOpipeCons;
    FnHwSetObufLatencies hwSetObufLatencies;
    FnHwSetupUpdate      hwFilterSetupUpdate;
    #endif

} UnitInfo;

typedef struct
{
    UnitInfo uInfo[EXE_NUM];

} SippGlobals;

extern SippGlobals gSipp;

////////////////////////////////////////////////////////
// Input & Output buffer data structure that maps on HW register lists
// Some HW filters have mutliple input or Output buffers...
struct SippHwBufS
{
    u32 base;     //+0x0000 : base address
    u32 cfg;      //+0x0004 : configuration
    u32 ls;       //+0x0008 : line stride
    u32 ps;       //+0x000C : plane stride
    u32 irqRate;  //+0x0010 : fields for Chunk-size !
    u32 fillCtrl; //+0x0014 : unused in sync mode
    u32 ctx;      //+0x0018 : context/status
};

typedef enum
{
    eSIPP_CMD_INIT_HW = 0x0,
    eSIPP_CMD_FINALISE_PIPE_SW,
    eSIPP_CMD_RESCHEDULE_PIPE_SW,
    eSIPP_CMD_DELETE_PIPE_SW,
    eSIPP_CMD_PROCESS_ITERS_HW

    /* Need to add the SW commands also */

} eSippCommand;

typedef void * SippCommandData;

//////////////////////////////////////////////////////////////
// SIPP HW Session State

typedef enum
{
    eSIPP_HW_INACTIVE  = 0,
    eSIPP_HW_ACTIVE

} eSIPP_HW_STATE;

//////////////////////////////////////////////////////////////
// SIPP HW Session Context Info

typedef struct
{
    eSIPP_HW_STATE      eState;

    // Hardware capablilities
    u32                 uHWFeatures;

    pSippPipeline       pSippPrevSvuPipe[SIPP_NUM_SVUS];
    pSippPipeline       pSippCurrSvuPipe[SIPP_NUM_SVUS];
    pSippPipeline       pSippLoadedHWPipe[SIPP_MAX_SUPPORTED_HW_PIPELINES];
    pSippPipeline       pSippCurrHWPipe[SIPP_MAX_SUPPORTED_HW_PIPELINES];

    u32                 uHWFilterUsageBitMask[SIPP_HW_FILTER_MASK_SIZE];

    u32                 uNumCurrHwPipes;
    u32                 useIntBar[SIPP_MAX_SUPPORTED_HW_PIPELINES];

    #ifdef SIPP_NO_IRQ_BARRIER2
    u32                 sippHWEOFMask[SIPP_MAX_SUPPORTED_HW_PIPELINES];
    #endif

    u32                 isrSetup;               // 0: default for scheduled pipes, 1: supports oPipe
    u32                 noIntBarSwitchPend;     // 0: default for scheduled pipes, 1: supports oPipe

} SIPP_HW_SESSION, * pSIPP_HW_SESSION;

////////////////////////////////////////////////////////
// CMX line buffer region list entry
typedef struct
{
    void * pNext;
    void * pNextChunkReg;
    u8     chainStart;
    u8     chainLinked;
    u32    regionAddr;
    u32    slice0Addr;
    u32    regionSize;
    u32    regionUsed;
    u32 *  regionUsedPtr;

} SippMemRegionListNode, *pSippMemRegionListNode;

////////////////////////////////////////////////////////
// CMX line buffer allocation map
struct SippCmxBufferMapS
{
    u32                    cmxSliceUsageBitMask;
    u32                    numCmxSlicesAvail;
    u32                    totalMem;
    pSippMemRegionListNode pCmxSliceRegionList[CMX_NSLICES];

};

typedef struct
{
    u32 cmxRegionIdx;
    u32 usedCount;
    u8  fullCmxSlice;

} cmxRegUsage;

typedef struct
{
    u32 freeBase;
    u32 sizeRemaining;

} memRegDescriptor, *pmemRegDescriptor;

////////////////////////////////////////////////////////
// Scheduling entity usage info for a Buffer

struct sSchedIBufUsageInfoS
{
    u32          consumptionLatency;
    u32          runFullSearch;
    // function pointer to a func ask
    SchedFuncAsk funcAsk;         // Scheduling & test purposes - only needed for input buffers
    u32          lastAskLineNo;
    u8           numLinesUsed;
    u8           hKerSz;
    u8           iPadLines;
    u32          outputHeight;
    void *       params;
    u8           KSDelta;        // For non resize filters, specifies the distance between kernel starts
                                 // for the first and last line of the iteration
};

////////////////////////////////////////////////////////
// OPipe Scheduling Entity

struct SippOseS
{
    SippFilter *         filts[SIPP_MAX_FILTS_OSE];
    psSchLineBuffer      oBufs[SIPP_SE_MAX_OBUFS];
    psSchLineBuffer      iBufs[SIPP_SE_MAX_IBUFS];
    SchedIBufUsageInfo * iBufCtx[SIPP_SE_MAX_IBUFS];
    u32                  numOBufs;
    u32                  numIBufs;
    u32                  linesPerIter;

    u32 *                ptrFiltKS[SIPP_SE_MAX_IBUFS];

    SippSchEnt **        consSE[SIPP_MAX_FILTS_OSE];
    u32                  consIbufIdx[SIPP_SE_MAX_CONSUMERS];
    u32                  numConsumers;

    u32                  numFilts;
    u32                  OSEFiltCfg;
    u32                  flags;

    SippSchEnt *         sch;
};

////////////////////////////////////////////////////////
// Sipp Heap

typedef struct
{
   u8  * sippHeap_start;
   u8  * sippHeap_end;
   u32   sippHeap_size;
   u8    assigned;
   u8    freeCounter;
   u8  * freePtr;

} tsSippHeap;

////////////////////////////////////////////////////////
// Sipp Heap Control Block

typedef struct
{
    #ifdef RT_DBG
    u32    magic;
    #endif
    u32    used_size; // [31]: `used` bit; [30:0]: size (incl header)
    void * pNext;
    #ifdef RT_DBG
    u32    PAD;
    #endif
    u32    data[];

} SippHeapCB, *pSippHeapCB;

#ifdef RT_DBG
#define SIPP_HCB_SIZE 16
#else
#define SIPP_HCB_SIZE 8
#endif

////////////////////////////////////////////////////////
// Sipp Physical pool descriptor

typedef struct
{
    const char * name;  // only for debug...
    u8         * start; // start addr
    u8         * pos;   // current position
    u8         * end;   // end addr
    tsSippHeap * pHeap; // Used if pool managed as a heap

} tSippPhysicalPool, * ptSippPhysicalPool;

////////////////////////////////////////////////////////
// Sipp Pipeline Virtual to Physical Mapping

struct SippVPhysMapS
{
    ptSippPhysicalPool physPoolMap[vPoolLast];
};

////////////////////////////////////////////////////////
// MLPI Start SW CQ backup struct

typedef struct
{
    SippFilter * firstRunFilts[SIPP_MAX_FILTERS_PER_PIPELINE];
    u32          nFirstRunFilts;
    u32          firstUseInProg;

} tMLPIStartCQCtrl, *ptMLPIStartCQCtrl;

////////////////////////////////////////////////////////
// SIPP oPipe Runtime output buffer info

typedef struct
{
    SippFilter * ptrFilt;
    u32          oBufIdx;

} sippOpipeBufInfo;


#endif // _SIPP_TYPES_PRIVATE_H_

