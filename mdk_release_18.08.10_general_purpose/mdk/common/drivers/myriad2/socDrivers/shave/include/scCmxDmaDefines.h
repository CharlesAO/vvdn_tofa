///
/// @file scCmxDmaDefines.h
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup scCmxDma
/// @{
/// @brief     Definitions header of CMX DMA driver for SHAVE
///


#ifndef COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_SHAVE_INCLUDE_SCCMXDMADEFINES_H_
#define COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_SHAVE_INCLUDE_SCCMXDMADEFINES_H_

// 0: Includes
// ----------------------------------------------------------------------------
#include "DrvCmxDmaSharedDefines.h"

// 1: Defines
// ----------------------------------------------------------------------------
#define SC_CMX_DMA_BACK_COMPATIBLE 1

#define DRV_CMX_DMA_LA_NUM 4
#define SC_CMX_DMA_ENABLE_BIT (1 << 2)
#define SC_CMX_DMA_ARB_MODE_BIT (1 << 7)

#define SC_CMX_DMA_MIN_BURST_LEN 1
#define SC_CMX_DMA_MAX_BURST_LEN 16
#define SC_CMX_DMA_MAX_IRQ_TRIG  15
#define SC_CMX_DMA_MAX_ID        15
#define SC_CMX_DMA_MAX_PRIO      3

#ifndef SC_CMX_DMA_DEFAULT_TRAN_PRIO
#define SC_CMX_DMA_DEFAULT_TRAN_PRIO    3
#endif

#ifndef SC_CMX_DMA_DEFAULT_BURST_SIZE
#define SC_CMX_DMA_DEFAULT_BURST_SIZE  16
#endif

//#define SC_CMX_DMA_CHECK_DESCR_ADDR

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------
//
typedef DrvCmxDmaTransactionType ScCmxDmaTransactionType;
typedef DrvCmxDmaTransactionStatus ScCmxDmaTransactionStatus;
typedef DrvCmxDmaAgent ScCmxDmaAgent;

typedef enum {
    SC_CMX_DMA_LA_NOT_BUSY = 0,
    SC_CMX_DMA_LA_BUSY
} ScCmxDmaLinkAgentStatus;

typedef struct {
    int32_t agent;
} ScCmxDmaSetupStruct;

// Bit field for fine-grained configuration of CMXDMA transaction
typedef struct {
    uint32_t type              : 2;  // Transaction type(1D/2D)
    uint32_t priority          : 2;  // Transaction priority(0 - 3)
    uint32_t burst_length      : 4;  // Burst length
    uint32_t id                : 4;  // Transaction ID
    uint32_t interrupt_trigger : 4;  // ID of interrupt to be generated when
                                    // the task is executed
    uint32_t reserved1         : 4;  // Reserved
    uint32_t disable_int       : 1;  // Disable interrupts
    uint32_t reserved2         : 6;  // Reserved
    uint32_t skip_nr           : 5;  // Skip descriptor
} ScCmxDmaConfigBits;

// 2D transaction type
typedef struct ScCmxDmaTransactionList {
    void *link_address;  // pointer to the next element in linked list
    union {
        ScCmxDmaConfigBits cfg_bits;
        uint32_t full_cfg_register;
    } cfg_link;
    void *src;  // Pointer to the source of the data transfer
    void *dst;  // Pointer to the destination
    uint32_t length;  // Transaction length
    uint32_t planes_no;  // Number of planes
    uint32_t src_width;  // Bytes of data required from one line of source
    uint32_t src_stride;  // Length in bytes from start of one line of data,
                         // to start of next line of data
    uint32_t dst_width;  // Bytes of data required from one line of destination
    uint32_t dst_stride;  // Length in bytes from start of one line of data,
                         // to start of next line of data
    uint32_t src_plane_stride;  // Source plane stride
    uint32_t dst_plane_stride;  // Destination plane stride

    // Additional members which don't belong to DESCRIPTOR definition i
    // as per CMXDMA HW spec, but are used by Leon for various purposes.
    uint32_t sw_id;
    uint32_t current_id;
} ScCmxDmaTransaction __attribute__ ((aligned(8)));  // dmaTransactionLists must be aligned to 64 bit
                                                       // boundaries (This is a requirement of the
                                                       // hardware DMA engine)

typedef struct ScCmxDmaTransactionHnd {
    ScCmxDmaTransaction *head;
    ScCmxDmaTransaction *tail;
} ScCmxDmaTransactionHnd;

typedef struct {
    uint8_t priority;
    uint8_t burst_size;
} ScCmxDmaConfParams;

typedef struct {
    ScCmxDmaTransaction *tail;
    uint32_t sw_id;
    uint8_t initialized;
} ScCmxDmaStatusInfo;

#ifdef SC_CMX_DMA_BACK_COMPATIBLE
#define DMA_1D_TRANSACTION DRV_CMX_DMA_1D_TRANSACTION
#define DMA_2D_TRANSACTION DRV_CMX_DMA_2D_TRANSACTION
// FIXME use the right value here
#define DMA_MUTEX_COMMON DRV_CMX_DMA_MUTEX_ID_LA_0

// definitions needed for old driver wrappers
typedef ScCmxDmaTransaction dmaTransactionList;
typedef ScCmxDmaTransaction dmaTransactionList_t;
typedef int32_t dmaRequesterId;

#endif

#endif  // COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_SHAVE_INCLUDE_SCCMXDMADEFINES_H_

///@}
