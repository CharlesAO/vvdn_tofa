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
* @file aca_awb_types.h
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

#ifndef  __ACA_INTERFACE_AWB_TYPES_H__
#define  __ACA_INTERFACE_AWB_TYPES_H__

#include <osal/osal_sysdep.h>
#include <osal/osal_stdtypes.h>
#include <osal/osal_time.h>
#include "dtp/dtp_server_defs.h"
#include <hal/hat_h3a_aewb.h>
#include <hal/hat_h3a_hist.h>
#include <hal/hat_wbal.h>
#include <hal/hat_matrix.h>
#include <hal/hat_faces.h>
#include <hal/hat_priority.h>
#include <hal/hal_camera_module/hat_cm_sensor.h>
#include <hal/hat_light.h>

#include "aca_afd_types.h"

#define STABILIZATION_BUFFER_SZ         12

#define MAX_STORE_RESTORE_BUFF_SIZE     30

/* Sets the desired white balance mode. */
typedef enum  {
    AWB_MODE_AUTO = 0,           /* Auto detect */
    AWB_MODE_MANUAL_KELVINS2GAINS, /* Manual mode temperature to gains*/
    AWB_MODE_MANUAL_GAINS2KELVINS, /* Manual mode gains to temperature*/
    AWB_MODE_DAYLIGHT,           /* Daylight mode */
    AWB_MODE_SUNSET,             /* Sunset mode */
    AWB_MODE_CLOUDY,             /* Cloudy mode */
    AWB_MODE_TUNGSTEN,           /* Tungsten mode */
    AWB_MODE_FLUORESCENT,        /* Fluorescent mode */
    AWB_MODE_INCANDESCENT,       /* Incandescent mode */
    AWB_MODE_OFFICE,             /* Office mode */
    AWB_MODE_SHADOW,             /* Shadow mode */
    AWB_MODE_FLASH,              /* Flash mode */
    AWB_MODE_HORIZON,            /* Horizon mode */
    AWB_MODE_WARM_FLUORESCENT,
    AWB_MODE_UNDERWATER,
    AWB_MODE_TWILIGHT,
    AWB_MODE_FIREWORKS,
    AWB_MODE_SUN,
    AWB_MODE_CNT
} awb_modes_t;

/*
 *  Following structure represent main Auto White balance algorithm operating modes.
 *  AWB mode - enum awb_calc_mode
 */
typedef enum  {
    AWB_CALC_MODE_PREVIEW = 0,      /* calculation for preview */
    AWB_CALC_MODE_PREVIEW_FAST,     /* AWB calculation for preview */
    AWB_CALC_MODE_CAPTURE,          /* AWB calculation for capture */
    AWB_CALC_MODE_CAPTURE_FAST,     /* AWB calculation for fast capture AWB */
    AWB_CALC_MODE_CAPTURE_FLASH,    /* AWB calculation for capture with flash used */
} aca_awb_calc_mode_t;

/*
 *  AWB Control - enum awb_control
 */
typedef enum {
    AWB_CTRL_MODE_NORMAL = 0,       /* AWB normal control mode */
    AWB_CTRL_MODE_SUSPEND           /* AWB suspend mode */
}aca_awb_control_t;

typedef enum {
    AWB_STATE_OFF = 0,
    AWB_STATE_LOCKED,
    AWB_STATE_RUNNING,
    AWB_STATE_FROZEN,
    AWB_STATE_MAX
} awb_state_t;

/*
 * Next Table represents AWB algorithm parameters that can be changed run time.
 * Structure was filled from 3AFW and send to AWB algorithm using *_control() API.
 * AWB configuration structure - struct awb_configuration
 */
typedef struct  {
    dtpdb_static_common_t * dtp_s_common;
    awb_modes_t             wb_mode;        /* Daylight, Cloudy... */
    uint16                  manual_colour_temp; /* Manual color temperature*/
    hat_wbal_coef_t         manual_wbal_coef; /* Manual awb gains */
    hat_rgb2rgb_t           manual_rgb2rgb; /* Manual settings of RGB2RGB */
}aca_awb_config_t;

/*
 * Following table shows input parameters passed to AWB *_process() method.
 * It contain H3A, Histogram statistic data, Sensor and ISP gain and exposure
 * time, which are used for collection of this data. Structure was filled from
 * 3AFW and send to AWB algorithm using *_process() API.
 * AWB Input - struct awb_calculation_input
*/
typedef struct  {
    dtpdb_dynamic_common_t *dtp_d_common;
    aca_awb_control_t control;              /* The control mode */
    aca_awb_calc_mode_t calculation;
    hat_exp_gain_t      exp_gain;           /* Pointer to exposure and gain */
    uint16  flash_light_ratio;              /* flash light ratio*/
    hat_lights_state_t flash;               /* LED intensities during the
                                               captured statistics */
    hat_faces_t faces;                      /* Structure for face detection */
    hat_reg_pri_t wb_region;                /* Structure for exposure regions */
    aca_mains_freq_t mains_freq;

    hat_h3a_aewb_stat_t *h3a_stat;          /* Pointer to H3A data buffer */
    hat_h3a_hist_stat_t hist;               /* Pointer to histogram data */
}aca_awb_calc_input_t;


typedef struct
{
    float sumR;
    float sumG;
    float sumB;
    float gainR;
    float gainG;
    float gainB;
    int32  Y_mean;
    uint32 total_expo_ms;
    uint16 awb_calc_type;
    uint16 enb_calc;
    uint32 Scene_white;
} aca_wb_data_info_t;
/*
Output from AWB algorithm is shown in next table . AWB algorithm is responsible
to fill this structure and the end of *_process () API.
AWB computation done - struct awb_calculation_output
*/
typedef struct {
    hat_wbal_coef_t wbal_coef;
    aca_wb_data_info_t info;
    uint32          colour_temp;
    uint32          converged;              /* 1 : WB succeeded, 0: failed */
    uint32          confidence;
    awb_state_t     state;
} awb_calc_output_t;


#endif /* __ACA_INTERFACE_AWB_TYPES_H__ */

