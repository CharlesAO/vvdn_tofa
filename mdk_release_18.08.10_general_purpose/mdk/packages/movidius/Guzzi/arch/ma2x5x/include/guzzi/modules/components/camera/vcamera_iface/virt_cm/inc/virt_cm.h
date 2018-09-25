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
* @file virt_cm.h
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

#ifndef __VIRT_CM_H__
#define __VIRT_CM_H__

#include <hal/hal_camera_module/hat_cm_sensor.h>
#include <hal/hal_camera_module/hat_cm_lens.h>
#include <hal/hal_camera_module/hat_cm_lights.h>
#include <hal/hal_camera_module/hai_cm_driver.h>
#include <gzz_cam_config.h>

typedef void* virt_cm_hndl_t;

typedef struct
{
    gzz_capture_intent_t intent;
    hat_size_t resolution;
    hat_rect_t crop;
    uint32     fr_duration;
    hat_range_float_t fps_range;
    gzz_cam_custom_usecase_t usecase;
} virt_cm_sen_select_mode_t;

typedef enum {
    VSEN_CFG_INTENT,
    VSEN_CFG_RESOLUTION,
    VSEN_CFG_CROP,
    VSEN_CFG_FPS_RANGE,
    VSEN_CFG_USECASE
} virt_cm_sen_cfg_type_t;

typedef enum {
    VLENS_CFG_APERTURE,
    VLENS_CFG_DENS_FILT,
    VLENS_CFG_FOC_LENGTH,
    VLENS_CFG_O_STAB_MODE,
} virt_cm_lens_cfg_type_t;

typedef struct {
    hat_cm_sensor_dyn_props_t   sensor_props;
    hat_cm_lens_dyn_props_t     lens_props;
    hat_cm_flash_dyn_props_t    flash_props;
} virt_cm_dyn_props_t;

typedef struct {
    uint32 sen_mode_idx;
    hat_sensor_features_t *sen;
} virt_cm_sen_mode_features_t;

int virt_cm_sen_config (virt_cm_hndl_t hndl, virt_cm_sen_cfg_type_t cfg_type, void* cfg);
int virt_cm_sensor_exp_gain (virt_cm_hndl_t hndl, uint32 exp_time, float again);
// Lens interface
int virt_cm_lens_config (virt_cm_hndl_t hndl, virt_cm_lens_cfg_type_t cfg_type, void* cfg);
int virt_cm_lens_move_to_pos (virt_cm_hndl_t hndl,
                              hat_cm_lens_move_list_t* move_list,
                              hat_cm_lens_move_cb_t micro_move_done_cb,
                              void* user_data);
int virt_cm_lens_meas_pos (virt_cm_hndl_t hndl, float* pos);
int virt_cm_lens_init (virt_cm_hndl_t hndl);
int virt_cm_lens_abort (virt_cm_hndl_t hndl);

// Lights interface
int virt_cm_lights_exec_seq     (virt_cm_hndl_t hndl, hat_light_ctrl_list_t *ctrl);
int virt_cm_lights_get_features (virt_cm_hndl_t hndl, hat_flash_bounds_t** features);

// CM interface
int virt_cm_destroy (virt_cm_hndl_t hndl);
int virt_cm_create (virt_cm_hndl_t *hndl, uint32 cam_id);
int virt_cm_get_all_camera_comp_ids(virt_cm_hndl_t hndl, /*OUT*/ hat_cm_component_ids_t* component_ids);
int virt_cm_get_dynamic_properties(virt_cm_hndl_t hndl, /*OUT*/ virt_cm_dyn_props_t* props);
int virt_cm_get_mode_features(virt_cm_hndl_t hndl, /*OUT*/ virt_cm_sen_mode_features_t *features);
int virt_cm_set_operating_mode(virt_cm_hndl_t hndl, virt_cm_sen_mode_features_t *features, uint32 sen_mode_idx);
int virt_cm_set_stream_mode(virt_cm_hndl_t hndl, int mode);
int virt_cm_start (virt_cm_hndl_t hndl);
int virt_cm_stop (virt_cm_hndl_t hndl);
int virt_cm_detect ();

#endif /* __VIRT_CM_H__ */
