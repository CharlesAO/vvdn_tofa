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
* @file hat_capture.h
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

#ifndef HAT_CAPTURE_H_
#define HAT_CAPTURE_H_

#include <dtp/dtp_server_defs.h>

typedef void* hat_capture_hndl_t;

typedef enum {
    HAT_CAPTURE_MODE_YUV        = 1 << 0,
    HAT_CAPTURE_MODE_RAW        = 1 << 1,
    HAT_CAPTURE_MODE_CLEAR_RAW  = 1 << 2,
    HAT_CAPTURE_MODE_RELATIVE   = 1 << 3,
} hat_capture_mode_t;

typedef struct
{
    hat_timestamp_t     ts;
    hat_capture_mode_t  mode;
} hat_capture_settings_t;

int hat_capture_trigger (hat_capture_hndl_t hndl,  hat_capture_settings_t *cap_cfg);

int hat_capture_destroy (hat_capture_hndl_t hndl);
int hat_capture_create (hat_capture_hndl_t *hndl);

#endif /* HAT_CAPTURE_H_ */

