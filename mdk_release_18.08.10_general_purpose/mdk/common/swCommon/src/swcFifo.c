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

#include <swcFifo.h>
#include <mvMacros.h>
#include <stdlib.h>

int32_t swcFifoInit(struct swcFifo_t * hndl, void* buffer, int32_t size)
{
    hndl->memory = buffer;
    hndl->size = size;

    hndl->unreadSize = 0;

    hndl->writeIndex = 0;
    hndl->activeWriteSize = 0;

    hndl->readIndex = 0;
    hndl->activeReadSize = 0;

    return 0;
}

//gets fifo base pointer to allow deallocate
int32_t swcFifoGetBasePtr(struct swcFifo_t * hndl, void** ptr)
{
    if(ptr == NULL)
        return -1;
    *ptr = hndl->memory;

    return 0;
}

int32_t swcFifoGetWritePtr(struct swcFifo_t * hndl, void ** ptr, uint32_t reqLen)
{
    int32_t returnedArea;

    if(ptr == NULL)
        return -1;

    if(hndl->activeWriteSize != 0)
        return -2;

    // check how much total size we have
    returnedArea = MIN(hndl->size - hndl->unreadSize, (int32_t)reqLen);
    // check how much data we have untill the end of FIFO
    returnedArea = MIN(hndl->size - hndl->writeIndex, returnedArea );

    // get out fast if no data avilable
    if(returnedArea == 0)
    {
        *ptr = NULL;
    }
    else
    {
        hndl->activeWriteSize = returnedArea;
        *ptr = &hndl->memory[hndl->writeIndex];
    }

    return returnedArea;
}

int32_t swcFifoMarkWriteDone(struct swcFifo_t * hndl)
{
    if(hndl->activeWriteSize == 0)
        return -2;

    hndl->writeIndex += hndl->activeWriteSize;
    if(hndl->writeIndex == hndl->size)
        hndl->writeIndex = 0;

    hndl->unreadSize += hndl->activeWriteSize;

    hndl->activeWriteSize = 0;

    return 0;
}

int32_t swcFifoGetReadPtr(struct swcFifo_t * hndl, void ** ptr, uint32_t reqLen)
{
    int32_t returnedArea;

    if(ptr == NULL)
        return -1;

    if(hndl->activeReadSize != 0)
        return -2;

    // we are limited either by the read size, or by the data available
    returnedArea = MIN(hndl->unreadSize, (int32_t)reqLen);

    // and limited to provide a contiguous array
    returnedArea = MIN(hndl->size - hndl->readIndex, returnedArea);

    if(returnedArea == 0)
    {
        *ptr = NULL;
    }
    else
    {
        hndl->activeReadSize = returnedArea;
        *ptr = &hndl->memory[hndl->readIndex];
    }

    return returnedArea;
}

int32_t swcFifoMarkReadDone(struct swcFifo_t * hndl)
{
    if(hndl->activeReadSize == 0)
        return -2;

    hndl->readIndex += hndl->activeReadSize;
    if(hndl->readIndex == hndl->size)
        hndl->readIndex = 0;

    hndl->unreadSize -= hndl->activeReadSize;

    hndl->activeReadSize = 0;

    return 0;
}

//returns the number of unused elements in the fifo
uint32_t swcFifoAvailable(struct swcFifo_t * hndl)
{
    if(hndl->size - hndl->unreadSize > 0)
        return hndl->size - hndl->unreadSize;
    else
        return 0;
}
//returns the number of maximum contiguous unused elements in the fifo
uint32_t swcFifoContigAvailable(struct swcFifo_t * hndl)
{
    if (hndl->writeIndex > hndl->readIndex){
        return MAX(hndl->size - hndl->writeIndex, hndl->readIndex);
    }else if (hndl->writeIndex < hndl->readIndex){
        return hndl->readIndex - hndl->writeIndex;
    }else if(hndl->writeIndex == hndl->readIndex){
        if(swcFifoLength(hndl) == 0)
            return MAX(hndl->size - hndl->writeIndex, hndl->readIndex);
    }
    return 0; //Fifo full
}
// returns the number of used elements in the fifo
uint32_t swcFifoLength(struct swcFifo_t * hndl)
{
    return hndl->unreadSize;
}

#define swcFifoPushNBit(hndl, data)                                        \
{                                                                          \
    if(hndl->size - hndl->unreadSize < (int32_t)sizeof(data))                  \
        return -1;                                                         \
                                                                           \
    *((__typeof__(&data))(&hndl->memory[hndl->writeIndex])) = data;        \
                                                                           \
    hndl->unreadSize += (int32_t)sizeof(data);                                 \
    hndl->writeIndex += (int32_t)sizeof(data);                                 \
    if(hndl->writeIndex == hndl->size)                                     \
        hndl->writeIndex = 0;                                              \
                                                                           \
    return 0;                                                              \
}

#define swcFifoPopNBit(hndl, data)                                          \
{                                                                           \
    if(hndl->unreadSize < (int32_t)sizeof(data))                                \
        return -1;                                                          \
                                                                            \
    data = *((__typeof__(&data))(&hndl->memory[hndl->readIndex]));          \
                                                                            \
    hndl->unreadSize -= (int32_t)sizeof(data);                                  \
    hndl->readIndex  += (int32_t)sizeof(data);                                  \
    if(hndl->readIndex == hndl->size)                                       \
        hndl->readIndex = 0;                                                \
                                                                            \
    return 0;                                                               \
}

int32_t swcFifoPush32Bit(struct swcFifo_t * hndl, uint32_t data)
{
    swcFifoPushNBit(hndl, data);
}

int32_t swcFifoPush16Bit(struct swcFifo_t * hndl, uint16_t data)
{
    swcFifoPushNBit(hndl, data);
}

int32_t swcFifoPush8Bit (struct swcFifo_t * hndl, uint8_t  data)
{
    swcFifoPushNBit(hndl, data);
}

int32_t swcFifoPop32Bit(struct swcFifo_t * hndl, uint32_t *data)
{
    swcFifoPopNBit(hndl, *data);
}

int32_t swcFifoPop16Bit(struct swcFifo_t * hndl, uint16_t *data)
{
    swcFifoPopNBit(hndl, *data);
}

int32_t swcFifoPop8Bit (struct swcFifo_t * hndl, uint8_t *data)
{
    swcFifoPopNBit(hndl, *data);
}
