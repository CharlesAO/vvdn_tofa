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
* @file plat_hw_resource.h
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

#ifndef __PLATFORM_HW_RESOURCE_H__
#define __PLATFORM_HW_RESOURCE_H__

#include <hal/hat_types.h>
#include <hal/include/hal/hal_camera_module/hat_cm_hw_resource.h>

#define container_of(PTR,TYPE,FIELD) \
    (TYPE *)((char *)(PTR) - (char *)&(((TYPE *)0)->FIELD))

/* ########################################### CLOCK #################################################### */
typedef struct {
    char    *name;
    uint32  id;
    uint32  freq; // Frequency Hz
} plat_res_desc_clock_t;

typedef int (*plat_clock_init_t)(plat_res_desc_clock_t *prms, hat_cm_socket_clock_hndl_t *hndl);

typedef struct {
    plat_res_desc_clock_t       init_data;
    plat_clock_init_t           init_function;
    hat_cm_socket_clock_hndl_t  hndl;
} plat_clock_t;

/* ########################################### POWER #################################################### */
typedef struct {
    uint32                  power_number;
    uint32                  default_value;          // in mV
    uint32                  on_value;               // in mV
    uint32                  off_value;              // in mV
} plat_power_dev_params_t;

typedef int (*plat_power_dev_init_t)(plat_power_dev_params_t *params, hat_cm_socket_power_hndl_t *hndl);

typedef int (*plat_power_dev_deinit_t)(hat_cm_socket_power_hndl_t* hndl);

typedef struct {
    plat_power_dev_params_t     params;
    plat_power_dev_init_t       init_function;
    plat_power_dev_deinit_t     deinit_func;
    hat_cm_socket_power_hndl_t  hndl;
} plat_power_dev_desc_t;

/* ########################################### CSI #################################################### */
typedef struct {
    const char *name;
    const int  ctx_num;
} plat_res_desc_csi2_t;

typedef int (*plat_csi2_init_t)(plat_res_desc_csi2_t *prms, hat_cm_socket_csi2_hndl_t *hndl);

typedef struct {
    plat_res_desc_csi2_t        init_data;
    plat_csi2_init_t            init_function;
    hat_cm_socket_csi2_hndl_t   hndl;
} plat_csi2_t;

/* ########################################### DUMMY #################################################### */
typedef struct {
    char    *name;
    uint32   id;
} plat_res_desc_dummy_t;

typedef int (*plat_dummy_init_t)(plat_res_desc_dummy_t *prms, hat_cm_socket_dummy_hndl_t *hndl);

typedef struct {
    plat_res_desc_dummy_t       init_data;
    plat_dummy_init_t           init_function;
    hat_cm_socket_dummy_hndl_t  hndl;
} plat_dummy_t;

/* ########################################### GPIO #################################################### */
typedef enum {
    PLAT_GPIO_IN = 1,
    PLAT_GPIO_OUT,
    PLAT_GPIO_OUT_PULL_UP,
    PLAT_GPIO_OUT_PULL_DOWN,
    PLAT_GPIO_OUT_OPEN,
    PLAT_GPIO_OUT_CLOCK,
/* should be last */
    PLAT_GPIO_DIR_MAX
} plat_gpio_direction_t;

typedef struct {
    uint32                  gpio_number;
    plat_gpio_direction_t   direction;
    uint32                  default_value;
    uint32                  on_value;
    uint32                  off_value;
    uint32                  debounce;   // if GPIO is input debounce time. If GPIO is clock - represents speed in Hz
} plat_res_desc_gpio_t;

/*
 * ****************************************************************************
 * ** GPIO Device **************************************************************
 * ****************************************************************************
 */
typedef struct {
    uint32                  gpio_number;
    plat_gpio_direction_t   direction;
    uint32                  default_value;
    uint32                  on_value;
    uint32                  off_value;
    uint32                  debounce;   // if GPIO is input debounce time. If GPIO is clock - represents speed in Hz
} plat_gpio_dev_params_t;

typedef hat_cm_socket_gpio_dev_t * plat_gpio_dev_init_t(plat_gpio_dev_params_t *params);
typedef int plat_gpio_dev_deinit_t(hat_cm_socket_gpio_dev_t *plat_dev);

typedef struct {
    plat_gpio_dev_params_t params;
    plat_gpio_dev_init_t *init;
    plat_gpio_dev_deinit_t *deinit;
    hat_cm_socket_gpio_dev_t *instance;
} plat_gpio_dev_desc_t;

//typedef int (*plat_gpio_init_t)(plat_res_desc_gpio_t *prms, hat_cm_socket_gpio_dev_t *hndl);
//
//typedef struct {
//    plat_res_desc_gpio_t        init_data;
//    plat_gpio_init_t            init_function;
//    hat_cm_socket_gpio_dev_t   hndl;
//} plat_gpio_t;

/*
 * ****************************************************************************
 * ** I2C Bus *****************************************************************
 * ****************************************************************************
 */

typedef struct {
    unsigned int channel;
    unsigned int speed;
} plat_i2c_bus_dev_params_t;

typedef hat_cm_socket_i2c_bus_dev_t * plat_i2c_bus_dev_init_t(plat_i2c_bus_dev_params_t *params);

typedef struct {
    plat_i2c_bus_dev_params_t params;
    plat_i2c_bus_dev_init_t *init;
    hat_cm_socket_i2c_bus_dev_t *instance;
} plat_i2c_bus_dev_desc_t;
/*
 * ****************************************************************************
 * ** I2C Device **************************************************************
 * ****************************************************************************
 */
typedef struct {
    plat_i2c_bus_dev_desc_t *bus_desc;
    unsigned int slave_addr;
    unsigned int slave_addr_bit_size;
    unsigned int reg_addr_size; /* In number of bytes */
} plat_i2c_dev_params_t;

typedef hat_cm_socket_i2c_dev_t * plat_i2c_dev_init_t(plat_i2c_dev_params_t *params);
typedef int plat_i2c_dev_deinit_t(hat_cm_socket_i2c_dev_t *plat_dev);

typedef struct {
    plat_i2c_dev_params_t    params;
    plat_i2c_dev_init_t     *init;
    plat_i2c_dev_deinit_t   *deinit;
    hat_cm_socket_i2c_dev_t *instance;
} plat_i2c_dev_desc_t;

#endif // __PLATFORM_HW_RESOURCE_H__

