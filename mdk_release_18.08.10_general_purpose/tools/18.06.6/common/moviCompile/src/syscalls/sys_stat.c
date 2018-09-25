/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys_stat.c
 *  Description:   Skeleton for system call '_stat'
 *  --------------------------------------------------------------------------- */

#include "shave_syscalls.h"


// Get the current status of the specified file.  Since there is no file-system
// on SHAVE, this will always fail.
__attribute__((weak)) int _stat(const char* file, struct stat* st)
{
    errno = EACCES;
    return -1;
}
