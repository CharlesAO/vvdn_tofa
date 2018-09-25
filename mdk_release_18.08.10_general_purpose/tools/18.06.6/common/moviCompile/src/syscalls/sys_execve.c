/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys_execve.c
 *  Description:   Skeleton for system call '_execve'
 *  --------------------------------------------------------------------------- */

#include "shave_syscalls.h"


// Transfer control over to a new process. This is not supported on SHAVE so it
// will always fail.
__attribute__((weak)) int _execve(const char* path, char* const argv[], char* const envp[])
{
    errno = ENOMEM;
    return -1;
}
