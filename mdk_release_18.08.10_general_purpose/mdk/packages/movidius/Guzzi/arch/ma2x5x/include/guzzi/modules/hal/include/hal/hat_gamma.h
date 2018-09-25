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
* @file hat_gamma.h
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

#ifndef __HAL_TYPES_GAMMA_H__
#define __HAL_TYPES_GAMMA_H__

#include <hal/hat_types.h>

#define HAT_GAMMA_LUT_SIZE      128   //

/* Lookup table for gamma */
typedef struct  {
    float i;    // Input value of pixel - values 0.0f - 1.0f map over input range
    float o;    // Output value of pixel - values 0.0f - 1.0f map over output range
}hat_gamma_point_t;

typedef struct  {
    uint32 tbl_size; 			// Count of available points
    hat_gamma_point_t *tbl;		// Should use BM_Buffer
}hat_gamma_color_t;

typedef struct  {
    hat_gamma_color_t r;
    hat_gamma_color_t g;
    hat_gamma_color_t b;
}hat_gamma_t;

#endif // __HAL_TYPES_GAMMA_H__

