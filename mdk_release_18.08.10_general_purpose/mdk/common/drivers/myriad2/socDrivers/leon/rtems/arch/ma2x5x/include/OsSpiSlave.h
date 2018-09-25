///
/// @file OsSpiSlave.h
/// 
/// 
/// @defgroup OsSpiSlave OsSpiSlave Driver
/// @{
/// @brief SpiSlave Driver API.
///

#ifndef OSSPISLAVE_H__
#define OSSPISLAVE_H__

#include "OsSpiCommon.h"

struct spiSlaveInitSettings_t
{
    struct spiGeneralSettings_t general;

    struct spiSlaveTransferSettings_t settings;
};

typedef enum {
    SPI_SLAVE_RXTX           = 3,
    SPI_SLAVE_SET_TIMEOUT_MS = 4
} SpiSlaveIoctls_t;

struct spiSlaveTransfer_t
{
    void *rxBuffer;
    void *txBuffer;
    size_t size;
};

extern rtems_driver_address_table spiSlaveDrvTbl;

#endif

///@}
