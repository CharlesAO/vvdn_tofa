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
* @file cam_algo_state.h
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

#ifndef CAM_ALGO_STATE_H_
#define CAM_ALGO_STATE_H_

#include "aca_awb_types.h"
#include "aca_af_types.h"
#include "aca_ae_calc_types.h"
#include "aca_ae_stab_types.h"
#include "sg_ae_smooth_types.h"
#include "sg_ae_distribution_types.h"
#include "aca_fmv_types.h"

//#include "vpipe_params/algo_vpipe.h"
#include "hal/hal_camera_module/hat_cm_sensor.h"
#include <osal/list_pool.h>

typedef struct {
    exposure_calc_output_t  ae_out;
    ae_stab_output_t        ae_stab;
    awb_calc_output_t       awb_out;
    focus_calc_output_t     focus;
    aca_afd_calc_output_t   afd_out;
    aca_fmv_calc_output_t   fmv_out;
} aca_algo_out_t;

typedef struct {
    ae_smooth_output_t      ae_smooth;
    ae_distr_calc_output_t  ae_distr;
}sg_algo_out_t;

typedef struct {
    hat_exp_gain_t   sensor;
}cam_ctrl_t;

typedef struct {
    uint32  fr_status;     // frame request status 0 = no error
    aca_algo_out_t aca;
    sg_algo_out_t  sg;
    cam_ctrl_t cam;
} cam_algo_state_t;


typedef struct  {
    struct osal_node               link;           // list link
    hat_light_ctrl_entry_t          ctrl;
}hat_light_ctrl_list_entry_t;

typedef list_pool_head_t hat_light_ctrl_list_t;

#endif /* CAM_ALGO_STATE_H_ */
