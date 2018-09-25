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

#ifndef  __OSAL_ATOM_LINUX_H__
#define  __OSAL_ATOM_LINUX_H__

#include <pthread.h>

struct osal_atomic {
   pthread_mutex_t mutex;
   uint32 value;
};

#define OSAL_ATOM_INIT                    { PTHREAD_MUTEX_INITIALIZER, 0 }
#define OSAL_ATOM_INIT_VALUE(val)         { PTHREAD_MUTEX_INITIALIZER, val }
#define OSAL_ATOM_CREATE(atom)            atomic atom = atom_INIT

#define osal_atom_init(atomic)            { pthread_mutex_init(&(atomic)->mutex, 0); (atomic)->value = 0; }
#define osal_atom_done(atomic)            pthread_mutex_destroy(&(atomic)->mutex);
#define osal_atom_lock(atomic)            pthread_mutex_lock(&(atomic)->mutex);
#define osal_atom_unlock(atomic)          pthread_mutex_unlock(&(atomic)->mutex);
#define osal_atom_get(atomic)             ((atomic)->value)

#define osal_atom_isSet(atomic,val)       ((((atomic)->value) & (val)) == (val))
#define osal_atom_isClr(atomic,val)       ((((atomic)->value) & (val)) == (0))
#define osal_atom_isAny(atomic,val)       ((((atomic)->value) & (val)) != (0))

#define osal_atom_set_(atomic,val)        (atomic)->value = (uint32)(val)
#define osal_atom_or_(atomic,val)         (atomic)->value |= (uint32)(val)
#define osal_atom_nor_(atomic,val)        (atomic)->value |= ~((uint32)(val))
#define osal_atom_and_(atomic,val)        (atomic)->value &= (uint32)(val)
#define osal_atom_xor_(atomic,val)        (atomic)->value ^= (uint32)(val)
#define osal_atom_nand_(atomic,val)       (atomic)->value &= ~((uint32)(val))
#define osal_atom_inc_(atomic,val)        { ((atomic)->value) += (val); }
#define osal_atom_dec_(atomic,val)        { ((atomic)->value) -= (val); }

#define osal_atom_set(atomic,val) \
    { osal_atom_lock(atomic); osal_atom_set_(atomic,val); osal_atom_unlock(atomic); }

#define osal_atom_or(atomic,val) \
    { osal_atom_lock(atomic); osal_atom_or_(atomic,val); osal_atom_unlock(atomic); }

#define osal_atom_nor(atomic,val) \
    { osal_atom_lock(atomic); osal_atom_nor_(atomic,val); osal_atom_unlock(atomic); }

#define osal_atom_and(atomic,val) \
    { osal_atom_lock(atomic); osal_atom_and_(atomic,val); osal_atom_unlock(atomic); }

#define osal_atom_xor(atomic,val) \
    { osal_atom_lock(atomic); osal_atom_xor_(atomic,val); osal_atom_unlock(atomic); }

#define osal_atom_nand(atomic,val) \
    { osal_atom_lock(atomic); osal_atom_nand_(atomic,val); osal_atom_unlock(atomic); }

#define osal_atom_inc(atomic,val) \
    { osal_atom_lock(atomic); osal_atom_inc_(atomic,val); osal_atom_unlock(atomic); }

#define osal_atom_dec(atomic,val) \
    { osal_atom_lock(atomic); osal_atom_dec_(atomic,val); osal_atom_unlock(atomic); }

#endif // __OSAL_ATOM_LINUX_H__

