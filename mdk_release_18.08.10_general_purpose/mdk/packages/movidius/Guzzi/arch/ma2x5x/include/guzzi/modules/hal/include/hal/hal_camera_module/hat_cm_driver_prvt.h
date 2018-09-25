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
* @file hat_cm_driver_prvt.h
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

#ifndef __HAT_CM_DRIVER_PRVT_H__
#define __HAT_CM_DRIVER_PRVT_H__

#include <hal/hal_camera_module/hat_cm_driver.h>
#include <hal/hal_camera_module/hat_cm_dtp_data.h>
#include <dtp/dtp_server_defs.h>
#include <hal/modules/hal_camera_module/sensors/sensors.h>
#include <hal/modules/hal_camera_module/lenses/lenses.h>
#include <hal/modules/hal_camera_module/lights/lights.h>
#include <hal/modules/hal_camera_module/nvms/nvms.h>

typedef struct {
    struct {
        dtp_hndl_t              hndl_dtp;
        dtp_leaf_data_t         leaf;
        hat_dtp_cm_db_t*        dtp_data;
    } cm;
    struct {
        dtp_hndl_t              hndl_dtp;
        dtp_leaf_data_t         leaf;
        hat_dtp_sensor_desc_t*  dtp_data;
    } sensor;
    struct {
        dtp_hndl_t              hndl_dtp;
        dtp_leaf_data_t         leaf;
        hat_dtp_lens_desc_t*    dtp_data;
    } lens;
    struct {
        dtp_hndl_t              hndl_dtp;
        dtp_leaf_data_t         leaf;
        hat_dtp_lights_desc_t*  dtp_data;
    } lights;
    struct {
        dtp_hndl_t              hndl_dtp;
        dtp_leaf_data_t         leaf;
        hat_dtp_nvm_desc_t*     dtp_data;
    } nvm;
} hat_cm_driver_dtp_db_t;

typedef struct {
    hat_dtp_cm_desc_t*      cm;
    hat_dtp_sensor_desc_t*  sensor;
    hat_dtp_lens_desc_t*    lens;
    hat_dtp_lights_desc_t*  lights;
    hat_dtp_nvm_desc_t*     nvm;
} hat_cm_driver_dtp_data_t;

typedef struct {
    HAT_CM_DRIVER_STATE_T       state;
    hat_cm_driver_dtp_db_t      dtp_db;
    hat_cm_driver_dtp_data_t    dtp_data;
    struct {
        hat_cm_ph_sen_handle_t    sensor;
        hat_cm_ph_lens_handle_t   lens;
        hat_cm_ph_lights_handle_t lights;
        hat_cm_ph_nvm_handle_t    nvm;
    } phys_handles;
    hat_cm_socket_obj_t         socket_obj;
    hat_cm_sensor_obj_t         sensor_obj;
    hat_cm_lens_obj_t           lens_obj;
    hat_cm_lights_obj_t         lights_obj;
    hat_cm_nvm_obj_t            nvm_obj;
} hat_cm_driver_prms_t;


typedef struct {
    hat_cm_driver_cr_prms_t cr_prms;
    hat_cm_driver_prms_t    prms;
} hat_cm_driver_prvt_data_t;

typedef struct {
    const hat_dtp_cm_desc_t* cm_entries[PH_SOCKET_MAX];
    hat_cm_driver_dtp_data_t module_desc[PH_SOCKET_MAX];
} hat_cm_driver_detected_t;

#endif // __HAT_CM_DRIVER_PRVT_H__
