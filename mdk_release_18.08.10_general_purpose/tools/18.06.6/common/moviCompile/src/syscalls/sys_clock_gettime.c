/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys_clock_gettime.c
 *  Description:   Skeleton for system call 'clock_gettime'
 *  --------------------------------------------------------------------------- */

#include "shave_syscalls.h"


// Implements the system get time functionality.  Since SHAVE does not support
// real-time clock information, this will always fail.
__attribute__((weak)) int clock_gettime(clockid_t _unusedArg1, struct timespec* _unusedArg2)
{
    errno = ENOSYS;
    return -1;
}
