///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup MessageProtocolRingBuffer Implementation
/// @{
/// @brief    MessageProtocolRingBuffer Implementation
///

#include "MessRingBuff.h"
#include <assert.h>
#include <rtems/bspIo.h>

#define CEIL_ALIGN(len, type) ((((len)+(sizeof(type)-1)) & (~(sizeof(type)-1))))


void InitMessageRB(MessageRingBuffer *mrb, void* buffer, s32 size)
{
	mrb->allMem = buffer;

	mrb->size   = size;
	mrb->availableLength = 0;

	mrb->wrPtr  = mrb->allMem;
	mrb->rdPtr  = mrb->allMem;
	mrb->endPtr = mrb->allMem;
}

#include <rtems.h>

void *getMessageRBWrPtr(MessageRingBuffer *mrb, s32 dataReqLen)
{
	void * wrptr;
	s32 fifoUsageLength;

	fifoUsageLength = CEIL_ALIGN(dataReqLen + sizeof(dataReqLen), u32); // add padding

	if(mrb->wrPtr + fifoUsageLength >= mrb->allMem + mrb->size) 
	{ // we don't have enough space for the full packet at the end of the ringbuffer
		mrb->endPtr = mrb->wrPtr;  // mark the end of the buffer right here
		mrb->wrPtr = mrb->allMem;  // loop-iddy loop 
	}

	// check we're not overflowing all FIFO

	if(mrb->availableLength + fifoUsageLength >= mrb->size)
	{
	    printk("ovf: 0x%lX --> %ld, %ld, %ld \n",
	           (u32)mrb, mrb->availableLength, fifoUsageLength, mrb->size);
	}

	assert(mrb->availableLength + fifoUsageLength < mrb->size);
	
    // allocate data at the wrPtr and return the current one
	wrptr = mrb->wrPtr;
	mrb->wrPtr += fifoUsageLength;

	if(mrb->endPtr < mrb->wrPtr)
	{
		mrb->endPtr = mrb->wrPtr;
	}

	mrb->activeWriteSize = fifoUsageLength;

	*(typeof(dataReqLen)*)wrptr = dataReqLen;
	wrptr += sizeof(dataReqLen);

	return wrptr;

}

void finishMessageRBWrite(MessageRingBuffer *mrb)
{
	mrb->availableLength += mrb->activeWriteSize;
	mrb->activeWriteSize = 0;
}


void *getMessageRBRdPtr(MessageRingBuffer *mrb, s32 *availableLength)
{
	void *rdptr;

	if(mrb->availableLength == 0)
	{
		*availableLength = 0;
		return (void *)NULL;
	}
	else
	{
		s32 messageDataLength;
		s32 messageFullLength;

		//messageDataLength = MIN(*availableLength, mrb->availableLength);

		// Loop around if end reached
		if(mrb->rdPtr == mrb->endPtr)
		{
			mrb->rdPtr = mrb->allMem;
		}
		
        // get the data size from the message mini-header
		messageDataLength = *(typeof(availableLength))mrb->rdPtr;
		messageFullLength = sizeof(*availableLength) + CEIL_ALIGN(messageDataLength, u32);

		// save the old one, and increment FIFO read pointer
		rdptr = mrb->rdPtr + sizeof(*availableLength);
		mrb->rdPtr = mrb->rdPtr + messageFullLength;

		// notify caller of available length and free that data
		*availableLength = messageDataLength;
		mrb->availableLength -= messageFullLength;

		return rdptr;
	}
	
}
