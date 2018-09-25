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
* @file sg_ae_limits_types.h
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

#ifndef  __SG_AE_LIMITS_TYPES_H__
#define  __SG_AE_LIMITS_TYPES_H__

#include <hal/hat_types.h>
#include <hal/hal_camera_module/hat_cm_sensor.h>
#include <hal/hat_light.h>
#include <hal/hal_camera_module/hat_aperture.h>
#include <hal/hat_dgain.h>
#include <dtp/dtp_server_defs.h>

#define MAX_AE_STAGES                   (10)

// AE control mode - enum fw_ae_exp_mode
typedef enum {
    SG_AE_MODE_AUTO = 0,             // Auto mode
    SG_AE_MODE_BACKLIGHT,            // Backlight mode - the backlight is less
                                     // weighted in calculations
    SG_AE_MODE_NIGHT,                // Night mode - longer exposure time
    SG_AE_MODE_SPORT,                // Sport mode - shorter exposure time
    SG_AE_MODE_VERYLONG,             // Very long mode - long exposure
    SG_AE_MODE_MACRO,
    SG_AE_MODE_PORTRAIT,
    SG_AE_MODE_LANDSCAPE,
    SG_AE_MODE_NIGHT_PORTRAIT,
    SG_AE_MODE_MANUAL,
    SG_AE_MODE_THEATRE,
    SG_AE_MODE_COUNT
} ae_exp_mode_t;

typedef enum {
    SG_AE_DISTR_MODE_PREVIEW,
    SG_AE_DISTR_MODE_CAPTURE,
    SG_AE_DISTR_MODE_VIDEO,
    // FLASH Modes used based on flash_required from AE and flash modes
    SG_AE_DISTR_MODE_PRE_FLASH,
    SG_AE_DISTR_MODE_CAPTURE_FLASH,
    SG_AE_DISTR_MODE_AF_ASSIST
}sg_ae_distr_calc_modes_t;

/**
 * represents current sensor mode limitations.
 *
 * @param shutter    min/max the exposure time lower limit, [us]; 0 - auto
 * @param again      min/max the analog gain lower limit, [0.1EV]; 0 - auto
 * @param dgain      min/max the digital gain lower limit, [0.1EV]; 0 - auto
 * @param aperture   To be developed
 */
typedef struct
{
    hat_range_t shutter;  // min/max the exposure time limit, [us]
    hat_range_float_t again;    // min/max the analog gain limit
    hat_range_float_t dgain;    // min/max the digital gain limit
    hat_range_t aperture_exp;   // min/max exposure thresholds for aperture
    hat_range_float_t aperture_gain;  // min/max gain thresholds for aperture
} sg_ae_distr_range_limits_t;

/* ========================================================================== */
/**
* This structure contains parameters for multistage AE
* for a specific AE mode.
*/
/* ========================================================================== */

typedef struct multistage_ae_t
{
    /** Exposure multistage thresholds for multistage AE, microseconds
    If less then MAX_AE_STAGES stages are needed, pad with 0.
    !!! MUST be in accending order !!!
    */
    uint32  exposure_us[MAX_AE_STAGES];

    /** Gain multistage thresholds for multistage AE, U16Q8
    If less then MAX_AE_STAGES stages are needed, pad with 0.
    Gain entries may be as many as exposure entries on one less.
    !!! MUST be in accending order !!!
    */
    float  gain[MAX_AE_STAGES];
} multistage_ae_t;

// ---------------------------------------------------------------------------
// AE Limits

typedef struct {
    dtpdb_static_common_t * dtp_s_common;
    ae_exp_mode_t         exp_mode;      // AE exposure mode
} sg_ae_limits_config_t;

// INPUT

typedef struct
{
    dtpdb_dynamic_common_t *dtp_d_common;
    sg_ae_distr_calc_modes_t distr_mode;
    hat_range_float_t   fps;                 // Frame rate required by application Min/Max fps. To select fixed frame rate min==max=fps
    uint32 ae_change_ratio;                  // Required change ratio for algorithm U8Q8
} sg_ae_limits_input_t;

// OUTPUT

typedef struct
{
    sg_ae_distr_range_limits_t limits;
    multistage_ae_t            multistage;
    uint32 ae_change_ratio;                 // Required change ratio for algorithm U8Q8
} sg_ae_limits_output_t;

// END AE Limits---------------------------------------------------

#endif // __SG_AE_LIMITS_TYPES_H__
