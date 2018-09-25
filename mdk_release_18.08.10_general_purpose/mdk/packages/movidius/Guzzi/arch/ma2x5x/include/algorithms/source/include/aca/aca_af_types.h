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
* @file aca_af_types.h
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

#ifndef  __ACA_INTERFACE_AF_TYPES_H__
#define  __ACA_INTERFACE_AF_TYPES_H__

#include <osal/osal_sysdep.h>
#include <osal/osal_stdtypes.h>
#include <osal/osal_time.h>
#include "dtp/dtp_server_defs.h"
#include <hal/hat_h3a_af.h>
#include <hal/hat_priority.h>
#include <hal/hat_faces.h>
#include "aca_fmv_types.h"

typedef enum
{
    AF_RANGE_NORMAL,
    AF_RANGE_MACRO,
    AF_RANGE_EXTENDED,
    AF_RANGE_CUSTOM1,
    AF_RANGE_CUSTOM2,
    AF_RANGE_INFINITY,
    AF_RANGE_HYPERFOCAL,
    MAX_AF_MODE_MODE
} focus_range_t;

typedef enum
{
    AF_MODE_OFF,
    AF_MODE_MANUAL,
    AF_MODE_SINGLE_FOCUS,
    AF_MODE_CONTINUOUS
} focus_mode_t;

typedef enum
{
    AF_SPOT_MULTI_AVERAGE,
    AF_SPOT_SINGLE_CENTER,
    AF_SPOT_MULTI_CENTER,
    AF_SPOT_MULTI_NORMAL
} focus_spot_weighting_t;

typedef enum {
    AF_STATE_RUNNING = 0,
    AF_STATE_FAIL,
    AF_STATE_SUCCESS,
    AF_STATE_SUSPEND,
    AF_STATE_IDLE,
    AF_STATE_WAIT_STABLE_SCENE
}focus_state_t;

typedef struct
{
    dtpdb_static_common_t * dtp_s_common;
    focus_mode_t  mode;
    focus_range_t range;
    focus_spot_weighting_t spot_weighting;
    float manual_lens;
} aca_af_config_t;

typedef struct {
    uint32 ae_conv_sts;
    uint32 req_total_exposure;
    hat_exp_gain_t applied_settings;
}aca_af_exposure_input;

typedef struct
{
    dtpdb_dynamic_common_t *dtp_d_common;
    hat_h3a_af_stat_t *af_stats;
    hat_faces_t        face_detect;   // Structure for face detection
    hat_reg_pri_t      focus_region;  // Structure for focus regions
    aca_fmv_calc_output_t  fmv_input;
    aca_af_exposure_input  exp_prms;
    focus_state_t          af_state; // HACK. Remove Me!!!

} aca_af_calc_input_t;

typedef enum {
    AF_LENS_MOVE_NONE,
    AF_LENS_MOVE_GLOBAL,
    AF_LENS_MOVE_LOCAL,
} move_type_t;

typedef struct
{
    uint32 seq_cnt;       // Focus iteration sequence counter - coming from aca_af_calc_input_t
    move_type_t   move;
    float pos;  // 0.0 - 1.0
    focus_state_t status;
} focus_calc_output_t;

typedef struct
{
    focus_calc_output_t    focus_out;       // Output structure from AF/CAF algorithm
} aca_focus_output_t;


#endif /* __ACA_INTERFACE_AF_TYPES_H__ */

