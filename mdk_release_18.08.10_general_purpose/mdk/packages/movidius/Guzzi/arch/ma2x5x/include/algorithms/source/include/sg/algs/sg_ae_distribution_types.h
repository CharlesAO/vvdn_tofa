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
* @file sg_ae_distribution_types.h
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

#ifndef  __SG_AE_DISTRIBUTION_TYPES_H__
#define  __SG_AE_DISTRIBUTION_TYPES_H__

#include <hal/hat_types.h>
#include <hal/hal_camera_module/hat_cm_sensor.h>
#include <hal/hat_light.h>
#include <hal/hal_camera_module/hat_aperture.h>
#include <hal/hat_dgain.h>

#include "aca_afd_types.h"
#include "sg_ae_limits_types.h"

#define MAX_REC_STAGES (10)


// AE calculation mode - enum fwat_ae_calc_mode
typedef enum {
    ACA_AE_CALC_MODE_PREVIEW = 0,             // Calculation for viewfinder
    ACA_AE_CALC_MODE_CAPTURE_PREFLASH,        // Calculation for preflash with bright frame stats
} aca_ae_calc_mode_t;

#if 0 // Could be replaced with range structure
// AE manual parameters - struct ae_manual_prms
typedef struct {
    uint32 shutter;      // Manual exposure time, [us]. 0 - auto
    uint32 manual_ISO;   // sensor sensitivity
    uint32 aperture;     // Manual aperture. 0 - auto
    uint16 mask;         // Mask - bitmask that shows which manual parameter is enabled
} sg_ae_distr_manual_prms_t;
#endif


// Enumeration used for frames description during preflash sequence
typedef enum {
    SG_AE_FLASH_MODE_LED,
    SG_AE_FLASH_MODE_XENON,
}sg_ae_distr_flash_calc_mode_t; // TODO:

/**
 * Indicates risk of camera shake. For instance, if exposure time is
 * greater than 1/f but less than 2/f, camera shake can be considered
 * MEDIUM_RISK. If exposure time is greater than 2/f, camera_shake can be
 * considered a HIGH_RISK.
 */
typedef enum {
    SG_AE_SHAKERISK_LOW = 0,
    SG_AE_SHAKERISK_MEDIUM,
    SG_AE_SHAKERISK_HIGH
} exposure_shake_risk_t;

// ---------------------------------------------------------------------------
// AE Distribution

typedef struct {
    dtpdb_static_common_t * dtp_s_common;
//    sg_ae_distr_ctrl_mode_t         ctrl_mode;
    sg_ae_distr_flash_calc_mode_t   flash_mode;
    hat_flash_bounds_t              litghts;      // light sources capabilities
} sg_ae_distr_config_t;

/*
 * Flash control - struct flash_control
 */
typedef struct
{
    hat_light_source_type_t light_src;
    uint32                  intensity;       /* Flash intensity to be used:  0..100% */
    int32                   start_time;      /* flash start delay relative to exposure start */
    uint32                  duration;        /* Flash duration [us] 0 - infinity */
}sg_distr_flash_ctrl_t;

typedef enum {
    SG_AE_DISTR_PRIORITY_AUTO,
    SG_AE_DISTR_PRIORITY_SHUTTER,
    SG_AE_DISTR_PRIORITY_GAIN
}sg_ae_distr_priority_t;

typedef struct
{
    dtpdb_dynamic_common_t *dtp_d_common;
    sg_ae_distr_priority_t   ae_priority;
    sg_ae_distr_calc_modes_t distr_mode;

    uint32                   texp;                // total exposure [us] to be distributed
    float                   exp_compensation;     // exposure compensation,

    aca_mains_freq_t     mains_freq;         // Mains frequency.

    // ????? TODO aren't they a tuning (DTP) settings or Sensor mode/Lens limitations
    sg_ae_distr_range_limits_t limits;            // Shutter, gain and aperture limits

    multistage_ae_t            multistage;

    uint32                   row_time;            // [ns] - Current readout time of line & blanking

    // ???? TODO does this really need - sensor will calculate (vary) them
    //uint32                   curent_blanking_lines; // Number of blanking lines

    uint32                   o_distance;        // distance to object in mm: 0 - not valid

    float                   sensitivity;

    uint16                   ave_y_level;       // the average Y level

    /** In case of AE with face tracking, AEC rises this flag in above scenario and
    when the faces are too dark, compared to the rest of the image, i.e. when
    AE regulates the brightness of the BIGGEST face  to its minimum acceptable
    value. */
    uint8                   ft_flash_required;
    uint32                  ae_change_ratio;   // Required change ratio for algorithm U8Q8
} sg_ae_distr_calc_input_t;

// OUTPUT

typedef struct
{
    hat_exp_gain_t          exp_gain;
    hat_dgain_t             pre_gain;                   // Before H3A
    hat_dgain_t             post_gain;                  // After H3A
    hat_lens_ctrl_t         aperture;
    hat_light_ctrl_entry_t  flash;
    uint32                  texp;
    uint8                   req_flash;       // 1 - Yes requires additional light
    uint8                   req_af_assist;   // 1 - Yes requires additional light during focus
    exposure_shake_risk_t   shake_risk;
    uint8                   ae_reach_target; // 1: reached required total exposure
    uint8                   distr_conv;      // 1 - AE is stable.
} ae_distr_calc_output_t;

// END AE Distribution---------------------------------------------------

#endif // __SG_AE_DISTRIBUTION_TYPES_H__

