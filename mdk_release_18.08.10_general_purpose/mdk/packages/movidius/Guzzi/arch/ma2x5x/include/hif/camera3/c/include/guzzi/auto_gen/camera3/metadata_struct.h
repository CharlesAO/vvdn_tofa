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
* @file camera3_metadata_struct.h
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

/*
 * ****************************************************************************
 * * controls
 * ****************************************************************************
 */
#define GUZZI_CAMERA3_CONTROLS_COLOR_CORRECTION_GAINS_SIZE \
    ( \
          GUZZI_CAMERA3_CONTROLS_COLOR_CORRECTION_GAINS_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_CONTROLS_COLOR_CORRECTION_TRANSFORM_SIZE \
    ( \
          GUZZI_CAMERA3_CONTROLS_COLOR_CORRECTION_TRANSFORM_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_CONTROLS_COLOR_CORRECTION_TRANSFORM_DIM_MAX_SIZE_2 \
    )
#define GUZZI_CAMERA3_CONTROLS_CONTROL_AE_REGIONS_SIZE \
    ( \
          GUZZI_CAMERA3_CONTROLS_CONTROL_AE_REGIONS_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_CONTROLS_CONTROL_AE_REGIONS_DIM_MAX_SIZE_2 \
    )
#define GUZZI_CAMERA3_CONTROLS_CONTROL_AE_TARGET_FPS_RANGE_SIZE \
    ( \
          GUZZI_CAMERA3_CONTROLS_CONTROL_AE_TARGET_FPS_RANGE_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_CONTROLS_CONTROL_AF_REGIONS_SIZE \
    ( \
          GUZZI_CAMERA3_CONTROLS_CONTROL_AF_REGIONS_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_CONTROLS_CONTROL_AF_REGIONS_DIM_MAX_SIZE_2 \
    )
#define GUZZI_CAMERA3_CONTROLS_CONTROL_AWB_REGIONS_SIZE \
    ( \
          GUZZI_CAMERA3_CONTROLS_CONTROL_AWB_REGIONS_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_CONTROLS_CONTROL_AWB_REGIONS_DIM_MAX_SIZE_2 \
    )
#define GUZZI_CAMERA3_CONTROLS_JPEG_GPS_COORDINATES_SIZE \
    ( \
          GUZZI_CAMERA3_CONTROLS_JPEG_GPS_COORDINATES_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_CONTROLS_JPEG_THUMBNAIL_SIZE_SIZE \
    ( \
          GUZZI_CAMERA3_CONTROLS_JPEG_THUMBNAIL_SIZE_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_CONTROLS_REQUEST_INPUT_STREAMS_SIZE \
    ( \
          GUZZI_CAMERA3_CONTROLS_REQUEST_INPUT_STREAMS_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_CONTROLS_REQUEST_OUTPUT_STREAMS_SIZE \
    ( \
          GUZZI_CAMERA3_CONTROLS_REQUEST_OUTPUT_STREAMS_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_CONTROLS_SCALER_CROP_REGION_SIZE \
    ( \
          GUZZI_CAMERA3_CONTROLS_SCALER_CROP_REGION_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_CONTROLS_TONEMAP_CURVE_BLUE_SIZE \
    ( \
          GUZZI_CAMERA3_CONTROLS_TONEMAP_CURVE_BLUE_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_CONTROLS_TONEMAP_CURVE_BLUE_DIM_MAX_SIZE_2 \
    )
#define GUZZI_CAMERA3_CONTROLS_TONEMAP_CURVE_GREEN_SIZE \
    ( \
          GUZZI_CAMERA3_CONTROLS_TONEMAP_CURVE_GREEN_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_CONTROLS_TONEMAP_CURVE_GREEN_DIM_MAX_SIZE_2 \
    )
#define GUZZI_CAMERA3_CONTROLS_TONEMAP_CURVE_RED_SIZE \
    ( \
          GUZZI_CAMERA3_CONTROLS_TONEMAP_CURVE_RED_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_CONTROLS_TONEMAP_CURVE_RED_DIM_MAX_SIZE_2 \
    )

/*
 * ****************************************************************************
 * * dynamic
 * ****************************************************************************
 */
#define GUZZI_CAMERA3_DYNAMIC_COLOR_CORRECTION_GAINS_SIZE \
    ( \
          GUZZI_CAMERA3_DYNAMIC_COLOR_CORRECTION_GAINS_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_DYNAMIC_COLOR_CORRECTION_TRANSFORM_SIZE \
    ( \
          GUZZI_CAMERA3_DYNAMIC_COLOR_CORRECTION_TRANSFORM_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_DYNAMIC_COLOR_CORRECTION_TRANSFORM_DIM_MAX_SIZE_2 \
    )
#define GUZZI_CAMERA3_DYNAMIC_CONTROL_AE_REGIONS_SIZE \
    ( \
          GUZZI_CAMERA3_DYNAMIC_CONTROL_AE_REGIONS_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_DYNAMIC_CONTROL_AE_REGIONS_DIM_MAX_SIZE_2 \
    )
#define GUZZI_CAMERA3_DYNAMIC_CONTROL_AF_REGIONS_SIZE \
    ( \
          GUZZI_CAMERA3_DYNAMIC_CONTROL_AF_REGIONS_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_DYNAMIC_CONTROL_AF_REGIONS_DIM_MAX_SIZE_2 \
    )
#define GUZZI_CAMERA3_DYNAMIC_CONTROL_AWB_REGIONS_SIZE \
    ( \
          GUZZI_CAMERA3_DYNAMIC_CONTROL_AWB_REGIONS_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_DYNAMIC_CONTROL_AWB_REGIONS_DIM_MAX_SIZE_2 \
    )
#define GUZZI_CAMERA3_DYNAMIC_JPEG_GPS_COORDINATES_SIZE \
    ( \
          GUZZI_CAMERA3_DYNAMIC_JPEG_GPS_COORDINATES_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_DYNAMIC_JPEG_THUMBNAIL_SIZE_SIZE \
    ( \
          GUZZI_CAMERA3_DYNAMIC_JPEG_THUMBNAIL_SIZE_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_DYNAMIC_LENS_FOCUS_RANGE_SIZE \
    ( \
          GUZZI_CAMERA3_DYNAMIC_LENS_FOCUS_RANGE_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_DYNAMIC_REQUEST_OUTPUT_STREAMS_SIZE \
    ( \
          GUZZI_CAMERA3_DYNAMIC_REQUEST_OUTPUT_STREAMS_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_DYNAMIC_SCALER_CROP_REGION_SIZE \
    ( \
          GUZZI_CAMERA3_DYNAMIC_SCALER_CROP_REGION_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_DYNAMIC_STATISTICS_FACE_IDS_SIZE \
    ( \
          GUZZI_CAMERA3_DYNAMIC_STATISTICS_FACE_IDS_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_DYNAMIC_STATISTICS_FACE_LANDMARKS_SIZE \
    ( \
          GUZZI_CAMERA3_DYNAMIC_STATISTICS_FACE_LANDMARKS_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_DYNAMIC_STATISTICS_FACE_LANDMARKS_DIM_MAX_SIZE_2 \
    )
#define GUZZI_CAMERA3_DYNAMIC_STATISTICS_FACE_RECTANGLES_SIZE \
    ( \
          GUZZI_CAMERA3_DYNAMIC_STATISTICS_FACE_RECTANGLES_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_DYNAMIC_STATISTICS_FACE_RECTANGLES_DIM_MAX_SIZE_2 \
    )
#define GUZZI_CAMERA3_DYNAMIC_STATISTICS_FACE_SCORES_SIZE \
    ( \
          GUZZI_CAMERA3_DYNAMIC_STATISTICS_FACE_SCORES_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_DYNAMIC_STATISTICS_HISTOGRAM_SIZE \
    ( \
          GUZZI_CAMERA3_DYNAMIC_STATISTICS_HISTOGRAM_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_DYNAMIC_STATISTICS_HISTOGRAM_DIM_MAX_SIZE_2 \
    )
#define GUZZI_CAMERA3_DYNAMIC_STATISTICS_LENS_SHADING_MAP_SIZE \
    ( \
          GUZZI_CAMERA3_DYNAMIC_STATISTICS_LENS_SHADING_MAP_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_DYNAMIC_STATISTICS_LENS_SHADING_MAP_DIM_MAX_SIZE_2 \
        * GUZZI_CAMERA3_DYNAMIC_STATISTICS_LENS_SHADING_MAP_DIM_MAX_SIZE_3 \
    )
#define GUZZI_CAMERA3_DYNAMIC_STATISTICS_PREDICTED_COLOR_GAINS_SIZE \
    ( \
          GUZZI_CAMERA3_DYNAMIC_STATISTICS_PREDICTED_COLOR_GAINS_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_DYNAMIC_STATISTICS_PREDICTED_COLOR_TRANSFORM_SIZE \
    ( \
          GUZZI_CAMERA3_DYNAMIC_STATISTICS_PREDICTED_COLOR_TRANSFORM_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_DYNAMIC_STATISTICS_PREDICTED_COLOR_TRANSFORM_DIM_MAX_SIZE_2 \
    )
#define GUZZI_CAMERA3_DYNAMIC_STATISTICS_SHARPNESS_MAP_SIZE \
    ( \
          GUZZI_CAMERA3_DYNAMIC_STATISTICS_SHARPNESS_MAP_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_DYNAMIC_STATISTICS_SHARPNESS_MAP_DIM_MAX_SIZE_2 \
        * GUZZI_CAMERA3_DYNAMIC_STATISTICS_SHARPNESS_MAP_DIM_MAX_SIZE_3 \
    )
#define GUZZI_CAMERA3_DYNAMIC_TONEMAP_CURVE_BLUE_SIZE \
    ( \
          GUZZI_CAMERA3_DYNAMIC_TONEMAP_CURVE_BLUE_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_DYNAMIC_TONEMAP_CURVE_BLUE_DIM_MAX_SIZE_2 \
    )
#define GUZZI_CAMERA3_DYNAMIC_TONEMAP_CURVE_GREEN_SIZE \
    ( \
          GUZZI_CAMERA3_DYNAMIC_TONEMAP_CURVE_GREEN_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_DYNAMIC_TONEMAP_CURVE_GREEN_DIM_MAX_SIZE_2 \
    )
#define GUZZI_CAMERA3_DYNAMIC_TONEMAP_CURVE_RED_SIZE \
    ( \
          GUZZI_CAMERA3_DYNAMIC_TONEMAP_CURVE_RED_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_DYNAMIC_TONEMAP_CURVE_RED_DIM_MAX_SIZE_2 \
    )

/*
 * ****************************************************************************
 * * static
 * ****************************************************************************
 */
#define GUZZI_CAMERA3_STATIC_CONTROL_AE_AVAILABLE_ANTIBANDING_MODES_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_CONTROL_AE_AVAILABLE_ANTIBANDING_MODES_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_CONTROL_AE_AVAILABLE_MODES_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_CONTROL_AE_AVAILABLE_MODES_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_CONTROL_AE_AVAILABLE_TARGET_FPS_RANGES_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_CONTROL_AE_AVAILABLE_TARGET_FPS_RANGES_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_STATIC_CONTROL_AE_AVAILABLE_TARGET_FPS_RANGES_DIM_MAX_SIZE_2 \
    )
#define GUZZI_CAMERA3_STATIC_CONTROL_AE_COMPENSATION_RANGE_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_CONTROL_AE_COMPENSATION_RANGE_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_CONTROL_AF_AVAILABLE_MODES_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_CONTROL_AF_AVAILABLE_MODES_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_CONTROL_AVAILABLE_EFFECTS_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_CONTROL_AVAILABLE_EFFECTS_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_CONTROL_AVAILABLE_SCENE_MODES_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_CONTROL_AVAILABLE_SCENE_MODES_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_CONTROL_AVAILABLE_VIDEO_STABILIZATION_MODES_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_CONTROL_AVAILABLE_VIDEO_STABILIZATION_MODES_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_CONTROL_AWB_AVAILABLE_MODES_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_CONTROL_AWB_AVAILABLE_MODES_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_CONTROL_SCENE_MODE_OVERRIDES_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_CONTROL_SCENE_MODE_OVERRIDES_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_STATIC_CONTROL_SCENE_MODE_OVERRIDES_DIM_MAX_SIZE_2 \
    )
#define GUZZI_CAMERA3_STATIC_HOT_PIXEL_INFO_MAP_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_HOT_PIXEL_INFO_MAP_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_STATIC_HOT_PIXEL_INFO_MAP_DIM_MAX_SIZE_2 \
    )
#define GUZZI_CAMERA3_STATIC_JPEG_AVAILABLE_THUMBNAIL_SIZES_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_JPEG_AVAILABLE_THUMBNAIL_SIZES_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_STATIC_JPEG_AVAILABLE_THUMBNAIL_SIZES_DIM_MAX_SIZE_2 \
    )
#define GUZZI_CAMERA3_STATIC_LED_AVAILABLE_LEDS_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_LED_AVAILABLE_LEDS_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_LENS_INFO_AVAILABLE_APERTURES_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_LENS_INFO_AVAILABLE_APERTURES_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_LENS_INFO_AVAILABLE_FILTER_DENSITIES_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_LENS_INFO_AVAILABLE_FILTER_DENSITIES_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_LENS_INFO_AVAILABLE_FOCAL_LENGTHS_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_LENS_INFO_AVAILABLE_FOCAL_LENGTHS_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_LENS_INFO_AVAILABLE_OPTICAL_STABILIZATION_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_LENS_INFO_AVAILABLE_OPTICAL_STABILIZATION_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_LENS_INFO_GEOMETRIC_CORRECTION_MAP_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_LENS_INFO_GEOMETRIC_CORRECTION_MAP_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_STATIC_LENS_INFO_GEOMETRIC_CORRECTION_MAP_DIM_MAX_SIZE_2 \
        * GUZZI_CAMERA3_STATIC_LENS_INFO_GEOMETRIC_CORRECTION_MAP_DIM_MAX_SIZE_3 \
        * GUZZI_CAMERA3_STATIC_LENS_INFO_GEOMETRIC_CORRECTION_MAP_DIM_MAX_SIZE_4 \
    )
#define GUZZI_CAMERA3_STATIC_LENS_INFO_GEOMETRIC_CORRECTION_MAP_SIZE_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_LENS_INFO_GEOMETRIC_CORRECTION_MAP_SIZE_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_LENS_INFO_SHADING_MAP_SIZE_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_LENS_INFO_SHADING_MAP_SIZE_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_LENS_OPTICAL_AXIS_ANGLE_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_LENS_OPTICAL_AXIS_ANGLE_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_LENS_POSITION_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_LENS_POSITION_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_REQUEST_MAX_NUM_OUTPUT_STREAMS_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_REQUEST_MAX_NUM_OUTPUT_STREAMS_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_REQUEST_MAX_NUM_REPROCESS_STREAMS_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_REQUEST_MAX_NUM_REPROCESS_STREAMS_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_SCALER_AVAILABLE_FORMATS_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_SCALER_AVAILABLE_FORMATS_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_SCALER_AVAILABLE_JPEG_MIN_DURATIONS_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_SCALER_AVAILABLE_JPEG_MIN_DURATIONS_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_SCALER_AVAILABLE_JPEG_SIZES_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_SCALER_AVAILABLE_JPEG_SIZES_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_STATIC_SCALER_AVAILABLE_JPEG_SIZES_DIM_MAX_SIZE_2 \
    )
#define GUZZI_CAMERA3_STATIC_SCALER_AVAILABLE_PROCESSED_MIN_DURATIONS_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_SCALER_AVAILABLE_PROCESSED_MIN_DURATIONS_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_SCALER_AVAILABLE_PROCESSED_SIZES_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_SCALER_AVAILABLE_PROCESSED_SIZES_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_STATIC_SCALER_AVAILABLE_PROCESSED_SIZES_DIM_MAX_SIZE_2 \
    )
#define GUZZI_CAMERA3_STATIC_SCALER_AVAILABLE_RAW_MIN_DURATIONS_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_SCALER_AVAILABLE_RAW_MIN_DURATIONS_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_SCALER_AVAILABLE_RAW_SIZES_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_SCALER_AVAILABLE_RAW_SIZES_DIM_MAX_SIZE_1 \
        * GUZZI_CAMERA3_STATIC_SCALER_AVAILABLE_RAW_SIZES_DIM_MAX_SIZE_2 \
    )
#define GUZZI_CAMERA3_STATIC_SENSOR_BLACK_LEVEL_PATTERN_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_SENSOR_BLACK_LEVEL_PATTERN_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_SENSOR_CALIBRATION_TRANSFORM1_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_SENSOR_CALIBRATION_TRANSFORM1_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_SENSOR_CALIBRATION_TRANSFORM2_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_SENSOR_CALIBRATION_TRANSFORM2_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_SENSOR_COLOR_TRANSFORM1_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_SENSOR_COLOR_TRANSFORM1_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_SENSOR_COLOR_TRANSFORM2_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_SENSOR_COLOR_TRANSFORM2_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_SENSOR_FORWARD_MATRIX1_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_SENSOR_FORWARD_MATRIX1_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_SENSOR_FORWARD_MATRIX2_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_SENSOR_FORWARD_MATRIX2_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_SENSOR_INFO_ACTIVE_ARRAY_SIZE_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_SENSOR_INFO_ACTIVE_ARRAY_SIZE_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_SENSOR_INFO_EXPOSURE_TIME_RANGE_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_SENSOR_INFO_EXPOSURE_TIME_RANGE_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_SENSOR_INFO_PHYSICAL_SIZE_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_SENSOR_INFO_PHYSICAL_SIZE_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_SENSOR_INFO_PIXEL_ARRAY_SIZE_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_SENSOR_INFO_PIXEL_ARRAY_SIZE_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_SENSOR_INFO_SENSITIVITY_RANGE_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_SENSOR_INFO_SENSITIVITY_RANGE_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_SENSOR_NOISE_MODEL_COEFFICIENTS_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_SENSOR_NOISE_MODEL_COEFFICIENTS_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_STATISTICS_INFO_AVAILABLE_FACE_DETECT_MODES_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_STATISTICS_INFO_AVAILABLE_FACE_DETECT_MODES_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_STATIC_STATISTICS_INFO_SHARPNESS_MAP_SIZE_SIZE \
    ( \
          GUZZI_CAMERA3_STATIC_STATISTICS_INFO_SHARPNESS_MAP_SIZE_DIM_MAX_SIZE_1 \
    )

/*
 * ****************************************************************************
 * ** Guzzi specific **********************************************************
 * ****************************************************************************
 */
/*
 * ****************************************************************************
 * * controls
 * ****************************************************************************
 */
#define GUZZI_CAMERA3_CONTROLS_CAPTURE_REQUEST_ACTIVE_SIZE \
    ( \
          GUZZI_CAMERA3_CONTROLS_CAPTURE_REQUEST_ACTIVE_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_CONTROLS_STREAM_CONFIG_FORMAT_SIZE \
    ( \
          GUZZI_CAMERA3_CONTROLS_STREAM_CONFIG_FORMAT_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_CONTROLS_STREAM_CONFIG_HEIGHT_SIZE \
    ( \
          GUZZI_CAMERA3_CONTROLS_STREAM_CONFIG_HEIGHT_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_CONTROLS_STREAM_CONFIG_ID_SIZE \
    ( \
          GUZZI_CAMERA3_CONTROLS_STREAM_CONFIG_ID_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_CONTROLS_STREAM_CONFIG_TYPE_SIZE \
    ( \
          GUZZI_CAMERA3_CONTROLS_STREAM_CONFIG_TYPE_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_CONTROLS_STREAM_CONFIG_WIDTH_SIZE \
    ( \
          GUZZI_CAMERA3_CONTROLS_STREAM_CONFIG_WIDTH_DIM_MAX_SIZE_1 \
    )
#define GUZZI_CAMERA3_CONTROLS_Z_CUSTOM_EXPOSURE_BRACKETING_SEQUENCE_SIZE \
    ( \
          GUZZI_CAMERA3_CONTROLS_Z_CUSTOM_EXPOSURE_BRACKETING_SEQUENCE_DIM_MAX_SIZE_1 \
    )

/*
 * ****************************************************************************
 * * controls
 * ****************************************************************************
 */
typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_black_level_lock_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_float_t v[GUZZI_CAMERA3_CONTROLS_COLOR_CORRECTION_GAINS_SIZE];
} guzzi_camera3_controls_color_correction_gains_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_color_correction_mode_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_rational_t v[GUZZI_CAMERA3_CONTROLS_COLOR_CORRECTION_TRANSFORM_SIZE];
} guzzi_camera3_controls_color_correction_transform_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_control_ae_antibanding_mode_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_controls_control_ae_exposure_compensation_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_control_ae_lock_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_control_ae_mode_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_control_ae_precapture_trigger_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_CONTROLS_CONTROL_AE_REGIONS_SIZE];
} guzzi_camera3_controls_control_ae_regions_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_CONTROLS_CONTROL_AE_TARGET_FPS_RANGE_SIZE];
} guzzi_camera3_controls_control_ae_target_fps_range_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_control_af_mode_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_CONTROLS_CONTROL_AF_REGIONS_SIZE];
} guzzi_camera3_controls_control_af_regions_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_control_af_trigger_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_control_awb_lock_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_control_awb_mode_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_CONTROLS_CONTROL_AWB_REGIONS_SIZE];
} guzzi_camera3_controls_control_awb_regions_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_control_capture_intent_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_control_effect_mode_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_control_mode_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_control_scene_mode_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_control_video_stabilization_mode_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_demosaic_mode_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_edge_mode_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_edge_strength_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_flash_firing_power_t;

typedef struct {
    guzzi_camera3_int64_t v;
} guzzi_camera3_controls_flash_firing_time_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_flash_mode_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_geometric_mode_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_geometric_strength_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_hot_pixel_mode_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_double_t v[GUZZI_CAMERA3_CONTROLS_JPEG_GPS_COORDINATES_SIZE];
} guzzi_camera3_controls_jpeg_gps_coordinates_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_jpeg_gps_processing_method_t;

typedef struct {
    guzzi_camera3_int64_t v;
} guzzi_camera3_controls_jpeg_gps_timestamp_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_controls_jpeg_orientation_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_jpeg_quality_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_jpeg_thumbnail_quality_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_CONTROLS_JPEG_THUMBNAIL_SIZE_SIZE];
} guzzi_camera3_controls_jpeg_thumbnail_size_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_led_transmit_t;

typedef struct {
    guzzi_camera3_float_t v;
} guzzi_camera3_controls_lens_aperture_t;

typedef struct {
    guzzi_camera3_float_t v;
} guzzi_camera3_controls_lens_filter_density_t;

typedef struct {
    guzzi_camera3_float_t v;
} guzzi_camera3_controls_lens_focal_length_t;

typedef struct {
    guzzi_camera3_float_t v;
} guzzi_camera3_controls_lens_focus_distance_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_lens_optical_stabilization_mode_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_noise_reduction_mode_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_noise_reduction_strength_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_controls_request_frame_count_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_controls_request_id_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_CONTROLS_REQUEST_INPUT_STREAMS_SIZE];
} guzzi_camera3_controls_request_input_streams_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_request_metadata_mode_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_CONTROLS_REQUEST_OUTPUT_STREAMS_SIZE];
} guzzi_camera3_controls_request_output_streams_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_request_type_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_CONTROLS_SCALER_CROP_REGION_SIZE];
} guzzi_camera3_controls_scaler_crop_region_t;

typedef struct {
    guzzi_camera3_int64_t v;
} guzzi_camera3_controls_sensor_exposure_time_t;

typedef struct {
    guzzi_camera3_int64_t v;
} guzzi_camera3_controls_sensor_frame_duration_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_controls_sensor_sensitivity_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_shading_mode_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_shading_strength_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_statistics_face_detect_mode_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_statistics_histogram_mode_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_statistics_lens_shading_map_mode_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_statistics_sharpness_map_mode_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_float_t v[GUZZI_CAMERA3_CONTROLS_TONEMAP_CURVE_BLUE_SIZE];
} guzzi_camera3_controls_tonemap_curve_blue_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_float_t v[GUZZI_CAMERA3_CONTROLS_TONEMAP_CURVE_GREEN_SIZE];
} guzzi_camera3_controls_tonemap_curve_green_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_float_t v[GUZZI_CAMERA3_CONTROLS_TONEMAP_CURVE_RED_SIZE];
} guzzi_camera3_controls_tonemap_curve_red_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_tonemap_mode_t;

/*
 * ****************************************************************************
 * * dynamic
 * ****************************************************************************
 */
typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_dynamic_black_level_lock_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_float_t v[GUZZI_CAMERA3_DYNAMIC_COLOR_CORRECTION_GAINS_SIZE];
} guzzi_camera3_dynamic_color_correction_gains_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_rational_t v[GUZZI_CAMERA3_DYNAMIC_COLOR_CORRECTION_TRANSFORM_SIZE];
} guzzi_camera3_dynamic_color_correction_transform_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_dynamic_control_ae_precapture_id_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_DYNAMIC_CONTROL_AE_REGIONS_SIZE];
} guzzi_camera3_dynamic_control_ae_regions_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_dynamic_control_ae_state_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_dynamic_control_af_mode_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_DYNAMIC_CONTROL_AF_REGIONS_SIZE];
} guzzi_camera3_dynamic_control_af_regions_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_dynamic_control_af_state_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_dynamic_control_af_trigger_id_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_dynamic_control_awb_mode_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_DYNAMIC_CONTROL_AWB_REGIONS_SIZE];
} guzzi_camera3_dynamic_control_awb_regions_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_dynamic_control_awb_state_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_dynamic_control_mode_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_dynamic_edge_mode_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_dynamic_flash_firing_power_t;

typedef struct {
    guzzi_camera3_int64_t v;
} guzzi_camera3_dynamic_flash_firing_time_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_dynamic_flash_mode_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_dynamic_flash_state_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_dynamic_hot_pixel_mode_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_double_t v[GUZZI_CAMERA3_DYNAMIC_JPEG_GPS_COORDINATES_SIZE];
} guzzi_camera3_dynamic_jpeg_gps_coordinates_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_dynamic_jpeg_gps_processing_method_t;

typedef struct {
    guzzi_camera3_int64_t v;
} guzzi_camera3_dynamic_jpeg_gps_timestamp_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_dynamic_jpeg_orientation_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_dynamic_jpeg_quality_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_dynamic_jpeg_size_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_dynamic_jpeg_thumbnail_quality_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_DYNAMIC_JPEG_THUMBNAIL_SIZE_SIZE];
} guzzi_camera3_dynamic_jpeg_thumbnail_size_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_dynamic_led_transmit_t;

typedef struct {
    guzzi_camera3_float_t v;
} guzzi_camera3_dynamic_lens_aperture_t;

typedef struct {
    guzzi_camera3_float_t v;
} guzzi_camera3_dynamic_lens_filter_density_t;

typedef struct {
    guzzi_camera3_float_t v;
} guzzi_camera3_dynamic_lens_focal_length_t;

typedef struct {
    guzzi_camera3_float_t v;
} guzzi_camera3_dynamic_lens_focus_distance_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_float_t v[GUZZI_CAMERA3_DYNAMIC_LENS_FOCUS_RANGE_SIZE];
} guzzi_camera3_dynamic_lens_focus_range_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_dynamic_lens_optical_stabilization_mode_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_dynamic_lens_state_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_dynamic_noise_reduction_mode_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_dynamic_quirks_partial_result_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_dynamic_request_frame_count_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_dynamic_request_id_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_dynamic_request_metadata_mode_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_DYNAMIC_REQUEST_OUTPUT_STREAMS_SIZE];
} guzzi_camera3_dynamic_request_output_streams_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_DYNAMIC_SCALER_CROP_REGION_SIZE];
} guzzi_camera3_dynamic_scaler_crop_region_t;

typedef struct {
    guzzi_camera3_int64_t v;
} guzzi_camera3_dynamic_sensor_exposure_time_t;

typedef struct {
    guzzi_camera3_int64_t v;
} guzzi_camera3_dynamic_sensor_frame_duration_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_dynamic_sensor_sensitivity_t;

typedef struct {
    guzzi_camera3_float_t v;
} guzzi_camera3_dynamic_sensor_temperature_t;

typedef struct {
    guzzi_camera3_int64_t v;
} guzzi_camera3_dynamic_sensor_timestamp_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_dynamic_shading_mode_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_dynamic_statistics_face_detect_mode_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_DYNAMIC_STATISTICS_FACE_IDS_SIZE];
} guzzi_camera3_dynamic_statistics_face_ids_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_DYNAMIC_STATISTICS_FACE_LANDMARKS_SIZE];
} guzzi_camera3_dynamic_statistics_face_landmarks_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_DYNAMIC_STATISTICS_FACE_RECTANGLES_SIZE];
} guzzi_camera3_dynamic_statistics_face_rectangles_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_byte_t v[GUZZI_CAMERA3_DYNAMIC_STATISTICS_FACE_SCORES_SIZE];
} guzzi_camera3_dynamic_statistics_face_scores_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_DYNAMIC_STATISTICS_HISTOGRAM_SIZE];
} guzzi_camera3_dynamic_statistics_histogram_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_dynamic_statistics_histogram_mode_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_int32_t dim_size_3;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_float_t v[GUZZI_CAMERA3_DYNAMIC_STATISTICS_LENS_SHADING_MAP_SIZE];
} guzzi_camera3_dynamic_statistics_lens_shading_map_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_float_t v[GUZZI_CAMERA3_DYNAMIC_STATISTICS_PREDICTED_COLOR_GAINS_SIZE];
} guzzi_camera3_dynamic_statistics_predicted_color_gains_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_rational_t v[GUZZI_CAMERA3_DYNAMIC_STATISTICS_PREDICTED_COLOR_TRANSFORM_SIZE];
} guzzi_camera3_dynamic_statistics_predicted_color_transform_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_dynamic_statistics_scene_flicker_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_int32_t dim_size_3;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_DYNAMIC_STATISTICS_SHARPNESS_MAP_SIZE];
} guzzi_camera3_dynamic_statistics_sharpness_map_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_dynamic_statistics_sharpness_map_mode_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_float_t v[GUZZI_CAMERA3_DYNAMIC_TONEMAP_CURVE_BLUE_SIZE];
} guzzi_camera3_dynamic_tonemap_curve_blue_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_float_t v[GUZZI_CAMERA3_DYNAMIC_TONEMAP_CURVE_GREEN_SIZE];
} guzzi_camera3_dynamic_tonemap_curve_green_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_float_t v[GUZZI_CAMERA3_DYNAMIC_TONEMAP_CURVE_RED_SIZE];
} guzzi_camera3_dynamic_tonemap_curve_red_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_dynamic_tonemap_mode_t;

/*
 * ****************************************************************************
 * * static
 * ****************************************************************************
 */
typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_byte_t v[GUZZI_CAMERA3_STATIC_CONTROL_AE_AVAILABLE_ANTIBANDING_MODES_SIZE];
} guzzi_camera3_static_control_ae_available_antibanding_modes_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_byte_t v[GUZZI_CAMERA3_STATIC_CONTROL_AE_AVAILABLE_MODES_SIZE];
} guzzi_camera3_static_control_ae_available_modes_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_STATIC_CONTROL_AE_AVAILABLE_TARGET_FPS_RANGES_SIZE];
} guzzi_camera3_static_control_ae_available_target_fps_ranges_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_STATIC_CONTROL_AE_COMPENSATION_RANGE_SIZE];
} guzzi_camera3_static_control_ae_compensation_range_t;

typedef struct {
    guzzi_camera3_rational_t v;
} guzzi_camera3_static_control_ae_compensation_step_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_byte_t v[GUZZI_CAMERA3_STATIC_CONTROL_AF_AVAILABLE_MODES_SIZE];
} guzzi_camera3_static_control_af_available_modes_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_byte_t v[GUZZI_CAMERA3_STATIC_CONTROL_AVAILABLE_EFFECTS_SIZE];
} guzzi_camera3_static_control_available_effects_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_byte_t v[GUZZI_CAMERA3_STATIC_CONTROL_AVAILABLE_SCENE_MODES_SIZE];
} guzzi_camera3_static_control_available_scene_modes_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_byte_t v[GUZZI_CAMERA3_STATIC_CONTROL_AVAILABLE_VIDEO_STABILIZATION_MODES_SIZE];
} guzzi_camera3_static_control_available_video_stabilization_modes_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_byte_t v[GUZZI_CAMERA3_STATIC_CONTROL_AWB_AVAILABLE_MODES_SIZE];
} guzzi_camera3_static_control_awb_available_modes_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_static_control_max_regions_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_byte_t v[GUZZI_CAMERA3_STATIC_CONTROL_SCENE_MODE_OVERRIDES_SIZE];
} guzzi_camera3_static_control_scene_mode_overrides_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_static_flash_color_temperature_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_static_flash_info_available_t;

typedef struct {
    guzzi_camera3_int64_t v;
} guzzi_camera3_static_flash_info_charge_duration_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_static_flash_max_energy_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_STATIC_HOT_PIXEL_INFO_MAP_SIZE];
} guzzi_camera3_static_hot_pixel_info_map_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_static_info_supported_hardware_level_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_STATIC_JPEG_AVAILABLE_THUMBNAIL_SIZES_SIZE];
} guzzi_camera3_static_jpeg_available_thumbnail_sizes_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_static_jpeg_max_size_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_byte_t v[GUZZI_CAMERA3_STATIC_LED_AVAILABLE_LEDS_SIZE];
} guzzi_camera3_static_led_available_leds_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_static_lens_facing_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_float_t v[GUZZI_CAMERA3_STATIC_LENS_INFO_AVAILABLE_APERTURES_SIZE];
} guzzi_camera3_static_lens_info_available_apertures_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_float_t v[GUZZI_CAMERA3_STATIC_LENS_INFO_AVAILABLE_FILTER_DENSITIES_SIZE];
} guzzi_camera3_static_lens_info_available_filter_densities_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_float_t v[GUZZI_CAMERA3_STATIC_LENS_INFO_AVAILABLE_FOCAL_LENGTHS_SIZE];
} guzzi_camera3_static_lens_info_available_focal_lengths_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_byte_t v[GUZZI_CAMERA3_STATIC_LENS_INFO_AVAILABLE_OPTICAL_STABILIZATION_SIZE];
} guzzi_camera3_static_lens_info_available_optical_stabilization_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_int32_t dim_size_3;
    guzzi_camera3_int32_t dim_size_4;
    guzzi_camera3_float_t v[GUZZI_CAMERA3_STATIC_LENS_INFO_GEOMETRIC_CORRECTION_MAP_SIZE];
} guzzi_camera3_static_lens_info_geometric_correction_map_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_STATIC_LENS_INFO_GEOMETRIC_CORRECTION_MAP_SIZE_SIZE];
} guzzi_camera3_static_lens_info_geometric_correction_map_size_t;

typedef struct {
    guzzi_camera3_float_t v;
} guzzi_camera3_static_lens_info_hyperfocal_distance_t;

typedef struct {
    guzzi_camera3_float_t v;
} guzzi_camera3_static_lens_info_minimum_focus_distance_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_STATIC_LENS_INFO_SHADING_MAP_SIZE_SIZE];
} guzzi_camera3_static_lens_info_shading_map_size_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_float_t v[GUZZI_CAMERA3_STATIC_LENS_OPTICAL_AXIS_ANGLE_SIZE];
} guzzi_camera3_static_lens_optical_axis_angle_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_float_t v[GUZZI_CAMERA3_STATIC_LENS_POSITION_SIZE];
} guzzi_camera3_static_lens_position_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_static_quirks_metering_crop_region_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_static_quirks_trigger_af_with_auto_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_static_quirks_use_partial_result_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_static_quirks_use_zsl_format_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_STATIC_REQUEST_MAX_NUM_OUTPUT_STREAMS_SIZE];
} guzzi_camera3_static_request_max_num_output_streams_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_STATIC_REQUEST_MAX_NUM_REPROCESS_STREAMS_SIZE];
} guzzi_camera3_static_request_max_num_reprocess_streams_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_STATIC_SCALER_AVAILABLE_FORMATS_SIZE];
} guzzi_camera3_static_scaler_available_formats_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int64_t v[GUZZI_CAMERA3_STATIC_SCALER_AVAILABLE_JPEG_MIN_DURATIONS_SIZE];
} guzzi_camera3_static_scaler_available_jpeg_min_durations_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_STATIC_SCALER_AVAILABLE_JPEG_SIZES_SIZE];
} guzzi_camera3_static_scaler_available_jpeg_sizes_t;

typedef struct {
    guzzi_camera3_float_t v;
} guzzi_camera3_static_scaler_available_max_digital_zoom_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int64_t v[GUZZI_CAMERA3_STATIC_SCALER_AVAILABLE_PROCESSED_MIN_DURATIONS_SIZE];
} guzzi_camera3_static_scaler_available_processed_min_durations_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_STATIC_SCALER_AVAILABLE_PROCESSED_SIZES_SIZE];
} guzzi_camera3_static_scaler_available_processed_sizes_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int64_t v[GUZZI_CAMERA3_STATIC_SCALER_AVAILABLE_RAW_MIN_DURATIONS_SIZE];
} guzzi_camera3_static_scaler_available_raw_min_durations_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dim_size_2;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_STATIC_SCALER_AVAILABLE_RAW_SIZES_SIZE];
} guzzi_camera3_static_scaler_available_raw_sizes_t;

typedef struct {
    guzzi_camera3_rational_t v;
} guzzi_camera3_static_sensor_base_gain_factor_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_STATIC_SENSOR_BLACK_LEVEL_PATTERN_SIZE];
} guzzi_camera3_static_sensor_black_level_pattern_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_rational_t v[GUZZI_CAMERA3_STATIC_SENSOR_CALIBRATION_TRANSFORM1_SIZE];
} guzzi_camera3_static_sensor_calibration_transform1_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_rational_t v[GUZZI_CAMERA3_STATIC_SENSOR_CALIBRATION_TRANSFORM2_SIZE];
} guzzi_camera3_static_sensor_calibration_transform2_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_rational_t v[GUZZI_CAMERA3_STATIC_SENSOR_COLOR_TRANSFORM1_SIZE];
} guzzi_camera3_static_sensor_color_transform1_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_rational_t v[GUZZI_CAMERA3_STATIC_SENSOR_COLOR_TRANSFORM2_SIZE];
} guzzi_camera3_static_sensor_color_transform2_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_rational_t v[GUZZI_CAMERA3_STATIC_SENSOR_FORWARD_MATRIX1_SIZE];
} guzzi_camera3_static_sensor_forward_matrix1_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_rational_t v[GUZZI_CAMERA3_STATIC_SENSOR_FORWARD_MATRIX2_SIZE];
} guzzi_camera3_static_sensor_forward_matrix2_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_STATIC_SENSOR_INFO_ACTIVE_ARRAY_SIZE_SIZE];
} guzzi_camera3_static_sensor_info_active_array_size_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_static_sensor_info_color_filter_arrangement_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int64_t v[GUZZI_CAMERA3_STATIC_SENSOR_INFO_EXPOSURE_TIME_RANGE_SIZE];
} guzzi_camera3_static_sensor_info_exposure_time_range_t;

typedef struct {
    guzzi_camera3_int64_t v;
} guzzi_camera3_static_sensor_info_max_frame_duration_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_float_t v[GUZZI_CAMERA3_STATIC_SENSOR_INFO_PHYSICAL_SIZE_SIZE];
} guzzi_camera3_static_sensor_info_physical_size_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_STATIC_SENSOR_INFO_PIXEL_ARRAY_SIZE_SIZE];
} guzzi_camera3_static_sensor_info_pixel_array_size_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_STATIC_SENSOR_INFO_SENSITIVITY_RANGE_SIZE];
} guzzi_camera3_static_sensor_info_sensitivity_range_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_static_sensor_info_white_level_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_static_sensor_max_analog_sensitivity_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_float_t v[GUZZI_CAMERA3_STATIC_SENSOR_NOISE_MODEL_COEFFICIENTS_SIZE];
} guzzi_camera3_static_sensor_noise_model_coefficients_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_static_sensor_orientation_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_static_sensor_reference_illuminant1_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_static_sensor_reference_illuminant2_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_byte_t v[GUZZI_CAMERA3_STATIC_STATISTICS_INFO_AVAILABLE_FACE_DETECT_MODES_SIZE];
} guzzi_camera3_static_statistics_info_available_face_detect_modes_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_static_statistics_info_histogram_bucket_count_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_static_statistics_info_max_face_count_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_static_statistics_info_max_histogram_count_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_static_statistics_info_max_sharpness_map_value_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_STATIC_STATISTICS_INFO_SHARPNESS_MAP_SIZE_SIZE];
} guzzi_camera3_static_statistics_info_sharpness_map_size_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_static_tonemap_max_curve_points_t;

/*
 * ****************************************************************************
 * ** Guzzi specific **********************************************************
 * ****************************************************************************
 */
/*
 * ****************************************************************************
 * * controls
 * ****************************************************************************
 */
typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_CONTROLS_CAPTURE_REQUEST_ACTIVE_SIZE];
} guzzi_camera3_controls_capture_request_active_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_controls_capture_request_frame_number_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_controls_capture_request_guzzi_fr_id_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_CONTROLS_STREAM_CONFIG_FORMAT_SIZE];
} guzzi_camera3_controls_stream_config_format_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_CONTROLS_STREAM_CONFIG_HEIGHT_SIZE];
} guzzi_camera3_controls_stream_config_height_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_CONTROLS_STREAM_CONFIG_ID_SIZE];
} guzzi_camera3_controls_stream_config_id_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_CONTROLS_STREAM_CONFIG_TYPE_SIZE];
} guzzi_camera3_controls_stream_config_type_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_CONTROLS_STREAM_CONFIG_WIDTH_SIZE];
} guzzi_camera3_controls_stream_config_width_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_controls_z_custom_capture_number_shots_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_z_custom_capture_mode_selection_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_controls_z_custom_crtl_brigthness_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_controls_z_custom_crtl_contrast_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_controls_z_custom_crtl_hue_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_controls_z_custom_crtl_saturation_t;

typedef struct {
    guzzi_camera3_int32_t dim_size_1;
    guzzi_camera3_int32_t dummy;
    guzzi_camera3_int32_t v[GUZZI_CAMERA3_CONTROLS_Z_CUSTOM_EXPOSURE_BRACKETING_SEQUENCE_SIZE];
} guzzi_camera3_controls_z_custom_exposure_bracketing_sequence_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_z_custom_usecase_selection_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_z_exposure_merger_weight_t;

typedef struct {
    guzzi_camera3_int32_t v;
} guzzi_camera3_controls_z_wb_manual_temperature_t;

typedef struct {
    guzzi_camera3_byte_t v;
} guzzi_camera3_controls_z_white_balance_merger_weight_t;

