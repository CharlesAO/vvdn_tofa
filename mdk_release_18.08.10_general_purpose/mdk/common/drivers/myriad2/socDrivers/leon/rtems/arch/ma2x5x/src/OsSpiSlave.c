///
/// @file OsSpiSlave.c
/// 
/// 
/// @ingroup OsSpiSlave
/// @{
///

#include "OsSpiSlave.h"

#include "HglSpi.h"

#include <rtems.h>
#include <rtems/libio.h>
#include <bsp.h>
#include <bsp/irq-generic.h>

#include <assert.h>
#include <malloc.h>

#define MAX_DEVNAME 48

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

static rtems_device_driver spi_slave_initialize(
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
        struct spiSlaveInitSettings_t *settings =
            (struct spiSlaveInitSettings_t *)e;
        char devname[MAX_DEVNAME] = "/dev/spiSlave";

        struct osSpiHandler_t * handler =
            (struct osSpiHandler_t *)malloc(sizeof(struct osSpiHandler_t));
        memset(handler, 0, sizeof(*handler));

        sc = OsSpiCommonInit(handler, HGL_SPI_SLAVE,
                             &settings->settings, &settings->general, 0);
        if(sc)
            return sc; //TODO check the returned values here are always in the rtems range

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

static rtems_device_driver spi_slave_open(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    // hush the compiler warning
    UNUSED(major);
    UNUSED(minor);
    UNUSED(e);

    return RTEMS_SUCCESSFUL;
}

static rtems_device_driver spi_slave_close(
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

static rtems_device_driver spi_slave_full_dup_trf(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *rxBuff, void *txBuff, u32 size)
{
    UNUSED(major);// hush the compiler warning.

    struct osSpiHandler_t * handler =
        (struct osSpiHandler_t *)OsSpiCommonGetHandler(getSpiBlockFromMinor(minor));

    return OsSpiCommonTrfRxTx(handler, rxBuff, txBuff, size); //TODO check the returned values here are always in the rtems range

}


static rtems_device_driver spi_slave_read(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    rtems_libio_rw_args_t *rw_args = (rtems_libio_rw_args_t*) e;
    rtems_device_driver rv;

    rv = spi_slave_full_dup_trf(major, minor,
                                rw_args->buffer, NULL, rw_args->count);

    if (rv < 0)
    {
        return rv;
    }
    else
    {
        rw_args->bytes_moved = rw_args->count;
        return RTEMS_SUCCESSFUL;
    }
}

static rtems_device_driver spi_slave_write(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    rtems_libio_rw_args_t *rw_args = (rtems_libio_rw_args_t*) e;
    rtems_device_driver rv;

    rv = spi_slave_full_dup_trf(major, minor,
                                NULL, rw_args->buffer, rw_args->count);

    if (rv < 0)
    {
        return rv;
    }
    else
    {
        rw_args->bytes_moved = rw_args->count;
        return RTEMS_SUCCESSFUL;
    }
}

static rtems_device_driver spi_slave_control(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    rtems_libio_ioctl_args_t * ctl = (rtems_libio_ioctl_args_t *) e;
    rtems_device_driver rc;

    if (!ctl || !ctl->buffer)
    {
        return RTEMS_INVALID_ADDRESS;
    }

    switch (ctl->command)
    {
    case SPI_SLAVE_RXTX:
    {
        struct spiSlaveTransfer_t * trf = (struct spiSlaveTransfer_t *)ctl->buffer;
        rc = spi_slave_full_dup_trf(major, minor,
                                      trf->rxBuffer, trf->txBuffer, trf->size);
        if(rc)
        {
            ctl->ioctl_return = rc;
        }
        else
            ctl->ioctl_return = trf->size;

        return rc;
    }
    case SPI_SLAVE_SET_TIMEOUT_MS:
    {
        u32 * timeout = (u32 *)ctl->buffer;
        if(timeout == NULL)
        {
            ctl->ioctl_return = -1;

            return RTEMS_NOT_CONFIGURED; //TODO(?): RTEMS_INVALID_NUMBER ? will "conflict" (duplicate) the default return
        }
        else
        {
            struct osSpiHandler_t * handler =
                (struct osSpiHandler_t *)OsSpiCommonGetHandler(getSpiBlockFromMinor(minor));
            // TODO: 1tick = 1ms for now
            handler->ioTimeoutTicks = *timeout;
            ctl->ioctl_return = 0;

            return RTEMS_SUCCESSFUL;
        }
    } break;
    default:
        return RTEMS_INVALID_NUMBER;
    }
}

rtems_driver_address_table spiSlaveDrvTbl = {                   \
    spi_slave_initialize,                                 \
    spi_slave_open,                                       \
    spi_slave_close,                                      \
    spi_slave_read,                                       \
    spi_slave_write,                                      \
    spi_slave_control                                     \
};

///@}
