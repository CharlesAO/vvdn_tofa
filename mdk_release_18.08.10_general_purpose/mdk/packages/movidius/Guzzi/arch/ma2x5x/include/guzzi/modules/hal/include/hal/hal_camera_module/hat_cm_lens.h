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
* @file hat_cm_lens.h
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

#ifndef __HAL_CM_LENS_H__
#define __HAL_CM_LENS_H__

#include <osal/osal_mutex.h>
#include <hal/hat_types.h>
#include <hal/include/hal/hal_camera_module/hat_cm_socket.h>

#define ROUND_FLOAT_X_TO_INT(x) \
    ((x) >= 0 ? (long)((x) + 0.5) : (long)((x) - 0.5))

#define HAT_CM_LENS_MAX_STEP_COUNT (128)
#define HAT_CM_LENS_MAX_STEP_MASK (HAT_CM_LENS_MAX_STEP_COUNT-1)

typedef enum {
    FACING_FRONT,
    FACING_BACK

} facing_t;

typedef struct {
    uint32 oper_count;
    const hat_cm_socket_command_entry_t* oper;
} hat_lens_oper_entry_t;

typedef struct {
    hat_lens_oper_entry_t pwr_off;
    hat_lens_oper_entry_t pwr_on;
    hat_lens_oper_entry_t init;
    hat_lens_oper_entry_t deactivate;
    hat_lens_oper_entry_t activate;
} hat_lens_operations_t;

//TODO: Discuss about lens features structure
typedef struct {
    uint16  shutter;
    uint16  blend;
    uint16  zoom;
    uint16  measure_pos;
    uint16  measure_time_us;
    float  inf_pos;  //infinity position[ 0.0 - 1.0]
    float  mac_pos;  //macro position[ 0.0 - 1.0]
    float  dist_tol; //Optimal distance tolerance [diopter].
    int32   *offset;
    void    *eeprom_version;
    // Lens operations
    const hat_lens_operations_t operations;
    // HAL 3.0 Specific
    uint16  available_apertures_size;
    const float  *available_apertures;          // List of supported aperture values.
    uint16  available_filt_densities_size;
    const float  *available_filt_densities;     // List of supported ND filter values
    uint16  available_focal_lengths_size;
    const float  *available_focal_lengths;      // If fitted with optical zoom, what focal
                                          // lengths are available. If not, the static focal length
    uint16  available_optical_st_size;
    const uint8   *available_optical_st;         // List of supported optical image stabilization modes.
    const float  *geom_correction_map;    // A low-resolution map for correction of geometric
                                          // distortions and chromatic aberrations, per color channel.
    int32  geom_correction_map_size[2];   // Dimensions of geometric correction map
    int32  hyperfocal_distance;           // Hyperfocal distance for this lens; set to 0 if fixed focus [diopters].
    float minimum_focus_distance;        // Shortest distance from front most surface of the lens that can be focused correctly [diopters].
    int32  shading_map_size[2];           // Dimensions of lens shading map.
    facing_t    facing;                   // Direction the camera faces relative to device screen;
    float  optical_axis_angle[2];        // Relative angle of camera optical axis to the perpendicular axis from the display
    float  position[3];                  // Coordinates of camera optical axis on device in [mm].
} hat_lens_features_t;



typedef struct {
    float   lens_pos;
    uint32  time_sleep_us;
} hat_cm_lens_micro_step_t;

typedef struct {
    osal_mutex *lock;
    volatile uint32 head;
    volatile uint32 tail;
    hat_cm_lens_micro_step_t   steps[HAT_CM_LENS_MAX_STEP_COUNT];
} hat_cm_lens_move_list_t;

static inline hat_cm_lens_micro_step_t* hat_cm_lens_move_list_get_head(hat_cm_lens_move_list_t *p_list)
{
    return &p_list->steps[p_list->head&HAT_CM_LENS_MAX_STEP_MASK];
}
static inline void hat_cm_lens_move_list_add_head(hat_cm_lens_move_list_t *p_list)
{
    p_list->head++;
}
static inline hat_cm_lens_micro_step_t* hat_cm_lens_move_list_get_tail(hat_cm_lens_move_list_t *p_list)
{
    return &p_list->steps[p_list->tail&HAT_CM_LENS_MAX_STEP_MASK];
}
static inline void hat_cm_lens_move_list_add_tail(hat_cm_lens_move_list_t *p_list)
{
    p_list->tail++;
}
static inline int hat_cm_lens_move_list_is_empty(hat_cm_lens_move_list_t *p_list)
{
    return (p_list->head == p_list->tail);
}
static inline int hat_cm_lens_move_list_empty_num(hat_cm_lens_move_list_t *p_list)
{
    return (HAT_CM_LENS_MAX_STEP_MASK - (p_list->head - p_list->tail));
}
static inline int hat_cm_lens_move_list_num(hat_cm_lens_move_list_t *p_list)
{
    return (p_list->head - p_list->tail);
}

static inline void hat_cm_lens_move_list_push(hat_cm_lens_move_list_t *p_list,
                                              float lens_pos,
                                              uint32 time_sleep_us)
{
    hat_cm_lens_micro_step_t *step;

    if (osal_mutex_lock_timeout(p_list->lock, DEFAULT_ALLOC_TIMEOUT_MS))
        return;
    if (hat_cm_lens_move_list_empty_num(p_list))
    {
        step = hat_cm_lens_move_list_get_head(p_list);
        step->lens_pos = lens_pos;
        step->time_sleep_us = time_sleep_us;
        hat_cm_lens_move_list_add_head(p_list);
    }
    osal_mutex_unlock(p_list->lock);
}

static inline hat_cm_lens_micro_step_t* hat_cm_lens_move_list_pop_first(hat_cm_lens_move_list_t *p_list)
{
hat_cm_lens_micro_step_t *p_entry = NULL;

    if (0 == osal_mutex_lock_timeout(p_list->lock, DEFAULT_ALLOC_TIMEOUT_MS))
    {
        if (!hat_cm_lens_move_list_is_empty(p_list))
        {
            p_entry = hat_cm_lens_move_list_get_tail(p_list);
        }
        osal_mutex_unlock(p_list->lock);
    }

    return p_entry;
}

static inline hat_cm_lens_micro_step_t* hat_cm_lens_move_list_pop_next(hat_cm_lens_move_list_t *p_list)
{
hat_cm_lens_micro_step_t *p_entry = NULL;

    if (0 == osal_mutex_lock_timeout(p_list->lock, DEFAULT_ALLOC_TIMEOUT_MS))
    {
        hat_cm_lens_move_list_add_tail(p_list);

        if (!hat_cm_lens_move_list_is_empty(p_list))
        {
            p_entry = hat_cm_lens_move_list_get_tail(p_list);
        }
        osal_mutex_unlock(p_list->lock);
    }

    return p_entry;
}

static inline void hat_cm_lens_move_list_flush(hat_cm_lens_move_list_t *p_list)
{
    osal_mutex_lock_timeout(p_list->lock, DEFAULT_ALLOC_TIMEOUT_MS);
    p_list->tail = p_list->head;
    osal_mutex_unlock(p_list->lock);
}


typedef enum {
    LENS_MOVE_ABORT,
    LENS_MOVE_ERROR,
    LENS_MOVE_LAST,
    LENS_MOVE_PROGRESS
} LENS_MOVE_DESCR_T;

typedef int (*hat_cm_lens_move_cb_t)(void* user_data,
                                           LENS_MOVE_DESCR_T descr,
                                           float micro_step_start,
                                           float micro_step_end,
                                           uint32 time_sleep_us);

typedef enum {
    LENS_OPT_STAB_MODE_OFF,
    LENS_OPT_STAB_MODE_ON,
} HAT_CM_LENS_OPT_STAB_MODE_T;

typedef enum {
    LENS_STATE_UNKNOWN,
    LENS_STATE_CREATED,
    LENS_STATE_OPENED,
    LENS_STATE_CLOSED,
} HAT_CM_LENS_STATE_T;

typedef struct {
    float near;
    float far;
}hat_cm_lens_foc_rng_t;

typedef struct {
    float                  aperture_size;
    float                  filter_density;
    float                  focal_length;
    float                  focus_distance;
    hat_cm_lens_foc_rng_t  focus_range;
    int8                   optical_stab_mode;
    int8                   state;
} hat_cm_lens_dyn_props_t;

typedef enum {
    LENS_POWER_MODE_OFF,  // lens is inactive
    LENS_POWER_MODE_ON    // lens is active
} HAT_CM_LENS_POWER_MODE_T;

typedef struct {
    int         id;
    const char* name;
} hat_cm_lens_cr_prms_t;

/* ----------------------------- Lens controls ------------------------------ */
typedef enum {
    LENS_CFG_DUMMY,
    LENS_CFG_MOVE_TO_POS,
    LENS_CFG_MOVE,
} HAT_CM_LENS_CONFIG_OPER_T;

typedef struct {
    HAT_CM_LENS_CONFIG_OPER_T cfg_type;
    union {
        struct {
            hat_cm_lens_move_list_t*    move_list;
            hat_cm_lens_move_cb_t       micro_step_done_cb;
            void*                       user_data;
        } move_to_pos;
        struct {
            hat_cm_lens_move_list_t*    move_list;
            hat_cm_lens_move_cb_t       micro_step_done_cb;
            void*                       user_data;
        } move;
    } cfg;
} hat_cm_lens_config_t;

typedef enum {
    LENS_CTRL_DUMMY,
    LENS_CTRL_INIT,
    LENS_CTRL_ABORT,
    LENS_CTRL_SET_APERTURE,
    LENS_CTRL_SET_DENSITY_FILTER,
    LENS_CTRL_SET_OPTIC_STAB_MODE,
    LENS_CTRL_SET_FOCAL_LENGHT
} HAT_CM_LENS_CONTROL_OPER_T;

typedef struct {
    HAT_CM_LENS_CONTROL_OPER_T ctrl_type;
    union {
        void*                       init_params;
        void*                       abort_params;
        float*                      aperture;
        float*                      dens_filter;
        float*                      focal_lenght;
        HAT_CM_LENS_OPT_STAB_MODE_T o_stab_mode;
    } ctrl;
} hat_cm_lens_control_t;

typedef enum {
    LENS_PROCESS_DUMMY,
} HAT_CM_LENS_PROCESS_OPER_T;

typedef struct {
    HAT_CM_LENS_PROCESS_OPER_T process_type;
    union {
    } process;
} hat_cm_lens_process_t;

typedef enum {
    LENS_QUERY_DUMMY,
    LENS_QUERY_GET_DYN_PROPS,
    LENS_QUERY_GET_FEATURES,
    LENS_QUERY_MEASURE_POS,
} HAT_CM_LENS_QUERY_OPER_T;

typedef struct {
    HAT_CM_LENS_QUERY_OPER_T    query_type;
    union {
        hat_cm_lens_dyn_props_t*    dyn_props;
        hat_lens_features_t**       features;
        float*                      pos_val;
    } query;
} hat_cm_lens_query_t;



typedef struct hat_cm_lens_obj hat_cm_lens_obj_t;

typedef hat_cm_lens_obj_t* hat_cm_lens_handle_t;

struct hat_cm_lens_obj {
    void *prvt;
    int (*set_event_cb)   (hat_cm_lens_handle_t hndl);

    int (*cm_lens_set_power_mode) (hat_cm_lens_handle_t hndl,
                                   HAT_CM_LENS_POWER_MODE_T mode);

    int (*cm_lens_init)   (hat_cm_lens_handle_t hndl);
    int (*move_to_pos)    (hat_cm_lens_handle_t hndl,
                           hat_cm_lens_move_list_t* move_list,
                           hat_cm_lens_move_cb_t micro_step_done_cb,
                           void* user_data);
    int (*move)           (hat_cm_lens_handle_t hndl,
                           hat_cm_lens_move_list_t* move_list,
                           hat_cm_lens_move_cb_t micro_step_done_cb,
                           void *user_data);
    int (*meas_pos)       (hat_cm_lens_handle_t hndl, float *pos_val);
    int (*park)           (hat_cm_lens_handle_t hndl);
    int (*cm_lens_abort)  (hat_cm_lens_handle_t hndl);
    int (*set_shutter)    (hat_cm_lens_handle_t hndl, int set);
    int (*set_aperture)   (hat_cm_lens_handle_t hndl, float* aper);
    int (*set_dens_filter)(hat_cm_lens_handle_t hndl, float* filter);
    int (*set_o_stab)     (hat_cm_lens_handle_t hndl, int mode);
    int (*set_foc_lenght) (hat_cm_lens_handle_t hndl, float* focal_lenght);
    int (*nvm_data_set)   (hat_cm_lens_handle_t hndl, void* data);
    int (*tuning_data_set)(hat_cm_lens_handle_t hndl, void* data);
    int (*cm_lens_get_state) (hat_cm_lens_handle_t hndl,
                              HAT_CM_LENS_STATE_T *state);
    int (*cm_lens_get_power_mode) (hat_cm_lens_handle_t hndl,
                                   HAT_CM_LENS_POWER_MODE_T *mode);
    int (*cm_lens_get_dyn_props) (hat_cm_lens_handle_t hndl,
                                  hat_cm_lens_dyn_props_t* dyn_props);
    int (*cm_lens_get_features)  (hat_cm_lens_handle_t hndl,
                                  hat_lens_features_t** features);

    int (*cm_lens_open)          (hat_cm_lens_handle_t hndl,
                                  void *sock_hndl,
                                  void *ph_lens_hndl,
                                  void* lens_features);
    int (*cm_lens_config)        (hat_cm_lens_handle_t hndl,
                                  hat_cm_lens_config_t* cfg_prms);
    int (*cm_lens_control)       (hat_cm_lens_handle_t hndl, void* ctrl_prms);
    int (*cm_lens_process)       (hat_cm_lens_handle_t hndl, void* process_prms);
    int (*cm_lens_query)         (hat_cm_lens_handle_t hndl, void* query_prms);
    int (*cm_lens_close)         (hat_cm_lens_handle_t hndl);
    int (*cm_lens_destroy)       (hat_cm_lens_handle_t hndl);
};

#endif // __HAL_CM_LENS_H__
