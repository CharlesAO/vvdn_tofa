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
* @file osal_stdtypes.h
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

#ifndef __OSAL_STDTYPES_H__
#define __OSAL_STDTYPES_H__

#if defined(___LINUX___)
#include "linux/osal_stdtypes.h"
#elif defined(___RTEMS___)
#include "rtems/osal_stdtypes.h"
#elif defined(___ANDROID___)
#include "android/osal_stdtypes.h"
#else
#endif

#define OSAL_PTR2PTR(type, var) ((void*)((type*)&(var)))

#define ARRAY_SIZE(arr)         ((int)(sizeof(arr) / sizeof((arr)[0])))

#define ALIGN(x, a)             ALIGN_MASK(x, (typeof(x))(a) - 1)
#define ALIGN_MASK(x, mask)     (((x) + (mask)) & ~(mask))

#undef offsetof
#ifdef __compiler_offsetof
#define offsetof(TYPE,MEMBER) __compiler_offsetof(TYPE,MEMBER)
#else   // __compiler_offsetof
#ifdef __builtin_offsetof
#define offsetof(TYPE,MEMBER) __builtin_offsetof(TYPE,MEMBER)
#else   // __builtin_offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif  // __builtin_offsetof
#endif  // __compiler_offsetof

#ifndef min_t
#define min_t(t, a, b) ({ \
	t __a = (a); \
	t __b = (b); \
	__a < __b ? __a : __b; })
#endif

#ifndef max_t
#define max_t(t, a, b) ({ \
    t __a = (a); \
    t __b = (b); \
    __a > __b ? __a : __b; })
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#undef NULL
#if defined(__cplusplus)
#define NULL 0
#else
#define NULL ((void *)0)
#endif

#define OSAL_CAST(dst, src) \
    ({dst = (typeof(dst))(&(src));})

#endif // __OSAL_STDTYPES_H__
