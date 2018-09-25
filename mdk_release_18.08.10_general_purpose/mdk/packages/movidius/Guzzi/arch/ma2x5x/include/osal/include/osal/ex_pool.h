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
* @file ex_pool.h
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

#ifndef __EX_POOL_H__
#define __EX_POOL_H__
#include <osal/osal_stdtypes.h>

//struct __ex_pool_node;

struct __ex_pool_node {
    struct __ex_pool_node* next;
    int   size;
    int   offset;
};

typedef struct __ex_pool_node ex_pool_node_t;

#define EX_POOL_ROOT(ptr, sz) .next=((struct __ex_pool_node*)(ptr)), \
                               .size=sz, \
                               .offset=-1

#define EX_POOL_NODE_DESC(nxt, sz, type, ptr) \
    {                                         \
        .next=nxt, .size=sz,                  \
        .offset=offsetof(type, ptr)           \
    }

#define EX_POOL_LIST_END {NULL, 0, -1 }
/*
 * Example buffers description
 *--------------------------------------------------------
 * typedef int buff_t[20];
 *
 * typedef struct {
 *     int  i1;
 *     char e2;
 *     buff_t *pb;
 *     char *cp;
 * }s1;
 *
 * typedef struct {
 *     int  i1;
 *     char e2;
 *     s1 * const p1;
 *     int  i2;
 *     s1 * const p2;
 *     unsigned u1;
 * }s2;
 *
 * ex_pool_node_t s1_pb_desc [] = {
 *      EX_POOL_NODE_DESC(NULL, sizeof (buff_t), s1, pb),
 *      EX_POOL_NODE_DESC(NULL, 10, s1, cp),
 *      EX_POOL_LIST_END
 * };
 *
 * ex_pool_node_t s2_desc [] = {
 *      EX_POOL_NODE_DESC(&s1_pb_desc, sizeof (s1), s2, p1),
 *      EX_POOL_NODE_DESC(&s1_pb_desc, sizeof (s1), s2, p2),
 *      EX_POOL_LIST_END
 * };
 *
 * ex_pool_node_t root =
 * {
 *      EX_POOL_ROOT(&s2_desc, sizeof (s2))
 * };
 */

typedef struct pool ex_pool_t;

void * ex_pool_alloc(ex_pool_t *pool);
void * ex_pool_try_alloc(ex_pool_t *pool);
void * ex_pool_alloc_timeout(ex_pool_t *pool, uint32 ms);
void ex_pool_destroy(ex_pool_t *pool);
ex_pool_t * ex_pool_create(char *pool_name,
                           ex_pool_node_t *const root_descriptor,
                           unsigned int elems_count);

#endif // __EX_POOL_H__
