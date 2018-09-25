///
/// @file SglResMgr.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup SglResMgr
/// @{
/// @brief     Resource Manager common implementation for all processors
///


#include "SglResMgr.h"

#include <stdint.h>
#include <stddef.h>

#include "SglResMgrDefines.h"
#include "SglResources.h"
#include "SglCommon.h"

// Common ResMgr location.
// The build system needs to process this variable so that it can be properly
// shared between the different CPUs. This has the following consequences:
//   - The corresponding symbol must be exported so this variable must remain
//     global even if its actual scope is limited to this file.
//   - The name of this variable cannot be changed.
struct SglResMgr __attribute__((section(".cmx.resmgr"))) __SglResMgrGlobal;

// Number of resource available for each resource type.
static const uint8_t kSglResMgrEntryNbPerType[SGL_RESMGR_RES_TYPE_NB] = {
    SGL_SHAVE_NB,
    SGL_HW_FILTER_NB,
    SGL_AHB_DMA_CHAN_NB,
    SGL_CMX_DMA_CHAN_NB,
    SGL_LCD_NB,
    SGL_MIPI_TX_NB,
    SGL_I2C_NB,
    SGL_I2S_NB,
    SGL_SPI_NB,
    SGL_SDIO_NB,
    SGL_UART_NB,
    SGL_GPIO_NB,
    SGL_GPIO_IRQ_NB,
    SGL_SHAVE_L2_CACHE_NB,
    SGL_RESMGR_MUTEX_NB
};

int32_t SglResMgrGetClockEntry(SglResMgrInfoClockEntry *infoclk) {
    if (infoclk == NULL)
        return SGL_ERR_PARAM;

    *infoclk = __SglResMgrGlobal.info.clock_entry;
    return SGL_SUCCESS;
}

int32_t SglResMgrGetClockEntryField(SglResMgrClk clock_entry, uint32_t *value) {
    switch(clock_entry) {
        case SGL_RESMGR_OSC1:
            *value = __SglResMgrGlobal.info.clock_entry.osc1;
            break;
        case SGL_RESMGR_OSC2:
            *value = __SglResMgrGlobal.info.clock_entry.osc2;
            break;
        case SGL_RESMGR_PLL1:
            *value = __SglResMgrGlobal.info.clock_entry.pll1_freq_khz;
            break;
        case SGL_RESMGR_PLL2:
            *value = __SglResMgrGlobal.info.clock_entry.pll2_freq_khz;
            break;
        case SGL_RESMGR_SYSCLK:
            *value = __SglResMgrGlobal.info.clock_entry.system_freq_khz;
            break;
        default:
            return SGL_ERR_PARAM;
    }
    return SGL_SUCCESS;
}

int32_t SglResMgrSetClockEntry(SglResMgrInfoClockEntry *infoclk) {
    if (infoclk == NULL)
        return SGL_ERR_PARAM;

    __SglResMgrGlobal.info.clock_entry = *infoclk;
    return SGL_SUCCESS;
}

int32_t SglResMgrSetClockEntryField(SglResMgrClk clock_entry, uint32_t value) {
    switch(clock_entry) {
        case SGL_RESMGR_OSC1:
            __SglResMgrGlobal.info.clock_entry.osc1 = value;
            break;
        case SGL_RESMGR_OSC2:
            __SglResMgrGlobal.info.clock_entry.osc2 = value;
            break;
        case SGL_RESMGR_PLL1:
            __SglResMgrGlobal.info.clock_entry.pll1_freq_khz = value;
            break;
        case SGL_RESMGR_PLL2:
            __SglResMgrGlobal.info.clock_entry.pll2_freq_khz = value;
            break;
        case SGL_RESMGR_SYSCLK:
            __SglResMgrGlobal.info.clock_entry.system_freq_khz = value;
            break;
        default:
            return SGL_ERR_PARAM;
    }
    return SGL_SUCCESS;
}

int32_t SglResMgrGetShaveL2cPart(SglResMgrShaveL2cPartEntry *part_entry,
                                 uint8_t id) {
    if ((part_entry == NULL) || (id >= SGL_SHAVE_L2C_PART_NB))
        return SGL_ERR_PARAM;

    *part_entry = *(&__SglResMgrGlobal.info.shave_l2c_part[id]);
    return SGL_SUCCESS;
}

int32_t SglResMgrGetShaveL2cPartMap(SglResMgrShaveL2cPartEntry part_entry[]) {
    int32_t ret_val = SGL_SUCCESS;
    for (int i = 0; i < SGL_SHAVE_L2C_PART_NB; i++) {
        ret_val = SglResMgrGetShaveL2cPart(&part_entry[i], i);
        if (ret_val != SGL_SUCCESS)
            break;
    }
    return ret_val;
}

int32_t SglResMgrSetShaveL2cPart(SglResMgrShaveL2cPartEntry *part_entry,
                                 uint8_t id) {
    if ((part_entry == NULL) || (id >= SGL_SHAVE_L2C_PART_NB))
        return SGL_ERR_PARAM;

    *(&__SglResMgrGlobal.info.shave_l2c_part[id]) = *part_entry;
    return SGL_SUCCESS;
}

int32_t SglResMgrSetShaveL2cPartMap(SglResMgrShaveL2cPartEntry part_entry[]) {
    int32_t ret_val = SGL_SUCCESS;
    for (int i = 0; i < SGL_SHAVE_L2C_PART_NB; i++) {
        ret_val = SglResMgrSetShaveL2cPart(&part_entry[i], i);
        if (ret_val != SGL_SUCCESS)
            break;
    }
    return ret_val;
}

static int32_t AllocRes(SglResMgrHandler *handler, SglResMgrType type,
                        uint8_t id, uint32_t owner) {
    if ((handler == NULL)
        || (type >= SGL_RESMGR_RES_TYPE_NB)
        || ((id >= kSglResMgrEntryNbPerType[type])
            && (id != SGL_RESMGR_REQ_ID_ANY))
        || (__SglResMgrGlobal.res_type_index[type] == SGL_RESMGR_INVALID))
        return SGL_ERR_PARAM;

    int32_t ret_val = SGL_SUCCESS;
    uint8_t res_type_index = __SglResMgrGlobal.res_type_index[type];
    struct SglResMgrEntry *type_entries =
        &__SglResMgrGlobal.res_entry[res_type_index];
    // First check the user is not trying to acquire a resource it already owns.
    if ((id != SGL_RESMGR_REQ_ID_ANY) && (type_entries[id].owner == owner))
        return SGL_ERR_OWNER;
    // If ID is not specified, search for the first free one and set ID
    // accordingly but stop at the one before last: the last entry will be
    // checked in the general resource availability check below.
    if (id == SGL_RESMGR_REQ_ID_ANY) {
        int i;
        for (i = 0; i < kSglResMgrEntryNbPerType[type] - 1; i++) {
            if (!type_entries[i].owner)
                break;
        }
        id = i;
    }
    // If the identified resource is available, allocate it.
    if (!type_entries[id].owner) {
        type_entries[id].owner = owner;
        handler->type = type;
        handler->id = id;
    } else {
        ret_val = SGL_ERR_RES_BUSY;
    }
    return ret_val;
}

static int32_t AllocMutex(SglResMgrHandler *handler, void *info,
                          uint32_t owner) {
    if ((handler == NULL)
        || (__SglResMgrGlobal.res_type_index[SGL_RESMGR_MUTEX]
            == SGL_RESMGR_INVALID))
        return SGL_ERR_PARAM;

    int32_t ret_val = SGL_SUCCESS;
    uint8_t res_type_index = __SglResMgrGlobal.res_type_index[SGL_RESMGR_MUTEX];
    if (res_type_index == SGL_RESMGR_INVALID)
        return SGL_ERR_PARAM;
    struct SglResMgrEntry *mutex_entries =
        &__SglResMgrGlobal.res_entry[res_type_index];
    // Look for an entry matching the info value. In case no one is found, look
    // for the first free entry at the same time.
    uint8_t mutex_id = SGL_RESMGR_INVALID;
    for (int i = 0; i < kSglResMgrEntryNbPerType[SGL_RESMGR_MUTEX]; i++) {
        if (mutex_entries[i].owner) {
            if (mutex_entries[i].info == info) {
                mutex_id = i;
                break;
            }
        } else {
            if (mutex_id == SGL_RESMGR_INVALID)
                mutex_id = i;
        }
    }
    if (mutex_id != SGL_RESMGR_INVALID) {
        // First check the user is not trying to acquire a mutex it already owns.
        if ((mutex_entries[mutex_id].owner & owner) != 0)
            return SGL_ERR_OWNER;
        // A proper mutex was found, allocate it.
        if (!mutex_entries[mutex_id].owner)
            mutex_entries[mutex_id].info = info;
        mutex_entries[mutex_id].owner |= owner;
        handler->type = SGL_RESMGR_MUTEX;
        handler->id = mutex_id;
    } else {
        ret_val = SGL_ERR_RES_BUSY;
    }
    return ret_val;
}

int32_t SglResMgrAllocate(SglResMgrRequest *request, uint32_t owner) {
    if (request == NULL)
        return SGL_ERR_PARAM;

    if (request->type == SGL_RESMGR_MUTEX)
        request->req_status = AllocMutex(request->handler,
                                         request->alloc_by.info, owner);
    else
        request->req_status = AllocRes(request->handler, request->type,
                                       request->alloc_by.id, owner);
    return request->req_status;
}

int32_t SglResMgrRelease(SglResMgrHandler *handler , uint32_t owner) {
    if ((handler == NULL)
        || (handler->type >= SGL_RESMGR_RES_TYPE_NB)
        || (handler->id >= kSglResMgrEntryNbPerType[handler->type]))
        return SGL_ERR_PARAM;

    uint8_t res_index = __SglResMgrGlobal.res_type_index[handler->type]
                        + handler->id;
    struct SglResMgrEntry *entry = &__SglResMgrGlobal.res_entry[res_index];

    // Check that retrieved entry matches handler information and that the
    // caller is indeed (one of) the owner(s) of the resource.
    if ((handler->id != entry->id) || (handler->type != entry->type))
        return SGL_ERR_PARAM;
    if ((entry->owner & owner) == 0)
        return SGL_ERR_OWNER;

    // Remove owner. For mutexes, also clear info field if there's no other
    // owner left.
    entry->owner &= ~owner;
    if(handler->type == SGL_RESMGR_MUTEX) {
        if (!entry->owner)
            entry->info = NULL;
    }
    return SGL_SUCCESS;
}

void SglResMgrInit(void) {
    if (!__SglResMgrGlobal.init_done) {
        int res_index = 0;
        for (int i = 0; i < SGL_RESMGR_RES_TYPE_NB; i++) {
            // Set type and ID for all resource entries.
            for (int j = 0; j < kSglResMgrEntryNbPerType[i]; j++) {
                __SglResMgrGlobal.res_entry[res_index + j].type = i;
                __SglResMgrGlobal.res_entry[res_index + j].id = j;
            }
            // Compute the index of the type sub-array.
            if (kSglResMgrEntryNbPerType[i] != 0) {
                __SglResMgrGlobal.res_type_index[i] = res_index;
                res_index += kSglResMgrEntryNbPerType[i];
            } else {
                __SglResMgrGlobal.res_type_index[i] = SGL_RESMGR_INVALID;
            }
        }
        __SglResMgrGlobal.init_done = 1;
    }
}

///@}
