/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys_read.c
 *  Description:   Skeleton for system call '_read'
 *  --------------------------------------------------------------------------- */

#include "shave_syscalls.h"


// Read 'len' bytes from 'file' into 'buffer'.  Since there is no file-system
// on SHAVE, this will always fail.
__attribute__((weak)) int _read(int file, void* buffer, size_t len)
{
    errno = EBADF;
    return -1;
}
