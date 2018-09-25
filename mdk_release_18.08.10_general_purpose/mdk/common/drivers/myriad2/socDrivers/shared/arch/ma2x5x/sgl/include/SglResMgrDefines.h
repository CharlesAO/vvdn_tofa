///
/// @file SglResMgrDefines.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup SglResMgr
/// @{
/// @brief     Resource Manager defines and types
///


#ifndef _SGL_RESMGR_DEFINES_H_
#define _SGL_RESMGR_DEFINES_H_

// 0: Includes
// ----------------------------------------------------------------------------
#include <stdint.h>

#include "SglResources.h"
#include "swcWhoAmI.h"

// 1: Defines
// ----------------------------------------------------------------------------

// ID of the mutex used to protect access to ResMgr global structure.
#define SGL_RESMGR_OWN_MUTEX_ID           (SGL_MUTEX_NB - 1)
// ID of the mutexes used to provide protected execution feature.
#define SGL_RESMGR_SAFE_EXEC_L0_MUTEX_ID  (SGL_MUTEX_NB - 2)
#define SGL_RESMGR_SAFE_EXEC_L1_MUTEX_ID  (SGL_MUTEX_NB - 3)
// ID of the mutexes used to provide system-wide, inter-LEONs and inter-SHAVEs
// locks.
#define SGL_RESMGR_CPU_LOCK_SYS_MUTEX_ID   (SGL_MUTEX_NB - 4)
#define SGL_RESMGR_CPU_LOCK_LEON_MUTEX_ID  (SGL_MUTEX_NB - 5)
#define SGL_RESMGR_CPU_LOCK_SHAVE_MUTEX_ID (SGL_MUTEX_NB - 6)
// Number of mutexes in general allocation pool.
#define SGL_RESMGR_MUTEX_NB                (SGL_MUTEX_NB - 6)

// Total number of HW resources to handle.
#define SGL_RESMGR_TOTAL_RES_NB \
    (SGL_SHAVE_NB + SGL_HW_FILTER_NB + SGL_AHB_DMA_CHAN_NB + SGL_CMX_DMA_CHAN_NB \
     + SGL_LCD_NB + SGL_MIPI_TX_NB + SGL_I2C_NB + SGL_I2S_NB + SGL_SPI_NB \
     + SGL_SDIO_NB + SGL_UART_NB + SGL_GPIO_NB + SGL_GPIO_IRQ_NB \
     + SGL_SHAVE_L2_CACHE_NB + SGL_RESMGR_MUTEX_NB)

// Macro to easily translate owner.
#define SGL_RESMGR_OWNER  (1 << (swcWhoAmI() - PROCESS_LEON_OS))

// Generic field invalidation value.
#define SGL_RESMGR_INVALID 0xFF

// Value to use on group allocation request if the user doesn't need a specific
// resource.
#define SGL_RESMGR_REQ_ID_ANY 0xFF

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------

typedef enum {
    SGL_RESMGR_SHAVE = 0,
    SGL_RESMGR_HW_FILTER,
    SGL_RESMGR_AHB_DMA_CHAN,
    SGL_RESMGR_CMX_DMA_CHAN,
    SGL_RESMGR_LCD,
    SGL_RESMGR_MIPI_TX,
    SGL_RESMGR_I2C,
    SGL_RESMGR_I2S,
    SGL_RESMGR_SPI,
    SGL_RESMGR_SDIO,
    SGL_RESMGR_UART,
    SGL_RESMGR_GPIO,
    SGL_RESMGR_GPIO_IRQ,
    SGL_RESMGR_SHAVE_L2_CACHE,
    SGL_RESMGR_MUTEX,
    SGL_RESMGR_RES_TYPE_NB,
} SglResMgrType;

typedef enum {
    SGL_RESMGR_OSC1,
    SGL_RESMGR_OSC2,
    SGL_RESMGR_PLL1,
    SGL_RESMGR_PLL2,
    SGL_RESMGR_SYSCLK
} SglResMgrClk;

// Shared clock information structure.
typedef struct {
    uint32_t osc1;
    uint32_t osc2;
    uint32_t pll1_freq_khz;
    uint32_t pll2_freq_khz;
    uint32_t system_freq_khz;
} SglResMgrInfoClockEntry;

// Shared SHAVE L2C partition information structure.
typedef struct {
    uint32_t start_addr;
    uint16_t size; // In kilobytes.
} SglResMgrShaveL2cPartEntry;

// Shared information management structure.
struct SglResMgrInfo {
    SglResMgrInfoClockEntry clock_entry;
    SglResMgrShaveL2cPartEntry shave_l2c_part[SGL_SHAVE_L2C_PART_NB];
};

// Resource entry in ResMgr resource management array.
struct SglResMgrEntry {
    // User-friendly ID, for mutexes only.
    void *info;
    // Owner(s).
    uint32_t owner;
    // Type of resource.
    uint8_t type;
    // Index of the resource.
    uint8_t id;
};

// User handler on allocated resources.
// Used to know which resource it got allocated with. This is also the structure
// to use when the user wants to release an allocated structure.
typedef struct {
    uint8_t type;
    uint8_t id;
} SglResMgrHandler;

// User resource allocation request structure.
// ID can be set to SGL_RESMGR_REQ_ID_ANY if the user does not need a specific
// resource of the requested type. Mutexes cannot be requested by ID.
// info is used to request a specific mutex and is not used for other resources.
// req_status is set by ResMgr to indicate if the allocation succeeded or not
// and the reason of failure if so.
typedef struct {
    int32_t req_status;
    SglResMgrHandler *handler;
    union {
        void *info;
        uint8_t id;
    } alloc_by;
    uint8_t type;
} SglResMgrRequest;

// ResMgr internal management structure.
struct SglResMgr {
    struct SglResMgrInfo info;
    struct SglResMgrEntry res_entry[SGL_RESMGR_TOTAL_RES_NB];
    // This stores the indexes of each resource type sub-array in res_entry[].
    uint8_t res_type_index[SGL_RESMGR_RES_TYPE_NB];
    uint8_t init_done;
};

#endif /* _SGL_RESMGR_DEFINES_H_ */

///@}
