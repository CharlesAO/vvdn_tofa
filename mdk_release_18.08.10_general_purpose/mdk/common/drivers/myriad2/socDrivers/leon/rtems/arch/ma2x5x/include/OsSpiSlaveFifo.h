///
/// @file OsSpiSlaveFifo.h
/// 
/// 
/// @defgroup OsSpiSlaveFifo OsSpiSlaveFifo Driver
/// @{
/// @brief SpiSlaveFifo Driver API.
/// 

#ifndef OSSPISLAVEFIFO_H__
#define OSSPISLAVEFIFO_H__

#include "OsSpiCommon.h"
#include "OsSpiSlave.h"

struct spiSlaveFifoSettings_t
{
    int dmaBlockSizeBytes;

    void *rxFifoLocation;
    u32   rxFifoSize;

    void *txFifoLocation;
    u32   txFifoSize;
};

struct spiSlaveFifoInitSettings_t
{
    struct spiGeneralSettings_t general;

    struct spiSlaveTransferSettings_t settings;

    struct spiSlaveFifoSettings_t fifo;
};

typedef enum {
    SPI_SLAVE_FIFO_RXTX           = 3,
    SPI_SLAVE_FIFO_SET_TIMEOUT_MS = 4
} SpiSlaveFifoIoctls_t;


extern rtems_driver_address_table spiSlaveFifoDrvTbl;

#endif

///@}
