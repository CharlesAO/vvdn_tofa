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
* @file osal_stdlib.h
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

#ifndef  __OSAL_STDLIB_H__
#define  __OSAL_STDLIB_H__

#if defined(___LINUX___)
#include "linux/osal_stdlib.h"
#elif defined(___RTEMS___)
#include "rtems/osal_stdlib.h"
#elif defined(___WINDOWS___)
#include "windows/osal_stdlib.h"
#elif defined(___SYMBIAN___)
#include "symbian/osal_stdlib.h"
#elif defined(___ANDROID___)
#include "android/osal_stdlib.h"
#elif defined(___DSPBIOS___)
#include "dspbios/osal_stdlib.h"
#else
#endif

#define malloc    "please use osal_malloc"
#define free      "please use osal_free"
#define calloc    "please use osal_calloc"
#define memalign  "please use osal_memalign"

void* osal_malloc(size_t size);
void* osal_calloc(size_t nelem, size_t elsize);
void * osal_memalign (size_t boundary, size_t size);
void osal_free(void *p);
void osal_mem_lock(void *p);

int osal_init(void);
int osal_exit(void);

#endif // __OSAL_STDLIB_H__

