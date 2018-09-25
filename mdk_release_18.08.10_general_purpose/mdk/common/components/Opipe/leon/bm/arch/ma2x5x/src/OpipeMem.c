///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
/// @brief     Opipe - default CMX circular-buffer mem-pool(for lazy people)
///            [inspired from previous work (sippV1)]
///

#include "Opipe.h"
#include "OpipeInternals.h"

//==================================================================================
#ifndef OPIPE_MEM_SECT
#define OPIPE_MEM_SECT ".shv0.S.init"
#endif
//override eg: CCOPT += -D'OPIPE_MEM_SECT=__attribute__((section(".shv6.S.init")))'

//==================================================================================
//By default, assume people won't use it
#ifndef OPIPE_MPOOL_SZ
#define OPIPE_MPOOL_SZ 4
#endif
//override eg: COPT += -D'OPIPE_MPOOL_SZ=131072'

//==================================================================================
//The actual mempool (holds circular buffers required by all pipes)
uint8_t gOpipeMpool[OPIPE_MPOOL_SZ] ALIGNED(8) SECTION(OPIPE_MEM_SECT);

//==================================================================================
//Helpe data struct to handle
struct circBuffPool
{
    uint32_t   usrReq;  //(debug purposes): holds user requirement
    uint32_t   start;   //start addr
    uint32_t   pos;     //current position
    uint32_t   end;     //end addr
} opipePool;

//##################################################################################
void opipeInitMemPool()
{
   opipePool.start  = (uint32_t)&gOpipeMpool;
   opipePool.end    = opipePool.start + sizeof(gOpipeMpool);
   opipePool.pos    = opipePool.start;
   opipePool.usrReq = 0; //no special requests initially
}

//##################################################################################
//[size] in bytes
//return buffers are always 64bit aligned !
uint32_t opipeAllocBuff(uint32_t size)
{
    uint32_t ret, newPos;

  //Make sure we return the next 8 byte aligned address within current pool
    ret = opipePool.pos = DW_ALIGN(opipePool.pos);

  //Advance new ptr. position for next alloc (i.e. skip over what's allocated now)
    newPos = opipePool.pos + size;
    if(newPos >= opipePool.end)
    {
       opipePool.usrReq = newPos - opipePool.start; //debug
       opipeError(OPIPE_ERR_OUT_OF_MEM);  //fatal error
       return (uint32_t) NULL;
    }

  //Else, allocation was successful, adjust pos for next alloc...
    opipePool.pos = newPos;
    printInt(ret); //VCS debug
    return ret;
}
