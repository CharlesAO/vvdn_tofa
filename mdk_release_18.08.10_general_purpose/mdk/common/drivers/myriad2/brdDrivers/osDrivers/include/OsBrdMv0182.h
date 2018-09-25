/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     rtems driver for board mv0182
///
#ifndef _OS_BRD_MV_0182_H
#define _OS_BRD_MV_0182_H

// 1: Includes
// ----------------------------------------------------------------------------
#include <mv_types.h>
#include <rtems.h>

#include <OsBmx055.h>

#ifdef __cplusplus
extern "C" {
#endif

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------
typedef enum {
    DRV_OS_BRD_182_DRV_SUCCESS           = RTEMS_SUCCESSFUL, // = OS_MYR_DRV_SUCCESS,
    DRV_OS_BRD_182_DRV_ERROR             = 0x8000 , //= OS_MYR_DRV_ERROR,
    DRV_OS_BRD_182_I2C_SLAVE_ERROR,
    DRV_OS_BRD_182_I2C_DRIVER_ERROR,
    DRV_OS_BRD_182_GPIO_DRIVER_ERROR,
    DRV_OS_BRD_182_CPR_INIT_ERROR,
    DRV_OS_BRD_182_CPR_OPEN_ERORR,
    DRV_OS_BRD_182_INVALID_CONFIG,
    DRV_OS_BRD_182_ALREADY_INITIALIZED,
    DRV_OS_BRD_182_ERROR_BRD_HANDLER_NULL,
} tyOsBrd182ErrorCode;

typedef enum
{
    OS_MV0182_REV_NOT_DETECTED = 0,
    OS_MV0182_R0R1             = 1,
    OS_MV0182_R2               = 2,
    OS_MV0182_R3               = 3,
    OS_MV0182_R4               = 4,
    OS_MV0182_REV_NOT_INIT     = 5,
    OS_MV0182_R5               = 6,
    OS_MV0212                  = 7,
} tyOsMv0182PcbRevision;

typedef enum tyOsMv0182I2CConfigType
{
    MV182_I2C_SLAVE = 0,
    MV182_I2C_MASTER,
} tyOsMv0182I2CConfigType;

typedef enum tyOsMv0182CamBBus
{
    MV182_I2C0 = 0,
    MV182_I2C1,
} tyOsMv0182CamBBus;

typedef enum tyOsBoard0182ConfigType
{
    MV182_END = 0,
    MV182_I2C0_STATE, // slave or master: tyOsMv0182I2CConfigType
    MV182_CAM_B_I2C_BUS, // i2c0 or i2c1: tyOsMv0182CamBBus
    // to be continued
} tyOsBoard0182ConfigType;

typedef struct tyOsBoard0182Configuration
{
    tyOsBoard0182ConfigType type;
    int value;
}tyOsBoard0182Configuration;

// 3:  Exported Functions
// ----------------------------------------------------------------------------

/// This function initialize the basic functions of MV0182 board: I2C busses,
/// external clock generator and sets up all GPIOS and detect board revision
/// @param[in]  pointer to the preinitialised handle for this MV0182 of type tyOsBoard0182Configurations.
//  Contain i2c busses configurations and external clock configuration.
/// @param[out] pointer to storage the board handler: revision, i2c busses and gpio driver major
tyOsBrd182ErrorCode osBoard0182Initialise(tyOsBoard0182Configuration* config);

#ifdef __cplusplus
}
#endif

#endif //_OS_BRD_MV_0182_H
