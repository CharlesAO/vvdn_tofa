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
* @file hat_luma_nf.h
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

#ifndef __HAL_TYPES_LUMA_NF_H__
#define __HAL_TYPES_LUMA_NF_H__

#include <hal/hat_types.h>

typedef struct  {
    hat_strength_t global_strength;               // float [0.0 .. 1.0] global strength
    hat_strength_t bright_tone_strength;          // float [0.0 .. 1.0] bright tones filtering
    hat_strength_t mid_tone_strength;             // float [0.0 .. 1.0] mid tones filtering
    hat_strength_t dark_tone_strength;            // float [0.0 .. 1.0] dark tones filtering
    hat_strength_t radial_strength;               // float [0.0 .. 1.0] strength in radial direction
    hat_strength_t add_noise_strength;			  // float [0.0 .. 1.0] additional noise strength
}hat_luma_nf_t;

#endif // __HAL_TYPES_LUMA_NF_H__

