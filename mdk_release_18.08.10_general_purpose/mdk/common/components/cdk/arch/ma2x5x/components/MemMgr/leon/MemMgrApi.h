
/**************************************************************************************************

 @File         : MemMgrApi.h
 @Author       : MT
 @Brief        : Contains Memory management Api
 Date          : 01 - March - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : © Movidius Srl 2014, © Movidius Ltd 2015

 Description :


 **************************************************************************************************/

#ifndef __IP_MEM_MGR_API_H__
#define __IP_MEM_MGR_API_H__

/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
 **************************************************************************************************/
#include "IcTypes.h"


/**************************************************************************************************
 ~~~  Basic typdefs
 **************************************************************************************************/
typedef uint32_t MemPoolId;
#define NO_VALID_MEM 0xFFFFFFFF //Invalid mempool ID

#ifndef MAX_MEMS_NR
#define MAX_MEMS_NR 9
#endif

/**************************************************************************************************
 ~~~  Exported Functions
 **************************************************************************************************/
void        MemMgrInit   ();
void        MemMgrReset  ();
void        MemMgrResetPool(MemPoolId pool);
MemPoolId   MemMgrAddPool(void* baseAdr,  uint32_t nBytes);
void*       MemMgrAlloc  (MemPoolId pool, uint32_t nBytes);
uint32_t    MemMgrGetFreeMem(MemPoolId pool);

#endif //__IP_MEM_MGR_API_H__
