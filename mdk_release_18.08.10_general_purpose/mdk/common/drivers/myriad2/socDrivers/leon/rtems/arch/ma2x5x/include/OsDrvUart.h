///
/// @file OsDrvUart.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup OsDrvUart OsUart Driver
/// @{
/// @brief API for the UART Driver.
///

#ifndef OSDRVUART_H_
#define OSDRVUART_H_

#include <DrvUart.h>
#include <DrvUartDefines.h>
#include <OsCommon.h>

#define BIT_TIME          (16)

struct osUartFifoSettings_t
{
    void *rxFifoLocation;
    u32   rxFifoSize;

    void *txFifoLocation;
    u32   txFifoSize;
};

struct osUartAFCSettings_t
{
    u8 *location;
    u32 transferSize;
    u32 index;
};

struct osUartInitSettings_t
{
    tDrvUartCfg uartCfg;
    struct osUartFifoSettings_t fifo;
    u32 interruptLevel;
};

typedef enum {
    UART_SET_TIMEOUT_MS     = 3,
    UART_LOOPBACK_ENABLED,
    UART_LOOPBACK_DISABLED,
    UART_SET_BAUDRATE,
    UART_SW_FIFO_ENABLED
} uartIoctls_t;

extern rtems_driver_address_table uartDrvTbl;

#endif /* OSDRVUART_H_ */

///@}
