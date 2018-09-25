/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys_unlink.c
 *  Description:   Skeleton for system call '_unlink'
 *  --------------------------------------------------------------------------- */

#include "shave_syscalls.h"


// Remove a link to a file.  Since there is no file-system on SHAVE, this will
// always fail.
__attribute__((weak)) int _unlink(const char* name)
{
    errno = EACCES;
    return -1;
}
