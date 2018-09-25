/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys_wait.c
 *  Description:   Skeleton for system call '_wait'
 *  --------------------------------------------------------------------------- */

#include "shave_syscalls.h"


// Wait until the status of a child process is available.  Since processes on
// SHAVE are not supported, this will always fail.
__attribute__((weak)) int _wait(int* status)
{
    errno = ECHILD;
    return -1;
}
