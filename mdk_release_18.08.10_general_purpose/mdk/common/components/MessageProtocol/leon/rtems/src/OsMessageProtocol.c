#include <assert.h>
#include <string.h>
#include <mv_types.h>

#include <rtems.h>
#include <rtems/libio.h>
#include <rtems/bspIo.h>

#include <OsDrvSpiSlaveCP.h>

#include "MessageProtocol.h"
#include "OsMessageProtocol.h"
#include "OsCommon.h"


#define MAX_DEVNAME 48

volatile u32 SpiSlaveVcMajor = 0;


DECLARE_OS_MESSAGING_VIRTUAL_CHANNEL(osVc0, "meta", 0, 1000, 64, 64, ".bss");


void OsMessagePassingInitialize(void)
{
    MessagePassingInitialize();
}


s32 OsMessagePassingInitializePhysicalChannel(PhysicalChannel *phyChannel,
                                              void *phyChannelContext,
                                              ChannelType ct)
{
    BaseMessagePassingInitializePhysicalChannel(phyChannel, phyChannelContext, ct);

// Register default virtual channel
    MessagePassingRegisterVirtualChannel(&osVc0.bmVC);
    return 0;
}


s32 OsMessagePassingRegisterVirtualChannel(PhysicalChannel *phyChannel, VirtualChannel *vc)
{
    UNUSED(phyChannel); // hush the compiler warning.

    MessagePassingRegisterVirtualChannel(vc);
    return 0;
}

s32 OsMessagePassingReadBlockEvent(u8 channel, void * buff, s32 size)
{
    OsVirtualChannel *vc;
    rtems_status_code rc;
    rtems_event_set events;
    s32 readSize;

    vc = (OsVirtualChannel *)MessagePassingGetVirtualChannel(channel);
    if(vc == NULL)
        return -101;

    while((readSize = BaseMessagePassingRead((VirtualChannel*)vc, buff, size)) == 0)
    {
        if(vc->rxWaitTaskId == 0)
            vc->rxWaitTaskId = rtems_task_self();

        rc = rtems_event_system_receive(
            OSCOMMON_EVENT_MPROTOCOL_VC_READ_DATA,
            RTEMS_EVENT_ALL,
            RTEMS_NO_TIMEOUT,
            &events     );
        RTEMS_CHECK_SC(rc, "Channel Read Event Receive issue !\n");
    }

    return readSize;
}


void OsMessagePassingFinalizeChannelRx(u8 channel)
{
    OsVirtualChannel * vc = (OsVirtualChannel *)MessagePassingGetVirtualChannel(channel);
    rtems_status_code sc;

    if(vc)
    {
        BaseMessagePassingFinalizeChannelRx(&vc->bmVC);

        if(vc->rxWaitTaskId != 0)
        {
            sc = rtems_event_system_send(vc->rxWaitTaskId, OSCOMMON_EVENT_MPROTOCOL_VC_READ_DATA);
            if(sc)
                printk("%s Message packet might be lost\n", __func__);
        }
    }
}



void* OsSpiPacketRxStartCb(u16 length, u8 channel, u8 flags)
{
    UNUSED(flags); // hush the compiler warning.

    return MessagePassingGetDriverRxBuffer(channel, length);
}

s32     OsSpiPacketRxDoneCb(u16 length, u8 channel, u8 flags, void* buffer)
{
    // hush the compiler warning.
    UNUSED(length);
    UNUSED(flags);
    UNUSED(buffer);

    // Mark more data as available
    OsMessagePassingFinalizeChannelRx(channel);
    return 0;
}

s32  OsSpiPacketPeOverCb(s32 *channel, void **buffer, s32 *size)
{
    MessagePassingFinalizePacketExchange(channel, buffer, size);
    return 0;
}

PacketWriteRequestCallback_t * OsMesasgePassingGetCbTxStart(PhysicalChannel pcList)
{
    switch(pcList.ct)
    {
    case SPISLAVE:
    default:
        return NULL;
    }
}

PacketReadRequestCallback_t * OsMesasgePassingGetCbRxStart(PhysicalChannel pcList)
{
    switch(pcList.ct)
    {
    case SPISLAVE:
        return OsSpiPacketRxStartCb;
    default:
        return NULL;
    }
}

PacketWriteDoneCallback_t * OsMesasgePassingGetCbTxDone(PhysicalChannel pcList)
{
    switch(pcList.ct)
    {
    case SPISLAVE:
    default:
        return NULL;
    }
}

PacketReadDoneCallback_t * OsMesasgePassingGetCbRxDone(PhysicalChannel pcList)
{
    switch(pcList.ct)
    {
    case SPISLAVE:
        return OsSpiPacketRxDoneCb;
    default:
        return NULL;
    }
}

PacketExchangeOverCallback_t * OsMesasgePassingGetCbPeOver(PhysicalChannel pcList)
{
    switch(pcList.ct)
    {
    case SPISLAVE:
        return OsSpiPacketPeOverCb;
    default:
        return NULL;
    }
}

PhysicalChannel spiPhyChannel;
spiHandler_t slvHandler;

rtems_status_code initializePhysicalChannel(void)
{
    rtems_status_code sc;
    OsMessagePassingInitialize();
    OsMessagePassingInitializePhysicalChannel(&spiPhyChannel, &slvHandler,
                                              SPISLAVE);

    sc = OsDrvSpiSlaveCPInitGlobally(
        &slvHandler,
        OsMesasgePassingGetCbTxStart(spiPhyChannel),
        OsMesasgePassingGetCbTxDone(spiPhyChannel),
        OsMesasgePassingGetCbRxStart(spiPhyChannel),
        OsMesasgePassingGetCbRxDone(spiPhyChannel),
        OsMesasgePassingGetCbPeOver(spiPhyChannel));
    return sc;
}

rtems_device_driver virtual_channel_initialize(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e
    )
{
    UNUSED(minor); // hush the compiler warning.

    if(e == NULL)
    {
        if(SpiSlaveVcMajor == 0)
        {
            rtems_status_code sc;


            sc = initializePhysicalChannel();
            RTEMS_CHECK_SC(sc, "SpiSlave not initialized properly\n");

            SpiSlaveVcMajor = major;
            return RTEMS_SUCCESSFUL;
        }
        else
        {
            return RTEMS_UNSATISFIED;
        }
    }
    else
    {
        OsVirtualChannel *vc = (OsVirtualChannel *)e;
        char devname[MAX_DEVNAME] = "/dev/";
        rtems_device_driver status;

        if(MessagePassingRegisterVirtualChannel(&vc->bmVC))
        {
            return RTEMS_UNSATISFIED;
        }

        assert(strlen((char *)vc->bmVC.name) + strlen(devname) < MAX_DEVNAME);

        // copy the name to the device string
        strcpy(&devname[strlen(devname)], (char *)vc->bmVC.name);

        if(SpiSlaveVcMajor == 0)
            return RTEMS_UNSATISFIED;

        //register with the minor number from virtual channel
        status = rtems_io_register_name(
            devname,
            major,
            (rtems_device_minor_number)vc->bmVC.id
            );

        if (status != RTEMS_SUCCESSFUL)
            rtems_fatal_error_occurred(status);

        return RTEMS_SUCCESSFUL;
    }
}

rtems_device_driver virtual_channel_open(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e
    )
{
    UNUSED(major); // hush the compiler warning.
    UNUSED(e); // hush the compiler warning.

    OsVirtualChannel *vc = (OsVirtualChannel *)MessagePassingGetVirtualChannel(minor);

    if(vc == NULL)
        return RTEMS_IO_ERROR;

    if(vc->rxWaitTaskId != 0)
        return RTEMS_IO_ERROR;

    vc->rxWaitTaskId = rtems_task_self();

    return RTEMS_SUCCESSFUL;
}

rtems_device_driver virtual_channel_close(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e
    )
{
    UNUSED(major); // hush the compiler warning.
    UNUSED(e); // hush the compiler warning.

    OsVirtualChannel *vc = (OsVirtualChannel *)MessagePassingGetVirtualChannel(minor);

    if(vc == NULL)
        return RTEMS_IO_ERROR;

    if(vc->rxWaitTaskId == 0)
        return RTEMS_IO_ERROR;

    vc->rxWaitTaskId = 0;

    return RTEMS_SUCCESSFUL;
}

rtems_device_driver virtual_channel_read(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e
    )
{
    UNUSED(major); // hush the compiler warning.

    rtems_libio_rw_args_t *rw_args = (rtems_libio_rw_args_t *) e;
    s32 readBytes;

    if ( rw_args == NULL )
        return RTEMS_IO_ERROR;

    readBytes = OsMessagePassingReadBlockEvent(minor, rw_args->buffer, rw_args->count);

    rw_args->bytes_moved = readBytes;

    if(readBytes >= 0)
        return RTEMS_SUCCESSFUL;
    else
        return RTEMS_TIMEOUT;
}

rtems_device_driver virtual_channel_write(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e
    )
{
    UNUSED(major); // hush the compiler warning.

    rtems_libio_rw_args_t *rw_args = (rtems_libio_rw_args_t *) e;
    s32 wroteBytes;

    if ( rw_args == NULL )
        return RTEMS_IO_ERROR;

    wroteBytes = MessagePassingWrite(minor, rw_args->buffer, rw_args->count);

    rw_args->bytes_moved = wroteBytes;

    if(wroteBytes > 0)
        return RTEMS_SUCCESSFUL;
    else
        return RTEMS_TIMEOUT;
}

rtems_device_driver virtual_channel_control(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e
    )
{
    // hush the compiler warning.
    UNUSED(major);
    UNUSED(minor);
    UNUSED(e);

    return RTEMS_SUCCESSFUL;
}
