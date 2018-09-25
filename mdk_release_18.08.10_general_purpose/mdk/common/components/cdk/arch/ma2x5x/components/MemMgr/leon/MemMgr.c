
/**************************************************************************************************

 @File         : ipMemMng.c
 @Author       : MT
 @Brief        : Contains Memory management functionality
 Date          : 01 - March - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : � Movidius Srl 2014, � Movidius Ltd 2015

 Description :
    This file allow initialisation and control of memory. Any application level memory allocation
    need to be made by using functions from this file

 **************************************************************************************************/

/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
 **************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "MemMgrApi.h"


/**************************************************************************************************
 ~~~  Basic typdefs
 **************************************************************************************************/
static struct pool_s
{
    const char *name;    //only for debug...
    uint8_t    *start;   //start addr
    uint8_t    *pos;     //current position
    uint8_t    *end;     //end addr
} ipPools[MAX_MEMS_NR];

/**************************************************************************************************
 ~~~  Local variables
 **************************************************************************************************/
static uint32_t memNr;

/**************************************************************************************************
 ~~~ Functions Implementation
 **************************************************************************************************/

void printfStatus() {
    uint32_t i;
    for (i = 0; i < MAX_MEMS_NR; i++) {
        printf("start - pos - end: %p, %p, %p \n", ipPools[i].start, ipPools[i].pos, ipPools[i].end);
    }
}

//return number of free DDR bytes for the specified pool
uint32_t MemMgrGetFreeMem(MemPoolId pool) {
    uint32_t freeBytes = 0;
    assert((ipPools[pool].end - ipPools[pool].pos)>0);
    freeBytes = ipPools[pool].end - ipPools[pool].pos;
    return freeBytes;
}
void MemMgrInit(void) {
    uint32_t i;
    for (i = 0; i < MAX_MEMS_NR; i++) {// init memory pools
        ipPools[i].name  = 0;
        ipPools[i].start = 0;
        ipPools[i].pos   = 0;
        ipPools[i].end   = 0;
    }
    memNr = 0; //no memp-oools defined yet
}

void MemMgrReset(void) {
    uint32_t i;
    for (i = 0; i < MAX_MEMS_NR; i++) {// reset memory
        ipPools[i].pos = ipPools[i].start;
    }
    //printf("MemMgrReset \n");
    //printfStatus();
}
void MemMgrResetPool(MemPoolId pool) {
    assert (pool<MAX_MEMS_NR);
    ipPools[pool].pos = ipPools[pool].start;
}

MemPoolId MemMgrAddPool(void* baseAdr, uint32_t nBytes) {
    //printf("baseAdr %x,  length %d :\n", baseAdr, nBytes);
    MemPoolId newMemPool;
    assert (memNr<MAX_MEMS_NR);
    ipPools[memNr].start = (uint8_t*) baseAdr;
    ipPools[memNr].pos   = ipPools[memNr].start;
    ipPools[memNr].end   = ipPools[memNr].start + nBytes;
    newMemPool = memNr;
    memNr++;
    //printfStatus();
    return newMemPool;
}

//IMPORTANT: LCD buffers are also allocated with these, so to help LCD dmas,
//           allign all returned values to 16Bytes
#define ALGN_TARGET 16       //ALIGN TARGET
#define ALGN (ALGN_TARGET-1)

void *MemMgrAlloc(MemPoolId pool, uint32_t nBytes) {
    //printf("pool %d, int n_bytes %d \n", pool, nBytes);

    assert(pool < MAX_MEMS_NR);
    if(0 == nBytes)
        return ((void*)0x00);
//    printf("AA %d-%d : %p, %p, %p\n", pool, nBytes, ipPools[(int)pool].start, ipPools[(int)pool].pos, ipPools[(int)pool].end);
    uint8_t *pos = ipPools[(int)pool].pos;
    uint8_t *end = ipPools[(int)pool].end;
    uint8_t *new_pos;
    //Make sure we return addr is aligned to ALGN_TARGET
    pos     = (uint8_t*)( (ALGN + (uint32_t)pos) & ~ALGN);
    //Advance new ptr. position for next alloc (i.e. skip over what's allocated now)
    new_pos = pos + nBytes;
    //printf("pool %d, n_bytes %d", pool, n_bytes);
    assert(new_pos <= end);
    //Save current ptr for next allocation
    ipPools[(uint32_t)pool].pos = new_pos;
    //printfStatus();
    return ((void*)pos);
}
