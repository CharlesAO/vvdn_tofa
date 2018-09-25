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

#ifndef _RPC_H
#define _RPC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <osal/osal_stdio.h>
#include <osal/osal_list.h>
#include <osal/osal_stdtypes.h>

#include <guzzi/rpc/transport.h>

#define RPC_FUNCTION_RETURN (1 << 31)

#define RPC_RET(FUNCTION) (RPC_FUNCTION_RETURN | (FUNCTION))

#define RPC_TRANSPORT_DEFAULT 0
#define RPC_SERVICE_DEFAULT 0

typedef int8 rpc_int8_t;
typedef uint8 rpc_uint8_t;
typedef int16 rpc_int16_t;
typedef uint16 rpc_uint16_t;
typedef int32 rpc_int32_t;
typedef uint32 rpc_uint32_t;
typedef int64 rpc_int64_t;
typedef uint64 rpc_uint64_t;

typedef struct rpc rpc_t;

typedef struct rpc_rx_thread rpc_rx_thread_t;

/* Packet definition */
typedef struct {
    rpc_int32_t service;
    rpc_uint32_t function;
    rpc_uint32_t call_id;
    rpc_int32_t status;
    rpc_uint32_t data_size;
    rpc_uint32_t sum;
} rpc_packet_header_t;

typedef struct {
    void *p;
    rpc_uint32_t size;
} rpc_packet_payload_t;

typedef struct {
    struct osal_node link;
    rpc_packet_header_t header;
    rpc_packet_payload_t payload[8];
} rpc_packet_t;

/* Function table */
typedef int rpc_function_t(rpc_t *rpc, rpc_packet_t *call);

#define RPC_SKEL_FUNCTIONS_TABLE_ENTRY_NIL \
    {0, NULL}
#define RPC_SKEL_FUNCTIONS_TABLE_ENTRY_IS_NIL(ENTRY) \
    (((ENTRY)->id == 0) && ((ENTRY)->func == NULL))

typedef struct {
    rpc_uint32_t id;
    rpc_function_t *func;
} rpc_skel_functions_table_entry_t;

/* Service */
typedef rpc_packet_t *rpc_service_packet_alloc_t(
        rpc_packet_header_t *packet_header,
        void *prv
    );
typedef void rpc_service_packet_free_t(
        rpc_packet_t *packet,
        void *prv
    );

rpc_packet_t *rpc_service_alloc_packet(
        rpc_t *rpc,
        int serv_n,
        rpc_packet_header_t *packet_header
    );
void rpc_service_free_packet(
        rpc_t *rpc,
        int serv_n,
        rpc_packet_t *packet
    );

void rpc_service_destroy(rpc_t *rpc, int serv_n);
int rpc_service_create(
        rpc_t *rpc,
        int serv_n,
        void *prv,
        rpc_service_packet_alloc_t *packet_alloc,
        rpc_service_packet_free_t *packet_free,
        int threads,
        int thread_stack_size
    );

/* Rx Thread */
rpc_rx_thread_t * rpc_rx_thread_create(
        rpc_t *rpc,
        int transport_n,
        unsigned int stack_size
    );
void rpc_rx_thread_destroy(rpc_rx_thread_t *rpc_rx_thread);

int rpc_transport_map(rpc_t *rpc, int name, transport_t *transport);
int rpc_transport_unmap(rpc_t *rpc, int name);

/* RPC */
#define RPC_CALL(RPC, FUNC, PACKET) \
    rpc_call(RPC, RPC_TRANSPORT_DEFAULT, RPC_SERVICE_DEFAULT, FUNC, PACKET)

#define RPC_RETURN(RPC, CALL, RESULT) \
    rpc_return(RPC, RPC_TRANSPORT_DEFAULT, RPC_SERVICE_DEFAULT, CALL, RESULT)

rpc_packet_t * rpc_call(
        rpc_t *rpc,
        rpc_int32_t transport_n,
        rpc_int32_t serv_n,
        rpc_uint32_t function,
        rpc_packet_t *packet
    );

int rpc_return(
        rpc_t *rpc,
        rpc_int32_t transport_n,
        rpc_int32_t serv_n,
        rpc_packet_t *call,
        rpc_packet_t *result
    );

void rpc_destroy(rpc_t *rpc);

rpc_t * rpc_create(
        unsigned int transport_ns_size,
        unsigned int service_ns_size,
        int functions_to_track,
        rpc_skel_functions_table_entry_t *skel_functions_table
    );

#ifdef __cplusplus
}
#endif

#endif /* _RPC_H */

