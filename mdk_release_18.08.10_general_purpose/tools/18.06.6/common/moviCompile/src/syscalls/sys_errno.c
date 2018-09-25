/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys_errno.c
 *  Description:   Simple implementation for 'errno'
 *  --------------------------------------------------------------------------- */

#include "shave_syscalls.h"


// 'errno' is used for error handling in the ISO C library
#undef errno
int errno = 0;

__attribute__((weak)) int* __errno(void) {
  return &errno;
}
