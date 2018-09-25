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
* @file flash_generator.h
*
* @author ( MM Solutions AD )
*
*/
/* -----------------------------------------------------------------------------
*!
*! Revision History
*! ===================================
*! Jun 16, 2014 : Author aiovtchev
*! Created
* =========================================================================== */



#ifndef FLASH_GENERATOR_H_
#define FLASH_GENERATOR_H_

#include <osal/osal_stdtypes.h>
#include <hal/hat_light.h>
#include <dtp/dtp_server_defs.h>
#include <cam_cl_frame_req.h>
#include "cam_algo_state.h"
#include <guzzi_event/include/guzzi_event.h>
#include <guzzi_event_global/include/guzzi_event_global.h>

/**
 * algorithm handle
 */
typedef void* fl_gen_hndl_t;

#define MAX_FASH_SEQUENCE  8 // max flash sequence duration in frames

typedef enum
{
    FL_GEN_TYPE_RED_EYE,
    FL_GEN_TYPE_PRE_FLASH,
    FL_GEN_TYPE_MAIN_FLASH,
}fl_seq_types_t;


/**
 * algorithm configuration
 *
 * @param dtp_s_common DTP static common
 *
 * @see dtpdb_static_common_t
 */
typedef struct
{
    dtpdb_static_common_t *dtp_s_common;
} fl_gen_config_t;

/**
 * algorithm input parameters
 *
 * @param seq_type calculation type
 * @param dtp_s_common DTP dynamic common parameters
 *
 * @see dtpdb_dynamic_common_t
 */
typedef struct
{
    dtpdb_dynamic_common_t *dtp_d_common;
    fl_seq_types_t  seq_type;
    uint32          frame_duration; // sensor readout [us]
} fl_gen_input_t;

/**
 * algorithm output data
 *
 * @param frames integral number of frames sequence duration will take
 * @param fr_ctrl array with detailed control for each buffer per frame
 * @param fl_ctrl outputs a list with light/flash sequence
 *
 * @see hat_light_ctrl_list_t
 * @see fl_gen_input_t
 */

typedef struct {
    cam_cl_algo_ctrl_t  **algo_flash_ctrl;
    uint32              end_seq_flags;
} cam_cl_algo_flash_ctrl_t;

typedef struct
{
    uint32                      frames;
    cam_cl_algo_flash_ctrl_t    frame_ctrl[MAX_FASH_SEQUENCE];
    hat_light_ctrl_list_t       *fl_ctrl;
} fl_gen_output_t;

typedef struct {
    uint32                      num_frames;
    cam_cl_algo_flash_ctrl_t    *frame_ctrl_list;
} frame_control_sequence_t;

/**
 * fl_gen_create() algorithm create API
 *
 * @param fl_gen_hndl - returns a algorithm handle
 * @return 0 - success (!0) on error
 */
int
fl_gen_create(fl_gen_hndl_t* fl_gen_hndl);

/**
 * fl_gen_destroy() algorithm destroy API
 *
 * @param fl_gen_hndl algorithm handle
 *
 * @return 0 - success (!0) on error
 */
int
fl_gen_destroy(fl_gen_hndl_t fl_gen_hndl);

/**
 * fl_gen_configuration() algorithm configuration API
 *
 * @param fl_gen_hndl algorithm handle
 * @param fl_gen_cfg  algorithm configuration settings
 *
 * @return 0 - success (!0) on error
 */
int
fl_gen_configuration(fl_gen_hndl_t fl_gen_hndl,
                            fl_gen_config_t* fl_gen_cfg);

/**
 * fl_gen_process() algorithm fl_gen_process API
 * calculates flash sequence based on input settings
 *
 * @param fl_gen_hndl algorithm handle
 * @param fl_gen_input  algorithm input settings
 * @param fl_gen_out    algorithm output
 *
 * @return 0 - success (!0) on error
 */
int
fl_gen_process(fl_gen_hndl_t fl_gen_hndl,
                      fl_gen_input_t* fl_gen_input,
                      fl_gen_output_t* fl_gen_out);

#endif /* FLASH_GENERATOR_H_ */
