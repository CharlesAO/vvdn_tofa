#include <stdio.h>
#include <stdlib.h>
#include <svuCommonShave.h>
#include <sys/shave_system.h>
#include "swcWhoAmI.h"
#include <assert.h>

// #define SHAVE_DYN_APP_CRT_TEST

/* The heap must be set before anything else happens on the Shave. */
__attribute__((no_instrument_function))
static void initHeap(_ExecutionContext_t* Context)
{
    /*Only reinitialize heap if it is required to do so*/
    if (Context->heap_address!=(void*)0x0){
        /*Don't really want printfs normally but it helps to keep this here*/
#ifdef SHAVE_DYN_APP_CRT_TEST
            scMutexRequest(4);
            printf("I'm initializing shave heap memory now at 0x%p, size %d bytes\n", Context->heap_address, Context->heap_size);
            scMutexRelease(4);
#endif
        __setheap((void*)Context->heap_address, Context->heap_size);
    }
}

// __attribute__((inline)) 
__attribute__((no_instrument_function))
static void FastMemClr(uint64_t * src, uint32_t length)
{
    assert(!(length % 8));
    for (void * p = src; p<(void *)((uint32_t)src+length); p += 8)
    {
        *(uint64_t *)p = (uint64_t) 0;
    }
}

/* The heap must be set before anything else happens on the Shave. */
__attribute__((no_instrument_function))
static void initBss(_ExecutionContext_t* Context)
{
    /*Only reinitialize heap if it is required to do so*/
    if (Context->appbssData_address!=(void*)0x0){
        /*Don't really want printfs normally but it helps to keep this here*/
#ifdef SHAVE_DYN_APP_CRT_TEST
            scMutexRequest(4);
//TODO: update the print for bss
            printf("I'm initializing shave bss data memory now at 0x%p, size %d bytes\n", Context->appbssData_address, Context->appbssData_size);
            scMutexRelease(4);
#endif
        FastMemClr(Context->appbssData_address, Context->appbssData_size);
    }

    if (Context->groupbssData_address!=(void*)0x0){
        /*Don't really want printfs normally but it helps to keep this here*/
#ifdef SHAVE_DYN_APP_CRT_TEST
            scMutexRequest(4);
//TODO: update the print for bss
            printf("I'm initializing shave group bss data memory now at 0x%p, size %d bytes\n", Context->groupbssData_address, Context->groupbssData_size);
            scMutexRelease(4);
#endif
        FastMemClr(Context->groupbssData_address, Context->groupbssData_size);
    }
}
extern unsigned int __init_array_start[];
extern unsigned int __init_array_end[];

void __attribute__((no_instrument_function,noinline)) masterEntry(_ExecutionContext_t *Context)
{
    initHeap(Context);

    typedef void (*pfunc)();

    pfunc *p;

    // Init BSS data section
    initBss(Context);

    // Call global classes constructors defined in common area
    pfunc *CTORS_BEGIN  = (pfunc*)__init_array_start;
    pfunc *CTORS_END    = (pfunc*)__init_array_end;

    for (p = CTORS_BEGIN; p < CTORS_END; p++)
        (*p)();

    // Call module's global classes constructors
    CTORS_BEGIN = (pfunc*)Context->ctors_start;
    CTORS_END   = (pfunc*)Context->ctors_end;

    for (p = CTORS_BEGIN; p < CTORS_END; p++)
        (*p)();
}

extern unsigned int __fini_array_start[];
extern unsigned int __fini_array_end[];

void __attribute__((no_instrument_function,noinline)) masterExit(_ExecutionContext_t *Context)
{
    typedef void (*pfunc)();

    pfunc *p;

#ifdef SHAVE_DYN_APP_CRT_TEST
    scMutexRequest(4);
    printf("masterExit\n");
    scMutexRelease(4);
#endif

    // Call global classes destructors defined in common area
    pfunc *DTORS_BEGIN  = (pfunc*)__fini_array_start;
    pfunc *DTORS_END    = (pfunc*)__fini_array_end;

    for(p = DTORS_BEGIN; p < DTORS_END; p++)
        (*p)();

    // Call module's global classes constructors
    DTORS_BEGIN = (pfunc*)Context->dtors_start;
    DTORS_END   = (pfunc*)Context->dtors_end;

    for(p = DTORS_BEGIN; p < DTORS_END; p++)
        (*p)();

}

