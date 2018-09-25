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
* @file hat_cm_lens_prvt.h
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

#ifndef __HAT_CM_LENS_PRVT_H__
#define __HAT_CM_LENS_PRVT_H__

#include "osal/osal_mutex.h"
#include <func_thread/include/func_thread.h>
#include <hal/hal_camera_module/hat_cm_lens.h>
#include <hal/hal_camera_module/hat_cm_socket.h>
#include <hal/modules/hal_camera_module/lenses/lens_phys.h>

typedef struct {
    osal_mutex                  *abort_lock;
    int                         abort;
    osal_mutex                  *move_list_lock;
    hat_cm_lens_move_list_t     *move_list;
    void                        *user_data;
    hat_cm_lens_move_cb_t  move_done_cb;
    osal_mutex                  *curr_pos_lock;
    float                       curr_pos;
    float                       curr_pos_to_client;

    hat_cm_socket_handle_t      socket_hndl;
    hat_cm_ph_lens_handle_t     ph_lens_hndl;
} hat_cm_lens_thread_data_t;

typedef struct {
    HAT_CM_LENS_STATE_T       state;
    hat_cm_lens_dyn_props_t   dyn_props;
    HAT_CM_LENS_POWER_MODE_T  pwr_mode;
    func_thread_t            *thread;
    hat_cm_lens_thread_data_t thread_data;
    hat_cm_ph_lens_handle_t   ph_lens_hndl;
    const hat_lens_features_t *lens_features;
    hat_cm_socket_handle_t    socket_hndl;
    hat_cm_lens_move_list_t   lens_move_list;
} hat_cm_lens_prms_t;

typedef struct {
    hat_cm_lens_cr_prms_t cr_prms;
    hat_cm_lens_prms_t    prms;
} hat_cm_lens_prvt_data_t;

#endif // __HAT_CM_LENS_PRVT_H__
