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
* @file sg_lens.h
*
* @author ( MM Solutions AD )
*
*/
/* -----------------------------------------------------------------------------
*!
*! Revision History
*! ===================================
*! 16-Dec-2014 : Author ( MM Solutions AD )
*! Created
* =========================================================================== */

#ifndef SG_LENS_H_
#define SG_LENS_H_

#include <guzzi_event/include/guzzi_event.h>
#include "aca_hllc.h"
#include "aca_af_types.h"

typedef struct sg_lens_thread_data sg_lens_thread_data_t;

typedef struct {
    dtpdb_dynamic_common_t  *dtp_d_common;
    focus_calc_output_t     focus_result;
} sg_lens_input_t;

typedef struct {
    dtpdb_static_common_t   *dtp_s_common;
    uint32                  frame_time;
    focus_range_t           focus_range;
} sg_lens_config_t;

typedef struct {
    guzzi_event_t       *evt_hndl;
    uint32              camera_id;
} sg_lens_create_params_t;

sg_lens_thread_data_t* sg_lens_create(sg_lens_create_params_t *params);

void sg_lens_destroy(sg_lens_thread_data_t *prv);
int sg_lens_start(sg_lens_thread_data_t *prv);
int sg_lens_stop(sg_lens_thread_data_t *prv);
int sg_lens_config(sg_lens_thread_data_t *prv, sg_lens_config_t *cfg);
int sg_lens_thr_process(sg_lens_thread_data_t *prv, sg_lens_input_t *in);
int sg_lens_process(sg_lens_thread_data_t *prv, sg_lens_input_t *in, uint32 *frm);
int sg_lens_init(sg_lens_thread_data_t *prv);

#endif /* SG_LENS_H_ */
