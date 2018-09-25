///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    Suspend LRT component: High level API implementation
///

// 1: Includes
// ----------------------------------------------------------------------------

#include <assert.h>
#include <rtems/libio.h>
#include <rtems/status-checks.h>
#include <stdlib.h>

#include "OsCommon.h"
#include "SuspendLrt.h"
#include "SuspendLrtLpApi.h"
#include "swcWhoAmI.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
static uint8_t initialized = 0;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
static inline void suspendLrtTrigger(void *arg) {
    uint32_t psr;
    UNUSED(arg);
    rtems_interrupt_disable(psr);

    SuspendLrt();

    rtems_interrupt_enable(psr);

    return;
}

// 6: Functions Implementation
// ----------------------------------------------------------------------------

rtems_status_code SuspendLrtInit(void) {

    if (swcWhoAmI() != PROCESS_LEON_RT) return RTEMS_INCORRECT_STATE;

    if (initialized == 0) {
        initialized = 1;
        BSP_Set_interrupt_type_priority(LP_INTERRUPT_NUM, POS_EDGE_INT,
                                        LP_INTERRUPT_PRIORITY);
        BSP_interrupt_register(LP_INTERRUPT_NUM, "SuspendLrt",
                               suspendLrtTrigger, NULL);
        return RTEMS_SUCCESSFUL;

    } else
        return RTEMS_RESOURCE_IN_USE;
}
