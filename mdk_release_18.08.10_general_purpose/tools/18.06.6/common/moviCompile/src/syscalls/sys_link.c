/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys_link.c
 *  Description:   Skeleton for system call '_link'
 *  --------------------------------------------------------------------------- */

#include "shave_syscalls.h"


// Creates a new directory entry (link) for the specified file.  Since there is
// no file system on SHAVE, this will always fail.
__attribute__((weak)) int _link(const char* path1, const char* path2)
{
    errno = EACCES;
    return -1;
}
