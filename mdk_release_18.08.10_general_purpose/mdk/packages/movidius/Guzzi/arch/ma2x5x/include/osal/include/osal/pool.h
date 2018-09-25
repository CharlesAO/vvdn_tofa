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
* @file pool.h
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

#ifndef _POOL_H
#define _POOL_H

#include <osal/osal_stdtypes.h>
#include <utils/mms_debug.h>
 
#define POOL_ALLOC_WHILE_TIMEOUT(pool) (                                        \
        {                                                                       \
            void *_p;                                                           \
            do {                                                                \
                _p = pool_alloc_timeout(pool, DEFAULT_ALLOC_TIMEOUT_MS);        \
                if (!_p)                                                        \
                    mmsdbg(DL_ERROR, "Pool alloc timeout line %d", __LINE__);   \
            } while (!_p);                                                      \
            _p;                                                                 \
        }                                                                       \
    )  

typedef struct pool pool_t;

void * pool_alloc(pool_t *pool);
void * pool_try_alloc(pool_t *pool);
void * pool_alloc_timeout(pool_t *pool, uint32 ms);
void pool_destroy(pool_t *pool);
pool_t * pool_create(
        char *pool_name,
        unsigned int elem_size,
        unsigned int elems_count
    );

#endif /* _POOL_H */
