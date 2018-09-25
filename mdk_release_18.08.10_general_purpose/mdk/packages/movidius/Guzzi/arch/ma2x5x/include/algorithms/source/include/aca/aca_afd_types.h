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
* @file aca_afd_types.h
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

#ifndef  __ACA_INTERFACE_AFD_TYPES_H__
#define  __ACA_INTERFACE_AFD_TYPES_H__

#include <osal/osal_stdtypes.h>
#include <osal/osal_time.h>
#include "dtp/dtp_server_defs.h"
#include <hal/hat_types.h>
#include <hal/hal_camera_module/hat_cm_sensor.h>
#include <hal/hat_h3a_aewb.h>

// ---------------------------------------------------------------------------
// AFD

typedef enum {
    MAINS_FREQ_UNKNOWN = 0,
    MAINS_FREQ_50HZ = 50,         // 50Hz
    MAINS_FREQ_60HZ = 60,         // 60Hz
} aca_mains_freq_t;

typedef enum {
    FLICKER_NO_DETECT,
    FLICKER_IN_PROGRESS,
    FLICKER_DETECTED,
    FLICKER_ERROR,
} aca_afd_status_t;

typedef enum {
    // AFD modes
    AFD_CALC_MODE_50Hz_GOOD,  // Without flicker
    AFD_CALC_MODE_50Hz_BAD,   // With heavy flicker
    AFD_CALC_MODE_60Hz_GOOD,
    AFD_CALC_MODE_60Hz_BAD,
}afd_calc_modes_t;

typedef struct
{
    dtpdb_static_common_t * dtp_s_common;
} aca_afd_config_t;

// INPUT
typedef struct {
    dtpdb_dynamic_common_t *dtp_d_common;
    afd_calc_modes_t calc_mode;
    hat_h3a_aewb_stat_t *h3a;     // H3A data buffer
    hat_exp_gain_t      exp_gain;
} aca_afd_calc_input_t;

// OUTPUT
typedef struct
{
    aca_afd_status_t status;
    aca_mains_freq_t mains_freq;        // Mains Freq.

} aca_afd_calc_output_t;

// END AFD --------------------------------------------------------------------

#endif // __ACA_INTERFACE_AFD_TYPES_H__

