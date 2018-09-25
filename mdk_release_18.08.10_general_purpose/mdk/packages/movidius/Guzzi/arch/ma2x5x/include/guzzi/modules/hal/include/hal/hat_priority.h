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
* @file hat_priority.h
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

#ifndef __HAL_TYPES_PRIORITY_H__
#define __HAL_TYPES_PRIORITY_H__

#include <hal/hat_types.h>

#define HAT_REGION_PRIORITY_MAX (35)  // Number of max region priorities

/* Structure for region priority */
typedef struct  {
//    uint8 update;               // Update counter: 0 not changed, !0 - update
//    uint8 enable;
    /* The bounding box of face (sizes converted in manager from pixels to relative) */
    hat_rect_float_t reg_box[HAT_REGION_PRIORITY_MAX];

    /* priority for each region - as a per-pixel weight, values from 1 to priority_max*/
    uint32 priority[HAT_REGION_PRIORITY_MAX];

    uint32 priority_max;       // The highest priority value
    int32 reg_num;             // Number of faces detected
}hat_reg_pri_t;

#endif // __HAL_PRIORITY_H__

