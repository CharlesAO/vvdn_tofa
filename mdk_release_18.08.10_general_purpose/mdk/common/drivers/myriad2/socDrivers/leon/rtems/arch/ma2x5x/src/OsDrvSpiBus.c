///
/// @file OsDrvSpiBus.c
/// 
/// 
/// @ingroup OsDrvSpiBus
/// @{
///

#include "OsDrvSpiBus.h"
#include <bsp/irq-generic.h>
#include "OsDrvCpr.h"
#include "OsCommon.h"
#include <rtems/status-checks.h>
#include <rtems/rtems/status.h>
#include "OsCross.h"
#include <rtems.h>
#include <assert.h>
#include <DrvGpio.h>
#include <mvMacros.h>

static rtems_status_code myr2SpiSendStart(rtems_libi2c_bus_t *bus)
{
    myr2SpiBusEntry_t *e = (myr2SpiBusEntry_t *) bus;
    if (e->hndl.tid == 0)
    {
        e->hndl.tid = rtems_task_self();
        return OS_MYR_DRV_SUCCESS;
    }
    else
    {
        return RTEMS_INCORRECT_STATE;
    }
}

static rtems_status_code myr2SpiSendAddr(rtems_libi2c_bus_t *bus,
                                         uint32_t addr,
                                         int rw)
{
    UNUSED(rw);// hush the compiler warning.

    myr2SpiBusEntry_t *e = (myr2SpiBusEntry_t *) bus;

    HGLSpiSetSlave(&e->hndl.hglHndl, addr);

    return OS_MYR_DRV_SUCCESS;
}

static rtems_status_code myr2SpiSendAddrUsingGpio(rtems_libi2c_bus_t *bus,
                                                  uint32_t addr,
                                                  int rw)
{
    UNUSED(rw);// hush the compiler warning.

    myr2SpiBusEntry_t *e = (myr2SpiBusEntry_t *) bus;

    e->activeSS = addr;
    DrvGpioSetPin(addr, 0);

    HGLSpiSetSlave(&e->hndl.hglHndl, 1); // just fake select a SS

    return OS_MYR_DRV_SUCCESS;
}

static s32 myr2SpiRW(myr2SpiBusEntry_t *e,
                     void *rx_buffer,
                     const void *tx_buffer,
                     s32 size)
{
    struct osSpiHandler_t * handler = &e->hndl;
    int sc;

    sc = OsSpiCommonTrfRxTx(handler, rx_buffer, tx_buffer, size);

    if(sc)
        return sc;

    // TODO: Find cleaner way
    while(HglSpiGetBusy(&e->hndl.hglHndl))
        asm volatile ("nop\n");

    return sc;
}

static int myr2SpiRead(rtems_libi2c_bus_t *bus,
                       unsigned char *rx_buffer,
                       int rx_size)
{
    myr2SpiBusEntry_t *e = (myr2SpiBusEntry_t *) bus;
    rtems_status_code sc;
    sc = myr2SpiRW(e, rx_buffer, NULL, rx_size);
    if (sc == RTEMS_SUCCESSFUL)
        return rx_size;
    else
        return -sc;
}

static int myr2SpiWrite(rtems_libi2c_bus_t *bus,
                        unsigned char *tx_buffer,
                        int tx_size)
{
    myr2SpiBusEntry_t *e = (myr2SpiBusEntry_t *) bus;
    rtems_status_code sc;
    sc = myr2SpiRW(e, NULL, tx_buffer, tx_size);

    if (sc == RTEMS_SUCCESSFUL)
        return tx_size;
    else
        return -sc;
}

static s32 getDivider(u32 requestedBaudRate)
{
    u32 sysClkKHz;

    // Get current sys clk:
    if (OsDrvCprGetSysClockKhz(&sysClkKHz) != OS_MYR_DRV_SUCCESS)
    {
        return -1;
    }

    u32 spiBlockClkKHz = sysClkKHz / 2;
    // LSB is ignored for the divider so round up to an even value
    u32 divider = ROUND_UP(spiBlockClkKHz * 1000 / requestedBaudRate, 2);

    return divider;
}

static int myr2SpiSetTrfMode(rtems_libi2c_bus_t *bus,
                             const rtems_libi2c_tfr_mode_t *tfr_mode)
{
    myr2SpiBusEntry_t *e = (myr2SpiBusEntry_t *) bus;
    int sc;

    e->baudRate = tfr_mode->baudrate;
    e->hndl.hglHndl.bytesPerWord = tfr_mode->bits_per_char / 8;
    e->cpol = tfr_mode->clock_inv;
    e->cpha = tfr_mode->clock_phs;
    e->idleChar = tfr_mode->idle_char;

    sc = HGLSpiUpdateConfiguration(&e->hndl.hglHndl,
                                   e->hndl.hglHndl.bytesPerWord,
                                   e->cpol,
                                   e->cpha,
                                   getDivider(e->baudRate));
    if (sc)
        return sc;

    return RTEMS_SUCCESSFUL;
}

static int myr2SpiIOCtl(rtems_libi2c_bus_t *bus, int cmd, void *arg)
{
    switch (cmd)
    {
    case RTEMS_LIBI2C_IOCTL_SET_TFRMODE:
        // will set the transfer mode (bit rate, clock phase and polaritiy,
        // bits per char ... according to the
        // rtems_libi2c_tfr_mode_t structure passed in.
        return -myr2SpiSetTrfMode(bus, (const rtems_libi2c_tfr_mode_t *) arg);
        break;
    case RTEMS_LIBI2C_IOCTL_READ_WRITE:
        // will send and receive data at the same time.
        return myr2SpiRW((myr2SpiBusEntry_t *) bus,
                         ((rtems_libi2c_read_write_t *) arg)->rd_buf,
                         ((rtems_libi2c_read_write_t *) arg)->wr_buf,
                         ((rtems_libi2c_read_write_t *) arg)->byte_cnt);
        break;
    default:
        // unknown commnad
        return RTEMS_INVALID_ID;
    }

    return OS_MYR_DRV_SUCCESS;
}

// Generate a STOP
static rtems_status_code myr2SpiSendStop(rtems_libi2c_bus_t *bus)
{
    myr2SpiBusEntry_t *e = (myr2SpiBusEntry_t *) bus;
    if (e->hndl.tid == rtems_task_self())
    {
        HGLSpiSetSlave(&e->hndl.hglHndl, 0);
        e->hndl.tid = 0;
        return OS_MYR_DRV_SUCCESS;
    }
    else
    {
        return RTEMS_INCORRECT_STATE;
    }
}

static rtems_status_code myr2SpiSendStopAsGpio(rtems_libi2c_bus_t *bus)
{
    myr2SpiBusEntry_t *e = (myr2SpiBusEntry_t *) bus;
    if (e->hndl.tid == rtems_task_self())
    {
        // TODO: Find cleaner way
        while(HglSpiGetBusy(&e->hndl.hglHndl))
            asm volatile ("nop\n");

        DrvGpioSetPin(e->activeSS, 1);
        HGLSpiSetSlave(&e->hndl.hglHndl, 0);
        e->hndl.tid = 0;
        return OS_MYR_DRV_SUCCESS;
    }
    else
    {
        return RTEMS_INCORRECT_STATE;
    }
}

static rtems_status_code myr2SpiInit(rtems_libi2c_bus_t *bus)
{
    myr2SpiBusEntry_t *e = (myr2SpiBusEntry_t *) bus;
    rtems_status_code sc;

    struct spiGeneralSettings_t general;
    struct spiSlaveTransferSettings_t settings;

    general.irqLevel = e->interruptPriority;
    general.spiDevice = e->hndl.hglHndl.device;
    general.useDma = e->hndl.hglHndl.useDma;

    settings.bytesPerWord = e->hndl.hglHndl.bytesPerWord;
    settings.cpol = e->cpol;
    settings.cpha = e->cpha;

    sc = OsSpiCommonInit(&e->hndl, HGL_SPI_MASTER,
                         &settings, &general,
                         getDivider(e->baudRate));
    if(sc)
        return sc;

    return RTEMS_SUCCESSFUL;
}

const rtems_libi2c_bus_ops_t myr2SpiOps =
{
    .init = myr2SpiInit,
    .send_start = myr2SpiSendStart,
    .send_stop = myr2SpiSendStop,
    .send_addr = myr2SpiSendAddr,
    .read_bytes = myr2SpiRead,
    .write_bytes = myr2SpiWrite,
    .ioctl = myr2SpiIOCtl
};

const rtems_libi2c_bus_ops_t myr2SpiSSUsingGpioOps =
{
    .init = myr2SpiInit,
    .send_start = myr2SpiSendStart,
    .send_stop = myr2SpiSendStopAsGpio,
    .send_addr = myr2SpiSendAddrUsingGpio,
    .read_bytes = myr2SpiRead,
    .write_bytes = myr2SpiWrite,
    .ioctl = myr2SpiIOCtl
};

///@}
