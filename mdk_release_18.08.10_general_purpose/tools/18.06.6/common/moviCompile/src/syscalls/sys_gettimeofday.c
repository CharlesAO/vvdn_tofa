/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys_gettimeofday.c
 *  Description:   Skeleton for system call 'gettimeofday'
 *  --------------------------------------------------------------------------- */

#include "shave_syscalls.h"


// Implements the system get time of day functionality.  Since SHAVE does not
// support real-time clock information, this will always fail.
__attribute__((weak)) int gettimeofday(struct timeval*__restrict _unusedArg1, struct timezone*__restrict _unusedArg2)
{
    errno = ENOSYS;
    return -1;
}
