/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2017-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys_mkdir.c
 *  Description:   Skeleton for system call '_mkdir'
 *  --------------------------------------------------------------------------- */

#include "shave_syscalls.h"


// Make a directory. This is not supported on SHAVE so it will always fail.
__attribute__((weak)) int _mkdir(const char* path, mode_t mode)
{
    errno = ENOSYS;
    return -1;
}
