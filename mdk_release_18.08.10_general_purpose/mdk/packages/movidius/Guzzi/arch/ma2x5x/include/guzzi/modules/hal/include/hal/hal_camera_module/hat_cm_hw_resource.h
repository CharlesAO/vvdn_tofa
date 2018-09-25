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
* @file hat_cm_hw_resource.h
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

#ifndef __HAT_HW_RESOURCE_H__
#define __HAT_HW_RESOURCE_H__

#include <hal/hat_types.h>

typedef enum {
    HAT_HW_RES_OFF,	// de-activate
    HAT_HW_RES_ON	// activate
} HAT_HW_RESOURCE_ACTION_T;


typedef struct {
    uint32 reg;
    uint32 num;
    uint8 *val;
} hat_cm_socket_i2c_cmd_t;

typedef struct {
    int a;
    int b;
} hat_cm_socket_csi2_cfg_t;

typedef struct {
    int c;
    int d;
} hat_cm_socket_csi2_ctx_cfg_t;

typedef struct {
    uint32  address;
    uint32  register_size;
    uint32  slave_addr_bit_size;
} hat_socket_comp_cfg_t;

typedef struct {
    hat_socket_comp_cfg_t   sensor;
    hat_socket_comp_cfg_t   lens;
    hat_socket_comp_cfg_t   light;
    hat_socket_comp_cfg_t   nvm;
} hat_cm_socket_open_params_t;

typedef enum {
    HAT_HW_QUERY_CAMIF_CTX,
    HAT_HW_QUERY_SENSOR_CLK,
} hat_cm_socket_query_params_t;


/* --- HW resource handles description -------------------------------------- */
typedef struct {
    int (*dummy_set)(HAT_HW_RESOURCE_ACTION_T action);
} hat_cm_socket_dummy_hndl_t;

typedef struct hat_cm_socket_power_dev_t hat_cm_socket_power_hndl_t;
struct hat_cm_socket_power_dev_t{
    int (*power_set)(hat_cm_socket_power_hndl_t *hndl, HAT_HW_RESOURCE_ACTION_T action);
    uint32 cnt;
};

typedef struct hat_cm_socket_gpio_dev hat_cm_socket_gpio_dev_t;
struct hat_cm_socket_gpio_dev {
    int (*gpio_set)(hat_cm_socket_gpio_dev_t *plat_dev, HAT_HW_RESOURCE_ACTION_T action);
    uint32 cnt;
};

typedef struct hat_cm_socket_clock_dev_t hat_cm_socket_clock_hndl_t;
struct hat_cm_socket_clock_dev_t{
    int (*clock_set)(hat_cm_socket_clock_hndl_t *hndl, HAT_HW_RESOURCE_ACTION_T action);
    uint32 freq;
    uint32 cnt;
};

typedef struct hat_cm_socket_i2c_bus_dev hat_cm_socket_i2c_bus_dev_t;

struct hat_cm_socket_i2c_bus_dev {
    int (*regbus)(hat_cm_socket_i2c_bus_dev_t *plat_dev, hat_socket_comp_cfg_t *params_dev);
    int (*write)(hat_cm_socket_i2c_bus_dev_t *plat_dev, uint32 reg, uint32 num, uint8 *val);
    int (*read)(hat_cm_socket_i2c_bus_dev_t *plat_dev, uint32 reg, uint32 num, uint8 *val);
};

typedef struct hat_cm_socket_i2c_dev hat_cm_socket_i2c_dev_t;

struct hat_cm_socket_i2c_dev {
    int (*regdev)(hat_cm_socket_i2c_dev_t *plat_dev, hat_socket_comp_cfg_t *params_dev);
    int (*write)(hat_cm_socket_i2c_dev_t *plat_dev, uint32 reg, uint32 num, uint8 *val);
    int (*read)(hat_cm_socket_i2c_dev_t *plat_dev, uint32 reg, uint32 num, uint8 *val);
};

typedef struct {
    int  ctx_num;
    int (*csi_cfg)(hat_cm_socket_csi2_cfg_t* cfg);
    int (*csi_ctx_cfg)(hat_cm_socket_csi2_ctx_cfg_t* ctx_cfg);
    int (*csi_ctx_start)(void);
    int (*csi_ctx_stop)(void);
    int (*csi_ctx_imm_stop)(void);
    int (*csi_ctx_set_buffer)(void* buff);
    int (*csi_ctx_enable_event)(void);
    int (*csi_ctx_disable_event)(void);
} hat_cm_socket_csi2_hndl_t;
/* -------------------------------------------------------------------------- */

#endif // __HAT_HW_RESOURCE_H__
