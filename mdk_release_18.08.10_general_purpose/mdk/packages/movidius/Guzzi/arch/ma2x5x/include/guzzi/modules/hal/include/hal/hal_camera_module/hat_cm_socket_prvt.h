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
* @file hat_cm_socket_prvt.h
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

#ifndef __HAT_CM_SOCKET_PRVT_H__
#define __HAT_CM_SOCKET_PRVT_H__

#include <platform/inc/plat_socket_pub.h>

/* --- HW resource description ---------------------------------------------- */
typedef hat_cm_socket_dummy_hndl_t* hat_cm_socket_hw_rsrc_dummy_hndl_t;
typedef hat_cm_socket_gpio_dev_t*  hat_cm_socket_hw_rsrc_gpio_hndl_t;
typedef hat_cm_socket_i2c_dev_t*   hat_cm_socket_hw_rsrc_i2c_hndl_t;
typedef hat_cm_socket_clock_hndl_t* hat_cm_socket_hw_rsrc_clock_hndl_t;
typedef hat_cm_socket_csi2_hndl_t*  hat_cm_socket_hw_rsrc_csi2_hndl_t;
typedef hat_cm_socket_power_hndl_t*  hat_cm_socket_hw_rsrc_power_hndl_t;

typedef union {
    hat_cm_socket_hw_rsrc_dummy_hndl_t dummy_hndl;
    hat_cm_socket_hw_rsrc_gpio_hndl_t  gpio_hndl;
    hat_cm_socket_hw_rsrc_i2c_hndl_t   i2c_hndl;
    hat_cm_socket_hw_rsrc_clock_hndl_t clock_hndl;
    hat_cm_socket_hw_rsrc_csi2_hndl_t  csi2_hndl;
    hat_cm_socket_hw_rsrc_power_hndl_t power_hndl;
} hat_cm_socket_hw_rsrc_desc_t;

typedef struct {
    PLAT_SOCKET_HW_RSRC_TYPE_T   hw_type;
    hat_cm_socket_hw_rsrc_desc_t hw_desc;
} hat_cm_socket_hw_rsrc_t;
/* -------------------------------------------------------------------------- */


/* --- DUMMY resource description ------------------------------------------- */
typedef union {
    hat_cm_socket_hw_rsrc_t dummy_hw_rsrc;
    hat_cm_socket_hw_rsrc_t power_hw_rsrc;
} hat_cm_socket_dumm_rsrc_desc_t;

typedef struct {
    PLAT_SOCKET_DUMM_RSRC_TYPE_T   type;
    hat_cm_socket_dumm_rsrc_desc_t rsrc;
} hat_cm_socket_dummy_rsrc_t;
/* -------------------------------------------------------------------------- */


/* --- SENSOR resource description ------------------------------------------ */
typedef union {
    /* Activate/deactivate type resources */
    hat_cm_socket_hw_rsrc_t dummy_hw_rsrc;
    hat_cm_socket_hw_rsrc_t power1_hw_rsrc;
    hat_cm_socket_hw_rsrc_t power2_hw_rsrc;
    hat_cm_socket_hw_rsrc_t power3_hw_rsrc;
    hat_cm_socket_hw_rsrc_t clock_hw_rsrc;
    hat_cm_socket_hw_rsrc_t enable_hw_rsrc;
    hat_cm_socket_hw_rsrc_t reset_hw_rsrc;
    hat_cm_socket_hw_rsrc_t xshtdwn_hw_rsrc;
    hat_cm_socket_hw_rsrc_t sync_hw_rsrc;

    /* CCI type resources */
    hat_cm_socket_hw_rsrc_t cci_hw_rsrc;

    /* Image Interface type resources */
    hat_cm_socket_hw_rsrc_t imgif_hw_rsrc;
} hat_cm_socket_sens_rsrc_desc_t;

typedef struct {
    PLAT_SOCKET_SENS_RSRC_TYPE_T   type;
    hat_cm_socket_sens_rsrc_desc_t rsrc;
} hat_cm_socket_sensor_rsrc_t;
/* -------------------------------------------------------------------------- */


/* --- LENS resource description -------------------------------------------- */
typedef union {
    /* Activate/deactivate type resources */
    hat_cm_socket_hw_rsrc_t dummy_hw_rsrc;
    hat_cm_socket_hw_rsrc_t power_hw_rsrc;
    hat_cm_socket_hw_rsrc_t clock_hw_rsrc;
    hat_cm_socket_hw_rsrc_t enable_hw_rsrc;
    /* CCI type resources */
    hat_cm_socket_hw_rsrc_t cci_hw_rsrc;
} hat_cm_socket_lens_rsrc_desc_t;

typedef struct {
    PLAT_SOCKET_LENS_RSRC_TYPE_T   type;
    hat_cm_socket_lens_rsrc_desc_t rsrc;
} hat_cm_socket_lens_rsrc_t;
/* -------------------------------------------------------------------------- */


/* --- FLASH resource description ------------------------------------------- */
typedef union {
    /* Activate/deactivate type resources */
    hat_cm_socket_hw_rsrc_t dummy_hw_rsrc;
    hat_cm_socket_hw_rsrc_t power_hw_rsrc;
    hat_cm_socket_hw_rsrc_t clock_hw_rsrc;
    hat_cm_socket_hw_rsrc_t torch_hw_rsrc;
    hat_cm_socket_hw_rsrc_t strobe_hw_rsrc;
    /* CCI type resources */
    hat_cm_socket_hw_rsrc_t cci_hw_rsrc;
} hat_cm_socket_flash_rsrc_desc_t;

typedef struct {
    PLAT_SOCKET_FLASH_RSRC_TYPE_T   type;
    hat_cm_socket_flash_rsrc_desc_t rsrc;
} hat_cm_socket_flash_rsrc_t;
/* -------------------------------------------------------------------------- */


/* --- NVM resource description --------------------------------------------- */
typedef union {
    /* Activate/deactivate type resources */
    hat_cm_socket_hw_rsrc_t dummy_hw_rsrc;
    hat_cm_socket_hw_rsrc_t power_hw_rsrc;
    hat_cm_socket_hw_rsrc_t clock_hw_rsrc;
    hat_cm_socket_hw_rsrc_t enable_hw_rsrc;
    /* CCI type resources */
    hat_cm_socket_hw_rsrc_t cci_hw_rsrc;
} hat_cm_socket_nvm_rsrc_desc_t;

typedef struct {
    PLAT_SOCKET_NVM_RSRC_TYPE_T   type;
    hat_cm_socket_nvm_rsrc_desc_t rsrc;
} hat_cm_socket_nvm_rsrc_t;
/* -------------------------------------------------------------------------- */


/* SOCKET resource description ---------------------------------------------- */
typedef union {
    hat_cm_socket_dummy_rsrc_t  dummy_resources[DUMM_RSRC_MAX];
    hat_cm_socket_sensor_rsrc_t sensor_resources[SEN_RSRC_MAX];
    hat_cm_socket_lens_rsrc_t   lens_resources[LENS_RSRC_MAX];
    hat_cm_socket_flash_rsrc_t  flash_resources[FLASH_RSRC_MAX];
    hat_cm_socket_nvm_rsrc_t    nvm_resources[NVM_RSRC_MAX];
} hat_cm_socket_rsrc_desc_t;

typedef struct  {
    PLAT_SOCKET_COMPONENT_T   comp;
    hat_cm_socket_rsrc_desc_t rsrc;
} hat_cm_socket_rsrc_t;
/* -------------------------------------------------------------------------- */


typedef struct {
    HAT_CM_SOCKET_STATE_T   state;
    hat_cm_socket_rsrc_t    socket_resources[SOCKET_COMP_MAX];
} hat_cm_socket_prms_t;


typedef struct {
    hat_cm_socket_cr_prms_t cr_prms;
    hat_cm_socket_prms_t    prms;
} hat_cm_socket_prvt_data_t;

#endif // __HAT_CM_SOCKET_PRVT_H__
