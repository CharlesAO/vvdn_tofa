/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys_fork.c
 *  Description:   Skeleton for system call '_fork'
 *  --------------------------------------------------------------------------- */

#include "shave_syscalls.h"


// Create a new process. This is not supported on SHAVE so it will always fail.
__attribute__((weak)) int _fork(void)
{
    errno = EAGAIN;
    return -1;
}
