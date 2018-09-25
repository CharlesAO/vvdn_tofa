/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2017-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys_fcntl.c
 *  Description:   Skeleton for system call '_fcntl'
 *  --------------------------------------------------------------------------- */

#include "shave_syscalls.h"


// File control. This is not supported on SHAVE so it will always fail.
__attribute__((weak)) int _fcntl(int fildes, int cmd, int arg)
{
    errno = ENOSYS;
    return -1;
}
