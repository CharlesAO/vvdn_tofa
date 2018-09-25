///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup Outside World Message Protocol API
/// @{
/// @brief    Outside World Message Protocol API.
///

#ifndef OS_MESSAGE_PROTOCOL_H_
#define OS_MESSAGE_PROTOCOL_H_


#include <rtems.h>

#include "MessageProtocol.h"

#ifdef __cplusplus
extern "C" {
#endif


    typedef struct {
        VirtualChannel bmVC;
        rtems_id rxWaitTaskId;
    } OsVirtualChannel;

#define DECLARE_OS_MESSAGING_VIRTUAL_CHANNEL(vcHandle, channelName,     \
                                             channelId, priority,       \
                                             rx_fifo_size, tx_fifo_size, \
                                             fifo_data_section)         \
    static u8 __attribute__((section(fifo_data_section))) message_rx_fifo_os_##channelId[rx_fifo_size]; \
    static u8 __attribute__((section(fifo_data_section))) message_tx_fifo_os_##channelId[tx_fifo_size]; \
    static OsVirtualChannel vcHandle =  {                               \
        .bmVC = {                                                       \
            .name = channelName,                                        \
            .id = channelId,                                            \
            .priority_level = priority,                                 \
            .rxFifo = MESSAGE_RING_BUFFER(message_rx_fifo_os_##channelId, rx_fifo_size), \
            .txFifo = MESSAGE_RING_BUFFER(message_tx_fifo_os_##channelId, tx_fifo_size), \
        },                                                              \
        .rxWaitTaskId = 0,                                              \
    };


    void OsMessagePassingInitialize(void);

    s32 OsMessagePassingInitializePhysicalChannel(PhysicalChannel *phyChannel,
                                                  void *phyChannelContext,
                                                  ChannelType ct);


    s32 OsMessagePassingRegisterVirtualChannel(PhysicalChannel *phyChannel, VirtualChannel *vc);

    s32 OsMessagePassingReadBlockEvent(u8 channel, void * buff, s32 size);

    PacketWriteRequestCallback_t * OsMesasgePassingGetCbTxStart(PhysicalChannel pcList);
    PacketReadRequestCallback_t * OsMesasgePassingGetCbRxStart(PhysicalChannel pcList);
    PacketWriteDoneCallback_t * OsMesasgePassingGetCbTxDone(PhysicalChannel pcList);
    PacketReadDoneCallback_t * OsMesasgePassingGetCbRxDone(PhysicalChannel pcList);
    PacketExchangeOverCallback_t * OsMesasgePassingGetCbPeOver(PhysicalChannel pcList);


#define DEV_VIRTUAL_CHANNEL_DRIVER_TABLE_ENTRY  {       \
        virtual_channel_initialize,                     \
            virtual_channel_open,                       \
            virtual_channel_close,                      \
            virtual_channel_read,                       \
            virtual_channel_write,                      \
            virtual_channel_control                     \
            }



#define DECLARE_COMMUNICATION_PROTOCOL_DRIVER_TABLE(drvTblName)         \
    rtems_driver_address_table drvTblName = DEV_VIRTUAL_CHANNEL_DRIVER_TABLE_ENTRY;


    rtems_device_driver virtual_channel_initialize(
        rtems_device_major_number major,
        rtems_device_minor_number minor,
        void *e
        );

    rtems_device_driver virtual_channel_open(
        rtems_device_major_number major,
        rtems_device_minor_number minor,
        void *e
        );

    rtems_device_driver virtual_channel_close(
        rtems_device_major_number major,
        rtems_device_minor_number minor,
        void *e
        );

    rtems_device_driver virtual_channel_read(
        rtems_device_major_number major,
        rtems_device_minor_number minor,
        void *e
        );

    rtems_device_driver virtual_channel_write(
        rtems_device_major_number major,
        rtems_device_minor_number minor,
        void *e
        );

    rtems_device_driver virtual_channel_control(
        rtems_device_major_number major,
        rtems_device_minor_number minor,
        void *e
        );




#ifdef __cplusplus
}
#endif


#endif
