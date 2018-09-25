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
* @file gzz_cam_config.h
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

#ifndef GZZ_CAM_CONFIG_H_
#define GZZ_CAM_CONFIG_H_

#include <configurator/include/configurator.h>
#include <hal/hat_types.h>
#include "aca_ae_calc_types.h"
#include "aca_awb_types.h"
#include "aca_af_types.h"
#include "gzz_effects_types.h"
#include "gzz_scene_types.h"
#include "hal/hat_gamma.h"

#define GZZ_CAM_EXP_BRACKETING_SEQUENCE_LENGHT   3

#define GZZ_CAM_CONFIG_MAX_STREAMS 8

#define is_gzz_cfg_changed(active, cuerrent, name)      \
    (!active && cuerrent) ? TRUE :                      \
        (active->name.ctrl.seq_num !=                   \
            cuerrent->name.ctrl.seq_num)                \

typedef enum {
    CAM_RGB2RGB_MANUAL,
    CAM_RGB2RGB_AUTO
} gzz_rgb2rgb_mode_t;

typedef enum {
    CAM_GAMMA_MANUAL,
    CAM_GAMMA_AUTO
}gzz_gamma_mode_t;

typedef enum {
    CAM_LSC_MANUAL,
    CAM_LSC_AUTO
}gzz_lsc_mode_t;

typedef struct {
    hat_size_t          lsc_tbl_size;    // Count of lsc table points
    hat_bayer_float_t   *lsc_tbl;        // [HIF_LENS_SHADING_MAP_SIZE];
}gzz_lsc_t;

typedef struct {
    hat_bayer_float_t   lsc_strength;
}gzz_lsc_strength_t;

typedef enum {
    CAM_EDGE_MANUAL,
    CAM_EDGE_AUTO
}gzz_edge_type_t;

typedef enum {
    CAM_CFAI_NORMAL,
    CAM_CFAI_FAST,
    CAM_CFAI_HQ
}gzz_cfai_mode_t;

typedef enum {
    CAM_LDC_MODE_OFF,
    CAM_LDC_MODE_ON
}gzz_ldc_mode_t;

typedef enum {
    CAM_DPC_MODE_OFF,
    CAM_DPC_MODE_ON
}gzz_dpc_mode_t;

typedef enum
{
    CAM_BLACK_LEVEL_LOCK_OFF,
    CAM_BLACK_LEVEL_LOCK_ON
} gzz_black_level_lock_t;

typedef enum {
    CAM_NF_MODE_OFF,
    CAM_NF_MODE_ON
}gzz_nf_mode_t;

typedef enum {
    CAM_FLICKER_MODE_OFF,
    CAM_FLICKER_MODE_50HZ,
    CAM_FLICKER_MODE_60HZ,
    CAM_FLICKER_MODE_AUTO
}gzz_flicker_mode_t;

typedef enum {
    CAM_FLASH_AUTO,
    CAM_FLASH_FORCE_OFF,
    CAM_FLASH_FORCE_ON
}gzz_flash_mode_t;

typedef enum
{
    CAM_ACA_MODE_OFF,
    CAM_ACA_MODE_AUTO,
    CAM_ACA_MODE_USE_SCENE_MODE
} gzz_3a_mode_t;

typedef enum
{
    CAM_AE_MODE_OFF,
    CAM_AE_MODE_ON,
    CAM_AE_MODE_ON_AUTO_FLASH,
    CAM_AE_MODE_ON_ALWAYS_FLASH,
    CAM_AE_MODE_ON_AUTO_FLASH_REDEYE
}gzz_ae_mode_t;


typedef enum
{
    CAM_ALG_LOCK_OFF,
    CAM_ALG_LOCK_ON
} gzz_lock_type_t;


typedef enum
{
    CAM_AF_TRIGGER_IDLE,
    CAM_AF_TRIGGER_START,
    CAM_AF_TRIGGER_CANCEL
} gzz_af_trigger_t;

typedef enum
{
    CAM_VIDEO_STABILIZATION_MODE_OFF,
    CAM_VIDEO_STABILIZATION_MODE_ON
} gzz_video_stabilization_mode_t;

typedef enum
{
    CAM_FACE_DETECT_MODE_OFF,
    CAM_FACE_DETECT_MODE_SIMPLE,
    CAM_FACE_DETECT_MODE_FULL
}gzz_face_detect_mode_t;


typedef enum
{
    CAM_CAPTURE_INTENT_CUSTOM,
    CAM_CAPTURE_INTENT_PREVIEW,
    CAM_CAPTURE_INTENT_STILL_CAPTURE,
    CAM_CAPTURE_INTENT_VIDEO_RECORD,
    CAM_CAPTURE_INTENT_VIDEO_SNAPSHOT,
    CAM_CAPTURE_INTENT_ZERO_SHUTTER_LAG
} gzz_capture_intent_t;

#define IS_CAPTURE_MODE(mode) ((CAM_CAPTURE_INTENT_STILL_CAPTURE == mode)|| \
                                  (CAM_CAPTURE_INTENT_VIDEO_SNAPSHOT == mode)|| \
                                  (CAM_CAPTURE_INTENT_ZERO_SHUTTER_LAG == mode))

typedef enum
{
    CAM_LENS_OPTICAL_STABILIZATION_MODE_OFF,
    CAM_LENS_OPTICAL_STABILIZATION_MODE_ON,
} gzz_lens_optical_stab_mode_t;



typedef enum
{
    CAM_METADATA_MODE_NONE,
    CAM_METADATA_MODE_FULL
} gzz_request_metadata_mode_t;

typedef enum {
    CAM_STAT_MODE_OFF,
    CAM_STAT_MODE_ON
} gzz_stat_mode_t;

typedef struct {
    gzz_stat_mode_t  hist;         // hif_histogram_mode_t
    gzz_stat_mode_t  af_stat;      // hif_sharpnes_mode_t
    gzz_stat_mode_t  lsc;          // hif_lsc_map_mode_t
}meta_stats_mode_t;

typedef enum {
    CAM_STREAM_OFF,
    CAM_STREAM_ON
} gzz_cam_stream_cntrl_t;

typedef struct {
    uint32          id;
    hat_pix_fmt_t   format;
    hat_size_t      size;
    uint32          type;
} gzz_cam_stream_t;


typedef enum {
    CAM_CUSTOM_USECASE_SELECTION_STILL,
    CAM_CUSTOM_USECASE_SELECTION_VIDEO,
    CAM_CUSTOM_USECASE_SELECTION_LOW_POWER_VIDEO,
} gzz_cam_custom_usecase_t;

typedef enum {
    CAM_CUSTOM_CAPTURE_MODE_SELECTION_TEMPORAL_BRACKETING,
    CAM_CUSTOM_CAPTURE_MODE_SELECTION_EXPOSURE_BRACKETING,
    CAM_CUSTOM_CAPTURE_MODE_SELECTION_LONG_EXPOSURE,
} gzz_cam_custom_capture_mode_t;


typedef struct {

    configurator_config_struct_node_t ctrl;

    uint32 request_id;

    struct {
        configurator_config_struct_node_t   ctrl;
        uint32                              val;
    } frame_number;

    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_cam_stream_t                    val[GZZ_CAM_CONFIG_MAX_STREAMS];
    } stream;

    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_cam_stream_cntrl_t              val[GZZ_CAM_CONFIG_MAX_STREAMS];
    } stream_cntrl;

    // manual camera frame duration
    struct {
        configurator_config_struct_node_t   ctrl;
        uint32                              val; // [nsec]
    } fr_duration;

    struct {
        configurator_config_struct_node_t   ctrl;
        hat_range_float_t                   val;
    } fps_range;

    // image sensor crop
    struct {
        configurator_config_struct_node_t   ctrl;
        hat_rect_t                          val;
    } sensor_crop;

    // manual rgb2rgb
    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_rgb2rgb_mode_t                  val;
    } rgb2rgb_mode;
    struct {
        configurator_config_struct_node_t   ctrl;
        hat_rgb2rgb_t                       val;
    } manual_rgb2rgb;

    // Manual gamma
    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_gamma_mode_t                    val;
    } gamma_mode;

    struct {
        configurator_config_struct_node_t   ctrl;
        hat_gamma_t                         val;
    } manual_gamma;

    // Manual LSC
    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_lsc_mode_t                      val;
    } lsc_mode;

    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_lsc_t                           val;
    } lsc_tbl;

    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_lsc_strength_t                  val;
    } lsc_strength;

    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_edge_type_t                     val;
    } ee_type;

    struct {
        configurator_config_struct_node_t   ctrl;
        hat_strength_t                      val;
    } ee_strength;

    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_cfai_mode_t                     val;
    } cfai_mode;

    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_ldc_mode_t                      val; // lens correction mode
    } ldc_mode;

    struct {
        configurator_config_struct_node_t   ctrl;
        hat_strength_t                      val;
    } ldc_strength;

    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_dpc_mode_t                      val;
    } dpc_mode;

    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_black_level_lock_t              val; // hif_black_level_lock_t
    } bl_lock;

    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_nf_mode_t                       val;
    } nf_mode;

    struct {
        configurator_config_struct_node_t   ctrl;
        hat_strength_t                      val;
    } nf_strength;

    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_flicker_mode_t                  val;
    } flicker_mode;

    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_flash_mode_t                    val;
    } flash_mode;

    struct {
        configurator_config_struct_node_t   ctrl;
        uint32                              val;
    } flash_pwr;

    struct {
        configurator_config_struct_node_t   ctrl;
        uint32                              val; // flash_time
    } flash_duration;

    struct {
        configurator_config_struct_node_t   ctrl;
        uint32                              val;
    } flash_firing_time;

//    gzz_available_leds_t            led_ctrl;

    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_3a_mode_t                       val;
    } aca_mode;

/*-------------------------------------------------------------*/
    // ae_exposure compensation
    struct {
        configurator_config_struct_node_t   ctrl;
        float                               val; //ae_ev;
    } exp_compensation;

    // manual camera exposure time
    struct {
        configurator_config_struct_node_t   ctrl;
        hat_exp_gain_t                      val; // hif_camera_exposure_time_t, hif_camera_sensitivity_t
    } manual_exposure;

    // Manual AE
    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_ae_mode_t                       val;
    } ae_mode;

    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_lock_type_t                     val;
    } ae_lock;

    struct {
        configurator_config_struct_node_t   ctrl;
        hat_reg_pri_t                       val; // hif_ae_regions_t
    } ae_regions;

//    gzz_ae_precapture_triggerlock_type_t pre_capture;

/*-------------------------------------------------------------*/
    // manual White Balance gains
    struct {
        configurator_config_struct_node_t   ctrl;
        hat_wbal_coef_t                     val; // hif_color_correction_gains_t
    } wb_gains;

    // manual color temperature
    // TODO - currently used as independent
    struct {
        configurator_config_struct_node_t   ctrl;
        uint32                              val;
    } manual_color_temp;

    // Manual White Balance
    struct {
        configurator_config_struct_node_t   ctrl;
        awb_modes_t                         val; // hif_awb_type_t
    } wb_mode;

    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_lock_type_t                     val;
    } wb_lock;

    struct {
        configurator_config_struct_node_t   ctrl;
        hat_reg_pri_t                       val;
    } wb_regions;
/*-------------------------------------------------------------*/

    struct {
        configurator_config_struct_node_t   ctrl;
        focus_range_t                       val; // hif_af_mode_t
    } af_range;

    struct {
        configurator_config_struct_node_t   ctrl;
        focus_mode_t                        val; // hif_af_mode_t
    } af_mode;

    struct {
        configurator_config_struct_node_t   ctrl;
        focus_spot_weighting_t              val;
    } af_weght;

    struct {
        configurator_config_struct_node_t   ctrl;
        hat_reg_pri_t                       val;
    } af_regions;

    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_af_trigger_t                    val; // hif_af_trigger_t
    } af_trigger;

    struct {
        configurator_config_struct_node_t   ctrl;
        float                               val;
    } lens_focal_len;
/*-------------------------------------------------------------*/
    // Effects
    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_effects_t                       val;
    } eff_type;

    // Scene
    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_scene_modes_t                   val;
    } scene_mode;

    // contrast
    struct {
        configurator_config_struct_node_t   ctrl;
        float                               val;
    } contrast;

    // saturation
    struct {
        configurator_config_struct_node_t   ctrl;
        float                               val;
    } saturation;

    // sharpness
    struct {
        configurator_config_struct_node_t   ctrl;
        float                               val;
    } sharpness;

    // brightness
    struct {
        configurator_config_struct_node_t   ctrl;
        float                               val;
    } brightness;

    // hue
    struct {
        configurator_config_struct_node_t   ctrl;
        float                               val;
    } hue;

    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_video_stabilization_mode_t      val; // hif_video_stabilization_mode_t
    } vstab_mode;

    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_face_detect_mode_t              val; // hif_face_detect_mode_t
    } fd_mode;

    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_capture_intent_t                val; // hif_capture_intent_type_t
    } cam_mode;

/* ------------------------------- LENS -------------------------------------*/

    struct {
        configurator_config_struct_node_t   ctrl;
        float                                val;
    } aperture;

    struct {
        configurator_config_struct_node_t   ctrl;
        float                                val;
    } filter_density;

    struct {
        configurator_config_struct_node_t   ctrl;
        float                                val;
    } focal_length;

    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_lens_optical_stab_mode_t        val; // hif_lens_optical_stab_mode_t
    } optical_stab_mode;

/* ----------------------------- META DATA ----------------------------------*/
    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_request_metadata_mode_t         val; // hif_request_metadata_mode_t
    } meta_mode;

    struct {
        configurator_config_struct_node_t   ctrl;
        meta_stats_mode_t                   val;
    } stats_enable;


    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_cam_custom_usecase_t            val;
    } custom_usecase_selection;

    struct {
        configurator_config_struct_node_t   ctrl;
        gzz_cam_custom_capture_mode_t       val;
    } custom_capture_mode_selection;

    struct {
        configurator_config_struct_node_t   ctrl;
        float val[GZZ_CAM_EXP_BRACKETING_SEQUENCE_LENGHT];
    } custom_exposure_bracketing_sequence;

    struct {
        configurator_config_struct_node_t   ctrl;
        uint32                              val;
    } custom_capture_number_shots;

/* ----------------------------- MERGERS ----------------------------------*/

    struct {
        configurator_config_struct_node_t   ctrl;
        uint32                              val; // weight for this camera [0-255]
    } exposure_merger;

    struct {
        configurator_config_struct_node_t   ctrl;
        uint32                              val; // weight for this camera [0-255]
    } white_balance_merger;

} cam_gzz_cfg_t;

#endif /* GZZ_CAM_CONFIG_H_ */
