///
/// @file OsSpiSlaveFifo.c
/// 
/// 
/// @ingroup OsSpiSlaveFifo
/// @{
///

#include "OsSpiSlaveFifo.h"

#include "HglSpi.h"

#include <rtems.h>
#include <rtems/libio.h>
#include <bsp.h>
#include <bsp/irq-generic.h>

#include <assert.h>
#include <malloc.h>
#include <mvMacros.h>

#include <swcFifo.h>
#include <swcLeonUtils.h>
#include <DrvLeonL2C.h>

#define MAX_DEVNAME 48

#define WAITING_RX (1<<0)
#define WAITING_TX (1<<1)

struct osSpiSlaveFifoHandler_t {
    struct osSpiHandler_t hndl;

    struct swcFifo_t rxFifo;
    struct swcFifo_t txFifo;

    u32 flags;

    u32 dmaBlockTs;
};

struct spiMinor_t {
    int reserved:16;
    int spiBlock:2;
};

static int getSpiBlockFromMinor(rtems_device_minor_number minor)
{
    struct spiMinor_t * sm = (struct spiMinor_t *)&minor;
    return sm->spiBlock;
}

static void setSpiBlockFromMinor(rtems_device_minor_number *minor,
                                 int spiBlock)
{
    struct spiMinor_t * sm = (struct spiMinor_t *)minor;
    sm->spiBlock = spiBlock;
}

static void initiateNewFifoTransfer(struct osSpiSlaveFifoHandler_t * handler)
{
    void *rxBuff, *txBuff;
    u32 rxSize, txSize, size;

    if(handler->flags == 0)
    {
        rxSize = swcFifoGetWritePtr(&handler->rxFifo, &rxBuff, handler->dmaBlockTs);
        txSize = swcFifoGetReadPtr(&handler->txFifo, &txBuff, handler->dmaBlockTs);

        if(rxBuff && txBuff)
            size = MIN(rxSize, txSize);
        else
            size = MAX(rxSize, txSize);

        handler->flags = WAITING_RX | WAITING_TX;

        if(size > 0)
            OsSpiCommonTrfRxTxNoWait(&handler->hndl, rxBuff, txBuff, size);
    }
}

static void fifoRxIsr(struct osSpiHandler_t * handler)
{
    struct osSpiSlaveFifoHandler_t * hndl =
        (struct osSpiSlaveFifoHandler_t *)handler;
    rtems_interrupt_level irqlevel = 0;

    swcFifoMarkWriteDone(&hndl->rxFifo);

    // Handle flags in a secure way
    rtems_interrupt_disable(irqlevel);
    hndl->flags &= ~WAITING_RX;
    initiateNewFifoTransfer(hndl);
    rtems_interrupt_enable(irqlevel);
}

static void fifoTxIsr(struct osSpiHandler_t * handler)
{
    struct osSpiSlaveFifoHandler_t * hndl =
        (struct osSpiSlaveFifoHandler_t *)handler;
    rtems_interrupt_level irqlevel = 0;

    swcFifoMarkReadDone(&hndl->txFifo);

    // Handle flags in a secure way
    rtems_interrupt_disable(irqlevel);
    hndl->flags &= ~WAITING_TX;
    initiateNewFifoTransfer(hndl);
    rtems_interrupt_enable(irqlevel);
}

static rtems_device_driver spi_slave_fifo_initialize(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    if(e == NULL)
    {
        return RTEMS_SUCCESSFUL;
    }
    else
    {
        rtems_device_driver sc;
        struct spiSlaveFifoInitSettings_t *settings =
            (struct spiSlaveFifoInitSettings_t *)e;
        char devname[MAX_DEVNAME] = "/dev/spiSlave";

        struct osSpiSlaveFifoHandler_t * handler =
            (struct osSpiSlaveFifoHandler_t *)malloc(
                sizeof(struct osSpiSlaveFifoHandler_t));
        memset(handler, 0, sizeof(*handler));

        sc = OsSpiCommonInit(&handler->hndl, HGL_SPI_SLAVE,   //TODO check the returned values here are always in the rtems range
                             &settings->settings, &settings->general, 0);
        if(sc)
            return sc;

        handler->hndl.osRxCb = fifoRxIsr;
        handler->hndl.osTxCb = fifoTxIsr;

        assert(settings->fifo.dmaBlockSizeBytes < 4096);
        handler->dmaBlockTs = settings->fifo.dmaBlockSizeBytes;

        assert(settings->fifo.rxFifoLocation);
        assert(settings->fifo.txFifoLocation);
        swcFifoInit(&handler->rxFifo, settings->fifo.rxFifoLocation,
                    settings->fifo.rxFifoSize);
        swcFifoInit(&handler->txFifo, settings->fifo.txFifoLocation,
                    settings->fifo.txFifoSize);

        // duplicate null termination as we are adding a digit before it
        devname[strlen(devname) + 1] = '\0';
        devname[strlen(devname) + 0] = '0' + (char)settings->general.spiDevice;

        setSpiBlockFromMinor(&minor, (char)settings->general.spiDevice);

        //register with the minor number from virtual channel
        sc = rtems_io_register_name(devname, major, minor);
        if (sc)
            return sc;

        return RTEMS_SUCCESSFUL;
    }
}

static rtems_device_driver spi_slave_fifo_open(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    UNUSED(major);// hush the compiler warning.
    UNUSED(e);// hush the compiler warning.

    struct osSpiSlaveFifoHandler_t *  handler =
        (struct osSpiSlaveFifoHandler_t *)OsSpiCommonGetHandler(
            getSpiBlockFromMinor(minor));

    handler->flags = 0;
    initiateNewFifoTransfer(handler);

    return RTEMS_SUCCESSFUL;
}

static rtems_device_driver spi_slave_fifo_close(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    // hush the compiler warning.
    UNUSED(major);
    UNUSED(minor);
    UNUSED(e);

    return RTEMS_SUCCESSFUL;
}

static rtems_device_driver spi_slave_fifo_read(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    UNUSED(major);// hush the compiler warning.

    rtems_libio_rw_args_t *rw_args = (rtems_libio_rw_args_t*) e;
    struct osSpiSlaveFifoHandler_t * handler =
        (struct osSpiSlaveFifoHandler_t *)OsSpiCommonGetHandler(getSpiBlockFromMinor(minor));
    void * rdPtr;
    void * crtPtr = rw_args->buffer;

    rw_args->bytes_moved = 0;
    while(rw_args->bytes_moved != rw_args->count)
    {
        int readyBytes = swcFifoGetReadPtr(&handler->rxFifo,
                                           &rdPtr,
                                           rw_args->count - rw_args->bytes_moved);
        if(readyBytes != 0)
        {
            memcpy(crtPtr, rdPtr, readyBytes);
            swcFifoMarkReadDone(&handler->rxFifo);

            crtPtr += readyBytes;
            rw_args->bytes_moved += readyBytes;
        }
        else
        {
            rtems_status_code rc;
            spiEvent_t ev;

            rc = OsSpiCommonWaitSPIEvent(&handler->hndl);
            ev = handler->hndl.eventFlags;
            if(ev == RTEMS_EVENT_RXOIS ||
               ev == RTEMS_EVENT_TXOIS ||
               ev == RTEMS_EVENT_RXUIS)
            {
                return RTEMS_IO_ERROR;
            }

            if(rc == RTEMS_TIMEOUT)
            {
                return RTEMS_TIMEOUT;
            }
        }
    }

    return RTEMS_SUCCESSFUL;
}

static rtems_device_driver spi_slave_fifo_write(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    UNUSED(major);// hush the compiler warning.

    rtems_libio_rw_args_t *rw_args = (rtems_libio_rw_args_t*) e;
    struct osSpiSlaveFifoHandler_t * handler =
        (struct osSpiSlaveFifoHandler_t *)OsSpiCommonGetHandler(
            getSpiBlockFromMinor(minor));
    void * wrPtr;
    void * crtPtr = rw_args->buffer;

    rw_args->bytes_moved = 0;
    while(rw_args->bytes_moved != rw_args->count)
    {
        int available = swcFifoGetWritePtr(&handler->txFifo,
                                           &wrPtr,
                                           rw_args->count - rw_args->bytes_moved);
        if(available != 0)
        {
            memcpy(wrPtr, crtPtr, available);
            swcFifoMarkWriteDone(&handler->txFifo);

            crtPtr += available;
            rw_args->bytes_moved += available;
        }
        else
        {
            rtems_task_wake_after(1); // TODO: better way required without interfering with read function
        }
    }

    return RTEMS_SUCCESSFUL;
}

static rtems_device_driver spi_slave_fifo_control(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    rtems_libio_ioctl_args_t * ctl = (rtems_libio_ioctl_args_t *) e;
    int rc;

    if (!ctl || !ctl->buffer)
    {
        return RTEMS_INVALID_ADDRESS;
    }

    switch (ctl->command)
    {
    case SPI_SLAVE_FIFO_RXTX:
    {
        struct spiSlaveTransfer_t * trf = (struct spiSlaveTransfer_t *)ctl->buffer;
        rtems_libio_rw_args_t w_args = {0}, r_args = {0};
        w_args.count = trf->size;
        r_args.count = trf->size;

        w_args.buffer = trf->txBuffer;
        r_args.buffer = trf->rxBuffer;

        rc = spi_slave_fifo_write(major, minor, &w_args);
        if(rc)
            return RTEMS_IO_ERROR;

        rc = spi_slave_fifo_read(major, minor, &r_args);
        if(rc)
            return RTEMS_IO_ERROR;

        if(w_args.bytes_moved != r_args.bytes_moved)
            return RTEMS_INVALID_NUMBER;

        ctl->ioctl_return = w_args.bytes_moved;

    }   break;
    case SPI_SLAVE_FIFO_SET_TIMEOUT_MS:
    {
        u32 * timeout = (u32 *)ctl->buffer;
        if(timeout == NULL)
        {
            ctl->ioctl_return = -1;
        }
        else
        {
            struct osSpiSlaveFifoHandler_t * handler =
                (struct osSpiSlaveFifoHandler_t *)OsSpiCommonGetHandler(getSpiBlockFromMinor(minor));
            // TODO: 1tick = 1ms for now
            handler->hndl.ioTimeoutTicks = *timeout;
            ctl->ioctl_return = 0;
        }
    } break;
    default:
        return RTEMS_INVALID_NUMBER;
    }

    return RTEMS_SUCCESSFUL;
}

rtems_driver_address_table spiSlaveFifoDrvTbl = {                   \
    spi_slave_fifo_initialize,                                 \
    spi_slave_fifo_open,                                       \
    spi_slave_fifo_close,                                      \
    spi_slave_fifo_read,                                       \
    spi_slave_fifo_write,                                      \
    spi_slave_fifo_control                                     \
};

///@}
