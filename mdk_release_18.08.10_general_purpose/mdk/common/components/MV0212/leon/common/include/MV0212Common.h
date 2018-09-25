///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @defgroup MV0212 MV0212
/// @defgroup MV0212Common MV0212 Common API
/// @ingroup  MV0212
/// @{
/// @brief     Common API for the MV0212 Board Component
///

#ifndef MV0212_COMMON
#define MV0212_COMMON

#include "MV0212CommonDefines.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Returns the revision number of the PCB
///
/// @param[out] brd_rev Board revision
/// @return 
///         - BRDCONFIG_SUCCESS
///         - BRDCONFIG_ERROR
///         - BRDCONFIG_I2C_SLAVE_ERROR
///         - BRDCONFIG_I2C_DRIVER_ERROR
///
int32_t BoardGetPCBRevision(uint32_t *brd_rev);

/// @brief This function initializes the basic functions of MV0212 board: I2C buses and sets up GPIOs
///
/// @param[in] init_cfg - The board configuration structure
/// @return
///         - BRDCONFIG_SUCCESS
///         - BRDCONFIG_ERROR
///         - BRDCONFIG_I2C_SLAVE_ERROR
///         - BRDCONFIG_I2C_DRIVER_ERROR
///
int32_t BoardInit(BoardConfigDesc *init_cfg);
/// @}

#ifdef __cplusplus
}
#endif

#endif
