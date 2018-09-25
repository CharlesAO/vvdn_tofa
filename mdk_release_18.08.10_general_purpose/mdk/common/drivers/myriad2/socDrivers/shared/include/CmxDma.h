///
/// @file CmxDma.h
/// 
///  
/// @defgroup CmxDma CmxDma Driver
/// @{
/// @brief Macros and functions for the CmxDma Driver.
///

#ifndef _COMMON_DMA_DEFS_H_
#define _COMMON_DMA_DEFS_H_

#ifndef USE_CMX_DMA_NEW_DRIVER

#include <mv_types.h>

#define DMA_1D_TRANSACTION 0x0 ///< 1D transaction type
#define DMA_2D_TRANSACTION 0x1 ///< 2D transaction type

#define DMA_ENABLE_BIT     (1 << 2)
#define DMA_DISABLE_BIT     (0 << 2)
#define DMA_RESET_BIT      (1 << 3)
#define DMA_PRIORITY(N) ((N & 3) << 2) ///< Transaction priority(0 - 3)
#define DMA_BURST_LENGTH(N) ((N & 0xF) << 4) ///< Burst length(number of transaction in burst)
#define DMA_TRANSACTION_ID(N) ((N & 0xF) << 8) ///< ID of transaction
#define DMA_INTERRUPT_TRIGGER(N) ((N & 0xF) << 12) ///< ID of the interrupt which will be triggered when the task is done
#define DMA_DISABLE_INTERRUPTS (1 << 21) ///< Stop the interrupt generation
#define DMA_ENABLE_INTERRUPTS (0 << 21) ///< Start the interrupt generation
#define DMA_SKIP_NR(N) ((N & 0x1F) << 27)///< Set the skip descriptor for a task, in order to determine from the skip register whether the task should be skipped or not.

#define NR_OF_LINK_AGENTS      4
#define START_LCA_BIT (1ULL << 32)///< Value used to start the Link Agent
#define LA_BUSY_OFFSET 2///< The offset for "Link Agent busy" bits in CDMA_STATUS register
#define LA_BUSY_MASK 0xF///< Bit mask used to check which link agent is busy. This value corresponds to status "All Link Agents Busy"
#define TASK_SKIP_DESCRIPTOR 31
#define DMA_MIN_BL 1
#define DMA_MAX_BL 16

#ifndef DMA_MUTEX_COMMON
    #define DMA_MUTEX_COMMON 6 ///< Mutex used to get syncronized access to CMXDMA
#endif

#ifndef DMA_MUTEX0
    #define DMA_MUTEX0 7 ///< Mutex used to get syncronized access to CMXDMA
#endif

#ifndef DMA_MUTEX1
    #define DMA_MUTEX1 8 ///< Mutex used to get syncronized access to CMXDMA
#endif

#ifndef DMA_MUTEX2
    #define DMA_MUTEX2 9 ///< Mutex used to get syncronized access to CMXDMA
#endif

#ifndef DMA_MUTEX3
    #define DMA_MUTEX3 10 ///< Mutex used to get syncronized access to CMXDMA
#endif

#define CMXDMA_SECTION __attribute__((section(".cmx.cdmaDescriptors")))

enum req_sm_states
{
    S_POP_REQ_IDLE   = 0,
    S_POP_REQ_ISSUE  = 1,
    S_POP_WAIT_RSP   = 2,
    S_POP_WAIT_TAKEN = 3,
    S_POP_STALL      = 4,
};

enum rsp_sm_states
{
    S_POP_IDLE    = 0,
    S_RECEIVE_CMD = 1,
};

enum dma_channels
{
    DMA_CHANNEL0 = 0,
    DMA_CHANNEL1 = 1,
    DMA_CHANNEL2 = 2,
    DMA_CHANNEL3 = 3,
};

enum dma_agents
{
    DMA_AGENT0 = 0,
    DMA_AGENT1 = 1,
    DMA_AGENT2 = 2,
    DMA_AGENT3 = 3,
};

typedef int dmaRequesterId;

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Initialize the array that holds the pointers to last tasks that are executed by a link agent
///
/// This is the only time we use the ___globalTail directly, every processor operating with it's own tail array.
/// @param[in] tail - Array of pointers to each entry of the ___globalTail.
/// @return void
///
void CmxDmaInitTail(u32 **tail[]);

#ifdef __cplusplus
}
#endif

#endif // USE_CMX_DMA_NEW_DRIVER

#endif // _COMMON_DMA_DEFS_H_


///@}
