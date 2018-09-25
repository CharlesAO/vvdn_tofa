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
* @file hat_cm_sensor_prvt.h
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

#ifndef __HAT_CM_SENSOR_PRVT_H__
#define __HAT_CM_SENSOR_PRVT_H__

#include <hal/hal_camera_module/hat_cm_sensor.h>
#include <hal/hal_camera_module/hat_cm_socket.h>
#include <hal/modules/hal_camera_module/sensors/sensor_phys.h>

typedef struct {
    HAT_CM_SENSOR_STATE_T       state;
    hat_cm_sensor_dyn_props_t   dyn_props;
    HAT_CM_SENSOR_POWER_MODE_T  pwr_mode;
    HAT_CM_SENSOR_STREAM_MODE_T stream_mode;
    hat_exp_gain_t              exp_gain_curr;
    hat_cm_sensor_frm_timing_t  ft_curr;
    hat_range_float_t           fps_range;
    hat_range_t                 exp_range;
    hat_range_float_t           again_range;
    hat_range_float_t           dgain_range;
    void                        *sen_specific_params;

    hat_cm_ph_sen_handle_t      ph_sen_hndl;
    const hat_sensor_features_t *sens_features;
    hat_cm_socket_handle_t      socket_hndl;
} hat_cm_sensor_prms_t;

typedef struct {
    hat_cm_sensor_cr_prms_t cr_prms;
    hat_cm_sensor_prms_t    prms;
} hat_cm_sensor_prvt_data_t;

#endif // __HAT_CM_SENSOR_PRVT_H__
