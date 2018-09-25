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
* @file camera3_metadata_enum.h
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
 * guzzi_camera3_enum_black_level_lock_t
 */
typedef enum {
    GUZZI_CAMERA3_ENUM_BLACK_LEVEL_LOCK_OFF,
    GUZZI_CAMERA3_ENUM_BLACK_LEVEL_LOCK_ON,
} guzzi_camera3_enum_black_level_lock_t;

/*
 * guzzi_camera3_enum_color_correction_mode_t
 */
typedef enum {
    /*
     * Use the android.colorCorrection.transform matrix
     * and android.colorCorrection.gains to do color conversion
     */
    GUZZI_CAMERA3_ENUM_COLOR_CORRECTION_MODE_TRANSFORM_MATRIX,
    /*
     * Must not slow down frame rate relative to raw
     * bayer output
     */
    GUZZI_CAMERA3_ENUM_COLOR_CORRECTION_MODE_FAST,
    /*
     * Frame rate may be reduced by high
     * quality
     */
    GUZZI_CAMERA3_ENUM_COLOR_CORRECTION_MODE_HIGH_QUALITY,
} guzzi_camera3_enum_color_correction_mode_t;

/*
 * guzzi_camera3_enum_control_ae_antibanding_mode_t
 */
typedef enum {
    GUZZI_CAMERA3_ENUM_CONTROL_AE_ANTIBANDING_MODE_OFF,
    GUZZI_CAMERA3_ENUM_CONTROL_AE_ANTIBANDING_MODE_50HZ,
    GUZZI_CAMERA3_ENUM_CONTROL_AE_ANTIBANDING_MODE_60HZ,
    GUZZI_CAMERA3_ENUM_CONTROL_AE_ANTIBANDING_MODE_AUTO,
} guzzi_camera3_enum_control_ae_antibanding_mode_t;

/*
 * guzzi_camera3_enum_control_ae_lock_t
 */
typedef enum {
    /*
     * Autoexposure lock is disabled; the AE algorithm
     * is free to update its parameters.
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AE_LOCK_OFF,
    /*
     * Autoexposure lock is enabled; the AE algorithm
     * must not update the exposure and sensitivity parameters
     * while the lock is active
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AE_LOCK_ON,
} guzzi_camera3_enum_control_ae_lock_t;

/*
 * guzzi_camera3_enum_control_ae_mode_t
 */
typedef enum {
    /*
     * Autoexposure is disabled; sensor.exposureTime,
     * sensor.sensitivity and sensor.frameDuration are used
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AE_MODE_OFF,
    /*
     * Autoexposure is active, no flash
     * control
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AE_MODE_ON,
    /*
     * if flash exists Autoexposure is active, auto
     * flash control; flash may be fired when precapture
     * trigger is activated, and for captures for which
     * captureIntent = STILL_CAPTURE
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AE_MODE_ON_AUTO_FLASH,
    /*
     * if flash exists Autoexposure is active, auto
     * flash control for precapture trigger and always flash
     * when captureIntent = STILL_CAPTURE
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AE_MODE_ON_ALWAYS_FLASH,
    /*
     * optional Automatic red eye reduction with flash.
     * If deemed necessary, red eye reduction sequence should
     * fire when precapture trigger is activated, and final
     * flash should fire when captureIntent =
     * STILL_CAPTURE
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AE_MODE_ON_AUTO_FLASH_REDEYE,
} guzzi_camera3_enum_control_ae_mode_t;

/*
 * guzzi_camera3_enum_control_ae_precapture_trigger_t
 */
typedef enum {
    /*
     * The trigger is idle.
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AE_PRECAPTURE_TRIGGER_IDLE,
    /*
     * The precapture metering sequence
     * must be started. The exact effect of the precapture
     * trigger depends on the current AE mode and
     * state.
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AE_PRECAPTURE_TRIGGER_START,
} guzzi_camera3_enum_control_ae_precapture_trigger_t;

/*
 * guzzi_camera3_enum_control_ae_state_t
 */
typedef enum {
    /*
     * AE is off.  When a camera device is opened, it starts in
     * this state.
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AE_STATE_INACTIVE,
    /*
     * AE doesn't yet have a good set of control values
     * for the current scene
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AE_STATE_SEARCHING,
    /*
     * AE has a good set of control values for the
     * current scene
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AE_STATE_CONVERGED,
    /*
     * AE has been locked (aeMode =
     * LOCKED)
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AE_STATE_LOCKED,
    /*
     * AE has a good set of control values, but flash
     * needs to be fired for good quality still
     * capture
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AE_STATE_FLASH_REQUIRED,
    /*
     * AE has been asked to do a precapture sequence
     * (through the
     * trigger_action(CAMERA2_TRIGGER_PRECAPTURE_METERING)
     * call), and is currently executing it. Once PRECAPTURE
     * completes, AE will transition to CONVERGED or
     * FLASH_REQUIRED as appropriate
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AE_STATE_PRECAPTURE,
} guzzi_camera3_enum_control_ae_state_t;

/*
 * guzzi_camera3_enum_control_af_mode_t
 */
typedef enum {
    /*
     * The 3A routines do not control the lens;
     * android.lens.focusDistance is controlled by the
     * application
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AF_MODE_OFF,
    /*
     * if lens is not fixed focus.
     * 
     * Use android.lens.minimumFocusDistance to determine if lens
     * is fixed focus In this mode, the lens does not move unless
     * the autofocus trigger action is called. When that trigger
     * is activated, AF must transition to ACTIVE_SCAN, then to
     * the outcome of the scan (FOCUSED or
     * NOT_FOCUSED).
     * 
     * Triggering cancel AF resets the lens position to default,
     * and sets the AF state to INACTIVE.
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AF_MODE_AUTO,
    /*
     * In this mode, the lens does not move unless the
     * autofocus trigger action is called.
     * 
     * When that trigger is activated, AF must transition to
     * ACTIVE_SCAN, then to the outcome of the scan (FOCUSED or
     * NOT_FOCUSED).  Triggering cancel AF resets the lens
     * position to default, and sets the AF state to
     * INACTIVE.
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AF_MODE_MACRO,
    /*
     * In this mode, the AF algorithm modifies the lens
     * position continually to attempt to provide a
     * constantly-in-focus image stream.
     * 
     * The focusing behavior should be suitable for good quality
     * video recording; typically this means slower focus
     * movement and no overshoots. When the AF trigger is not
     * involved, the AF algorithm should start in INACTIVE state,
     * and then transition into PASSIVE_SCAN and PASSIVE_FOCUSED
     * states as appropriate. When the AF trigger is activated,
     * the algorithm should immediately transition into
     * AF_FOCUSED or AF_NOT_FOCUSED as appropriate, and lock the
     * lens position until a cancel AF trigger is received.
     * 
     * Once cancel is received, the algorithm should transition
     * back to INACTIVE and resume passive scan. Note that this
     * behavior is not identical to CONTINUOUS_PICTURE, since an
     * ongoing PASSIVE_SCAN must immediately be
     * canceled.
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AF_MODE_CONTINUOUS_VIDEO,
    /*
     * In this mode, the AF algorithm modifies the lens
     * position continually to attempt to provide a
     * constantly-in-focus image stream.
     * 
     * The focusing behavior should be suitable for still image
     * capture; typically this means focusing as fast as
     * possible. When the AF trigger is not involved, the AF
     * algorithm should start in INACTIVE state, and then
     * transition into PASSIVE_SCAN and PASSIVE_FOCUSED states as
     * appropriate as it attempts to maintain focus. When the AF
     * trigger is activated, the algorithm should finish its
     * PASSIVE_SCAN if active, and then transition into
     * AF_FOCUSED or AF_NOT_FOCUSED as appropriate, and lock the
     * lens position until a cancel AF trigger is received.
     * 
     * When the AF cancel trigger is activated, the algorithm
     * should transition back to INACTIVE and then act as if it
     * has just been started.
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AF_MODE_CONTINUOUS_PICTURE,
    /*
     * Extended depth of field (digital focus). AF
     * trigger is ignored, AF state should always be
     * INACTIVE.
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AF_MODE_EDOF,
} guzzi_camera3_enum_control_af_mode_t;

/*
 * guzzi_camera3_enum_control_af_state_t
 */
typedef enum {
    /*
     * AF off or has not yet tried to scan/been asked
     * to scan.  When a camera device is opened, it starts in
     * this state.
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AF_STATE_INACTIVE,
    /*
     * if CONTINUOUS_* modes are supported. AF is
     * currently doing an AF scan initiated by a continuous
     * autofocus mode
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AF_STATE_PASSIVE_SCAN,
    /*
     * if CONTINUOUS_* modes are supported. AF currently
     * believes it is in focus, but may restart scanning at
     * any time.
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AF_STATE_PASSIVE_FOCUSED,
    /*
     * if AUTO or MACRO modes are supported. AF is doing
     * an AF scan because it was triggered by AF
     * trigger
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AF_STATE_ACTIVE_SCAN,
    /*
     * if any AF mode besides OFF is supported. AF
     * believes it is focused correctly and is
     * locked
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AF_STATE_FOCUSED_LOCKED,
    /*
     * if any AF mode besides OFF is supported. AF has
     * failed to focus successfully and is
     * locked
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AF_STATE_NOT_FOCUSED_LOCKED,
    /*
     * if CONTINUOUS_* modes are supported. AF finished a
     * passive scan without finding focus, and may restart
     * scanning at any time.
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AF_STATE_PASSIVE_UNFOCUSED,
} guzzi_camera3_enum_control_af_state_t;

/*
 * guzzi_camera3_enum_control_af_trigger_t
 */
typedef enum {
    /*
     * The trigger is idle.
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AF_TRIGGER_IDLE,
    /*
     * Autofocus must trigger now.
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AF_TRIGGER_START,
    /*
     * Autofocus must return to initial
     * state, and cancel any active trigger.
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AF_TRIGGER_CANCEL,
} guzzi_camera3_enum_control_af_trigger_t;

/*
 * guzzi_camera3_enum_control_awb_lock_t
 */
typedef enum {
    /*
     * Auto-whitebalance lock is disabled; the AWB
     * algorithm is free to update its parameters if in AUTO
     * mode.
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AWB_LOCK_OFF,
    /*
     * Auto-whitebalance lock is enabled; the AWB
     * algorithm must not update the exposure and sensitivity
     * parameters while the lock is active
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AWB_LOCK_ON,
} guzzi_camera3_enum_control_awb_lock_t;

/*
 * guzzi_camera3_enum_control_awb_mode_t
 */
typedef enum {
    GUZZI_CAMERA3_ENUM_CONTROL_AWB_MODE_OFF,
    GUZZI_CAMERA3_ENUM_CONTROL_AWB_MODE_AUTO,
    GUZZI_CAMERA3_ENUM_CONTROL_AWB_MODE_INCANDESCENT,
    GUZZI_CAMERA3_ENUM_CONTROL_AWB_MODE_FLUORESCENT,
    GUZZI_CAMERA3_ENUM_CONTROL_AWB_MODE_WARM_FLUORESCENT,
    GUZZI_CAMERA3_ENUM_CONTROL_AWB_MODE_DAYLIGHT,
    GUZZI_CAMERA3_ENUM_CONTROL_AWB_MODE_CLOUDY_DAYLIGHT,
    GUZZI_CAMERA3_ENUM_CONTROL_AWB_MODE_TWILIGHT,
    GUZZI_CAMERA3_ENUM_CONTROL_AWB_MODE_SHADE,
} guzzi_camera3_enum_control_awb_mode_t;

/*
 * guzzi_camera3_enum_control_awb_state_t
 */
typedef enum {
    /*
     * AWB is not in auto mode.  When a camera device is opened, it
     * starts in this state.
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AWB_STATE_INACTIVE,
    /*
     * AWB doesn't yet have a good set of control
     * values for the current scene
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AWB_STATE_SEARCHING,
    /*
     * AWB has a good set of control values for the
     * current scene
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AWB_STATE_CONVERGED,
    /*
     * AE has been locked (aeMode =
     * LOCKED)
     */
    GUZZI_CAMERA3_ENUM_CONTROL_AWB_STATE_LOCKED,
} guzzi_camera3_enum_control_awb_state_t;

/*
 * guzzi_camera3_enum_control_capture_intent_t
 */
typedef enum {
    /*
     * This request doesn't fall into the other
     * categories. Default to preview-like
     * behavior.
     */
    GUZZI_CAMERA3_ENUM_CONTROL_CAPTURE_INTENT_CUSTOM,
    /*
     * This request is for a preview-like usecase. The
     * precapture trigger may be used to start off a metering
     * w/flash sequence
     */
    GUZZI_CAMERA3_ENUM_CONTROL_CAPTURE_INTENT_PREVIEW,
    /*
     * This request is for a still capture-type
     * usecase.
     */
    GUZZI_CAMERA3_ENUM_CONTROL_CAPTURE_INTENT_STILL_CAPTURE,
    /*
     * This request is for a video recording
     * usecase.
     */
    GUZZI_CAMERA3_ENUM_CONTROL_CAPTURE_INTENT_VIDEO_RECORD,
    /*
     * This request is for a video snapshot (still
     * image while recording video) usecase
     */
    GUZZI_CAMERA3_ENUM_CONTROL_CAPTURE_INTENT_VIDEO_SNAPSHOT,
    /*
     * This request is for a ZSL usecase; the
     * application will stream full-resolution images and
     * reprocess one or several later for a final
     * capture
     */
    GUZZI_CAMERA3_ENUM_CONTROL_CAPTURE_INTENT_ZERO_SHUTTER_LAG,
} guzzi_camera3_enum_control_capture_intent_t;

/*
 * guzzi_camera3_enum_control_effect_mode_t
 */
typedef enum {
    GUZZI_CAMERA3_ENUM_CONTROL_EFFECT_MODE_OFF,
    GUZZI_CAMERA3_ENUM_CONTROL_EFFECT_MODE_MONO,
    GUZZI_CAMERA3_ENUM_CONTROL_EFFECT_MODE_NEGATIVE,
    GUZZI_CAMERA3_ENUM_CONTROL_EFFECT_MODE_SOLARIZE,
    GUZZI_CAMERA3_ENUM_CONTROL_EFFECT_MODE_SEPIA,
    GUZZI_CAMERA3_ENUM_CONTROL_EFFECT_MODE_POSTERIZE,
    GUZZI_CAMERA3_ENUM_CONTROL_EFFECT_MODE_WHITEBOARD,
    GUZZI_CAMERA3_ENUM_CONTROL_EFFECT_MODE_BLACKBOARD,
    GUZZI_CAMERA3_ENUM_CONTROL_EFFECT_MODE_AQUA,
} guzzi_camera3_enum_control_effect_mode_t;

/*
 * guzzi_camera3_enum_control_mode_t
 */
typedef enum {
    /*
     * Full application control of pipeline. All 3A
     * routines are disabled, no other settings in
     * android.control.* have any effect
     */
    GUZZI_CAMERA3_ENUM_CONTROL_MODE_OFF,
    /*
     * Use settings for each individual 3A routine.
     * Manual control of capture parameters is disabled. All
     * controls in android.control.* besides sceneMode take
     * effect
     */
    GUZZI_CAMERA3_ENUM_CONTROL_MODE_AUTO,
    /*
     * Use specific scene mode. Enabling this disables
     * control.aeMode, control.awbMode and control.afMode
     * controls; the HAL must ignore those settings while
     * USE_SCENE_MODE is active (except for FACE_PRIORITY
     * scene mode). Other control entries are still active.
     * This setting can only be used if availableSceneModes !=
     * UNSUPPORTED
     */
    GUZZI_CAMERA3_ENUM_CONTROL_MODE_USE_SCENE_MODE,
} guzzi_camera3_enum_control_mode_t;

/*
 * guzzi_camera3_enum_control_scene_mode_t
 */
typedef enum {
    GUZZI_CAMERA3_ENUM_CONTROL_SCENE_MODE_UNSUPPORTED = 0,
    /*
     * if face detection support exists Use face
     * detection data to drive 3A routines. If face detection
     * statistics are disabled, should still operate correctly
     * (but not return face detection statistics to the
     * framework).
     * 
     * Unlike the other scene modes, aeMode, awbMode, and afMode
     * remain active when FACE_PRIORITY is set. This is due to
     * compatibility concerns with the old camera
     * API
     */
    GUZZI_CAMERA3_ENUM_CONTROL_SCENE_MODE_FACE_PRIORITY,
    GUZZI_CAMERA3_ENUM_CONTROL_SCENE_MODE_ACTION,
    GUZZI_CAMERA3_ENUM_CONTROL_SCENE_MODE_PORTRAIT,
    GUZZI_CAMERA3_ENUM_CONTROL_SCENE_MODE_LANDSCAPE,
    GUZZI_CAMERA3_ENUM_CONTROL_SCENE_MODE_NIGHT,
    GUZZI_CAMERA3_ENUM_CONTROL_SCENE_MODE_NIGHT_PORTRAIT,
    GUZZI_CAMERA3_ENUM_CONTROL_SCENE_MODE_THEATRE,
    GUZZI_CAMERA3_ENUM_CONTROL_SCENE_MODE_BEACH,
    GUZZI_CAMERA3_ENUM_CONTROL_SCENE_MODE_SNOW,
    GUZZI_CAMERA3_ENUM_CONTROL_SCENE_MODE_SUNSET,
    GUZZI_CAMERA3_ENUM_CONTROL_SCENE_MODE_STEADYPHOTO,
    GUZZI_CAMERA3_ENUM_CONTROL_SCENE_MODE_FIREWORKS,
    GUZZI_CAMERA3_ENUM_CONTROL_SCENE_MODE_SPORTS,
    GUZZI_CAMERA3_ENUM_CONTROL_SCENE_MODE_PARTY,
    GUZZI_CAMERA3_ENUM_CONTROL_SCENE_MODE_CANDLELIGHT,
    GUZZI_CAMERA3_ENUM_CONTROL_SCENE_MODE_BARCODE,
} guzzi_camera3_enum_control_scene_mode_t;

/*
 * guzzi_camera3_enum_control_video_stabilization_mode_t
 */
typedef enum {
    GUZZI_CAMERA3_ENUM_CONTROL_VIDEO_STABILIZATION_MODE_OFF,
    GUZZI_CAMERA3_ENUM_CONTROL_VIDEO_STABILIZATION_MODE_ON,
} guzzi_camera3_enum_control_video_stabilization_mode_t;

/*
 * guzzi_camera3_enum_demosaic_mode_t
 */
typedef enum {
    /*
     * Minimal or no slowdown of frame rate compared to
     * Bayer RAW output
     */
    GUZZI_CAMERA3_ENUM_DEMOSAIC_MODE_FAST,
    /*
     * High-quality may reduce output frame
     * rate
     */
    GUZZI_CAMERA3_ENUM_DEMOSAIC_MODE_HIGH_QUALITY,
} guzzi_camera3_enum_demosaic_mode_t;

/*
 * guzzi_camera3_enum_edge_mode_t
 */
typedef enum {
    /*
     * No edge enhancement is applied
     */
    GUZZI_CAMERA3_ENUM_EDGE_MODE_OFF,
    /*
     * Must not slow down frame rate relative to raw
     * bayer output
     */
    GUZZI_CAMERA3_ENUM_EDGE_MODE_FAST,
    /*
     * Frame rate may be reduced by high
     * quality
     */
    GUZZI_CAMERA3_ENUM_EDGE_MODE_HIGH_QUALITY,
} guzzi_camera3_enum_edge_mode_t;

/*
 * guzzi_camera3_enum_flash_mode_t
 */
typedef enum {
    /*
     * Do not fire the flash for this
     * capture
     */
    GUZZI_CAMERA3_ENUM_FLASH_MODE_OFF,
    /*
     * if android.flash.available is true Fire flash
     * for this capture based on firingPower,
     * firingTime.
     */
    GUZZI_CAMERA3_ENUM_FLASH_MODE_SINGLE,
    /*
     * if android.flash.available is true Flash
     * continuously on, power set by
     * firingPower
     */
    GUZZI_CAMERA3_ENUM_FLASH_MODE_TORCH,
} guzzi_camera3_enum_flash_mode_t;

/*
 * guzzi_camera3_enum_flash_state_t
 */
typedef enum {
    /*
     * No flash on camera
     */
    GUZZI_CAMERA3_ENUM_FLASH_STATE_UNAVAILABLE,
    /*
     * if android.flash.available is true Flash is
     * charging and cannot be fired
     */
    GUZZI_CAMERA3_ENUM_FLASH_STATE_CHARGING,
    /*
     * if android.flash.available is true Flash is
     * ready to fire
     */
    GUZZI_CAMERA3_ENUM_FLASH_STATE_READY,
    /*
     * if android.flash.available is true Flash fired
     * for this capture
     */
    GUZZI_CAMERA3_ENUM_FLASH_STATE_FIRED,
} guzzi_camera3_enum_flash_state_t;

/*
 * guzzi_camera3_enum_geometric_mode_t
 */
typedef enum {
    /*
     * No geometric correction is
     * applied
     */
    GUZZI_CAMERA3_ENUM_GEOMETRIC_MODE_OFF,
    /*
     * Must not slow down frame rate relative to raw
     * bayer output
     */
    GUZZI_CAMERA3_ENUM_GEOMETRIC_MODE_FAST,
    /*
     * Frame rate may be reduced by high
     * quality
     */
    GUZZI_CAMERA3_ENUM_GEOMETRIC_MODE_HIGH_QUALITY,
} guzzi_camera3_enum_geometric_mode_t;

/*
 * guzzi_camera3_enum_hot_pixel_mode_t
 */
typedef enum {
    /*
     * No hot pixel correction can be
     * applied
     */
    GUZZI_CAMERA3_ENUM_HOT_PIXEL_MODE_OFF,
    /*
     * Frame rate must not be reduced compared to raw
     * Bayer output
     */
    GUZZI_CAMERA3_ENUM_HOT_PIXEL_MODE_FAST,
    /*
     * Frame rate may be reduced by high
     * quality
     */
    GUZZI_CAMERA3_ENUM_HOT_PIXEL_MODE_HIGH_QUALITY,
} guzzi_camera3_enum_hot_pixel_mode_t;

/*
 * guzzi_camera3_enum_info_supported_hardware_level_t
 */
typedef enum {
    GUZZI_CAMERA3_ENUM_INFO_SUPPORTED_HARDWARE_LEVEL_LIMITED,
    GUZZI_CAMERA3_ENUM_INFO_SUPPORTED_HARDWARE_LEVEL_FULL,
} guzzi_camera3_enum_info_supported_hardware_level_t;

/*
 * guzzi_camera3_enum_led_available_leds_t
 */
typedef enum {
    /*
     * android.led.transmit control is used
     */
    GUZZI_CAMERA3_ENUM_LED_AVAILABLE_LEDS_TRANSMIT,
} guzzi_camera3_enum_led_available_leds_t;

/*
 * guzzi_camera3_enum_led_transmit_t
 */
typedef enum {
    GUZZI_CAMERA3_ENUM_LED_TRANSMIT_OFF,
    GUZZI_CAMERA3_ENUM_LED_TRANSMIT_ON,
} guzzi_camera3_enum_led_transmit_t;

/*
 * guzzi_camera3_enum_lens_facing_t
 */
typedef enum {
    GUZZI_CAMERA3_ENUM_LENS_FACING_FRONT,
    GUZZI_CAMERA3_ENUM_LENS_FACING_BACK,
} guzzi_camera3_enum_lens_facing_t;

/*
 * guzzi_camera3_enum_lens_optical_stabilization_mode_t
 */
typedef enum {
    GUZZI_CAMERA3_ENUM_LENS_OPTICAL_STABILIZATION_MODE_OFF,
    GUZZI_CAMERA3_ENUM_LENS_OPTICAL_STABILIZATION_MODE_ON,
} guzzi_camera3_enum_lens_optical_stabilization_mode_t;

/*
 * guzzi_camera3_enum_lens_state_t
 */
typedef enum {
    GUZZI_CAMERA3_ENUM_LENS_STATE_STATIONARY,
    GUZZI_CAMERA3_ENUM_LENS_STATE_MOVING,
} guzzi_camera3_enum_lens_state_t;

/*
 * guzzi_camera3_enum_noise_reduction_mode_t
 */
typedef enum {
    /*
     * No noise reduction is applied
     */
    GUZZI_CAMERA3_ENUM_NOISE_REDUCTION_MODE_OFF,
    /*
     * Must not slow down frame rate relative to raw
     * bayer output
     */
    GUZZI_CAMERA3_ENUM_NOISE_REDUCTION_MODE_FAST,
    /*
     * May slow down frame rate to provide highest
     * quality
     */
    GUZZI_CAMERA3_ENUM_NOISE_REDUCTION_MODE_HIGH_QUALITY,
} guzzi_camera3_enum_noise_reduction_mode_t;

/*
 * guzzi_camera3_enum_quirks_partial_result_t
 */
typedef enum {
    /*
     * The last or only metadata result buffer
     * for this capture.
     */
    GUZZI_CAMERA3_ENUM_QUIRKS_PARTIAL_RESULT_FINAL,
    /*
     * A partial buffer of result metadata for this
     * capture. More result buffers for this capture will be sent
     * by the HAL, the last of which will be marked
     * FINAL.
     */
    GUZZI_CAMERA3_ENUM_QUIRKS_PARTIAL_RESULT_PARTIAL,
} guzzi_camera3_enum_quirks_partial_result_t;

/*
 * guzzi_camera3_enum_request_metadata_mode_t
 */
typedef enum {
    /*
     * No metadata should be produced on output, except
     * for application-bound buffer data. If no
     * application-bound streams exist, no frame should be
     * placed in the output frame queue. If such streams
     * exist, a frame should be placed on the output queue
     * with null metadata but with the necessary output buffer
     * information. Timestamp information should still be
     * included with any output stream buffers
     */
    GUZZI_CAMERA3_ENUM_REQUEST_METADATA_MODE_NONE,
    /*
     * All metadata should be produced. Statistics will
     * only be produced if they are separately
     * enabled
     */
    GUZZI_CAMERA3_ENUM_REQUEST_METADATA_MODE_FULL,
} guzzi_camera3_enum_request_metadata_mode_t;

/*
 * guzzi_camera3_enum_request_type_t
 */
typedef enum {
    /*
     * Capture a new image from the imaging hardware,
     * and process it according to the
     * settings
     */
    GUZZI_CAMERA3_ENUM_REQUEST_TYPE_CAPTURE,
    /*
     * Process previously captured data; the
     * android.request.inputStream parameter determines the
     * source reprocessing stream. TODO: Mark dynamic metadata
     * needed for reprocessing with [RP]
     */
    GUZZI_CAMERA3_ENUM_REQUEST_TYPE_REPROCESS,
} guzzi_camera3_enum_request_type_t;

/*
 * guzzi_camera3_enum_scaler_available_formats_t
 * values from HAL_PIXEL_FORMAT_* in /system/core/include/system/graphics.h
 */
typedef enum {
    GUZZI_CAMERA3_ENUM_SCALER_AVAILABLE_FORMATS_RAW_SENSOR = 0x20,
    /*
     * YCrCb 4:2:0 Planar
     */
    GUZZI_CAMERA3_ENUM_SCALER_AVAILABLE_FORMATS_YV12 = 0x32315659,
    /*
     * NV21
     */
    GUZZI_CAMERA3_ENUM_SCALER_AVAILABLE_FORMATS_YCRCB_420_SP = 0x11,
    /*
     * Hal Implementation Defined
     */
    GUZZI_CAMERA3_ENUM_SCALER_AVAILABLE_FORMATS_IMPLEMENTATION_DEFINED = 0x22,
    /*
     * Flexible YUV420 Format
     */
    GUZZI_CAMERA3_ENUM_SCALER_AVAILABLE_FORMATS_YCBCR_420_888 = 0x23,
    /*
     * JPEG
     */
    GUZZI_CAMERA3_ENUM_SCALER_AVAILABLE_FORMATS_BLOB = 0x21,
} guzzi_camera3_enum_scaler_available_formats_t;

/*
 * guzzi_camera3_enum_sensor_info_color_filter_arrangement_t
 */
typedef enum {
    GUZZI_CAMERA3_ENUM_SENSOR_INFO_COLOR_FILTER_ARRANGEMENT_RGGB,
    GUZZI_CAMERA3_ENUM_SENSOR_INFO_COLOR_FILTER_ARRANGEMENT_GRBG,
    GUZZI_CAMERA3_ENUM_SENSOR_INFO_COLOR_FILTER_ARRANGEMENT_GBRG,
    GUZZI_CAMERA3_ENUM_SENSOR_INFO_COLOR_FILTER_ARRANGEMENT_BGGR,
    /*
     * Sensor is not Bayer; output has 3 16-bit
     * values for each pixel, instead of just 1 16-bit value
     * per pixel.
     */
    GUZZI_CAMERA3_ENUM_SENSOR_INFO_COLOR_FILTER_ARRANGEMENT_RGB,
} guzzi_camera3_enum_sensor_info_color_filter_arrangement_t;

/*
 * guzzi_camera3_enum_sensor_reference_illuminant1_t
 */
typedef enum {
    GUZZI_CAMERA3_ENUM_SENSOR_REFERENCE_ILLUMINANT1_DAYLIGHT = 1,
    GUZZI_CAMERA3_ENUM_SENSOR_REFERENCE_ILLUMINANT1_FLUORESCENT = 2,
    /*
     * Incandescent light
     */
    GUZZI_CAMERA3_ENUM_SENSOR_REFERENCE_ILLUMINANT1_TUNGSTEN = 3,
    GUZZI_CAMERA3_ENUM_SENSOR_REFERENCE_ILLUMINANT1_FLASH = 4,
    GUZZI_CAMERA3_ENUM_SENSOR_REFERENCE_ILLUMINANT1_FINE_WEATHER = 9,
    GUZZI_CAMERA3_ENUM_SENSOR_REFERENCE_ILLUMINANT1_CLOUDY_WEATHER = 10,
    GUZZI_CAMERA3_ENUM_SENSOR_REFERENCE_ILLUMINANT1_SHADE = 11,
    /*
     * D 5700 - 7100K
     */
    GUZZI_CAMERA3_ENUM_SENSOR_REFERENCE_ILLUMINANT1_DAYLIGHT_FLUORESCENT = 12,
    /*
     * N 4600 - 5400K
     */
    GUZZI_CAMERA3_ENUM_SENSOR_REFERENCE_ILLUMINANT1_DAY_WHITE_FLUORESCENT = 13,
    /*
     * W 3900 - 4500K
     */
    GUZZI_CAMERA3_ENUM_SENSOR_REFERENCE_ILLUMINANT1_COOL_WHITE_FLUORESCENT = 14,
    /*
     * WW 3200 - 3700K
     */
    GUZZI_CAMERA3_ENUM_SENSOR_REFERENCE_ILLUMINANT1_WHITE_FLUORESCENT = 15,
    GUZZI_CAMERA3_ENUM_SENSOR_REFERENCE_ILLUMINANT1_STANDARD_A = 17,
    GUZZI_CAMERA3_ENUM_SENSOR_REFERENCE_ILLUMINANT1_STANDARD_B = 18,
    GUZZI_CAMERA3_ENUM_SENSOR_REFERENCE_ILLUMINANT1_STANDARD_C = 19,
    GUZZI_CAMERA3_ENUM_SENSOR_REFERENCE_ILLUMINANT1_D55 = 20,
    GUZZI_CAMERA3_ENUM_SENSOR_REFERENCE_ILLUMINANT1_D65 = 21,
    GUZZI_CAMERA3_ENUM_SENSOR_REFERENCE_ILLUMINANT1_D75 = 22,
    GUZZI_CAMERA3_ENUM_SENSOR_REFERENCE_ILLUMINANT1_D50 = 23,
    GUZZI_CAMERA3_ENUM_SENSOR_REFERENCE_ILLUMINANT1_ISO_STUDIO_TUNGSTEN = 24,
} guzzi_camera3_enum_sensor_reference_illuminant1_t;

/*
 * guzzi_camera3_enum_shading_mode_t
 */
typedef enum {
    /*
     * No shading correction is applied
     */
    GUZZI_CAMERA3_ENUM_SHADING_MODE_OFF,
    /*
     * Must not slow down frame rate relative to raw
     * bayer output
     */
    GUZZI_CAMERA3_ENUM_SHADING_MODE_FAST,
    /*
     * Frame rate may be reduced by high
     * quality
     */
    GUZZI_CAMERA3_ENUM_SHADING_MODE_HIGH_QUALITY,
} guzzi_camera3_enum_shading_mode_t;

/*
 * guzzi_camera3_enum_statistics_face_detect_mode_t
 */
typedef enum {
    GUZZI_CAMERA3_ENUM_STATISTICS_FACE_DETECT_MODE_OFF,
    /*
     * Optional Return rectangle and confidence
     * only
     */
    GUZZI_CAMERA3_ENUM_STATISTICS_FACE_DETECT_MODE_SIMPLE,
    /*
     * Optional Return all face
     * metadata
     */
    GUZZI_CAMERA3_ENUM_STATISTICS_FACE_DETECT_MODE_FULL,
} guzzi_camera3_enum_statistics_face_detect_mode_t;

/*
 * guzzi_camera3_enum_statistics_histogram_mode_t
 */
typedef enum {
    GUZZI_CAMERA3_ENUM_STATISTICS_HISTOGRAM_MODE_OFF,
    GUZZI_CAMERA3_ENUM_STATISTICS_HISTOGRAM_MODE_ON,
} guzzi_camera3_enum_statistics_histogram_mode_t;

/*
 * guzzi_camera3_enum_statistics_lens_shading_map_mode_t
 */
typedef enum {
    GUZZI_CAMERA3_ENUM_STATISTICS_LENS_SHADING_MAP_MODE_OFF,
    GUZZI_CAMERA3_ENUM_STATISTICS_LENS_SHADING_MAP_MODE_ON,
} guzzi_camera3_enum_statistics_lens_shading_map_mode_t;

/*
 * guzzi_camera3_enum_statistics_scene_flicker_t
 */
typedef enum {
    GUZZI_CAMERA3_ENUM_STATISTICS_SCENE_FLICKER_NONE,
    GUZZI_CAMERA3_ENUM_STATISTICS_SCENE_FLICKER_50HZ,
    GUZZI_CAMERA3_ENUM_STATISTICS_SCENE_FLICKER_60HZ,
} guzzi_camera3_enum_statistics_scene_flicker_t;

/*
 * guzzi_camera3_enum_statistics_sharpness_map_mode_t
 */
typedef enum {
    GUZZI_CAMERA3_ENUM_STATISTICS_SHARPNESS_MAP_MODE_OFF,
    GUZZI_CAMERA3_ENUM_STATISTICS_SHARPNESS_MAP_MODE_ON,
} guzzi_camera3_enum_statistics_sharpness_map_mode_t;

/*
 * guzzi_camera3_enum_tonemap_mode_t
 */
typedef enum {
    /*
     * Use the tone mapping curve specified in
     * android.tonemap.curve
     */
    GUZZI_CAMERA3_ENUM_TONEMAP_MODE_CONTRAST_CURVE,
    /*
     * Must not slow down frame rate relative to raw
     * bayer output
     */
    GUZZI_CAMERA3_ENUM_TONEMAP_MODE_FAST,
    /*
     * Frame rate may be reduced by high
     * quality
     */
    GUZZI_CAMERA3_ENUM_TONEMAP_MODE_HIGH_QUALITY,
} guzzi_camera3_enum_tonemap_mode_t;

/*
 * ****************************************************************************
 * ** Guzzi specific **********************************************************
 * ****************************************************************************
 */
/*
 * guzzi_camera3_enum_capture_request_active_t
 */
typedef enum {
    GUZZI_CAMERA3_ENUM_CAPTURE_REQUEST_ACTIVE_OFF,
    GUZZI_CAMERA3_ENUM_CAPTURE_REQUEST_ACTIVE_ON,
} guzzi_camera3_enum_capture_request_active_t;

/*
 * guzzi_camera3_enum_stream_config_format_t
 */
typedef enum {
    GUZZI_CAMERA3_ENUM_STREAM_CONFIG_FORMAT_IMAGE_FORMAT_RAW10,
    GUZZI_CAMERA3_ENUM_STREAM_CONFIG_FORMAT_IMAGE_FORMAT_UYVY,
    GUZZI_CAMERA3_ENUM_STREAM_CONFIG_FORMAT_IMAGE_FORMAT_NV12,
} guzzi_camera3_enum_stream_config_format_t;

/*
 * guzzi_camera3_enum_z_custom_capture_mode_selection_t
 */
typedef enum {
    GUZZI_CAMERA3_ENUM_Z_CUSTOM_CAPTURE_MODE_SELECTION_TEMPORAL_BRACKETING,
    GUZZI_CAMERA3_ENUM_Z_CUSTOM_CAPTURE_MODE_SELECTION_EXPOSURE_BRACKETING,
    GUZZI_CAMERA3_ENUM_Z_CUSTOM_CAPTURE_MODE_SELECTION_LONG_EXPOSURE,
} guzzi_camera3_enum_z_custom_capture_mode_selection_t;

/*
 * guzzi_camera3_enum_z_custom_usecase_selection_t
 */
typedef enum {
    GUZZI_CAMERA3_ENUM_Z_CUSTOM_USECASE_SELECTION_STILL,
    GUZZI_CAMERA3_ENUM_Z_CUSTOM_USECASE_SELECTION_VIDEO,
    GUZZI_CAMERA3_ENUM_Z_CUSTOM_USECASE_SELECTION_LOW_POWER_VIDEO,
} guzzi_camera3_enum_z_custom_usecase_selection_t;

