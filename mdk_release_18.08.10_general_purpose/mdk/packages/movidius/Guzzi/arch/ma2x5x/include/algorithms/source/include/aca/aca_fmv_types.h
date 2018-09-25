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
* @file aca_fmv_types.h
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

#ifndef  __ACA_INTERFACE_FMV_TYPES_H__
#define  __ACA_INTERFACE_FMV_TYPES_H__

#include <osal/osal_stdtypes.h>
#include <osal/osal_time.h>
#include "dtp/dtp_server_defs.h"
#include <hal/hat_types.h>
#include <hal/hal_camera_module/hat_cm_sensor.h>
#include <hal/hat_h3a_aewb.h>

// ---------------------------------------------------------------------------
// FMV

typedef struct
{
    dtpdb_static_common_t * dtp_s_common;
} aca_fmv_config_t;

// INPUT
typedef struct {
    dtpdb_dynamic_common_t *dtp_d_common;
    hat_h3a_aewb_stat_t *h3a;     // H3A data buffer
    hat_exp_gain_t      *exp_gain;
    uint32               total_exp_us;
    uint32               use_exp;
} aca_fmv_calc_input_t;

// OUTPUT
typedef struct
{
    int32 cam_pos_x;
    int32 cam_pos_y;
    int32 cam_zoom;
    int32 scene_num;
    uint32 bad_match;
    uint32 stable_scene;
} aca_fmv_calc_output_t;

// END FMV --------------------------------------------------------------------

#endif // __ACA_INTERFACE_FMV_TYPES_H__

