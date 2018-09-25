
///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup MessageProtocolRingBuffer API
/// @{
/// @brief    MessageProtocolRingBuffer API.
///

#ifndef MESS_RING_BUFF_H_
#define MESS_RING_BUFF_H_

#include <mv_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	void * allMem;
	s32 size;
	s32 availableLength;
	s32 activeWriteSize;

	void *wrPtr;
	void *rdPtr;
	void *endPtr;
} MessageRingBuffer;

#define MESSAGE_RING_BUFFER(buffer, rb_size)	\
	{											\
		.allMem = buffer,						\
			.wrPtr = buffer,					\
			.rdPtr = buffer,					\
			.endPtr = buffer,					\
			.size = rb_size,					\
			.availableLength = 0,				\
			}

void InitMessageRB(MessageRingBuffer *mrb, void* buffer, s32 size);
void *getMessageRBWrPtr(MessageRingBuffer *mrb, s32 neededLength);
void finishMessageRBWrite(MessageRingBuffer *mrb);
void *getMessageRBRdPtr(MessageRingBuffer *mrb, s32 *availableLength);

#ifdef __cplusplus
}
#endif


#endif
