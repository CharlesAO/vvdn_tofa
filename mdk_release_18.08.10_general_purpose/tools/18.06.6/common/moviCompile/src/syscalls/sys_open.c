/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys_open.c
 *  Description:   Skeleton for system call '_open'
 *  --------------------------------------------------------------------------- */

#include "shave_syscalls.h"


// Opens the specified file. Since there is no file-system on SHAVE, this will
// always fail.
__attribute__((weak)) int _open(const char* name, int flags, int mode)
{
    errno = EACCES;
    return -1;
}
