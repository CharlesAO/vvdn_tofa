/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys_times.c
 *  Description:   Skeleton for system call '_times'
 *  --------------------------------------------------------------------------- */

#include "shave_syscalls.h"


// Fill the 'tms' structure pointed to by 'buffer' with time-accounting 
// information.  This is only a stub for now, but this could be implemented at a
// later stage.
__attribute__((weak)) clock_t _times(struct tms* buf)
{
    errno = ENOSYS;
    return (clock_t)(-1);
}
