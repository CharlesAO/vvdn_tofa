///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup Outside World Message Protocol API
/// @{
/// @brief    Outside World Message Protocol API.
///

#ifndef MESSAGE_PROTOCOL_H_
#define MESSAGE_PROTOCOL_H_

#include <mv_types.h>
#include "MessRingBuff.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifndef MAX_COUNT_MESSAGING_PHYSICAL_CHANNELS
#define MAX_COUNT_MESSAGING_PHYSICAL_CHANNELS 1
#endif

#ifndef MAX_COUNT_MESSAGING_VIRTUAL_CHANNELS
#define MAX_COUNT_MESSAGING_VIRTUAL_CHANNELS 20
#endif


typedef void* (PacketWriteRequestCallback_t)(u16 length, u8 channel, u8 flags);
typedef void* (PacketReadRequestCallback_t) (u16 length, u8 channel, u8 flags);
typedef s32	  (PacketWriteDoneCallback_t)	(u16 length, u8 channel, u8 flags, void* buffer);
typedef s32	  (PacketReadDoneCallback_t)	(u16 length, u8 channel, u8 flags, void* buffer);
typedef s32   (PacketExchangeOverCallback_t)(s32 *channel, void **buffer, s32 *size);

typedef enum {
	SPISLAVE,
} ChannelType;

typedef enum {
	TX_IDLE,
	TX_PENDING,
} txPending_t;

typedef struct {
	ChannelType ct;
	void *context;
} PhysicalChannel;

typedef struct {
	u8 id;
	u8 name[32];
	u32 priority_level;

	PhysicalChannel *phyChannel;

	MessageRingBuffer rxFifo;
	MessageRingBuffer txFifo;
} VirtualChannel;




void MessagePassingInitialize(void);
s32 MessagePassingInitializePhysicalChannel(PhysicalChannel* phyChannel, 
														 void *phyChannelContext,
														 ChannelType ct);
s32 BaseMessagePassingInitializePhysicalChannel(PhysicalChannel* phyChannel, 
														 void *phyChannelContext,
														 ChannelType ct);




s32 MessagePassingRegisterVirtualChannel(VirtualChannel *vc);

PacketWriteRequestCallback_t * MesasgePassingGetCbTxStart(PhysicalChannel pc);
PacketWriteDoneCallback_t *    MesasgePassingGetCbTxDone(PhysicalChannel pc);
PacketReadRequestCallback_t *  MesasgePassingGetCbRxStart(PhysicalChannel pc);
PacketReadDoneCallback_t *     MesasgePassingGetCbRxDone(PhysicalChannel pc);
PacketExchangeOverCallback_t * MesasgePassingGetCbPeOver(PhysicalChannel pc);

s32 MessagePassingWrite(u8 channel, void * buff, s32 size);
s32 MessagePassingRead(u8 channel, void * buff, s32 size);

s32 OsMessagePassingReadBlockEvent(u8 channel, void * buff, s32 size);

#define DECLARE_MESSAGING_VIRTUAL_CHANNEL(vcHandle, channelName,		\
										  channelId, priority,			\
										  rx_fifo_size, tx_fifo_size,	\
										  fifo_data_section)			\
	static u8 __attribute__((section(fifo_data_section))) message_rx_fifo_bm_##channelId[rx_fifo_size]; \
	static u8 __attribute__((section(fifo_data_section))) message_tx_fifo_bm_##channelId[tx_fifo_size]; \
	static VirtualChannel vcHandle =									\
	{																	\
	 .name = channelName,												\
		  .id = channelId,												\
		  .priority_level = priority,									\
		  .rxFifo = MESSAGE_RING_BUFFER(message_rx_fifo_bm_##channelId, rx_fifo_size), \
										.txFifo = MESSAGE_RING_BUFFER(message_tx_fifo_bm_##channelId, tx_fifo_size), \
										};

// low-level driver API
void * MessagePassingGetDriverRxBuffer(u8 channel, s32 length);

void MessagePassingFinalizeChannelRx(u8 channel);
s32 MessagePassingFinalizePacketExchange(s32 *channel, void **buffer, s32 *size);

VirtualChannel *MessagePassingGetVirtualChannel(u8 channelId);
s32 BaseMessagePassingRead(VirtualChannel *vc, void * buff, s32 size);

void BaseMessagePassingFinalizeChannelRx(VirtualChannel *vc);

#ifdef __cplusplus
}
#endif


#endif
