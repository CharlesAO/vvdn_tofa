/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys_close.c
 *  Description:   Skeleton for system call '_close'
 *  --------------------------------------------------------------------------- */

#include "shave_syscalls.h"


// Close the specified file. Since there is no file system on SHAVE, this will always fail.
__attribute__((weak)) int _close(int file)
{
    errno = EBADF;
    return -1;
}
