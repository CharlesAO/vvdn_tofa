///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup Outside World Message Protocol Implementation
/// @{
/// @brief    Outside World Message Protocol Implementation
///

#include <assert.h>
#include <limits.h>
#include <mv_types.h>
#include <string.h>
#include <DrvGpio.h>
#include <DrvTimer.h>
#include <swcLeonUtils.h>
#include <stdio.h>
#include "MessageProtocol.h"
#include <DrvSpiSlaveCP.h>
#include <OsDrvSpiSlaveCP.h>

#ifdef MYRIAD2
#define message_memcpy(dst, src, size) memcpy(dst, src, size)
#else
#define message_memcpy(dst, src, size) swcLeonMemCpy(dst, be_pointer, src, be_pointer, size)
#endif

#define MIN(a,b)								\
	({ __typeof__ (a) _a = (a);					\
		__typeof__ (b) _b = (b);				\
		_a < _b ? _a : _b; })

DECLARE_MESSAGING_VIRTUAL_CHANNEL(bmVc0, "debug_bm", 0, 1000, 64*1024, 64*1024, ".cmx.bss");

static PhysicalChannel *pcList[MAX_COUNT_MESSAGING_PHYSICAL_CHANNELS];
static s32              pcCount;

static VirtualChannel *vcList[MAX_COUNT_MESSAGING_VIRTUAL_CHANNELS];
static s32             vcCount;

void MessagePassingInitialize(void)
{
	int i;
	// Make sure we have no channels registered
	vcCount = 0;
	for(i = 0 ; i < MAX_COUNT_MESSAGING_VIRTUAL_CHANNELS ; i++)
	{
		vcList[i] = NULL;
	}

	pcCount = 0;
	for(i = 0 ; i < MAX_COUNT_MESSAGING_PHYSICAL_CHANNELS ; i++)
	{
		pcList[i] = NULL;
	}
}
		
s32 BaseMessagePassingInitializePhysicalChannel(PhysicalChannel *phyChannel,
												void *phyChannelContext,
												ChannelType ct)
{
	switch(ct)
	{
	case SPISLAVE:
	{
		phyChannel->ct = ct;
		phyChannel->context = phyChannelContext;

		pcList[pcCount++] = phyChannel;
	
		return 0;
	}break;
	// TBD: other types of physical channels
	default:
		return -1;
	}
}

s32 MessagePassingInitializePhysicalChannel(PhysicalChannel *phyChannel,
											void *phyChannelContext,
											ChannelType ct)
{
	BaseMessagePassingInitializePhysicalChannel(phyChannel, phyChannelContext, ct);

// Register default virtual channel
	MessagePassingRegisterVirtualChannel(&bmVc0);
	return 0;
}

VirtualChannel *MessagePassingGetVirtualChannel(u8 channelId)
{
	int i;
	for(i = 0 ; i < vcCount; i++)
		if(vcList[i]->id == channelId)
			return vcList[i];
	return NULL;
}

s32 MessagePassingRegisterVirtualChannel(VirtualChannel *vc)
{
	int i;

	if(vcCount >= MAX_COUNT_MESSAGING_VIRTUAL_CHANNELS)
	{
		return -5;
	}

	// Virtual channel is already registered
	if(MessagePassingGetVirtualChannel(vc->id) != NULL)
	{
		return -4;
	}

	if(pcList[0] == NULL)
	{
		return -5;
	}
	
	vc->phyChannel = pcList[0];

	// search for insertion point in descending order of priority
	for(i = vcCount ; i > 0 ; i--)
	{
		if(vcList[i-1]->priority_level < vc->priority_level)
		{
			// move down already registered channels
			vcList[i] = vcList[i-1];
		}
		else
		{
			vcList[i] = vc; // insert our channel
			vcCount++; // increment total channels number
			return 0;
		}
	}

	// channel of index 0 registered (biggest priority)
	if(i == 0)
	{
		vcList[0] = vc; // insert our channel
		vcCount++; // increment total channels number
		return 0;
	}

	return -87;
	
}


void* SpiPacketRxStartCb(u16 length, u8 channel, u8 flags)
{
    UNUSED(flags); // hush the compiler warning.

	return MessagePassingGetDriverRxBuffer(channel, length);
}

s32	SpiPacketRxDoneCb(u16 length, u8 channel, u8 flags, void* buffer)
{
    // hush the compiler warning.
    UNUSED(length);
    UNUSED(flags);
    UNUSED(buffer);

	// Mark more data as available
	MessagePassingFinalizeChannelRx(channel);
	return 0;
}

s32  SpiPacketPeOverCb(s32 *channel, void **buffer, s32 *size)
{
	MessagePassingFinalizePacketExchange(channel, buffer, size);
	return 0;
}



PacketWriteRequestCallback_t * MesasgePassingGetCbTxStart(PhysicalChannel pcList)
{
	switch(pcList.ct)
	{
	case SPISLAVE:
	default:
		return NULL;
	}
}

PacketReadRequestCallback_t * MesasgePassingGetCbRxStart(PhysicalChannel pcList)
{
	switch(pcList.ct)
	{
	case SPISLAVE:
		return SpiPacketRxStartCb;
	default:
		return NULL;
	}
}

PacketWriteDoneCallback_t * MesasgePassingGetCbTxDone(PhysicalChannel pcList)
{
	switch(pcList.ct)
	{
	case SPISLAVE:
	default:
		return NULL;
	}
}

PacketReadDoneCallback_t * MesasgePassingGetCbRxDone(PhysicalChannel pcList)
{
	switch(pcList.ct)
	{
	case SPISLAVE:
		return SpiPacketRxDoneCb;
	default:
		return NULL;
	}
}

PacketExchangeOverCallback_t * MesasgePassingGetCbPeOver(PhysicalChannel pcList)
{
	switch(pcList.ct)
	{
	case SPISLAVE:
		return SpiPacketPeOverCb;
	default:
		return NULL;
	}
}

s32 BaseMessagePassingRead(VirtualChannel *vc, void * buff, s32 size)
{
	void *rdptr;
	s32 validSize = 0;

	assert(vc); // just make sure
	
	validSize = size;
	rdptr = getMessageRBRdPtr(&vc->rxFifo, &validSize);
	
	if(rdptr && validSize > 0)
	{
		message_memcpy(buff, rdptr, validSize);
		return validSize;
	}
	else
	{
		return 0;
	}
}

s32 MessagePassingRead(u8 channel, void * buff, s32 size)
{
	VirtualChannel *vc;

	vc = (VirtualChannel *)MessagePassingGetVirtualChannel(channel);
	if(vc == NULL)
		return -101;

	return BaseMessagePassingRead(vc, buff, size);
}

s32 MessagePassingWrite(u8 channel, void * buff, s32 size)
{
	VirtualChannel *vc;
	
	void *txptr;

	vc = MessagePassingGetVirtualChannel(channel);
	if(vc == NULL)
		return -101;
	
	txptr = getMessageRBWrPtr(&vc->txFifo, size);
	
	if(txptr)
	{	   
		message_memcpy(txptr, buff, size);
		finishMessageRBWrite(&vc->txFifo);

		DrvSpiSlaveCPNotifyDataToSend(vc->phyChannel->context);

		return size;
	}
	else
	{
		return 0;
	}
}

void * MessagePassingGetDriverRxBuffer(u8 channel, s32 length)
{
	VirtualChannel * vc;

	vc = MessagePassingGetVirtualChannel(channel);

	if(vc == NULL)
	{
		return NULL;
	}
	else
	{
		return getMessageRBWrPtr(&vc->rxFifo, length);
	}
}


void BaseMessagePassingFinalizeChannelRx(VirtualChannel *vc)
{
	assert(vc);
	finishMessageRBWrite(&vc->rxFifo);
}


void MessagePassingFinalizeChannelRx(u8 channel)
{
	VirtualChannel * vc = (VirtualChannel *)MessagePassingGetVirtualChannel(channel);
	
	if(vc)
		BaseMessagePassingFinalizeChannelRx(vc);
}

s32 MessagePassingFinalizePacketExchange(s32 *channel, void **buffer, s32 *size)
{
	s32 i;
	void *txptr;
	s32 rsize;

	for(i = 0; i < vcCount; i++)
	{
		rsize = USHRT_MAX; // we could have a huge packet to send 
		txptr = getMessageRBRdPtr(&vcList[i]->txFifo, &rsize);
		if(txptr)
		{
			*channel = vcList[i]->id;
			*size = rsize;
			*buffer = txptr;
			return 1;
		}
		
	}
	return 0;
}
