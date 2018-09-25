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
* @file hai_cm_socket.h
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

#ifndef __HAI_CM_SOCKET_H__
#define __HAI_CM_SOCKET_H__

#include <hal/hal_camera_module/hat_cm_socket.h>

/* ========================================================================== */
/**
* hai_cm_socket_create()
*/
/* ========================================================================== */
int hai_cm_socket_create(hat_cm_socket_handle_t hndl,
    hat_cm_socket_cr_prms_t *cr_prms);

int execute_cmd_entry(hat_cm_socket_handle_t socket_hndl,
    hat_cm_socket_command_entry_t* cmd_entry,
    hat_cm_socket_cmd_entry_result_t* res);

int execute_cmd_strip(hat_cm_socket_handle_t socket_hndl,
    const hat_cm_socket_command_entry_t* cmd_strip,
    hat_cm_socket_cmd_strip_result_t* res);

#endif // __HAI_CM_SOCKET_H__
