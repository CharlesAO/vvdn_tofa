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
* @file cam_cl_frame_req.h
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

#ifndef CAM_CL_FRAME_REQ_H_
#define CAM_CL_FRAME_REQ_H_

#include "osal/osal_stdtypes.h"
#include "osal/osal_time.h"
#include "osal/list_pool.h"

typedef enum {
// ACA Algos
    CL_ALGO_AE,    // auto exposure
    CL_ALGO_AWB,   // auto white balance
    CL_ALGO_AF,    // auto focus
    CL_ALGO_CAF,   // continuous auto focus
    CL_ALGO_AFD,   // auto flicker detect
    CL_ALGO_FMV,   // auto flicker detect
    MAX_CL_ALGO_ACA
} cam_cl_aca_algo_t;

typedef enum {
    // Image algos
    CL_ALGO_ISP = (MAX_CL_ALGO_ACA +1),
    CL_ALGO_ZSL_QUEUE,
    CL_ALGO_CAM_CAPTURE_PROCESS,
    CL_ALGO_MAX,
} cam_cl_img_algo_t;

typedef enum {
// ACA Algos
    CL_BUFF_STATS_AEWB,         // Auto exposure and white balance statistics
    CL_BUFF_STATS_AF,           // Auto focus statistics
    CL_BUFF_STATS_HISTOGRAM,
// Image Buffers
    CL_BUFF_ZSL_QUEUE,
    CL_BUFF_ISP,
    CL_BUFF_MAX,
} cam_cl_buff_t;

typedef enum {
// Algorithm processing control
    CL_FLAG_FORCE_PROCESS    = (1<<0) , // Forces algorithm processing for this frame
    CL_FLAG_WAIT_PROCESS_END = (1<<1) , // Wait for algorithm processing end. Usually used with CL_FLAG_FORCE_PROCESS flag
// Events - the event ID is algorithm specific
    CL_FLAG_FORCE_PRE_CALC_EVENT  = (1<<2) , // Forces algorithm to generate event before processing start
    CL_FLAG_FORCE_POST_CALC_EVENT = (1<<3) , // Forces algorithm to generate event after processing finish
} cam_cl_algo_flags_t;


typedef struct {
    void*  buff_hndl;    // IPIPE ZSL Buffer handle
//    uint32 source;
//    osal_timeval ts;
//    uint32 seq_no;
    uint32 flags;        // Processing flags - see icCaptureFlags
} cam_capture_request_t;

typedef struct {
    uint32 flags;        // Processing flags - see icIspGroup
    uint32 en_flags;     // Enable flags     - icIspEnable
    uint32 out_flags;    // Output control   - icPipeCtlFlags
} cam_isp_params_t;

typedef struct
{
    cam_cl_buff_t      buff_type;
    uint32             id;                // request id
    uint32             processing_flags;  // processing control flags

    union
    {
        cam_cl_aca_algo_t      aca_Algo;      // algorithm ID
        cam_cl_img_algo_t      img_Algo;      // algorithm ID
    };

    uint8               nMode;      // Operating mode - fast, normal...
    uint8               nSub_mode;  // Operating sub-mode - Locked...

    union alg_specific_t
    {
        uint32     nIteration; // In case of sequence CL specifies number (row) from list - AFD list
        cam_isp_params_t isp;  // CL_ALGO_ISP params
        cam_capture_request_t capt_request;  // CL_ALGO_CAM_CAPTURE_PROCESS params
    }alg_specific;
} cam_cl_algo_ctrl_t;

typedef struct
{
    struct osal_node   link;
    cam_cl_algo_ctrl_t  ctrl;
} cam_cl_algo_ctrl_list_t;

typedef list_pool_head_t cam_fr_buff_ctrl_list_t;

// algo status
// AFD List
// Bracketing List
// pre-flash Exposure, Gain
// flash Exposure, Gain
// Flash control

static inline void init_algo_ctrl_entry(
                                   cam_cl_algo_ctrl_t *alg,
                                   cam_cl_buff_t buff_type,
                                   uint32 algo_type,
                                   uint8  mode,
                                   uint8  sub_mode,
                                   uint32 flags)
{
    alg->aca_Algo  = algo_type;
    alg->nMode     = mode;
    alg->nSub_mode = sub_mode;
    alg->processing_flags = flags;
    alg->id = 0;
    alg->buff_type = buff_type;
}
#endif /* CAM_CL_FRAME_REQ_H_ */
