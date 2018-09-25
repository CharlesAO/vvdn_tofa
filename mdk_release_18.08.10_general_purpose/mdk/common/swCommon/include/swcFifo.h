/*
* Copyright 2017 Intel Corporation.
* The source code, information and material ("Material") contained herein is
* owned by Intel Corporation or its suppliers or licensors, and title to such
* Material remains with Intel Corporation or its suppliers or licensors.
* The Material contains proprietary information of Intel or its suppliers and
* licensors. The Material is protected by worldwide copyright laws and treaty
* provisions.
* No part of the Material may be used, copied, reproduced, modified, published,
* uploaded, posted, transmitted, distributed or disclosed in any way without
* Intel's prior express written permission. No license under any patent,
* copyright or other intellectual property rights in the Material is granted to
* or conferred upon you, either expressly, by implication, inducement, estoppel
* or otherwise.
* Any license under such intellectual property rights must be express and
* approved by Intel in writing.
*/

#ifndef SWCFIFO_H__
#define SWCFIFO_H__
#include <stdint.h>

typedef struct swcFifo_t {
    uint8_t *memory;
    int32_t size;

    int32_t unreadSize;

    int32_t writeIndex;
    int32_t activeWriteSize;

    int32_t readIndex;
    int32_t activeReadSize;

} swcFifo_t;

int32_t swcFifoGetBasePtr(struct swcFifo_t * hndl, void** ptr);
int32_t swcFifoInit(struct swcFifo_t * hndl, void* buffer, int32_t size);

int32_t swcFifoPush32Bit(struct swcFifo_t * hndl, uint32_t data);
int32_t swcFifoPush16Bit(struct swcFifo_t * hndl, uint16_t data);
int32_t swcFifoPush8Bit (struct swcFifo_t * hndl, uint8_t  data);

int32_t swcFifoPop32Bit(struct swcFifo_t * hndl, uint32_t *data);
int32_t swcFifoPop16Bit(struct swcFifo_t * hndl, uint16_t *data);
int32_t swcFifoPop8Bit (struct swcFifo_t * hndl, uint8_t  *data);

int32_t swcFifoGetWritePtr(struct swcFifo_t * hndl, void ** ptr, uint32_t reqLen);
int32_t swcFifoMarkWriteDone(struct swcFifo_t * hndl);

int32_t swcFifoGetReadPtr(struct swcFifo_t * hndl, void ** ptr, uint32_t reqLen);
int32_t swcFifoMarkReadDone(struct swcFifo_t * hndl);

uint32_t swcFifoAvailable(struct swcFifo_t * hndl);
uint32_t swcFifoContigAvailable(struct swcFifo_t * hndl);

uint32_t swcFifoLength(struct swcFifo_t * hndl);

#endif
