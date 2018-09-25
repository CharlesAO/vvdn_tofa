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
* @file hat_cm_lights.h
*
* @author ( MM Solutions AD )
*
*/
/* -----------------------------------------------------------------------------
*!
*! Revision History
*! ===================================
*! Jun 10, 2014 : Author aiovtchev
*! Created
* =========================================================================== */



#ifndef HAT_CM_LIGHTS_H_
#define HAT_CM_LIGHTS_H_

#include <hal/hat_light.h>
#include "cam_algo_state.h"

typedef struct hat_cm_lights_obj hat_cm_lights_obj_t;

typedef hat_cm_lights_obj_t* hat_cm_lights_handle_t;

typedef int (*hat_cm_lights_cb_t)(void* user_data);

typedef enum {
    LIGHTS_STATE_UNKNOWN,
    LIGHTS_STATE_CREATED,
    LIGHTS_STATE_OPENED,
    LIGHTS_STATE_CLOSED,
} HAT_CM_LIGHTS_STATE_T;

typedef enum {
    LIGHTS_DYN_MODE_OFF,          // Do not fire the flash for this capture
    LIGHTS_DYN_MODE_SINGLE,       // available is true Fire flash for this capture based on firingPower, firingTime.
    LIGHTS_DYN_MODE_TOURCH,       // available is true Flash continuously on, power set by firingPower
} HAT_CM_LIGHTS_DYN_MODE_T;

typedef enum {
    LIGHTS_DYN_STATE_UNAVAILABLE, // No flash on camera
    LIGHTS_DYN_STATE_CHARGING,    // available is true Flash is charging and cannot be fired
    LIGHTS_DYN_STATE_READY,       // available is true Flash is ready to fire
    LIGHTS_DYN_STATE_FIRED,       // available is true Flash fired for this capture
} HAT_CM_LIGHTS_DYN_STATE_T;

typedef struct {
    int8                       firing_power;
    int64                      firing_time_ns;
    HAT_CM_LIGHTS_DYN_MODE_T   mode;
    HAT_CM_LIGHTS_DYN_STATE_T  state;
} hat_cm_flash_dyn_props_t;

typedef enum {
    LIGHTS_POWER_MODE_OFF,
    LIGHTS_POWER_MODE_ON
} HAT_CM_LIGHTS_POWER_MODE_T;

typedef struct {
    int         id;
    const char *name;
} hat_cm_lights_cr_prms_t;

/* ----------------------------- Light controls ----------------------------- */
typedef enum {
    LIGHT_CFG_DUMMY,
} HAT_CM_LIGHT_CONFIG_OPER_T;

typedef struct {
    HAT_CM_LIGHT_CONFIG_OPER_T cfg_type;
    union {
    } cfg;
} hat_cm_light_config_t;

typedef enum {
    LIGHT_CTRL_DUMMY,
    LIGHT_CTRL_POWER_CMD,
} HAT_CM_LIGHT_CONTROL_OPER_T;

typedef struct {
    HAT_CM_LIGHT_CONTROL_OPER_T ctrl_type;
    union {
        hat_light_ctrl_list_t    *pwr_cmd;
    } ctrl;
} hat_cm_light_control_t;

typedef enum {
    LIGHT_PROCESS_DUMMY,
} HAT_CM_LIGHT_PROCESS_OPER_T;

typedef struct {
    HAT_CM_LIGHT_PROCESS_OPER_T process_type;
    union {
    } process;
} hat_cm_light_process_t;

typedef enum {
    LIGHT_QUERY_DUMMY,
    LIGHT_QUERY_GET_DYN_PROPS,
    LIGHT_QUERY_GET_FEATURES,
} HAT_CM_LIGHT_QUERY_OPER_T;

typedef struct  {
    uint32 oper_count;
    const hat_cm_socket_command_entry_t* oper;
} hat_lights_oper_entry_t;


typedef struct  {
    hat_lights_oper_entry_t pwr_seq_on;
    hat_lights_oper_entry_t pwr_seq_off;
    hat_lights_oper_entry_t init_seq;
    hat_lights_oper_entry_t deinit_seq;
} hat_lights_operations_t;

typedef struct  {
    hat_light_spec_features_t      spec_features;
    uint32                         num_light_intensities;   // number of valid entries in the list
    const hat_light_entry_t       *entries;
    const hat_lights_operations_t  operations;
}hat_light_el_features_t;

typedef struct  {
    hat_light_el_features_t flash;
    hat_light_el_features_t torch;
    hat_light_el_features_t video;
    hat_light_el_features_t af_assist;
    hat_light_el_features_t privacy;
    hat_light_el_features_t red_eye; // Blink period is HW (driver) specific
}hat_lights_features_t;

typedef struct {
    HAT_CM_LIGHT_QUERY_OPER_T query_type;
    union {
        hat_cm_flash_dyn_props_t*   dyn_props;
        hat_flash_bounds_t**        features;
    } query;
} hat_cm_light_query_t;



struct hat_cm_lights_obj
{
    void *prvt;

    int (*power_cmd)        (hat_cm_lights_handle_t hndl,
                                hat_light_ctrl_list_t *ctrl);

    int (*get_state)       (hat_cm_lights_handle_t hndl,
                            HAT_CM_LIGHTS_STATE_T *state);

    int (*get_dyn_props)   (hat_cm_lights_handle_t hndl,
                            hat_cm_flash_dyn_props_t* dyn_props);

    int (*get_features)    (hat_cm_lights_handle_t hndl,
                            hat_flash_bounds_t** features);

    int (*open)            (hat_cm_lights_handle_t hndl,
                            void *sock_hndl, void *ph_light_hndl, void* sens_features);

    int (*config)          (hat_cm_lights_handle_t hndl, void* cfg_prms);

    int (*control)         (hat_cm_lights_handle_t hndl, void* ctrl_prms);

    int (*process)         (hat_cm_lights_handle_t hndl, void* process_prms);

    int (*query)           (hat_cm_lights_handle_t hndl, void* query_prms);

    int (*close)           (hat_cm_lights_handle_t hndl);

    int (*destroy)         (hat_cm_lights_handle_t hndl);

};

int hai_cm_lights_create(hat_cm_lights_handle_t   hndl,
                            hat_cm_lights_cr_prms_t *cr_prms);

#endif /* HAT_CM_LIGHTS_H_ */
