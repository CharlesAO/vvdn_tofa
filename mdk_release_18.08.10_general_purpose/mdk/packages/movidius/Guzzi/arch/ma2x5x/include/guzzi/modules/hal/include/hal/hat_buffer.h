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
* @file hat_buffer.h
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

#ifndef __HAL_TYPE_BUFFER_H__
#define __HAL_TYPE_BUFFER_H__

#include <osal/osal_stdtypes.h>
#include <osal/osal_time.h>

#include <hal/hat_types.h>
#include <hal/hat_img_fmt.h>

struct hat_queue;
struct hat_buf;

typedef enum  {
    HAT_BUF_STATE_IDLE = 0,
    HAT_BUF_STATE_DONE,
    HAT_BUF_STATE_PREPARED,
    HAT_BUF_STATE_QUEUED,
    HAT_BUF_STATE_ACTIVE,
    HAT_BUF_STATE_FLUSHED,
    HAT_BUF_STATE_ERROR,
}hat_buf_state_t;

typedef struct  {
    uint32 index;

    /* Buffer state */
    hat_buf_state_t state;

    /* Image format */
    hat_img_fmt_t image;

    /* Timestamp of buffer */
    struct timeval time_stamp;
}hat_buf_info_t;

#endif // __HAL_TYPE_BUFFER_H__

