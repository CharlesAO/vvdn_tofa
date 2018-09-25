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
* @file osal_atomic.h
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

#ifndef  __OSAL_ATOM_H__
#define  __OSAL_ATOM_H__

#include <osal/osal_stdtypes.h>

#if defined(___LINUX___)
#include "linux/osal_atomic.h"
#elif defined(___RTEMS___)
#include "rtems/osal_atomic.h"
#elif defined(___WINDOWS___)
#elif defined(___SYMBIAN___)
#elif defined(___ANDROID___)
#include "android/osal_atomic.h"
#elif defined(___DSPBIOS___)
#else
#include "dummy/osal_atomic.h"
#endif

typedef struct osal_atomic osal_atomic;

static inline int osal_bit_test_set(struct osal_atomic *atomic, int nbit)
{
    uint32 val, mask = (1 << nbit);

    osal_atom_lock(atomic);
    val = osal_atom_get(atomic) & mask;
    osal_atom_or_(atomic, mask);
    osal_atom_unlock(atomic);

    return !!val;
}

static inline int osal_bit_test_clr(struct osal_atomic *atomic, int nbit)
{
    uint32 val, mask = (1 << nbit);

    osal_atom_lock(atomic);
    val = osal_atom_get(atomic) & mask;
    osal_atom_and_(atomic, ~mask);
    osal_atom_unlock(atomic);

    return !!val;
}

static inline void osal_bit_set(struct osal_atomic *atomic, int nbit)
{
    osal_atom_or(atomic, 1 << nbit);
}

static inline void osal_bit_clr(struct osal_atomic *atomic, int nbit)
{
    osal_atom_nand(atomic, 1 << nbit);
}

static inline int osal_bit_test(struct osal_atomic *atomic, int nbit)
{
    return !!(osal_atom_get(atomic) & (1 << nbit));
}

#endif // __OSAL_ATOM_H__
