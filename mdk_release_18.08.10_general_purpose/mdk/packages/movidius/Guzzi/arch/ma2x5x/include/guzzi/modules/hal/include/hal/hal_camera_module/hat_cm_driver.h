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
* @file hat_cm_driver.h
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

#ifndef __HAT_CM_DRIVER_H__
#define __HAT_CM_DRIVER_H__

#include <hal/hat_types.h>

#include <hal/hal_camera_module/hat_cm_sensor.h>
#include <hal/hal_camera_module/hat_cm_lens.h>
#include <hal/hal_camera_module/hat_cm_lights.h>

typedef enum {
    CM_DRV_STATE_UNKNOWN,
    CM_DRV_STATE_CREATED,
    CM_DRV_STATE_OPENED,
    CM_DRV_STATE_CLOSED,
} HAT_CM_DRIVER_STATE_T;



typedef enum {
    CM_DRV_COMP_DUMMY,
    CM_DRV_COMP_SENSOR,
    CM_DRV_COMP_LENS,
    CM_DRV_COMP_LIGHT,
    CM_DRV_COMP_NVM,
    CM_DRV_COMP_CAMERA,
} HAT_CM_DRIVER_COMPONENT_T;



typedef struct {
    int id;
    char *name;
} hat_cm_driver_cr_prms_t;

typedef struct {
    int cm_idx;
    int sensor_idx;
    int lens_idx;
    int lights_idx;
    int nvm_idx;
} hat_cm_component_ids_t;

typedef enum {
    CAMERA_QUERY_DUMMY,
    CAMERA_QUERY_GET_MOD_IDS,
    CAMERA_QUERY_GET_IMGIF_CTX,
} HAT_CM_CAMERA_QUERY_OPER_T;

typedef struct {
    HAT_CM_CAMERA_QUERY_OPER_T query_type;
    union {
        hat_cm_component_ids_t* component_ids;
        int imgif_ctx;
    } query;
} hat_cm_camera_query_t;

typedef struct hat_cm_driver_obj hat_cm_driver_obj_t;

typedef hat_cm_driver_obj_t* hat_cm_driver_handle_t;

struct hat_cm_driver_obj {
    void *prvt;
    int (*get_state)(hat_cm_driver_handle_t hndl, HAT_CM_DRIVER_STATE_T *state);
    int (*load_dtp)(hat_cm_driver_handle_t hndl);
    int (*unload_dtp)(hat_cm_driver_handle_t hndl);
    int (*detect)(hat_cm_driver_handle_t hndl);

    int (*open)(hat_cm_driver_handle_t hndl);
    int (*hal_cm_config)(hat_cm_driver_handle_t hndl,
                         HAT_CM_DRIVER_COMPONENT_T comp,
                         void* cfg_prms);
    int (*hal_cm_control)(hat_cm_driver_handle_t hndl,
        HAT_CM_DRIVER_COMPONENT_T comp, void* ctrl_prms);
    int (*process)(hat_cm_driver_handle_t hndl,
        HAT_CM_DRIVER_COMPONENT_T comp, void* prms);
    int (*query)(hat_cm_driver_handle_t hndl,
        HAT_CM_DRIVER_COMPONENT_T comp, void* query_prms);
    int (*close)(hat_cm_driver_handle_t hndl);
    int (*destroy)(hat_cm_driver_handle_t hndl);
};


typedef struct
{
    uint32_t maxWidth;
    uint32_t maxHeight;
    uint32_t maxBpp;
    uint32_t maxPixels;
} hat_camera_limits_t;

int hai_cm_driver_get_camera_limits(int camera_id,
                                    hat_camera_limits_t *limits);

#endif // __HAT_CM_DRIVER_H__
