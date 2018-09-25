///
/// @file HglCmxDmaDefines.h
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup HglCmxDma
/// @{
/// @brief     Definitions header of HGL for CMX DMA
///


#ifndef COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_LEON_HGL_INCLUDE_HGLCMXDMADEFINES_H_
#define COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_LEON_HGL_INCLUDE_HGLCMXDMADEFINES_H_

#include <stdint.h>
#include "registersMyriad.h"

#define HGL_CMX_DMA_MIN_BURST_LEN 1
#define HGL_CMX_DMA_MAX_BURST_LEN 16
#define HGL_CMX_DMA_MAX_IRQ_TRIG 15
#define HGL_CMX_DMA_MAX_ID 15
#define HGL_CMX_DMA_MAX_PRIO 3

#define HGL_CMX_DMA_LA_NUM 4
#define HGL_CMX_DMA_LA_START 1
#define HGL_CMX_DMA_AGENT_BUSY_OFFSET 2
#define HGL_CMX_DMA_LANE_BUSY_OFFSET 6
#define HGL_CMX_DMA_LA_BUSY_MASK \
  0xF  // Bit mask used to check which link agent is busy.

#define HGL_CMX_DMA_ENABLE_BIT (1 << 2)
#define HGL_CMX_DMA_DISABLE_BIT (0 << 2)
#define HGL_CMX_DMA_RESET_BIT (1 << 3)
#define HGL_CMX_DMA_ARB_MODE_BIT (1 << 7)

#define HGL_CMX_DMA_AGENT_IS_BUSY(agent)                    \
  (GET_REG_WORD_VAL(CDMA_STATUS_ADR) &                      \
         (1 << HGL_CMX_DMA_AGENT_BUSY_OFFSET)  << (agent))  \

#define HGL_CMX_DMA_LA_BUSY_MASK 0xF  // Bit mask used to check which link
                                      // agent is busy. This value corresponds
                                      // to status "All Link Agents Busy"

#define CMXDMA_SECTION __attribute__((section(".cmx_direct.data")))

//#define HGL_CMX_DMA_CHECK_DESCR_ADDR

// Bit field for fine-grained configuration of CMXDMA transaction
typedef struct {
  uint32_t type : 2;               // Transaction type(1D/2D)
  uint32_t priority : 2;           // Transaction priority(0 - 3)
  uint32_t burst_length : 4;       // Burst length
  uint32_t id : 4;                 // Transaction ID
  uint32_t interrupt_trigger : 4;  // ID of interrupt to be generated when
                                   // the task is executed
  uint32_t reserved1 : 4;          // Reserved
  uint32_t disable_int : 1;        // Disable interrupts
  uint32_t reserved2 : 6;          // Reserved
  uint32_t skip_nr : 5;            // Skip descriptor
} HglCmxDmaConfigBits;

// 2D transaction type
typedef struct HglCmxDmaTransactionList {
  void *link_address;  // pointer to the next element in linked list
  union {
    HglCmxDmaConfigBits cfg_bits;
    uint32_t full_cfg_register;
  } cfg_link;
  void *src;            // Pointer to the source of the data transfer
  void *dst;            // Pointer to the destination
  uint32_t length;      // Transaction length
  uint32_t planes_no;   // Number of planes
  uint32_t src_width;   // Bytes of data required from one line of source
  uint32_t src_stride;  // Length in bytes from start of one line of data,
                        // to start of next line of data
  uint32_t dst_width;   // Bytes of data required from one line of destination
  uint32_t dst_stride;  // Length in bytes from start of one line of data,
                        // to start of next line of data
  uint32_t src_plane_stride;  // Source plane stride
  uint32_t dst_plane_stride;  // Destination plane stride
  uint32_t sw_id;
  uint32_t user_data_0;
} HglCmxDmaTransaction __attribute__((
    aligned(8)));  // dmaTransactionLists must be aligned to 64 bit
                   // boundaries (This is a requirement of the
                   // hardware DMA engine)

typedef enum {
  HGL_CMX_DMA_ARB_MODE_RR = 0,
  HGL_CMX_DMA_ARB_MODE_PRIO
} HglCmxDmaArbMode;

typedef enum {
  HGL_CMX_DMA_LA_NOT_BUSY = 0,
  HGL_CMX_DMA_LA_BUSY
} HglCmxDmaLinkAgentStatus;

typedef enum {
  S_POP_REQ_IDLE = 0,
  S_POP_REQ_ISSUE = 1,
  S_POP_WAIT_RSP = 2,
  S_POP_WAIT_TAKEN = 3,
  S_POP_STALL = 4,
} HglCmxDmaReqSmState;

typedef struct {
  HglCmxDmaTransaction *tail;
  uint32_t sw_id;
  uint8_t initialized;
} HglCmxDmaStatusInfo;

#endif  // COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_LEON_HGL_INCLUDE_HGLCMXDMADEFINES_H_

///@}
