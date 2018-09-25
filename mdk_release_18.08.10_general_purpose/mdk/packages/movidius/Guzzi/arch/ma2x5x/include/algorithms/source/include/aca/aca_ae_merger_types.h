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
* @file aca_ae_merger_types.h
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

#ifndef  __ACA_INTERFACE_AE_MERGER_TYPES_H__
#define  __ACA_INTERFACE_AE_MERGER_TYPES_H__

#include <osal/osal_stdtypes.h>
#include <osal/osal_time.h>
#include "dtp/dtp_server_defs.h"
#include "aca_ae_stab_types.h"

// ---------------------------------------------------------------------------
// AE_MERGER

typedef struct
{
    dtpdb_static_common_t * dtp_s_common;
    uint32                  camera_id;
    uint32                  weight;
} aca_ae_merger_config_t;


typedef struct
{
    int valid;
    uint32            weight;
    ae_stab_output_t  v;
} per_camera_ae_merger_data_t;

typedef struct
{
    uint32 texp;             
    uint32 ae_change_ratio;
    float camera_ratio; // ratio between two "near" camera's total exposures
    uint32 manual_weight; //set as input parameter weight - manual weight
    uint32 calc_weight; // weight will be calculated based on ae_change_ratio
} camera_data_t;

typedef struct {
    uint32 calc_total_exp;
    int     num_entries;
    camera_data_t cameras_data[6];
} aca_ae_merger_dbg_info_t;


// INPUT
typedef struct {
    int                             num_entries;
    per_camera_ae_merger_data_t     *p_in_data;
} aca_ae_merger_calc_input_t;

// OUTPUT
typedef struct
{
    ae_stab_output_t       out_data;
    aca_ae_merger_dbg_info_t  dbg_info;
} aca_ae_merger_calc_output_t;

// END AE_MERGER --------------------------------------------------------------------

#endif // __ACA_INTERFACE_AE_MERGER_TYPES_H__

