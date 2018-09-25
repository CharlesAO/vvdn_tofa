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
* @file aca_ae_calc_types.h
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

#ifndef  __ACA_INTERFACE_AE_CALC_TYPES_H__
#define  __ACA_INTERFACE_AE_CALC_TYPES_H__

#include <osal/osal_stdtypes.h>

#include "aca_ae_types.h"
#include "dtp/dtp_server_defs.h"
#include <hal/hal_camera_module/hat_aperture.h>
#include <hal/hat_priority.h>
#include <hal/hat_light.h>
#include <hal/hat_h3a_aewb.h>
#include <hal/hal_camera_module/hat_cm_sensor.h>
#include <hal/hat_faces.h>
#include <hal/hat_wbal.h>

// AE spot weighting modes
typedef enum  {
    ACA_AE_SPOTWEIGHTING_NORMAL = 0,
    ACA_AE_SPOTWEIGHTING_CENTER,
    ACA_AE_SPOTWEIGHTING_WIDE,
    ACA_AE_SPOTWEIGHTING_BACKLIGHT,
} exposure_spot_weighting_t;

typedef enum {
    ACA_AE_STATUS_UNDER = 0,
    ACA_AE_STATUS_OVER,
    ACA_AE_STATUS_GOOD,
} exposure_status_t;


/**
 * Auto Exposure configuration
 *
 * @param view_mode               video or capture view finder.
 *
 * @param exp_mode                AE exposure mode in case of conflict exp_mode
 *                                is with priority over metering mode. For example
 *                                ACA_AE_MODE_BACKLIGHT will overwrite metering mode
 *
 * @param metering                selects metering mode
 *                                Note: exp_mode has highest priority
 *
 * @param EV_compensation         exposure  compensation, [EV] (positive and negative)
 *
 * @param draft2fine_sens sensor  sensitivity in current mode relative to capture. 0x100
 *                                is capture sensitivity
 *
 * @param iso_base                ISO for the current sensor, ISO setting based
 *                                on reference ISO
 *
 */
typedef struct {
    dtpdb_static_common_t *  dtp_s_common;
    exposure_spot_weighting_t metering;   // selects metering mode
    float sensitivity;                      // Sensor mode sensitivity against full mode
    uint16 iso_base;                         // ISO for the current sensor
} aca_ae_calc_cfg_t;

/**
 * Auto Exposure input data
 *
 * @param ctrl_mode      AE control mode
 * @param calc_mode      AE calculation mode
 * @param aperture       the aperture used while collecting statistics,
 * @param flash_state    LED intensities during the captured statistics
 * @param o_distance     distance to object: 0 macro mode, !0 other modes
 * @param h3a_cfg        H3A configuration
 * @param *h3a           H3A data buffer
 * @param exp_gain       the exposure and gain used while collecting statistics,
 * @param wbal_coef      WBal coefficients
 * @param face_detect    contains detected faces
 * @param exp_region     Structure for exposure regions
 */
typedef struct {
    dtpdb_dynamic_common_t *dtp_d_common;
//    aca_ae_ctrl_mode_t ctrl_mode;         // AE control mode
//    aca_ae_calc_mode_t      calc_mode;           // AE calculation mode
    hat_lens_state_t        aperture;            // the aperture used while collecting statistics,
    hat_lights_state_t      flash_state;         // LED intensities during the captured statistics

    hat_h3a_aewb_stat_t *h3a;      // H3A data buffer

    hat_exp_gain_t            applied;
    float                    pre_gain;       // H3A pre-gain

    hat_wbal_coef_t     wbal_coef;     // WBal coef
    hat_faces_t         face_detect;   // Structure for face detection
    hat_reg_pri_t       exp_region;    // Structure for exposure regions
} aca_ae_calc_input_t;


typedef struct
{
    uint32 req_texp;          // calculated total exposure against sensor full mode [us]
    uint16 illuminance;       // absolute illumination of the scene, [lx]
    uint16 ave_y_level;       // the average Y level
    uint32 ae_change_ratio;   // Required change ratio for algorithm U8Q8
    uint32 flash_light_ratio; // U32Q16 flash light ratio
    exposure_status_t status;
    /** In case of AE with face tracking, AEC rises this flag in above scenario and
    when the faces are too dark, compared to the rest of the image, i.e. when
    AE regulates the brightness of the BIGGEST face  to its minimum acceptable
    value. */
    uint8                   ft_flash_required;
} exposure_calc_output_t;

#endif // __ACA_INTERFACE_AE_CALC_TYPES_H__

