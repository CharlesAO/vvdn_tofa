/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys_getpid.c
 *  Description:   Skeleton for system call '_getpid'
 *  --------------------------------------------------------------------------- */

#include "shave_syscalls.h"


// There are no processes on SHAVE so this always returns 1.
__attribute__((weak)) int _getpid(void)
{
    return 1;
}
