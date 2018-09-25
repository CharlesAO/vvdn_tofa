/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys_sbrk.c
 *  Description:   Skeleton for system call '_sbrk'
 *  --------------------------------------------------------------------------- */

#include "shave_syscalls.h"


// This is used by Newlib's implementation of 'malloc' to increase the program's
// data space on the heap.  Since we aren't using  this implementation at the
// moment, we do not need this function.
__attribute__((weak)) void* _sbrk(ptrdiff_t incr)
{
    return NULL;
}
