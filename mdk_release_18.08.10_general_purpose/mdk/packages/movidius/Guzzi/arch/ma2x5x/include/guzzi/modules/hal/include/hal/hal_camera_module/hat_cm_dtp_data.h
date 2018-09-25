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
* @file hat_cm_dtp_data.h
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

#ifndef __HAT_CM_DTP_DATA__
#define __HAT_CM_DTP_DATA__

#include <hal/hal_camera_module/hat_cm_socket.h>
#include <hal/hal_camera_module/hat_cm_sensor.h>
#include <hal/hal_camera_module/hat_cm_lens.h>
#include <hal/hal_camera_module/hat_cm_lights.h>
#include <hal/hal_camera_module/hat_cm_nvm.h>
#include <dtp/dtp_server_defs.h>

typedef struct {
    dtp_cam_module_id_t cm_dtp_id;
    const char*        cm_name;
    dtp_sensors_id_t    sen_dtp_id;
    dtp_lenses_id_t     lens_dtp_id;
    dtp_lights_id_t     lights_dtp_id;
    dtp_nvm_id_t        nvm_dtp_id;
    uint32              socket_mask;
    struct {
        uint32  entry_count;
        const hat_cm_socket_command_entry_t* entries;
    } detect_seq;
    struct {
        hat_cm_socket_open_params_t cp;
    } cm_features;
} hat_dtp_cm_desc_t;

typedef struct {
    uint32                      entry_count;
    const hat_dtp_cm_desc_t*    cm_entries;
} hat_dtp_cm_db_t;



typedef struct {
    int                     sensor_id;
    const char*             sensor_name;
    hat_sensor_features_t   sensor_features;
} hat_dtp_sensor_desc_t;



typedef struct {
    int                 lens_id;
    const char*         lens_name;
    hat_lens_features_t lens_features;
} hat_dtp_lens_desc_t;



typedef struct {
    int                    lights_id;
    const char*            lights_name;
    hat_lights_features_t  lights_features;
} hat_dtp_lights_desc_t;



typedef struct {
    int                 nvm_id;
    const char*         nvm_name;
    hat_nvm_features_t  nvm_features;
} hat_dtp_nvm_desc_t;

#endif //__HAT_CM_DTP_DATA__
