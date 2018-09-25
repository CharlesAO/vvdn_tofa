/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys_nanosleep.c
 *  Description:   Skeleton for system call 'nanosleep'
 *  --------------------------------------------------------------------------- */

#include "shave_syscalls.h"


// Implements the 'nanosleep' functionality.  Since SHAVE does not support
// timed 'sleep', this will always fail.
__attribute__((weak)) int nanosleep(const struct timespec* _unusedArg1, struct timespec* _unusedArg2)
{
    errno = ENOSYS;
    return -1;
}
