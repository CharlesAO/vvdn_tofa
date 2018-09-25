/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys_environ.c
 *  Description:   Skeleton for supporting 'environ'
 *  --------------------------------------------------------------------------- */

#include "shave_syscalls.h"


// 'environ' is NULL terminated list of strings representing the environment variables.
// Since there is no environment for SHAVE, this is a simple list with one NULL entry.
char* __env[1] = { 0 };
char** environ = __env;
