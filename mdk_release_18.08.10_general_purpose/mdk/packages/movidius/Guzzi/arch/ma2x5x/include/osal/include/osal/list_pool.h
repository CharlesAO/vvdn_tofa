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
* @file list_pool.h
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

#ifndef __LIST_POOL_H__
#define __LIST_POOL_H__
#include <osal/osal_stdtypes.h>
#include <osal/osal_list.h>

/* Example
 *
 *
 * #include <pool/include/list_pool.h>
 * #include <error_handle/include/error_handle.h>
 * typedef struct
 * {
 *     char text[30];
 *     struct list_head link;
 *     uint32 cnt;
 * }list_pool_element_t;
 *
 * list_pool_t *tlp;
 * void test_list_pool(void)
 * {
 *     list_pool_head_t *h1, *h2, *h3, *h4;
 *     list_pool_element_t *el;
 *     int cnt;
 *
 *     tlp = list_pool_create("Test list pool", 3, sizeof(list_pool_element_t), offsetof(list_pool_element_t, link), 5);
 *
 *     h1 = list_pool_alloc_head(tlp);
 *     GOTO_EXIT_IF(h1 == NULL, 1);
 *     h2 = list_pool_alloc_head(tlp);
 *     GOTO_EXIT_IF(h2 == NULL, 1);
 *     h3 = list_pool_alloc_head(tlp);
 *     GOTO_EXIT_IF(h3 == NULL, 1);
 *
 *     h4 = list_pool_alloc_head_timeout(tlp, 100);
 *     GOTO_EXIT_IF(h4 != NULL, 1);
 *
 *     for (cnt = 0; cnt < 4; ++cnt) {
 *         el = list_pool_append_element(tlp, h1);
 *         sprintf(el->text, "Elem %d", cnt);
 *         el->cnt = cnt;
 *     }
 *
 *     list_for_each_entry(el,&h1->list,link)
 *     {
 *         printf("\nTest list pool: element text: %s\n", el->text);
 *     }
 *
 *     el = list_pool_append_element(tlp, h2);
 *     el = list_pool_append_element(tlp, h2);
 *
 *     osal_mem_lock(h1);
 *
 *     osal_free(h1);
 *     osal_free(h1);
 *
 *     osal_free(h2);
 *     osal_free(h3);
 *     list_pool_destroy(tlp);
 *
 *     return;
 *
 * EXIT_1:
 *     printf("\nERROR: test_list_pool fail\n");
 *
 * }
 *
 */

#define LIST_POOL_ALLOC_WHILE_TIMEOUT(pool) (                                        \
        {                                                                       \
            void *_p;                                                           \
            do {                                                                \
                _p = list_pool_alloc_head_timeout(pool, DEFAULT_ALLOC_TIMEOUT_MS);        \
                if (!_p)                                                        \
                    mmsdbg(DL_ERROR, "List pool alloc head timeout line %d", __LINE__);   \
            } while (!_p);                                                      \
            _p;                                                                 \
        }                                                                       \
    )

typedef struct  {
    uint32  magic;
    struct osal_node list;
}list_pool_head_t;

typedef struct pool list_pool_t;

list_pool_head_t * list_pool_alloc_head(list_pool_t *pool);

list_pool_head_t * list_pool_try_alloc_head(list_pool_t *pool);

list_pool_head_t * list_pool_alloc_head_timeout(list_pool_t *pool, uint32 ms);

void * list_pool_append_element(list_pool_t *pool, list_pool_head_t * head);

void list_pool_destroy(list_pool_t *pool);

list_pool_t * list_pool_create(  char *pool_name,
                                   unsigned int list_count,
                                   unsigned int elem_size,
                                   unsigned int elem_head_offset,
                                   unsigned int elems_count
                               );

#endif // __LIST_POOL_H__
