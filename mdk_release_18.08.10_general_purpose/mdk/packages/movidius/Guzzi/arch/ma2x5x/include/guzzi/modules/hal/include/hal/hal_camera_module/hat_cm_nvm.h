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
* @file hat_cm_nvm.h
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

#ifndef __HAL_CM_NVM_H__
#define __HAL_CM_NVM_H__

#include <hal/hat_types.h>
#include <hal/hat_pix_fmt.h>
#include <hal/hal_camera_module/hat_cm_nvm_data_struct.h>
#include <hal/include/hal/hal_camera_module/hat_cm_socket.h>

#ifndef UNUSED
#define UNUSED(x) (void)x
#endif

#define ROUND_FLOAT_X_TO_INT(x) \
    ((x) >= 0 ? (long)((x) + 0.5) : (long)((x) - 0.5))

typedef struct {
    uint32 oper_count;
    const hat_cm_socket_command_entry_t* oper;
} hat_nvm_oper_entry_t;

typedef struct {
    hat_nvm_oper_entry_t pwr_off;
    hat_nvm_oper_entry_t pwr_on;
    hat_nvm_oper_entry_t init;
} hat_nvm_operations_t;

typedef enum {
    NVM_TYPE_DUMMY_ROM,
    NVM_TYPE_OTP_ROM,
    NVM_TYPE_EEPROM,
    NVM_TYPE_FROM,
} HAT_CM_NVM_TYPE_T;

//TODO: Discuss about NVM features structure
typedef struct {
    uint32 feat1;
    HAT_CM_NVM_TYPE_T nvm_type;
    uint32 nvm_size_bytes;
    // NVM operations
    const hat_nvm_operations_t operations;
} hat_nvm_features_t;



typedef enum {
    NVM_STATE_UNKNOWN,
    NVM_STATE_CREATED,
    NVM_STATE_OPENED,
    NVM_STATE_CLOSED,
} HAT_CM_NVM_STATE_T;

typedef enum {
    NVM_POWER_MODE_OFF,  // nvm is inactive
    NVM_POWER_MODE_ON    // nvm is active
} HAT_CM_NVM_POWER_MODE_T;


typedef enum {
    NVM_PWR_OFF,
    NVM_PWR_ON
} HAT_CM_NVM_PWR_T;

typedef struct {
    int         id;
    const char* name;
} hat_cm_nvm_cr_prms_t;

/* ---------------------------- NVM controls ----------------------------- */
typedef enum {
    NVM_CFG_DUMMY,
} HAT_CM_NVM_CONFIG_OPER_T;

typedef struct {
    HAT_CM_NVM_CONFIG_OPER_T cfg_type;
    union {
    } cfg;
} hat_cm_nvm_config_t;

typedef enum {
    NVM_CTRL_DUMMY,
    NVM_CTRL_POWER,
    NVM_CTRL_INIT,
    NVM_CTRL_WRITE,
    NVM_CTRL_READ,
} HAT_CM_NVM_CONTROL_OPER_T;

typedef struct {
    HAT_CM_NVM_CONTROL_OPER_T ctrl_type;
    union {
        HAT_CM_NVM_PWR_T power;
        void*            init;
        struct {
            int start;
            int size;
            void *buf;
        } io;
    } ctrl;
} hat_cm_nvm_control_t;

typedef enum {
    NVM_PROCESS_DUMMY,
} HAT_CM_NVM_PROCESS_OPER_T;

typedef struct {
    HAT_CM_NVM_PROCESS_OPER_T process_type;
    union {
    } process;
} hat_cm_nvm_process_t;

typedef enum {
    NVM_QUERY_DUMMY,
    NVM_QUERY_GET_FEATURES,
    NVM_QUERY_GET_NVM_DATA,
} HAT_CM_NVM_QUERY_OPER_T;

typedef struct {
    HAT_CM_NVM_QUERY_OPER_T type;
    union {
        hat_nvm_features_t **features;
        nvm_info_t *nvm_info;
    };
} hat_cm_nvm_query_t;



typedef struct hat_cm_nvm_obj hat_cm_nvm_obj_t;

typedef hat_cm_nvm_obj_t* hat_cm_nvm_handle_t;

struct hat_cm_nvm_obj {
    /* Private properties */
    void *prvt;

    /* Private methods */
    int (*set_power_mode) (hat_cm_nvm_handle_t hndl,
                           HAT_CM_NVM_POWER_MODE_T mode);

    int (*init)           (hat_cm_nvm_handle_t hndl);

    int (*read)           (hat_cm_nvm_handle_t hndl,
                           void* buff,
                           uint32 start,
                           uint32 size);

    int (*write)          (hat_cm_nvm_handle_t hndl,
                           void* buff,
                           uint32 start,
                           uint32 size);

    int (*get_features)   (hat_cm_nvm_handle_t hndl,
                           hat_nvm_features_t** features);

    int (*get_power_mode) (hat_cm_nvm_handle_t hndl,
                           HAT_CM_NVM_POWER_MODE_T *mode);

    int (*get_state)      (hat_cm_nvm_handle_t hndl,
                           HAT_CM_NVM_STATE_T *state);

    /* Public methods visible by the CM driver */
    int (*open)    (hat_cm_nvm_handle_t hndl,
                   void *sock_hndl,
                   void *ph_nvm_hndl,
                   void* nvm_features);
    int (*config)  (hat_cm_nvm_handle_t hndl, void* cfg_prms);
    int (*control) (hat_cm_nvm_handle_t hndl, void* ctrl_prms);
    int (*process) (hat_cm_nvm_handle_t hndl, void* process_prms);
    int (*query)   (hat_cm_nvm_handle_t hndl, void* query_prms);
    int (*close)   (hat_cm_nvm_handle_t hndl);
    int (*destroy) (hat_cm_nvm_handle_t hndl);
};

#endif // __HAL_CM_NVM_H__
