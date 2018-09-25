// ***************************************************************************
// Copyright (C) 2011 Movidius Ltd. All rights reserved
// ---------------------------------------------------------------------------
// File       : limits.h
// Description: Partial implementation of standard library stdlib.c
// ---------------------------------------------------------------------------
// HISTORY
// Version        | Date       | Owner         | Purpose
// ---------------------------------------------------------------------------
// 0.1            | 22.06.2011 | Alex Dura     | Initial version
// ***************************************************************************
#include "stdio.h"
#include <stdlib.h>
#include "stddef.h"
#include "stdint.h"
#include "limits.h"
#include "stddef.h"
#include "string.h"
#include "float.h"
#include "ctype.h"
#include "wchar.h"
#include <sys/reent.h>
#include "mvHelpersApi.h"

typedef void* __malloc_ptr_t;

// malloc, free
#define MALLOC_PRINTF(...)
#define FREE_PRINTF(...)

//Default heap of 6 KB, same as default stack
#ifndef DEFAULT_HEAP_SIZE
#define DEFAULT_HEAP_SIZE (6*1024)
#endif

#define CONSTRUCTORS __attribute__((section(".init")))
#define DESTRUCTORS __attribute__((section(".fini")))

#define MCB_MAGIC 0x42434D4C /* little-endian 'LMCB' */

//Some atexit forward declarations
enum __atexit_types
{
    __et_atexit,
    __et_onexit,
    __et_cxa
};

void __call_exitprocs(int, void*);
int __register_exitproc(int, void (*fn)(void), void*, void*);

//Allocate default heap size of default size bytes.
//Since the type of this array is uint32_t we need to divide by 4 when
//declaring the section
uint32_t __attribute__((section(".heapSection.data"))) defaultHeap[DEFAULT_HEAP_SIZE
                / 4];

uint32_t *__heap_start__;
uint32_t *__heap_end__;

static char initialInputBuffer[70] =
                "The Movidius MDK default implementation does not support user input\n";

//static void *heap_ptr;

// Function that calls global constructors before the main() function
// It is placed in separate .init section before text section
void CONSTRUCTORS __mvInit(void)
{
    typedef void (*pfunc)(void);

    extern pfunc CTORS_BEGIN[];
    extern pfunc CTORS_END[];

    pfunc *p;

    for (p = CTORS_BEGIN; p < CTORS_END; p++)
        (*p)();
}

// Redefined function - automatically managed by the compiler,
// filling a list with destructors pointers
// this implementation is derived from the newlib implementation of __cxa_atexit
int __cxa_atexit(void (*fn)(void), void * arg, void * dso_handle)
{
    return __register_exitproc(__et_cxa, fn, arg, dso_handle);
}

// Function that calls global destructors after the end of main() 
// It calls the destructors in order viceversa of the constructors
// Function is placed in separate .fini section after text section
void DESTRUCTORS __mvFini(void)
{
    //Call the entire atexit stack
    __call_exitprocs(__et_onexit, NULL);
    __call_exitprocs(__et_cxa, NULL);
    __call_exitprocs(__et_atexit, NULL);
}

void __mvHeapInit(void)
{
    uint32_t heap_size = 0;

    __heap_start__ = &defaultHeap[0];
    __heap_end__ = &defaultHeap[DEFAULT_HEAP_SIZE / 4];

    heap_size = DEFAULT_HEAP_SIZE;

    memset(__heap_start__, 0x00, heap_size);

    //Also, set the initial read pointer here so that we don't have a arbitrary value in there
    mvInputBuffer = initialInputBuffer;

    //printf("Heap size initialized: %u\n", heap_size);
}

void mvSetHeap(uint32_t start_address, uint32_t size)
{
    __heap_start__ = (uint32_t *) start_address;
    __heap_end__ = (uint32_t *) (start_address + size - 1);

    //And initialize the heap
    memset(__heap_start__, 0x00, size);
}

typedef struct
{
    uint32_t magic; // 'LMCB'
    uint32_t used_size; // [31]: `used` bit; [30:0]: size (incl header)
    uint32_t data[];
} MCB_t, *MCB_p;

void *_malloc_r(struct _reent * unused, size_t size)
{
    (void) unused; // "use" the variables to hush the compiler warning.

    // initialize heap, if need be (only on first call)
    if ((*(uint32_t*) __heap_start__) != MCB_MAGIC)
    {
        *(uint32_t*) __heap_start__ = MCB_MAGIC;
        *(1 + (uint32_t*) __heap_start__) = (uint32_t) __heap_end__
                                            - (uint32_t) __heap_start__;
    }
    if (!size)
        return 0;
    // include MCB's header in size, and align to 8n
    size = (size + 8 + 7) & ~7;
    // start scanning the MCBs
    MCB_p t = (MCB_p) __heap_start__;
    while ((uint32_t) t < (uint32_t) __heap_end__)
    {
        uint32_t bsize = t->used_size & ~0x80000000;
        MALLOC_PRINTF( "  @0x%08X { size=0x%X, %sused }: ", t, bsize, ( t->used_size >> 31 )?"":"not " );
        // if the current block is invalid, fail
        if (t->magic != MCB_MAGIC)
        {
            MALLOC_PRINTF( "INVALID\n" );
            return 0;
        }
        // if the current block is free
        if (!(t->used_size >> 31))
        {
            // if the current block is large enough
            if (bsize >= size)
            {
                // create a new MCB with the remaining memory
                uint32_t next_size = t->used_size - size;
                MALLOC_PRINTF( "ok, left=0x%X", next_size );
                // IF there's any memory left
                if (next_size)
                {
                    // create a new free MCB
                    *(uint32_t*) (size + (uint32_t) t) = MCB_MAGIC;
                    *(1 + (uint32_t*) (size + (uint32_t) t)) = next_size;
                    MALLOC_PRINTF( "; next@0x%08X\n", size+(uint32_t)t );
                }
                else
                {
                    MALLOC_PRINTF( "; end\n", size+(uint32_t)t );
                }
                // change the size of the current block
                t->used_size = 0x80000000 | size;
                //// zero-fill it
                //memsetx( t->data, 0, size-4 );
                // return the allocated block
                return t->data;
            }
        }
        else
        {
            MALLOC_PRINTF( "skipping\n" );
        }
        t = (MCB_p) (bsize + (uint32_t) t);
    }MALLOC_PRINTF( "\n" );
    // if we got here, no block is large enough, so fail
    return 0;
}

void _free_r(struct _reent * unused, __malloc_ptr_t ptrLoc)
{
    (void) unused; // "use" the variables to hush the compiler warning.

    void* ptr;
    ptr = (void*) ptrLoc;
    // check that the pointer is aligned properly
    if ((uint32_t) ptr & 7)
        return;
    // check that the pointer is inside the heap
    MCB_p t = (MCB_p) ((uint32_t) ptr - 8), t2;
    if ((uint32_t) t < (uint32_t) __heap_start__ || (uint32_t) t
                                                    >= (uint32_t) __heap_end__)
        return;
    // pass 1: free the block
    t->used_size &= ~0x80000000;
    // pass 2: defragment the free memory
    t = (MCB_p) __heap_start__;
    t2 = (MCB_p) ((t->used_size & ~0x80000000) + (uint32_t) t);
    while ((uint32_t) t2 < (uint32_t) __heap_end__)
    {
        // abort if we went past ptr
        if ((uint32_t) t > (uint32_t) ptr)
            return;
        // if the blocks at t and t2 are both free
        if (!(t->used_size & 0x80000000) && !(t2->used_size & 0x80000000))
        {
            FREE_PRINTF("  free() joining @0x%08X and @0x%08X\n", t, t2);
            // join them
            t->used_size += t2->used_size;
            t2->magic = *((uint32_t*) &"HEAP");
            t2->used_size = *((uint32_t*) &"HEAP");
        }
        else
            // go to the next
            t = t2;
        t2 = (MCB_p) ((t->used_size & ~0x80000000) + (uint32_t) t);
    }
}

void *_realloc_r(struct _reent * unused, void *ptr, size_t size)
{
    if (size != 0)
    {
        if (ptr != NULL)
        {
            void *buffer = _malloc_r(unused, size);
            if (buffer != NULL)
            {
                memcpy(buffer, ptr, size);
                _free_r(unused, ptr);
            }
            return buffer;
        }
        else
            return _malloc_r(unused, size);
    }
    else
    {
        _free_r(unused, ptr);
        return NULL;
    }
}

void *_calloc_r(struct _reent * unused, size_t num, size_t size)
{
    void *result;
    result = _malloc_r(unused, size * num);
    if (result != NULL)
    {
        memset(result, 0, size * num);
    }
    else
    {
        return NULL;
    }
    return result;
}

//And now the non-reentrant versions of the functions (just same as the above really)
void *malloc(size_t size)
{
    return _malloc_r((struct _reent*) NULL, size);
}

void free(__malloc_ptr_t ptrLoc)
{
    _free_r((struct _reent*) NULL, ptrLoc);
    return;
}

void *realloc(void *ptr, size_t size)
{
    return _realloc_r((struct _reent*) NULL, ptr, size);
}

void *calloc(size_t num, size_t size)
{
    return _calloc_r((struct _reent*) NULL, num, size);
}

typedef uint32_t guard_type;

// Test the lowest bit.
int is_initialized(guard_type* guard_object)
{
    return (*guard_object) & 1;
}

int __cxa_guard_acquire(guard_type* guard_object)
{
    return !is_initialized(guard_object);
}

void __cxa_guard_release(guard_type* guard_object)
{
    *guard_object = 1;
}

void __cxa_guard_abort(guard_type* guard_object)
{
    *guard_object = 0;
}
