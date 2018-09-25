/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys_kill.c
 *  Description:   Skeleton for system call '_kill'
 *  --------------------------------------------------------------------------- */

#include "shave_syscalls.h"


// Sends signal sig to process pid. Since there are no processes on SHAVE, this
// will always fail.
__attribute__((weak)) int _kill(int pid, int sig)
{
    errno = ESRCH;
    return -1;
}
