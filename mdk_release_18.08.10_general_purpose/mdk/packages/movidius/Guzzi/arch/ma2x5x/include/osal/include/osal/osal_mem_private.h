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
* @file osal_mem_private.h
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

#ifndef _OSAL_MAM_PRIVATE_H
#define _OSAL_MAM_PRIVATE_H

#include <osal/osal_stdtypes.h>

#ifdef OSAL_MEM_PRIVATE

#undef malloc
#undef free

#define OSAL_MEM_HEADER_SIZE_ALIGNMENT 16
#define OSAL_MEM_HEADER_SIZE \
    ALIGN(sizeof (osal_mem_header_t), OSAL_MEM_HEADER_SIZE_ALIGNMENT)

#define OSAL_MEM_HEADER_MAGIC_ALLOCATED 0x55AAAA55
#define OSAL_MEM_HEADER_MAGIC_FREE 0x66DDDD66

typedef struct osal_mem_header osal_mem_header_t;

typedef void osal_mem_free_t(void *vph, osal_mem_header_t *oh);
typedef void osal_mem_lock_t(void *vph, osal_mem_header_t *oh);

struct osal_mem_header {
    uint32 magic;
    uint32 block_size;    // size of allocated memory from system memory (original request + _osal_malloc_hdr_t)
    uint8* block_address;
    osal_mem_free_t *free;
    osal_mem_lock_t *lock;
};

static inline osal_mem_header_t * osal_mem_client_to_oh(void *p)
{
    return (osal_mem_header_t *)((char *)p - OSAL_MEM_HEADER_SIZE);
}

static inline void * osal_mem_oh_to_client(osal_mem_header_t *oh)
{
    return (char *)oh + OSAL_MEM_HEADER_SIZE;
}

static inline void osal_mem_mark_alloced(osal_mem_header_t *oh)
{
    oh->magic = OSAL_MEM_HEADER_MAGIC_ALLOCATED;
}

static inline void osal_mem_mark_free(osal_mem_header_t *oh)
{
    oh->magic = OSAL_MEM_HEADER_MAGIC_FREE;
}

static inline void osal_mem_header_populate(
        osal_mem_header_t *oh,
        uint32 size,
        void *block_address,
        osal_mem_free_t *free,
        osal_mem_lock_t *lock
    )
{
    oh->block_size = size;
    oh->block_address = block_address;
    oh->free = free;
    oh->lock = lock;

    osal_mem_mark_alloced(oh);
}

#else
#error DO NOT INCLUDE THIS FILE UNLESS YOU KNOW WHAT YOU ARE DOING
#endif /* OSAL_MEM_PRIVATE */

#endif /* _OSAL_MAM_PRIVATE_H */
