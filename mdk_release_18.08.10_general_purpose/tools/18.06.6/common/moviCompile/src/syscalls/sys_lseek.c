/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys_lseek.c
 *  Description:   Skeleton for system call '_lseek'
 *  --------------------------------------------------------------------------- */

#include "shave_syscalls.h"


// Set the file offset for the specified file.  Since there is no file-system on
// SHAVE, this will always fail.
__attribute__((weak)) off_t _lseek(int file, off_t offset, int dir)
{
    errno = EBADF;
    return (off_t)(-1);
}
