// -----------------------------------------------------------------------------
// Copyright (C) 2015 Movidius Ltd. All rights reserved
//
// Filename         : sippCfg.h
// Company          : Movidius
// Description      : Public header file for exported SIPP Framework types
//
// -----------------------------------------------------------------------------


#ifndef _SIPP_CFG_H_
#define _SIPP_CFG_H_

#include <sippPlatform.h>

////////////////////////////////////////////////////////
// Pipeline limits

#define SIPP_MAX_SUPPORTED_PIPELINES       8        /* Should we read this in from a cfg struct and then in SWInit */
                                                    /* malloc all the items it needs? */
#define SIPP_MAX_FILTERS_PER_PIPELINE     32
#define SIPP_MAX_BUFFERS_PER_PIPELINE     (SIPP_MAX_FILTERS_PER_PIPELINE + (SIPP_MAX_FILTERS_PER_PIPELINE>>1))
#define SIPP_FILTER_MAX_PARENTS            4
#define SIPP_FILTER_MAX_CONSUMERS          6
#define SIPP_MAX_DMA_FILTERS_PER_PIPELINE 32

#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
#define SIPP_FILTER_MAX_IBUFS             0x4
#define SIPP_FILTER_MAX_OBUFS             0x2
#define SIPP_SE_MAX_IBUFS                 0x6
#define SIPP_SE_MAX_OBUFS                 0x4       /* Caters for the case that portions of the oPipe are run in */
#define SIPP_SE_MAX_CONSUMERS             0x6
#endif                                              /* combination with SW kernels */

#define SIPP_NUM_SVUS                     12
#define SIPP_MAX_SE_PER_PIPE              SIPP_MAX_FILTERS_PER_PIPELINE

#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
#define SIPP_MAX_FILTS_OSE                16
#define SIPP_MAX_OSE_PER_PIPE             4
#endif

#define SIPP_MAX_LINES_PER_ITER           16

////////////////////////////////////////////////////////
// SIPP Filter Output Dumps, undef to cancel dumps

#ifdef SIPP_PC
#define SIPP_F_DUMPS
#endif

////////////////////////////////////////////////////////
// CMX-DMA settings

#ifndef SIPP_CDMA_INT_NO
#define SIPP_CDMA_INT_NO 12 //int bit [0..15]
#endif

#ifndef SIPP_CDMA_AGENT_NO
#define SIPP_CDMA_AGENT_NO 0 //link agent [0..3]
#endif

#ifndef SIPP_OPIPE_CDMA_AGENT_NO
#define SIPP_OPIPE_CDMA_AGENT_NO 3 //link agent [0..3]
#endif

#define SIPP_NUM_DESCS_PER_CDMA 4             // When using cmxdma driver, defines the number of dma transcation types to be held by each dma unit

////////////////////////////////////////////////////////
// SIPP within Media Subsystem settings
#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
#define SIPP_NUM_IRQ_BARRIERS 0x4
#endif

#define SIPP_MAX_SUPPORTED_HW_PIPELINES  SIPP_NUM_IRQ_BARRIERS
#define SIPP_DEFAULT_SLICE_SIZE   (128 << 0xA)

////////////////////////////////////////////////////////
// Memory usage setting

#ifndef SIPP_CMX_POOL_SZ
#define SIPP_CMX_POOL_SZ (192*1024)
#endif

#ifndef SIPP_DDR_POOL_SZ
#define SIPP_DDR_POOL_SZ (4*1024*1024)
#endif

////////////////////////////////////////////////////////
// Miscellaneous resource settings

#ifndef BUFF_HUGE_SZ
#define BUFF_HUGE_SZ     128 //MAX number of lines allocated in each buffer at schedule time
#endif

// Maximum number of error events to hold in the history
#define SIPP_ERROR_HISTORY_SIZE 16

////////////////////////////////////////////////////////
// Access scheduler config
#define SIPP_ACCESS_SCHEDULER_QU_SIZE   ( SIPP_MAX_SUPPORTED_PIPELINES * 8 )
#define SIPP_MAX_EVENTS_PER_PIPE        0x8

////////////////////////////////////////////////////////
// RTOS settings

#define SIPP_PAL_MESSAGE_SIZE    0x4
#define SIPP_THREAD_STACK_SIZE   0x2000
#define SIPP_THREAD_NAME         "/SIPP"
#define SIPP_THREAD_PRIORITY     225                           // This should be an OS agnostic figure!!

#define SIPP_ACCESS_SCHEDULER_QU_NAME   "/SIPPQU"

// Runtimes coexistence

#ifdef  SIPP_ENABLE_RT_COEXISTENCE
#define NO_INTERRUPT_BARRIERS_MODE
#endif

////////////////////////////////////////////////////////
// Re-route irqs to Leon OS

#define DYNAMIC_IRQ_SIPP_0 IRQ_DYNAMIC_6
#define DYNAMIC_IRQ_SIPP_1 IRQ_DYNAMIC_7
#define DYNAMIC_IRQ_SIPP_2 IRQ_DYNAMIC_8

#endif /* _SIPP_CFG_H_ */
