/* =============================================================================
* Copyright (c) 2017-2018 MM Solutions AD
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
* @file osal_list.h
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

#ifndef __OSAL_LIST_H__
#define __OSAL_LIST_H__

#include <osal/osal_stdio.h>

#define prefetch(x) ((x)=(x))

/*
 * Double linked list implementation - simple.
 */

struct osal_node {
    struct osal_node *next, *previous;
};

typedef struct osal_node osal_node_t;

#define OSAL_LIST_HEAD_INIT(element) { &(element), &(element) }

#define OSAL_LIST_HEAD(element) \
        osal_node_t element = OSAL_LIST_HEAD_INIT(element)

/**
 * osal_list_init_head - API used to initial list's head
 * @node: first node to be initial
 */
static inline void osal_list_init_head(osal_node_t *node)
{
    node->next = node;
    node->previous = node;
}

/**
 * osal_list_insert - API used to add a new node after the head
 * @new: new node to be added
 * @head: list head to add - implementing stacks
 */
static inline void osal_list_insert(osal_node_t *new, osal_node_t *head)
{
    osal_node_t *__new, *__prev, *__next;
    __new = new;
    __prev = head;
    __next = head->next;

    __next->previous = __new;
    __new->next = __next;
    __new->previous = __prev;
    __prev->next = __new;
}

/**
 * osal_list_append - API used to add a new node before the head
 * @new: new node to be added
 * @head: list head to add - implementing queues
 */
static inline void osal_list_append(osal_node_t *new, osal_node_t *head)
{
    osal_node_t *__new, *__prev, *__next;
    __new = new;
    __prev = head->previous;
    __next = head;

    __next->previous = __new;
    __new->next = __next;
    __new->previous = __prev;
    __prev->next = __new;
}

/**
 * osal_list_delete - API used to delete node from list.
 * @entry: node to be removed from the list.
 */
static inline void osal_list_delete(osal_node_t *entry)
{
    osal_node_t *__prev, *__next;
    __prev = entry->previous;
    __next = entry->next;

    __next->previous = __prev;
    __prev->next = __next;
    entry->next = NULL;
    entry->previous = NULL;
}

/**
 * osal_list_substitute - API used to replace old node by new one
 * @old: node which should be replaced
 * @new: node to insert
 */
static inline void osal_list_substitute(osal_node_t *old,
                osal_node_t *new)
{
    new->next = old->next;
    new->next->previous = new;
    new->previous = old->previous;
    new->previous->next = new;
}

/**
 * osal_list_shift - API used to delete from one list and add as another's head
 * @list: node to move
 * @head: head that will precede node
 */
static inline void osal_list_shift(osal_node_t *list, osal_node_t *head)
{
    osal_node_t *__prev, *__next;
    __prev = list->previous;
    __next = list->next;

    __next->previous = __prev;
    __prev->next = __next;
    osal_list_insert(list, head);
}

/**
 * osal_list_move - API used to delete from one list and add as another's tail
 * @list: node to move
 * @head: head that will follow node
 */
static inline void osal_list_move(osal_node_t *node,
                  osal_node_t *head)
{
    osal_node_t *__prev, *__next;
    __prev = node->previous;
    __next = node->next;

    __next->previous = __prev;
    __prev->next = __next;
    osal_list_append(node, head);
}

/**
 * osal_list_is_empty - API used to tests whether a list is empty
 * @head: list to test.
 */
static inline int osal_list_is_empty(const osal_node_t *head)
{
    return head->next == head;
}

/**
 * osal_list_is_empty_safe - API used to tests whether a list is empty
 * and not being modified
 * @head: list to check
 */
static inline int osal_list_is_empty_safe(const osal_node_t *head)
{
    osal_node_t *next = head->next;
    return (next == head) && (next == head->previous);
}

/**
 * osal_list_in - get struct for this node
 * @osal_ptr: &struct osal_head pointer.
 * @type:     type of struct this is embedded in.
 * @element:  name of list_struct within struct.
 */
#define osal_list_in(osal_ptr, type, element) ({ \
    const typeof( ((type *)0)->element ) *__mosal_ptr = (osal_ptr);    \
    (type *)( (char *)__mosal_ptr - offsetof(type,element) );})

/**
 * osal_list_first_in - get first node from a list,
 * it is expected to be not empty.
 * @osal_ptr: osal head to take node from.
 * @type:     type of struct this is embedded in.
 * @element:  name of list_struct within struct.
 */
#define osal_list_first_in(osal_ptr, type, element) \
    osal_list_in((osal_ptr)->next, type, element)

/**
 * osal_list_iterate - iterate over a list
 * @cursor: &struct osal_head to use as a loop cursor.
 * @head:   head of the list.
 */
#define osal_list_iterate(cursor, head) \
    for (cursor = (head)->next; prefetch(cursor->next), cursor != (head); \
            cursor = cursor->next)

/**
 * osal_list_iterate_in - iterate over list of given type
 * @cursor:  type * to use as a loop cursor.
 * @head:    head of the list.
 * @element: name of osal_list_struct within the struct.
 */
#define osal_list_iterate_in(cursor, head, element)                \
    for (cursor = osal_list_in((head)->next, typeof(*cursor), element);    \
         prefetch(cursor->element.next), &cursor->element != (head);     \
         cursor = osal_list_in(cursor->element.next, typeof(*cursor), element))

/**
 * osal_list_iterate_in_opposite - iterate backwards over list of given type.
 * @cursor:  type * to use as a loop cursor.
 * @head:    head of the list.
 * @element: name of osal_list_struct within struct.
 */
#define osal_list_iterate_in_opposite(cursor, head, element)            \
    for (cursor = osal_list_in((head)->previous, typeof(*cursor), element);    \
         prefetch(cursor->element.previous), &cursor->element != (head);     \
         cursor = osal_list_in(cursor->element.previous, typeof(*cursor), element))

/**
 * osal_list_iterate_in_safe - iterate over list of given type safe
 * against removal of list entry
 * @cursor:  type * to use as a loop cursor.
 * @storage: another type * to use as temporary storage
 * @head:    head of the list.
 * @element: name of osal_list_struct within struct.
 */
#define osal_list_iterate_in_safe(cursor, storage, head, element)            \
    for (cursor = osal_list_in((head)->next, typeof(*cursor), element),    \
        storage = osal_list_in(cursor->element.next, typeof(*cursor), element);    \
         &cursor->element != (head);                     \
         cursor = storage, storage = osal_list_in(storage->element.next, \
                                                  typeof(*storage), element))

/**
 * osal_list_iterate_in_safe_opposite - iterate backwards over list
 * of given type, safe against removal of list node.
 * @cursor:  type * to use as a loop cursor.
 * @storage: another type * to use as temporary storage
 * @head:    head of the list.
 * @element: name of osal_list_struct within struct.
 */
#define osal_list_iterate_in_safe_opposite(cursor, storage, head, element)        \
    for (cursor = osal_list_in((head)->previous, typeof(*cursor), element),    \
        storage = osal_list_in(cursor->element.previous, typeof(*cursor), element);    \
         &cursor->element != (head);                     \
         cursor = storage, storage = osal_list_in(storage->element.previous, \
                                                  typeof(*storage), element))

#endif // __OSAL_LIST_H__
