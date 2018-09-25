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
*! 16-Jun-2014 : Author ( MM Solutions AD )
*! Created
* =========================================================================== */

#ifndef __OSAL_STDTYPES_RTEMS_H__
#define __OSAL_STDTYPES_RTEMS_H__

#include <stdint.h>
#include <mv_types.h>

/* --- Unsigned types --- */
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;
typedef unsigned int        Bool;

/* --- Signed types --- */
typedef signed char         int8;
typedef signed short        int16;
typedef signed int          int32;
typedef signed long long    int64;

typedef unsigned int        uint;

typedef int                 osal_handle;

#ifdef __PTRDIFF_TYPE__
typedef signed __PTRDIFF_TYPE__ intptr;
typedef unsigned __PTRDIFF_TYPE__ uintptr;
#else
// Should be sufficient for 64-bit machines also
typedef int64 intptr;
typedef uint64 uintptr;
#endif

#ifndef __SIZE_TYPE__
#define __SIZE_TYPE__
typedef unsigned int        size_t;
#endif

// ALIGN has different meaning in RTEMS machine-dependent code for SPARC
#undef ALIGN

#define ALIGN(x, a)             ALIGN_MASK(x, (typeof(x))(a) - 1)
#define ALIGN_MASK(x, mask)     (((x) + (mask)) & ~(mask))

#endif // __OSAL_STDTYPES_RTEMS_H__
