/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys_fstat.c
 *  Description:   Skeleton for system call '_fstat'
 *  --------------------------------------------------------------------------- */

#include "shave_syscalls.h"


// Get the current status of the specified file. Since there is no file-system
// on SHAVE, this will always fail.
__attribute__((weak)) int _fstat(int file, struct stat * st)
{
    errno = EBADF;
    return -1;
}
