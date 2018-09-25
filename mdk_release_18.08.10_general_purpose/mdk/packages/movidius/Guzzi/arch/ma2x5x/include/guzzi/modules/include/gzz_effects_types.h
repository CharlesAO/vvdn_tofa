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
* @file gzz_effects_types.h
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

#ifndef GZZ_EFFECTS_TYPES_H_
#define GZZ_EFFECTS_TYPES_H_

typedef enum {
    CAM_EFFECT_NONE,
    CAM_EFFECT_SEPIA,
    CAM_EFFECT_NEGATIVE,
    CAM_EFFECT_GRAYSCALE,
    CAM_EFFECT_NATURAL,
    CAM_EFFECT_VIVID,
    CAM_EFFECT_COLORSWAP,
    CAM_EFFECT_SOLARIZE,
    CAM_EFFECT_OUT_OF_FOCUS,
    CAM_EFFECT_BLACKWHITE,
    CAM_EFFECT_WHITE_BOARD,
    CAM_EFFECT_BLACK_BOARD,
    CAM_EFFECT_AQUA,
    CAM_EFFECT_POSTERIZE,
    CAM_EFFECT_RED_TINT,
    CAM_EFFECT_GREEN_TINT,
    CAM_EFFECT_BLUE_TINT,
    MAX_CAM_EFFECTS_TYPE
} gzz_effects_t;

#endif /* GZZ_EFFECTS_TYPES_H_ */
