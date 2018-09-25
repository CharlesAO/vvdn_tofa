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
* @file
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

#ifndef _RPC_TRANSPORT_H
#define _RPC_TRANSPORT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct transport transport_t;

struct transport {
    int (*read)(transport_t *transport, void *p, unsigned int size);
    int (*write)(transport_t *transport, void *p, unsigned int size);
    void (*write_lock)(transport_t *transport);
    void (*write_unlock)(transport_t *transport);
    void (*exit)(transport_t *transport);
};

static inline int transport_read(transport_t *transport, void *p, unsigned int size)
{
    return transport->read(transport, p, size);
}

static inline int transport_write(transport_t *transport, void *p, unsigned int size)
{
    return transport->write(transport, p, size);
}

static inline void transport_write_lock(transport_t *transport)
{
    transport->write_lock(transport);
}

static inline void transport_write_unlock(transport_t *transport)
{
    transport->write_unlock(transport);
}

static inline void transport_exit(transport_t *transport)
{
    transport->exit(transport);
}

#ifdef __cplusplus
}
#endif

#endif /* _RPC_TRANSPORT_H */

