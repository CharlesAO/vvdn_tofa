
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
* @file
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

#ifndef _RPC_GUZZI_NVM_READER_H
#define _RPC_GUZZI_NVM_READER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <guzzi/rpc/rpc.h>

#define RPC_GUZZI_NVM_READER_FUNCTION_ID(N) \
    (rpc_guzzi_nvm_reader_function_id_base + (N))

#define RPC_GUZZI_NVM_READER_GET_NUMBER_OF_CAMERAS  RPC_GUZZI_NVM_READER_FUNCTION_ID(1)
#define RPC_GUZZI_NVM_READER_READ                   RPC_GUZZI_NVM_READER_FUNCTION_ID(2)
#define RPC_GUZZI_NVM_READER_DESTROY                RPC_GUZZI_NVM_READER_FUNCTION_ID(3)
#define RPC_GUZZI_NVM_READER_CREATE                 RPC_GUZZI_NVM_READER_FUNCTION_ID(4)

#define RPC_GUZZI_NVM_READER_INSTANCES_MAX 32
#define RPC_GUZZI_NVM_READER_INSTANCE_INVALID 0xBAD01234
#define RPC_GUZZI_NVM_READER_BUFFER_SIZE_MAX (4*1024)

extern const rpc_uint32_t rpc_guzzi_nvm_reader_function_id_base;

/*
 * ****************************************************************************
 * ** RPC variant of NVM Parsed struct (keep in sync with nvm_info_t) *********
 * ****************************************************************************
 */
/* NVM Parsed */
typedef struct {
    rpc_uint8_t sensor_specific[64];
    struct {
        rpc_uint32_t manufacturer_id;
        rpc_uint16_t sensor_id;
        rpc_uint16_t camera_module_ver;
        rpc_uint16_t voice_coil_motor_id;
        rpc_uint16_t module_assembling_id;
        rpc_uint16_t sensor_ver;
        rpc_uint16_t isp_ver;
        rpc_uint16_t firmware_ver;
        rpc_uint16_t nvm_data_ver;
        rpc_uint16_t pcb_fpc_ver;
        rpc_uint16_t opt_mech_ver;
        rpc_uint16_t shading_data_ver;
        rpc_uint8_t register_map;
        rpc_uint8_t sw_version[3];
        rpc_uint8_t serial_num[16];
    } version;
    struct {
        rpc_uint32_t spots_uarr_size;
    } center;
    struct {
        rpc_uint16_t xsz;
        rpc_uint16_t ysz;
        rpc_uint32_t grids_uarr_size;
    } lsc;
    struct {
        rpc_int32_t start_current;
        rpc_int32_t inf_pos_typ;
        rpc_int32_t mac_pos_typ;
        rpc_int32_t inf_pos_min;
        rpc_int32_t inf_pos_max;
        rpc_int32_t mac_pos_min;
        rpc_int32_t mac_pos_max;
        rpc_uint8_t tal;
        rpc_uint8_t tah;
        rpc_uint8_t tbl;
        rpc_uint8_t tbh;
        rpc_uint8_t period;
        rpc_uint32_t pulse_time;
        rpc_uint32_t pwm_per_um_fwd;
        rpc_uint32_t pwm_per_um_bkwd;
    } positions;
    struct {
        rpc_uint16_t r;
        rpc_uint16_t gr;
        rpc_uint16_t gb;
        rpc_uint16_t b;
    } bl_vs_gain[8];
    rpc_uint16_t pedestal;
    struct {
        rpc_uint32_t dpix_info_uarr_size;
    } dpc;
    rpc_uint8_t var_size_data[0];
/*
 * var_size_data contains (use helper functions to access):
 *   rpc_nvm_center_spot_t spots[];
 *   rpc_nvm_lsc_spot_sens_grid_t grids[];
 *   rpc_nvm_spot_sens_t pdata[];
 *   rpc_nvm_dpix_info_t dpix_info[];
 */
} rpc_nvm_info_t;

typedef struct {
    rpc_uint16_t r;
    rpc_uint16_t gr;
    rpc_uint16_t b;
    rpc_uint16_t gb;
} rpc_nvm_spot_sens_t;

typedef struct {
    rpc_uint16_t valid;
    rpc_uint16_t x;
    rpc_uint16_t y;
} rpc_nvm_lsc_center_data_t;

typedef struct {
    rpc_uint16_t color_temp;
    rpc_nvm_spot_sens_t center;
} rpc_nvm_center_spot_t;

typedef struct {
    rpc_uint16_t color_temp;
    rpc_nvm_lsc_center_data_t center_pax_r;
    rpc_nvm_lsc_center_data_t center_pax_gr;
    rpc_nvm_lsc_center_data_t center_pax_b;
    rpc_nvm_lsc_center_data_t center_pax_gb;
} rpc_nvm_lsc_spot_sens_grid_t;

typedef struct {
    rpc_uint32_t noise;
    rpc_uint16_t x_coord;
    rpc_uint16_t y_coord;
} rpc_nvm_dpix_info_t;

/*
 * ****************************************************************************
 * ** RPC functions structs ***************************************************
 * ****************************************************************************
 */
/* Number of cameras */
typedef struct {
    rpc_uint32_t instance;
} rpc_guzzi_nvm_reader_get_number_of_cameras_t;

typedef struct {
    rpc_uint32_t instance;
    rpc_int32_t number_of_cameras;
} rpc_guzzi_nvm_reader_get_number_of_cameras_return_t;

/* Read */
typedef struct {
    rpc_uint32_t instance;
    rpc_int32_t camera_id;
} rpc_guzzi_nvm_reader_read_t;

typedef struct {
    rpc_uint32_t instance;
    rpc_int32_t camera_id;
    rpc_uint8_t data[0];
} rpc_guzzi_nvm_reader_read_return_t;

/* Destroy */
typedef struct {
    rpc_uint32_t instance;
} rpc_guzzi_nvm_reader_destroy_t;

typedef struct {
    rpc_uint32_t instance;
} rpc_guzzi_nvm_reader_destroy_return_t;

/* Create */
typedef struct {
} rpc_guzzi_nvm_reader_create_t;

typedef struct {
    rpc_uint32_t instance;
} rpc_guzzi_nvm_reader_create_return_t;

#ifdef __cplusplus
}
#endif

#endif /* _RPC_GUZZI_NVM_READER_H */

