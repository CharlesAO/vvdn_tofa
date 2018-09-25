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
* @file sg_ae_smooth_types.h
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

#ifndef  __SG_INTERFACE_AE_TYPES_SMOOTH_H__
#define  __SG_INTERFACE_AE_TYPES_SMOOTH_H__

#include <osal/osal_stdtypes.h>
#include <dtp/dtp_server_defs.h>

typedef enum {
    SG_AE_SMOOTH_MODE_FAST,                  // Capture
    SG_AE_SMOOTH_MODE_LOW,                   // User case
    SG_AE_SMOOTH_MODE_MEDIUM,                // Preview
    SG_AE_SMOOTH_MODE_HIGH,                  // Video mode
} sg_ae_smooth_mode_t;

/**
 * Auto Exposure stabilization configuration
 * *
 */
typedef struct {
    dtpdb_static_common_t * dtp_s_common;
    sg_ae_smooth_mode_t mode;                  // Stabilization mode
} sg_ae_smooth_config_t;

/**
 * Auto Exposure stabilization input data
 *
 */
typedef struct {
    dtpdb_dynamic_common_t *dtp_d_common;
    uint32 mode;                            // Calculation mode
    uint32 texp;                            // Total exposure
    uint32 ae_change_ratio;                 // Required change ratio for algorithm U8Q8
} sg_ae_smooth_input_t;


typedef struct
{
    uint32 texp;                            // Total exposure
    uint32 ae_change_ratio;                 // Required change ratio for algorithm U8Q8
} ae_smooth_output_t;

#endif // __SG_INTERFACE_AE_TYPES_SMOOTH_H__

