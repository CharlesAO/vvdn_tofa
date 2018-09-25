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
* @file hat_cm_sensor.h
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

#ifndef __HAL_CM_SENSOR_H__
#define __HAL_CM_SENSOR_H__

#include <hal/hat_types.h>
#include <hal/hat_pix_fmt.h>
#include <hal/include/hal/hal_camera_module/hat_cm_socket.h>
#include <hal/include/hal/hal_camera_module/hat_cm_mipi_rx.h>

#define ROUND_FLOAT_X_TO_INT(x) \
    ((x) >= 0 ? (long)((x) + 0.5) : (long)((x) - 0.5))

typedef enum {
    REF_ILL_NONE = 0,
    REF_ILL_DAYLIGHT = 1,
    REF_ILL_FLUORESCENT = 2,
    REF_ILL_TUNGSTEN = 3,                 // Incandescent light;
    REF_ILL_FLASH = 4,
    REF_ILL_FINE_WEATHER = 9,
    REF_ILL_CLOUDY_WEATHER = 10,
    REF_ILL_SHADE = 11,
    REF_ILL_DAYLIGHT_FLUORESCENT = 12,    // D 5700 - 7100K;
    REF_ILL_DAY_WHITE_FLUORESCENT = 13,   // N 4600 - 5400K;
    REF_ILL_COOL_WHITE_FLUORESCENT = 14,  // W 3900 - 4500K
    REF_ILL_WHITE_FLUORESCENT = 15,       // WW 3200 - 3700K
    REF_ILL_STANDARD_A = 17,
    REF_ILL_STANDARD_B = 18,
    REF_ILL_STANDARD_C = 19,
    REF_ILL_D55 = 20,
    REF_ILL_D65 = 21,
    REF_ILL_D75 = 22,
    REF_ILL_D50 = 23,
    REF_ILL_ISO_STUDIO_TUNGSTEN = 24
}reference_illuminant_t;

typedef struct
{
    int                  intent; // should be gzz_capture_intent_t
    hat_size_t           resolution;
    hat_rect_t           crop;
    hat_range_float_t    fps_range;
    int                  pipeline_id; // should be gzz_cam_custom_usecase_t
} hat_cm_sensor_select_mode_t;

typedef struct {
    uint32 oper_count;
    const hat_cm_socket_command_entry_t* oper;
    uint32 data;
} hat_sensor_oper_entry_t;

typedef enum {
    SENSOR_INIT_1,
    SENSOR_INIT_2,
    SENSOR_INIT_3,
    SENSOR_INIT_MAX,
} hat_sensor_init_idx;

typedef struct {
    hat_sensor_oper_entry_t pwr_off;
    hat_sensor_oper_entry_t pwr_on;
    hat_sensor_oper_entry_t init[SENSOR_INIT_MAX];
    hat_sensor_oper_entry_t stream_off;
    hat_sensor_oper_entry_t stream_on;
    hat_sensor_oper_entry_t gph_on;
    hat_sensor_oper_entry_t gph_off;
} hat_sensor_operations_t;

/**
 * Sensor mode parameters
 *
 * @param field_of_view Sensor active view area in physical area [pixels]
 * @param active        Sensor active output pixels [pixels]
 * @param pixels        Equivalent of a digital pixel in physical pixels
 * @param aspect        Aspect ration on 1 pixel
 * @param fps           sensor mode Min/Max fps
 * @param exposure      Min/Max exposure time in [us]
 * @param format        Color format and pattern
 *
 * @param v_flip        [ON/OFF]
 * @param h_mirror      [ON/OFF]
 * @param sensitivity   U16Q8, Sensitivity: binning/skipping/average
 * @param pixel_clock   pixel clock [Hz]
 * @param row_time      One line readout time (line & blanking) [ns]
 */
typedef struct {

    hat_rect_t        field_of_view;   // Sensor active view area in physical area [pixels]
    hat_size_t        active;          // Sensor active output pixels [pixels]
    hat_size_t        pixels;          // Equivalent of a digital pixel in physical pixels
    hat_size_t        aspect;          // Aspect ration on 1 pixel
    hat_range_float_t fps;             // sensor mode Min/Max fps
    hat_range_t       exposure;        // Min & Max exposure time in [us]
    hat_pix_fmt_t     format;                // Color format and pattern
    //    uint16                  rotate;          // 0, 90, 180, 270 - Rotate TODO should come from somewhere???
    uint16            v_flip;          // [ON/OFF]
    uint16            h_mirror;        // [ON/OFF]
    float             sensitivity;     //  mode (binning/skipping/average) sensitivity against full mode
    uint32            pixel_clock;     // In Hz
    uint32            row_time;        // [ns] - Readout time of line & blanking
    uint32            frame_time;      // [us] - Readout time of the frame & blanking
    uint32            ppln;            // Initial number PixelsPerLiNe
    uint32            lpfr;            // Initial number LinesPerFRame
    uint32            num_bad_frames;  // Number of bad frames after mode switch
    const void       *mode_settings;
    const uint32      mode_settings_num;

    uint32            pipeline_id;
    mipi_rx_cfg_t     mipi_rx_cfg;
    void             *sensor_specific_data;
} hat_sensor_mode_t;

typedef struct {
    uint32 frm_length_lines;        //LPFR register addres high byte
    uint32 frm_length_lines_size;   //LPFR size in bytes
    uint32 line_length_pck;         //PPLN register addres high byte
    uint32 line_length_pck_size;    //PPLN size in bytes
} hat_sensor_frame_size_regs_t;

/**
 * Sensor physical features
 *
 * @param array_size        Physical area (valid + black pixels)
 * @param valid_pixels      Sensor full area in physical area
 * @param base_ISO          real sensor sensitivity with gain = 1 [U32Q16]
 * @param again             min/max sensor gain analog
 * @param dgain             min/max sensor gain dnalog
 * @param exp_gain_delay    exposure to gain delay [frames]
 * @param operations        Sensor operations
 * @param modes             Sensor modes description
 * @param powerup_bad_frames  Number of bad frames after powerup
 * @param temp_sensor_supp   If sensor support temperature sensor - 1. If no - 0.
HAL3 specific data.
 * @param sensitivity_range          Range of valid sensitivities Min <= 100, Max >= 1600
 * @param color_filter_arrangement   Arrangement of color filters on sensor
 * @param exposure_time_range        Range of valid exposure times in [nanoseconds]
 * @param max_frame_duration         Maximum possible frame duration in [nanoseconds].
 * @param physical_size              The physical dimensions of the full pixel array in [mm]
 * @param white_level                Maximum raw value output by sensor
 * @param base_gain_factor           Gain factor from electrons to raw units when ISO=100
 * @param black_level_pattern        A fixed black level offset for each of the Bayer mosaic channels.
 * @param calib_transform_1          Per-device calibration on top of color space transform 1
 * @param calib_transform_2          Per-device calibration on top of color space transform 2
 * @param color_transform_1          Linear mapping from XYZ (D50) color space to reference linear sensor color.
 * @param color_transform_2          Linear mapping from XYZ (D50) color space to reference linear sensor color.
 * @param forward_matrix_1           Used by DNG for better WB adaptation.
 * @param forward_matrix_2           Used by DNG for better WB adaptation.
 * @param max_analog_sensitivity      Maximum sensitivity that is implemented purely through analog gain.
 * @param noise_model_coef           Estimation of sensor noise characteristics.
 * @param orientation                degrees clockwise rotation
 * @param reference_illuminant_1     Light source used to define transform 1.
 * @param reference_illuminant_2     Light source used to define transform 2.
 * @param frame_size_regs;           sensor frame timing registers LPFR/PPLN
 *
 *@see hat_sensor_mode_t
*/
typedef struct  {
    hat_size_t              array_size;     // Physical area (valid + black pixels)
    hat_rect_t              valid_pixels;   // Sensor full area in physical area
    uint32                  base_ISO;       // real sensor sensitivity with gain = 1 [U32Q16]
    hat_bounds_float_t      again;          // min/max sensor gain analog
    hat_bounds_float_t      dgain;          // min/max sensor gain dnalog
    uint32                  exp_gain_delay; // exposure to gain delay [frames]
    uint32                  global_delay;   // delay to output frame with applied parameters [frames]
    // Sensor operations
    const hat_sensor_operations_t operations;
    // Sensor modes description
    struct {
        uint32                   num;   // number of sensor modes
        const hat_sensor_mode_t *list; // list with sensor modes
    } modes;
    uint32                  powerup_bad_frames;  // Number of bad frames after powerup
    uint8                   temp_sensor_supp;    // If sensor support temperature sensor - 1. If no - 0.
    //HAL3 specific data.
    int32                   sensitivity_range[2];     // Range of valid sensitivities Min <= 100, Max >= 1600
    hat_pix_order_t         color_filter_arrangement; // Arrangement of color filters on sensor TODO we have same info in sens mode descriptor
    int64                   exposure_time_range[2];   // Range of valid exposure times in [nanoseconds] TODO we have same info in sens mode descriptor
    int64                   max_frame_duration;       // Maximum possible frame duration in [nanoseconds].
    float                  physical_size[2];         // The physical dimensions of the full pixel array in [mm]
    int32                   white_level;              // Maximum raw value output by sensor
    hat_fract_t             base_gain_factor;         // Gain factor from electrons to raw units when ISO=100
    int32                   black_level_pattern[4];   // A fixed black level offset for each of the Bayer mosaic channels.
    hat_fract_t             calib_transform_1[9];     // Per-device calibration on top of color space transform 1
    hat_fract_t             calib_transform_2[9];     // Per-device calibration on top of color space transform 2
    hat_fract_t             color_transform_1[9];     // Linear mapping from XYZ (D50) color space to reference linear sensor color.
    hat_fract_t             color_transform_2[9];     // Linear mapping from XYZ (D50) color space to reference linear sensor color.
    hat_fract_t             forward_matrix_1[9];      // Used by DNG for better WB adaptation.
    hat_fract_t             forward_matrix_2[9];      // Used by DNG for better WB adaptation.
    int32                   max_analog_sensitivity;    // Maximum sensitivity that is implemented purely through analog gain.
    float                  noise_model_coef[2];      // Estimation of sensor noise characteristics.
    int32                   orientation;              // degrees clockwise rotation TODO we have same info in sens mode descriptor
    reference_illuminant_t  reference_illuminant_1;   // Light source used to define transform 1.
    reference_illuminant_t  reference_illuminant_2;   // Light source used to define transform 2.
    hat_sensor_frame_size_regs_t frame_size_regs;     // sensor frame timing registers LPFR/PPLN

} hat_sensor_features_t;

/**
 * Sensor configuration
 *
 * @param sensor_mode_idx  [0-num_sen_modes] index in sen_modes list
 * @param fps              request Min/Max fps. To select fixed frame rate: min==max=fps
 *
 * @see hat_sensor_features_t
 */
typedef struct {
    uint32      sensor_mode_idx;     // [0-num_sen_modes] index in sen_modes list
    hat_range_t frame_duration;      // frame duration min/max in usec - calculate from fps min/max
    uint32      current_frame_duration;
} hat_sen_config_t;

// required sensor gain is again * dgain
typedef struct {
    uint32 exposure;    // U32Q0 Exposure time in us
    float  again;       // sensor analog gain
    float  dgain;       // sensor analog ghat_sensor_mode_tain
}hat_exp_gain_t;

typedef struct {
    hat_exp_gain_t applied;    //
    uint32  row_time;          // [ns] - Readout time of line & blanking
    uint32  blanking_lines;    // Number of blanking lines
}hat_sen_report;








typedef enum {
    SENSOR_STATE_UNKNOWN,
    SENSOR_STATE_CREATED,
    SENSOR_STATE_OPENED,
    SENSOR_STATE_CLOSED,
} HAT_CM_SENSOR_STATE_T;

typedef struct {
    int64 exposure_time_ns;
    int64 frameDuration_ns;
    int32 sensitivity_iso;
    int64 exp_start_timestamp_ns;
    float sensor_temperature_c;
} hat_cm_sensor_dyn_props_t;

typedef enum {
    SENSOR_POWER_MODE_OFF,  // sensor is inactive
    SENSOR_POWER_MODE_ON    // sensor is active
} HAT_CM_SENSOR_POWER_MODE_T;

typedef enum {
    SENSOR_STREAM_MODE_OFF,  // sensor is not streaming (SW standby)
    SENSOR_STREAM_MODE_ON    // sensor is streaming
} HAT_CM_SENSOR_STREAM_MODE_T;

typedef struct {
    float  fps;
    uint32 pix_clk_hz;
    uint32 linetime_ns;
    uint32 frametime_us;
    uint32 ppln;
    uint32 lpfr;
} hat_cm_sensor_frm_timing_t;

typedef struct {
    int         id;
    const char* name;
} hat_cm_sensor_cr_prms_t;

/* ---------------------------- Sensor controls ----------------------------- */
typedef enum {
    SENSOR_CFG_DUMMY,
    SENSOR_CFG_SET_EXP_GAIN,
    SENSOR_CFG_SET_FRAME_DURATION,
    SENSOR_CFG_SET_MODE,
    SENSOR_CFG_POWER_ON,
    SENSOR_CFG_POWER_OFF,
    SENSOR_CFG_INIT,
} HAT_CM_SENSOR_CONFIG_OPER_T;

typedef struct {
    HAT_CM_SENSOR_CONFIG_OPER_T cfg_type;
    union {
        hat_exp_gain_t    exp_gain;
        uint32            frm_duration;
        hat_sen_config_t* sen_cfg;
        uint32            init_data;
    } cfg;
} hat_cm_sensor_config_t;

typedef enum {
    SENSOR_CTRL_DUMMY,
    SENSOR_CTRL_START_STOP,
} HAT_CM_SENSOR_CONTROL_OPER_T;

typedef struct {
    HAT_CM_SENSOR_CONTROL_OPER_T ctrl_type;
    union {
        HAT_CM_SENSOR_STREAM_MODE_T mode;
    } ctrl;
} hat_cm_sensor_control_t;

typedef enum {
    SENSOR_PROCESS_DUMMY,
} HAT_CM_SENSOR_PROCESS_OPER_T;

typedef struct {
    HAT_CM_SENSOR_PROCESS_OPER_T process_type;
    union {
    } process;
} hat_cm_sensor_process_t;

typedef enum {
    SENSOR_QUERY_DUMMY,
    SENSOR_QUERY_GET_DYN_PROPS,
    SENSOR_QUERY_GET_FEATURES,
    SENSOR_QUERY_SELECT_MODE,
} HAT_CM_SENSOR_QUERY_OPER_T;

typedef struct {
    HAT_CM_SENSOR_QUERY_OPER_T query_type;
    union {
        hat_cm_sensor_dyn_props_t*          dyn_props;
        hat_sensor_features_t**             features;
        struct {
            hat_cm_sensor_select_mode_t*    mode_rqst;
            uint32*                         mode_index;
        } sel_mode;
    } query;
} hat_cm_sensor_query_t;



typedef struct hat_cm_sensor_obj hat_cm_sensor_obj_t;

typedef hat_cm_sensor_obj_t* hat_cm_sensor_handle_t;

struct hat_cm_sensor_obj {
    void *prvt;

    int (*set_event_cb)    (hat_cm_sensor_handle_t hndl);

    int (*set_power_mode)  (hat_cm_sensor_handle_t hndl,
                            HAT_CM_SENSOR_POWER_MODE_T mode);

    int (*hal_cm_sen_init) (hat_cm_sensor_handle_t hndl, uint32 init_idx);

    int (*select_mode)     (hat_cm_sensor_handle_t hndl,
                            hat_cm_sensor_select_mode_t* mode_rqst,
                            uint32* mode_index);

    int (*set_mode)        (hat_cm_sensor_handle_t hndl,
                            hat_sen_config_t *sen_cfg);

    int (*set_stream_mode) (hat_cm_sensor_handle_t hndl,
                            HAT_CM_SENSOR_STREAM_MODE_T mode);

    int (*set_exp_gain)    (hat_cm_sensor_handle_t hndl,
                            hat_exp_gain_t exp_gain);

    int (*set_frm_duration) (hat_cm_sensor_handle_t hndl, uint32 duration);

    int (*get_dyn_props)   (hat_cm_sensor_handle_t hndl,
                            hat_cm_sensor_dyn_props_t* dyn_props);

    int (*get_features)    (hat_cm_sensor_handle_t hndl,
                            hat_sensor_features_t** features);

    int (*get_stream_mode) (hat_cm_sensor_handle_t hndl,
                            HAT_CM_SENSOR_STREAM_MODE_T *mode);

    int (*get_power_mode)  (hat_cm_sensor_handle_t hndl,
                            HAT_CM_SENSOR_POWER_MODE_T *mode);

    int (*get_mode)        (hat_cm_sensor_handle_t hndl);

    int (*get_exp_gain)    (hat_cm_sensor_handle_t hndl,
                            hat_exp_gain_t *exp_gain);

    int (*get_fps)        (hat_cm_sensor_handle_t hndl);

    int (*get_state)      (hat_cm_sensor_handle_t hndl,
                           HAT_CM_SENSOR_STATE_T *state);

    int (*open)           (hat_cm_sensor_handle_t hndl,
                           void *sock_hndl,
                           void *ph_sen_hndl,
                           void* sens_features);

    int (*sen_obj_config) (hat_cm_sensor_handle_t hndl, void* cfg_prms);

    int (*control)        (hat_cm_sensor_handle_t hndl, void* ctrl_prms);

    int (*process)        (hat_cm_sensor_handle_t hndl, void* process_prms);

    int (*query)          (hat_cm_sensor_handle_t hndl, void* query_prms);

    int (*close)          (hat_cm_sensor_handle_t hndl);

    int (*destroy)        (hat_cm_sensor_handle_t hndl);
};

#endif // __HAL_CM_SENSOR_H__
