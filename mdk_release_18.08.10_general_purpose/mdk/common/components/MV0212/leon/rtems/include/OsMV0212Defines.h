///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Definitions and types needed by the RTEMS MV0212 Board Component
///
///

#ifndef OS_MV0212_DEFINES
#define OS_MV0212_DEFINES

typedef enum
{
    BRD_UNINITIALIZED = 1,          // Board status not initialized
    BRD_INITIALIZED,                // Board initialized
    BRD_INITIALIZATION_IN_PROGRESS, // Board opened, ready to be used
    BRD_ALREADY_INITIALIZED         // Board has been already initialized
} BoardStatusCode;

typedef enum
{
    MV212_I2C_SLAVE = 0,
    MV212_I2C_MASTER,
} MV0212I2CConfigType;

typedef enum
{
    MV212_I2C0 = 0,
    MV212_I2C1,
} MV0212CamBBus;

typedef enum
{
    MV212_END = 0,
    MV212_I2C0_STATE,    // slave or master: MV0212I2CConfigType
    MV212_CAM_B_I2C_BUS, // i2c0 or i2c1: MV0212CamBBus
    // to be continued
} MV0212ConfigType;

typedef struct
{
    MV0212ConfigType type;
    int32_t value;
} BoardI2CDevConfiguration;

#endif
