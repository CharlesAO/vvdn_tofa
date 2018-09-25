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
* @file plat_socket.h
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

#ifndef __PLAT_SOCKET_H__
#define __PLAT_SOCKET_H__

#include <hal/hat_types.h>
#include "plat_socket_pub.h"
#include <hal/hal_camera_module/hat_cm_hw_resource.h>

typedef enum {
    PLAT_SOCKET_STATE_UNKNOWN,
    PLAT_SOCKET_STATE_CREATED,
    PLAT_SOCKET_STATE_OPENED,
    PLAT_SOCKET_STATE_CLOSED
} PLAT_PH_SOCKET_STATE_T;

typedef void* plat_res_init_t;

typedef enum {
    PLAT_RES_CLOSE,  // resource is not initialized
    PLAT_RES_OPEN,   // resource is initialized and open
    PLAT_RES_IN_USE, // resource is in use (active)
} PLAT_HW_RESOURCE_STATUS_T;

typedef enum {
    PLAT_HW_RES_OFF, // de-activate
    PLAT_HW_RES_ON   // activate
} plat_hw_resource_action_t;

typedef struct {
    PLAT_SOCKET_HW_RSRC_TYPE_T      type;
    PLAT_HW_RESOURCE_STATUS_T       status;
    plat_res_init_t                 init;   // resource init sequence
} plat_resource_t;

typedef struct {
    plat_resource_t *res_desc;
} plat_res_desc_t;

typedef plat_res_desc_t (*plat_res_dummy_desc_t)[DUMM_RSRC_MAX];
typedef plat_res_desc_t (*plat_res_sensor_desc_t)[SEN_RSRC_MAX];
typedef plat_res_desc_t (*plat_res_lens_desc_t)[LENS_RSRC_MAX];
typedef plat_res_desc_t (*plat_res_flash_desc_t)[FLASH_RSRC_MAX];
typedef plat_res_desc_t (*plat_res_nvm_desc_t)[NVM_RSRC_MAX];

typedef struct {
    plat_res_dummy_desc_t  dummy_res;
    plat_res_sensor_desc_t sensor_res;
    plat_res_lens_desc_t   lens_res;
    plat_res_flash_desc_t  flash_res;
    plat_res_nvm_desc_t    nvm_res;
} plat_socket_res_desc_t;

typedef struct plat_socket_obj plat_socket_obj_t;

typedef plat_socket_obj_t* plat_socket_handle_t;

struct plat_socket_obj {
    PLAT_PH_SOCKET_STATE_T state;
    plat_socket_res_desc_t* soc_res;
    int (*detect)(plat_socket_handle_t hndl);
    int (*open)(plat_socket_obj_t *socket, hat_cm_socket_open_params_t *params);
    int (*close)(plat_socket_handle_t hndl);
    int (*deinit)(plat_socket_handle_t hndl);
};

/* ========================================================================== */
/**
* plat_socket_init()
*/
/* ========================================================================== */
int plat_socket_init(plat_socket_obj_t *socket);

/* ========================================================================== */
/**
* plat_socket_detect()
*/
/* ========================================================================== */
int plat_socket_detect(plat_socket_handle_t hndl);

/* ========================================================================== */
/**
* plat_socket_open()
*/
/* ========================================================================== */
int plat_socket_open(plat_socket_obj_t *socket,
    hat_cm_socket_open_params_t *params);

/* ========================================================================== */
/**
* plat_socket_close()
*/
/* ========================================================================== */
int plat_socket_close(plat_socket_handle_t hndl);

/* ========================================================================== */
/**
* plat_socket_deinit()
*/
/* ========================================================================== */
int plat_socket_deinit(plat_socket_handle_t hndl);

#endif // __PLAT_SOCKET_H__
