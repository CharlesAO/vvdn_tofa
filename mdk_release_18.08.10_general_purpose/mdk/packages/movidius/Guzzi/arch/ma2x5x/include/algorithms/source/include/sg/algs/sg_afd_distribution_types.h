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
* @file sg_afd_distribution_types.h
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

#ifndef  __SG_AFD_DISTRIBUTION_TYPES_H__
#define  __SG_AFD_DISTRIBUTION_TYPES_H__

#include <hal/hat_types.h>
#include "sg_ae_limits_types.h"



// ---------------------------------------------------------------------------
// AFD Distribution

typedef enum {
    // AFD modes - these modes
    SG_AFD_DISTR_MODE_50Hz_GOOD,  // Without flicker
    SG_AFD_DISTR_MODE_50Hz_BAD,   // With heavy flicker
    SG_AFD_DISTR_MODE_60Hz_GOOD,
    SG_AFD_DISTR_MODE_60Hz_BAD,
}sg_afd_distr_modes_t;

typedef struct {
    dtpdb_static_common_t * dtp_s_common;
} sg_afd_distr_config_t;


typedef struct
{
    dtpdb_dynamic_common_t *dtp_d_common;
    sg_afd_distr_modes_t afd_distr_mode;
    uint32 current_exp;
    float current_gain;
    sg_ae_distr_range_limits_t limits;
} sg_afd_distr_calc_input_t;

// OUTPUT

typedef struct
{
    uint32      exp;
    float      gain;
} afd_distr_calc_output_t;

// END AFD Distribution---------------------------------------------------

#endif // __SG_AFD_DISTRIBUTION_TYPES_H__

