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
* @file osal_mutex.h
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

#ifndef  __OSAL_MUTEX_H__
#define  __OSAL_MUTEX_H__

#include <osal/osal_stdtypes.h>

/* ==========================================================================
 *                                OSAL MUTEX
 * ========================================================================= */

#define OSAL_MUTEX_TIMEDOUT     1
#define OSAL_MUTEX_MAXWAIT      (~(uint32)0)

struct osal_mutex;
typedef struct osal_mutex osal_mutex;

/** API used to create an object of osal_mutex type */
osal_mutex *osal_mutex_create(void);

/** API used to destroy an object of osal_mutex type */
void osal_mutex_destroy(osal_mutex *mutex);

/** API used to lock an object of osal_mutex type
 *  @return 0 on success,
 *         -1 on error
 */
int osal_mutex_lock(osal_mutex *mutex);

/** API used to lock an object of osal_mutex type
 *  with a timeout in milliseconds.
 *  @return 0 on success,
 *         -1 on error
 */
int osal_mutex_lock_timeout(osal_mutex *mutex, uint32 ms);

/** API used to try lock an object of osal_mutex type
 *  This call is non-blocking.
 *  @return 0 on success,
 *         -1 on error
 */
int osal_mutex_trylock(osal_mutex *mutex);

/** API used to unlock an object of osal_mutex type
 *  @return 0 on success,
 *         -1 on error
 */
int osal_mutex_unlock(osal_mutex *mutex);

/* ==========================================================================
 *                                OSAL SEMAPHORES
 * ========================================================================= */
struct osal_sem;
typedef struct osal_sem osal_sem;

/** API used to create an object of osal_sem type,
 *  initialized with value.
 *  @return NULL on failure
 */
osal_sem *osal_sem_create(uint32 init_value);

/** API used to destroy an object of osal_sem type */
void osal_sem_destroy(osal_sem *sem);

/** API used to initialize an object of osal_sem type */
int osal_sem_init(osal_sem *sem, uint32 value);

/**
 * API used to suspend the calling thread until the object
 * of osal_sem type pointed to by sem has a positive count.
 * It then atomically decreases the the object of osal_sem type
 * count.
 */
int osal_sem_wait(osal_sem *sem);

/** API used as non-blocking variant of osal_sem_wait().
 *  @return 0 on success,
 *         -1 on error
 */
int osal_sem_try_wait(osal_sem *sem);

/** API used as variant of osal_sem_wait() with a timeout in milliseconds
 *  @return 0 on success,
 *         -1 on error
 */
int osal_sem_wait_timeout(osal_sem *sem, uint32 ms);

/** API used to atomically increase the count
 *  of object of osal_sem type (not blocking).
 *  @return 0 on success,
 *         -1 on error
 */
int osal_sem_post(osal_sem *sem);

/** API used to return the current count of the object of osal_sem type */
uint32 osal_sem_value(osal_sem *sem);

/* ==========================================================================
 *                                OSAL Condition
 * ========================================================================= */
struct osal_cond;
typedef struct osal_cond osal_cond;

/** API used to create an object of osal_cond type */
osal_cond *osal_cond_create(void);

/** API used to destroy an object of osal_cond type */
void osal_cond_destroy(osal_cond *cond);

/** API used to unblock one of the threads
 *  that are waiting on the object of osal_cond type
 *  @return 0 on success,
 *         -1 on error
 */
int osal_cond_signal(osal_cond *cond);

/** API used to unblock all threads
 *  that are waiting on the object of osal_cond type
 *  @return 0 on success,
 *         -1 on error
 */
int osal_cond_broadcast(osal_cond *cond);

/** API used to wait on the object of osal_cond type and
 *  unlocks the provided object of osal_mutex type.
 *  @return 0 on success,
 *         -1 on error
 */
int osal_cond_wait(osal_cond *cond, osal_mutex *mutx);

/** API used to wait for at most 'ms' milliseconds
 *  @return 0 on success,
 *         -1 on error
 */
int osal_cond_wait_timeout(osal_cond *cond, osal_mutex *mutex, uint32 ms);

#endif /* __OSAL_MUTEX_H__ */
