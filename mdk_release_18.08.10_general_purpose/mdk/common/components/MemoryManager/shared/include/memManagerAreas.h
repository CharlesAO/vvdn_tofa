#ifndef _MEMMANAGERAREAS_H_
#define _MEMMANAGERAREAS_H_

#include "swcMutexUsage.h"
#include <stdlib.h>

extern unsigned char __ddrSpaceStart[], __ddrSpaceEnd[];            /* DDR section margins */

#ifndef MEM_MGR_MUTEX
#define MEM_MGR_MUTEX RESMGR_L0_MUTEX_30
#endif

#ifndef SYS_NMB_ALLOCS
#define SYS_NMB_ALLOCS 256
#endif

#ifndef DEFAULT_ALLOC_RATE
#define DEFAULT_ALLOC_RATE 1024 //1kb
#endif

typedef enum { FREE = 0, USED, STATIC, STACK, LARGE} allocState;
typedef enum { UNINITIALIZED = 0, INITIALIZED = 1} initState;

// Memory Areas
typedef enum{
    CMX_AREA0 = 0,
    CMX_AREA1,
    CMX_AREA2,
    CMX_AREA3,
    CMX_AREA4,
    CMX_AREA5,
    CMX_AREA6,
    CMX_AREA7,
    CMX_AREA8,
    CMX_AREA9,
    CMX_AREA10,
    CMX_AREA11,
#ifdef MA2X8X
    CMX_AREA12,
    CMX_AREA13,
    CMX_AREA14,
    CMX_AREA15,
#endif/* MA2x8x */
    CMX_TOTAL_COUNT,
    DDR_AREA = CMX_TOTAL_COUNT,
    MEM_TOTAL_COUNT
}MemMgrAreas;

typedef struct{
    void* address;                      /*address of the allocation*/
    size_t size;                        /*size of the allocator unit*/
    unsigned int state;                 /*state of the allocator(used/unused/static)*/
}Allocator;

typedef struct{
    void* address;                        /*address of the memory area*/
    size_t size;                          /*total size of the current memory area*/
    size_t usage;                         /*used memory inside current memory area*/
    size_t allocRate;                     /*size of smallest memory allocation unit*/
    unsigned int nmbOfAllocs;             /*number of allocations inside memory area*/
    Allocator allocators[SYS_NMB_ALLOCS]; /*allocator units structure*/
}MemArea;

typedef struct{
    unsigned int isInitialized;         /*manager initialized state*/
    unsigned int version;               /*structure version*/
    MemArea areas[MEM_TOTAL_COUNT];     /*areas of memory*/
}MemManag;

/*******MEMORY STRUCTURE*******/
extern MemManag __MemManager;
/******************************/

#endif
