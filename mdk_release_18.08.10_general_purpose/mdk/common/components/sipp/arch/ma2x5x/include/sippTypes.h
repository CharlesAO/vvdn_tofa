// -----------------------------------------------------------------------------
// Copyright (C) 2015 Movidius Ltd. All rights reserved
//
// Company          : Movidius
// Description      : Public header file for exported SIPP Framework types
//
// Notes            : #ifdefs in this file must be reflected in LEON and SHAVE
//                    compiler options
// -----------------------------------------------------------------------------

#ifndef _SIPP_TYPES_H_
#define _SIPP_TYPES_H_

#include <sippBaseTypes.h>
#include <sippAccessSchedulerTypes.h>
#include <sippEvents.h>
#include <mv_types.h>
#if defined(__sparc)
#include <DrvCmxDma.h>
#ifdef USE_CMX_DMA_NEW_DRIVER
#include <DrvCdma.h>
#endif
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

//////////////////////////////////////////////////////////////
// SIPP STATUS enumeration

// TBD - How many of this do I need?

typedef enum
{
    /*   0  0x00  */   eSIPP_STATUS_OK = 0,
    /*   1  0x01  */   eSIPP_STATUS_ALREADY_INIT,
    /*   2  0x02  */   eSIPP_STATUS_NOT_INIT,
    /*   3  0x03  */   eSIPP_STATUS_INTERNAL_ERROR,
    /*   4  0x04  */   eSIPP_STATUS_BAD_HANDLE,
    /*   5  0x05  */   eSIPP_STATUS_BAD_PARAMETER,
    /*   6  0x06  */   eSIPP_STATUS_BAD_LENGTH,
    /*   7  0x07  */   eSIPP_STATUS_BAD_UNIT,
    /*   8  0x08  */   eSIPP_STATUS_RESOURCE_ERROR,
    /*   9  0x09  */   eSIPP_STATUS_CLOSED_HANDLE,
    /*  10  0x0A  */   eSIPP_STATUS_TIMEOUT,
    /*  11  0x0B  */   eSIPP_STATUS_NOT_ATTACHED,
    /*  12  0x0C  */   eSIPP_STATUS_NOT_SUPPORTED,
    /*  13  0x0D  */   eSIPP_STATUS_REOPENED_HANDLE,
    /*  14  0x0E  */   eSIPP_STATUS_INVALID,
    /*  15  0x0F  */   eSIPP_STATUS_DESTROYED,
    /*  16  0x10  */   eSIPP_STATUS_DISCONNECTED,
    /*  17  0x11  */   eSIPP_STATUS_BUSY,
    /*  18  0x12  */   eSIPP_STATUS_IN_USE,
    /*  19  0x13  */   eSIPP_STATUS_CANCELLED,
    /*  20  0x14  */   eSIPP_STATUS_UNDEFINED,
    /*  21  0x15  */   eSIPP_STATUS_UNKNOWN,
    /*  22  0x16  */   eSIPP_STATUS_NOT_FOUND,
    /*  23  0x17  */   eSIPP_STATUS_NOT_AVAILABLE,
    /*  24  0x18  */   eSIPP_STATUS_NOT_COMPATIBLE,
    /*  25  0x19  */   eSIPP_STATUS_NOT_IMPLEMENTED,
    /*  26  0x1A  */   eSIPP_STATUS_EMPTY,
    /*  27  0x1B  */   eSIPP_STATUS_FULL,
    /*  28  0x1C  */   eSIPP_STATUS_FAILURE,
    /*  29  0x1D  */   eSIPP_STATUS_ALREADY_ATTACHED,
    /*  30  0x1E  */   eSIPP_STATUS_ALREADY_DONE,
    /*  31  0x1F  */   eSIPP_STATUS_ASLEEP,
    /*  32  0x20  */   eSIPP_STATUS_BAD_ATTACHMENT,
    /*  33  0x21  */   eSIPP_STATUS_BAD_COMMAND,
    /*  34  0x22  */   eSIPP_STATUS_INT_HANDLED,
    /*  35  0x23  */   eSIPP_STATUS_INT_NOT_HANDLED,
    /*  36  0x24  */   eSIPP_STATUS_NOT_SET,
    /*  37  0x25  */   eSIPP_STATUS_NOT_HOOKED,
    /*  38  0x26  */   eSIPP_STATUS_COMPLETE,
    /*  39  0x27  */   eSIPP_STATUS_INVALID_NODE,
    /*  40  0x28  */   eSIPP_STATUS_DUPLICATE_NODE,
    /*  41  0x29  */   eSIPP_STATUS_HARDWARE_NOT_FOUND,
    /*  42  0x2A  */   eSIPP_STATUS_ILLEGAL_OPERATION,
    /*  43  0x2B  */   eSIPP_STATUS_INCOMPATIBLE_FORMATS,
    /*  44  0x2C  */   eSIPP_STATUS_INVALID_DEVICE,
    /*  45  0x2D  */   eSIPP_STATUS_INVALID_EDGE,
    /*  46  0x2E  */   eSIPP_STATUS_INVALID_NUMBER,
    /*  47  0x2F  */   eSIPP_STATUS_INVALID_STATE,
    /*  48  0x30  */   eSIPP_STATUS_INVALID_TYPE,
    /*  49  0x31  */   eSIPP_STATUS_STOPPED,
    /*  50  0x32  */   eSIPP_STATUS_SUSPENDED,
    /*  51  0x33  */   eSIPP_STATUS_TERMINATED,
    /* Last Entry */   eSIPP_STATUS_CODE_LAST = eSIPP_STATUS_TERMINATED

} eSIPP_STATUS;

//////////////////////////////////////////////////////////
// Internal Error codes
enum
{
     E_SUCCESS                =  0, // Success
     E_OUT_OF_MEM             =  1, // Out of memory (a requirement on a memory pool could not be satisfied). Try increase pool size
     E_INVALID_MEM_P          =  2, // Invalid Memory Pool
     E_PAR_NOT_FOUND          =  3, // Parent not found (a filter is looking for a parent and is not found in parent list)
     E_DATA_NOT_FOUND         =  4, //
     E_RUN_DON_T_KNOW         =  5, // Scheduler: Cannot schedule filter
     E_INVALID_HW_PARAM       =  6, // Invalid HW Parameter
     E_INVLD_FILT_FIRST_SLICE =  7, // First slice of a filter is smaller than first slice of its pipelie
     E_INVLD_FILT_LAST_SLICE  =  8, // Last slice of a filter is greater than last slice of its pipelie
     E_MISSING_SHAVE_IMAGE    =  9, // Set on myriad :if pipeline uses SW filters, but shave image is NULL
     E_UNIMPLEMENTED_FEAT     = 10, // Marks unimplemented feature
     E_PC_CMX_MEM_ALLOC_ERR   = 11, // On PC builds only: marks that CMX memory buffer could not be allocated
     E_OPT_EXEC_NUM           = 12, // E_OPT_EXEC_NUM is defined to a value smaller than number of filters in the pipeline. Try increase.
     E_CANNOT_FINISH_FILTER   = 13, // Scheduler cannot finish schedule of a filter till filter height is reached. Can happen for very small image heights
     E_DATA_ALIGN             = 14, // Internal shave data alignment error (some data mebers need to be 8B aligned to meet Shave ASM optimisations)
     E_INVLD_MIPI_RX_LOOPBACK = 15, // Loopback not supported for the given mipi RX units (only RX 1 and 3 support loopback)
     E_TOO_MANY_FILTERS       = 16, // MAX_FILTERS was exceeded, cannot e > 32 for now
     E_INVLD_MULTI_INSTANCE   = 17, // Invalid use of multi-instance: applies to all HW filters!
     E_INVLD_HW_ID            = 18, // Invalid use of ID=13 (previously allocated to DS), or mipi-rx2
     E_TOO_MANY_PARENTS       = 19, // A filter's number of parents exceeds SIPP_FILTER_MAX_PARENTS
     E_TOO_MANY_CONSUMERS     = 20, // A filter's number of consumers exceeds MAX_CONSUMERS
     E_RUNS_ITER_GROUPS       = 21, // A pipeline needs to have PLF_RUNS_ITER_GROUPS flag SET prior to finalization if runs on sippProcessIters
     E_TOO_MANY_DMAS          = 22, // Number of DMA filters defined in the pipeline exceeds MAX_DMA_FILTERS
     E_INVLD_SLICE_WIDTH      = 23, // Invalid Slice width (must be multiple of 8)
     E_OSE_CREATION_ERROR     = 24, // Something caused an error in OSE creation algo
     E_CDMA_QU_OVERFLOW       = 25, // CMX DMA task qu has overflowed
     E_PC_RUNTIME_FAILURE     = 26, // PC Model experienced runtime error
     E_SCHEDULING_OVF         = 27, // Too many events queued in scheduler
     E_BLOCK_CALL_REJECTED    = 28, // A blocking API call was rejected as pending operations remain
     E_PRECOMP_SCHED          = 29, // An error was detected in an attempt to use a precompiled schedule for the pipeline
     E_FINALISE_FAIL          = 30, // An attempt to finalise a pipe failed often due to one of the errors above but this allows a catch all test
     E_HEAP_CREATION_FAIL     = 31, // An error occurred in attempt to create a heap

     E_LAST                   = E_HEAP_CREATION_FAIL
};

#define SIPP_ERROR_MASK_SIZE ((E_LAST/32)+1)

//////////////////////////////////////////////////////////
//Early typedef declarations
typedef struct SippFilterS          SippFilter, *pSippFilter;
typedef struct SippPipelineS        SippPipeline, *pSippPipeline;
typedef struct SippSchEntS          SippSchEnt;
typedef struct SippHwBufS           SippHwBuf;
typedef struct SippCmxBufferMapS    SippCmxBufferMap;
typedef struct SippCmxBufferMapS *  pSippCmxBufferMap;
typedef struct SippVPhysMapS        tSippVPhysMap, *pSippVPhysMap;
typedef struct SippFilterSchedInfoS SippFilterSchedInfo, *pSippFilterSchedInfo;

#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
typedef struct SippOseS             SippOse, *pSippOse;
#endif

typedef u8 (*SchedFuncAsk ) (SippSchEnt *ptrSE, u32 iBufId, s32 line_no, u32 linesPerIter);

typedef void (*FnSvuRun)    (SippFilter   *fptr);
typedef void (*SippCallback)(SippPipeline *sourcePl);

typedef struct sSchLineBufferS      sSchLineBuffer, *psSchLineBuffer;
typedef struct sSchedIBufUsageInfoS SchedIBufUsageInfo;

typedef struct sSippCdmaQuS         sSippCdmaQu, *psSippCdmaQu;
typedef struct sSippCdmaQuEntryS    sSippCdmaQuEntry, *psSippCdmaQuEntry;
typedef struct sSippCMDQuS          sSippCMDQu, *psSippCMDQu;
typedef struct sSippCMDQuEntryS     sSippCMDQuEntry, *psSippCMDQuEntry;

//////////////////////////////////////////////////////////
// Per Pipeline common info (accessible on filters side as well)
typedef struct
{
    // These must be first, so I read them with pl + P_O_GLOBAL_NFO
    u32            sliceFirst;     // First slice [0..11]
    u32            sliceLast;      // Last slice [0..11]

    u32            sliceSize;      // Slice Size in Bytes
    u32            curFrame;       // Current frame number

    SippPipeline * pl;             // Ref to pipeline struct (needed by Shaves...)
    u32            numShaves;

} CommInfo;

//////////////////////////////////////////////////////////
// Horizontal padding offsets
//
// Padding logic is:
//  LEFT  : *(lnToPad+dstLeftO)  = *(lnToPad + srcLeftO)
//  RIGHT : *(lnToPad+dstRightO) = *(lnToPad + srcRightO)
//
//  These are the offsets from lnToPad pointer computed from Leon side.
//  These offsets are defined per Plane and per Shave.
//  These offsets might differ from one shave to another and from
//  one plane to another, due to shifted STARTS and shifted planes
//  within a buffer.
typedef struct HorizPaddingOffS
{
    s32 srcLeftO ;  // Offset to get to SRC-Left  position
    s32 srcRightO;  // Offset to get to SRC-Right position
    s32 dstLeftO ;  // Offset to get to DST-Left  position
    s32 dstRightO;  // Offset to get to DST-Right position

} HorizPaddingOff;

//////////////////////////////////////////////////////////
//
typedef struct HPadInfoS
{
    SippFilter       *fptr;       // Pointer to Filter to be padded
    u32               svuPadFunc; // Optimized ASM Shave Padding function
    HorizPaddingOff  *padOffsets; // precomputed offsets for each xPlane and xShave
    u32               CT1;        // (fptr->nPlanes) * sizeof(HorizPaddingOff)
    u32               oBufId;     // Output buffer ID within fptr

} HPadInfo;

////////////////////////////////////////////////////////
// Scheduling Info at Setup (Not in sippSchTypes so that
// precompiled schedules may be used while
// still only including public header files)
typedef struct SchedInfoS
{
    u32 sippHwWaitMask;
    u32 shaveMask;
    u32 dmaMask;         // Bottom 24 bits
    u32 allMask;
    #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    u32 sippIbflIncMask;
    #ifdef BUGZILLA_22709
    u32 sippHwWaitEoFMask;
    #endif
    #endif

} SchedInfo;

////////////////////////////////////////////////////////
// used only by M2 pointer access ASM optimized routines
// to save a few c.c. and memory accesses
typedef struct ParentInfoS
{
    u32   nPlanes; //parent->nPlanes
    u32   outOff;  //parent->outOff

} ParentInfo;


#ifdef SIPP_SCRT_ENABLE_OPIPE
//////////////////////////////////////////////////////////////
// oPipe runtime managed buffer entry
typedef struct
{
    psSchLineBuffer pBuf;
    SippFilter *    pFilt;
    u16             linesNextFill;
    u8              oBufIdx;
    u32             clientCountMap[SIPP_FILTER_MAX_CONSUMERS];
    u16             clientCount[SIPP_FILTER_MAX_CONSUMERS];
    u32             clientCountIncr[SIPP_FILTER_MAX_CONSUMERS];
    u16             sinkLineCount[SIPP_FILTER_MAX_CONSUMERS];
    #ifdef SIPP_ADD_OPIPE_RT_CHECKS
    volatile u32 *  iBufCtxReg[SIPP_FILTER_MAX_CONSUMERS];
    volatile u32 *  oBufCtxReg;
    u16             lastLineServiced[SIPP_FILTER_MAX_CONSUMERS];
    u16             lastOutLineServiced;
    u16             totalLinesServiced;
    #endif
    u16             numClients;

} SippOPipeBuf, *pSippOPipeBuf;

#endif

////////////////////////////////////////////////////////
// 2D CMXDMA descriptor as in CMXDMA doc
typedef struct
{
    u64 dscCtrlLinkAddr; // CDMA_CFG_LINK_ADR
    u64 dscDstSrcAddr;   // CDMA_DST_SRC_ADR
    u64 dscPlanesLen;    // CDMA_LEN_ADR
    u64 dscSrcStrdWidth; // CDMA_SRC_STRIDE_WIDTH_ADR
    u64 dscDstStrdWidth; // CDMA_DST_STRIDE_WIDTH_ADR
    u64 dscPlStrides;    // CMDA_PLANE_STRIDE_WIDTH

} DmaDesc;

#ifdef SIPP_SCRT_ENABLE_OPIPE

#if defined(__sparc)
////////////////////////////////////////////////////////
// Customised CMXDMA descriptor for oPipe runtime
typedef struct
{
    #ifdef USE_CMX_DMA_NEW_DRIVER
    DrvCmxDmaTransaction dmaDsc;
    #else
    dmaTransactionList dmaDsc;
    #endif

    pSippFilter        pFilt;
    pSippOPipeBuf      pBuf;
    u32                managedBufClientIdx;
    u32                linesInTransaction;

} SippDmaDesc;

#endif

////////////////////////////////////////////////////////
// CMXDMA transaction list descriptor
typedef struct
{
    #if defined(__sparc)
    #ifdef USE_CMX_DMA_NEW_DRIVER
    DrvCmxDmaTransactionHnd  dmaHnd[SIPP_NUM_DESCS_PER_CDMA];
    #endif
    SippDmaDesc              sippDmaDsc[SIPP_NUM_DESCS_PER_CDMA];
    #endif
    u32         curDsc;



} DMATransDesc;

#endif

//////////////////////////////////////////////////////////
// DMA filter mode enumeration

typedef enum
{
    DMA_MODE_STANDARD = 0x0,
    DMA_MODE_PARTIAL_LINE             // Uses only part of source and/or fills only part destination line

} eDmaMode;

//////////////////////////////////////////////////////////
// DMA partials line configuration

typedef union
{
    u32 ddrLineStride;

} DMAPartialCfg;

//////////////////////////////////////////////////////////
// Extended DMA configuration union

typedef union
{
    u32             DMACfgPlaceholder;
    DMAPartialCfg   tPartialCfg;

} DMAExtCfg;

////////////////////////////////////////////////////////
// SIPP memory region descriptor
typedef struct
{
    u32 regionOffset;
    u32 regionSize;
    u32 regionUsed;    // Only for client debug

} SippMemRegion;

////////////////////////////////////////////////////////
// Virtual Memory pools definition
//
// Virtual Pools can be uniquely freed on a per pipeline
// basis, so pools are defined which are likely to contain
// structs which may be functionally grouped in terms of
// their allocation, usage and freeing

typedef enum
{
    vPoolGeneral = 0,
    vPoolPipeStructs,
    vPoolFilterLineBuf,
    vPoolCMXDMADesc,
    vPoolSchedule,
    vPoolScheduleTemp,
    vPoolFilterLineBuf0,
    vPoolFilterLineBuf1,
    vPoolFilterLineBuf2,
    vPoolFilterLineBuf3,
    vPoolFilterLineBuf4,
    vPoolFilterLineBuf5,
    vPoolFilterLineBuf6,
    vPoolFilterLineBuf7,
    vPoolFilterLineBuf8,
    vPoolFilterLineBuf9,
    vPoolFilterLineBuf10,
    vPoolFilterLineBuf11,
    vPoolLast

} SippVirtualPool;

////////////////////////////////////////////////////////
// SIPP virtual pool memory control block
typedef struct
{
    void *        pVPoolListStart[vPoolLast];
    void *        pVPoolListEnd[vPoolLast];
    u32           numVPools;
    u32           pipeIdx;
    pSippVPhysMap pVirtPhysMap;

} tSippMCB, *ptSippMCB;

////////////////////////////////////////////////////////
// SIPP pixel location
typedef struct
{
  UInt32 YPos[2];
  UInt32 XPos[12];

} SippPixelChunkPos;




////////////////////////////////////////////////////////
// CMX DMA Sw command queue entry

struct sSippCdmaQuEntryS
{
    u64 addr;
    u64 value;
};

////////////////////////////////////////////////////////
// CMX DMA Sw command queue

struct sSippCdmaQuS
{
    sSippCdmaQuEntry * entry;
    u32                numEntries;
    u32                size;
};

////////////////////////////////////////////////////////
// General SIPP Sw command queue entry

struct sSippCMDQuEntryS
{
    u32 addr;
    u32 value;
};

////////////////////////////////////////////////////////
// General SIPP Sw command queue

struct sSippCMDQuS
{
    sSippCMDQuEntry * quEntry;
    u32               quNum;
    u32               quSize;
};

//////////////////////////////////////////////////////////////
// SIPP Framework Event info callback

typedef void ( * sippEventCallback_t )(
    SippPipeline *             pPipeline,
    eSIPP_PIPELINE_EVENT       eEvent,
    SIPP_PIPELINE_EVENT_DATA * ptEventData
);

//////////////////////////////////////////////////////////////
// Modular Scheduler and RunTime Support

//////////////////////////////////////////////////////////////
// Scheduler
typedef u32 ( * sippSchedFunc ) (
    pSippPipeline pipeLine,
    bool allocMem,
    bool reschedPipe
);

typedef void ( * sippSchedSetBufConsModels ) (
    pSippPipeline pPipe
);

//////////////////////////////////////////////////////////////
// Runtime
typedef void ( * sippRuntimeClaimHWResourceFunc ) (
    pSippPipeline pPipe
);

typedef void ( * sippRuntimeFunc ) (
    pSippPipeline                    pPipe,
    eSIPP_ACCESS_SCHEDULER_EVENT     eEvent,
    SIPP_ACCESS_SCHEDULER_EVENT_DATA pData
);

typedef u32 ( * sippRunIterDoneFunc ) (
    pSippPipeline pPipe
);

typedef void ( * sippRunNextIterFunc ) (
    pSippPipeline pPipe
);

typedef void ( * sippRunFrameReset ) (
    pSippPipeline pPipe
);

//////////////////////////////////////////////////////////////
// Runtime Statistics
typedef struct
{
    u64 * iterTime;
    u32   DMAWaitCycles;
    u32   HWWaitCycles;
    u32   ShaveWaitCycles;
    u32   ScheduleCycles;

} tRTStats, * ptRTStats;

//========================================================================================
//Main SIPP Pipeline data structure
struct SippPipelineS
{
    CommInfo     gi;                    // Global info
    u32          oldRunMask;            // Prev iteration exec mask
    u32          canRunMask;            // Current iteration exec mask
    u32          nFilters;              // Number of filters in pipeline
    u32          nPadFilters;           // Number of filters in pipeline that require hPadding (see padList)
    u32          svuWinRegs[4];         // Shaves win regs : 4wins, symetrical for all shave, SLICE_SIZE apart !
    u8         * mbinImg;               // Shaves SIPP mbin
    u32          nIter;                 // Number of iterations / frame
    u32          linesPerIter;          // Number of lines to be produced by constituent filters in one scheduling iteration / default to 1
    u32          svuCmd;                // [Leon]-[MasterSvu] sync (only used by PC-model and Myriad1)
    u32          svuSyncMtx[2];         // A Pair of mutexes used for Leon-Shaves synchronization
    u32          svuSyncSem;            // Semaphore variable (shaves decrement when they finish, on ZERO a swic is generated)
    u32          svuSyncMtxAddr;        // Precomputed Mutex Request Addr
    u32          svuSyncMtxParity;
    SippFilter * filters[SIPP_MAX_FILTERS_PER_PIPELINE];  // All filters
    HPadInfo     padList[SIPP_MAX_FILTERS_PER_PIPELINE];  // All filters that require SW hPadding

    // References to filters specific to each exec unit.
    SippFilter * filtersHw    [SIPP_MAX_FILTERS_PER_PIPELINE];     u32 nFiltersHw;
    SippFilter * filtersSvu   [SIPP_MAX_FILTERS_PER_PIPELINE];     u32 nFiltersSvu;
    SippFilter * filtersDMA   [SIPP_MAX_DMA_FILTERS_PER_PIPELINE]; u32 nFiltersDMA;

    SippFilter * firstRunFilts[SIPP_MAX_FILTERS_PER_PIPELINE];     u32 nFirstRunFilts;

    s32          hwSippFirst [EXE_NUM]; // Index of first filter to run on a given HW unit
    u32          hwSippFltCnt[EXE_NUM]; // Num of filters that execute on a given HW unit

    u32          shadowSelect;
    u32          hwSippCtxSwMask;       // Alternate to SIPP_INT1_ADR, to cope with multiple-context/HW_unit
    SchedInfo *  schedInfo;             // Fixed-size schedule info (all/shave/dma/sipp_start masks)
    u32          schedInfoEntries;
    u32 *        schedInfoCtx;          // Variable sched info for sipp HW filters CTX switch
    u32          schedInfoCtxSz;        // size in u32 words (this offset gets added to *schedInfoCtx)
    u32          schedConsumeMask;
    u32          sliceSz;               // Slice size (for chunking mode)
    int          dbgLevel;
    u32          iteration;             // Current iteration
    u32          flags;
    u32          svuStack;

    u32          startDelta;
    u32          ibflIncDelta;
    u32          firstUseMask;          // Each bit represents a constituent filter having run for the first time
    u32          firstIbflUseMask;      // Each bit represents a constituent filter having updated its op buffer for the first time
    u32          firstUseInProg;

    u32          nxtExeNo[SIPP_MAX_FILTERS_PER_PIPELINE]; // Temp buffer to compute next filter execution numbers
                                                          // allocated per pipeline to allow concurency

    // All of this may be considered for removal or only if process iters is defined
    u32          multiHwCtx;      // 0: if num_Filter per HW unit <=1; 1 else
    u8         * ddrCmxBackupAdr; // location in DDR where the slices need to be saved !
    u32          ddrCmxBackupLen; // size of data sections from a SLICE
    u64          dmaTaskList;     // dma command to start all dma filters in current iter

    //Save CMX data sections (static, stack, line pool) to DDR for pipeline switch
    DmaDesc      dmaCmxPush;
    DmaDesc      dmaCmxPop;
    u64          pushCmd;  //triggers dmaPush
    u64          popCmd;   //triggers dmaPop

    sippEventCallback_t pfCallback;
    u32                 uHWFilterUsageBitMask[SIPP_HW_FILTER_MASK_SIZE];
    u32                 uHWFilterIBufUsageMask;
    u32                 uHWFilterOBufUsageMask;

    // Support for CMX line buffer memory allocation api
    u8                  useCmxRegMap;
    u32                 numMemRegions;
    pSippCmxBufferMap   pCmxMap;
    u8                  cmxMapResult;

    #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    u32                 oPipeCfg;
    pSippOse            pOSE[SIPP_MAX_OSE_PER_PIPE];
    u32                 numOSE;
    #endif

    SippSchEnt *        pSE[SIPP_MAX_SE_PER_PIPE];
    u32                 numSE;

    u32                 HWPipeID;
    u32                 itersLeft;
    u32                 endIter;
    u32                 isrFlags;
    u32                 useSyncRuntime;

    #ifdef RT_DBG
    u32                 dbgArea[32];
    u32                 dbgAreaIndex;
    #endif
    u32                 errorStatus[SIPP_ERROR_MASK_SIZE];

    u32                 allDoneMask;
    u32                 setIterMask;

    sippSchedFunc                   pfnSippSchedule;
    sippSchedSetBufConsModels       pfnSippScheSetBufConsModels;
    sippRuntimeClaimHWResourceFunc  pfnSippRuntimeClaimHWResource;
    sippRuntimeFunc                 pfnSippRuntime;
    sippRunIterDoneFunc             pfnSippRunIterDone;
    sippRunNextIterFunc             pfnSippRunNextIter;
    sippRunFrameReset               pfnSippRunFrameReset;

    sSippCdmaQu                     tCmxDmaQu[0x2];
    sSippCMDQu                      tCMDUpdateQu;
    sSippCMDQu                      tCMDStartQu;

    u32                             runAddSchedCheck;

    #ifdef SIPP_RUNTIME_PROFILING
    ptRTStats                       pRTStats;
    #endif

    #ifdef SIPP_F_DUMPS //debug output files for all filters in pipeline
    FILE       * filtFile[SIPP_MAX_FILTERS_PER_PIPELINE];
    char         filtName[SIPP_MAX_FILTERS_PER_PIPELINE][256];
    #endif

    // Heap control
    tSippMCB     tHeapMCB;
    u8 *         lineMemPoolBase;

    #ifdef SIPP_SCRT_ENABLE_OPIPE
    pSippOPipeBuf managedBufList;
    u8            numManagedBufs;
    #endif
};

//========================================================================================
struct SippFilterS
{
    u32           exeNo;                               // Actual number of executed runs
    u32           nPlanes[SIPP_FILTER_MAX_OBUFS];      // Number of planes - specific to an output buffer

    u32           nCons;
    u32           outputH;                             // Output height
    u32           linesPerIter;                        // The number of lines to be output each iteration under normal considtions
    u32           firstIterLines;                      // The number of lines to be output on the first iteration the filter outputs
    u32           firstRunNumLines;                    // Used to control first iteration filter starting
    u32           firstRunRollLines;                   // Used to control first iteration buffer rolling
    u32           outLineDeficit;                      // For MLPI, represents linesPerIter - firstIterLines
    u32           schNo;                               // Current scheduled num (can be in advance with 1 iteration vs. runNo)
    SippFilter *  parents   [SIPP_FILTER_MAX_PARENTS];
    u32           parentsKS [SIPP_FILTER_MAX_PARENTS]; // In parent buffs
    ParentInfo    parInfo   [SIPP_FILTER_MAX_PARENTS]; // only for M2 ASM optimization !
    pSippPipeline pPipe;                               // Pointer to the pipe  structure of which this filter is a member
    FnSvuRun      funcSvuRun;                          // Filter-Shave function (if any)
    void       *  params;                              // Points to filter's private persistent data store, if any
    u32           flags;

    // Parent info
    u32           nParents;                            // Number of parents
    u32           nLinesUsed[SIPP_FILTER_MAX_PARENTS]; // Number of lines needed from parent buffs

    u32              id;                               // Filter unique id (gets allocated incrementally at CREATE time)
    u32              firstOutSlc;                      // First output slice
    u32              nCtxLoads;
    u32              unit;                             // HW Execution unit that will actually run the filter
    const CommInfo * gi;

    // Consumer data
    SippFilter * cons[SIPP_FILTER_MAX_CONSUMERS];

    // Double bufferered base line pointers
    u32        * dbLinesIn[SIPP_FILTER_MAX_PARENTS][2][SIPP_MAX_LINES_PER_ITER];     // Consider mallocing when we know the number of lines per iter?

    u32          outputW;           // Total line width, ignoring slices [pixels]
    u32          sliceWidth;        // One slice's output width          [pixels]
    u32          sliceWidthLastSvu; // Last Shave slice-width

    // When we get around to optimising SHAVE runtimes - I assume this could be structured better
    u32          linesThisIter[2];                                                 // Used by Sw filters
    u32          hPadding[SIPP_FILTER_MAX_OBUFS];                                  // Horizontal Padding [pixels]
    u32          bpp[SIPP_FILTER_MAX_OBUFS];                                       // Bits per pixel
    s32        * outOff[SIPP_FILTER_MAX_OBUFS];                                    // precomputed CT to help Consumers derive pointers
    u8         * dbLineOut [SIPP_FILTER_MAX_OBUFS][2][SIPP_MAX_LINES_PER_ITER];
    u8         * lnToPad[SIPP_FILTER_MAX_OBUFS][2][SIPP_MAX_LINES_PER_ITER];       // line to be padded
    u32          lineStride[SIPP_FILTER_MAX_OBUFS];                                // Line stride [pixels] (including Left+Right hPadding)
    u32          planeStride[SIPP_FILTER_MAX_OBUFS];                               // Plane stride [pixels] (including paddings)
    u8         * outputBuffer[SIPP_FILTER_MAX_OBUFS];                              // The OUTPUT BUFFER - may need more than one of these now
    u8         * outLinePtr[SIPP_FILTER_MAX_OBUFS][SIPP_MAX_LINES_PER_ITER];       // Ptr to current produced line
    u32          outLineOffset[SIPP_FILTER_MAX_OBUFS];                             // For MLPI, specifies the distance to the output line in the linearised circular buffer
    u8         * linePtrs[SIPP_FILTER_MAX_OBUFS];                                  // Circular output buffer linear view
    u8        ** linePtrs1stBase[SIPP_FILTER_MAX_OBUFS];                           //
    u8         * linePtrs2ndBase[SIPP_FILTER_MAX_OBUFS];                           //
    u8         * linePtrs3rdBase[SIPP_FILTER_MAX_OBUFS];                           //
    u8         * linePtrs4thBase[SIPP_FILTER_MAX_OBUFS];                           //
    u8         * linePtrs5thBase[SIPP_FILTER_MAX_OBUFS];                           //
    u8           oBufAlloc[SIPP_FILTER_MAX_OBUFS];                                 // Specifies line buffer was allocated in a memory region assigned
                                                                                   // via sippAllocCmxMemRegion API
    u32          nLines[SIPP_FILTER_MAX_OBUFS];                                    // Number of lines in output buffer
    u32          parentOBufIdx[SIPP_FILTER_MAX_IBUFS];                             // Hold the index within the parent filters' oBuf lists
                                                                                   // of this filter's input buffers

    // For HW-SIPP filters that require context switch, alloc I/O Buff configs
    // (including ICTX/OCTX). Filter specific params are already stored in params area

    // TODO - rename to hwIBuf
    SippHwBuf * iBuf[SIPP_FILTER_MAX_IBUFS];        //some filters use 2 input buffers
    SippHwBuf * oBuf[SIPP_FILTER_MAX_OBUFS];

    SippSchEnt *         sch;            // Used at schedule & PC_test time ONLY
    pSippFilterSchedInfo pOpipeSch;      // Used in oPipe buffer sizing algo only

    psSchLineBuffer      oBufs[SIPP_FILTER_MAX_OBUFS];
    psSchLineBuffer      iBufs[SIPP_FILTER_MAX_IBUFS];
    SchedIBufUsageInfo * iBufCtx[SIPP_FILTER_MAX_IBUFS];
    u32                  numOBufs;
    u32                  numIBufs;
    u32                  consIbufIdx[SIPP_FILTER_MAX_CONSUMERS];
    u16 *                KSIterList[SIPP_FILTER_MAX_IBUFS];

    #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    u32                  iBufIds;        // Allows any scheduler targeting MA2x5x to know which IBuf Ids
    u32                  oBufIds;
    u8                   memOSE;
    pSippOse             pOSE;
    u8                   OBufProdLatency[SIPP_FILTER_MAX_OBUFS];
    u8                   oseLatency[SIPP_FILTER_MAX_OBUFS];
    #endif

    #ifdef BUGZILLA_22709
    u32                 bug22709Active;
    #endif

    SippPixelChunkPos * ptrChunkPos;
    u32                 errorStatus[SIPP_ERROR_MASK_SIZE];

    u32                 bytesPerPix; // Only for ASM optimization !
                                     // Bytes per pixel as power of TWO (e.g. 1BPP=>0, 2BPP=>1 4BPP=>2)
    u8                  linesPerIterShift;

    #ifdef RT_DBG
    u32                 dbgArea[8];
    u32                 dbgAreaIndex;
    #endif

    // Put some PC only data at tail of struct so as not to interfere with production of assembler offsets file
    #ifdef SIPP_PC
    u8 *                outLinePtrPrev[SIPP_FILTER_MAX_OBUFS][SIPP_MAX_LINES_PER_ITER];   // Ptr to previous produced line
    #endif

    #ifdef SIPP_SCRT_ENABLE_OPIPE
    u8                  irqRatePow;  // Set IRQ rate control in powers of two
    #endif
};

//========================================================================================
// Raw stats collection types

typedef struct
{
    uint32_t count    [4]; // number of pixels in alternative accumulation.
    uint32_t accum    [4]; // accumulation of pixels within limits
    uint32_t alt_accum[4]; // accumulation of pixels outside limits

} ae_patch_stats;

typedef struct
{
    int32_t UNDEFINED;                   // So that entire struct is 8-Byte multiple
    int32_t sum_all_green;
    int32_t filter1_sum_max_green;
    int32_t filter2_sum_max_green;
    int32_t filter1_number_of_used_pixels_green;
    int32_t filter1_sum_green;
    int32_t filter2_number_of_used_pixels_green;
    int32_t filter2_sum_green;

} AF_paxel_statistics;

#endif /* _SIPP_TYPES_H_ */

