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
* @file camera3_metadata_index.h
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

typedef enum {
    GUZZI_CAMERA3_INDEX_CONTROLS_BLACK_LEVEL_LOCK, /* guzzi_camera3_controls_black_level_lock_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_COLOR_CORRECTION_GAINS, /* guzzi_camera3_controls_color_correction_gains_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_COLOR_CORRECTION_MODE, /* guzzi_camera3_controls_color_correction_mode_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_COLOR_CORRECTION_TRANSFORM, /* guzzi_camera3_controls_color_correction_transform_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_CONTROL_AE_ANTIBANDING_MODE, /* guzzi_camera3_controls_control_ae_antibanding_mode_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_CONTROL_AE_EXPOSURE_COMPENSATION, /* guzzi_camera3_controls_control_ae_exposure_compensation_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_CONTROL_AE_LOCK, /* guzzi_camera3_controls_control_ae_lock_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_CONTROL_AE_MODE, /* guzzi_camera3_controls_control_ae_mode_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_CONTROL_AE_PRECAPTURE_TRIGGER, /* guzzi_camera3_controls_control_ae_precapture_trigger_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_CONTROL_AE_REGIONS, /* guzzi_camera3_controls_control_ae_regions_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_CONTROL_AE_TARGET_FPS_RANGE, /* guzzi_camera3_controls_control_ae_target_fps_range_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_CONTROL_AF_MODE, /* guzzi_camera3_controls_control_af_mode_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_CONTROL_AF_REGIONS, /* guzzi_camera3_controls_control_af_regions_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_CONTROL_AF_TRIGGER, /* guzzi_camera3_controls_control_af_trigger_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_CONTROL_AWB_LOCK, /* guzzi_camera3_controls_control_awb_lock_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_CONTROL_AWB_MODE, /* guzzi_camera3_controls_control_awb_mode_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_CONTROL_AWB_REGIONS, /* guzzi_camera3_controls_control_awb_regions_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_CONTROL_CAPTURE_INTENT, /* guzzi_camera3_controls_control_capture_intent_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_CONTROL_EFFECT_MODE, /* guzzi_camera3_controls_control_effect_mode_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_CONTROL_MODE, /* guzzi_camera3_controls_control_mode_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_CONTROL_SCENE_MODE, /* guzzi_camera3_controls_control_scene_mode_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_CONTROL_VIDEO_STABILIZATION_MODE, /* guzzi_camera3_controls_control_video_stabilization_mode_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_DEMOSAIC_MODE, /* guzzi_camera3_controls_demosaic_mode_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_EDGE_MODE, /* guzzi_camera3_controls_edge_mode_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_EDGE_STRENGTH, /* guzzi_camera3_controls_edge_strength_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_FLASH_FIRING_POWER, /* guzzi_camera3_controls_flash_firing_power_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_FLASH_FIRING_TIME, /* guzzi_camera3_controls_flash_firing_time_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_FLASH_MODE, /* guzzi_camera3_controls_flash_mode_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_GEOMETRIC_MODE, /* guzzi_camera3_controls_geometric_mode_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_GEOMETRIC_STRENGTH, /* guzzi_camera3_controls_geometric_strength_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_HOT_PIXEL_MODE, /* guzzi_camera3_controls_hot_pixel_mode_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_JPEG_GPS_COORDINATES, /* guzzi_camera3_controls_jpeg_gps_coordinates_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_JPEG_GPS_PROCESSING_METHOD, /* guzzi_camera3_controls_jpeg_gps_processing_method_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_JPEG_GPS_TIMESTAMP, /* guzzi_camera3_controls_jpeg_gps_timestamp_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_JPEG_ORIENTATION, /* guzzi_camera3_controls_jpeg_orientation_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_JPEG_QUALITY, /* guzzi_camera3_controls_jpeg_quality_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_JPEG_THUMBNAIL_QUALITY, /* guzzi_camera3_controls_jpeg_thumbnail_quality_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_JPEG_THUMBNAIL_SIZE, /* guzzi_camera3_controls_jpeg_thumbnail_size_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_LED_TRANSMIT, /* guzzi_camera3_controls_led_transmit_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_LENS_APERTURE, /* guzzi_camera3_controls_lens_aperture_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_LENS_FILTER_DENSITY, /* guzzi_camera3_controls_lens_filter_density_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_LENS_FOCAL_LENGTH, /* guzzi_camera3_controls_lens_focal_length_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_LENS_FOCUS_DISTANCE, /* guzzi_camera3_controls_lens_focus_distance_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_LENS_OPTICAL_STABILIZATION_MODE, /* guzzi_camera3_controls_lens_optical_stabilization_mode_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_NOISE_REDUCTION_MODE, /* guzzi_camera3_controls_noise_reduction_mode_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_NOISE_REDUCTION_STRENGTH, /* guzzi_camera3_controls_noise_reduction_strength_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_REQUEST_FRAME_COUNT, /* guzzi_camera3_controls_request_frame_count_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_REQUEST_ID, /* guzzi_camera3_controls_request_id_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_REQUEST_INPUT_STREAMS, /* guzzi_camera3_controls_request_input_streams_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_REQUEST_METADATA_MODE, /* guzzi_camera3_controls_request_metadata_mode_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_REQUEST_OUTPUT_STREAMS, /* guzzi_camera3_controls_request_output_streams_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_REQUEST_TYPE, /* guzzi_camera3_controls_request_type_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_SCALER_CROP_REGION, /* guzzi_camera3_controls_scaler_crop_region_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_SENSOR_EXPOSURE_TIME, /* guzzi_camera3_controls_sensor_exposure_time_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_SENSOR_FRAME_DURATION, /* guzzi_camera3_controls_sensor_frame_duration_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_SENSOR_SENSITIVITY, /* guzzi_camera3_controls_sensor_sensitivity_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_SHADING_MODE, /* guzzi_camera3_controls_shading_mode_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_SHADING_STRENGTH, /* guzzi_camera3_controls_shading_strength_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_STATISTICS_FACE_DETECT_MODE, /* guzzi_camera3_controls_statistics_face_detect_mode_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_STATISTICS_HISTOGRAM_MODE, /* guzzi_camera3_controls_statistics_histogram_mode_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_STATISTICS_LENS_SHADING_MAP_MODE, /* guzzi_camera3_controls_statistics_lens_shading_map_mode_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_STATISTICS_SHARPNESS_MAP_MODE, /* guzzi_camera3_controls_statistics_sharpness_map_mode_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_TONEMAP_CURVE_BLUE, /* guzzi_camera3_controls_tonemap_curve_blue_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_TONEMAP_CURVE_GREEN, /* guzzi_camera3_controls_tonemap_curve_green_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_TONEMAP_CURVE_RED, /* guzzi_camera3_controls_tonemap_curve_red_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_TONEMAP_MODE, /* guzzi_camera3_controls_tonemap_mode_t */

    GUZZI_CAMERA3_INDEX_DYNAMIC_BLACK_LEVEL_LOCK, /* guzzi_camera3_dynamic_black_level_lock_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_COLOR_CORRECTION_GAINS, /* guzzi_camera3_dynamic_color_correction_gains_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_COLOR_CORRECTION_TRANSFORM, /* guzzi_camera3_dynamic_color_correction_transform_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_CONTROL_AE_PRECAPTURE_ID, /* guzzi_camera3_dynamic_control_ae_precapture_id_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_CONTROL_AE_REGIONS, /* guzzi_camera3_dynamic_control_ae_regions_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_CONTROL_AE_STATE, /* guzzi_camera3_dynamic_control_ae_state_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_CONTROL_AF_MODE, /* guzzi_camera3_dynamic_control_af_mode_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_CONTROL_AF_REGIONS, /* guzzi_camera3_dynamic_control_af_regions_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_CONTROL_AF_STATE, /* guzzi_camera3_dynamic_control_af_state_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_CONTROL_AF_TRIGGER_ID, /* guzzi_camera3_dynamic_control_af_trigger_id_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_CONTROL_AWB_MODE, /* guzzi_camera3_dynamic_control_awb_mode_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_CONTROL_AWB_REGIONS, /* guzzi_camera3_dynamic_control_awb_regions_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_CONTROL_AWB_STATE, /* guzzi_camera3_dynamic_control_awb_state_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_CONTROL_MODE, /* guzzi_camera3_dynamic_control_mode_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_EDGE_MODE, /* guzzi_camera3_dynamic_edge_mode_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_FLASH_FIRING_POWER, /* guzzi_camera3_dynamic_flash_firing_power_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_FLASH_FIRING_TIME, /* guzzi_camera3_dynamic_flash_firing_time_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_FLASH_MODE, /* guzzi_camera3_dynamic_flash_mode_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_FLASH_STATE, /* guzzi_camera3_dynamic_flash_state_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_HOT_PIXEL_MODE, /* guzzi_camera3_dynamic_hot_pixel_mode_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_JPEG_GPS_COORDINATES, /* guzzi_camera3_dynamic_jpeg_gps_coordinates_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_JPEG_GPS_PROCESSING_METHOD, /* guzzi_camera3_dynamic_jpeg_gps_processing_method_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_JPEG_GPS_TIMESTAMP, /* guzzi_camera3_dynamic_jpeg_gps_timestamp_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_JPEG_ORIENTATION, /* guzzi_camera3_dynamic_jpeg_orientation_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_JPEG_QUALITY, /* guzzi_camera3_dynamic_jpeg_quality_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_JPEG_SIZE, /* guzzi_camera3_dynamic_jpeg_size_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_JPEG_THUMBNAIL_QUALITY, /* guzzi_camera3_dynamic_jpeg_thumbnail_quality_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_JPEG_THUMBNAIL_SIZE, /* guzzi_camera3_dynamic_jpeg_thumbnail_size_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_LED_TRANSMIT, /* guzzi_camera3_dynamic_led_transmit_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_LENS_APERTURE, /* guzzi_camera3_dynamic_lens_aperture_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_LENS_FILTER_DENSITY, /* guzzi_camera3_dynamic_lens_filter_density_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_LENS_FOCAL_LENGTH, /* guzzi_camera3_dynamic_lens_focal_length_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_LENS_FOCUS_DISTANCE, /* guzzi_camera3_dynamic_lens_focus_distance_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_LENS_FOCUS_RANGE, /* guzzi_camera3_dynamic_lens_focus_range_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_LENS_OPTICAL_STABILIZATION_MODE, /* guzzi_camera3_dynamic_lens_optical_stabilization_mode_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_LENS_STATE, /* guzzi_camera3_dynamic_lens_state_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_NOISE_REDUCTION_MODE, /* guzzi_camera3_dynamic_noise_reduction_mode_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_QUIRKS_PARTIAL_RESULT, /* guzzi_camera3_dynamic_quirks_partial_result_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_REQUEST_FRAME_COUNT, /* guzzi_camera3_dynamic_request_frame_count_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_REQUEST_ID, /* guzzi_camera3_dynamic_request_id_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_REQUEST_METADATA_MODE, /* guzzi_camera3_dynamic_request_metadata_mode_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_REQUEST_OUTPUT_STREAMS, /* guzzi_camera3_dynamic_request_output_streams_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_SCALER_CROP_REGION, /* guzzi_camera3_dynamic_scaler_crop_region_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_SENSOR_EXPOSURE_TIME, /* guzzi_camera3_dynamic_sensor_exposure_time_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_SENSOR_FRAME_DURATION, /* guzzi_camera3_dynamic_sensor_frame_duration_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_SENSOR_SENSITIVITY, /* guzzi_camera3_dynamic_sensor_sensitivity_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_SENSOR_TEMPERATURE, /* guzzi_camera3_dynamic_sensor_temperature_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_SENSOR_TIMESTAMP, /* guzzi_camera3_dynamic_sensor_timestamp_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_SHADING_MODE, /* guzzi_camera3_dynamic_shading_mode_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_STATISTICS_FACE_DETECT_MODE, /* guzzi_camera3_dynamic_statistics_face_detect_mode_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_STATISTICS_FACE_IDS, /* guzzi_camera3_dynamic_statistics_face_ids_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_STATISTICS_FACE_LANDMARKS, /* guzzi_camera3_dynamic_statistics_face_landmarks_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_STATISTICS_FACE_RECTANGLES, /* guzzi_camera3_dynamic_statistics_face_rectangles_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_STATISTICS_FACE_SCORES, /* guzzi_camera3_dynamic_statistics_face_scores_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_STATISTICS_HISTOGRAM, /* guzzi_camera3_dynamic_statistics_histogram_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_STATISTICS_HISTOGRAM_MODE, /* guzzi_camera3_dynamic_statistics_histogram_mode_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_STATISTICS_LENS_SHADING_MAP, /* guzzi_camera3_dynamic_statistics_lens_shading_map_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_STATISTICS_PREDICTED_COLOR_GAINS, /* guzzi_camera3_dynamic_statistics_predicted_color_gains_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_STATISTICS_PREDICTED_COLOR_TRANSFORM, /* guzzi_camera3_dynamic_statistics_predicted_color_transform_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_STATISTICS_SCENE_FLICKER, /* guzzi_camera3_dynamic_statistics_scene_flicker_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_STATISTICS_SHARPNESS_MAP, /* guzzi_camera3_dynamic_statistics_sharpness_map_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_STATISTICS_SHARPNESS_MAP_MODE, /* guzzi_camera3_dynamic_statistics_sharpness_map_mode_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_TONEMAP_CURVE_BLUE, /* guzzi_camera3_dynamic_tonemap_curve_blue_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_TONEMAP_CURVE_GREEN, /* guzzi_camera3_dynamic_tonemap_curve_green_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_TONEMAP_CURVE_RED, /* guzzi_camera3_dynamic_tonemap_curve_red_t */
    GUZZI_CAMERA3_INDEX_DYNAMIC_TONEMAP_MODE, /* guzzi_camera3_dynamic_tonemap_mode_t */

    GUZZI_CAMERA3_INDEX_STATIC_CONTROL_AE_AVAILABLE_ANTIBANDING_MODES, /* guzzi_camera3_static_control_ae_available_antibanding_modes_t */
    GUZZI_CAMERA3_INDEX_STATIC_CONTROL_AE_AVAILABLE_MODES, /* guzzi_camera3_static_control_ae_available_modes_t */
    GUZZI_CAMERA3_INDEX_STATIC_CONTROL_AE_AVAILABLE_TARGET_FPS_RANGES, /* guzzi_camera3_static_control_ae_available_target_fps_ranges_t */
    GUZZI_CAMERA3_INDEX_STATIC_CONTROL_AE_COMPENSATION_RANGE, /* guzzi_camera3_static_control_ae_compensation_range_t */
    GUZZI_CAMERA3_INDEX_STATIC_CONTROL_AE_COMPENSATION_STEP, /* guzzi_camera3_static_control_ae_compensation_step_t */
    GUZZI_CAMERA3_INDEX_STATIC_CONTROL_AF_AVAILABLE_MODES, /* guzzi_camera3_static_control_af_available_modes_t */
    GUZZI_CAMERA3_INDEX_STATIC_CONTROL_AVAILABLE_EFFECTS, /* guzzi_camera3_static_control_available_effects_t */
    GUZZI_CAMERA3_INDEX_STATIC_CONTROL_AVAILABLE_SCENE_MODES, /* guzzi_camera3_static_control_available_scene_modes_t */
    GUZZI_CAMERA3_INDEX_STATIC_CONTROL_AVAILABLE_VIDEO_STABILIZATION_MODES, /* guzzi_camera3_static_control_available_video_stabilization_modes_t */
    GUZZI_CAMERA3_INDEX_STATIC_CONTROL_AWB_AVAILABLE_MODES, /* guzzi_camera3_static_control_awb_available_modes_t */
    GUZZI_CAMERA3_INDEX_STATIC_CONTROL_MAX_REGIONS, /* guzzi_camera3_static_control_max_regions_t */
    GUZZI_CAMERA3_INDEX_STATIC_CONTROL_SCENE_MODE_OVERRIDES, /* guzzi_camera3_static_control_scene_mode_overrides_t */
    GUZZI_CAMERA3_INDEX_STATIC_FLASH_COLOR_TEMPERATURE, /* guzzi_camera3_static_flash_color_temperature_t */
    GUZZI_CAMERA3_INDEX_STATIC_FLASH_INFO_AVAILABLE, /* guzzi_camera3_static_flash_info_available_t */
    GUZZI_CAMERA3_INDEX_STATIC_FLASH_INFO_CHARGE_DURATION, /* guzzi_camera3_static_flash_info_charge_duration_t */
    GUZZI_CAMERA3_INDEX_STATIC_FLASH_MAX_ENERGY, /* guzzi_camera3_static_flash_max_energy_t */
    GUZZI_CAMERA3_INDEX_STATIC_HOT_PIXEL_INFO_MAP, /* guzzi_camera3_static_hot_pixel_info_map_t */
    GUZZI_CAMERA3_INDEX_STATIC_INFO_SUPPORTED_HARDWARE_LEVEL, /* guzzi_camera3_static_info_supported_hardware_level_t */
    GUZZI_CAMERA3_INDEX_STATIC_JPEG_AVAILABLE_THUMBNAIL_SIZES, /* guzzi_camera3_static_jpeg_available_thumbnail_sizes_t */
    GUZZI_CAMERA3_INDEX_STATIC_JPEG_MAX_SIZE, /* guzzi_camera3_static_jpeg_max_size_t */
    GUZZI_CAMERA3_INDEX_STATIC_LED_AVAILABLE_LEDS, /* guzzi_camera3_static_led_available_leds_t */
    GUZZI_CAMERA3_INDEX_STATIC_LENS_FACING, /* guzzi_camera3_static_lens_facing_t */
    GUZZI_CAMERA3_INDEX_STATIC_LENS_INFO_AVAILABLE_APERTURES, /* guzzi_camera3_static_lens_info_available_apertures_t */
    GUZZI_CAMERA3_INDEX_STATIC_LENS_INFO_AVAILABLE_FILTER_DENSITIES, /* guzzi_camera3_static_lens_info_available_filter_densities_t */
    GUZZI_CAMERA3_INDEX_STATIC_LENS_INFO_AVAILABLE_FOCAL_LENGTHS, /* guzzi_camera3_static_lens_info_available_focal_lengths_t */
    GUZZI_CAMERA3_INDEX_STATIC_LENS_INFO_AVAILABLE_OPTICAL_STABILIZATION, /* guzzi_camera3_static_lens_info_available_optical_stabilization_t */
    GUZZI_CAMERA3_INDEX_STATIC_LENS_INFO_GEOMETRIC_CORRECTION_MAP, /* guzzi_camera3_static_lens_info_geometric_correction_map_t */
    GUZZI_CAMERA3_INDEX_STATIC_LENS_INFO_GEOMETRIC_CORRECTION_MAP_SIZE, /* guzzi_camera3_static_lens_info_geometric_correction_map_size_t */
    GUZZI_CAMERA3_INDEX_STATIC_LENS_INFO_HYPERFOCAL_DISTANCE, /* guzzi_camera3_static_lens_info_hyperfocal_distance_t */
    GUZZI_CAMERA3_INDEX_STATIC_LENS_INFO_MINIMUM_FOCUS_DISTANCE, /* guzzi_camera3_static_lens_info_minimum_focus_distance_t */
    GUZZI_CAMERA3_INDEX_STATIC_LENS_INFO_SHADING_MAP_SIZE, /* guzzi_camera3_static_lens_info_shading_map_size_t */
    GUZZI_CAMERA3_INDEX_STATIC_LENS_OPTICAL_AXIS_ANGLE, /* guzzi_camera3_static_lens_optical_axis_angle_t */
    GUZZI_CAMERA3_INDEX_STATIC_LENS_POSITION, /* guzzi_camera3_static_lens_position_t */
    GUZZI_CAMERA3_INDEX_STATIC_QUIRKS_METERING_CROP_REGION, /* guzzi_camera3_static_quirks_metering_crop_region_t */
    GUZZI_CAMERA3_INDEX_STATIC_QUIRKS_TRIGGER_AF_WITH_AUTO, /* guzzi_camera3_static_quirks_trigger_af_with_auto_t */
    GUZZI_CAMERA3_INDEX_STATIC_QUIRKS_USE_PARTIAL_RESULT, /* guzzi_camera3_static_quirks_use_partial_result_t */
    GUZZI_CAMERA3_INDEX_STATIC_QUIRKS_USE_ZSL_FORMAT, /* guzzi_camera3_static_quirks_use_zsl_format_t */
    GUZZI_CAMERA3_INDEX_STATIC_REQUEST_MAX_NUM_OUTPUT_STREAMS, /* guzzi_camera3_static_request_max_num_output_streams_t */
    GUZZI_CAMERA3_INDEX_STATIC_REQUEST_MAX_NUM_REPROCESS_STREAMS, /* guzzi_camera3_static_request_max_num_reprocess_streams_t */
    GUZZI_CAMERA3_INDEX_STATIC_SCALER_AVAILABLE_FORMATS, /* guzzi_camera3_static_scaler_available_formats_t */
    GUZZI_CAMERA3_INDEX_STATIC_SCALER_AVAILABLE_JPEG_MIN_DURATIONS, /* guzzi_camera3_static_scaler_available_jpeg_min_durations_t */
    GUZZI_CAMERA3_INDEX_STATIC_SCALER_AVAILABLE_JPEG_SIZES, /* guzzi_camera3_static_scaler_available_jpeg_sizes_t */
    GUZZI_CAMERA3_INDEX_STATIC_SCALER_AVAILABLE_MAX_DIGITAL_ZOOM, /* guzzi_camera3_static_scaler_available_max_digital_zoom_t */
    GUZZI_CAMERA3_INDEX_STATIC_SCALER_AVAILABLE_PROCESSED_MIN_DURATIONS, /* guzzi_camera3_static_scaler_available_processed_min_durations_t */
    GUZZI_CAMERA3_INDEX_STATIC_SCALER_AVAILABLE_PROCESSED_SIZES, /* guzzi_camera3_static_scaler_available_processed_sizes_t */
    GUZZI_CAMERA3_INDEX_STATIC_SCALER_AVAILABLE_RAW_MIN_DURATIONS, /* guzzi_camera3_static_scaler_available_raw_min_durations_t */
    GUZZI_CAMERA3_INDEX_STATIC_SCALER_AVAILABLE_RAW_SIZES, /* guzzi_camera3_static_scaler_available_raw_sizes_t */
    GUZZI_CAMERA3_INDEX_STATIC_SENSOR_BASE_GAIN_FACTOR, /* guzzi_camera3_static_sensor_base_gain_factor_t */
    GUZZI_CAMERA3_INDEX_STATIC_SENSOR_BLACK_LEVEL_PATTERN, /* guzzi_camera3_static_sensor_black_level_pattern_t */
    GUZZI_CAMERA3_INDEX_STATIC_SENSOR_CALIBRATION_TRANSFORM1, /* guzzi_camera3_static_sensor_calibration_transform1_t */
    GUZZI_CAMERA3_INDEX_STATIC_SENSOR_CALIBRATION_TRANSFORM2, /* guzzi_camera3_static_sensor_calibration_transform2_t */
    GUZZI_CAMERA3_INDEX_STATIC_SENSOR_COLOR_TRANSFORM1, /* guzzi_camera3_static_sensor_color_transform1_t */
    GUZZI_CAMERA3_INDEX_STATIC_SENSOR_COLOR_TRANSFORM2, /* guzzi_camera3_static_sensor_color_transform2_t */
    GUZZI_CAMERA3_INDEX_STATIC_SENSOR_FORWARD_MATRIX1, /* guzzi_camera3_static_sensor_forward_matrix1_t */
    GUZZI_CAMERA3_INDEX_STATIC_SENSOR_FORWARD_MATRIX2, /* guzzi_camera3_static_sensor_forward_matrix2_t */
    GUZZI_CAMERA3_INDEX_STATIC_SENSOR_INFO_ACTIVE_ARRAY_SIZE, /* guzzi_camera3_static_sensor_info_active_array_size_t */
    GUZZI_CAMERA3_INDEX_STATIC_SENSOR_INFO_COLOR_FILTER_ARRANGEMENT, /* guzzi_camera3_static_sensor_info_color_filter_arrangement_t */
    GUZZI_CAMERA3_INDEX_STATIC_SENSOR_INFO_EXPOSURE_TIME_RANGE, /* guzzi_camera3_static_sensor_info_exposure_time_range_t */
    GUZZI_CAMERA3_INDEX_STATIC_SENSOR_INFO_MAX_FRAME_DURATION, /* guzzi_camera3_static_sensor_info_max_frame_duration_t */
    GUZZI_CAMERA3_INDEX_STATIC_SENSOR_INFO_PHYSICAL_SIZE, /* guzzi_camera3_static_sensor_info_physical_size_t */
    GUZZI_CAMERA3_INDEX_STATIC_SENSOR_INFO_PIXEL_ARRAY_SIZE, /* guzzi_camera3_static_sensor_info_pixel_array_size_t */
    GUZZI_CAMERA3_INDEX_STATIC_SENSOR_INFO_SENSITIVITY_RANGE, /* guzzi_camera3_static_sensor_info_sensitivity_range_t */
    GUZZI_CAMERA3_INDEX_STATIC_SENSOR_INFO_WHITE_LEVEL, /* guzzi_camera3_static_sensor_info_white_level_t */
    GUZZI_CAMERA3_INDEX_STATIC_SENSOR_MAX_ANALOG_SENSITIVITY, /* guzzi_camera3_static_sensor_max_analog_sensitivity_t */
    GUZZI_CAMERA3_INDEX_STATIC_SENSOR_NOISE_MODEL_COEFFICIENTS, /* guzzi_camera3_static_sensor_noise_model_coefficients_t */
    GUZZI_CAMERA3_INDEX_STATIC_SENSOR_ORIENTATION, /* guzzi_camera3_static_sensor_orientation_t */
    GUZZI_CAMERA3_INDEX_STATIC_SENSOR_REFERENCE_ILLUMINANT1, /* guzzi_camera3_static_sensor_reference_illuminant1_t */
    GUZZI_CAMERA3_INDEX_STATIC_SENSOR_REFERENCE_ILLUMINANT2, /* guzzi_camera3_static_sensor_reference_illuminant2_t */
    GUZZI_CAMERA3_INDEX_STATIC_STATISTICS_INFO_AVAILABLE_FACE_DETECT_MODES, /* guzzi_camera3_static_statistics_info_available_face_detect_modes_t */
    GUZZI_CAMERA3_INDEX_STATIC_STATISTICS_INFO_HISTOGRAM_BUCKET_COUNT, /* guzzi_camera3_static_statistics_info_histogram_bucket_count_t */
    GUZZI_CAMERA3_INDEX_STATIC_STATISTICS_INFO_MAX_FACE_COUNT, /* guzzi_camera3_static_statistics_info_max_face_count_t */
    GUZZI_CAMERA3_INDEX_STATIC_STATISTICS_INFO_MAX_HISTOGRAM_COUNT, /* guzzi_camera3_static_statistics_info_max_histogram_count_t */
    GUZZI_CAMERA3_INDEX_STATIC_STATISTICS_INFO_MAX_SHARPNESS_MAP_VALUE, /* guzzi_camera3_static_statistics_info_max_sharpness_map_value_t */
    GUZZI_CAMERA3_INDEX_STATIC_STATISTICS_INFO_SHARPNESS_MAP_SIZE, /* guzzi_camera3_static_statistics_info_sharpness_map_size_t */
    GUZZI_CAMERA3_INDEX_STATIC_TONEMAP_MAX_CURVE_POINTS, /* guzzi_camera3_static_tonemap_max_curve_points_t */

    /* Guzzi specific */
    GUZZI_CAMERA3_INDEX_CONTROLS_CAPTURE_REQUEST_ACTIVE, /* guzzi_camera3_controls_capture_request_active_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_CAPTURE_REQUEST_FRAME_NUMBER, /* guzzi_camera3_controls_capture_request_frame_number_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_CAPTURE_REQUEST_GUZZI_FR_ID, /* guzzi_camera3_controls_capture_request_guzzi_fr_id_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_STREAM_CONFIG_FORMAT, /* guzzi_camera3_controls_stream_config_format_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_STREAM_CONFIG_HEIGHT, /* guzzi_camera3_controls_stream_config_height_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_STREAM_CONFIG_ID, /* guzzi_camera3_controls_stream_config_id_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_STREAM_CONFIG_TYPE, /* guzzi_camera3_controls_stream_config_type_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_STREAM_CONFIG_WIDTH, /* guzzi_camera3_controls_stream_config_width_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_Z_CUSTOM_CAPTURE_NUMBER_SHOTS, /* guzzi_camera3_controls_z_custom_capture_number_shots_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_Z_CUSTOM_CAPTURE_MODE_SELECTION, /* guzzi_camera3_controls_z_custom_capture_mode_selection_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_Z_CUSTOM_CRTL_BRIGTHNESS, /* guzzi_camera3_controls_z_custom_crtl_brigthness_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_Z_CUSTOM_CRTL_CONTRAST, /* guzzi_camera3_controls_z_custom_crtl_contrast_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_Z_CUSTOM_CRTL_HUE, /* guzzi_camera3_controls_z_custom_crtl_hue_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_Z_CUSTOM_CRTL_SATURATION, /* guzzi_camera3_controls_z_custom_crtl_saturation_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_Z_CUSTOM_EXPOSURE_BRACKETING_SEQUENCE, /* guzzi_camera3_controls_z_custom_exposure_bracketing_sequence_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_Z_CUSTOM_USECASE_SELECTION, /* guzzi_camera3_controls_z_custom_usecase_selection_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_Z_EXPOSURE_MERGER_WEIGHT, /* guzzi_camera3_controls_z_exposure_merger_weight_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_Z_WB_MANUAL_TEMPERATURE, /* guzzi_camera3_controls_z_wb_manual_temperature_t */
    GUZZI_CAMERA3_INDEX_CONTROLS_Z_WHITE_BALANCE_MERGER_WEIGHT, /* guzzi_camera3_controls_z_white_balance_merger_weight_t */

    GUZZI_CAMERA3_INDEX_METADATA_END_MARKER = 0xE0FD, /* guzzi_camera3_metadata_t */
    GUZZI_CAMERA3_INDEX_INVALID = 0xBAD0
} guzzi_camera3_metadata_index_t;

