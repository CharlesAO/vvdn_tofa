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
* @file sg_exp_bracketing.h
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

#ifndef  __SETTINGS_GENERATOR_EXP_BRACKETING_H__
#define  __SETTINGS_GENERATOR_EXP_BRACKETING_H__

#include <osal/osal_stdtypes.h>
#include <osal/osal_time.h>

#include <hal/hat_types.h>
#include <hal/hat_priority.h>
#include <hal/hat_aperture.h>
#include <hal/hat_h3a_aewb.h>
#include <hal/hat_faces.h>

#include <fw3a/fw3a_exposure.h>

#include "algos/fwat_common.h"
#include "algos/fwat_version.h"
#include "algos/fwat_flash.h"

#define MAX_REC_STAGES (10)

// AE calculation mode - enum fwat_ae_calc_mode
enum fwat_ae_calc_mode {
    AE_CALC_MODE_PREVIEW = 0,           // Calculation for viewfinder
    AE_CALC_MODE_PREVIEW_FAST,          // Calculation for quick viewfinder
    AE_CALC_MODE_ALLOT,                 // Allot calculation mode
    AE_CALC_MODE_ALLOT_CAPTURE,         // Allot Capture calculation mode
    AE_CALC_MODE_FOCUS,                 // calculation mode for focus
    AE_CALC_MODE_CAPTURE,               // Calculation for capure
    AE_CALC_MODE_CAPTURE_PREFLASH,      // Calculation for preflash
    AE_CALC_MODE_CAPTURE_PREFLASH_BRIGHT, // Calculation for preflash with bright frame stats
    AE_CALC_MODE_CAPTURE_PREFLASH_DARK,   // Calculation for preflash with dark frame stats
    AE_CALC_MODE_CAPTURE_MAINFLASH,     // Calculation for mainflash
    AE_CALC_MODE_ZERO_SHUTTER,          // Calculation for zero shutter lag mode
    AE_CALC_MODE_BRACKETING             // Bracketing
};

enum fwat_ae_view_mode {
    AE_VIEW_MODE_VID = 0,
    AE_VIEW_MODE_CAP
};

// AE control mode - enum fwat_ae_ctrl_mode
enum fwat_ae_ctrl_mode {
    AE_CTRL_MODE_NORMAL = 0,            // AE normal control mode
    AE_CTRL_MODE_SUSPEND                // AE suspend mode
};

enum ae_correct_flicker {
    REC_MAINS_50HZ = 0,                 // 50Hz
    REC_MAINS_60HZ,                     // 60Hz
    REC_MAINS_OFF                       // Disabled
};

enum exposure_status {
    AE_STATUS_UNDER = 0,
    AE_STATUS_OVER,
    AE_STATUS_GOOD,
    AE_STATUS_PREFLASH     // Preflash done?
};


// Indicates risk of camera shake. For instance, if exposure time is
// greater than 1/f but less than 2/f, camera shake can be considered
// MEDIUM_RISK. If exposure time is greater than 2/f, camera_shake can be
// considered a HIGH_RISK.
enum exposure_shake_risk {
    AE_SHAKE_LOW_RISK = 0,
    AE_SHAKE_MEDIUM_RISK,
    AE_SHAKE_HIGH_RISK
};

enum exposure_bracketing_sequence {
    AE_BRACKETING_SEQUENCE_N_MIN_MAX,    // NORMAL MIN MAX sequence
    AE_BRACKETING_SEQUENCE_N_MAX_MIN,    // NORMAL MAX MIN sequence
    AE_BRACKETING_SEQUENCE_MIN_N_MAX,    // MAIN NORMAL MAX sequence
    AE_BRACKETING_SEQUENCE_MAX_N_MIN,    // MAX MIN NORMAL sequence
    AE_BRACKETING_SEQUENCE_MIN_MAX_N,    // MIN MAX NORMAL sequence
    AE_BRACKETING_SEQUENCE_MAX_MIN_N,    // MAX MIN NORMAL sequence
    AE_BRACKETING_SEQUENCE_N_MIN_MAX_ALT, // NORMAL MIN MAX alternating sequence
    AE_BRACKETING_SEQUENCE_N_MAX_MIN_ALT, // NORMAL MAX MIN alternating sequence
};

// AE manual parameters - struct ae_manual_prms
struct ae_manual_prms {
    enum fw3a_ae_spot_weighting spot_weighting;  // Parameter related to the
    uint32 shutter;                         // Manual exposure time,
                                            // [us]. 0 - auto
    uint32 again;                           // Manual analogue gain,
                                            // [0.1EV]. 0 - auto
    uint32 dgain;                           // Manual digital gain,
                                            // [0.1EV]. 0 - auto
    uint32 aperture;                        // Manual aperture. 0 - auto
    uint16 mask;                            // Mask - bitmask that shows
                                            // which manual parameter is
                                            // enabled
};

// Next Table defines structure representing current sensor mode limitations.
// Structure was filled from 3AFW and passed to AE algorithm using *_control() API.
// AE range - struct  ae_range_limits
struct ae_range_limits {
    uint32 min_shutter;                     // the exposure time lower
                                            // limit, [us]; 0 - auto
    uint32 max_shutter;                     // the exposure time upper
                                            // limit, [us]; 0 - auto
    uint32 min_again;                       // the analogue gain lower
                                            // limit, [0.1EV]; 0 - auto
    uint32 max_again;                       // the analogue gain upper
                                            // limit, [0.1EV]; 0 - auto
    uint32 min_dgain;                       // the digital gain lower
                                            // limit, [0.1EV]; 0 - auto
    uint32 max_dgain;                       // the digital gain upper
                                            // limit, [0.1EV]; 0 - auto
    uint32 aperture;                        // To be developed
};

typedef struct
{
    /** Exposure multistage thresholds for multistage AE, microseconds
    If less then MAX_REC_STAGES stages are needed, pad with 0.
    !!! MUST be in ascending order !!!
    */
    uint32  exposure_us[MAX_REC_STAGES];

    /** Gain multistage thresholds for multistage AE, U16Q8
    If less then MAX_REC_STAGES stages are needed, pad with 0.
    Gain entries may be as many as exposure entries on one less.
    !!! MUST be in ascending order !!!
    */
    uint16  gain[MAX_REC_STAGES];
} rel_multistage_ae_t;

// Enumeration used for frames description during preflash sequence
enum ae_flash_calc_mode {
    AE_FLASH_MODE_LED,
    AE_FLASH_MODE_XENON,
    AE_FLASH_MODE_CAPTURE
};

// ---------------------------------------------------------------------------
// AE Distribution
// INPUT
enum fw3a_mains_freq mains_freq;        // Mains Freq.
uint8 framerate;                        // Framerate required by application
struct ae_manual_prms manual;           // Manual exposure parameters
struct ae_range_limits limits;          // Shutter, gain and aperture limits

// OUTPUT
struct hat_exp_gain    exp_gain;
struct hat_lens_config aperture;                // Aperture

struct hat_exp_gain    exp_gain_preflash;
struct hat_lens_config aperture_pre_flash;      // Aperture for preflash

struct hat_exp_gain    exp_gain_cap;
struct hat_lens_config aperture_cap;            // Aperture for capture
// END AE Distribution---------------------------------------------------


// ---------------------------------------------------------------------------
// AFD
// INPUT
hat_h3a_aewb_stat_t *h3a;     // H3A data buffer
struct hat_exp_gain      exp_gain;

uint32 rows_in_pax;                     // needed for flicker detection
uint32 row_time_10ns;                   // needed for flicker detection
                                        // region of interest
uint32 ver_win_count;                   // needed for flicker detection
uint32 hor_win_count;                   // needed for flicker detection


// OUTPUT
enum fw3a_mains_freq mains_freq;        // Mains Freq.
// END AFD --------------------------------------------------------------------

//
// Complete Auto Exposure configuration structure was shown in next table.
// Structure was filled from 3AFW and passed to AE algorithm using *_control() API.
// AE configuration structure - struct ae_configuration
struct ae_configuration {
    enum fw3a_ae_exp_mode exp_mode;         // AE exposure mode
    enum fwat_ae_view_mode view_mode;       // AE mode indicating wether we
                                            // are in video or capture
                                            // viewfinder.
    enum ae_flash_calc_mode flash_calc_mode;// Stage in preflash/mainflash sequence
    int16 EV_compensation;                  // exposure compensation,
                                            // [EV] (positive and negative)
    uint16 draft2fine_sens;                 // Sensor mode (sensitivity)
                                            // 0x100 is capture sensitivity
    uint16 iso_base;                        // ISO for the current sensor, ISO setting based on reference ISO
};

struct preflash_frame_params {
    void  *frame_ptr;
    uint32 frame_size_x;                    // X size in bytes of preflash frame
    uint32 frame_size_y;                    // Y size in bytes of preflash frame
    uint32 frame_ppln;                      // Pixel per line of preflash frame
};

//
// Following table describe complete H3A statistic information that is passed to
// Auto Exposure algorithm. Structure was filled from 3AFW and passed to AE algorithm
// using *_process() API.
// AE Input - struct ae_calculation_input
struct ae_calculation_input {
    enum fwat_ae_ctrl_mode ctrl_mode;           // AE control mode
    enum fwat_ae_calc_mode calc_mode;           // AE calculation mode
    struct hat_lens_config aperture;            // the aperture used while collecting statistics,
    struct fw_flash_state flash_state;          // LED intensities during the captured statistics
    struct preflash_frame_params preflash_frm;  // Preflash frame
    uint32 o_distance;                           // distance to object: 0 macro mode, !0 other modes

    hat_h3a_aewb_cfg_t  h3a_cfg;  // H3A configuration
    hat_h3a_aewb_stat_t *h3a;     // H3A data buffer
    struct hat_exp_gain      exp_gain;
//    uint32 sensor_dgain;                // Sensor digital gain
    hat_wbal_coef_t wbal_coef;    // WBal coef
    hat_faces_t     face_detect;   // Structure for face detection
    hat_reg_pri_t   exp_region;    // Structure for exposure regions
};

//
// Output from Auto Exposure algorithm is shown in next table. Structure was
// filled from AE algorithm.
// AE calculation complete structure - struct ae_calculation_output
struct ae_calculation_output {


    uint32 texp;              // total exposure of the statistics, [us]
    uint32 texp_flash;        // total exposure of the statistics, [us]
    uint32 texp_pre_flash;    // total exposure of the statistics, [us]

    uint16 illuminance;       // absolute illumination of the scene, [lx]

    uint16 ave_y_level;       // the average Y level, calculated from the
                              // statistics

    uint32 converged;         // 1: the algorithm is converged; 0: the
                              // algorithm is not converged

    uint16 flash_light_ratio; // flash light ratio

//    uint16 af_assist_required;              // af assist control flag

    uint8 ae_reach_target;    // 1: reached required total exposure
                              // 0 - not reached, yet.

    enum exposure_status status;

    enum exposure_shake_risk risk;

    uint32   required_flash;     // 0 - No;
                                 // 1 - Yes requires additional light - main flash,
                                 // torch, assist etc.

    struct fw_flash_ctrl flash; // Suggested main/pre-flash flash intensity
};

#endif // __SETTINGS_GENERATOR_EXP_BRACKETING_H__

