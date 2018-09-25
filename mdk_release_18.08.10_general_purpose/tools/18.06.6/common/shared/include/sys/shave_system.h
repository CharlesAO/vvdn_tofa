/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2016-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys/shave_system.h
 *  Description:   Internal system interfaces for SHAVE specific configuration.
 *  --------------------------------------------------------------------------- */

#ifndef _MCC_SYS_SHAVE_SYSTEM_H_
#define _MCC_SYS_SHAVE_SYSTEM_H_ (1)


#ifndef __myriad2__
# error "sys/shave_system.h: this header is for the Myriad2 processor!"
#endif

#pragma GCC system_header

#include <sys/__moviconfig.h>

#include <stddef.h>
#include <stdint.h>


_MV_EXTERNC_BEGIN


/*
 * This macro specifies the version of the '_ExecutionContext_t' implemented by this file
 * and the supporting functions in the C RunTime (CRT).
 */
#define __EXECUTIONCONTEXTVERSION ((uint32_t)0x101)   /* Version 1.01 */


/* ----------------------------------------------------------------------------------------
 * System Execution:
 *   Extended interface for managing the initialization, finalization and termination
 *   of SHAVE programs.
 */
typedef void (*_TorFn_t)(void);

/*
 * This structure describes the elements of the Dynamic Loading Context that are handled by the CRT
 * initialization and finalization routines.  All "Entry-Points" defined for SHAVE with
 * '__attribute__((dllexport))' capture the following values:
 *
 *    IRF 19  - This is the initial stack pointer, and is also used to measure stack usage when stack-instrumentation is enabled
 *    IRF 20  - This is the lowest allowed value for the stack pointer (the stack grows towards zero)
 *              If exceeded (i.e. a lower value occurs), then the stack has overflowed
 *    IRF 21  - This is the pointer to the '_ExecutionContext_t' object described below
 */
typedef struct _ExecutionContext_t {
    uint32_t  version;               /* The version number for this definition of '_ExecutionContext_t' */

    _TorFn_t* ctors_start;           /* '__init_array_start' to allow loading the application constructors */
    _TorFn_t* ctors_end;             /* '__init_array_end' symbol marks the end of the application .ctors sections */

    _TorFn_t* dtors_start;           /* '__fini_array_start' to allow loading the application destructors */
    _TorFn_t* dtors_end;             /* '__fini_array_end' symbol marks the end of the application .dtors sections */

    uint64_t* appbssData_address;    /* The address of the bss data section */
    uint32_t  appbssData_size;       /* This field specifies the bss data section size in Bytes */
    uint64_t* groupbssData_address;  /* The address of the bss group data section */
    uint32_t  groupbssData_size;     /* This field specifies the bss group data section size in Bytes */

    void*     heap_address;          /* The address of the heap - do not initialise the heap if NULL */
    size_t    heap_size;             /* This field specifies the heap size in Bytes */
    uint32_t  stack_size;            /* This field specifies the stack size in Bytes - not used by the CRT v1.0x */
    void*     moviprof_data;         /* A pointer to 'moviProf' implementation defined data */
} _ExecutionContext_t;


#ifdef __shave__
/* Retrieve the Execution Context pointer passed to the most recent Entry-Point in IRF:I21 */
_MV_EXTERNC_DECL     const _ExecutionContext_t* __restrict __executionContext;
_MV_PURE_INLINE_DEFN const _ExecutionContext_t* __restrict __get_executioncontext(void) { return __executionContext; }

/* Support for initialization and finalization */
_MV_EXTERN_DECL      void                                  __crtinit(void);
_MV_NORETURN_DECL    int32_t                               __crtfini(int32_t);

# ifndef __STRICT_ANSI__
/* The function '_exit' is not ISO compliant, but it is a commonly expected extension */
_MV_NORETURN_DECL    void                                  _exit(int32_t);
# endif /* __STRICT_ANSI__ */
#else /* __shave__ */
/* Entry-Point interface for Leon invocation:
   ========================================== */

/* When stack usage instrumentation is enabled, this will return the lowest "top-of-stack" address since the most recent Entry-Point was called */
_MV_EXTERN_DECL      uint32_t                              _EP_get_stackhighwater(void);

/* Support for initialization and finalization */
_MV_EXTERN_DECL      int32_t                               _EP_start(int32_t, const char*[]);
_MV_EXTERN_DECL      int32_t                               _EP_crtinit(void);
_MV_EXTERN_DECL      int32_t                               _EP_crtfini(int32_t);
#endif /* __shave__ */


/* ----------------------------------------------------------------------------------------
 * Heap Management:
 *   Allows the programmer to specify the action that should be performed when an error
 *   occurs in the heap functions.  This also returns the previous value for the handler.
 *   The handler must be a SHAVE callable function, and not an Entry-Point.
 */
typedef enum _HeapErrorType_t {
    _HET_setheap_invalid_args,    /* The arguments to '__setheap' or '_EP_setheap' are invalid. */
    _HET_malloc_out_of_memory,    /* 'malloc' is unable to satisfy the request for memory. */
    _HET_free_unallocated_block,  /* Attempt to free a block of memory that is already free. */
    _HET_free_invalid_block       /* Attempt to free memory that is not in the heap. */
} _HeapErrorType_t;

typedef void (*_HeapErrorHandler_t)(_HeapErrorType_t);

#ifdef __shave__
/* Allows the programmer to provide a custom handler for Heap Error conditions */
_MV_EXTERN_DECL      _HeapErrorHandler_t                   __set_heap_error_handler(_HeapErrorHandler_t);

/* Allows the programmer to specify an alternative location for the heap. */
_MV_EXTERN_DECL      void                                  __setheap(void*, size_t);
#else /* __shave__ */
/* Entry-Point interface for Leon invocation:
   ========================================== */

/* Allows the programmer to provide a custom handler for Heap Error conditions */
_MV_EXTERN_DECL      _HeapErrorHandler_t                   _EP_set_heap_error_handler(_HeapErrorHandler_t);

/* Allows the programmer to specify an alternative location for the heap. */
_MV_EXTERN_DECL      int32_t                               _EP_setheap(void*, size_t);
#endif /* __shave__ */


_MV_EXTERNC_END


#endif /* _MCC_SYS_SHAVE_SYSTEM_H_ */
