/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys_isatty.c
 *  Description:   Skeleton for system call '_isatty'
 *  --------------------------------------------------------------------------- */

#include "shave_syscalls.h"


// There are no TTYs or files on SHAVE so this will always fail.
__attribute__((weak)) int _isatty(int file)
{
    errno = EBADF;
    return 0;
}
