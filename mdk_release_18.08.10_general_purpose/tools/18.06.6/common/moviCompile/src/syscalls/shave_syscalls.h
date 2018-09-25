/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   shave_syscalls.h
 *  Description:   Support header for the SHAVE skeleton system-calls
 *  --------------------------------------------------------------------------- */

#ifndef SHAVE_SYSCALLS_H
#define SHAVE_SYSCALLS_H (1)


#include <stdint.h>
#include <errno.h>
#include <reent.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <_syslist.h>
#include <time.h>
#include <unistd.h>


/* 'errno' is used for error handling in the ISO C library */
#undef errno
extern int errno;


#endif /* SHAVE_SYSCALLS_H */
