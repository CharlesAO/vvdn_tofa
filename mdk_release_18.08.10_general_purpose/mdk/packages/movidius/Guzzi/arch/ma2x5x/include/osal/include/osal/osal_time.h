/* =============================================================================
* Copyright (c) 2013-2014 MM Solutions AD
* All rights reserved. Property of MM Solutions AD.
*
* This source code may not be used against the terms and conditions stipulated
* in the licensing agreement under which it has been supplied, or without the
* written permission of MM Solutions. Rights to use, copy, modify, and
* distribute or disclose this source code and its documentation are granted only
* through signed licensing agreement, provided that this copyright notice
* appears in all copies, modifications, and distributions and subject to the
* following conditions:
* THIS SOURCE CODE AND ACCOMPANYING DOCUMENTATION, IS PROVIDED AS IS, WITHOUT
* WARRANTY OF ANY KIND, EXPRESS OR IMPLIED. MM SOLUTIONS SPECIFICALLY DISCLAIMS
* ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN
* NO EVENT SHALL MM SOLUTIONS BE LIABLE TO ANY PARTY FOR ANY CLAIM, DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST
* PROFITS, OR OTHER LIABILITY, ARISING OUT OF THE USE OF OR IN CONNECTION WITH
* THIS SOURCE CODE AND ITS DOCUMENTATION.
* =========================================================================== */
/**
* @file osal_time.h
*
* @author ( MM Solutions AD )
*
*/
/* -----------------------------------------------------------------------------
*!
*! Revision History
*! ===================================
*! 05-Nov-2013 : Author ( MM Solutions AD )
*! Created
* =========================================================================== */

#ifndef  __OSAL_TIME_H__
#define  __OSAL_TIME_H__

#include <osal/osal_stdtypes.h>

#if defined(___LINUX___)
#include "linux/osal_time.h"
#elif defined(___RTEMS___)
#include "rtems/osal_time.h"
#elif defined(___WINDOWS___)
#include "windows/osal_time.h"
#elif defined(___SYMBIAN___)
#include "symbian/osal_time.h"
#elif defined(___ANDROID___)
#include "android/osal_time.h"
#elif defined(___DSPBIOS___)
#include "dspbios/osal_time.h"
#else
#endif

#define TIMEVAL_TO_MILISEC(a)    (((a.tv_usec) / 1000) + ((a.tv_sec) * 1000))

int osal_get_time(osal_timeval *time);
int osal_sleep(unsigned int secs);
int osal_usleep(unsigned int usecs);
int osal_nsleep(unsigned int nsecs);
int osal_get_timeval(osal_timeval *time);
int osal_time_add(osal_timeval *time, osal_timeval *x, osal_timeval *y);
int osal_time_sub(osal_timeval *time, osal_timeval *x, osal_timeval *y);
int osal_time_inc(osal_timeval *time, int64 usec);

#endif /* __OSAL_TIME_H__ */

