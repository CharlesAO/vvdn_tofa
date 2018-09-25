/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2017-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys_rename.c
 *  Description:   Skeleton for system call 'rename'
 *  --------------------------------------------------------------------------- */

#include "shave_syscalls.h"


// File control. This is not supported on SHAVE so it will always fail.
__attribute__((weak)) int rename(const char* old, const char* new)
{
    errno = ENOSYS;
    return -1;
}
