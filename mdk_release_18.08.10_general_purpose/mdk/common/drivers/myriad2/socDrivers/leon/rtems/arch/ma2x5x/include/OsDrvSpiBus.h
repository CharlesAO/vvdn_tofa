///
/// @file OsDrvSpiBus.h
/// 
/// 
/// @defgroup OsDrvSpiBus OsSpiBus Driver
/// @{
/// @brief SpiBus Driver macros.
///

#ifndef SPI_BUS_H
#define SPI_BUS_H

#include <mv_types.h>
#include <rtems/libi2c.h>
#include <rtems.h>
#include "OsSpiCommon.h"

/// DECLARE_SPI_BUS description:
//  libi2c address will mean the slave select number from SPI block (0-3)
//  Arguments:
//      busName_ SPI Bus Name
//      busIndex_ SPI Bus Index. Can be 1, 2 or 3 for SPI1, SPI2 and SPI3
//      pin_cfg_ refer to the global myr_Spi_gpio_cfg for details
//      baudRate_ can be 1, 2 or 3 for SPI Slow, Fast or High Speed
//      interrupt_type_ check ICB for interrupt types
//      interruptPriority_ priority from 0 to 15
#define DECLARE_SPI_BUS(busName_, busIndex_, cpol_, cpha_,              \
                        bytesPerWord_, useDma_, baudRate_,              \
                        interruptPriority_)                             \
    myr2SpiBusEntry_t busName_ =                                        \
    {                                                                   \
        .bus =                                                          \
        {                                                               \
            .ops = &myr2SpiOps,                                         \
            .size = sizeof(myr2SpiBusEntry_t)                           \
        },                                                              \
        .hndl =                                                         \
        {                                                               \
            .hglHndl =                                                  \
            {                                                           \
                .device = busIndex_ - 1,                                \
                .regs = (volatile struct myr2SpiRegs *) SPI##busIndex_##_BASE_ADR, \
                .bytesPerWord = bytesPerWord_,                          \
                .useDma = useDma_,                                      \
            },                                                          \
        .irqVector = (IRQ_SPI1 + busIndex_ - 1),                        \
        },                                                              \
        .baudRate = baudRate_,                                          \
        .cpol = cpol_,                                                  \
        .cpha = cpha_,                                                  \
        .interruptPriority = interruptPriority_,                        \
    };

/// DECLARE_SPI_BUS_GPIO_SS description:
// libi2c address will mean the GPIO number (any GPIO)
//  Arguments:
//      busName_ SPI Bus Name
//      busIndex_ SPI Bus Index. Can be 1, 2 or 3 for SPI1, SPI2 and SPI3
//      pin_cfg_ refer to the global myr_Spi_gpio_cfg for details
//      baudRate_ can be 1, 2 or 3 for SPI Slow, Fast or High Speed
//      interrupt_type_ check ICB for interrupt types
//      interruptPriority_ priority from 0 to 15
#define DECLARE_SPI_BUS_GPIO_SS(busName_, busIndex_, cpol_, cpha_,      \
                                bytesPerWord_, useDma_, baudRate_,       \
                                interruptPriority_)                     \
    myr2SpiBusEntry_t busName_ =                                        \
    {                                                                   \
        .bus =                                                          \
        {                                                               \
            .ops = &myr2SpiSSUsingGpioOps,                                         \
            .size = sizeof(myr2SpiBusEntry_t)                           \
        },                                                              \
        .hndl =                                                         \
        {                                                               \
            .hglHndl =                                                  \
            {                                                           \
                .device = busIndex_ - 1,                                \
                .regs = (volatile struct myr2SpiRegs *) SPI##busIndex_##_BASE_ADR, \
                .bytesPerWord = bytesPerWord_,                          \
                .useDma = useDma_,                                      \
            },                                                          \
        .irqVector = (IRQ_SPI1 + busIndex_ - 1),                        \
        },                                                              \
        .baudRate = baudRate_,                                          \
        .cpol = cpol_,                                                  \
        .cpha = cpha_,                                                  \
        .interruptPriority = interruptPriority_,                        \
    };

typedef struct myr2SpiBusEntry
{
    rtems_libi2c_bus_t bus;
    struct osSpiHandler_t hndl;

    u32 baudRate;

    u32 cpol;
    u32 cpha;

    u32 interruptPriority;

    u32 idleChar;

    u32 activeSS;

} myr2SpiBusEntry_t;

// libi2c address will mean the slave select number from SPI block (0-3)
extern const rtems_libi2c_bus_ops_t myr2SpiOps;

// libi2c address will mean the GPIO number (any GPIO)
extern const rtems_libi2c_bus_ops_t myr2SpiSSUsingGpioOps;

#endif

///@}
