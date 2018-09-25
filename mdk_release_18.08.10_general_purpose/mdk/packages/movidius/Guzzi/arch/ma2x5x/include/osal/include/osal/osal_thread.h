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
* @file osal_thread.h
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

#ifndef  __OSAL_THREAD_H__
#define  __OSAL_THREAD_H__

#include <osal/osal_stdtypes.h>
#include <osal/osal_sysdep.h>

#define OSAL_THR_FLAG_SCHED_RR       (1 << 0)       // Round-robin scheduler
#define OSAL_THR_FLAG_SCHED_FIFO     (1 << 1)       // FIFO scheduler

typedef void *osal_thread_routine(void *);
typedef int osal_thread_done(void *);

struct osal_thread;
typedef struct osal_thread osal_thread;

int osal_thread_init(void);
void osal_thread_exit(void);

/*
 * stack_size <= 0 - Default size
 */
struct osal_thread *osal_thread_create(const char *name, void *priv,
        osal_thread_routine *cb_routine, osal_thread_done *cb_done,
        int flags, int stack_size);

int osal_thread_destroy(struct osal_thread *ctx);

int osal_thread_priority(struct osal_thread *ctx, uint32 priority);

/* Return ID of current thread */
struct osal_thread * osal_thread_self(void);

#endif /* __OSAL_THREAD_H__ */
