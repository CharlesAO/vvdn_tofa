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
* @file aca_hllc_types.h
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

#ifndef  __ACA_INTERFACE_HLLC_TYPES_H__
#define  __ACA_INTERFACE_HLLC_TYPES_H__

#include <osal/osal_sysdep.h>
#include <osal/osal_stdtypes.h>
#include <osal/osal_time.h>
#include "dtp/dtp_server_defs.h"
#include <hal/hal_camera_module/hat_cm_sensor.h>
#include <hal/hal_camera_module/hat_cm_lens.h>
#include <aca_af_types.h>

/* ======================================================================= */
/** hllc_status_t - HLLC status This enumeration type is used by HLLC functions
* to return they status.
*
*  @param  HLLC_STATUS_SUCCESS    This status is returned when function is
*  complete successful.
*
*  @param  HLLC_STATUS_FAIL       This status is returned when there is error in
*  function execution.
*
*  @param  HLLC_STATUS_CONTINUE   This status is returned when function is
*  complete successful, but hllc algorithm need more calculation from other
*  functions.
*
*  @param  HLLC_STATUS_READ_LENS   This status is returned when function is
*  complete successful, but hllc algorithm need to know real lens position.
*/
/*  ==================================================================== */

typedef enum
{
    HLLC_STATUS_SUCCESS = 0,
    HLLC_STATUS_FAIL,
    HLLC_STATUS_CONTINUE,
    HLLC_STATUS_READ_LENS

} hllc_status_t;


/* ======================================================================= */
/** hllc_mov2pos_accuracy_t - Different lens movement accuracy. This
*   enumeration type is used by HLLC algorithm to know how to move lens.
*
*  @param  HLLC_MOV2POS_ACCURACY_INIT        If lens should be move to
*  desired lens position.
*
*  @param  HLLC_MOV2POS_ACCURACY_FINE        If lens should be move exactly
*  to desired lens position.
*
*  @param  HLLC_MOV2POS_ACCURACY_CALIBRATE   If lens calibration should be
*  done, caused by lens drifting.
*/
/*  ==================================================================== */
typedef enum
{
    HLLC_MOV2POS_ACCURACY_INIT = 0,
    HLLC_MOV2POS_ACCURACY_FINE,
    HLLC_MOV2POS_ACCURACY_CALIBRATE
} hllc_mov2pos_accuracy_t;

/* ======================================================================= */
/** hllc_cam_mode_t  describes the preview cameras modes. This
*   enumeration type is used by DTP parser to parse different usecases
*
*  @param  HLLC_CAM_MODE_STILL_CAPTURE  preview is in mode different from
*  video.
*
*  @param  HLLC_CAM_MODE_VIDEO          preview is in video mode
*/
/*  ==================================================================== */

typedef enum
{
    HLLC_CAM_MODE_STILL_CAPTURE,
    HLLC_CAM_MODE_VIDEO
} hllc_cam_mode_t;

/* ======================================================================= */
/** hllc_cmd_t  describes HLLC commands.
*
*  @param  HLLC_CMD_MOV2POS   - move command. Move lens to desired position.
*
*  @param  HLLC_CMD_MOVE      - move command. Move lens with desired step.
*/
/*  ==================================================================== */

typedef enum
{
    HLLC_CMD_MOV2POS,
    HLLC_CMD_MOVE,
    HLLC_CMD_MOVE_NONE
} hllc_cmd_t;

/* ========================================================================== */
/**
* This is a structure description for lens_drv_move_t.
* Move command to lens driver.
*
* @param  move_pos - int32 - New acutator position (realtive / absolute)
*
* @param  step_time_sleep -int32 - Time for lens move in us.
*
* @param  actual_lens_position -int32 - Absolute position of acutator.
*/
/* ========================================================================== */

typedef struct {
    float                   move_pos;
    int32                   step_time_sleep_us;
    float                   actual_lens_position;
} lens_drv_move_t;

/* ========================================================================== */
/**
* This is a structure description for lens_eeprom_t.
* Lens EEPROM info.
*
* @param  tal - uint8 - Piezo TAL.
* @param  tah - uint8 - Piezo TAH.
* @param  tbl - uint8 - Piezo TBL.
* @param  tbh - uint8 - Piezo TBH.
* @param  period - uint8 - Piezo period.
* @param  pulse_time - uint32 - Piezo pulse time.
* @param  pwm_per_um_fwd - uint32 - PWM period Forward. [um]
* @param  pwm_per_um_bkwd - uint32 -PWM period Backward. [um]

*/
/* ========================================================================== */

typedef struct {
    uint8                    tal;
    uint8                    tah;
    uint8                    tbl;
    uint8                    tbh;
    uint8                    period;
    uint32                   pulse_time;
    uint32                   pwm_per_um_fwd;
    uint32                   pwm_per_um_bkwd;
} lens_eeprom_t;

/* ========================================================================== */
/**
* This is a structure description for hllc_cfg_t - configure data.
*
* @param  hllc_cam_mode - hllc_cam_mode_t - preview mode.
*
* @param  eeprom_lens_info - lens_eeprom_t * - Lens EEPROM info.
*/
/* ========================================================================== */

typedef struct {
    hllc_cam_mode_t         hllc_cam_mode;
    lens_eeprom_t           *eeprom_lens_info;
} hllc_cfg_t;
/* ========================================================================== */
/**
* This is a structure description for hllc_move_absolute_t - move to position command.
*
* @param  pos - int32 - absolute position.
*
* @param  accuracy - hllc_mov2pos_accuracy_t - how precise to position the lenses.
*
* @param use_history - uint8 - Use history flag.
*/
/* ========================================================================== */

typedef struct {
    float                      pos;
    hllc_mov2pos_accuracy_t     accuracy;
    uint8                       use_history;
} hllc_move_absolute_t;

/* ========================================================================== */
/**
* This is a structure description for hllc_query_absolute_t - move to position query.
*
* @param  pos_from - int32 - absolute position to start from.
*
* @param  pos_to - int32 - absolute position to end to.
*
* @param  accuracy - hllc_mov2pos_accuracy_t - how precise to position the lenses.
*/
/* ========================================================================== */

typedef struct {
    float                   pos_from;
    float                   pos_to;
    hllc_mov2pos_accuracy_t accuracy;
} hllc_query_absolute_t;

/* ========================================================================== */
/**
* This is a structure description for hllc_move_t - the move command.
*
* @param  step_size - int32 - Size of move step.
*/
/* ========================================================================== */

typedef struct {
    float                   step_size;
} hllc_move_t;

/* ========================================================================== */
/**
* This is a structure description for hllc_cmd_t - HLLC do command.
*
* @param  uint32      - seq_cnt        - Sequence counter, incremented
*                                        always when configuration is changed
*
* @param  cmd_type    - hllc_cmd_t     - Command type.
*
* @param  move_cmd    - hllc_move_t    - Move command.
*
* @param  mov2pos_cmd - hllc_move_absolute_t - Move to position command.
*/
/* ========================================================================== */

typedef struct {
    uint32                  seq_cnt;
    hllc_cmd_t              cmd_type;
    hllc_move_t             move_cmd;
    hllc_move_absolute_t    mov2pos_cmd;
} hllc_do_cmd_t;



/* Sets the desired white balance mode. */
typedef enum  {
    HLLC_MODE_AUTO = 0,           /* Auto detect */

} hllc_modes_t;

/*
 * Next Table represents HLLC algorithm parameters that can be changed run time.
 * Structure was filled from 3AFW and send to HLLC algorithm using *_control() API.
 * HLLC configuration structure - struct hllc_configuration
 */
typedef struct  {
    dtpdb_static_common_t * dtp_s_common;
    focus_range_t           focus_range;
}aca_hllc_config_t;

/*
 * Following table shows input parameters passed to HLLC *_process() method.
 * It contain H3A, Histogram statistic data, Sensor and ISP gain and exposure
 * time, which are used for collection of this data. Structure was filled from
 * 3AFW and send to HLLC algorithm using *_process() API.
 * HLLC Input - struct hllc_calculation_input
*/
typedef struct  {
    dtpdb_dynamic_common_t *dtp_d_common;
    hllc_do_cmd_t          move_data;
    float                  current_position;
    uint32                 move_time;
}aca_hllc_calc_input_t;

#define HLLC_MAX_STEP_COUNT          (128)

typedef enum {
    HLLC_ACTION_INIT,
    HLLC_ACTION_CALIBRATE,
    HLLC_ACTION_MOVE,
    HLLC_ACTION_ABORT,
    HLLC_ACTION_MOVE2POS,
    HLLC_ACTION_MOVE_NONE,
} hllc_action_cmd_t;

/*
Output from HLLC algorithm is shown in next table . HLLC algorithm is responsible
to fill this structure and the end of *_process () API.
HLLC computation done - struct hllc_calculation_output
*/
typedef struct {
    uint32 seq_cnt;
    hllc_action_cmd_t action;
    hat_cm_lens_move_list_t*  step_list;
} aca_hllc_calc_output_t;


#endif /* __ACA_INTERFACE_HLLC_TYPES_H__ */

