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
* @file hat_nvm.h
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

#ifndef __HAL_TYPES_OTP_H__
#define __HAL_TYPES_OTP_H__

#include <hal/hat_types.h>

#define HAT_OTP_LIB_MAX_SPOTS           (64)
#define HAT_OTP_LIB_MAX_BL_GAINS        (8)

/**
* This structure contain general sensor specific info->
*/
struct hat_otp_version {
    uint32 manufacturer_id;
    uint16 sensor_id;
    uint16 camera_module_ver;
    uint16 sensor_ver;

    uint16 isp_ver;
    uint16 firmware_ver;
    uint16 eeprom_data_ver;
    uint16 pcb_fpc_ver;
    uint16 opt_mech_ver;
    uint16 shading_data_ver;
    uint16 voice_coil_motor_id;
    uint16 module_assembling_id;
    uint8 register_map;
    uint8 sw_version[3];
    uint8 serial_num[16];
};

/*************************************************************/
/************************  LSC DATA  *************************/
/*************************************************************/
struct hat_otp_spot_sens_pix {
    uint32 r;        /* FORMAT OF THE DATA IS U8Q24*/
    uint32 gr;       /* FORMAT OF THE DATA IS U8Q24*/
    uint32 b;        /* FORMAT OF THE DATA IS U8Q24*/
    uint32 gb;       /* FORMAT OF THE DATA IS U8Q24*/
};

struct hat_otp_spot_sens {
    int count;
    struct hat_otp_spot_sens_pix *spot_sens;
};

struct hat_otp_lsc_center {
    uint16 x;
    uint16 y;
};

struct hat_otp_lsc_spot_sens_grid {
    uint16 color_temp; /* keeps color temp of currecnt lsc set*/
    struct hat_otp_lsc_center center_pax_r;
    struct hat_otp_lsc_center center_pax_gr;
    struct hat_otp_lsc_center center_pax_gb;
    struct hat_otp_lsc_center center_pax_b;
    struct hat_otp_spot_sens spot_sens;
};

struct hat_otp_lsc {
    uint16 count;       /* number of color temperature grids contained in data */
    hat_size_t size;
    struct hat_otp_lsc_spot_sens_grid *grids; /* ptr to grids*/
};

/*************************************************************/
/*******************  AWB DATA CENTRAL SPOT  *****************/
/*************************************************************/

struct hat_otp_center_spot_pix {
    uint16 color_temp;
    struct hat_otp_spot_sens_pix center;
};

struct hat_otp_center_spot {
    uint16 count;
    struct hat_otp_center_spot_pix spots[];
};

/*************************************************************/
/*******************  DEFECT PIXELS INFO  ********************/
/*************************************************************/
enum hat_otp_dpix_noise_type {
    HAT_OTP_DPIX_SINGLE_NOISE = 0,
    HAT_OTP_DPIX_DOUBLE_NOISES1,
    HAT_OTP_DPIX_DOUBLE_NOISES2,
    HAT_OTP_DPIX_USER_CUSTOM,
};

struct hat_otp_dpix_noise {
    enum hat_otp_dpix_noise_type noise;
    uint16 x_coord;
    uint16 y_coord;
};

struct hat_otp_dpc {
    uint8 count;                            /* the number of defect pixels */
    struct hat_otp_dpix_noise dpix_info[];  /* defect pixel coordinates */
};


/*************************************************************/
/********************  LENS DRIVER INFO  *********************/
/*************************************************************/
struct hat_otp_lens_pos {
    int32 typ;
    int32 min;
    int32 max;
};

struct hat_otp_lens_time {
    int8 l;                                 // Low
    int8 h;                                 // High
};

/**
* This structure contain hat_otp_lens_pos driver specific info->
*/
struct hat_otp_lens {
    int32 start_current;
    struct hat_otp_lens_pos inf;            // Infinity
    struct hat_otp_lens_pos mac;            // Macto
    struct hat_otp_lens_time ta;            // Time A pulse Low/High
    struct hat_otp_lens_time tb;            // Time B pulse Low/High
    uint8 period;
    uint32 pulse_time;
    uint32 pwm_per_um_fwd;
    uint32 pwm_per_um_bkwd;
};


/*************************************************************/
/*****************  LENS BLACK LEVEL INFO  *******************/
/*************************************************************/
/**
* This structure contain Black level compensation vs gain
*/
struct hat_otp_black_vs_gain_pix {
    uint16 r;
    uint16 gr;
    uint16 gb;
    uint16 b;
};

struct hat_otp_black_vs_gain {
    uint16 count;
    struct hat_otp_black_vs_gain_pix bl[];
};

#endif // __HAL_TYPES_OTP_H__

