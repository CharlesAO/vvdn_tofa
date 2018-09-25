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

#ifndef _RPC_GUZZI_CAMERA3_H
#define _RPC_GUZZI_CAMERA3_H

#ifdef __cplusplus
extern "C" {
#endif

#include <guzzi/camera3/metadata.h>
#include <guzzi/camera3/camera3.h>
#include <guzzi/rpc/rpc.h>

#define RPC_GUZZI_CAMERA3_STREAMS_MAX GUZZI_CAMERA3_STREAMS_MAX
#define RPC_GUZZI_CAMERA3_INSTANCES_MAX 32
#define RPC_GUZZI_CAMERA3_INSTANCE_INVALID 0xBAD01234

#define RPC_GUZZI_CAMERA3_FUNCTION_ID(N) \
    (rpc_guzzi_camera3_function_id_base + (N))

#define RPC_GUZZI_CAMERA3_GET_NUMBER_OF_CAMERAS RPC_GUZZI_CAMERA3_FUNCTION_ID(1)
#define RPC_GUZZI_CAMERA3_GET_CAMERA_INFO       RPC_GUZZI_CAMERA3_FUNCTION_ID(2)
#define RPC_GUZZI_CAMERA3_REQUEST_DEFAULTS      RPC_GUZZI_CAMERA3_FUNCTION_ID(3)
#define RPC_GUZZI_CAMERA3_STREAM_CONFIGURATION  RPC_GUZZI_CAMERA3_FUNCTION_ID(4)
#define RPC_GUZZI_CAMERA3_CAPTURE_REQUEST       RPC_GUZZI_CAMERA3_FUNCTION_ID(5)
#define RPC_GUZZI_CAMERA3_FLUSH                 RPC_GUZZI_CAMERA3_FUNCTION_ID(6)
#define RPC_GUZZI_CAMERA3_DESTROY               RPC_GUZZI_CAMERA3_FUNCTION_ID(7)
#define RPC_GUZZI_CAMERA3_CREATE                RPC_GUZZI_CAMERA3_FUNCTION_ID(8)
#define RPC_GUZZI_CAMERA3_CALLBACK_NOTIFY       RPC_GUZZI_CAMERA3_FUNCTION_ID(9)

extern const rpc_uint32_t rpc_guzzi_camera3_function_id_base;

/* Get number of cameras */
typedef struct {
} rpc_guzzi_camera3_get_number_of_cameras_t;

typedef struct {
    rpc_int32_t number_of_cameras;
} rpc_guzzi_camera3_get_number_of_cameras_return_t;

/* Get camera info */
typedef struct {
    rpc_int32_t camera_id;
} rpc_guzzi_camera3_get_info_t;

typedef struct {
    rpc_int32_t camera_id;
    rpc_int32_t result;
    rpc_int32_t facing;
    rpc_int32_t orientation;
    guzzi_camera3_metadata_static_t metadata_static[0];
} rpc_guzzi_camera3_get_info_return_t;

/* Callback notify */
typedef struct {
    rpc_uint32_t instance;
    rpc_uint32_t event;
    union {
        struct {
            rpc_uint32_t frame_number;
            rpc_uint64_t timestamp;
        } shutter;
    };
} rpc_guzzi_camera3_callback_notify_t;

typedef struct {
    rpc_uint32_t instance;
} rpc_guzzi_camera3_callback_notify_return_t;

/* Request default controls */
typedef struct {
    rpc_uint32_t instance;
    rpc_uint32_t type;
} rpc_guzzi_camera3_request_defaults_t;

typedef struct {
    rpc_uint32_t instance;
    rpc_int32_t result;
    guzzi_camera3_metadata_controls_t metadata[0];
} rpc_guzzi_camera3_request_defaults_return_t;

/* Stream configuration */
typedef struct {
    rpc_uint32_t id;
    rpc_uint32_t type;
    rpc_uint32_t format;
    rpc_uint32_t width;
    rpc_uint32_t height;
} rpc_guzzi_camera3_stream_t;

typedef struct {
    rpc_uint32_t instance;
    rpc_uint32_t num_streams;
    rpc_guzzi_camera3_stream_t streams[RPC_GUZZI_CAMERA3_STREAMS_MAX];
} rpc_guzzi_camera3_stream_configuration_t;

typedef struct {
    rpc_uint32_t instance;
    rpc_int32_t result;
} rpc_guzzi_camera3_stream_configuration_return_t;

/* Capture request */
typedef struct {
    rpc_uint32_t instance;
    rpc_uint32_t frame_number;
    rpc_uint32_t streams_id[RPC_GUZZI_CAMERA3_STREAMS_MAX];
    guzzi_camera3_metadata_controls_t settings[0];
} rpc_guzzi_camera3_capture_request_t;

typedef struct {
    rpc_uint32_t instance;
    rpc_int32_t result;
} rpc_guzzi_camera3_capture_request_return_t;

/* Flush */
typedef struct {
    rpc_uint32_t instance;
} rpc_guzzi_camera3_flush_t;

typedef struct {
    rpc_uint32_t instance;
} rpc_guzzi_camera3_flush_return_t;

/* Destroy */
typedef struct {
    rpc_uint32_t instance;
} rpc_guzzi_camera3_destroy_t;

typedef struct {
    rpc_uint32_t instance;
} rpc_guzzi_camera3_destroy_return_t;

/* Create */
typedef struct {
    rpc_int32_t camera_id;
} rpc_guzzi_camera3_create_t;

typedef struct {
    rpc_uint32_t instance;
} rpc_guzzi_camera3_create_return_t;

#ifdef __cplusplus
}
#endif

#endif /* _RPC_GUZZI_CAMERA3_H */

